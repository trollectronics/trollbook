library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use work.wor_logic.all;

entity vga is
	generic(
		peripheral_id : integer range 0 to 15;
		
		depth_r : integer;
		depth_g : integer;
		depth_b : integer;
		
		line_front_porch : integer;
		line_hsync : integer;
		line_back_porch : integer;
		line_end : integer;
		
		frame_front_porch : integer;
		frame_vsync : integer;
		frame_back_porch : integer;
		frame_end : integer;
		
		ll_a_start : integer;
		ll_a_end : integer;
		ll_a_length : integer
	);
	port(
		reset : in std_logic;
		clk : in std_logic;
		r : out std_logic_vector(depth_r - 1 downto 0);
		g : out std_logic_vector(depth_g - 1 downto 0);
		b : out std_logic_vector(depth_b - 1 downto 0);
		hsync : out std_logic := '1';
		vsync : out std_logic := '1';
		
		den : out std_logic := '1';
		
		power_on : out std_logic;
		backlight_pwm : out std_logic;
		
		ll_a : out std_logic_vector(ll_a_length - 1 downto 0);
		ll_d : in std_logic_vector(15 downto 0);
		ll_ce : out std_logic;
		
		chipset_a : in std_logic_vector(7 downto 0);
		bus_d : in std_logic_vector(31 downto 0);
		bus_q : out std_logic_vector(31 downto 0);
		bus_rw : in std_logic;
		bus_siz : in std_logic_vector(1 downto 0);
		chipset_ce : in std_logic_vector(15 downto 0);
		chipset_ack : out wor_logic_vector(15 downto 0);
		chipset_nack : out wor_logic_vector(15 downto 0);
		chipset_int : out std_logic_vector(15 downto 0)
	);
end vga;

architecture arch of vga is
	constant CURSOR_W : integer := 8;
	constant CURSOR_H : integer := 16;
	
	type state_type is (visible, front, sync, back);
	
	signal hstate, hstate_next : state_type;
	signal vstate, vstate_next : state_type;
	
	signal new_line : std_logic;
	
	signal pixel_counter : integer range 0 to line_end;
	signal pixel_counter_next : integer range 0 to line_end + 1;
	signal line_counter : integer range 0 to frame_end;
	signal line_counter_next : integer range 0 to frame_end + 2;
	
	signal hvisible, hvisible_next : std_logic;
	signal vvisible, vvisible_next : std_logic;
	signal hsync_next : std_logic;
	signal vsync_next : std_logic;
	
	signal hsync_internal : std_logic;
	signal vsync_internal : std_logic;
	signal vsync_old : std_logic;
	signal den_internal : std_logic_vector(1 downto 0);
	signal den_out : std_logic;
	
	signal rgb : std_logic_vector((depth_r + depth_g + depth_b) - 1 downto 0);
	signal palette_a : std_logic_vector(7 downto 0);
	
	signal second_pixel : std_logic_vector(15 downto 0);
	signal second_pixel_next : std_logic_vector(15 downto 0);
	
	signal visible_counter_save : integer;
	signal visible_counter_save_next : integer;
	signal visible_counter : integer ;--range 0 to ll_a_end - ll_a_start;
	signal visible_counter_next : integer;-- range 0 to ll_a_end - ll_a_start;
	
	signal ll_a_next : std_logic_vector(ll_a_length - 1 downto 0);
	signal ll_ce_internal : std_logic;
	signal ll_ce_next : std_logic;
	
	signal palette_clk : std_logic;
	signal palette_wr_a : std_logic_vector(7 downto 0);
	signal palette_d : std_logic_vector(15 downto 0);
	signal palette_write : std_logic;
	
	signal cursor_x : std_logic_vector(9 downto 0);
	signal cursor_y : std_logic_vector(8 downto 0);
	
	signal cursor_x_count : unsigned(2 downto 0);
	signal cursor_y_count : unsigned(3 downto 0);
	
	signal cursor_a : std_logic_vector(6 downto 0);
	signal cursor_q : std_logic_vector(1 downto 0);
	
	type cursor_state is (idle, drawing);
	
	signal cursor_x_state : cursor_state;
	signal cursor_y_state : cursor_state;
	
	signal interrupt : std_logic;
	
	
	signal enabled : std_logic;
	signal mode, mode_next, mode_staging : std_logic;
	signal current_buffer, current_buffer_staging : std_logic;
	
	signal backlight_compare : std_logic_vector(7 downto 0);
	signal backlight_counter : unsigned(16 downto 0);
