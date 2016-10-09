library ieee;
use ieee.std_logic_1164.all;

package wor_logic is
	function resolve_wor(s : std_ulogic_vector) return std_ulogic;

	type stdlogic_table is array(std_ulogic, std_ulogic) of std_ulogic;
	constant resolution_table : stdlogic_table := (
	--      ---------------------------------------------------------
	--      |  U    X    0    1    Z    W    L    H    -        |   |  
	--      ---------------------------------------------------------
		( 'U', 'U', 'U', 'U', 'U', 'U', 'U', 'U', 'U' ), -- | U |
		( 'U', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X' ), -- | X |
		( 'U', 'X', '0', '1', '0', '0', '0', '0', 'X' ), -- | 0 |
		( 'U', 'X', '1', '1', '1', '1', '1', '1', 'X' ), -- | 1 |
		( 'U', 'X', '0', '1', 'Z', 'W', 'L', 'H', 'X' ), -- | Z |
		( 'U', 'X', '0', '1', 'W', 'W', 'W', 'W', 'X' ), -- | W |
		( 'U', 'X', '0', '1', 'L', 'W', 'L', 'W', 'X' ), -- | L |
		( 'U', 'X', '0', '1', 'H', 'W', 'W', 'H', 'X' ), -- | H |
		( 'U', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X' )  -- | - |
	);


	subtype wor_logic is resolve_wor std_ulogic;
	type wor_logic_vector is array ( natural range <>) of wor_logic;
end package;

package body wor_logic is
	function resolve_wor(s : std_ulogic_vector) return std_ulogic is
		variable result : std_ulogic := 'Z';  -- weakest state default
	begin
		if  s'length = 1 then
			return s(s'low);
		else
			for i in s'range loop
			result := resolution_table(result, s(i));
			end loop;
		end if;
		return result;
	end resolve_wor;
end wor_logic;
