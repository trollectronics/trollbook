EESchema Schematic File Version 4
LIBS:battery-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
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
L device:Battery_Cell BT5001
U 1 1 589ABFFA
P 7000 2800
F 0 "BT5001" H 7118 2896 50  0000 L CNN
F 1 "LiPo 2200mAh 51x68x5" H 7118 2805 50  0000 L CNN
F 2 "Socket_Strips:Socket_Strip_Straight_1x02_Pitch1.27mm" V -3000 810 50  0001 C CNN
F 3 "" V -3000 810 50  0001 C CNN
	1    7000 2800
	1    0    0    -1  
$EndComp
$Comp
L references:TL431DBZ U5002
U 1 1 589AC001
P 5550 2800
F 0 "U5002" V 5600 2450 60  0000 L CNN
F 1 "TL431" V 5400 2450 60  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H -4350 1000 60  0001 C CNN
F 3 "" H -4350 1000 60  0001 C CNN
	1    5550 2800
	0    -1   -1   0   
$EndComp
$Comp
L device:POT RV5001
U 1 1 589AC008
P 5000 2800
F 0 "RV5001" H 4930 2846 50  0000 R CNN
F 1 "47k" H 4930 2755 50  0000 R CNN
F 2 "lib:3364W" H -4200 1800 50  0001 C CNN
F 3 "" H -4200 1800 50  0001 C CNN
	1    5000 2800
	1    0    0    -1  
$EndComp
$Comp
L device:R R5002
U 1 1 589AC00F
P 5000 2500
F 0 "R5002" H 5070 2546 50  0000 L CNN
F 1 "150k" H 5070 2455 50  0000 L CNN
F 2 "Resistors_SMD:R_0805" V -3770 1300 50  0001 C CNN
F 3 "" H -3700 1300 50  0001 C CNN
	1    5000 2500
	1    0    0    -1  
$EndComp
$Comp
L device:R R5005
U 1 1 589AC016
P 5000 3100
F 0 "R5005" H 5070 3146 50  0000 L CNN
F 1 "220k" H 5070 3055 50  0000 L CNN
F 2 "Resistors_SMD:R_0805" V -3720 650 50  0001 C CNN
F 3 "" H -3650 650 50  0001 C CNN
	1    5000 3100
	1    0    0    -1  
$EndComp
$Comp
L device:R R5001
U 1 1 589AC01D
P 5550 2300
F 0 "R5001" H 5620 2346 50  0000 L CNN
F 1 "10k" H 5620 2255 50  0000 L CNN
F 2 "Resistors_SMD:R_0805" V -3720 1100 50  0001 C CNN
F 3 "" H -3650 1100 50  0001 C CNN
	1    5550 2300
	1    0    0    -1  
$EndComp
$Comp
L device:R R5003
U 1 1 589AC024
P 6100 2900
F 0 "R5003" H 6170 2946 50  0000 L CNN
F 1 "40R" H 6170 2855 50  0000 L CNN
F 2 "Resistors_SMD:R_0805" V -3870 800 50  0001 C CNN
F 3 "" H -3800 800 50  0001 C CNN
	1    6100 2900
	1    0    0    -1  
$EndComp
Wire Wire Line
	5000 3250 5000 3300
Wire Wire Line
	5000 3300 5550 3300
Wire Wire Line
	6100 3050 6100 3300
Wire Wire Line
	6100 2750 6100 2700
Connection ~ 6100 3300
Wire Wire Line
	5000 2350 5000 2150
Wire Wire Line
	3350 2150 3500 2150
Wire Wire Line
	7000 2150 7000 2600
Connection ~ 5550 2150
Wire Wire Line
	6100 2300 6100 2150
Connection ~ 6100 2150
Wire Wire Line
	5800 2500 5550 2500
Wire Wire Line
	5550 2450 5550 2500
Connection ~ 5550 2500
Wire Wire Line
	5150 2800 5450 2800
Wire Wire Line
	5550 2900 5550 3300
Connection ~ 5550 3300
$Comp
L device:Battery_Cell BT5002
U 1 1 589AC03D
P 7000 3950
F 0 "BT5002" H 7118 4046 50  0000 L CNN
F 1 "LiPo 2200mAh 51x68x5" H 7118 3955 50  0000 L CNN
F 2 "Socket_Strips:Socket_Strip_Straight_1x02_Pitch1.27mm" V -3000 1960 50  0001 C CNN
F 3 "" V -3000 1960 50  0001 C CNN
	1    7000 3950
	1    0    0    -1  
