library ieee;
use ieee.std_logic_1164.all;

entity spi is
	port(
		reset : in std_logic;
		clk : in std_logic;
		
		miso : in std_logic;
		mosi : out std_logic;
		sck : out std_logic;
		ss : out std_logic_vector(2 downto 0)
	);
end spi;

architecture arch of spi is
begin
	mosi <= '1';
	sck <= '0';
	ss <= "000";
end arch;
