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
Sheet 9 10
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
L 10056847-XY1LF CONN9001
U 1 1 581F82EF
P 5550 2150
F 0 "CONN9001" H 5550 2937 60  0000 C CNN
F 1 "10056847-XY1LF" H 5550 2831 60  0000 C CNN
F 2 "lib:10056847-XY1LF" H 5550 2150 60  0001 C CNN
F 3 "" H 5550 2150 60  0001 C CNN
	1    5550 2150
	1    0    0    -1  
$EndComp
Text HLabel 6200 1650 2    60   Output ~ 0
MISO
Text HLabel 6200 1750 2    60   Input ~ 0
MOSI
Text HLabel 6200 1850 2    60   Input ~ 0
SCK
Text HLabel 4900 2300 0    60   Input ~ 0
SS1
Text HLabel 4900 2400 0    60   Input ~ 0
SS2
Text HLabel 4900 2500 0    60   Input ~ 0
SS3
Text HLabel 4900 2600 0    60   Input ~ 0
SS4
Text HLabel 6200 2350 2    60   Output ~ 0
INTERRUPT
Text HLabel 6200 2050 2    60   Input ~ 0
TX
Text HLabel 6200 2150 2    60   Output ~ 0
RX
Text HLabel 6200 2450 2    60   BiDi ~ 0
SDA
Text HLabel 6200 2550 2    60   Input ~ 0
SCL
Text HLabel 4400 1700 0    60   UnSpc ~ 0
DETECT
$Comp
L C C9001
U 1 1 58246CCC
P 4400 2050
F 0 "C9001" H 4000 2100 50  0000 L CNN
F 1 "10u" H 4050 2000 50  0000 L CNN
F 2 "" H 4438 1900 50  0001 C CNN
F 3 "" H 4400 2050 50  0000 C CNN
	1    4400 2050
	1    0    0    -1  
$EndComp
$Comp
L +3V3 #PWR9002
U 1 1 58A3C2FE
P 4250 1900
F 0 "#PWR9002" H -300 800 50  0001 C CNN
F 1 "+3V3" V 4265 2028 50  0000 L CNN
F 2 "" H -300 950 50  0001 C CNN
F 3 "" H -300 950 50  0001 C CNN
	1    4250 1900
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR9004
U 1 1 58A3C347
P 4250 2200
F 0 "#PWR9004" H -700 1000 50  0001 C CNN
F 1 "GND" V 4255 2072 50  0000 R CNN
F 2 "" H -700 1250 50  0001 C CNN
F 3 "" H -700 1250 50  0001 C CNN
	1    4250 2200
	0    1    1    0   
$EndComp
$Comp
L GND #PWR9003
U 1 1 58A3DB73
P 6650 1950
F 0 "#PWR9003" H 1700 750 50  0001 C CNN
F 1 "GND" V 6655 1822 50  0000 R CNN
F 2 "" H 1700 1000 50  0001 C CNN
F 3 "" H 1700 1000 50  0001 C CNN
	1    6650 1950
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR9005
U 1 1 58A3DB88
P 6650 2250
F 0 "#PWR9005" H 1700 1050 50  0001 C CNN
F 1 "GND" V 6655 2122 50  0000 R CNN
F 2 "" H 1700 1300 50  0001 C CNN
F 3 "" H 1700 1300 50  0001 C CNN
	1    6650 2250
	0    -1   -1   0   
$EndComp
Wire Wire Line
	6650 1950 6200 1950
Wire Wire Line
	6650 2250 6200 2250
Text HLabel 4900 2100 0    60   Output ~ 0
AN
Wire Wire Line
	4900 1800 4900 2000
Connection ~ 4900 1900
Wire Wire Line
	4250 1900 4900 1900
Connection ~ 4400 1900
Wire Wire Line
	4250 2200 4900 2200
Connection ~ 4400 2200
Wire Wire Line
	4900 1700 4400 1700
$Comp
L R R9001
U 1 1 58A5EC2B
P 4550 1550
F 0 "R9001" H 4620 1596 50  0000 L CNN
F 1 "47k" H 4620 1505 50  0000 L CNN
F 2 "Resistors_SMD:R_0805" V -370 200 50  0001 C CNN
F 3 "" H -300 200 50  0001 C CNN
	1    4550 1550
	1    0    0    -1  
$EndComp
$Comp
L +3V3 #PWR9001
U 1 1 58A5EC6F
P 4550 1400
F 0 "#PWR9001" H -400 -200 50  0001 C CNN
F 1 "+3V3" H 4565 1573 50  0000 C CNN
F 2 "" H -400 -50 50  0001 C CNN
F 3 "" H -400 -50 50  0001 C CNN
	1    4550 1400
	1    0    0    -1  
$EndComp
$EndSCHEMATC
