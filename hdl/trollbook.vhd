library ieee;
use ieee.std_logic_1164.all;

entity trollbook is
	port(
		vga_r : out std_logic_vector(4 downto 0);
		vga_g : out std_logic_vector(5 downto 0);
		vga_b : out std_logic_vector(4 downto 0);
		vga_hsync : out std_logic;
		vga_vsync : out std_logic;
		vga_clk : out std_logic;
		vga_den : out std_logic;
		clk : in std_logic
	);
end entity;

architecture arch of trollbook is
	constant depth_r : integer := 5;
	constant depth_g : integer := 6;
	constant depth_b : integer := 5;
	
	
	component vga is
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
	end component;
begin
	u1: vga generic map(depth_r => depth_r, depth_g => depth_g, depth_b => depth_b,
		line_front_porch => 800, line_hsync => 800 + 40, line_back_porch => 800 + 40 + 48, line_end => 928,
		frame_front_porch => 480, frame_vsync => 480 + 13, frame_back_porch => 480 + 13 + 3, frame_end => 525)
		port map(reset => '0', clk => clk, r => vga_r, g => vga_g, b => vga_b, hsync => vga_hsync, vsync => vga_vsync, den => vga_den);
	
	vga_clk <= clk;
end arch;
