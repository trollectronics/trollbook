library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity uart is
	port(
		reset : in std_logic;
		clk : in std_logic;
		
		rx : in std_logic;
		tx : out std_logic
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
	signal baud_div : std_logic_vector(15 downto 0) := x"0363"; --9600 baud
	signal rx_full : std_logic;
	signal tx_empty : std_logic;
	signal rx_active : std_logic;
	
	signal tx_buffer : std_logic_vector(7 downto 0) := x"A5";
	signal rx_buffer : std_logic_vector(7 downto 0);
	
	signal tx_internal, tx_next : std_logic;
	signal rx_prev : std_logic;
begin
	process(rxstate, rxcount, rx, rx_prev) begin
		rxcount_next <= (rxcount - 1) mod 4;
		rxstate_next <= rxstate;
		rx_active <= '0';
		
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
				rxstate_next <= idle;
		end case;
	end process;
	
	process(txstate, txcount, tx_internal, tx_buffer) begin
		txcount_next <= (txcount - 1) mod 4;
		txstate_next <= txstate;
		
		tx_next <= tx_internal;
		
		case txstate is
			when idle =>
				txcount_next <= 0;
				tx_next <= '1';
			when start =>
				txstate_next <= bit0;
				tx_next <= '0';
			when bit0 | bit1 | bit2 | bit3 | bit4 | bit5 | bit6 =>
				tx_next <= tx_buffer(state_type'pos(txstate) - state_type'pos(bit0));
				txstate_next <= state_type'succ(txstate);
			when bit7 =>
				tx_next <= tx_buffer(state_type'pos(txstate) - state_type'pos(bit0));
				txstate_next <= stop;
			when parity =>
				tx_next <= '1';
				txstate_next <= stop;
			when stop =>
				tx_next <= '1';
				txstate_next <= idle;
		end case;
	end process;
	
	process(reset, clk) begin
		if reset = '1' then
			rxstate <= idle;
			txstate <= start;
			
			tx_buffer <= x"A5";
			baud_div <=  x"0363";
			
			rx_prev <= '1';
			rx_buffer <= x"FF";
			
			rxcount <= 3;
			txcount <= 3;
			tx_internal <= '1';
			baud_count <= 0;
			
			tx_empty <= '1';
			rx_full <= '0';
		elsif rising_edge(clk) then
			if baud_count = 0 then
				rxcount <= rxcount_next;
				txcount <= txcount_next;
				baud_count <= to_integer(unsigned(baud_div));
				
				rx_prev <= rx;
				
				if rxcount = 0 then
					if rx_active = '1' then
						rx_buffer <= rx_buffer(6 downto 0) & rx;
					end if;
					rxstate <= rxstate_next;
				end if;
				
				if txcount = 0 then
					tx_internal <= tx_next;
					txstate <= txstate_next;
				end if;
			else
				baud_count <= baud_count - 1;
			end if;
		end if;
	end process;
	
	tx <= tx_internal;
end arch;
