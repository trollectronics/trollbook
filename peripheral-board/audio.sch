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
LIBS:card-edge
LIBS:switchmode
LIBS:joystick
LIBS:peripheral-board-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 6 9
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L JACK_TRS_6PINS J6001
U 1 1 5819EF20
P 4500 2350
F 0 "J6001" H 4481 2875 50  0000 C CNN
F 1 "JACK_TRS_6PINS" H 4481 2784 50  0000 C CNN
F 2 "" H 4600 2200 50  0001 C CNN
F 3 "" H 4600 2200 50  0000 C CNN
	1    4500 2350
	1    0    0    -1  
$EndComp
Text Notes 6200 2250 0    60   ~ 0
Digital pot\nAmp
$Comp
L TCA0372 U6001
U 1 1 581D2053
P 4700 3700
F 0 "U6001" H 4600 3800 60  0000 L CNN
F 1 "TCA0372" H 4500 3700 60  0000 L CNN
F 2 "Housings_SOIC:SOIC-16_7.5x10.3mm_Pitch1.27mm" H 4700 3700 60  0001 C CNN
F 3 "" H 4700 3700 60  0001 C CNN
	1    4700 3700
	1    0    0    -1  
$EndComp
$Comp
L TCA0372 U6001
U 2 1 581D2125
P 4700 5100
F 0 "U6001" H 4600 5200 60  0000 L CNN
F 1 "TCA0372" H 4500 5100 60  0000 L CNN
F 2 "Housings_SOIC:SOIC-16_7.5x10.3mm_Pitch1.27mm" H 4700 5100 60  0001 C CNN
F 3 "" H 4700 5100 60  0001 C CNN
	2    4700 5100
	1    0    0    -1  
$EndComp
Text HLabel 1800 3550 0    60   Input ~ 0
L
Text HLabel 1800 3650 0    60   Input ~ 0
R
Wire Wire Line
	1800 3650 2150 3650
Wire Wire Line
	2150 3650 2150 3550
Wire Wire Line
	2150 3550 1800 3550
Text HLabel 1750 3950 0    60   BiDi ~ 0
SDA
Text HLabel 1750 3850 0    60   Input ~ 0
SCL
Wire Wire Line
	1750 3850 2150 3850
Wire Wire Line
	2150 3850 2150 3950
Wire Wire Line
	2150 3950 1750 3950
$EndSCHEMATC
