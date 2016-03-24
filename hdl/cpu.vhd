library ieee;

entity cpu is
	port(
		reset : in std_logic;
		clk : in std_logic;
		
		tt : in std_logic_vector(1 downto 0);
		tm : in std_logic_vector(2 downto 0);
		siz : in std_logic_vector(1 downto 0);
		rw : in std_logic;
		ts : in std_logic;
		tip : in std_logic;
		ta : out std_logic;
		tea : out std_logic;
		tbi :  out std_logic;
		ipl : out std_logic_vector(2 downto 0);
		bclk : out std_logic;
		lfo : out std_logic;
		scd : in std_logic;
		rsti : out std_logic;
		rsto : in std_logic
	);
end cpu;

architecture arch of cpu is
begin
	ta <= '1';
	tea <= '1';
	tbi <= '1';
	ipl <= "111";
	bclk <= '0';
	lfo <= '0';
	rsti <= '0';
end arch;
