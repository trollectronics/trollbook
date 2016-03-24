library ieee;

entity llram is
	port(
		reset : in std_logic;
		clk : in std_logic;
		
		a : out std_logic_vector(18 downto 1);
		d : inout std_logic_vector(15 downto 0);
		ce : out std_logic;
		we : out std_logic;
		lb : out std_logic;
		ub : out std_logic;
		oe : out std_logic
	);
end llram;

architecture arch of llram is
begin
	a <= (others => '0');
	
	oe <= '1':
	ce <= '1';
	
	we <= '1';
	ub, lb <= "11";
end arch;
