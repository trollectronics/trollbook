library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity sdram is
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
		cke : out std_logic;
		
		bus_a : in std_logic_vector(31 downto 0);
		bus_d : in std_logic_vector(31 downto 0);
		bus_q : out std_logic_vector(31 downto 0);
		bus_rw : in std_logic;
		bus_siz : in std_logic_vector(1 downto 0);
		bus_ce : in std_logic;
		bus_ack : out std_logic
	);
end sdram;

architecture arch of sdram is
	constant RCD_CYCLES : integer := 1;
	constant CAS_LATENCY_CYCLES : integer := 2;
	constant INIT_STABILIZE_CYCLES : integer := 20;
	constant REFRESH_CYCLES : integer := 250;
	
	constant MODE_WBL : std_logic := '0';
	constant MODE_CAS_LATENCY : std_logic_vector(2 downto 0) := "010";
	constant MODE_BT : std_logic := '0';
	constant MODE_BURST_LENGTH : std_logic_vector(2 downto 0) := "010";
	
	type state_type is (
		powerdown, init, config, idle, open_row, read_command, write_command, read_precharge, write_precharge, waitstate, refresh
	);
	
	signal state, state_next : state_type;
	
	signal a_next, a_internal : std_logic_vector(12 downto 0);
	signal b_next, b_internal : std_logic_vector(1 downto 0);
	signal cas_next, ras_next, we_next, cas_internal, ras_internal, we_internal : std_logic;
	signal ldqm_next, udqm_next, ldqm_internal, udqm_internal : std_logic_vector(1 downto 0);
	signal cs_next, cs_internal : std_logic_vector(1 downto 0);
	signal cke_next, cke_internal : std_logic;
	 
	signal counter, counter_next : integer range 0 to INIT_STABILIZE_CYCLES;
	signal refresh_counter, refresh_counter_next : integer range 0 to REFRESH_CYCLES*2;
	
	signal bus_ack_internal, bus_ack_next : std_logic;
