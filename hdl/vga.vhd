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
		frame_end : integer
	);
	port(
		reset : in std_logic;
		clk : in std_logic;
		r : out std_logic_vector(depth_r - 1 downto 0);
		g : out std_logic_vector(depth_g - 1 downto 0);
		b : out std_logic_vector(depth_b - 1 downto 0);
		hsync : out std_logic := '1';
		vsync : out std_logic := '1';
		
		den : out std_logic := '1'
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
begin
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
		else
			if rising_edge(clk) then
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
	
	g1: for i in depth_r - 1 downto 0 generate
		r(i) <= '0';
	end generate;
	
	g <=  std_logic_vector(to_unsigned(pixel_counter, g'length));
	b <=  std_logic_vector(to_unsigned(line_counter, g'length));
	
	den_internal <= hvisible and vvisible;
	den <= den_internal;
	--g <= (others => '1') when (pixel_counter mod 2) = 0 and den_internal = '1' else (others => '0');
	--b <= (others => '1') when (line_counter mod 2) = 0 and den_internal = '1' else (others => '0');
end arch;
