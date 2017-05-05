library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use work.wor_logic.all;

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
		sync : out std_logic;
		
		ll_a : out std_logic_vector(17 downto 0);
		ll_d : in std_logic_vector(15 downto 0);
		ll_ce : out std_logic;
		ll_ack : in std_logic;
		
		chipset_a : in std_logic_vector(7 downto 0);
		bus_d : in std_logic_vector(31 downto 0);
		bus_q : out std_logic_vector(31 downto 0);
		bus_rw : in std_logic;
		bus_siz : in std_logic_vector(1 downto 0);
		chipset_ce : in std_logic_vector(15 downto 0);
		chipset_ack : out wor_logic_vector(15 downto 0);
		chipset_nack : out wor_logic_vector(15 downto 0)
		--chipset_int : out std_logic_vector(15 downto 0)
	);
end sound;

architecture arch of sound is
	type llram_state_type is (
		idle, word_read, done
	);
	
	type spi_state_type is (
		idle, bit_cha, bit_bufa, bit_gaina, bit_shdna, bit7a, bit6a, bit5a, bit4a, bit3a, bit2a, bit1a, bit0a, bit_x0a, bit_x1a, bit_x2a, bit_x3a, flip_ss0, flip_ss1,
		bit_chb, bit_bufb, bit_gainb, bit_shdnb, bit7b, bit6b, bit5b, bit4b, bit3b, bit2b, bit1b, bit0b, bit_x0b, bit_x1b, bit_x2b, bit_x3b, stop
	);
	
	signal spi_state, spi_state_next : spi_state_type;
	
	signal spi_count : unsigned(1 downto 0);
	signal spi_count_next : unsigned(1 downto 0);
	
	signal mosi_internal, mosi_next : std_logic;
	
	signal ss_internal, ss_next : std_logic;
	
	signal busy, busy_next : std_logic;
	
	signal sck_next, sck_internal : std_logic;
	
	signal sample_buffer, sample_buffer_next : std_logic_vector(15 downto 0);
	
	signal llram_state, llram_state_next : llram_state_type;
	
	signal ll_a_next, ll_a_internal : std_logic_vector(17 downto 0);
	signal ll_ce_next, ll_ce_internal : std_logic;
	
	signal ll_a_base : std_logic_vector(10 downto 0);
	signal address_counter, address_counter_next : unsigned(9 downto 0);
	signal buffer_size : std_logic_vector(1 downto 0);
	
	signal sync_next : std_logic;
	signal need_more_33 : std_logic;
	signal ack_more_33, ack_more_33_next : std_logic;
	
	-- 24 MHz domain
	signal sample_counter, sample_counter_next, sample_compare : unsigned(10 downto 0);
	signal need_more_24, need_more_24_next : std_logic;
	signal ack_more_24 : std_logic;
	signal enabled : std_logic;
