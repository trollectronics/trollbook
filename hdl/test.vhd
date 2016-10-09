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
	
	signal write_a : integer := 0;--524288;
	
	signal uart : std_logic;
	
	signal ram_a : std_logic_vector(12 downto 0);
	signal ram_b : std_logic_vector(1 downto 0);
	signal ram_cas : std_logic;
	signal ram_ras : std_logic;
	signal ram_we : std_logic;
	signal ram_ldqm : std_logic_vector(1 downto 0);
	signal ram_udqm : std_logic_vector(1 downto 0);
	signal ram_cs : std_logic_vector(1 downto 0);
	signal ram_cke : std_logic;
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
		
		ram_a => ram_a, ram_b => ram_b, ram_cas => ram_cas, ram_ras => ram_ras,
		ram_we => ram_we, ram_ldqm => ram_ldqm, ram_udqm => ram_udqm, ram_cs => ram_cs, ram_cke => ram_cke,
		
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
			t <= t + 1;
		elsif rising_edge(clk33) then
			t <= t + 1;
		end if;
	end process;
	
	sdram: process(clk33, ram_ldqm, ram_udqm) is
		type sdram_state_type is (cas3, cas2, data0, data1, data2, data3, idle);
		variable sdram_state : sdram_state_type := idle;
		
		function mask(data : std_logic_vector(31 downto 0); ldqm : std_logic_vector(1 downto 0); udqm : std_logic_vector(1 downto 0)) return std_logic_vector is
			variable m : std_logic_vector(31 downto 0) := (others => '1');
		begin
			if udqm(1) = '1' then m := m and x"00FFFFFF"; end if;
			if ldqm(1) = '1' then m := m and x"FF00FFFF"; end if;
			if udqm(0) = '1' then m := m and x"FFFF00FF"; end if;
			if ldqm(0) = '1' then m := m and x"FFFFFF00"; end if;
			return data and m;
		end function;
	begin
		if falling_edge(clk33) then		
			if sdram_state /= idle then
				sdram_state := sdram_state_type'rightof(sdram_state);
			end if;
			
			if ram_cs = "00" and cpu_rw = '1' then
				if ram_ras = '1' and ram_cas = '0' and ram_we = '1' then
					sdram_state := cas2;
				end if;
			end if;
		end if;
		
		case sdram_state is
				when data0 =>
					d <= mask(x"DEADBEE0", ram_ldqm, ram_udqm);
				when data1 =>
					d <= mask(x"DEADBEE1", ram_ldqm, ram_udqm);
				when data2 =>
					d <= mask(x"DEADBEE2", ram_ldqm, ram_udqm);
				when data3 =>
					d <= mask(x"DEADBEE3", ram_ldqm, ram_udqm);
				
				when others =>
					d <= (others => 'Z');
			end case;
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
				cpu_tip <= '0';
			when 244 =>
				a <= x"00000008";
				cpu_siz <= "10";
				cpu_ts <= '0';
				cpu_tip <= '0';
			when 246 =>
				cpu_ts <= '1';
				cpu_tip <= '0';
			when 248 =>
				a <= x"0000000A";
				cpu_siz <= "01";
				cpu_ts <= '0';
				cpu_tip <= '0';
			when 250 =>
				cpu_ts <= '1';
				cpu_tip <= '0';
			
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
			-- when 264 =>
				-- a <= (others => '1');
				-- cpu_siz <= "11";
				-- cpu_tip <= '1';
				-- cpu_tt <= "11";
				-- cpu_tm <= "111";
			
			
			--write to uart
			when 270 =>
				a <= x"00100900"; --std_logic_vector(to_unsigned(write_a, 32));
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
				a <= x"00100800"; --std_logic_vector(to_unsigned(write_a, 32));
				cpu_siz <= "00";
				cpu_tt <= "00";
				cpu_rw <= '0';
				cpu_ts <= '0';
				cpu_tip <= '0';
				cpu_tm <= "001";
			when 282 =>
				cpu_ts <= '1';
				d <= x"00000090"; --x"deadcafe";
			
			--read from SPI
			when 284 =>
				a <= x"00100804"; --std_logic_vector(to_unsigned(write_a, 32));
				d <= (others => 'Z');
				cpu_siz <= "00";
				cpu_tt <= "00";
				cpu_rw <= '1';
				cpu_ts <= '0';
				cpu_tip <= '0';
				cpu_tm <= "001";
			when 286=>
				cpu_ts <= '1';
			
			-- read from llram
			when 360 =>
				a <= x"00080000";
				d <= (others => 'Z');
				cpu_siz <= "11";
				cpu_tt <= "00";
				cpu_rw <= '1';
				cpu_ts <= '0';
				cpu_tip <= '0';
				cpu_tm <= "001";
			when 362 =>
				cpu_ts <= '1';
			
			when 380 => --sdram
				a <= x"40000000";
				d <= (others => 'Z');
				cpu_siz <= "11";
				cpu_tt <= "00";
				cpu_rw <= '1';
				cpu_ts <= '0';
				cpu_tip <= '0';
				cpu_tm <= "001";
			when 382 =>
				cpu_ts <= '1';
			
			when 420 => --sdram
				a <= x"40000000";
				cpu_siz <= "10";
				cpu_tt <= "00";
				cpu_rw <= '0';
				cpu_ts <= '0';
				cpu_tip <= '0';
				cpu_tm <= "001";
			when 422 =>
				cpu_ts <= '1';
				d <= x"CAFEBEE0";
			when 425 | 427 | 429 | 431 | 433 | 435 =>
				if cpu_ta = '0' and clk33'event then
					d <= std_logic_vector(unsigned(d) + 1);
				end if;
			
			
			-- read from llram
			when 440 =>
				a <= x"00080000";
				d <= (others => 'Z');
				cpu_siz <= "00";
				cpu_tt <= "00";
				cpu_rw <= '1';
				cpu_ts <= '0';
				cpu_tip <= '0';
				cpu_tm <= "001";
			when 442 =>
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
		end case;
		
		if rising_edge(cpu_ta) then
			a <= (others => '1');
			cpu_siz <= "11";
			cpu_tip <= '1';
			cpu_tt <= "11";
			cpu_tm <= "111";
			cpu_rw <= '1';
			d <= (others => 'Z');
		end if;
	end process;
	
end tb_trollbook;
