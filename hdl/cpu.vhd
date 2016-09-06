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
		
		bus_a : out std_logic_vector(31 downto 0);
		bus_d : in std_logic_vector(31 downto 0);
		bus_q : out std_logic_vector(31 downto 0);
		bus_rw : out std_logic;
		bus_siz : out std_logic_vector(1 downto 0);
		
		bus_ce_uart : out std_logic;
		bus_ack_uart : in std_logic;
		bus_ce_llram : out std_logic;
		bus_ack_llram : in std_logic
		bus_ce_spi: out std_logic;
		bus_ack_spi: in std_logic;
	);
end cpu;

architecture arch of cpu is
	type state_type is (idle, read_normal, write_normal, read_ack, write_ack,
		read_burst0, read_burst1, read_burst2, read_burst3,
		write_burst0, write_burst1, write_burst2, write_burst3);
	
	signal state, state_next : state_type;
	
	signal ta_next : std_logic;
	signal q_next : std_logic_vector(q'range);
	signal oe_next : std_logic;
	
	signal bootrom_q : std_logic_vector(31 downto 0);
	
	signal ce, ce_next : std_logic_vector(7 downto 0);
	signal ack : std_logic;
	
	signal done : std_logic;
begin
	u_bootrom: entity work.bootrom port map(
		address => a(8 downto 2),
		q => bootrom_q
	);
	
	--ta <= '1';
	tea <= '1';
	tbi <= '0';
	ipl <= "111";
	lfo <= '0';
	
	bus_a <= a;
	bus_q <= d;
	bus_siz <= "11" when siz = "11" else not siz;
	bus_rw <= not rw;
	
	bus_ce_spi <= ce(3);
	bus_ce_uart <= ce(2);
	bus_ce_llram <= ce(1);
	
	process(a, bus_ack_uart, bus_ack_llram, bootrom_q, bus_d, tip) begin
		q_next <= (others => '0');
		if tip = '0' then
			case a(23 downto 19) is
				when "00000" => --bootrom
					q_next <= bootrom_q;
					ce_next <= "00000001";
					ack <= '1';
				when "00001" => --llram
					ce_next <= "00000010";
					q_next <= bus_d;
					ack <= bus_ack_llram;
				when "00010" => --chipset
					q_next <= bus_d;
					ce_next <= "00000100";
					ack <= bus_ack_uart;
				when "00011" => -- spi
					q_next <= bus_d;
					ce_next <= "00001000";
					ack <= bus_spi_spi;
				when others =>
					ce_next <= (others => '0');
					ack <= '0';
			end case;
		else
			ce_next <= (others => '0');
			ack <= '0';
		end if;
	end process;
	
	process(state, ts, tt, rw, a, bootrom_q, ack, ce_next)
		variable check : std_logic_vector(3 downto 0);
	begin
		state_next <= state;
		--q_next <= (others => '0');
		oe_next <= '0';
		ta_next <= '1';
		done <= '0';
		check := ts & tt & rw;
		
		case state is
			when idle =>
				case check is
					when "0001" =>
						state_next <= read_normal;
					when "0011" =>
						state_next <= read_burst0;
					when "0000" =>
						state_next <= write_normal;
					when "0010" =>
						state_next <= write_burst0;
					when others =>
				end case;
			
			when read_normal | read_burst0=>
				--q_next <= bootrom_q;
				if ack = '1' then
					ta_next <= '0';
					oe_next <= '1';
					state_next <= idle;
					done <= '1';
				end if;
			
			-- when read_burst0 =>
				----q_next <= bootrom_q;
				-- if ce_next(0) = '1' then
					-- ta_next <= '0';
					-- oe_next <= '1';
					-- state_next <= idle;
				-- elsif ack = '1' then
					-- state_next <= read_ack;
				-- end if;
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
				if ack = '1' then
					state_next <= idle;
					ta_next <= '0';
					done <= '1';
				end if;
			
			when write_burst0 =>
				--ta_next <= '0';
				--state_next <= write_burst1;
				if ack = '1' then
					state_next <= idle;
					ta_next <= '0';
					done <= '1';
				end if;
			when write_burst1 =>
				ta_next <= '0';
				state_next <= write_burst2;
			when write_burst2 =>
				ta_next <= '0';
				state_next <= write_burst3;
			when write_burst3 =>
				ta_next <= '0';
				state_next <= idle;
			
			when read_ack =>
				oe_next <= '1';
				ta_next <= '0';
				state_next <= idle;
			
			when write_ack =>
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
			
			ce <= (others => '0');
		elsif rising_edge(clk) then
			state <= state_next;
			if done = '1' then
				ce <= (others => '0');
			else
				ce <= ce_next;
			end if;
			
		elsif falling_edge(clk) then
			q <= q_next;
			ta <= ta_next;
			oe <= oe_next;
		end if;
	end process;
	
	bclk <= clk;
end arch;
