library ieee;
use ieee.std_logic_1164.all;

entity trollbook is
	port(
		a : in std_logic_vector(31 downto 0);
		d : inout std_logic_vector(31 downto 0);
		
		cpu_tt : in std_logic_vector(1 downto 0);
		cpu_tm : in std_logic_vector(2 downto 0);
		cpu_siz : in std_logic_vector(1 downto 0);
		cpu_rw : in std_logic;
		cpu_ts : in std_logic;
		cpu_tip : in std_logic;
		cpu_ta : out std_logic;
		cpu_tea : out std_logic;
		cpu_tbi :  out std_logic;
		cpu_ipl : out std_logic_vector(2 downto 0);
		cpu_clk : out std_logic;
		cpu_lfo : out std_logic;
		cpu_scd : in std_logic;
		cpu_rsti : out std_logic;
		cpu_rsto : in std_logic;
		
		ram_a : out std_logic_vector(12 downto 0);
		ram_b : out std_logic_vector(1 downto 0);
		ram_cas : out std_logic;
		ram_ras : out std_logic;
		ram_we : out std_logic;
		ram_ldqm : out std_logic_vector(1 downto 0);
		ram_udqm : out std_logic_vector(1 downto 0);
		ram_cs : out std_logic_vector(1 downto 0);
		ram_cke : out std_logic;
		
		ll_a : out std_logic_vector(18 downto 1);
		ll_d : inout std_logic_vector(15 downto 0);
		ll_ce : out std_logic;
		ll_we : out std_logic;
		ll_lb : out std_logic;
		ll_ub : out std_logic;
		ll_oe : out std_logic;
		
		vga_r : out std_logic_vector(4 downto 0);
		vga_g : out std_logic_vector(5 downto 0);
		vga_b : out std_logic_vector(4 downto 0);
		vga_hsync : out std_logic;
		vga_vsync : out std_logic;
		vga_den : out std_logic;
		vga_pwr : out std_logic;
		vga_pwm : out std_logic;
		
		snd_mosi : out std_logic;
		snd_clk : out std_logic;
		snd_ss : out std_logic;
		snd_sync : out std_logic;
		
		spi_miso : in std_logic;
		spi_mosi : out std_logic;
		spi_clk : out std_logic;
		spi_ss : out std_logic_vector(2 downto 0);
		
		uart_rx : in std_logic;
		uart_tx : out std_logic;
		
		ext_int : in std_logic_vector(3 downto 0);
		
		clk33 : in std_logic;
		clk12 : in std_logic;
		
		pwron_reset : in std_logic
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
		port map(reset => '0', clk => clk33, r => vga_r, g => vga_g, b => vga_b, hsync => vga_hsync, vsync => vga_vsync, den => vga_den);
	
	vga_pwr <= '0';
	vga_pwm <= '1';
	
end arch;
