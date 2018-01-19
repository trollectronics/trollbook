EESchema Schematic File Version 4
LIBS:peripheral-board-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 3 8
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
L peripheral-board-rescue:PSPNUB-RESCUE-peripheral-board U3001
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
L peripheral-board-rescue:+3V3-RESCUE-peripheral-board #PWR034
U 1 1 582269F3
P 5100 1800
F 0 "#PWR034" H 5100 1650 50  0001 C CNN
F 1 "+3V3" H 5115 1973 50  0000 C CNN
F 2 "" H 5100 1800 50  0000 C CNN
F 3 "" H 5100 1800 50  0000 C CNN
	1    5100 1800
	1    0    0    -1  
$EndComp
$Comp
L peripheral-board-rescue:GND-RESCUE-peripheral-board #PWR035
U 1 1 58226A0A
P 5100 3400
F 0 "#PWR035" H 5100 3150 50  0001 C CNN
F 1 "GND" H 5105 3227 50  0000 C CNN
F 2 "" H 5100 3400 50  0000 C CNN
F 3 "" H 5100 3400 50  0000 C CNN
	1    5100 3400
	1    0    0    -1  
$EndComp
Text HLabel 6050 2550 2    60   Output ~ 0
X
Text HLabel 6050 2650 2    60   Output ~ 0
Y
Wire Wire Line
	6050 2550 5600 2550
Wire Wire Line
	5600 2650 6050 2650
$Comp
L peripheral-board-rescue:R-RESCUE-peripheral-board R3001
U 1 1 58E56E4E
P 5100 1950
F 0 "R3001" H 5170 1996 50  0000 L CNN
F 1 "0" H 5170 1905 50  0000 L CNN
F 2 "Resistors_SMD:R_1206" V 5030 1950 50  0001 C CNN
F 3 "" H 5100 1950 50  0001 C CNN
	1    5100 1950
	1    0    0    -1  
$EndComp
$Comp
L peripheral-board-rescue:R-RESCUE-peripheral-board R3002
U 1 1 58E56F28
P 5100 3250
F 0 "R3002" H 5170 3296 50  0000 L CNN
F 1 "0" H 5170 3205 50  0000 L CNN
F 2 "Resistors_SMD:R_1206" V 5030 3250 50  0001 C CNN
F 3 "" H 5100 3250 50  0001 C CNN
	1    5100 3250
	1    0    0    -1  
$EndComp
$EndSCHEMATC
