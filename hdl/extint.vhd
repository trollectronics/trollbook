library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use work.wor_logic.all;

entity extint is
	generic(
		peripheral_id : integer range 0 to 31
	);
	port(
		reset : in std_logic;
		clk : in std_logic;
		
		interrupt : in std_logic_vector(3 downto 0);
		
		chipset_a : in std_logic_vector(7 downto 0);
		bus_d : in std_logic_vector(31 downto 0);
		bus_q : out std_logic_vector(31 downto 0);
		bus_rw : in std_logic;
		bus_siz : in std_logic_vector(1 downto 0);
		chipset_ce : in std_logic_vector(15 downto 0);
		chipset_ack : out wor_logic_vector(15 downto 0);
		chipset_nack : out wor_logic_vector(15 downto 0)
	);
end extint;

architecture arch of extint is
	signal interrupt_reg : std_logic_vector(3 downto 0);
begin
	process(reset, clk) begin
		if reset = '1' then
			interrupt_reg <= (others => '1');
		elsif falling_edge(clk) then
			interrupt_reg <= interrupt;
		end if;
	end process;
	
	process(reset, clk) begin
		if reset = '1' then
			bus_q <= (others => 'Z');
		elsif falling_edge(clk) then
			if chipset_ce(peripheral_id) = '1' then
				bus_q <= x"0000000" & interrupt_reg;
			else
				bus_q <= (others => 'Z');	
			end if;
		end if;
	end process;
	
	chipset_ack <= (peripheral_id => '1', others => '0');
	chipset_nack <= (peripheral_id => '0', others => '0');
end arch;
