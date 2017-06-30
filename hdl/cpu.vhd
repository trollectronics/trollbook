library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

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
		
		bus_ce_llram : out std_logic;
		bus_ack_llram : in std_logic;
		bus_nack_llram : in std_logic;
		
		bus_ce_chipset : out std_logic;
		bus_ack_chipset : in std_logic;
		bus_nack_chipset : in std_logic;
		
		bus_ce_sdram : out std_logic;
		bus_ack_sdram : in std_logic;
		bus_nack_sdram : in std_logic;
		
		interrupt_level : in std_logic_vector(2 downto 0)
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
	signal tbi_next : std_logic;
	signal ipl_next : std_logic_vector(2 downto 0);
	
	signal bootrom_q : std_logic_vector(31 downto 0);
	
	signal ce, ce_next : std_logic_vector(3 downto 0);
	signal ack : std_logic;
	
	signal done : std_logic;
	
	signal bus_siz_internal : std_logic_vector(1 downto 0);
begin
	u_bootrom: entity work.bootrom port map(
		address => a(8 downto 2),
		q => bootrom_q
	);
	
	tea <= '1';
	
	lfo <= '1';
	
	bus_a <= a when tip = '0' else (others => '0');
	bus_q <= d;
	bus_rw <= not rw;
	
	bus_ce_sdram <= ce(3);
	bus_ce_chipset <= ce(2);
	bus_ce_llram <= ce(1);
	
	bus_siz <= bus_siz_internal;
	
	ipl_next <= not interrupt_level;
	
	process(a, bus_ack_chipset, bus_nack_chipset, bus_ack_llram, bootrom_q, bus_d, tip, bus_ack_sdram, siz, tt, rw) begin
		q_next <= (others => '0');
		tbi_next <= '0';
		
		if siz = "11" then
			bus_siz_internal <= "11";
		else
			bus_siz_internal <= not siz;
		end if;
		--add burst disable the same way
		
		if tip = '0' and tt(1) = '0' then --normal read/write
			case a(31) is
				when '0' =>
					case a(20 downto 19) is
						when "00" => --bootrom
							q_next <= bootrom_q;
							ce_next <= "0001";
							ack <= '1';
						when "01" => --llram
							ce_next <= "0010";
							q_next <= bus_d;
							ack <= bus_ack_llram;
						when "10" => --chipset
							q_next <= bus_d;
							ce_next <= "0100";
							ack <= bus_ack_chipset;
						
							
						when others =>
							ce_next <= (others => '0');
							ack <= '0';
					end case;
				
				when '1' => -- sdram
					q_next <= x"DEADBEEF";
					ce_next <= "1000";
					ack <= bus_ack_sdram;
					if rw = '1' then
						bus_siz_internal <= not siz;
						tbi_next <= '1';
					end if;
				
				when others =>
					ce_next <= (others => '0');
					ack <= '0';
			end case;
		else
			ce_next <= (others => '0');
			ack <= '0';
		end if;
	end process;
	
	process(state, ts, tt, rw, a, bootrom_q, ack, ce_next, bus_siz_internal)
		variable check : std_logic_vector(1 downto 0);
	begin
		state_next <= state;
		--q_next <= (others => '0');
		oe_next <= '0';
		ta_next <= '1';
		done <= '0';
		check := ts & rw;
		
		case state is
			when idle =>
				if ts = '0' and tt = "11" then
					state_next <= write_ack;
				else
					case check is
						when "01" =>
							if bus_siz_internal = "00" then
								state_next <= read_burst0;
							else
								state_next <= read_normal;
							end if;
						when "00" =>
							if bus_siz_internal = "00" then
								state_next <= write_burst0;
							else
								state_next <= write_normal;
							end if;
						when others =>
					end case;
				end if;
			
			when read_normal =>
				--q_next <= bootrom_q;
				if ack = '1' then
					ta_next <= '0';
					if a(31) = '0' then -- no OE for sdram
						oe_next <= '1';
					end if;
					state_next <= idle;
					done <= '1';
				end if;
			
			when read_burst0 =>
				if ack = '1' then
					ta_next <= '0';
					state_next <= read_burst1;
				end if;
			when read_burst1 =>
				ta_next <= '0';
				state_next <= read_burst2;
			when read_burst2 =>
				ta_next <= '0';
				state_next <= read_burst3;
			when read_burst3 =>
				ta_next <= '0';
				done <= '1';
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
					state_next <= write_burst1;
					ta_next <= '0';
				end if;
			when write_burst1 =>
				ta_next <= '0';
				state_next <= write_burst2;
			when write_burst2 =>
				ta_next <= '0';
				state_next <= write_burst3;
			when write_burst3 =>
				ta_next <= '0';
				done <= '1';
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
			tbi <= '0';
			ipl <= "111";
			
			ce <= (others => '0');
		elsif falling_edge(clk) then
			state <= state_next;
			if done = '1' then
				ce <= (others => '0');
			else
				ce <= ce_next;
			end if;
				
		elsif rising_edge(clk) then
			q <= q_next;
			ta <= ta_next;
			oe <= oe_next;
			tbi <= tbi_next;
			
			ipl <= ipl_next;
		end if;
	end process;
	
	bclk <= not clk;
end arch;
