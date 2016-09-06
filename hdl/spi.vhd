library ieee;
use ieee.std_logic_1164.all;

entity spi is
	port(
		reset : in std_logic;
		clk : in std_logic;
		
		miso : in std_logic;
		mosi : out std_logic;
		sck : out std_logic;
		ss : out std_logic_vector(2 downto 0);

		bus_a: in std_logic_vector(31 downto 0);
		bus_d: in std_logic_vector(31 downto 0);
		bus_q: out std_logic_vector(31 downto 0);
		bus_rw: in std_logic;
		bus_siz: in std_logic_vector(1 downto 0);
		bus_ce: in std_logic;
		bus_ack: out std_logic
	);
end spi;

architecture arch of spi is
	signal clk_div: std_logic;
	signal ce_reg: std_logic_vector(2 downto 0);
	signal data_in: std_logic_vector(7 downto 0);
	signal data_out: std_logic_vector(7 downto 0);
	signal data_internal: std_logic_vector(7 downto 0);
	signal miso_flop: std_logic;

	type state_type is (
		idle, buff, bit0, bit1, bit2, bit3, bit4, bit5, bit6, bit7, ack;
	);
	
	signal transstate, transstate_next: state_type;
	signal sendstate, sendstate_next: state_type;
begin
	mosi <= '1';
	sck <= '0';
	ss <= ce_reg;
	bus_ack <= '0';
	q <= x"000000" & data_in;
	d <= x"000000" & data_out;
	mosi <= data_out(7);
	sendstate_next <= idle;
	transstate_next <= idle;

	process (clk) begin
		if rising_edge(clk) then
			clk_div = not clk_div;
		end if;
	end process;
	
	process (transstate, data_in, miso_flop) begin
		case transstate is
			when ack =>
				transstate_next <= idle;
				ack <= '1';
			when idle =>
				if (bus_ce = '1' and rw = '1' and bus_a(2) = '1') then
					transstate_next <= buff;
					data_internal <= miso_flop & data_in(6 downto 0);
				end if;
			when buff =>
				data_internal <= miso_flop & data_in(6 downto 0);
				sendstate_next <= bit0;
				transstate_next <= bit0;
			when bit0 =>
				data_internal <= data_in(7) & miso_flop & data_in(5 downto 0);
				transstate_next <= bit1;
			when bit1 =>
				data_internal <= data_in(7 downto 6) & miso_flop & data_in(4 downto 0);
				transstate_next <= bit2;
			when bit2 =>
				data_internal <= data_in(7 downto 5) & miso_flop & data_in(3 downto 0);
				transstate_next <= bit3;
			when bit3 =>
				data_internal <= data_in(7 downto 4) & miso_flop & data_in(2 downto 0);
				transstate_next <= bit4;
			when bit4 =>
				data_internal <= data_in(7 downto 3) & miso_flop & data_in(1 downto 0);
				transstate_next <= bit5;
			when bit5 =>
				data_internal <= data_in(7 downto 2) & miso_flop & data_in(0);
				transstate_next <= bit6;
			when bit6 =>
				data_internal <= data_in(7 downto 1) & miso_flop;
				transstate_next <= ack;
			when others =>
		end case:
	end process;

	process (sendstate, data_out) begin
		case sendstate is 
			when bit0 =>
				mosi <= data_out(6);
				sendstate_next <= bit1;
			when bit1 =>
				mosi <= data_out(5);
				sendstate_next <= bit2;
			when bit2 =>
				mosi <= data_out(4);
				sendstate_next <= bit3;
			when bit3 =>
				mosi <= data_out(3);
				sendstate_next <= bit4;
			when bit4 =>
				mosi <= data_out(2);
				sendstate_next <= bit5;
			when bit5 =>
				mosi <= data_out(1);
				sendstate_next <= bit6;
			when bit6 =>
				mosi <= data_out(0);
				sendstate_next <= idle;
			others =>
		end case;
	end process;

	process (bus_a, bus_rw) begin
		if (bus_rw = '0') then
			bus_ack <= '1';
		elsif (bus_a(2) = '0' then
			bus_ack <= '1';
			if (rising_edge(clk)) then
				ce_reg <= x"0000000" & '0' & bus_d(2 downto 0);
			end if;
		end if;
	end process;

	process (sendstate, clk_div) begin
		case buff | bit0 | bit1 | bit2 | bit3 | bit4 | bit5 | bit6 is
			sck <= clk_div;
		others =>
	end process;

	process(reset, clk_div) begin
		if reset = '1' then
			ce_reg <= "000";
			data_in <= x"FF";
			data_out <= x"FF";
			transstate <= idle;
			sendstate <= idle;
			mosi_flop <= '1';
		elsif rising_edge(clk_div) then
			transstate <= transstate_next;
			data_in <= data_internal;
		elsif falling_edge(clk_div) then
			mosi_flop <= mosi;
			sendstate <= sendstate_next;
		end if;
	end process;
end arch;
