library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity llram is
	generic(
		data_width : integer;
		addr_width : integer
	);
	port(
		reset : in std_logic;
		clk : in std_logic;
		
		a : out std_logic_vector(addr_width - 1 downto 0);
		d : in std_logic_vector(data_width - 1 downto 0);
		q : out std_logic_vector(data_width - 1 downto 0);
		ce : out std_logic;
		we : out std_logic;
		lb : out std_logic;
		ub : out std_logic;
		oe : out std_logic;
		
		vga_a : in std_logic_vector(addr_width - 1 downto 0);
		vga_q : out std_logic_vector(data_width - 1 downto 0);
		vga_ce : in std_logic;
		
		cpu_a : in std_logic_vector(addr_width - 1 downto 0);
		cpu_d : in std_logic_vector(data_width - 1 downto 0);
		cpu_q : out std_logic_vector(data_width - 1 downto 0);
		cpu_rw : in std_logic;
		cpu_siz : in std_logic_vector(1 downto 0);
		cpu_ce : in std_logic;
		cpu_ack : out std_logic;
		
		snd_a : in std_logic_vector(addr_width - 1 downto 0);
		snd_q : out std_logic_vector(data_width - 1 downto 0);
		snd_ce : in std_logic
	);
end llram;

architecture arch of llram is
	constant count_to : integer := 192000*2;
	signal count : integer range 0 to count_to + 1;
	signal count_next : integer range 0 to count_to + 1;
begin
	ce <= '0';
	ub <= '0';
	lb <= '0';
	
	process(reset, clk) begin
		if reset = '1' then
			count <= count_to;
		elsif falling_edge(clk) then
			--ce <= '1';
			count <= count_next;
		end if;
	end process;
	
	process(vga_ce, snd_ce, cpu_ce, vga_a, snd_a, cpu_a, d, count, clk, cpu_rw, cpu_d) begin
		vga_q <= (others => '1');
		cpu_q <= (others => '1');
		snd_q <= (others => '1');
		q <= (others => '1');
		we <= '1';
		oe <= '1';
		count_next <= count;
		
		cpu_ack <= '0';
		
		if count = count_to then
			a <= (others => '1');
			we <= '1';
			
			if vga_ce = '1' then
				oe <= '0';
				a <= vga_a;
				vga_q <= vga_a(6 downto 0) & '0' & vga_a(6 downto 0) & '1';
				vga_q <= d;
				--vga_q <= x"FEFF";
			elsif snd_ce = '1' then
				oe <= '0';
				a <= snd_a;
				snd_q <= d;
			elsif cpu_ce = '1' then
				oe <= cpu_rw;
				we <= not cpu_rw;
				a <= cpu_a;
				cpu_q <= d;
				q <= cpu_d;
				cpu_ack <= '1';
			end if;
				
		else
			count_next <= count + 1;
			oe <= '1';
			if count mod 2 = 0 then
				we <= '0';
			else
				we <= '1';
			end if;
			--we <= '0';
			a <= std_logic_vector(to_unsigned(count/2, a'length));
			--q <= x"FFFE";
			q <= std_logic_vector(to_unsigned(count, q'length));
		end if;
	end process;
	
end arch;
