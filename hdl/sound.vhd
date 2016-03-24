library ieee;
use ieee.std_logic_1164.all;

entity sound is
	port(
		reset : in std_logic;
		clk : in std_logic;
		
		mosi : out std_logic;
		sck : out std_logic;
		ss : out std_logic;
		sync : out std_logic
	);
end sound;

architecture arch of sound is
begin
	ss <= '1';
	mosi <= '1';
	sck <= '0';
	sync <= '0';
end arch;