$EndComp
$Comp
L references:TL431DBZ U5003
U 1 1 589AC044
P 5550 3950
F 0 "U5003" V 5600 3600 60  0000 L CNN
F 1 "TL431" V 5400 3600 60  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H -4350 2150 60  0001 C CNN
F 3 "" H -4350 2150 60  0001 C CNN
	1    5550 3950
	0    -1   -1   0   
$EndComp
$Comp
L device:POT RV5002
U 1 1 589AC04B
P 5000 3950
F 0 "RV5002" H 4930 3996 50  0000 R CNN
F 1 "47k" H 4930 3905 50  0000 R CNN
F 2 "lib:3364W" H -4200 2950 50  0001 C CNN
F 3 "" H -4200 2950 50  0001 C CNN
	1    5000 3950
	1    0    0    -1  
$EndComp
$Comp
L device:R R5007
U 1 1 589AC052
P 5000 3650
F 0 "R5007" H 5070 3696 50  0000 L CNN
F 1 "150k" H 5070 3605 50  0000 L CNN
F 2 "Resistors_SMD:R_0805" V -3770 2450 50  0001 C CNN
F 3 "" H -3700 2450 50  0001 C CNN
	1    5000 3650
	1    0    0    -1  
$EndComp
$Comp
L device:R R5010
U 1 1 589AC059
P 5000 4250
F 0 "R5010" H 5070 4296 50  0000 L CNN
F 1 "220k" H 5070 4205 50  0000 L CNN
F 2 "Resistors_SMD:R_0805" V -3720 1800 50  0001 C CNN
F 3 "" H -3650 1800 50  0001 C CNN
	1    5000 4250
	1    0    0    -1  
$EndComp
$Comp
L device:R R5006
U 1 1 589AC060
P 5550 3450
F 0 "R5006" H 5620 3496 50  0000 L CNN
F 1 "10k" H 5620 3405 50  0000 L CNN
F 2 "Resistors_SMD:R_0805" V -3720 2250 50  0001 C CNN
F 3 "" H -3650 2250 50  0001 C CNN
	1    5550 3450
	1    0    0    -1  
$EndComp
$Comp
L device:R R5008
U 1 1 589AC067
P 6100 4050
F 0 "R5008" H 6170 4096 50  0000 L CNN
F 1 "40R" H 6170 4005 50  0000 L CNN
F 2 "Resistors_SMD:R_0805" V -3870 1950 50  0001 C CNN
F 3 "" H -3800 1950 50  0001 C CNN
	1    6100 4050
	1    0    0    -1  
$EndComp
Wire Wire Line
	5000 4400 5000 4450
Wire Wire Line
	3350 4450 3450 4450
Wire Wire Line
	6100 4450 6100 4200
Wire Wire Line
	6100 3900 6100 3850
Connection ~ 6100 4450
Wire Wire Line
	5800 3650 5550 3650
Wire Wire Line
	5550 3600 5550 3650
Connection ~ 5550 3650
Wire Wire Line
	5150 3950 5450 3950
Wire Wire Line
	5550 4050 5550 4450
Connection ~ 5550 4450
$Comp
L device:Thermistor_NTC TH5001
U 1 1 589AC07A
P 4150 4250
F 0 "TH5001" H 4247 4296 50  0000 L CNN
F 1 "10k" H 4247 4205 50  0000 L CNN
F 2 "Capacitors_ThroughHole:CP_Radial_Tantal_D4.5mm_P2.50mm" H -3550 3150 50  0001 C CNN
F 3 "" H -3550 3150 50  0001 C CNN
	1    4150 4250
	1    0    0    -1  
$EndComp
Text HLabel 3350 2150 0    60   UnSpc ~ 0
B+
Text HLabel 3350 4450 0    60   UnSpc ~ 0
B-
Text HLabel 3350 4050 0    60   UnSpc ~ 0
NTC
Connection ~ 5000 4450
Connection ~ 5000 2150
Wire Wire Line
	4150 4100 4150 4050
Wire Wire Line
	4150 4050 3700 4050
Wire Wire Line
	4150 4400 4150 4450
Connection ~ 4150 4450
Wire Wire Line
	7000 4450 7000 4050
