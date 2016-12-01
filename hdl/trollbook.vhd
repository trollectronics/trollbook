library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use work.wor_logic.all;

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
		
		ll_a : out std_logic_vector(17 downto 0);
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
	
	signal cpu_q : std_logic_vector(31 downto 0);
	signal cpu_oe : std_logic;
	signal internal_reset : std_logic;
	
	signal ll_q : std_logic_vector(15 downto 0);
	signal ll_oe_internal : std_logic;
	
	signal ll_vga_a : std_logic_vector(17 downto 0);
	signal ll_vga_q : std_logic_vector(15 downto 0);
	signal ll_vga_ce : std_logic;
	
	signal ll_cpu_a : std_logic_vector(17 downto 0);
	signal ll_cpu_d : std_logic_vector(15 downto 0);
	signal ll_cpu_q : std_logic_vector(15 downto 0);
	signal ll_cpu_rw : std_logic;
	signal ll_cpu_lb : std_logic;
	signal ll_cpu_ub : std_logic;
	signal ll_cpu_ce : std_logic;
	signal ll_cpu_ack : std_logic;
	
	signal bus_a : std_logic_vector(31 downto 0);
	signal bus_d : std_logic_vector(31 downto 0);
	signal bus_q : std_logic_vector(31 downto 0);
	signal bus_siz : std_logic_vector(1 downto 0);
	signal bus_rw : std_logic;
	
	signal bus_ce_llram : std_logic;
	signal bus_ack_llram : std_logic;
	signal bus_nack_llram : std_logic;
	
	signal bus_ce_chipset : std_logic;
	signal bus_ack_chipset : std_logic;
	signal bus_nack_chipset : std_logic;
	
	signal bus_ce_sdram : std_logic;
	signal bus_ack_sdram : std_logic;
	signal bus_nack_sdram : std_logic;
	
	signal ll_ub_internal : std_logic;
	
	signal chipset_ce : std_logic_vector(15 downto 0);
	signal chipset_ack : wor_logic_vector(15 downto 0);
	signal chipset_nack : wor_logic_vector(15 downto 0);
