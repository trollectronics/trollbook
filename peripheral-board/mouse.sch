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
Sheet 3 10
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
L PSPNUB U3001
U 1 1 5822698D
P 5100 2600
F 0 "U3001" H 4772 2653 60  0000 R CNN
F 1 "PSPNUB" H 4772 2547 60  0000 R CNN
F 2 "lib:PSPNUB" H 5100 2600 60  0001 C CNN
F 3 "" H 5100 2600 60  0001 C CNN
	1    5100 2600
	1    0    0    -1  
$EndComp
$Comp
L +3V3 #PWR029
U 1 1 582269F3
P 5100 2050
F 0 "#PWR029" H 5100 1900 50  0001 C CNN
F 1 "+3V3" H 5115 2223 50  0000 C CNN
F 2 "" H 5100 2050 50  0000 C CNN
F 3 "" H 5100 2050 50  0000 C CNN
	1    5100 2050
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR030
U 1 1 58226A0A
P 5100 3150
F 0 "#PWR030" H 5100 2900 50  0001 C CNN
F 1 "GND" H 5105 2977 50  0000 C CNN
F 2 "" H 5100 3150 50  0000 C CNN
F 3 "" H 5100 3150 50  0000 C CNN
	1    5100 3150
	1    0    0    -1  
$EndComp
Wire Wire Line
	5100 2050 5100 2100
Wire Wire Line
	5100 3100 5100 3150
Text HLabel 6050 2550 2    60   Output ~ 0
X
Text HLabel 6050 2650 2    60   Output ~ 0
Y
Wire Wire Line
	6050 2550 5600 2550
Wire Wire Line
	5600 2650 6050 2650
$EndSCHEMATC