Wire Wire Line
	7000 2900 7000 3300
Connection ~ 7000 3300
$Comp
L device:R R5004
U 1 1 589AEB2B
P 6400 2900
F 0 "R5004" H 6470 2946 50  0000 L CNN
F 1 "40R" H 6470 2855 50  0000 L CNN
F 2 "Resistors_SMD:R_0805" V -3570 800 50  0001 C CNN
F 3 "" H -3500 800 50  0001 C CNN
	1    6400 2900
	1    0    0    -1  
$EndComp
Wire Wire Line
	6400 2750 6100 2750
Wire Wire Line
	6400 3050 6100 3050
$Comp
L device:R R5009
U 1 1 589AEC5C
P 6400 4050
F 0 "R5009" H 6470 4096 50  0000 L CNN
F 1 "40R" H 6470 4005 50  0000 L CNN
F 2 "Resistors_SMD:R_0805" V -3570 1950 50  0001 C CNN
F 3 "" H -3500 1950 50  0001 C CNN
	1    6400 4050
	1    0    0    -1  
$EndComp
Wire Wire Line
	6400 3900 6100 3900
Wire Wire Line
	6100 4200 6400 4200
Text Notes 6150 3150 0    60   ~ 0
2 × 1/4 W
Text Notes 6150 4300 0    60   ~ 0
2 × 1/4 W
$Comp
L device:Q_PMOS_GSD Q5001
U 1 1 58A97AA9
P 6000 2500
F 0 "Q5001" H 6206 2454 50  0000 L CNN
F 1 "IRLML2244TRPbF" H 6206 2545 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H -850 700 50  0001 C CNN
F 3 "" H -1050 600 50  0001 C CNN
	1    6000 2500
	1    0    0    1   
$EndComp
$Comp
L device:Q_PMOS_GSD Q5002
U 1 1 58A97BDE
P 6000 3650
F 0 "Q5002" H 6206 3604 50  0000 L CNN
F 1 "IRLML2244TRPbF" H 6206 3695 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H -850 1850 50  0001 C CNN
F 3 "" H -1050 1750 50  0001 C CNN
	1    6000 3650
	1    0    0    1   
$EndComp
Text Notes 7150 2950 0    60   ~ 0
505168
Text Notes 7150 4100 0    60   ~ 0
505168
Text Notes 3700 3050 0    60   ~ 0
pot: 3364W-1-503E
$Comp
L battery-rescue:CONN_01X03-RESCUE-battery J1001
U 1 1 58E61493
P 2200 3250
F 0 "J1001" H 2119 2925 50  0000 C CNN
F 1 "CONN_01X03" H 2119 3016 50  0000 C CNN
F 2 "Socket_Strips:Socket_Strip_Straight_1x03_Pitch2.54mm" H 2200 3250 50  0001 C CNN
F 3 "" H 2200 3250 50  0001 C CNN
	1    2200 3250
	-1   0    0    1   
$EndComp
Wire Wire Line
	2400 3150 3500 3150
Wire Wire Line
	3500 3150 3500 2150
Connection ~ 3500 2150
Wire Wire Line
	2400 3250 3700 3250
Wire Wire Line
	3700 3250 3700 4050
Connection ~ 3700 4050
Wire Wire Line
	2400 3350 3450 3350
Wire Wire Line
	3450 3350 3450 4450
Connection ~ 3450 4450
Connection ~ 5000 3300
Wire Wire Line
	6100 3300 7000 3300
Wire Wire Line
	6100 3300 6100 3450
Wire Wire Line
	5550 2150 6100 2150
Wire Wire Line
	6100 2150 7000 2150
Wire Wire Line
	5550 2500 5550 2700
Wire Wire Line
	5550 3300 6100 3300
Wire Wire Line
	6100 4450 7000 4450
Wire Wire Line
	5550 3650 5550 3850
Wire Wire Line
	5550 4450 6100 4450
Wire Wire Line
	5000 4450 5550 4450
Wire Wire Line
	5000 2150 5550 2150
Wire Wire Line
	4150 4450 5000 4450
Wire Wire Line
	7000 3300 7000 3750
Wire Wire Line
	3500 2150 5000 2150
Wire Wire Line
	3700 4050 3350 4050
Wire Wire Line
	3450 4450 4150 4450
Wire Wire Line
	5000 3300 5000 3500
$EndSCHEMATC