begin
	u_palette: entity work.palette port map(
		data => palette_d,
		address => palette_a,
		inclock => palette_clk,
		we => palette_write,
		q => rgb
	);
	
	u_cursor: entity work.cursor port map(
		data => (others => '0'),
		address => cursor_a,
		inclock=> clk,
		we => '0',
		q => cursor_q
	);
	
	palette_clk <= clk;
	
	process(reset, clk) begin
		if reset = '1' then
			backlight_counter <= (others => '0');
			backlight_pwm <= '0';
		elsif falling_edge(clk) then
			
			if std_logic_vector(backlight_counter(15 downto 8)) = backlight_compare then
				backlight_pwm <= '1';
			elsif std_logic_vector(backlight_counter(15 downto 8)) = x"00" then
				backlight_pwm <= '0';
			end if;
			
			backlight_counter <= backlight_counter + 1;
		end if;
	end process;
	
	hlogic: process(hstate, pixel_counter, hsync_internal, hvisible) begin
		hstate_next <= hstate;
		pixel_counter_next <= pixel_counter + 1;
		new_line <= '0';
		hsync_next <= hsync_internal;
		hvisible_next <= hvisible;
		
		case hstate is
			when visible =>
				if pixel_counter = line_front_porch - 1 then
					hstate_next <= front;
					hvisible_next <= '0';
				end if;
			when front =>
				if pixel_counter = line_hsync - 1 then
					hstate_next <= sync;
					hsync_next <= '0';
					new_line <= '1';
				end if;
			when sync =>
				if pixel_counter = line_back_porch - 1 then
					hstate_next <= back;
					hsync_next <= '1';
				end if;
			when back =>
				if pixel_counter = line_end - 1 then
					hstate_next <= visible;
					pixel_counter_next <= 0;
					
					hvisible_next <= '1';
				end if;
		end case;
	end process;

	vlogic: process(vstate, line_counter, vsync_internal, vvisible, mode_staging, mode) begin
		vstate_next <= vstate;
		line_counter_next <= line_counter + 1;
		vsync_next <= vsync_internal;
		vvisible_next <= vvisible;
		mode_next <= mode;
		
		case vstate is
			when visible =>
				if line_counter = frame_front_porch - 1 then
					vstate_next <= front;
					vvisible_next <= '0';
				end if;
			when front =>
				vvisible_next <= '0';
				if line_counter = frame_vsync - 1 then
					vstate_next <= sync;
					vsync_next <= '0';
				end if;
			when sync =>
				if line_counter = frame_back_porch - 1 then
					vstate_next <= back;
					vsync_next <= '1';
				end if;
			when back =>
				if line_counter = frame_end - 1 then
					vstate_next <= visible;
					line_counter_next <= 0;
					vvisible_next <= '1';
					mode_next <= mode_staging;
				end if;
		end case;
	end process;
	
	hregs: process(clk, reset, new_line) begin
		if reset = '1' then
			hstate <= back;
			vstate <= back;
			vsync_internal <= '1';
			hsync_internal <= '1';
			hvisible <= '0';
			vvisible <= '0';
			pixel_counter <= line_end - 10;
			line_counter <= frame_end - 1;
			visible_counter <= 0;
			visible_counter_save <= 0;
			ll_a <= (others => '0');
			ll_ce_internal <= '0';
			second_pixel <= (others => '0');
			den_internal <= "00";
			den_out <= '0';
			
			current_buffer <= '0';
			mode <= '0';
		else
			if falling_edge(clk) then
				
			elsif rising_edge(clk) then
				second_pixel <= second_pixel_next;
				--set up llram adress
				
				visible_counter <= visible_counter_next;
				visible_counter_save <= visible_counter_save_next;
				ll_a <= ll_a_next;
				ll_ce_internal <= ll_ce_next;
				
				
				hstate <= hstate_next;
				pixel_counter <= pixel_counter_next;
				hvisible <= hvisible_next;
				hsync_internal <= hsync_next;
				
				den_internal(1) <= hvisible and vvisible;
				den_internal(0) <= den_internal(1);
				den_out <= den_internal(0);
				
				if new_line = '1' then
					vstate <= vstate_next;
					line_counter <= line_counter_next;
					vvisible <= vvisible_next;
					vsync_internal <= vsync_next;
					current_buffer <= current_buffer_staging;
					mode <= mode_next;
				end if;
			end if;
		end if;
	end process;
	
	process(visible_counter, vstate, second_pixel, hstate, pixel_counter, mode, visible_counter_save, current_buffer, line_counter) begin
		ll_a_next <= (others => '1');
		ll_ce_next <= '0';
		visible_counter_save_next <= visible_counter_save;
		
		if mode = '1' then
			-- Half mode
			if pixel_counter = 0 then
				visible_counter_save_next <= visible_counter;
			end if;
			
			if hstate = visible and vstate = visible and pixel_counter mod 4 = 0 then
				visible_counter_next <= visible_counter + 1;
				ll_a_next <= std_logic_vector(to_unsigned(ll_a_start + visible_counter, ll_a_length)) or ( "0" & current_buffer & x"0000");
				ll_ce_next <= '1';
				
			elsif hstate = sync and (line_counter mod 2 = 1) then
				visible_counter_next <= visible_counter_save;
			elsif vstate = sync then
				visible_counter_next <= 0;
			else
				visible_counter_next <= visible_counter;
			end if;
		else
			-- Normal mode
			if hstate = visible and vstate = visible and pixel_counter mod 2 = 0 then
				visible_counter_next <= visible_counter + 1;
				ll_a_next <= std_logic_vector(to_unsigned(ll_a_start + visible_counter, ll_a_length));
				ll_ce_next <= '1';
				
			elsif vstate = sync then
				visible_counter_next <= 0;
			else
				visible_counter_next <= visible_counter;
			end if;
		end if;
	end process;
	
	process(clk, reset) begin
		if reset = '1' then
			vsync_old <= '1';
			interrupt <= '0';
		elsif rising_edge(clk) then
			interrupt <= '0';
			if vsync_old = '1' and vsync_internal = '0' then
				interrupt <= '1';
			end if;
			
			vsync_old <= vsync_internal;
		end if;
	end process;
	
	process(clk, reset) begin
		if reset = '1' then
			cursor_x_state <= idle;
			cursor_y_state <= idle;
			cursor_x_count <= (others => '0');
			cursor_y_count <= (others => '0');
			cursor_a <= (others => '0');
		elsif rising_edge(clk) then
			if std_logic_vector(to_unsigned(pixel_counter_next, cursor_x'length)) = cursor_x then
				cursor_x_state <= drawing;
			end if;
			
			if std_logic_vector(to_unsigned(line_counter, cursor_y'length)) = cursor_y then
				cursor_y_state <= drawing;
			end if;
			
			if cursor_x_state = drawing then
				cursor_x_count <= cursor_x_count + 1;
				
				if cursor_x_count = (CURSOR_W - 1) then
					cursor_x_state <= idle;
				end if;
			end if;
			
			if cursor_y_state = drawing and pixel_counter = line_hsync - 1 then --line len -1 ?
				cursor_y_count <= cursor_y_count + 1;
				
				if cursor_y_count = (CURSOR_H - 1) then
					cursor_y_state <= idle;
				end if;
			end if;
			
			if cursor_x_state = drawing and cursor_y_state = drawing then
				cursor_a <= std_logic_vector(unsigned(cursor_a) + 1);
			end if;
		end if;
	end process;
	
	ll_ce <= ll_ce_internal;
	
	process(pixel_counter, second_pixel, ll_d, mode, palette_write, palette_wr_a) begin
		second_pixel_next <= second_pixel;
		palette_a <= (others => '1');
		
		if mode = '1' then
			--Half mode
			if pixel_counter mod 4 = 2 then
				palette_a <= ll_d(15 downto 8);
				second_pixel_next <= ll_d;
			elsif pixel_counter mod 4 = 3 then
				palette_a <= second_pixel(15 downto 8);
			elsif pixel_counter mod 4 = 0 then
				palette_a <= second_pixel(7 downto 0);
			elsif pixel_counter mod 4 = 1 then
				palette_a <= second_pixel(7 downto 0);
			end if;
		else
			--Normal mode
			if pixel_counter mod 2 = 0 then
				palette_a <= ll_d(15 downto 8);
				second_pixel_next <= ll_d;
			else
				palette_a <= second_pixel(7 downto 0);
				second_pixel_next <= second_pixel;
			end if;
		end if;

		if palette_write = '1' then
			palette_a <= palette_wr_a;
		end if;
	end process;
	
	process(den_out, rgb, cursor_q, cursor_x_state, cursor_y_state) begin
		b <= rgb(depth_b - 1 downto 0);
		g <= rgb(depth_b + depth_g - 1 downto depth_b);
		r <= rgb(rgb'length - 1 downto depth_b + depth_g);
		
		if cursor_x_state = drawing and cursor_y_state = drawing then
			if cursor_q(1) = '1' then
				b <= (others => cursor_q(0));
				g <= (others => cursor_q(0));
				r <= (others => cursor_q(0));
			end if;
		end if;
		
		if den_out = '0' then
			b <= (others => '0');
			g <= (others => '0');
			r <= (others => '0');
		end if;
	end process;
	
	process(reset, clk) is
		variable check : std_logic_vector(1 downto 0);
	begin
		if reset = '1' then
			enabled <= '1';
			mode_staging <= '0';
			current_buffer_staging <= '1';
			palette_write <= '0';
			palette_wr_a <= (others => '0');
			palette_d <= (others => '0');
			
			backlight_compare <= x"00";
			
			cursor_x <= std_logic_vector(to_unsigned(58, cursor_x'length));
			cursor_y <= std_logic_vector(to_unsigned(46, cursor_y'length));
		elsif falling_edge(clk) then
			check := chipset_ce(peripheral_id) & bus_rw;
			palette_write <= '0';
			
			case check is
				when "11" =>
					case chipset_a(5 downto 2) is
						when "0000" =>
							enabled <= bus_d(0);
							mode_staging <= bus_d(1);
							current_buffer_staging <= bus_d(2);
						when "0001" =>
							backlight_compare <= bus_d(7 downto 0);

						when "0100" =>
							palette_wr_a <= bus_d(7 downto 0);
						when "0101" =>
							palette_d <= bus_d(15 downto 0);
							palette_write <= '1';

						when "1010" =>
							cursor_x <= bus_d(9 downto 0);
						when "1011" =>
							cursor_y <= bus_d(8 downto 0);
						when others =>
						
					end case;
					
				when others =>
			end case;
			
		end if;
	end process;
	
	process(reset, clk) begin
		if reset = '1' then
			bus_q <= (others => 'Z');
		elsif falling_edge(clk) then
			if chipset_ce(peripheral_id) = '1' then
				case chipset_a(5 downto 2) is
					when "0000" =>
						bus_q <= x"0000000" & "0" & current_buffer & mode & enabled;
					when "0001" =>
						bus_q <= x"000000" & backlight_compare;

					when "0100" =>
						bus_q <= x"000000" & palette_wr_a;

					when "1010" =>
						bus_q <= x"0000" & "000000" & cursor_x;
					when "1011" =>
						bus_q <= x"0000" & "0000000" & cursor_y;
					when others =>
						bus_q <= (others => 'Z');
				end case;
			else
				bus_q <= (others => 'Z');
			end if;
		end if;
	end process;
	
	
	chipset_int <= (peripheral_id => interrupt, others => 'Z');
	chipset_ack <= (peripheral_id => '1', others => '0');
	chipset_nack <= (peripheral_id => '0', others => '0');
	
	den <= den_out;
	
	power_on <= not enabled;
	
	hsync <= not hsync_internal;
	vsync <= vsync_internal;
end arch;
