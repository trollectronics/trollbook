EESchema Schematic File Version 2
LIBS:peripheral-board-rescue
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
LIBS:vref
LIBS:switches
LIBS:peripheral-board-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 10 10
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
L CONN_01X06 P10001
U 1 1 581F8990
P 4700 2400
F 0 "P10001" H 4777 2441 50  0000 L CNN
F 1 "VGA" H 4777 2350 50  0000 L CNN
F 2 "Socket_Strips:Socket_Strip_Straight_1x06" H 4700 2400 50  0001 C CNN
F 3 "" H 4700 2400 50  0000 C CNN
	1    4700 2400
	1    0    0    -1  
$EndComp
Wire Wire Line
	4500 2150 3650 2150
Wire Wire Line
	4500 2250 3650 2250
Wire Wire Line
	4500 2350 3650 2350
Wire Wire Line
	4500 2550 3650 2550
Wire Wire Line
	4500 2650 3650 2650
Text Label 4150 2150 0    60   ~ 0
R
Text Label 4150 2250 0    60   ~ 0
G
Text Label 4150 2350 0    60   ~ 0
B
Text Label 4150 2550 0    60   ~ 0
HSYNC
Text Label 4150 2650 0    60   ~ 0
VSYNC
$Comp
L GND #PWR10001
U 1 1 581F8A49
P 4100 2450
F 0 "#PWR10001" H 4100 2200 50  0001 C CNN
F 1 "GND" V 4105 2322 50  0000 R CNN
F 2 "" H 4100 2450 50  0000 C CNN
F 3 "" H 4100 2450 50  0000 C CNN
	1    4100 2450
	0    1    1    0   
$EndComp
Wire Wire Line
	4500 2450 4100 2450
Text Notes 6700 1250 0    60   ~ 0
Also: VGA DSUB\n
$Comp
L DB15_FEMALE_HighDensity J10001
U 1 1 589C4FB4
P 7150 2650
F 0 "J10001" H 7150 3517 50  0000 C CNN
F 1 "DB15_FEMALE_HighDensity" H 7150 3426 50  0000 C CNN
F 2 "lib:HDDSUB-15-VGA" H -1200 50  50  0001 C CNN
F 3 "" H -1200 50  50  0001 C CNN
	1    7150 2650
	1    0    0    -1  
$EndComp
$EndSCHEMATC
