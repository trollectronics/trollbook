EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:amp
LIBS:peripheral-board-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 6
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Sheet
S 4600 1350 1400 700 
U 581919F6
F0 "Keyboard" 60
F1 "keyboard.sch" 60
$EndSheet
$Sheet
S 5550 3850 1250 700 
U 5819E78C
F0 "Mouse" 60
F1 "mouse.sch" 60
$EndSheet
$Comp
L ATTINY48-A IC?
U 1 1 5819EBDE
P 2950 4200
F 0 "IC?" H 2950 5957 50  0000 C CNN
F 1 "ATTINY48-A" H 2950 5866 50  0000 C CNN
F 2 "TQFP32" H 2950 5775 50  0000 C CIN
F 3 "" H 2950 4200 50  0000 C CNN
	1    2950 4200
	1    0    0    -1  
$EndComp
Wire Wire Line
	3900 3450 4400 3450
Wire Wire Line
	3900 3550 4400 3550
Wire Wire Line
	3900 3650 4400 3650
Wire Wire Line
	3900 3750 4400 3750
Text Label 4000 3450 0    60   ~ 0
SPI_SS
Text Label 4000 3550 0    60   ~ 0
SPI_MOSI
Text Label 4000 3650 0    60   ~ 0
SPI_MISO
Text Label 4000 3750 0    60   ~ 0
SPI_SCK
Wire Wire Line
	3900 4500 4400 4500
Wire Wire Line
	3900 4600 4400 4600
Text Label 4000 4500 0    60   ~ 0
I2C_SDA
Text Label 4000 4600 0    60   ~ 0
I2C_SCL
Wire Wire Line
	3900 4100 4400 4100
Wire Wire Line
	3900 4200 4400 4200
Text Label 4000 4100 0    60   ~ 0
MOUSE_X
Text Label 4000 4200 0    60   ~ 0
MOUSE_Y
$Sheet
S 9100 2550 1050 800 
U 5819EDEF
F0 "Power Supply" 60
F1 "power.sch" 60
$EndSheet
$Sheet
S 7950 4150 1250 800 
U 5819EDF3
F0 "RTC" 60
F1 "rtc.sch" 60
$EndSheet
Text Notes 7400 2700 0    60   ~ 0
VGA\nExternal connector\nBattery
$Sheet
S 8200 800  1000 650 
U 5819EF10
F0 "Audio" 60
F1 "audio.sch" 60
$EndSheet
$EndSCHEMATC