begin
	u_cpu: entity work.cpu port map(reset => internal_reset, clk => clk33,
		a => a, d => d, q => cpu_q, oe => cpu_oe,
		tt => cpu_tt, tm => cpu_tm, siz => cpu_siz, rw => cpu_rw, ts => cpu_ts, tip => cpu_tip, ta => cpu_ta, tea => cpu_tea,
		tbi => cpu_tbi, ipl => cpu_ipl, bclk => cpu_clk, lfo => cpu_lfo, scd => cpu_scd, rsti => cpu_rsti, rsto => cpu_rsto,
		
		bus_a => bus_a, bus_d => bus_q, bus_q => bus_d, bus_rw => bus_rw, bus_siz => bus_siz,
		
		bus_ce_llram => bus_ce_llram, bus_ack_llram => bus_ack_llram, bus_nack_llram => bus_nack_llram,
		bus_ce_chipset => bus_ce_chipset, bus_ack_chipset => bus_ack_chipset, bus_nack_chipset => bus_nack_chipset,
		bus_ce_sdram => bus_ce_sdram, bus_ack_sdram => bus_ack_sdram, bus_nack_sdram => bus_nack_sdram);
	
	-- *** RAM *** --
	
	u_sdram: entity work.sdram port map(reset => internal_reset, clk => clk33,
		a => ram_a, b => ram_b, cas => ram_cas, ras => ram_ras, we => ram_we, ldqm => ram_ldqm, udqm => ram_udqm,
		cs => ram_cs, cke => ram_cke,
		
		bus_a => bus_a, bus_d => bus_d, bus_q => bus_q,
		bus_rw => bus_rw, bus_siz => bus_siz, bus_ce => bus_ce_sdram, bus_ack => bus_ack_sdram);
	
	u_llram: entity work.llram generic map(data_width => 16, addr_width => 18)
		port map(reset => internal_reset, clk => clk33,
		a => ll_a, d => ll_d, q => ll_q, ce => ll_ce, we => ll_we, lb => ll_lb, ub => ll_ub_internal, oe => ll_oe_internal,
		vga_a => ll_vga_a, vga_q => ll_vga_q, vga_ce => ll_vga_ce,
		cpu_a => ll_cpu_a, cpu_d => ll_cpu_q, cpu_q => ll_cpu_d, cpu_rw => ll_cpu_rw, cpu_lb => ll_cpu_lb, cpu_ub => ll_cpu_ub,
		cpu_ce => ll_cpu_ce, cpu_ack => ll_cpu_ack,
		snd_a => (others => '1'), snd_q => open, snd_ce => '0');
	
	u_reset: entity work.reset port map(clk => clk33, pwron_reset => pwron_reset, reset => internal_reset);
	
	-- *** Bus adapters *** --
	
	u_llram_bus_adapter: entity work.llram_bus_adapter port map(clk => clk33, reset => internal_reset,
		ll_a => ll_cpu_a, ll_d => ll_cpu_d, ll_q => ll_cpu_q, ll_rw => ll_cpu_rw,
		ll_lb => ll_cpu_lb, ll_ub => ll_cpu_ub, ll_ce => ll_cpu_ce, ll_ack => ll_cpu_ack,
		bus_a => bus_a, bus_d => bus_d, bus_q => bus_q, bus_siz => bus_siz, bus_rw => bus_rw,
		bus_ce => bus_ce_llram, bus_ack => bus_ack_llram);
	
	u_chipset: entity work.chipset port map(
			reset => internal_reset, clk => clk33,
			
			bus_a => bus_a,
			bus_ce_chipset => bus_ce_chipset,
			bus_ack_chipset => bus_ack_chipset,
			bus_nack_chipset => bus_nack_chipset,
			
			chipset_ce => chipset_ce,
			chipset_ack => chipset_ack,
			chipset_nack => chipset_nack
		);
	
	-- *** System peripherals *** --
	
	-- u_sysinfo
	-- u_timer
	
	-- *** Hardware Peripherals *** --
	
	u_spi: entity work.spi generic map(peripheral_id => 8)
		port map(reset => internal_reset, clk => clk33,
		miso => spi_miso, mosi => spi_mosi, sck => spi_clk, ss => spi_ss,
		chipset_a => bus_a(7 downto 0), bus_d => bus_d, bus_q => bus_q,
		bus_rw => bus_rw, bus_siz => bus_siz,
		chipset_ce => chipset_ce, chipset_ack => chipset_ack, chipset_nack => chipset_nack);
	
	u_uart: entity work.uart generic map(peripheral_id => 9)
		port map(reset => internal_reset, clk => clk33,
		rx => uart_rx, tx => uart_tx,
		chipset_a => bus_a(7 downto 0), bus_d => bus_d, bus_q => bus_q,
		bus_rw => bus_rw, bus_siz => bus_siz,
		chipset_ce => chipset_ce, chipset_ack => chipset_ack, chipset_nack => chipset_nack);
	
	u_vga: entity work.vga generic map(peripheral_id => 10,
		depth_r => depth_r, depth_g => depth_g, depth_b => depth_b,
		line_front_porch => 800, line_hsync => 800 + 40, line_back_porch => 800 + 40 + 48, line_end => 928, --for LCD
		frame_front_porch => 480, frame_vsync => 480 + 13, frame_back_porch => 480 + 13 + 3, frame_end => 525, --for LCD
		ll_a_start => 0, ll_a_end => 800*480, ll_a_length => 18)
		port map(reset => internal_reset, clk => clk33, r => vga_r, g => vga_g, b => vga_b,
		hsync => vga_hsync, vsync => vga_vsync, den => vga_den,
		ll_a => ll_vga_a, ll_d => ll_vga_q, ll_ce => ll_vga_ce);
	
	u_sound: entity work.sound generic map(peripheral_id => 11)
		port map(reset => internal_reset, clk => clk12, mosi => open, sck => snd_clk, ss => snd_ss, sync => snd_sync);
	
	u_extint: entity work.extint generic map(peripheral_id => 12)
		port map(reset => internal_reset, clk => clk33,
		interrupt => ext_int,
		chipset_a => bus_a(7 downto 0), bus_d => bus_d, bus_q => bus_q,
		bus_rw => bus_rw, bus_siz => bus_siz,
		chipset_ce => chipset_ce, chipset_ack => chipset_ack, chipset_nack => chipset_nack);
	
	-- *** Output drivers *** --
	
	chipset_ack <= (others => '0');
	chipset_nack <= (others => '0');
	
	bus_nack_llram <= '0';
	bus_nack_sdram <= '0';
	
	vga_pwr <= internal_reset;
	vga_pwm <= not internal_reset;
	
	ll_d <= (others => 'Z') when ll_oe_internal <= '0' else ll_q;
	d <= (others => 'Z') when cpu_oe = '0' else cpu_q;
	
	snd_mosi <= cpu_siz(1);
	
	--spi_mosi <= ll_cpu_ce;
	--spi_clk <= ll_cpu_ack;
	--snd_mosi <= ll_ub_internal;
	ll_ub <= ll_ub_internal;
	
	ll_oe <= ll_oe_internal;
end arch;
