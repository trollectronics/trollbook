library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity llram_bus_adapter is
	port(
		reset : in std_logic;
		clk : in std_logic;
		
		ll_a : out std_logic_vector(17 downto 0);
		ll_d : in std_logic_vector(15 downto 0);
		ll_q : out std_logic_vector(15 downto 0);
		ll_rw : out std_logic;
		ll_lb : out std_logic;
		ll_ub : out std_logic;
		ll_ce : out std_logic;
		ll_ack : in std_logic;
		
		bus_a : in std_logic_vector(31 downto 0);
		bus_d : in std_logic_vector(31 downto 0);
		bus_q : out std_logic_vector(31 downto 0);
		bus_rw : in std_logic;
		bus_siz : in std_logic_vector(1 downto 0);
		bus_ce : in std_logic;
		bus_ack : out std_logic
	);
end llram_bus_adapter;

architecture arch of llram_bus_adapter is
	type state_type is (idle, done,
		word_read, long_read_first, long_read_second,
		word_read_and_a_half, long_read_first_and_a_half, long_read_second_and_a_half,
		word_write, long_write_first, long_write_second);
	
	signal state, state_next : state_type;
	
	signal ll_lb_internal, ll_lb_next : std_logic;
	signal ll_ub_internal, ll_ub_next : std_logic;
	signal ll_a0_next, ll_a0_internal : std_logic;
	signal ll_ce_next, ll_ce_internal : std_logic;
	signal ll_rw_next : std_logic;
	
	signal bus_q_internal, bus_q_next : std_logic_vector(bus_q'range);
	signal bus_ack_next, bus_ack_internal : std_logic;
	
	type buffer_rec is record
		addr : std_logic_vector(17 downto 0);
		data : std_logic_vector(15 downto 0);
		siz : std_logic_vector(1 downto 0);
	end record;
	
	type buffer_entry is array(natural range <>) of buffer_rec;
	
	signal write_buffer : buffer_entry(3 downto 0);
	signal buffer_pos : unsigned(1 downto 0);
begin
	bus_q <= bus_q_internal when bus_ce = '1' else (others => 'Z');
	ll_lb <= ll_lb_internal;
	ll_ub <= ll_ub_internal;
	ll_ce <= ll_ce_internal;
	
	ll_q <= bus_d(15 downto 0) when ll_a0_internal = '1' else bus_d(31 downto 16);
	
	process(state, bus_rw, bus_siz, bus_q_internal, ll_lb_internal, ll_ub_internal, ll_a0_internal, ll_ce_internal,
		bus_a, bus_ce, ll_d, ll_ack) is
		variable check : std_logic_vector(2 downto 0);
	begin
		check := bus_rw & bus_siz;
		state_next <= state;
		bus_q_next <= bus_q_internal;
		ll_lb_next <= ll_lb_internal;
		ll_ub_next <= ll_ub_internal;
		ll_a0_next <= ll_a0_internal;
		ll_ce_next <= ll_ce_internal;
		ll_rw_next <= '0';
		
		bus_ack_next <= '0';
		
		case state is
			when idle =>
				ll_lb_next <= '0';
				ll_ub_next <= '0';
				ll_a0_next <= '0';
				ll_ce_next <= '0';
				
				if bus_ce = '1' then
					case check is
						--read
						when "000" => --line
						when "001" => --word
							state_next <= word_read;
							ll_lb_next <= '1';
							ll_ub_next <= '1';
							ll_a0_next <= bus_a(1);
							ll_ce_next <= '1';
						when "010" => --byte
							state_next <= word_read;
							ll_lb_next <= bus_a(0);
							ll_ub_next <= not bus_a(0);
							ll_a0_next <= bus_a(1);
							ll_ce_next <= '1';
						when "011" => --long
							state_next <= long_read_first;
							ll_lb_next <= '1';
							ll_ub_next <= '1';
							ll_a0_next <= '0';
							ll_ce_next <= '1';
						
						--write
						when "100" => --line
						when "101" => --word
							state_next <= word_write;
							ll_lb_next <= '1';
							ll_ub_next <= '1';
							ll_a0_next <= bus_a(1);
							ll_ce_next <= '1';
							ll_rw_next <= '1';
						when "110" => --byte
							state_next <= word_write;
							ll_lb_next <= bus_a(0);
							ll_ub_next <= not bus_a(0);
							ll_a0_next <= bus_a(1);
							ll_ce_next <= '1';
							ll_rw_next <= '1';
						when "111" => --long
							state_next <= long_write_first;
							ll_lb_next <= '1';
							ll_ub_next <= '1';
							ll_a0_next <= '0';
							ll_ce_next <= '1';
							ll_rw_next <= '1';
						when others =>
					end case;
				end if;
			
			when done =>
				ll_lb_next <= '0';
				ll_ub_next <= '0';
				ll_a0_next <= '0';
				ll_ce_next <= '0';
				--bus_ack_next <= '1';
				
				if bus_ce = '0' then
					bus_ack_next <= '0';
					state_next <= idle;
				end if;
			
			when word_read =>
				if ll_ack = '1' then
					state_next <= word_read_and_a_half;
					bus_ack_next <= '1';
					ll_ce_next <= '0';
					if bus_a(1) = '0' then
						bus_q_next <= ll_d & x"0000";
					else
						bus_q_next <= x"0000" & ll_d;
					end if;
				end if;
			when word_read_and_a_half =>
				if bus_a(1) = '0' then
					bus_q_next <= ll_d & x"0000";
				else
					bus_q_next <= x"0000" & ll_d;
				end if;
				
				state_next <= done;
				
			when long_read_first =>
				if ll_ack = '1' then
					state_next <= long_read_first_and_a_half;
					bus_q_next <= ll_d & bus_q_internal(15 downto 0);
				end if;
				
			when long_read_first_and_a_half =>
				bus_q_next <= ll_d & bus_q_internal(15 downto 0);
				state_next <= long_read_second;
				ll_a0_next <= '1';
			
			when long_read_second =>
				if ll_ack = '1' then
					state_next <= long_read_second_and_a_half;
					bus_ack_next <= '1';
					ll_ce_next <= '0';
					bus_q_next <= bus_q_internal(31 downto 16) & ll_d;
				end if;
			
			when long_read_second_and_a_half =>
				bus_q_next <= bus_q_internal(31 downto 16) & ll_d;
				state_next <= done;
			
			when word_write =>
				ll_rw_next <= '1';
				if ll_ack = '1' then
					state_next <= done;
					bus_ack_next <= '1';
					ll_ce_next <= '0';
					ll_rw_next <= '0';
				end if;
			when long_write_first =>
				ll_rw_next <= '1';
				if ll_ack = '1' then
					ll_a0_next <= '1';
					state_next <= long_write_second;
					ll_ce_next <= '1';
					ll_rw_next <= '1';
				end if;
			when long_write_second =>
				ll_rw_next <= '1';
				if ll_ack = '1' then
					state_next <= done;
					bus_ack_next <= '1';
					ll_ce_next <= '0';
					ll_rw_next <= '0';
				end if;
		end case;
	end process;
	
	process(clk, reset) begin
		if reset = '1' then
			ll_lb_internal <= '0';
			ll_ub_internal <= '0';
			ll_a <= (others => '0');
			ll_a0_internal <= '0';
			ll_ce_internal <= '0';
			ll_rw <= '0';
			bus_ack <= '0';
			bus_ack_internal <= '0';
			bus_q_internal <= (others => '0');
			state <= idle;
		elsif falling_edge(clk) then
			ll_lb_internal <= ll_lb_next;
			ll_ub_internal <= ll_ub_next;
			ll_a0_internal <= ll_a0_next;
			ll_a <= bus_a(18 downto 2) & ll_a0_next;
			ll_ce_internal <= ll_ce_next;
			ll_rw <= ll_rw_next;
			
			bus_ack_internal <= bus_ack_next;
			bus_ack <= bus_ack_internal;
			
			state <= state_next;
		elsif rising_edge(clk) then
			bus_q_internal <= bus_q_next;
		end if;
	end process;
end arch;
