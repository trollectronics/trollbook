library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

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
	signal ll_lb : std_logic;
	signal ll_ub : std_logic;
	signal t : integer := 0;
	
	signal cpu_tt : std_logic_vector(1 downto 0);
	signal cpu_tm : std_logic_vector(2 downto 0);
	signal cpu_siz : std_logic_vector(1 downto 0);
	signal cpu_rw : std_logic;
	signal cpu_ts : std_logic;
	signal cpu_tip : std_logic;
	signal cpu_ta : std_logic;
	
	signal transfer_done : boolean := false;
	
	signal write_a : integer := 0;--524288;
	
	signal uart : std_logic;
begin
	clk33 <= not clk33 after 15 ns;
	clk12 <= not clk12 after 41 ns;
	
	pwron_reset <= '0', '1' after 100 ns;
	
	ll_d(15 downto 8) <= (others => 'Z') when ll_ce /= '0' or ll_oe /= '0' or ll_ub /= '0' else x"DE" when ll_a(0) = '0' else  x"BE";
	ll_d(7 downto 0) <= (others => 'Z') when ll_ce /= '0' or ll_oe /= '0' or ll_lb /= '0' else x"AD" when ll_a(0) = '0' else  x"EF";
	
	-- 9600 baud
	--uart <= '1', '0' after 235 us, '1' after (235 us + 104 us), '0' after (235 us + 104*3 us),'1' after (235 us + 104*5 us),
	--	'0' after (235 us + 104*6 us), '1' after (235 us + 104*7 us), '0' after (235 us + 104*8 us), '1' after (235 us + 104*9 us);
	
	-- 115200 baud
	uart <= '1', '0' after 235 us, '1' after (235 us + 8.680 us), '0' after (235 us + 8.680*3 us),'1' after (235 us + 8.680*5 us),
		'0' after (235 us + 8.680*6 us), '1' after (235 us + 8.680*7 us), '0' after (235 us + 8.680*8 us), '1' after (235 us + 8.680*9 us);
	
	u_dut: trollbook port map(
		a => a, d => d,
		cpu_tt => cpu_tt, cpu_tm => cpu_tm, cpu_siz => cpu_siz, cpu_rw => cpu_rw,
		cpu_ts => cpu_ts, cpu_tip => cpu_tip, cpu_ta => cpu_ta, cpu_tea => open, 
		cpu_tbi => open, cpu_ipl => open, cpu_clk => open, cpu_lfo => open,
		cpu_scd => '1', cpu_rsti => open, cpu_rsto => '1',
		
		ram_a => open, ram_b => open, ram_cas => open, ram_ras => open,
		ram_we => open, ram_ldqm => open, ram_udqm => open, ram_cs => open, ram_cke =>open,
		
		ll_a => ll_a, ll_d => ll_d, ll_ce => ll_ce, ll_we => open,
		ll_lb => ll_lb, ll_ub => ll_ub, ll_oe => ll_oe,
		
		vga_r => open, vga_g => open, vga_b => open, vga_hsync => open, vga_vsync => open,
		vga_den => open, vga_pwr => open, vga_pwm => open,
		
		snd_mosi => open, snd_clk => open, snd_ss => open, snd_sync => open,
		
		spi_miso => '0', spi_mosi => open, spi_clk => open, spi_ss => open,
		
		uart_rx => uart, uart_tx => open,
		
		ext_int => "1111",
		
		clk33 => clk33, clk12 => clk12,
		
		pwron_reset => pwron_reset
	);
	
	
	process(clk33) begin
		if falling_edge(clk33) then
			if transfer_done = true then--and t > 472 then
				--t <= 466;
				--write_a <= write_a + 2;
			else
				t <= t + 1;
			end if;
		elsif rising_edge(clk33) then
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
			
			
			--write to uart
			when 270 =>
				a <= x"00100000"; --std_logic_vector(to_unsigned(write_a, 32));
				cpu_siz <= "00";
				cpu_tt <= "00";
				cpu_rw <= '0';
				cpu_ts <= '0';
				cpu_tip <= '0';
				cpu_tm <= "001";
			when 272 =>
				cpu_ts <= '1';
				d <= x"000000A5"; --x"deadcafe";

			
			--write to SPI
			when 280 =>
				a <= x"00200000"; --std_logic_vector(to_unsigned(write_a, 32));
				cpu_siz <= "00";
				cpu_tt <= "00";
				cpu_rw <= '0';
				cpu_ts <= '0';
				cpu_tip <= '0';
				cpu_tm <= "001";
			when 282 =>
				cpu_ts <= '1';
				d <= x"000000A1"; --x"deadcafe";
			
			-- read from llram
			when 360 =>
				a <= x"00080000";
				d <= (others => 'Z');
				cpu_siz <= "00";
				cpu_tt <= "00";
				cpu_rw <= '1';
				cpu_ts <= '0';
				cpu_tip <= '0';
				cpu_tm <= "001";
			when 362 =>
				cpu_ts <= '1';
			
			
			when 470 =>
				a <= std_logic_vector(to_unsigned(write_a, 32));
				d <= (others => 'Z');
				cpu_siz <= "10";
				cpu_tt <= "00";
				cpu_rw <= '1';
				cpu_ts <= '0';
				cpu_tip <= '0';
				cpu_tm <= "001";
			when 472 =>
				cpu_ts <= '1';
			
			-- read from llram
			when 1910 =>
				a <= x"00080000";
				d <= (others => 'Z');
				cpu_siz <= "00";
				cpu_tt <= "00";
				cpu_rw <= '1';
				cpu_ts <= '0';
				cpu_tip <= '0';
				cpu_tm <= "001";
			when 1912 =>
				cpu_ts <= '1';
			
			-- when 470 =>
				-- a <= std_logic_vector(to_unsigned(write_a, 32));
				-- cpu_siz <= "00";
				-- cpu_tt <= "00";
				-- cpu_rw <= '0';
				-- cpu_ts <= '0';
				-- cpu_tip <= '0';
				-- cpu_tm <= "001";
			-- when 472 =>
				-- cpu_ts <= '1';
				-- d <= x"DEADBEEF"; --x"deadcafe";
			
			when others =>
				if transfer_done = true then
					a <= (others => '1');
					cpu_siz <= "11";
					cpu_tip <= '1';
					cpu_tt <= "11";
					cpu_tm <= "111";
					cpu_rw <= '1';
					d <= (others => 'Z');
					transfer_done <= false;
				end if;
				
				if t mod 2 = 1 and t > 260 then
					if cpu_ta = '0' then
						transfer_done <= true;
					end if;
				end if;
		end case;
	end process;
	
end tb_trollbook;