begin
	ss <= ss_internal;
	sck <= sck_internal;
	mosi <= mosi_internal;
	
	ll_ce <= ll_ce_internal;
	ll_a <= ll_a_internal;
	
	llram_machine: process(llram_state, need_more_33, sample_buffer, ll_d, ll_ce_internal, ack_more_33, ll_ack, ll_a_internal, address_counter, buffer_size, ll_a_base) begin
		llram_state_next <= llram_state;
		ll_ce_next <= ll_ce_internal;
		ll_a_next <= ll_a_internal;
		sample_buffer_next <= sample_buffer;
		ack_more_33_next <= ack_more_33;
		
		address_counter_next <= address_counter;
		
		case llram_state is
			when idle =>
				ack_more_33_next <= '0';
				
				if need_more_33 = '1' then
					llram_state_next <= word_read;
					
					case buffer_size is
						when "00" =>
							ll_a_next <= ll_a_base(10 downto 0) & std_logic_vector(address_counter(6 downto 0));
						when "01" =>
							ll_a_next <= ll_a_base(9 downto 0) & std_logic_vector(address_counter(7 downto 0));
						when "10" =>
							ll_a_next <= ll_a_base(8 downto 0) & std_logic_vector(address_counter(8 downto 0));
						when others =>
							ll_a_next <= ll_a_base(7 downto 0) & std_logic_vector(address_counter(9 downto 0));
							
					end case;
					
					ll_ce_next <= '1';
					ack_more_33_next <= '1';
				end if;
			
			when word_read =>
				if ll_ack = '1' then
					llram_state_next <= done;
					ll_ce_next <= '0';
					sample_buffer_next <= ll_d;
				end if;
			
			when done =>
				if need_more_33 = '0' then
					llram_state_next <= idle;
					address_counter_next <= address_counter + 1;
				end if;
		end case;
	end process;
	
	process(clk, reset) begin
		if reset = '1' then
			ack_more_33 <= '0';
			need_more_33 <= '0';
			
			ll_ce_internal <= '0';
			ll_a_internal <= (others => '0');
			
			sample_buffer <= (others => '0');
			
			address_counter <= (others => '0');
		elsif rising_edge(clk) then
			need_more_33 <= need_more_24;
			ack_more_33 <= ack_more_33_next;
			
			llram_state <= llram_state_next;
			ll_ce_internal <= ll_ce_next;
			ll_a_internal <= ll_a_next;
			sample_buffer <= sample_buffer_next;
			
			address_counter <= address_counter_next;
		end if;
	end process;
	
	counter24_state_logic: process(sample_counter, need_more_24, ack_more_24, enabled, sample_compare) begin
		sync_next <= '1';
		need_more_24_next <= need_more_24;
		
		if enabled = '0' then
			sample_counter_next <= (others => '0');
		elsif sample_counter + 1 = sample_compare then
			sample_counter_next <= (others => '0');
			sync_next <= '0';
		else
			sample_counter_next <= sample_counter + 1;
		end if;
		
		if sample_counter = to_unsigned(0, sample_counter'length) then
			sync_next <= '0';
			need_more_24_next <= '1';
		end if;
		
		if ack_more_24 = '1' then
			need_more_24_next <= '0';
		end if;
	end process;
	
	counter24: process(clk_snd, reset) begin
		if reset = '1' then
			sample_counter <= (others => '0');
			
			need_more_24 <= '0';
			ack_more_24 <= '0';
			sync <= '1';
		elsif rising_edge(clk_snd) then
			ack_more_24 <= ack_more_33;
			need_more_24 <= need_more_24_next;
			sample_counter <= sample_counter_next;
			sync <= sync_next;
		end if;
	end process;
	
	spi_state_logic: process(spi_state, spi_count, mosi_internal, busy, sck_internal, llram_state, ss_internal, sample_buffer) begin
		spi_count_next <= spi_count + 1;
		
		if spi_count = to_unsigned(1, spi_count'length) then
			sck_next <= '0';
		elsif spi_count = to_unsigned(3, spi_count'length) then
			sck_next <= '1';
			spi_count_next <= to_unsigned(0, spi_count'length);
		else
			sck_next <= sck_internal;
		end if;
		
		spi_state_next <= spi_state;
		mosi_next <= mosi_internal;
		busy_next <= busy;
		ss_next <= ss_internal;
		
		case spi_state is
			when idle =>
				spi_count_next <= to_unsigned(0, spi_count'length);
				mosi_next <= '1';
				busy_next <= '0';
				if llram_state = done then
					spi_count_next <= to_unsigned(1, spi_count'length);
					spi_state_next <= bit_cha;
					mosi_next <= '1';
					busy_next <= '1';
					ss_next <= '0';
				end if;
			
			when bit_cha =>
				mosi_next <= mosi_internal;
				if spi_count = to_unsigned(1, spi_count'length) then
					mosi_next <= '0';
				end if;
				
				if spi_count = to_unsigned(3, spi_count'length) then
					spi_state_next <= spi_state_type'succ(spi_state);
				end if;
			
			when flip_ss0 =>
				mosi_next <= mosi_internal;
				
				if spi_count = to_unsigned(1, spi_count'length) then
					mosi_next <= '1';
					sck_next <= '0';
					ss_next <= '1';
				end if;
				
				if spi_count = to_unsigned(3, spi_count'length) then
					spi_state_next <= spi_state_type'succ(spi_state);
					ss_next <= '0';
					sck_next <= '0';
				end if;
			
			when flip_ss1 =>
				mosi_next <= mosi_internal;
				if spi_count = to_unsigned(1, spi_count'length) then
					mosi_next <= '1';
					sck_next <= '0';
					ss_next <= '0';
				end if;
				
				if spi_count = to_unsigned(3, spi_count'length) then
					spi_state_next <= spi_state_type'succ(spi_state);
					sck_next <= '0';
				end if;
			
			when bit_chb =>
				mosi_next <= mosi_internal;
				if spi_count = to_unsigned(1, spi_count'length) then
					mosi_next <= '1';
				end if;
				
				if spi_count = to_unsigned(3, spi_count'length) then
					spi_state_next <= spi_state_type'succ(spi_state);
				end if;
			
			when bit_bufa | bit_gaina | bit_shdna | bit_x0a | bit_x1a | bit_x2a | bit_x3a | bit_bufb | bit_gainb | bit_shdnb | bit_x0b | bit_x1b | bit_x2b | bit_x3b =>
				mosi_next <= mosi_internal;
				if spi_count = to_unsigned(1, spi_count'length) then
					mosi_next <= '1';
				end if;
				
				if spi_count = to_unsigned(3, spi_count'length) then
					spi_state_next <= spi_state_type'succ(spi_state);
				end if;
			
			when bit7a | bit6a | bit5a | bit4a | bit3a | bit2a | bit1a | bit0a =>
				mosi_next <= mosi_internal;
				if spi_count = to_unsigned(1, spi_count'length) then
					mosi_next <= sample_buffer(15 - (spi_state_type'pos(spi_state) - spi_state_type'pos(bit7a)));
				end if;
				
				if spi_count = to_unsigned(3, spi_count'length) then
					spi_state_next <= spi_state_type'succ(spi_state);
				end if;
			
			when bit7b | bit6b | bit5b | bit4b | bit3b | bit2b | bit1b | bit0b =>
				mosi_next <= mosi_internal;
				if spi_count = to_unsigned(1, spi_count'length) then
					mosi_next <= sample_buffer(7 - (spi_state_type'pos(spi_state) - spi_state_type'pos(bit7b)));
				end if;
				
				if spi_count = to_unsigned(3, spi_count'length) then
					spi_state_next <= spi_state_type'succ(spi_state);
				end if;
			
			when stop =>
				mosi_next <= '1';
				if spi_count = to_unsigned(1, spi_count'length) then
					spi_count_next <= to_unsigned(0, spi_count'length);
					busy_next <= '0';
					spi_state_next <= idle;
					mosi_next <= '1';
					ss_next <= '1';
				end if;
		end case;
	end process;
	
	process(reset, clk) is
		variable check : std_logic_vector(1 downto 0);
	begin
		if reset = '1' then
			spi_state <= idle;
			
			spi_count <= to_unsigned(0, spi_count'length);
			busy <= '0';
			ss_internal <= '1';
			sck_internal <= '0';
			
			mosi_internal <= '1';
		elsif falling_edge(clk) then
			spi_state <= spi_state_next;
			spi_count <= spi_count_next;
			busy <= busy_next;
			
			sck_internal <= sck_next;
			mosi_internal <= mosi_next;
			
			ss_internal <= ss_next;
		elsif rising_edge(clk) then
			
		end if;
	end process;
	
	bus_logic: process(reset, clk) is
		variable check : std_logic_vector(1 downto 0);
	begin
		if reset = '1' then
			sample_compare <= to_unsigned(256, sample_compare'length);
			enabled <= '1';
			buffer_size <= "11";
			ll_a_base <= x"A5" & "010";
		elsif falling_edge(clk) then
			check := chipset_ce(peripheral_id) & bus_rw;
			
			case check is
				when "11" =>
					case chipset_a(4 downto 2) is
						when "000" => -- Control
							enabled <= bus_d(0);
						when "001" => -- Status
						when "010" => -- Base address
							ll_a_base <= bus_d(18 downto 8);
						when "011" => -- Buffer size
							buffer_size <= bus_d(buffer_size'range);
						when "100" => -- Sampling rate
							sample_compare <= unsigned(bus_d(sample_compare'range));
						when others =>
						
					end case;
					
				when others =>
			end case;
			
		end if;
	end process;
	
	process(reset, clk) is
		variable current_buffer : std_logic_vector(address_counter'range);
	begin
		if reset = '1' then
			bus_q <= (others => 'Z');
		elsif falling_edge(clk) then
			if chipset_ce(peripheral_id) = '1' then
				case chipset_a(4 downto 2) is
					when "000" => -- Control
							bus_q <= x"0000000" & "000" & enabled;
						when "001" => -- Status
							current_buffer := std_logic_vector(address_counter srl (3 + to_integer(unsigned(buffer_size))));
							bus_q <= x"0000000" & "000" & current_buffer(0);
						when "010" => -- Base address
							bus_q <= (others => '0');
							bus_q(18 downto 8) <= ll_a_base;
						when "011" => -- Buffer size
							bus_q <= (others => '0');
							bus_q(buffer_size'range) <= buffer_size;
						when "100" => -- Sampling rate
							bus_q <= (others => '0');
							bus_q(sample_compare'range) <= std_logic_vector(sample_compare);
						when others =>
							bus_q <= (others => 'Z');
				end case;
			else
				bus_q <= (others => 'Z');
			end if;
		end if;
	end process;
	
	chipset_ack <= (peripheral_id => '1', others => '0');
	chipset_nack <= (peripheral_id => '0', others => '0');
	--chipset_int <= (peripheral_id => '0', others => '0');
end arch;
