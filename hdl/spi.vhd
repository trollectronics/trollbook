library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity spi is
	port(
		reset : in std_logic;
		clk : in std_logic;
		
		miso : in std_logic;
		mosi : out std_logic;
		sck : out std_logic;
		ss : out std_logic_vector(2 downto 0);
		
		bus_a : in std_logic_vector(31 downto 0);
		bus_d : in std_logic_vector(31 downto 0);
		bus_q : out std_logic_vector(31 downto 0);
		bus_rw : in std_logic;
		bus_siz : in std_logic_vector(1 downto 0);
		bus_ce : in std_logic;
		bus_ack : out std_logic
	);
end spi;

architecture arch of spi is
	type state_type is (
		idle, start, bit7, bit6, bit5, bit4, bit3, bit2, bit1, bit0
	);
	
	signal state, state_next : state_type;
	
	signal count : integer range 0 to (2 ** 16 - 1);
	signal count_next : integer range 0 to (2 ** 16 - 1);
	signal baud_div : std_logic_vector(15 downto 0);
	
	signal mosi_buffer, miso_buffer, miso_buffer_next : std_logic_vector(7 downto 0);
	signal mosi_internal, mosi_next : std_logic;
	
	signal ss_internal : std_logic_vector(2 downto 0);
	
	signal busy, busy_next : std_logic;
	
	signal sck_next, sck_internal : std_logic;
begin
	
	ss <= ss_internal;
	sck <= sck_internal;
	mosi <= mosi_internal;
	bus_ack <= '1';
	
	process(state, count, baud_div, miso_buffer, mosi_internal, busy, sck_internal, mosi_buffer, miso) begin
		if count = to_integer(unsigned(baud_div)) then
			count_next <= 0;
		else
			count_next <= count + 1;
		end if;
		
		if count = to_integer(unsigned(baud_div))/2 then
			sck_next <= '0';
		elsif count = to_integer(unsigned(baud_div)) then
			sck_next <= '1';
		else
			sck_next <= sck_internal;
		end if;
		
		state_next <= state;
		miso_buffer_next <= miso_buffer;
		mosi_next <= mosi_internal;
		busy_next <= busy;
		
		case state is
			when idle =>
				count_next <= 0;
				mosi_next <= '1';
				busy_next <= '0';
				if busy = '1' then
					count_next <= to_integer(unsigned(baud_div));
					state_next <= start;
					mosi_next <= mosi_buffer(7);
					busy_next <= '1';
				end if;
			when start =>
				mosi_next <= mosi_buffer(7);
				if count = to_integer(unsigned(baud_div)) then
					miso_buffer_next <= miso_buffer(6 downto 0) & miso;
					state_next <= bit7;
				end if;
			when bit7 | bit6 | bit5 | bit4 | bit3 | bit2 | bit1 =>
				mosi_next <= mosi_buffer(7 - (state_type'pos(state) + 1 - state_type'pos(bit7)));
				if count = to_integer(unsigned(baud_div)) then
					miso_buffer_next <= miso_buffer(6 downto 0) & miso;
					state_next <= state_type'succ(state);
				end if;
			when bit0 =>
				mosi_next <= '1';
				busy_next <= '0';
				state_next <= idle;
				count_next <= to_integer(unsigned(baud_div))/2;
		end case;
	end process;
	
	process(reset, clk, baud_div) is
		variable check : std_logic_vector(1 downto 0);
	begin
		if reset = '1' then
			state <= idle;
			
			baud_div <=  x"0001";
			count <= 0;
			busy <= '0';
			ss_internal <= (others => '0');
			sck_internal <= '0';
			
			miso_buffer <= (others => '1');
			mosi_buffer <= (others => '1');
			mosi_internal <= '1';
		elsif rising_edge(clk) then
			check := bus_ce & bus_rw;
			
			state <= state_next;
			count <= count_next;
			busy <= busy_next;
			
			
			
			case check is
				when "11" =>
					case bus_a(3 downto 2) is
						when "00" =>
							mosi_buffer <= bus_d(7 downto 0);
							busy <= '1';
						when "01" =>
							baud_div <= bus_d(31 downto 16);
							state <= idle;
							busy <= '0';
							count <= 0;
						when "10" =>
							state <= idle;
							busy <= '0';
							count <= 0;
							
							ss_internal <= bus_d(2 downto 0);
						when others =>
						
					end case;
					
				when others =>
			end case;
			
			sck_internal <= sck_next;
			miso_buffer <= miso_buffer_next;
			mosi_internal <= mosi_next;
		elsif falling_edge(clk) then
			
		end if;
	end process;
	
	process(bus_ce, bus_a, miso_buffer, busy, ss_internal, baud_div) begin
		bus_q <= (others => 'Z');
		
		if bus_ce = '1' then
			case bus_a(3 downto 2) is
				when "00" =>
					bus_q <= x"000000" & miso_buffer;
				when "01" =>
					bus_q <= baud_div & "000000000000000" & busy;
				when "10" =>
					bus_q <= x"000000" & "00000" & ss_internal;
				when others =>
			end case;
		end if;
	end process;
end arch;