begin
	bus_q <= (others => 'Z');
	bus_ack <= bus_ack_internal;
	
	--a <= (others => '0');
	--b <= (others => '0');
	
	--ldqm <= "00";
	--udqm <= "00";
	
	--we <= '1';
	--cs <= "11";
	--cke <= '0';
	
	--ras <= '1';
	--cas <= '1';
	
	a <= a_internal;
	b <= b_internal;
	
	ldqm <= ldqm_internal;
	udqm <= udqm_internal;
	
	cs <= cs_internal;
	cke <= cke_internal;
	
	ras <= ras_internal;
	cas <= cas_internal;
	we <= we_internal;
	
	process(state, bus_a, bus_siz, bus_rw, bus_ce, a_internal, b_internal, ldqm_internal, udqm_internal, cs_internal, cke_internal, refresh_counter, counter) is
		type addr_type is record
			bank : std_logic_vector(1 downto 0);
			row : std_logic_vector(12 downto 0);
			col : std_logic_vector(8 downto 0);
		end record;
		
		function vector_to_addr_type(vec : std_logic_vector(31 downto 0)) return addr_type is
			variable ret : addr_type;
			variable count : integer := 2;
		begin
			ret.col := vec(ret.col'length - 1 + count downto count);
			count := count + ret.col'length;
			ret.row := vec(ret.row'length - 1 + count downto count);
			count := count + ret.row'length;
			ret.bank := vec(ret.bank'length - 1 + count downto count);
			
			return ret;
		end function;
		
		variable addr : addr_type;
	begin
		addr := vector_to_addr_type(bus_a);
		
		state_next <= state;	
		a_next <= (others => '0');
		b_next <= (others => '0');
		
		ldqm_next <= ldqm_internal;
		udqm_next <= udqm_internal;
		
		cs_next <= cs_internal;
		cke_next <= cke_internal;
		
		ras_next <= '1';
		cas_next <= '1';
		we_next <= '1';
		
		if refresh_counter /= 0 then
			refresh_counter_next <= refresh_counter - 1;
		else
			refresh_counter_next <= refresh_counter;
		end if;
		
		if counter /= 0 then
			counter_next <= counter - 1;
		else
			counter_next <= counter;
		end if;
		
		bus_ack_next <= '0';
		
		case state is
			when powerdown =>
			
			when init =>
				cke_next <= '1';
				if counter = 0 then
					ras_next <= '0';
					cas_next <= '1';
					we_next <= '0';
					cs_next <= "00";
					a_next(10) <= '1';
					state_next <= config;
					counter_next <= RCD_CYCLES;
				end if;
			
			when config =>
				ras_next <= '1';
				cas_next <= '1';
				we_next <= '1';
				
				if counter = 0 then
					ras_next <= '0';
					cas_next <= '0';
					we_next <= '0';
					
					a_next <= "000" & MODE_WBL & "00" & MODE_CAS_LATENCY & MODE_BT & MODE_BURST_LENGTH;
					b_next <= "00";
					
					state_next <= waitstate;
				end if;
			
			when idle =>
				cs_next <= "11";
				if refresh_counter = 0 then
					ras_next <= '0';
					cas_next <= '0';
					we_next <= '1';
					cs_next <= "00";
					state_next <= refresh;
				elsif bus_ce = '1' then
					state_next <= open_row;
					a_next <= std_logic_vector(resize(unsigned(addr.row), a_next'length));
					b_next <= addr.bank;
					
					ras_next <= '0';
					cas_next <= '1';
					we_next <= '1';
					cs_next <= "00";
					
					ldqm_next <= "11";
					udqm_next <= "11";
					
					counter_next <= RCD_CYCLES;
				end if;
			
			when open_row =>
				if counter = 0 then
					a_next <= "0000" & addr.col; --no auto precharge
					b_next <= addr.bank;
					ras_next <= '1';
					cas_next <= '0';
					
					if bus_siz = "00" then
						--burst
						counter_next <= 3;
					else
						counter_next <= 0;
					end if;
					
					udqm_next(0) <= (bus_siz(1) and (not bus_siz(0)) and (not bus_a(1))) or (bus_siz(1) and (not bus_siz(0)) and bus_a(0)) or ((not bus_siz(1)) and bus_siz(0) and (not bus_a(1)));
					ldqm_next(0) <= (bus_siz(1) and (not bus_siz(0)) and (not bus_a(1))) or (bus_siz(1) and (not bus_siz(0)) and (not bus_a(0))) or ((not bus_siz(1)) and bus_siz(0) and (not bus_a(1)));
					ldqm_next(1) <= (bus_siz(1) and (not bus_siz(0)) and (not bus_a(0))) or (bus_siz(1) and (not bus_siz(0)) and bus_a(1)) or ((not bus_siz(1)) and bus_siz(0) and bus_a(1));
					udqm_next(1) <= (bus_siz(1) and (not bus_siz(0)) and bus_a(0)) or (bus_siz(1) and (not bus_siz(0)) and bus_a(1)) or ((not bus_siz(1)) and bus_siz(0) and bus_a(1));

					if bus_rw = '0' then --read
						state_next <= read_command;
						we_next <= '1';
					else --write
						state_next <= write_command;
						we_next <= '0';
						bus_ack_next <= '1';
					end if;
				end if;
				
			when read_command =>
				bus_ack_next <= '1';
				
				if counter = 0 then
					a_next(10) <= '0';
					b_next <= addr.bank;
					ras_next <= '0';
					cas_next <= '1';
					we_next <= '0';
					state_next <= read_precharge;
					ldqm_next <= "11";
					udqm_next <= "11";
				end if;
			
			when write_command =>
				if counter = 0 then
					a_next(10) <= '0';
					b_next <= addr.bank;
					ras_next <= '0';
					cas_next <= '1';
					we_next <= '0';
					state_next <= write_precharge;
					bus_ack_next <= '0';
					ldqm_next <= "11";
					udqm_next <= "11";
				end if;
			
			when read_precharge =>
				ras_next <= '1';
				cas_next <= '1';
				we_next <= '1';
				cs_next <= "11";
				state_next <= waitstate;
				bus_ack_next <= '1';
				
			when write_precharge =>
				ras_next <= '1';
				cas_next <= '1';
				we_next <= '1';
				cs_next <= "11";
				state_next <= waitstate;
			
			when waitstate =>
				ldqm_next <= "11";
				udqm_next <= "11";
				state_next <= idle;
			
			when refresh =>
				ras_next <= '1';
				cas_next <= '1';
				we_next <= '1';
				cs_next <= "11";
				state_next <= waitstate;
				refresh_counter_next <= REFRESH_CYCLES;
			
			when others =>
		end case;
	end process;
	
	process(reset, clk) is begin
		if reset = '1' then
			a_internal <= (others => '0');
			b_internal <= (others => '0');
			
			ldqm_internal <= "11";
			udqm_internal <= "11";
			
			cs_internal <= "11";
			cke_internal <= '0';
			
			ras_internal <= '1';
			cas_internal <= '1';
			we_internal <= '1';
			
			refresh_counter <= REFRESH_CYCLES*2;
			counter <= INIT_STABILIZE_CYCLES;
			state <= init;
			
			bus_ack_internal <= '0';
		elsif falling_edge(clk) then
			state <= state_next;
			
			a_internal <= a_next;
			b_internal <= b_next;
			
			ldqm_internal <= ldqm_next;
			udqm_internal <= udqm_next;
			
			we_internal <= we_next;
			cs_internal <= cs_next;
			cke_internal <= cke_next;
			
			ras_internal <= ras_next;
			cas_internal <= cas_next;
			
			counter <= counter_next;
			refresh_counter <= refresh_counter_next;
			
			bus_ack_internal <= bus_ack_next;
		elsif rising_edge(clk) then	
			
		end if;
	end process;
end arch;
