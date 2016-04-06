library ieee;
use ieee.std_logic_1164.all;

entity test is end entity;

architecture tb_trollbook of test is
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
	
	signal clk33 : std_logic := '0';
	signal clk12 : std_logic := '0';
	signal pwron_reset : std_logic;
	
	signal d : std_logic_vector(31 downto 0);
	signal a : std_logic_vector(31 downto 0);
	signal ll_d : std_logic_vector(15 downto 0);
	signal ll_a : std_logic_vector(17 downto 0);
	signal ll_ce : std_logic;
	signal ll_oe : std_logic;
	signal t : integer := 0;
	
	signal cpu_tt : std_logic_vector(1 downto 0);
	signal cpu_tm : std_logic_vector(2 downto 0);
	signal cpu_siz : std_logic_vector(1 downto 0);
	signal cpu_rw : std_logic;
	signal cpu_ts : std_logic;
	signal cpu_tip : std_logic;
	signal cpu_ta : std_logic;
	
	signal write_done : boolean := false;
begin
	clk33 <= not clk33 after 15 ns;
	clk12 <= not clk12 after 384 ns;
	
	pwron_reset <= '0', '1' after 100 ns;
	
	ll_d <= ll_a(6 downto 0) & '0' & ll_a(6 downto 0) & '1' when ll_ce = '0' and ll_oe = '0' else (others => 'Z');
	
	u_dut: trollbook port map(
		a => a, d => d,
		cpu_tt => cpu_tt, cpu_tm => cpu_tm, cpu_siz => cpu_siz, cpu_rw => cpu_rw,
		cpu_ts => cpu_ts, cpu_tip => cpu_tip, cpu_ta => cpu_ta, cpu_tea => open, 
		cpu_tbi => open, cpu_ipl => open, cpu_clk => open, cpu_lfo => open,
		cpu_scd => '1', cpu_rsti => open, cpu_rsto => '1',
		
		ram_a => open, ram_b => open, ram_cas => open, ram_ras => open,
		ram_we => open, ram_ldqm => open, ram_udqm => open, ram_cs => open, ram_cke =>open,
		
		ll_a => ll_a, ll_d => ll_d, ll_ce => ll_ce, ll_we => open,
		ll_lb => open, ll_ub => open, ll_oe => ll_oe,
		
		vga_r => open, vga_g => open, vga_b => open, vga_hsync => open, vga_vsync => open,
		vga_den => open, vga_pwr => open, vga_pwm => open,
		
		snd_mosi => open, snd_clk => open, snd_ss => open, snd_sync => open,
		
		spi_miso => '1', spi_mosi => open, spi_clk => open, spi_ss => open,
		
		uart_rx => '1', uart_tx => open,
		
		ext_int => "1111",
		
		clk33 => clk33, clk12 => clk12,
		
		pwron_reset => pwron_reset
	);
	
	
	process(clk33) begin
		if rising_edge(clk33) then
			t <= t + 1;
		elsif falling_edge(clk33) then
			t <= t + 1;
		end if;
	end process;
	
	process(t, cpu_ta, clk33) begin
		case t is
			-- reset
			when 0 =>
				a <= (others => '1');
				d <= (others => 'Z');
				cpu_siz <= "11";
				cpu_tt <= "11";
				cpu_rw <= '1';
				cpu_ts <= '1';
				cpu_tip <= '1';
				cpu_tm <= "111";
			
			-- read cycle
			when 240 =>
				a <= x"00000007";
				d <= (others => 'Z');
				cpu_siz <= "01";
				cpu_tt <= "00";
				cpu_rw <= '1';
				cpu_ts <= '0';
				cpu_tip <= '0';
				cpu_tm <= "001";
			when 242 =>
				cpu_ts <= '1';
			when 244 =>
				a <= x"00000008";
				cpu_siz <= "10";
				cpu_ts <= '0';
			when 246 =>
				cpu_ts <= '1';
			when 248 =>
				a <= x"0000000A";
				cpu_siz <= "01";
				cpu_ts <= '0';
			when 250 =>
				cpu_ts <= '1';
			when 252 =>
				a <= (others => '1');
				cpu_siz <= "11";
				cpu_tip <= '1';
				cpu_tt <= "11";
				cpu_tm <= "111";
			
			when 260 =>
				a <= x"00000008";
				d <= (others => 'Z');
				cpu_siz <= "00";
				cpu_tt <= "00";
				cpu_rw <= '1';
				cpu_ts <= '0';
				cpu_tip <= '0';
				cpu_tm <= "001";
			when 262 =>
				cpu_ts <= '1';
			when 264 =>
				a <= (others => '1');
				cpu_siz <= "11";
				cpu_tip <= '1';
				cpu_tt <= "11";
				cpu_tm <= "111";
			
			
			-- write cycle
			when 270 =>
				write_done <= false;
				a <= x"abcd1234";
				cpu_siz <= "10";
				cpu_tt <= "00";
				cpu_rw <= '0';
				cpu_ts <= '0';
				cpu_tip <= '0';
				cpu_tm <= "001";
			when 272 =>
				cpu_ts <= '1';
				d <= x"0000cafe";
			when 274 | 276 | 278 | 280 =>
				if cpu_ta = '0' then
					write_done <= true;
				end if;
			
			when others =>
				if write_done = true then
					a <= (others => '1');
					cpu_siz <= "11";
					cpu_tip <= '1';
					cpu_tt <= "11";
					cpu_tm <= "111";
					cpu_rw <= '1';
					d <= (others => 'Z');
					write_done <= false;
				end if;
		end case;
	end process;
	
end tb_trollbook;
