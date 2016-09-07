onerror {resume}
quietly WaveActivateNextPane {} 0
add wave -noupdate -format Literal -radix hexadecimal /test/u_dut/a
add wave -noupdate -format Literal -radix hexadecimal /test/u_dut/d
add wave -noupdate -format Literal /test/u_dut/cpu_tt
add wave -noupdate -format Literal /test/u_dut/cpu_tm
add wave -noupdate -format Literal /test/u_dut/cpu_siz
add wave -noupdate -format Logic /test/u_dut/cpu_rw
add wave -noupdate -format Logic /test/u_dut/cpu_ts
add wave -noupdate -format Logic /test/u_dut/cpu_tip
add wave -noupdate -format Logic /test/u_dut/cpu_ta
add wave -noupdate -format Logic /test/u_dut/cpu_tea
add wave -noupdate -format Logic /test/u_dut/cpu_tbi
add wave -noupdate -format Literal /test/u_dut/cpu_ipl
add wave -noupdate -format Logic /test/u_dut/cpu_clk
add wave -noupdate -format Logic /test/u_dut/cpu_lfo
add wave -noupdate -format Logic /test/u_dut/cpu_scd
add wave -noupdate -format Logic /test/u_dut/cpu_rsti
add wave -noupdate -format Logic /test/u_dut/cpu_rsto
add wave -noupdate -format Literal -radix hexadecimal /test/u_dut/ram_a
add wave -noupdate -format Literal /test/u_dut/ram_b
add wave -noupdate -format Logic /test/u_dut/ram_cas
add wave -noupdate -format Logic /test/u_dut/ram_ras
add wave -noupdate -format Logic /test/u_dut/ram_we
add wave -noupdate -format Literal /test/u_dut/ram_ldqm
add wave -noupdate -format Literal /test/u_dut/ram_udqm
add wave -noupdate -format Literal /test/u_dut/ram_cs
add wave -noupdate -format Logic /test/u_dut/ram_cke
add wave -noupdate -format Literal -radix hexadecimal /test/u_dut/ll_a
add wave -noupdate -format Literal -radix hexadecimal /test/u_dut/ll_d
add wave -noupdate -format Logic /test/u_dut/ll_ce
add wave -noupdate -format Logic /test/u_dut/ll_we
add wave -noupdate -format Logic /test/u_dut/ll_lb
add wave -noupdate -format Logic /test/u_dut/ll_ub
add wave -noupdate -format Logic /test/u_dut/ll_oe
add wave -noupdate -format Literal /test/u_dut/vga_r
add wave -noupdate -format Literal /test/u_dut/vga_g
add wave -noupdate -format Literal /test/u_dut/vga_b
add wave -noupdate -format Logic /test/u_dut/vga_hsync
add wave -noupdate -format Logic /test/u_dut/vga_vsync
add wave -noupdate -format Logic /test/u_dut/vga_den
add wave -noupdate -format Logic /test/u_dut/vga_pwr
add wave -noupdate -format Logic /test/u_dut/vga_pwm
add wave -noupdate -format Logic /test/u_dut/snd_mosi
add wave -noupdate -format Logic /test/u_dut/snd_clk
add wave -noupdate -format Logic /test/u_dut/snd_ss
add wave -noupdate -format Logic /test/u_dut/snd_sync
add wave -noupdate -format Logic /test/u_dut/spi_miso
add wave -noupdate -format Logic /test/u_dut/spi_mosi
add wave -noupdate -format Logic /test/u_dut/spi_clk
add wave -noupdate -format Literal /test/u_dut/spi_ss
add wave -noupdate -format Logic /test/u_dut/uart_rx
add wave -noupdate -format Logic /test/u_dut/uart_tx
add wave -noupdate -format Literal /test/u_dut/ext_int
add wave -noupdate -format Logic /test/u_dut/clk33
add wave -noupdate -format Logic /test/u_dut/clk12
add wave -noupdate -format Logic /test/u_dut/pwron_reset
add wave -noupdate -format Literal -radix hexadecimal /test/u_dut/cpu_q
add wave -noupdate -format Logic /test/u_dut/cpu_oe
add wave -noupdate -format Logic /test/u_dut/internal_reset
add wave -noupdate -format Literal -radix hexadecimal /test/u_dut/ll_q
add wave -noupdate -format Logic /test/u_dut/ll_oe_internal
add wave -noupdate -format Literal -radix hexadecimal /test/u_dut/ll_vga_a
add wave -noupdate -format Literal -radix hexadecimal /test/u_dut/ll_vga_q
add wave -noupdate -format Logic /test/u_dut/ll_vga_ce
add wave -noupdate -format Literal -radix hexadecimal /test/u_dut/ll_cpu_a
add wave -noupdate -format Literal -radix hexadecimal /test/u_dut/ll_cpu_d
add wave -noupdate -format Literal -radix hexadecimal /test/u_dut/ll_cpu_q
add wave -noupdate -format Logic /test/u_dut/ll_cpu_rw
add wave -noupdate -format Logic /test/u_dut/ll_cpu_lb
add wave -noupdate -format Logic /test/u_dut/ll_cpu_ub
add wave -noupdate -format Logic /test/u_dut/ll_cpu_ce
add wave -noupdate -format Logic /test/u_dut/ll_cpu_ack
add wave -noupdate -format Literal -radix hexadecimal /test/u_dut/bus_a
add wave -noupdate -format Literal -radix hexadecimal /test/u_dut/bus_d
add wave -noupdate -format Literal -radix hexadecimal /test/u_dut/bus_q
add wave -noupdate -format Literal /test/u_dut/bus_siz
add wave -noupdate -format Logic /test/u_dut/bus_rw
add wave -noupdate -format Logic /test/u_dut/bus_ce_uart
add wave -noupdate -format Logic /test/u_dut/bus_ack_uart
add wave -noupdate -format Logic /test/u_dut/bus_ce_llram
add wave -noupdate -format Logic /test/u_dut/bus_ack_llram
add wave -noupdate -format Logic /test/u_dut/bus_ce_spi
add wave -noupdate -format Logic /test/u_dut/bus_ack_spi
add wave -noupdate -format Logic /test/u_dut/ll_ub_internal
add wave -noupdate -divider -height 25 SPI
add wave -noupdate -format Logic /test/u_dut/u_spi/reset
add wave -noupdate -format Logic /test/u_dut/u_spi/clk
add wave -noupdate -format Logic /test/u_dut/u_spi/miso
add wave -noupdate -format Logic /test/u_dut/u_spi/mosi
add wave -noupdate -format Logic /test/u_dut/u_spi/sck
add wave -noupdate -format Literal /test/u_dut/u_spi/ss
add wave -noupdate -format Literal -radix hexadecimal /test/u_dut/u_spi/bus_a
add wave -noupdate -format Literal -radix hexadecimal /test/u_dut/u_spi/bus_d
add wave -noupdate -format Literal -radix hexadecimal /test/u_dut/u_spi/bus_q
add wave -noupdate -format Logic /test/u_dut/u_spi/bus_rw
add wave -noupdate -format Literal /test/u_dut/u_spi/bus_siz
add wave -noupdate -format Logic /test/u_dut/u_spi/bus_ce
add wave -noupdate -format Logic /test/u_dut/u_spi/bus_ack
add wave -noupdate -format Literal /test/u_dut/u_spi/state
add wave -noupdate -format Literal /test/u_dut/u_spi/state_next
add wave -noupdate -format Literal /test/u_dut/u_spi/count
add wave -noupdate -format Literal /test/u_dut/u_spi/count_next
add wave -noupdate -format Literal -radix hexadecimal /test/u_dut/u_spi/baud_div
add wave -noupdate -format Literal -radix hexadecimal /test/u_dut/u_spi/mosi_buffer
add wave -noupdate -format Literal -radix hexadecimal /test/u_dut/u_spi/miso_buffer
add wave -noupdate -format Literal -radix hexadecimal /test/u_dut/u_spi/miso_buffer_next
add wave -noupdate -format Logic /test/u_dut/u_spi/mosi_internal
add wave -noupdate -format Logic /test/u_dut/u_spi/mosi_next
add wave -noupdate -format Literal /test/u_dut/u_spi/ss_internal
add wave -noupdate -format Logic /test/u_dut/u_spi/busy
add wave -noupdate -format Logic /test/u_dut/u_spi/busy_next
add wave -noupdate -format Logic /test/u_dut/u_spi/sck_next
add wave -noupdate -format Logic /test/u_dut/u_spi/sck_internal
TreeUpdate [SetDefaultTree]
WaveRestoreCursors {{Cursor 1} {4784 ns} 0}
configure wave -namecolwidth 254
configure wave -valuecolwidth 100
configure wave -justifyvalue left
configure wave -signalnamewidth 0
configure wave -snapdistance 10
configure wave -datasetprefix 0
configure wave -rowmargin 4
configure wave -childrowmargin 2
configure wave -gridoffset 0
configure wave -gridperiod 1
configure wave -griddelta 40
configure wave -timeline 0
configure wave -timelineunits ns
update
WaveRestoreZoom {4197 ns} {5013 ns}
