library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use work.wor_logic.all;

entity interrupt is
	generic(
		peripheral_id : integer range 0 to 15
	);
	port(
		reset : in std_logic;
		clk : in std_logic;
		
		external_interrupt : in std_logic_vector(3 downto 0);
		
		chipset_a : in std_logic_vector(7 downto 0);
		bus_d : in std_logic_vector(31 downto 0);
		bus_q : out std_logic_vector(31 downto 0);
		bus_rw : in std_logic;
		bus_siz : in std_logic_vector(1 downto 0);
		chipset_ce : in std_logic_vector(15 downto 0);
		chipset_ack : out wor_logic_vector(15 downto 0);
		chipset_nack : out wor_logic_vector(15 downto 0);
		chipset_int : in std_logic_vector(15 downto 0);
		
		cpu_interrupt_level : out std_logic_vector(2 downto 0)
	);
end interrupt;

architecture arch of interrupt is
	constant INTERRUPT_AVAILBLE : std_logic_vector(31 downto 0) := x"000FFFFF";
	
	signal interrupt_reg : std_logic_vector(3 downto 0);
	
	type flag_type is array(natural range <>) of std_logic;
	type priority_type is array(natural range <>) of std_logic_vector(2 downto 0);
	signal flag : flag_type(19 downto 0);
	signal flag_next : flag_type(19 downto 0);
	signal priority : priority_type(19 downto 0);
	
	signal cpu_interrupt_level_internal : std_logic_vector(2 downto 0);
begin
	cpu_interrupt_level <= cpu_interrupt_level_internal;
	
	process(flag, chipset_int, external_interrupt, priority) begin
		for i in 0 to 31 loop
			if INTERRUPT_AVAILBLE(i) = '1' then
				flag_next(i) <= flag(i);
			end if;
		end loop;
		
		for i in 0 to 15 loop
			if chipset_int(i) = '1' then
				flag_next(i) <= '1';
			end if;
		end loop;
		
		for i in 0 to 3 loop
			if external_interrupt(i) = '0' then
				flag_next(i + 16) <= '1';
			end if;
		end loop;
	end process;
	
	process(reset, clk) is
		variable level : std_logic_vector(2 downto 0);
		variable bits : unsigned(7 downto 0);
	begin
		if reset = '1' then
			cpu_interrupt_level_internal <= (others => '0');
		elsif falling_edge(clk) then
			level := (others => '0');
			bits := (others => '0');
			
			for i in 31 downto 1 loop
				if INTERRUPT_AVAILBLE(i) = '1' then
					if flag(i) = '1' then
						bits := bits or ("00000001" sll to_integer(unsigned(priority(i))));
					end if;
				end if;
			end loop;
			
			for i in 0 to 7 loop
				if bits(i) = '1' then
					level := std_logic_vector(to_unsigned(i, 3));
				end if;
			end loop;
			
			cpu_interrupt_level_internal <= level;
		end if;
	end process;
	
	process(reset, clk) is
		variable i : integer;
	begin
		if reset = '1' then
			bus_q <= (others => 'Z');
			
			for i in 0 to 31 loop
				if INTERRUPT_AVAILBLE(i) = '1' then
					flag(i) <= '0';
					priority(i) <= "000";
				end if;
			end loop;
		elsif falling_edge(clk) then
			for i in 0 to 31 loop
				if INTERRUPT_AVAILBLE(i) = '1' then
					flag(i) <= flag_next(i);
				end if;
			end loop;
			
			if chipset_ce(peripheral_id) = '1' then
				i := to_integer(unsigned(chipset_a(6 downto 2)));
				
				if bus_rw = '1' then
					if INTERRUPT_AVAILBLE(i) = '1' then
						if chipset_a(7) = '0' then
							priority(i) <= bus_d(2 downto 0);
						else
							flag(i) <= bus_d(0);
						end if;
					end if;
				else
					if chipset_a(7 downto 2) = "100000" then -- status
						bus_q <= x"000" & external_interrupt & x"000" & '0' & cpu_interrupt_level_internal;
					elsif chipset_a(7 downto 2) = "000000" then -- control
						bus_q <= (others => '0');
					elsif INTERRUPT_AVAILBLE(i) = '1' then
						if chipset_a(7) = '0' then
							bus_q <= x"0000000" & "0" & priority(i);
						else
							bus_q <= x"0000000" & "000" & flag(i);
						end if;
					end if;
				end if;
			else
				bus_q <= (others => 'Z');	
			end if;
		end if;
	end process;
	
	chipset_ack <= (peripheral_id => '1', others => '0');
	chipset_nack <= (peripheral_id => '0', others => '0');
end arch;
