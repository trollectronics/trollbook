entity test is end entity;

architecture tb_trollbook is
	component trollbook is
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
	end component;
	
	signal clk33, clk12, pwron_reset;
begin
	clk33 <= not clk33 after 30ns;
	clk12 <= not clk12 after 768ns;
	
	pwron_reset <= '0', '1' after 100ns;
	
	u1: trollbook port map(
		a => (others => '0), d => (others => 'z'),
		cpu_tt => "00", cpu_tm => "000", cpu_siz => "00", cpu_rw => '0',
		cpu_ts => '1', cpu_tip => '1', cpu_ta => open, cpu_tea => open, 
		cpu_tbi => open, cpu_ipl => open, cpu_clk => clk33, cpu_lfo => open,
		cpu_scd => '1', cpu_rsti => open, cpu_rsto => '1',
		
		ram_a => open, ramb => open, ram_cas => open, ram_ras => open,
		ram_we => open, ram_ldqm => open, ram_udqm => open, ram_cs => open, ram_cke =>open,
		
		ll_a => open, ll_d => (others => 'Z'), ll_ce => open, ll_we => open,
		ll_lb => open, ll_ub => open, ll_oe => open,
		
		vga_r => open, vga_g => open, vga_b => open, vga_hsync => open, vga_vsync => open,
		vga_den => open, vga_pwr => open, vga_pwm => open,
		
		snd_mosi => open, snd_clk => open, snd_ss => open, snd_ss => open,
		
		spi_miso => '1', spi_mosi => open, spi_clk => open, spi_ss => open,
		
		uart_rx => '1', uart_tx => open,
		
		ext_int => "111",
		
		clk33 => clk33, clk12 => clk12,
		
		pwron_reset => pwron_reset
	);
	
end tb_trollbook;
