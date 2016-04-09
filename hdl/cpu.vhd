library ieee;
use ieee.std_logic_1164.all;

entity cpu is
	port(
		reset : in std_logic;
		clk : in std_logic;
		
		a : in std_logic_vector(31 downto 0);
		d : in std_logic_vector(31 downto 0);
		q : out std_logic_vector(31 downto 0);
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
		rsto : in std_logic;
		
		ll_a : out std_logic_vector(17 downto 0);
		ll_d : in std_logic_vector(15 downto 0);
		ll_q : out std_logic_vector(15 downto 0);
		ll_rw : out std_logic;
		ll_siz : out std_logic_vector(1 downto 0);
		ll_ce : out std_logic;
		ll_ack : in std_logic;
		
		bus_a : out std_logic_vector(31 downto 0);
		bus_d : in std_logic_vector(31 downto 0);
		bus_q : out std_logic_vector(31 downto 0);
		bus_rw : out std_logic;
		bus_siz : out std_logic_vector(1 downto 0);
		
		bus_ce_uart : out std_logic;
		bus_ack_uart : in std_logic
	);
end cpu;

architecture arch of cpu is
	type state_type is (idle, read_normal, 
		write_normal, write_ack,
		read_burst0, read_burst1, read_burst2, read_burst3,
		write_burst0, write_burst1, write_burst2, write_burst3);
	
	signal state, state_next : state_type;
	
	signal ta_next : std_logic;
	signal q_next : std_logic_vector(q'range);
	signal oe_next : std_logic;
	
	signal bootrom_q : std_logic_vector(31 downto 0);
	
	signal ll_ce_next : std_logic;
	signal ll_rw_next : std_logic;
	
	signal ce, ce_next : std_logic_vector(7 downto 0);
	signal ack : std_logic;
begin
	u_bootrom: entity work.bootrom port map(
		address => a(7 downto 2),
		q => bootrom_q
	);
	
	--ta <= '1';
	tea <= '1';
	tbi <= '0';
	ipl <= "111";
	lfo <= '0';
	
	ll_a <= a(18 downto 1);
	ll_siz <= not siz;
	ll_q <= d(15 downto 0) when a(1) = '1' else d(31 downto 16);
	
	bus_a <= a;
	bus_q <= d;
	bus_siz <= not siz;
	bus_rw <= not rw;
	
	bus_ce_uart <= ce(2);
	
	process(a, bus_ack_uart, ll_ack) begin
		q_next <= (others => '0');
		if tip = '0' then
			case a(23 downto 19) is
				when "00000" => --bootrom
					q_next <= bootrom_q;
					ce_next <= "00000001";
					ack <= '1';
				when "00001" => --llram
					ce_next <= "00000010";
					ack <= ll_ack;
				when "00010" => --chipset
					q_next <= bus_d;
					ce_next <= "00000100";
					ack <= bus_ack_uart;
				when others =>
					ce_next <= (others => '0');
					ack <= '0';
			end case;
		else
			ce_next <= (others => '0');
			ack <= '0';
		end if;
	end process;
	
	process(state, ts, tt, rw, a, bootrom_q, ll_ack)
		variable check : std_logic_vector(3 downto 0);
	begin
		state_next <= state;
		--q_next <= (others => '0');
		oe_next <= '0';
		ta_next <= '1';
		check := ts & tt & rw;
		
		ll_ce_next <= '0';
		ll_rw_next <= '0';
		
		case state is
			when idle =>
				case check is
					when "0001" =>
						state_next <= read_normal;
					when "0011" =>
						state_next <= read_burst0;
					when "0000" =>
						state_next <= write_normal;
						ll_ce_next <= '1';
						ll_rw_next <= '1';
					when "0010" =>
						state_next <= write_burst0;
					when others =>
				end case;
			
			when read_normal =>
				--q_next <= bootrom_q;
				ta_next <= '0';
				oe_next <= '1';
				state_next <= idle;
			
			when read_burst0 =>
				--q_next <= bootrom_q;
				ta_next <= '0';
				oe_next <= '1';
				state_next <= idle;
				--bursting disabled
			when read_burst1 =>
				--q_next <= x"60046004";
				ta_next <= '0';
				oe_next <= '1';
				state_next <= read_burst2;
			when read_burst2 =>
				--q_next <= x"60046004";
				ta_next <= '0';
				oe_next <= '1';
				state_next <= read_burst3;
			when read_burst3 =>
				--q_next <= x"60046004";
				ta_next <= '0';
				oe_next <= '1';
				state_next <= idle;
			
			when write_normal =>
				ll_rw_next <= '1';
				ll_ce_next <= '1';
				
				if ll_ack = '1' then
					state_next <= write_ack;
				end if;
			
			when write_ack =>
				ta_next <= '0';
				state_next <= idle;
			
			when write_burst0 =>
				ta_next <= '0';
				state_next <= write_burst1;
			when write_burst1 =>
				ta_next <= '0';
				state_next <= write_burst2;
			when write_burst2 =>
				ta_next <= '0';
				state_next <= write_burst3;
			when write_burst3 =>
				ta_next <= '0';
				state_next <= idle;
			
		end case;
	end process;
	
	process(reset, clk) begin
		rsti <= '1';
		if reset = '1' then
			state <= idle;
			rsti <= '0';
			ta <= '1';
			q <= (others => '1');
			oe <= '0';
			
			ll_ce <= '0';
			ll_rw <= '0';
			
			ce <= (others => '0');
		elsif rising_edge(clk) then
			state <= state_next;
		elsif falling_edge(clk) then
			q <= q_next;
			ta <= ta_next;
			oe <= oe_next;
			ll_ce <= ll_ce_next;
			ll_rw <= ll_rw_next;
			
			ce <= ce_next;
		end if;
	end process;
	
	bclk <= clk;
end arch;
