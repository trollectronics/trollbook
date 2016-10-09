library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use work.wor_logic.all;

entity chipset is
	port(
		bus_a : in std_logic_vector(31 downto 0);
		bus_ce_chipset : in std_logic;
		bus_ack_chipset : out std_logic;
		bus_nack_chipset : out std_logic;
		
		chipset_ce : out std_logic_vector(15 downto 0);
		chipset_ack : in wor_logic_vector(15 downto 0);
		chipset_nack : in wor_logic_vector(15 downto 0)
	);
end chipset;

architecture arch of chipset is
	
begin
	process(bus_ce_chipset, bus_a, chipset_ack, chipset_nack) is
	begin
		chipset_ce <= (others => '0');
		bus_ack_chipset <= '1'; --TODO: fix
		bus_nack_chipset <= '0';
		
		if bus_ce_chipset = '1' then
			for i in 0 to 15 loop
				if i = to_integer(unsigned(bus_a(11 downto 8))) then
					chipset_ce(i) <= '1';
					--bus_ack_chipset <= chipset_ack(i);
					--bus_nack_chipset <= chipset_nack(i);
				end if;
			end loop;
		end if;
	end process;
end arch;
