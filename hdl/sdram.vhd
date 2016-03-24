library ieee;
use ieee.std_logic_1164.all;

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
		cke : out std_logic
	);
end sdram;

architecture arch of sdram is
begin
	a <= (others => '0');
	b <= (others => '0');
	
	ldqm <= "00";
	udqm <= "00";
	
	we <= '1';
	cs <= "11";
	cke <= '0';
	
	ras <= '1';
	cas <= '1';
end arch;
