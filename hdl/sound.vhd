library ieee;
use ieee.std_logic_1164.all;

entity sound is
	generic(
		peripheral_id : integer range 0 to 15
	);
	port(
		reset : in std_logic;
		clk : in std_logic;
		clk_snd : in std_logic;
		
		mosi : out std_logic;
		sck : out std_logic;
		ss : out std_logic;
		sync : out std_logic
	);
end sound;

architecture arch of sound is
	constant baud_div : integer := 256;
	
	type state_type is (
		idle, bit_ch, bit_buf, bit_gain, bit_shdn, bit7, bit6, bit5, bit4, bit3, bit2, bit1, bit0, bit_x0, bit_x1, bit_x2, bit_x3, stop
	);
	
	signal state, state_next : state_type;
	
	signal count : integer range 0 to (baud_div - 1);
	signal count_next : integer range 0 to (baud_div - 1);
	
	signal mosi_buffer : std_logic_vector(15 downto 0);
	signal mosi_internal, mosi_next : std_logic;
	
	signal ss_internal : std_logic;
	
	signal busy, busy_next : std_logic;
	
	signal sck_next, sck_internal : std_logic;
begin
	sync <= '0';
	
	ss <= ss_internal;
	sck <= sck_internal;
	mosi <= mosi_internal;
	
	process(state, count, mosi_internal, busy, sck_internal, mosi_buffer) begin
		count_next <= count + 1;
		
		if count = baud_div/2 then
			sck_next <= '0';
		elsif count = (baud_div - 1) then
			sck_next <= '1';
		else
			sck_next <= sck_internal;
		end if;
		
		state_next <= state;
		mosi_next <= mosi_internal;
		busy_next <= busy;
		
		case state is
			when idle =>
				count_next <= 0;
				mosi_next <= '1';
				busy_next <= '0';
				if busy = '1' then
					count_next <= baud_div/2;
					state_next <= bit_ch;
					mosi_next <= '1';
					busy_next <= '1';
				end if;
			when bit_ch | bit_buf | bit_gain | bit_shdn | bit_x0 | bit_x1 | bit_x2 | bit_x3 =>
				mosi_next <= mosi_internal;
				if count = baud_div/2 then
					mosi_next <= '1';
				end if;
				
				if count = (baud_div - 1) then
					state_next <= state_type'succ(state);
				end if;
			when bit7 | bit6 | bit5 | bit4 | bit3 | bit2 | bit1 | bit0 =>
				mosi_next <= mosi_internal;
				if count = baud_div/2 then
					mosi_next <= mosi_buffer(7 - (state_type'pos(state) - state_type'pos(bit7)));
				end if;
				
				if count = (baud_div - 1) then
					state_next <= state_type'succ(state);
				end if;
			when stop =>
				mosi_next <= mosi_buffer(0);
				if count = baud_div/2 then
					count_next <= 0;
					busy_next <= '0';
					state_next <= idle;
					mosi_next <= '1';
				end if;
		end case;
	end process;
	
	process(reset, clk_snd) is
		variable check : std_logic_vector(1 downto 0);
	begin
		if reset = '1' then
			state <= idle;
			
			count <= 0;
			busy <= '0';
			ss_internal <= '0';
			sck_internal <= '0';
			
			mosi_buffer <= (others => '1');
			mosi_internal <= '1';
		elsif falling_edge(clk_snd) then
			state <= state_next;
			count <= count_next;
			busy <= busy_next;
			
			sck_internal <= sck_next;
			mosi_internal <= mosi_next;
		elsif rising_edge(clk_snd) then
			
		end if;
	end process;
end arch;
