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
	signal q_next : std_logic_vector(data_width - 1 downto 0);
	signal a_next : std_logic_vector(addr_width - 1 downto 0);
	
	signal oe_next : std_logic;
	signal cpu_ack_next, snd_ack_next : std_logic;
	
	signal we_next : std_logic_vector(1 downto 0);
	signal we_internal : std_logic_vector(1 downto 0);
begin
	ce <= '0';
	ub <= '0';
	lb <= '0';
	
	vga_q <= d;
	cpu_q <= d;
	snd_q <= d;
	
	we <= we_internal(0) or we_internal(1);
	
	process(reset, clk) begin
		if reset = '1' then
			we_internal <= "11";
			oe <= '1';
			cpu_ack <= '0';
			a <= (others => '1');
			q <= (others => '1');
		elsif falling_edge(clk) then
			we_internal(0) <= we_next(0);
			oe <= oe_next;
			a <= a_next;
			q <= q_next;
			
			cpu_ack <= cpu_ack_next;
			--snd_ack <= cpu_ack_next;
		elsif rising_edge(clk) then
			we_internal(1) <= we_next(1);
		end if;
	end process;
	
	process(vga_ce, snd_ce, cpu_ce, vga_a, snd_a, cpu_rw, cpu_a, cpu_d) begin
		q_next <= (others => '1');
		we_next <= "11";
		oe_next <= '1';
		
		cpu_ack_next <= '0';
		snd_ack_next <= '0';
		
		a_next <= (others => '1');
		
		if vga_ce = '1' then
			oe_next <= '0';
			a_next <= vga_a;
		elsif snd_ce = '1' then
			oe_next <= '0';
			a_next <= snd_a;
			--snd_ack_next <= '1';
		elsif cpu_ce = '1' then
			oe_next <= cpu_rw;
			we_next(0) <= not cpu_rw;
			we_next(1) <= not cpu_rw;
			a_next <= cpu_a;
			q_next <= cpu_d;
			cpu_ack_next <= '1';
		end if;
	end process;
	
end arch;
