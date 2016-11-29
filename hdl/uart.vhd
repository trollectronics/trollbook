library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use work.wor_logic.all;

entity uart is
	generic(
		peripheral_id : integer range 0 to 31
	);
	port(
		reset : in std_logic;
		clk : in std_logic;
		
		rx : in std_logic;
		tx : out std_logic;
		
		chipset_a : in std_logic_vector(7 downto 0);
		bus_d : in std_logic_vector(31 downto 0);
		bus_q : out std_logic_vector(31 downto 0);
		bus_rw : in std_logic;
		bus_siz : in std_logic_vector(1 downto 0);
		chipset_ce : in std_logic_vector(15 downto 0);
		chipset_ack : out wor_logic_vector(15 downto 0);
		chipset_nack : out wor_logic_vector(15 downto 0)
	);
end uart;

architecture arch of uart is
	type state_type is (
		idle, start, bit0, bit1, bit2, bit3, bit4, bit5, bit6, bit7, parity, stop1, stop2
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
	process(rxstate, rxcount, rx, rx_prev, rx_buffer, rx_full, rx_buffer_internal, chipset_ce, bus_rw, chipset_a, baud_count) begin
		rxcount_next <= (rxcount - 1) mod 4;
		rxstate_next <= rxstate;
		rx_active <= '0';
		
		rx_buffer_next <= rx_buffer;
		
		if chipset_ce(peripheral_id) = '1' and bus_rw = '0' and chipset_a(2) = '0' then
			rx_full_next <= '0';
		else
			rx_full_next <= rx_full;
		end if;
		
		case rxstate is
			when idle =>
				rxcount_next <= 0;
				if rx_prev = '0' and rx = '0' and baud_count = 0 then
					rxstate_next <= start;
					rxcount_next <= 3;
				end if;
				
			when start =>
				rx_active <= '1';
				if baud_count = 0 and rxcount = 0 then
					rxstate_next <= bit0;
				end if;
			when bit0 | bit1 | bit2 | bit3 | bit4 | bit5 | bit6 =>
				rx_active <= '1';
				if baud_count = 0 and rxcount = 0 then
					rxstate_next <= state_type'succ(rxstate);
				end if;
			when bit7 =>
				if baud_count = 0 and rxcount = 0 then
					rxstate_next <= stop1;
					end if;
			when parity =>
				if baud_count = 0 and rxcount = 0 then
					rxstate_next <= stop1;
				end if;
			when stop1 =>
				rx_buffer_next <= rx_buffer_internal;
				rx_full_next <= '1';
				rxstate_next <= idle;
			
			when stop2 =>
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
				if tx_empty = '0' then
					txstate_next <= start;
					tx_next <= '0';
				end if;
			when start =>
				txstate_next <= bit0;
				tx_next <= tx_buffer_internal(0);
			when bit0 | bit1 | bit2 | bit3 | bit4 | bit5 | bit6 =>
				tx_next <= tx_buffer_internal(state_type'pos(txstate) + 1 - state_type'pos(bit0));
				txstate_next <= state_type'succ(txstate);
			when bit7 =>
				tx_next <= '1';
				txstate_next <= stop1;
			when parity =>
				tx_next <= '1';
				txstate_next <= stop1;
			when stop1 =>
				tx_next <= '1';
				txstate_next <= stop2;
			when stop2 =>
				tx_next <= '1';
				txstate_next <= idle;
				tx_empty_next <= '1';
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
		elsif falling_edge(clk) then
			if baud_count = 0 then
				rxcount <= rxcount_next;
				txcount <= txcount_next;
				baud_count <= to_integer(unsigned(baud_div));
				
				rx_prev <= rx;
				
				tx_buffer_internal <= tx_buffer_next;
				
				if rxcount = 0 then
					if rx_active = '1' then
						rx_buffer_internal <= rx & rx_buffer_internal(7 downto 1);
					end if;
				end if;
				
				if txcount = 0 then
					tx_empty <= tx_empty_next;
					tx_internal <= tx_next;
					txstate <= txstate_next;
					if txstate_next = idle then
						txcount <= 0;
					elsif txstate_next = start then
						txcount <= 3;
					end if;
				end if;
			else
				baud_count <= baud_count - 1;
			end if;
			
			rxstate <= rxstate_next;
			rx_buffer <= rx_buffer_next;
			rx_full <= rx_full_next;
			
			if chipset_ce(peripheral_id) = '1' then
				if bus_rw = '1' then
					case chipset_a(2) is
						when '0' =>
							tx_buffer_internal <= bus_d(7 downto 0);
							tx_empty <= '0';
							--txstate <= start;
							--txcount <= 0;
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
	
	process(chipset_ce, rx_buffer, chipset_a, baud_div, rx_active, rx_full, tx_empty) begin
		if chipset_ce(peripheral_id) = '1' then
			if chipset_a(2) = '0' then
				bus_q <= x"000000" & rx_buffer;
			else
				bus_q <= baud_div & "0000000000000" & rx_active & rx_full & tx_empty;
			end if;
		else
			bus_q <= (others => 'Z');
		end if;
	end process;
	
	
	chipset_ack <= (peripheral_id => '1', others => '0');
	chipset_nack <= (peripheral_id => '0', others => '0');
	
	tx <= tx_internal;
end arch;
