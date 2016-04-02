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
		rsto : in std_logic
	);
end cpu;

architecture arch of cpu is
	type state_type is (idle, read_normal, write_normal,
		read_burst0, read_burst1, read_burst2, read_burst3,
		write_burst0, write_burst1, write_burst2, write_burst3);
	
	signal state, state_next : state_type;
	
	signal ta_next : std_logic;
	signal q_next : std_logic_vector(q'range);
	signal oe_next : std_logic;
begin
	--ta <= '1';
	tea <= '1';
	tbi <= '1';
	ipl <= "111";
	lfo <= '0';
	
	process(state, ts, tt, rw)
		variable check : std_logic_vector(3 downto 0);
	begin
		state_next <= state;
		q_next <= (others => '0');
		oe_next <= '0';
		ta_next <= '1';
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
			
			when read_normal =>
				q_next <= x"60046004";
				ta_next <= '0';
				oe_next <= '1';
				state_next <= idle;
			
			when read_burst0 =>
				q_next <= x"60046004";
				ta_next <= '0';
				oe_next <= '1';
				state_next <= read_burst1;
			when read_burst1 =>
				q_next <= x"60046004";
				ta_next <= '0';
				oe_next <= '1';
				state_next <= read_burst2;
			when read_burst2 =>
				q_next <= x"60046004";
				ta_next <= '0';
				oe_next <= '1';
				state_next <= read_burst3;
			when read_burst3 =>
				q_next <= x"60046004";
				ta_next <= '0';
				oe_next <= '1';
				state_next <= idle;
			
			when write_normal =>
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
		elsif rising_edge(clk) then
			state <= state_next;
		elsif falling_edge(clk) then
			q <= q_next;
			ta <= ta_next;
			oe <= oe_next;
		end if;
	end process;
	
	bclk <= clk;
end arch;
