library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use work.wor_logic.all;

entity chipset is
	port(
		reset : in std_logic;
		clk : in std_logic;
		
		bus_a : in std_logic_vector(31 downto 0);
		bus_ce_chipset : in std_logic;
		bus_ack_chipset : out std_logic;
		bus_nack_chipset : out std_logic;
		bus_d : in std_logic_vector(31 downto 0);
		bus_q : out std_logic_vector(31 downto 0);
		bus_rw : in std_logic;
		bus_siz : in std_logic_vector(1 downto 0);
		
		chipset_a : out std_logic_vector(7 downto 0);
		chipset_d : in std_logic_vector(31 downto 0);
		chipset_q : out std_logic_vector(31 downto 0);
		chipset_rw : out std_logic;
		chipset_siz : out std_logic_vector(1 downto 0);
		
		chipset_ce : out std_logic_vector(15 downto 0);
		chipset_ack : in wor_logic_vector(15 downto 0);
		chipset_nack : in wor_logic_vector(15 downto 0)
	);
end chipset;

architecture arch of chipset is
	signal ack : std_logic_vector(1 downto 0);
begin
	--process(bus_ce_chipset, bus_a, chipset_ack, chipset_nack) is
	--begin
		
	--end process;
	
	process(reset, clk) begin
		if reset = '1' then
			bus_ack_chipset <= '0';
			bus_nack_chipset <= '0'; --TODO: fix
			chipset_ce <= (others => '0');
			ack <= (others => '0');
		elsif falling_edge(clk) then
			if bus_ce_chipset = '1'  then
				bus_ack_chipset <= ack(1);
				ack(0) <= '1';
				ack(1) <= ack(0);
				bus_q <= chipset_d;
			else
				bus_ack_chipset <= '0';
				ack <= (others => '0');
				bus_q <= (others => 'Z');
			end if;
			
			chipset_a <= bus_a(7 downto 0);
			chipset_rw <= bus_rw;
			chipset_siz <= bus_siz;
			chipset_q <= bus_d;
			
			chipset_ce <= (others => '0');
			bus_nack_chipset <= '0'; --TODO: fix
			--bus_ack_chipset <= '0';
			
			if bus_ce_chipset = '1' then
				for i in 0 to 15 loop
					if i = to_integer(unsigned(bus_a(11 downto 8))) then
						chipset_ce(i) <= '1';
						--bus_ack_chipset <= chipset_ack(i);
						--bus_nack_chipset <= chipset_nack(i);
					end if;
				end loop;
			end if;
		
		end if;
	end process;
	
end arch;
