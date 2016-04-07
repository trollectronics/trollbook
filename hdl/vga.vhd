library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity vga is
	generic(
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
		
		ll_a : out std_logic_vector(ll_a_length - 1 downto 0);
		ll_d : in std_logic_vector(15 downto 0);
		ll_ce : out std_logic
	);
end vga;

architecture arch of vga is
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
	signal den_internal : std_logic_vector(1 downto 0);
	
	signal rgb : std_logic_vector((depth_r + depth_g + depth_b) - 1 downto 0);
	signal palette_a : std_logic_vector(7 downto 0);
	
	signal second_pixel : std_logic_vector(7 downto 0);
	signal second_pixel_next : std_logic_vector(7 downto 0);
	
	signal visible_counter : integer ;--range 0 to ll_a_end - ll_a_start;
	signal visible_counter_next : integer;-- range 0 to ll_a_end - ll_a_start;
	
	signal ll_a_next : std_logic_vector(ll_a_length - 1 downto 0);
	signal ll_ce_internal : std_logic;
	signal ll_ce_next : std_logic;
	
	signal palette_clk : std_logic;
begin
	u_palette: entity work.palette port map(
		data => (others => '0'),
		address => palette_a,
		inclock => palette_clk,
		we => '0',
		q => rgb
	);
	
	palette_clk <= not clk;
	
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
					new_line <= '1';
					hvisible_next <= '1';
				end if;
		end case;
	end process;

	vlogic: process(vstate, line_counter, vsync_internal, vvisible) begin
		vstate_next <= vstate;
		line_counter_next <= line_counter + 1;
		vsync_next <= vsync_internal;
		vvisible_next <= vvisible;
		
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
			ll_a <= (others => '0');
			ll_ce_internal <= '0';
			second_pixel <= (others => '0');
			den_internal <= "00";
			den <= '0';
		else
			if rising_edge(clk) then
				
			elsif falling_edge(clk) then
				second_pixel <= second_pixel_next;
				--set up llram adress
				
				visible_counter <= visible_counter_next;
				ll_a <= ll_a_next;
				ll_ce_internal <= ll_ce_next;
				
				
				hstate <= hstate_next;
				pixel_counter <= pixel_counter_next;
				hvisible <= hvisible_next;
				hsync_internal <= hsync_next;
				
				den_internal(1) <= hvisible and vvisible;
				den_internal(0) <= den_internal(1);
				den <= den_internal(0);
				
				if new_line = '1' then
					vstate <= vstate_next;
					line_counter <= line_counter_next;
					vvisible <= vvisible_next;
					vsync_internal <= vsync_next;
				end if;
			end if;
		end if;
	end process;
	
	process(visible_counter, vstate, second_pixel, hstate, pixel_counter) begin
		ll_a_next <= (others => '1');
		ll_ce_next <= '0';
		
		if hstate = visible and vstate = visible and pixel_counter mod 2 = 0 then
			visible_counter_next <= visible_counter + 1;
			ll_a_next <= std_logic_vector(to_unsigned(ll_a_start + visible_counter, ll_a_length));
			ll_ce_next <= '1';
			
		elsif vstate = sync then
			visible_counter_next <= 0;
		else
			visible_counter_next <= visible_counter;
		end if;
	end process;
	
	--g <=  std_logic_vector(to_unsigned(pixel_counter, g'length));
	--b <=  std_logic_vector(to_unsigned(line_counter, b'length));
	
	ll_ce <= ll_ce_internal;
	
	palette_a <= ll_d(15 downto 8) when pixel_counter mod 2 = 0 else second_pixel;
	--palette_a <= x"FE" when ll_ce_internal = '1' else x"FF";
	second_pixel_next <= ll_d(7 downto 0) when pixel_counter mod 2 = 0 else second_pixel;
	
	r <= rgb(depth_r - 1 downto 0);
	g <= rgb(depth_r + depth_g - 1 downto depth_r);
	b <= rgb(rgb'length - 1 downto depth_r + depth_g);
	
	hsync <= hsync_internal;
	vsync <= vsync_internal;
	
	--g <= (others => '1') when (pixel_counter mod 2) = 0 and den_internal = '1' else (others => '0');
	--b <= (others => '1') when (line_counter mod 2) = 0 and den_internal = '1' else (others => '0');
	
end arch;
