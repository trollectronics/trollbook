library ieee;
use ieee.std_logic_1164.all;

entity uart is
	port(
		reset : in std_logic;
		clk : in std_logic;
		
		rx : in std_logic;
		tx : out std_logic
	);
end uart;

architecture arch of uart is
begin
	tx <= '1';
end arch;
