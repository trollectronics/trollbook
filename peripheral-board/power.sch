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
LIBS:digital-pot
LIBS:peripheral-board-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 4 9
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
L BARREL_JACK CON4001
U 1 1 581A8CF2
P 2300 1800
F 0 "CON4001" H 2281 2125 50  0000 C CNN
F 1 "BARREL_JACK" H 2281 2034 50  0000 C CNN
F 2 "" H 2300 1800 50  0001 C CNN
F 3 "" H 2300 1800 50  0000 C CNN
	1    2300 1800
	1    0    0    -1  
$EndComp
$Comp
L AP1509 U4001
U 1 1 58224E1C
P 7150 3550
F 0 "U4001" H 7150 3600 60  0000 C CNN
F 1 "AP1509" H 7150 3500 60  0000 C CNN
F 2 "" H 7150 3550 60  0001 C CNN
F 3 "" H 7150 3550 60  0001 C CNN
	1    7150 3550
	1    0    0    -1  
$EndComp
$Comp
L LM27313 U4002
U 1 1 582259B3
P 5150 4600
F 0 "U4002" H 5150 4650 60  0000 C CNN
F 1 "LM27313" H 5150 4550 60  0000 C CNN
F 2 "" H 5150 4600 60  0001 C CNN
F 3 "" H 5150 4600 60  0001 C CNN
	1    5150 4600
	1    0    0    -1  
$EndComp
Text Notes 4600 2450 0    60   ~ 0
LM2735 Sepic converter?
Text HLabel 3050 3050 0    60   Input ~ 0
~SHDN
Wire Wire Line
	3050 3050 4450 3050
Wire Wire Line
	4450 3050 4450 4600
$EndSCHEMATC
