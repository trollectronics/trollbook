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
	
	signal pixel_counter : integer range 0 to line_end := 0;
	signal pixel_counter_next : integer range 0 to line_end + 1 := 0;
	signal line_counter : integer range 0 to frame_end := 0;
	signal line_counter_next : integer range 0 to frame_end + 2 := 0;
	
	signal hvisible, hvisible_next : std_logic := '0';
	signal vvisible, vvisible_next : std_logic := '0';
	signal hsync_next : std_logic := '1';
	signal vsync_next : std_logic := '1';
	
	signal den_internal : std_logic := '1';
	
	signal rgb : std_logic_vector((depth_r + depth_g + depth_b) - 1 downto 0);
	signal palette_a : std_logic_vector(7 downto 0);
	
	signal second_pixel : std_logic_vector(7 downto 0);
	signal second_pixel_next : std_logic_vector(7 downto 0);
	
	signal visible_counter : integer range 0 to ll_a_end - ll_a_start;
	signal visible_counter_next : integer range 0 to ll_a_end - ll_a_start;
	
	signal palette_clk : std_logic;
	
	signal ll_a_next : std_logic_vector(ll_a_length - 1 downto 0);
	signal ll_ce_internal : std_logic;
	signal ll_ce_next : std_logic;
begin
	u_palette: entity work.palette port map(
		address => palette_a,
		data => (others => '1'),
		inclock => palette_clk,
		outclock => palette_clk,
		we => '0',
		q => rgb
	);
	
	hlogic: process(hstate, pixel_counter) begin
		hstate_next <= hstate;
		pixel_counter_next <= pixel_counter + 1;
		
		case hstate is
			when visible =>
				hsync_next <= '1';
				hvisible_next <= '1';
				if pixel_counter = line_front_porch then
					hstate_next <= front;
				end if;
			when front =>
				hsync_next <= '1';
				hvisible_next <= '0';
				if pixel_counter = line_hsync then
					hstate_next <= sync;
				end if;
			when sync =>
				hsync_next <= '0';
				hvisible_next <= '0';
				if pixel_counter = line_back_porch then
					hstate_next <= back;
				end if;
			when back =>
				hsync_next <= '1';
				hvisible_next <= '0';
				if pixel_counter = line_end then
					hstate_next <= visible;
					pixel_counter_next <= 0;
				end if;
		end case;
	end process;

	vlogic: process(vstate, line_counter) begin
		vstate_next <= vstate;
		line_counter_next <= line_counter + 1;
		
		case vstate is
			when visible =>
				vsync_next <= '1';
				vvisible_next <= '1';
				if line_counter = frame_front_porch then
					vstate_next <= front;
				end if;
			when front =>
				vsync_next <= '1';
				vvisible_next <= '0';
				if line_counter = frame_vsync then
					vstate_next <= sync;
				end if;
			when sync =>
				vsync_next <= '0';
				vvisible_next <= '0';
				if line_counter = frame_back_porch then
					vstate_next <= back;
				end if;
			when back =>
				vsync_next <= '1';
				vvisible_next <= '0';
				if line_counter = frame_end then
					vstate_next <= visible;
					line_counter_next <= 0;
				end if;
		end case;
	end process;
	
	hregs: process(clk, reset, hvisible) begin
		if reset = '1' then
			hstate <= visible;
			vstate <= visible;
			vsync <= '1';
			hsync <= '1';
			hvisible <= '0';
			vvisible <= '0';
			pixel_counter <= 0;
			visible_counter <= 0;
			ll_a <= (others => '0');
			ll_ce_internal <= '0';
			second_pixel <= (others => '0');
		else
			if rising_edge(clk) then
				--set up llram adress
				visible_counter <= visible_counter_next;
				ll_a <= ll_a_next;
				ll_ce_internal <= ll_ce_next;
			elsif falling_edge(clk) then
				second_pixel <= second_pixel_next;
				
				hstate <= hstate_next;
				pixel_counter <= pixel_counter_next;
				hvisible <= hvisible_next;
				hsync <= hsync_next;
			end if;
			
			if rising_edge(hvisible) then
				vstate <= vstate_next;
				line_counter <= line_counter_next;
				vvisible <= vvisible_next;
				vsync <= vsync_next;
			end if;
		end if;
	end process;
	
	process(visible_counter, den_internal, vstate, pixel_counter, second_pixel, hstate) begin
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
	
	den_internal <= hvisible and vvisible;
	den <= den_internal;
	
	ll_ce <= ll_ce_internal;
	
	palette_a <= ll_d(7 downto 0) when ll_ce_internal = '1' else second_pixel;
	second_pixel_next <= ll_d(15 downto 8);
	
	r <= rgb(depth_r - 1 downto 0);
	g <= rgb(depth_r + depth_g - 1 downto depth_r);
	b <= rgb(rgb'length - 1 downto depth_r + depth_g);
	
	palette_clk <= not clk;
	
	--g <= (others => '1') when (pixel_counter mod 2) = 0 and den_internal = '1' else (others => '0');
	--b <= (others => '1') when (line_counter mod 2) = 0 and den_internal = '1' else (others => '0');
	
end arch;
