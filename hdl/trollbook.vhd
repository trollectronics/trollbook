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
	end component;
	
	component sound is
		port(
			reset : in std_logic;
			clk : in std_logic;
			
			mosi : out std_logic;
			sck : out std_logic;
			ss : out std_logic;
			sync : out std_logic
		);
	end component;
	
	component sdram is
		port(
			reset : in std_logic;
			clk : in std_logic;
			
			a : out std_logic_vector(12 downto 0);
			b : out std_logic_vector(1 downto 0);
			cas : out std_logic;
			ras : out std_logic;
			we : out std_logic;
			ldqm : out std_logic_vector(1 downto 0);
			udqm : out std_logic_vector(1 downto 0);
			cs : out std_logic_vector(1 downto 0);
			cke : out std_logic
		);
	end component;
	
	component cpu is
		port(
			reset : in std_logic;
			clk : in std_logic;
			
			a : in std_logic_vector(31 downto 0);
			d_in : in std_logic_vector(31 downto 0);
			d_out : out std_logic_vector(31 downto 0);
			oe : out std_logic;
		
			tt : in std_logic_vector(1 downto 0);
			tm : in std_logic_vector(2 downto 0);
			siz : in std_logic_vector(1 downto 0);
			rw : in std_logic;
			ts : in std_logic;
			tip : in std_logic;
			ta : out std_logic;
			tea : out std_logic;
			tbi :  out std_logic;
			ipl : out std_logic_vector(2 downto 0);
			bclk : out std_logic;
			lfo : out std_logic;
			scd : in std_logic;
			rsti : out std_logic;
			rsto : in std_logic
		);
	end component;
	
	component llram is
		generic(
			data_width : integer;
			addr_width : integer
		);
		port(
			reset : in std_logic;
			clk : in std_logic;
			
			a : out std_logic_vector(addr_width - 1 downto 0);
			d : inout std_logic_vector(data_width - 1 downto 0);
			q : out std_logic_vector(data_width - 1 downto 0);
			ce : out std_logic;
			we : out std_logic;
			lb : out std_logic;
			ub : out std_logic;
			oe : out std_logic;
			
			vga_a : in std_logic_vector(addr_width - 1 downto 0);
			vga_q : out std_logic_vector(data_width - 1 downto 0);
			vga_ce : in std_logic;
			
			cpu_a : in std_logic_vector(addr_width - 1 downto 0);
			cpu_d : in std_logic_vector(data_width - 1 downto 0);
			cpu_q : out std_logic_vector(data_width - 1 downto 0);
			cpu_rw : in std_logic;
			cpu_siz : in std_logic_vector(1 downto 0);
			cpu_ce : in std_logic;
			
			snd_a : in std_logic_vector(addr_width - 1 downto 0);
			snd_q : out std_logic_vector(data_width - 1 downto 0);
			snd_ce : in std_logic
		);
	end component;
	
	component spi is
		port(
			reset : in std_logic;
			clk : in std_logic;
			
			miso : in std_logic;
			mosi : out std_logic;
			sck : out std_logic;
			ss : out std_logic_vector(2 downto 0)
		);
	end component;
	
	component uart is
		port(
			reset : in std_logic;
			clk : in std_logic;
			
			rx : in std_logic;
			tx : out std_logic
		);
	end component;
	
	component reset is
		port(
			clk : in std_logic;
			pwron_reset : in std_logic;
			reset : out std_logic
		);
	end component;
	
	signal cpu_d_out : std_logic_vector(31 downto 0);
	signal cpu_oe : std_logic;
	signal internal_reset : std_logic;
	
	signal ll_q: std_logic_vector(15 downto 0);
	
	signal ll_vga_a : std_logic_vector(17 downto 0);
	signal ll_vga_q : std_logic_vector(15 downto 0);
	signal ll_vga_ce : std_logic;
begin
	u_vga: vga generic map(depth_r => depth_r, depth_g => depth_g, depth_b => depth_b,
		line_front_porch => 800, line_hsync => 800 + 40, line_back_porch => 800 + 40 + 48, line_end => 928,
		frame_front_porch => 480, frame_vsync => 480 + 13, frame_back_porch => 480 + 13 + 3, frame_end => 525,
		ll_a_start => 0, ll_a_end => 800*480, ll_a_length => 18)
		port map(reset => internal_reset, clk => clk33, r => open, g => vga_g, b => vga_b,
		hsync => vga_hsync, vsync => vga_vsync, den => vga_den,
		ll_a => ll_vga_a, ll_d => ll_vga_q, ll_ce => ll_vga_ce);
	
	u_sound: sound port map(reset => internal_reset, clk => clk12, mosi => snd_mosi, sck => snd_clk, ss => snd_ss, sync => snd_sync);
	
	u_sdram: sdram port map(reset => internal_reset, clk => clk33,
		a => ram_a, b => ram_b, cas => ram_cas, ras => ram_ras, we => ram_we, ldqm => ram_ldqm, udqm => ram_udqm,
		cs => ram_cs, cke => ram_cke);
	
	u_cpu: cpu port map(reset => internal_reset, clk => clk33,
		a => a, d_in => d, d_out => cpu_d_out, oe => cpu_oe,
		tt => cpu_tt, tm => cpu_tm, siz => cpu_siz, rw => cpu_rw, ts => cpu_ts, tip => cpu_tip, ta => cpu_ta, tea => cpu_tea,
		tbi => cpu_tbi, ipl => cpu_ipl, bclk => cpu_clk, lfo => cpu_lfo, scd => cpu_scd, rsti => cpu_rsti, rsto => cpu_rsto);
	
	u_llram: llram generic map(data_width => 16, addr_width => 18)
		port map(reset => internal_reset, clk => clk33,
		a => ll_a, d => ll_d, q => ll_q, ce => ll_ce, we => ll_we, lb => ll_lb, ub => ll_ub, oe => ll_oe,
		vga_a => ll_vga_a, vga_q => ll_vga_q, vga_ce => ll_vga_ce,
		cpu_a => (others => '1'), cpu_d => (others => '1'), cpu_q => open, cpu_rw => '0', cpu_siz => "00", cpu_ce => '0',
		snd_a => (others => '1'), snd_q => open, snd_ce => '0');
	
	u_spi: spi port map(reset => internal_reset, clk => clk33,
		miso => spi_miso, mosi => spi_mosi, sck => spi_clk, ss => spi_ss);
	
	u_uart: uart port map(reset => internal_reset, clk => clk33,
		rx => uart_rx, tx => uart_tx);
	
	u_reset: reset port map(clk => clk33, pwron_reset => pwron_reset, reset => internal_reset);
	
	vga_pwr <= internal_reset;
	vga_pwm <= not internal_reset;
	
	process(cpu_oe, cpu_d_out) begin
		d <= (others => 'Z');
		ll_d <= (others => 'Z');
		
		if cpu_oe = '1' then
			d <= cpu_d_out;
		end if;
	end process;
	
	vga_r <= a(4 downto 0);
end arch;
