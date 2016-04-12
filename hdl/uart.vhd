library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity uart is
	port(
		reset : in std_logic;
		clk : in std_logic;
		
		rx : in std_logic;
		tx : out std_logic;
		
		bus_a : in std_logic_vector(31 downto 0);
		bus_d : in std_logic_vector(31 downto 0);
		bus_q : out std_logic_vector(31 downto 0);
		bus_rw : in std_logic;
		bus_siz : in std_logic_vector(1 downto 0);
		bus_ce : in std_logic;
		bus_ack : out std_logic
	);
end uart;

architecture arch of uart is
	type state_type is (
		idle, start, bit0, bit1, bit2, bit3, bit4, bit5, bit6, bit7, parity, stop
	);
	
	signal rxstate, rxstate_next : state_type;
	signal txstate, txstate_next : state_type;
	
	signal rxcount, rxcount_next : integer range 0 to 3;
	signal txcount, txcount_next : integer range 0 to 3;
	
	signal baud_count : integer range 0 to (2 ** 16 - 1);
	signal baud_count_next : integer range 0 to (2 ** 16 - 1);
	signal baud_div : std_logic_vector(15 downto 0);
	signal rx_full, rx_full_next : std_logic;
	signal tx_empty, tx_empty_next : std_logic;
	signal rx_active : std_logic;
	
	signal tx_buffer_internal, tx_buffer_next : std_logic_vector(7 downto 0);
	signal rx_buffer_internal, rx_buffer, rx_buffer_next : std_logic_vector(7 downto 0);
	
	signal tx_internal, tx_next : std_logic;
	signal rx_prev : std_logic;
begin
	process(rxstate, rxcount, rx, rx_prev, rx_buffer, rx_full, rx_buffer_internal) begin
		rxcount_next <= (rxcount - 1) mod 4;
		rxstate_next <= rxstate;
		rx_active <= '0';
		
		rx_buffer_next <= rx_buffer;
		rx_full_next <= rx_full;
		
		case rxstate is
			when idle =>
				rxcount_next <= 0;
				
				if rx_prev = '0' and rx = '0' then
					rxstate_next <= start;
					rxcount_next <= 3;
				end if;
				
			when start =>
				rxstate_next <= bit0;
				rx_active <= '1';
			when bit0 | bit1 | bit2 | bit3 | bit4 | bit5 | bit6 =>
				rxstate_next <= state_type'succ(rxstate);
				rx_active <= '1';
			when bit7 =>
				rxstate_next <= stop;
			when parity =>
				rxstate_next <= stop;
			when stop =>
				rxcount_next <= 0;
				rx_buffer_next <= rx_buffer_internal;
				rx_full_next <= '1';
				rxstate_next <= idle;
		end case;
	end process;
	
	process(txstate, txcount, tx_internal, tx_buffer_internal, tx_empty) begin
		txcount_next <= (txcount - 1) mod 4;
		txstate_next <= txstate;
		
		tx_next <= tx_internal;
		tx_buffer_next <= tx_buffer_internal;
		tx_empty_next <= tx_empty;
		
		case txstate is
			when idle =>
				txcount_next <= 0;
				tx_next <= '1';
			when start =>
				txstate_next <= bit0;
				tx_next <= '0';
			when bit0 | bit1 | bit2 | bit3 | bit4 | bit5 | bit6 =>
				tx_next <= tx_buffer_internal(state_type'pos(txstate) - state_type'pos(bit0));
				txstate_next <= state_type'succ(txstate);
			when bit7 =>
				tx_next <= tx_buffer_internal(state_type'pos(txstate) - state_type'pos(bit0));
				txstate_next <= stop;
			when parity =>
				tx_next <= '1';
				txstate_next <= stop;
			when stop =>
				tx_empty_next <= '1';
				tx_next <= '1';
				txstate_next <= idle;
		end case;
	end process;
	
	process(reset, clk) begin
		if reset = '1' then
			rxstate <= idle;
			txstate <= idle;
			
			tx_buffer_internal <= x"FF";
			--(33 MHz)/(baud*4)
			--baud_div <= x"0363"; --9600 baud
			baud_div <=  x"0049"; --115200 baud
			
			tx_internal <= '1';
			
			rx_prev <= '1';
			rx_buffer_internal <= x"FF";
			rx_buffer <= x"FF";
			
			rxcount <= 3;
			txcount <= 3;
			baud_count <= 0;
			
			tx_empty <= '1';
			rx_full <= '0';
		elsif rising_edge(clk) then
			if baud_count = 0 then
				rxcount <= rxcount_next;
				txcount <= txcount_next;
				baud_count <= to_integer(unsigned(baud_div));
				
				rx_prev <= rx;
				
				rx_buffer <= rx_buffer_next;
				tx_buffer_internal <= tx_buffer_next;
				
				if rxcount = 0 then
					rx_full <= rx_full_next;
					if rx_active = '1' then
						rx_buffer_internal <= rx & rx_buffer_internal(7 downto 1);
					end if;
					rxstate <= rxstate_next;
				end if;
				
				if txcount = 0 then
					tx_empty <= tx_empty_next;
					tx_internal <= tx_next;
					txstate <= txstate_next;
				end if;
			else
				baud_count <= baud_count - 1;
			end if;
			
			if bus_ce = '1' then
				if bus_rw = '0' and bus_a(2) = '0' then
					rx_full <= '0';
				elsif bus_rw = '1' then
					case bus_a(2) is
						when '0' =>
							tx_buffer_internal <= bus_d(7 downto 0);
							tx_empty <= '0';
							txstate <= start;
							txcount <= 3;
						when '1' =>
							baud_div <= bus_d(31 downto 16);
							txstate <= idle;
							rxstate <= idle;
							rx_prev <= '1';
							txcount <= 3;
							rxcount <= 3;
						when others =>
					end case;
				end if;
			end if;
		end if;
	end process;
	
	process(bus_ce, rx_buffer, bus_a, baud_div, rx_active, rx_full, tx_empty) begin
		if bus_ce = '1' then
			if bus_a(2) = '0' then
				bus_q <= x"000000" & rx_buffer;
			else
				bus_q <= baud_div & "0000000000000" & rx_active & rx_full & tx_empty;
			end if;
		else
			bus_q <= (others => 'Z');
		end if;
	end process;
	
	tx <= tx_internal;
	bus_ack <= '1';
end arch;
