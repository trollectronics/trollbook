library ieee;
use ieee.std_logic_1164.all;

entity reset is
	port(
		clk : in std_logic;
		pwron_reset : in std_logic;
		reset : out std_logic
	);
end reset;

architecture arch of reset is
	constant reset_cycles : integer := 100;
	
	signal state : integer range 0 to reset_cycles + 1;
begin
	process(pwron_reset, clk) is begin
		if pwron_reset = '0' then
			state <= 0;
		elsif rising_edge(clk) then
			if(state /= reset_cycles) then
				state <= state + 1;
			end if;
		end if;
	end process;
	
	reset <= '0' when state = reset_cycles else '1';
end arch;
