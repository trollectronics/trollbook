EESchema Schematic File Version 4
LIBS:peripheral-board-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 4 8
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
L peripheral-board-rescue:BARREL_JACK-RESCUE-peripheral-board CON4001
U 1 1 581A8CF2
P 1500 1650
F 0 "CON4001" H 1481 1975 50  0000 C CNN
F 1 "BARREL_JACK" H 1481 1884 50  0000 C CNN
F 2 "Connect:JACK_ALIM" H 1500 1650 50  0001 C CNN
F 3 "" H 1500 1650 50  0000 C CNN
	1    1500 1650
	1    0    0    -1  
$EndComp
$Comp
L peripheral-board-rescue:AP1509-RESCUE-peripheral-board U4001
U 1 1 58224E1C
P 7900 5150
F 0 "U4001" H 7900 5200 60  0000 C CNN
F 1 "AP1509" H 7900 5100 60  0000 C CNN
F 2 "Housings_SOIC:SOIC-8_3.9x4.9mm_Pitch1.27mm" H 7900 5150 60  0001 C CNN
F 3 "" H 7900 5150 60  0001 C CNN
	1    7900 5150
	1    0    0    -1  
$EndComp
$Comp
L peripheral-board-rescue:LM27313-RESCUE-peripheral-board U4002
U 1 1 582259B3
P 3000 5050
F 0 "U4002" H 3000 5100 60  0000 C CNN
F 1 "LM27313" H 3000 5000 60  0000 C CNN
F 2 "TO_SOT_Packages_SMD:SOT-23-5" H 3000 5050 60  0001 C CNN
F 3 "" H 3000 5050 60  0001 C CNN
	1    3000 5050
	1    0    0    -1  
$EndComp
Text HLabel 7300 5150 0    60   Input ~ 0
SHDN_3
$Comp
L peripheral-board-rescue:LT3652-RESCUE-peripheral-board U4003
U 1 1 5899C8AB
P 5300 2450
F 0 "U4003" H 5300 2500 60  0000 C CNN
F 1 "LT3652" H 5300 2400 60  0000 C CNN
F 2 "Housings_SSOP:MSOP-12-1EP_3x4mm_Pitch0.65mm" H -3550 -100 60  0001 C CNN
F 3 "" H -3550 -100 60  0001 C CNN
	1    5300 2450
	1    0    0    -1  
$EndComp
$Comp
L peripheral-board-rescue:D-RESCUE-peripheral-board D4006
U 1 1 589A0D43
P 6450 2200
F 0 "D4006" H 6450 2100 50  0000 C CNN
F 1 "1n4148" H 6450 2325 50  0000 C CNN
F 2 "Diodes_SMD:D_MiniMELF_Standard" H -1250 50  50  0001 C CNN
F 3 "" H -1250 50  50  0001 C CNN
	1    6450 2200
	1    0    0    -1  
$EndComp
$Comp
L peripheral-board-rescue:D_Schottky-RESCUE-peripheral-board D4005
U 1 1 589A1463
P 6900 2000
F 0 "D4005" H 6900 2216 50  0000 C CNN
F 1 "RBE2VA20ATR" H 6900 2125 50  0000 C CNN
F 2 "Diodes_SMD:D_TUMD2" H -750 350 50  0001 C CNN
F 3 "" H -750 350 50  0001 C CNN
	1    6900 2000
	1    0    0    -1  
$EndComp
$Comp
L peripheral-board-rescue:L-RESCUE-peripheral-board L4001
U 1 1 589A1B1D
P 6700 2200
F 0 "L4001" H 6753 2246 50  0000 L CNN
F 1 "8u2" H 6753 2155 50  0000 L CNN
F 2 "Choke_SMD:Choke_SMD_6.3x6.3_H3" H -750 850 50  0001 C CNN
F 3 "" H -750 850 50  0001 C CNN
	1    6700 2200
	1    0    0    -1  
$EndComp
$Comp
L peripheral-board-rescue:C_Small-RESCUE-peripheral-board C4002
U 1 1 589A1C63
P 6050 2100
F 0 "C4002" H 6050 2300 50  0000 L CNN
F 1 "1u" H 6142 2055 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206" H -1100 400 50  0001 C CNN
F 3 "" H -1100 400 50  0001 C CNN
	1    6050 2100
	1    0    0    -1  
$EndComp
$Comp
L peripheral-board-rescue:GND-RESCUE-peripheral-board #PWR036
U 1 1 589A1FA0
P 7050 2000
F 0 "#PWR036" H -1000 100 50  0001 C CNN
F 1 "GND" V 7055 1872 50  0000 R CNN
F 2 "" H -1000 350 50  0001 C CNN
F 3 "" H -1000 350 50  0001 C CNN
	1    7050 2000
	0    -1   -1   0   
$EndComp
$Comp
L peripheral-board-rescue:R-RESCUE-peripheral-board R4007
U 1 1 589A2225
P 6200 2750
F 0 "R4007" H 6250 2800 50  0000 L CNN
F 1 "626k" H 6270 2705 50  0000 L CNN
F 2 "Resistors_SMD:R_0805" V -1720 200 50  0001 C CNN
F 3 "" H -1650 200 50  0001 C CNN
	1    6200 2750
	1    0    0    -1  
$EndComp
$Comp
L peripheral-board-rescue:R-RESCUE-peripheral-board R4009
U 1 1 589A22C3
P 6550 2900
F 0 "R4009" V 6650 2900 50  0000 C CNN
F 1 "412k" V 6550 2900 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V -1520 400 50  0001 C CNN
F 3 "" H -1450 400 50  0001 C CNN
	1    6550 2900
	0    1    1    0   
$EndComp
$Comp
L peripheral-board-rescue:GND-RESCUE-peripheral-board #PWR037
U 1 1 589A2378
P 7200 2900
F 0 "#PWR037" H 7250 2950 50  0001 C CNN
F 1 "GND" V 7200 2700 50  0000 C CNN
F 2 "" H -750 100 50  0001 C CNN
F 3 "" H -750 100 50  0001 C CNN
	1    7200 2900
	0    -1   -1   0   
$EndComp
$Comp
L peripheral-board-rescue:C-RESCUE-peripheral-board C4003
U 1 1 589A242E
P 6800 2750
F 0 "C4003" H 6800 2850 50  0000 L CNN
F 1 "10u" H 6800 2650 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206" H -1112 100 50  0001 C CNN
F 3 "" H -1150 250 50  0001 C CNN
	1    6800 2750
	1    0    0    -1  
$EndComp
$Comp
L peripheral-board-rescue:CP-RESCUE-peripheral-board C4004
U 1 1 589A24A0
P 7150 2750
F 0 "C4004" H 7150 2850 50  0000 L CNN
F 1 "100u" H 7300 2750 50  0000 L CNN
F 2 "Capacitors_Tantalum_SMD:TantalC_SizeD_EIA-7343_Reflow" H -962 150 50  0001 C CNN
F 3 "" H -1000 300 50  0001 C CNN
	1    7150 2750
	1    0    0    -1  
$EndComp
$Comp
L peripheral-board-rescue:R-RESCUE-peripheral-board R4005
U 1 1 589A2E18
P 6950 2400
F 0 "R4005" V 6850 2400 50  0000 C CNN
F 1 "0R05" V 6950 2400 50  0000 C CNN
F 2 "Resistors_SMD:R_1812" V -770 0   50  0001 C CNN
F 3 "" H -700 0   50  0001 C CNN
	1    6950 2400
	0    -1   -1   0   
$EndComp
$Comp
L peripheral-board-rescue:+BATT-RESCUE-peripheral-board #PWR038
U 1 1 589A3E06
P 7500 2350
F 0 "#PWR038" H -350 800 50  0001 C CNN
F 1 "+BATT" H 7515 2523 50  0000 C CNN
F 2 "" H -350 950 50  0001 C CNN
F 3 "" H -350 950 50  0001 C CNN
	1    7500 2350
	1    0    0    -1  
$EndComp
$Comp
L peripheral-board-rescue:GND-RESCUE-peripheral-board #PWR039
U 1 1 589A4F62
P 5300 3300
F 0 "#PWR039" H -750 -100 50  0001 C CNN
F 1 "GND" H 5305 3127 50  0000 C CNN
F 2 "" H -750 150 50  0001 C CNN
F 3 "" H -750 150 50  0001 C CNN
	1    5300 3300
	1    0    0    -1  
$EndComp
$Comp
L peripheral-board-rescue:C-RESCUE-peripheral-board C4005
U 1 1 589A5433
P 4550 3100
F 0 "C4005" H 4665 3146 50  0000 L CNN
F 1 "680n" H 4665 3055 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 138 100 50  0001 C CNN
F 3 "" H 100 250 50  0001 C CNN
	1    4550 3100
	1    0    0    -1  
$EndComp
$Comp
L peripheral-board-rescue:R-RESCUE-peripheral-board R4003
U 1 1 589A5A9A
P 4450 2000
F 0 "R4003" V 4350 2000 50  0000 C CNN
F 1 "10k" V 4450 2000 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V -270 450 50  0001 C CNN
F 3 "" H -200 450 50  0001 C CNN
	1    4450 2000
	0    1    1    0   
$EndComp
$Comp
L peripheral-board-rescue:R-RESCUE-peripheral-board R4004
U 1 1 589A5C55
P 4450 2100
F 0 "R4004" V 4550 2100 50  0000 C CNN
F 1 "10k" V 4450 2100 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V -270 550 50  0001 C CNN
F 3 "" H -200 550 50  0001 C CNN
	1    4450 2100
	0    1    1    0   
$EndComp
$Comp
L peripheral-board-rescue:D_Schottky-RESCUE-peripheral-board D4002
U 1 1 589A6E37
P 3550 1550
F 0 "D4002" H 3550 1334 50  0000 C CNN
F 1 "RBE2VA20ATR" H 3550 1425 50  0000 C CNN
F 2 "Diodes_SMD:D_TUMD2" H -600 350 50  0001 C CNN
F 3 "" H -600 350 50  0001 C CNN
	1    3550 1550
	-1   0    0    1   
$EndComp
$Comp
L peripheral-board-rescue:C-RESCUE-peripheral-board C4001
U 1 1 589A7064
P 3300 1750
F 0 "C4001" H 3415 1796 50  0000 L CNN
F 1 "1u" H 3415 1705 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206" H -1162 700 50  0001 C CNN
F 3 "" H -1200 850 50  0001 C CNN
	1    3300 1750
	1    0    0    -1  
$EndComp
$Comp
L peripheral-board-rescue:GND-RESCUE-peripheral-board #PWR040
U 1 1 589A711D
P 3300 1900
F 0 "#PWR040" H -250 -1350 50  0001 C CNN
F 1 "GND" H 3305 1727 50  0000 C CNN
F 2 "" H -250 -1100 50  0001 C CNN
F 3 "" H -250 -1100 50  0001 C CNN
	1    3300 1900
	1    0    0    -1  
$EndComp
$Comp
L peripheral-board-rescue:R-RESCUE-peripheral-board R4006
U 1 1 589A73D6
P 3050 2500
F 0 "R4006" H 3120 2546 50  0000 L CNN
F 1 "47k" H 3120 2455 50  0000 L CNN
F 2 "Resistors_SMD:R_0805" V -2620 1400 50  0001 C CNN
F 3 "" H -2550 1400 50  0001 C CNN
	1    3050 2500
	1    0    0    -1  
$EndComp
$Comp
L peripheral-board-rescue:R-RESCUE-peripheral-board R4002
U 1 1 589A76A9
P 3300 1200
F 0 "R4002" H 3370 1246 50  0000 L CNN
F 1 "330k" H 3370 1155 50  0000 L CNN
F 2 "Resistors_SMD:R_0805" V -2720 400 50  0001 C CNN
F 3 "" H -2650 400 50  0001 C CNN
	1    3300 1200
	1    0    0    -1  
$EndComp
$Comp
L peripheral-board-rescue:R-RESCUE-peripheral-board R4008
U 1 1 589A799A
P 3050 2900
F 0 "R4008" H 3120 2946 50  0000 L CNN
F 1 "51k" H 3120 2855 50  0000 L CNN
F 2 "Resistors_SMD:R_0805" V -470 100 50  0001 C CNN
F 3 "" H -400 100 50  0001 C CNN
	1    3050 2900
	1    0    0    -1  
$EndComp
$Comp
L peripheral-board-rescue:GND-RESCUE-peripheral-board #PWR041
U 1 1 589A7BC3
P 3050 3050
F 0 "#PWR041" H -500 -200 50  0001 C CNN
F 1 "GND" H 3055 2877 50  0000 C CNN
F 2 "" H -500 50  50  0001 C CNN
F 3 "" H -500 50  50  0001 C CNN
	1    3050 3050
	1    0    0    -1  
$EndComp
$Comp
L peripheral-board-rescue:GND-RESCUE-peripheral-board #PWR042
U 1 1 589A855C
P 1850 1800
F 0 "#PWR042" H -850 -600 50  0001 C CNN
F 1 "GND" H 1855 1627 50  0000 C CNN
F 2 "" H -850 -350 50  0001 C CNN
F 3 "" H -850 -350 50  0001 C CNN
	1    1850 1800
	1    0    0    -1  
$EndComp
$Comp
L peripheral-board-rescue:+12V-RESCUE-peripheral-board #PWR043
U 1 1 589A8BB3
P 2150 1300
F 0 "#PWR043" H -950 100 50  0001 C CNN
F 1 "+12V" H 2165 1473 50  0000 C CNN
F 2 "" H -950 250 50  0001 C CNN
F 3 "" H -950 250 50  0001 C CNN
	1    2150 1300
	1    0    0    -1  
$EndComp
$Comp
L peripheral-board-rescue:PWR_FLAG-RESCUE-peripheral-board #FLG044
U 1 1 589A8CB7
P 1850 1300
F 0 "#FLG044" H -900 75  50  0001 C CNN
F 1 "PWR_FLAG" H 1850 1474 50  0000 C CNN
F 2 "" H -900 0   50  0001 C CNN
F 3 "" H -900 0   50  0001 C CNN
	1    1850 1300
	1    0    0    -1  
$EndComp
Text Notes 3650 1200 0    60   ~ 0
Change value to set dropout voltage\n
Text Notes 2000 2750 0    60   ~ 0
Add shutdown input\n
Text Label 6800 3050 0    60   ~ 0
BATTERY_NTC
$Comp
L peripheral-board-rescue:PWR_FLAG-RESCUE-peripheral-board #FLG045
U 1 1 589AA4ED
P 7150 2350
F 0 "#FLG045" H 7200 2400 50  0001 C CNN
F 1 "PWR_FLAG" H 7150 2524 50  0000 C CNN
F 2 "" H -900 -650 50  0001 C CNN
F 3 "" H -900 -650 50  0001 C CNN
	1    7150 2350
	1    0    0    -1  
$EndComp
$Comp
L peripheral-board-rescue:D_Schottky-RESCUE-peripheral-board D4001
U 1 1 589B3608
P 9800 1500
F 0 "D4001" V 9850 1350 50  0000 C CNN
F 1 "RBE2VA20ATR" V 9750 1200 50  0000 C CNN
F 2 "Diodes_SMD:D_TUMD2" H 2150 -150 50  0001 C CNN
F 3 "" H 2150 -150 50  0001 C CNN
	1    9800 1500
	0    -1   -1   0   
$EndComp
$Comp
L peripheral-board-rescue:+BATT-RESCUE-peripheral-board #PWR046
U 1 1 589B38CB
P 8200 1650
F 0 "#PWR046" H 350 100 50  0001 C CNN
F 1 "+BATT" H 8215 1823 50  0000 C CNN
F 2 "" H 350 250 50  0001 C CNN
F 3 "" H 350 250 50  0001 C CNN
	1    8200 1650
	0    -1   -1   0   
$EndComp
$Comp
L peripheral-board-rescue:+12V-RESCUE-peripheral-board #PWR047
U 1 1 589B39F6
P 8200 1350
F 0 "#PWR047" H 5100 150 50  0001 C CNN
F 1 "+12V" H 8215 1523 50  0000 C CNN
F 2 "" H 5100 300 50  0001 C CNN
F 3 "" H 5100 300 50  0001 C CNN
	1    8200 1350
	0    -1   -1   0   
$EndComp
$Comp
L peripheral-board-rescue:R-RESCUE-peripheral-board R4001
U 1 1 589B4CD6
P 9250 1100
F 0 "R4001" V 9457 1100 50  0000 C CNN
F 1 "10k" V 9366 1100 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 4780 -2600 50  0001 C CNN
F 3 "" H 4850 -2600 50  0001 C CNN
	1    9250 1100
	0    -1   -1   0   
$EndComp
$Comp
L peripheral-board-rescue:GND-RESCUE-peripheral-board #PWR048
U 1 1 589B4E54
P 9100 1100
F 0 "#PWR048" H 5850 -2700 50  0001 C CNN
F 1 "GND" V 9105 972 50  0000 R CNN
F 2 "" H 5850 -2450 50  0001 C CNN
F 3 "" H 5850 -2450 50  0001 C CNN
	1    9100 1100
	0    1    1    0   
$EndComp
$Comp
L peripheral-board-rescue:PWR_FLAG-RESCUE-peripheral-board #FLG049
U 1 1 589B64BF
P 10350 1650
F 0 "#FLG049" H 6000 -2675 50  0001 C CNN
F 1 "PWR_FLAG" H 10350 1824 50  0000 C CNN
F 2 "" H 6000 -2750 50  0001 C CNN
F 3 "" H 6000 -2750 50  0001 C CNN
	1    10350 1650
	1    0    0    -1  
$EndComp
$Comp
L peripheral-board-rescue:D_Schottky-RESCUE-peripheral-board D4008
U 1 1 589B7900
P 8600 5400
F 0 "D4008" V 8554 5479 50  0000 L CNN
F 1 "RBE2VA20ATR" V 8645 5479 50  0000 L CNN
F 2 "Diodes_SMD:D_TUMD2" H 300 -200 50  0001 C CNN
F 3 "" H 300 -200 50  0001 C CNN
	1    8600 5400
	0    1    1    0   
$EndComp
$Comp
L peripheral-board-rescue:L-RESCUE-peripheral-board L4003
U 1 1 589B7AE5
P 8950 5200
F 0 "L4003" V 8900 5200 50  0000 C CNN
F 1 "47u" V 9000 5050 50  0000 C CNN
F 2 "Choke_SMD:Choke_SMD_12x12mm_h6mm" H 100 -200 50  0001 C CNN
F 3 "" H 100 -200 50  0001 C CNN
	1    8950 5200
	0    -1   -1   0   
$EndComp
$Comp
L peripheral-board-rescue:CP-RESCUE-peripheral-board C4010
U 1 1 589B7C32
P 9300 5400
F 0 "C4010" H 9418 5446 50  0000 L CNN
F 1 "100u" H 9418 5355 50  0000 L CNN
F 2 "Capacitors_SMD:c_elec_6.3x5.8" H 1088 -350 50  0001 C CNN
F 3 "" H 1050 -200 50  0001 C CNN
	1    9300 5400
	1    0    0    -1  
$EndComp
$Comp
L peripheral-board-rescue:CP-RESCUE-peripheral-board C4007
U 1 1 589B7D69
P 7700 4600
F 0 "C4007" V 7445 4600 50  0000 C CNN
F 1 "100u" V 7536 4600 50  0000 C CNN
F 2 "Capacitors_Tantalum_SMD:TantalC_SizeD_EIA-7343_Reflow" H 2438 -1550 50  0001 C CNN
F 3 "" H 2400 -1400 50  0001 C CNN
	1    7700 4600
	0    1    1    0   
$EndComp
$Comp
L peripheral-board-rescue:GND-RESCUE-peripheral-board #PWR050
U 1 1 589B8109
P 7550 4600
F 0 "#PWR050" H 7600 4650 50  0001 C CNN
F 1 "GND" V 7555 4472 50  0000 R CNN
F 2 "" H 500 -200 50  0001 C CNN
F 3 "" H 500 -200 50  0001 C CNN
	1    7550 4600
	0    1    1    0   
$EndComp
$Comp
L peripheral-board-rescue:GND-RESCUE-peripheral-board #PWR051
U 1 1 589B85D2
P 7750 5750
F 0 "#PWR051" H 7800 5800 50  0001 C CNN
F 1 "GND" H 7755 5577 50  0000 C CNN
F 2 "" H 650 -750 50  0001 C CNN
F 3 "" H 650 -750 50  0001 C CNN
	1    7750 5750
	1    0    0    -1  
$EndComp
$Comp
L peripheral-board-rescue:+3V3-RESCUE-peripheral-board #PWR052
U 1 1 589B91D5
P 9900 5200
F 0 "#PWR052" H 9950 5250 50  0001 C CNN
F 1 "+3V3" H 9915 5373 50  0000 C CNN
F 2 "" H 1700 -500 50  0001 C CNN
F 3 "" H 1700 -500 50  0001 C CNN
	1    9900 5200
	1    0    0    -1  
$EndComp
$Comp
L peripheral-board-rescue:PWR_FLAG-RESCUE-peripheral-board #FLG053
U 1 1 589B9237
P 10000 5200
F 0 "#FLG053" H 10050 5250 50  0001 C CNN
F 1 "PWR_FLAG" H 10000 5374 50  0000 C CNN
F 2 "" H 1750 -550 50  0001 C CNN
F 3 "" H 1750 -550 50  0001 C CNN
	1    10000 5200
	0    1    1    0   
$EndComp
$Comp
L peripheral-board-rescue:C-RESCUE-peripheral-board C4006
U 1 1 589BA27A
P 2800 4450
F 0 "C4006" V 2548 4450 50  0000 C CNN
F 1 "2.2u" V 2639 4450 50  0000 C CNN
F 2 "Capacitors_SMD:C_1206" H -1512 -500 50  0001 C CNN
F 3 "" H -1550 -350 50  0001 C CNN
	1    2800 4450
	0    1    1    0   
$EndComp
$Comp
L peripheral-board-rescue:L-RESCUE-peripheral-board L4002
U 1 1 589BA4F8
P 3600 4450
F 0 "L4002" V 3790 4450 50  0000 C CNN
F 1 "8u2" V 3699 4450 50  0000 C CNN
F 2 "Choke_SMD:Choke_SMD_6.3x6.3_H3" H -1850 -550 50  0001 C CNN
F 3 "" H -1850 -550 50  0001 C CNN
	1    3600 4450
	0    -1   -1   0   
$EndComp
$Comp
L peripheral-board-rescue:D_Schottky-RESCUE-peripheral-board D4007
U 1 1 589BA689
P 3950 5000
F 0 "D4007" H 3950 4784 50  0000 C CNN
F 1 "NSR0530HT1G" H 3950 4875 50  0000 C CNN
F 2 "Diodes_SMD:D_SOD-323" H -400 -450 50  0001 C CNN
F 3 "" H -400 -450 50  0001 C CNN
	1    3950 5000
	-1   0    0    1   
$EndComp
$Comp
L peripheral-board-rescue:GND-RESCUE-peripheral-board #PWR054
U 1 1 589BA9B7
P 2650 4450
F 0 "#PWR054" H 2700 4500 50  0001 C CNN
F 1 "GND" V 2655 4322 50  0000 R CNN
F 2 "" H -4450 -2050 50  0001 C CNN
F 3 "" H -4450 -2050 50  0001 C CNN
	1    2650 4450
	0    1    1    0   
$EndComp
$Comp
L peripheral-board-rescue:GND-RESCUE-peripheral-board #PWR055
U 1 1 589BAA61
P 3000 5750
F 0 "#PWR055" H 3050 5800 50  0001 C CNN
F 1 "GND" H 3005 5577 50  0000 C CNN
F 2 "" H -4100 -750 50  0001 C CNN
F 3 "" H -4100 -750 50  0001 C CNN
	1    3000 5750
	1    0    0    -1  
$EndComp
$Comp
L peripheral-board-rescue:R-RESCUE-peripheral-board R4012
U 1 1 589BAB6A
P 3950 5100
F 0 "R4012" V 3850 5150 50  0000 C CNN
F 1 "220k" V 3950 5100 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V -1070 -400 50  0001 C CNN
F 3 "" H -1000 -400 50  0001 C CNN
	1    3950 5100
	0    -1   -1   0   
$EndComp
$Comp
L peripheral-board-rescue:R-RESCUE-peripheral-board R4013
U 1 1 589BAE7E
P 3750 5550
F 0 "R4013" H 3450 5550 50  0000 L CNN
F 1 "12k" H 3500 5450 50  0000 L CNN
F 2 "Resistors_SMD:R_0805" V -1420 -200 50  0001 C CNN
F 3 "" H -1350 -200 50  0001 C CNN
	1    3750 5550
	1    0    0    -1  
$EndComp
$Comp
L peripheral-board-rescue:C-RESCUE-peripheral-board C4008
U 1 1 589BB298
P 3950 5300
F 0 "C4008" V 4100 5300 50  0000 C CNN
F 1 "120p" V 4000 5450 50  0000 C CNN
F 2 "Capacitors_SMD:C_0805" H -912 -350 50  0001 C CNN
F 3 "" H -950 -200 50  0001 C CNN
	1    3950 5300
	0    1    1    0   
$EndComp
$Comp
L peripheral-board-rescue:C-RESCUE-peripheral-board C4009
U 1 1 589BB6C3
P 4400 5300
F 0 "C4009" H 4515 5346 50  0000 L CNN
F 1 "10u" H 4515 5255 50  0000 L CNN
F 2 "Capacitors_SMD:C_1206" H -662 -650 50  0001 C CNN
F 3 "" H -700 -500 50  0001 C CNN
	1    4400 5300
	1    0    0    -1  
$EndComp
$Comp
L peripheral-board-rescue:+24V-RESCUE-peripheral-board #PWR056
U 1 1 589BBC33
P 4800 5000
F 0 "#PWR056" H 4850 5050 50  0001 C CNN
F 1 "+24V" H 4815 5173 50  0000 C CNN
F 2 "" H -100 -50 50  0001 C CNN
F 3 "" H -100 -50 50  0001 C CNN
	1    4800 5000
	1    0    0    -1  
$EndComp
$Comp
L peripheral-board-rescue:PWR_FLAG-RESCUE-peripheral-board #FLG057
U 1 1 589BBD5B
P 4500 5000
F 0 "#FLG057" H 4550 5050 50  0001 C CNN
F 1 "PWR_FLAG" H 4500 5174 50  0000 C CNN
F 2 "" H -400 -550 50  0001 C CNN
F 3 "" H -400 -550 50  0001 C CNN
	1    4500 5000
	1    0    0    -1  
$EndComp
Text HLabel 2300 5050 0    60   Input ~ 0
~SHDN_24
$Comp
L peripheral-board-rescue:VAA-RESCUE-peripheral-board #PWR058
U 1 1 58A8EB71
P 10600 1650
F 0 "#PWR058" H 10650 1700 50  0001 C CNN
F 1 "VAA" H 10617 1823 50  0000 C CNN
F 2 "" H -250 900 50  0001 C CNN
F 3 "" H -250 900 50  0001 C CNN
	1    10600 1650
	1    0    0    -1  
$EndComp
$Comp
L peripheral-board-rescue:VAA-RESCUE-peripheral-board #PWR059
U 1 1 58A8ECBE
P 7900 4400
F 0 "#PWR059" H 7950 4450 50  0001 C CNN
F 1 "VAA" H 7917 4573 50  0000 C CNN
F 2 "" H -2950 3650 50  0001 C CNN
F 3 "" H -2950 3650 50  0001 C CNN
	1    7900 4400
	1    0    0    -1  
$EndComp
$Comp
L peripheral-board-rescue:VAA-RESCUE-peripheral-board #PWR060
U 1 1 58A8F0E4
P 3000 4400
F 0 "#PWR060" H 3050 4450 50  0001 C CNN
F 1 "VAA" H 3017 4573 50  0000 C CNN
F 2 "" H -7850 3650 50  0001 C CNN
F 3 "" H -7850 3650 50  0001 C CNN
	1    3000 4400
	1    0    0    -1  
$EndComp
$Comp
L peripheral-board-rescue:Q_PMOS_GSD-RESCUE-peripheral-board Q4001
U 1 1 58A96BE8
P 9400 1550
F 0 "Q4001" V 9650 1550 50  0000 C CNN
F 1 "IRLML2244TRPbF" V 9741 1550 50  0000 C CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H -800 -150 50  0001 C CNN
F 3 "" H -1000 -250 50  0001 C CNN
	1    9400 1550
	0    -1   1    0   
$EndComp
$Comp
L peripheral-board-rescue:CP-RESCUE-peripheral-board C4011
U 1 1 58A4BBDE
P 9800 5400
F 0 "C4011" H 9918 5446 50  0000 L CNN
F 1 "100u" H 9918 5355 50  0000 L CNN
F 2 "Capacitors_SMD:c_elec_6.3x5.8" H 1588 -350 50  0001 C CNN
F 3 "" H 1550 -200 50  0001 C CNN
	1    9800 5400
	1    0    0    -1  
$EndComp
$Comp
L peripheral-board-rescue:LED_Dual_CAC-RESCUE-peripheral-board D4003
U 1 1 58A4B915
P 4000 2050
F 0 "D4003" H 4000 1650 50  0000 C CNN
F 1 "SSL-LX3059IGW-CA" H 4000 1750 50  0000 C CNN
F 2 "LEDs:LED-5MM-3" H -1900 1050 50  0001 C CNN
F 3 "" H -1900 1050 50  0001 C CNN
	1    4000 2050
	1    0    0    -1  
$EndComp
Text Label 6000 2000 0    60   ~ 0
CHRG_SW
Text Label 6000 2400 0    60   ~ 0
CHRG_SENSE
NoConn ~ 1800 1650
$Comp
L peripheral-board-rescue:PWR_FLAG-RESCUE-peripheral-board #FLG061
U 1 1 58A9FA40
P 5350 1000
F 0 "#FLG061" H -1000 225 50  0001 C CNN
F 1 "PWR_FLAG" H 5350 1174 50  0000 C CNN
F 2 "" H -1000 150 50  0001 C CNN
F 3 "" H -1000 150 50  0001 C CNN
	1    5350 1000
	1    0    0    -1  
$EndComp
$Comp
L peripheral-board-rescue:PWR_FLAG-RESCUE-peripheral-board #FLG062
U 1 1 58A9FB2F
P 5250 1550
F 0 "#FLG062" H -1100 775 50  0001 C CNN
F 1 "PWR_FLAG" H 5250 1724 50  0000 C CNN
F 2 "" H -1100 700 50  0001 C CNN
F 3 "" H -1100 700 50  0001 C CNN
	1    5250 1550
	1    0    0    -1  
$EndComp
$Comp
L peripheral-board-rescue:CONN_01X05-RESCUE-peripheral-board J4001
U 1 1 58AB7B70
P 7850 1900
F 0 "J4001" V 7815 1612 50  0000 R CNN
F 1 "53047-0510" V 7724 1612 50  0000 R CNN
F 2 "Connectors_Molex:Connector_Molex_PicoBlade_53047-0510" H -2350 -250 50  0001 C CNN
F 3 "" H -2350 -250 50  0001 C CNN
	1    7850 1900
	0    -1   -1   0   
$EndComp
Wire Wire Line
	6700 2900 7200 2900
Connection ~ 7150 2900
Connection ~ 6800 2900
Wire Wire Line
	7150 2600 6000 2600
Connection ~ 6800 2600
Connection ~ 6200 2600
Wire Wire Line
	6000 2900 6400 2900
Connection ~ 6200 2900
Wire Wire Line
	6000 2800 6050 2800
Wire Wire Line
	6000 2400 6800 2400
Wire Wire Line
	6700 2400 6700 2350
Wire Wire Line
	7150 2350 7150 2600
Connection ~ 6700 2400
Connection ~ 7150 2400
Wire Wire Line
	6700 2050 6700 2000
Wire Wire Line
	6000 2000 6750 2000
Connection ~ 6700 2000
Connection ~ 6050 2000
Wire Wire Line
	6000 2200 6300 2200
Connection ~ 6050 2200
Wire Wire Line
	6600 2200 6600 2600
Connection ~ 6600 2600
Wire Wire Line
	7500 2350 7500 2400
Connection ~ 7500 2400
Wire Wire Line
	4600 2900 4550 2900
Wire Wire Line
	4550 2900 4550 2950
Wire Wire Line
	5250 1650 5250 1550
Wire Wire Line
	5250 1550 3700 1550
Wire Wire Line
	1800 1550 3400 1550
Wire Wire Line
	3300 1350 3300 1600
Connection ~ 3300 1550
Wire Wire Line
	4600 2700 3050 2700
Wire Wire Line
	3050 2650 3050 2750
Connection ~ 3050 2700
Wire Wire Line
	3050 1000 3050 2350
Wire Wire Line
	3050 1000 5350 1000
Wire Wire Line
	5350 1000 5350 1650
Wire Wire Line
	3300 1050 3300 1000
Connection ~ 3300 1000
Wire Wire Line
	1800 1750 1850 1750
Wire Wire Line
	1850 1750 1850 1800
Wire Wire Line
	1850 1550 1850 1300
Wire Wire Line
	2150 1550 2150 1300
Connection ~ 1850 1550
Connection ~ 2150 1550
Wire Wire Line
	8150 3050 6050 3050
Wire Wire Line
	6050 3050 6050 2800
Wire Wire Line
	4550 3250 8100 3250
Connection ~ 5300 3250
Wire Wire Line
	5300 3300 5300 3250
Connection ~ 9800 1650
Wire Wire Line
	9200 1650 8200 1650
Wire Wire Line
	8200 1350 9800 1350
Connection ~ 9400 1350
Wire Wire Line
	9400 1100 9400 1350
Wire Wire Line
	3000 5600 3000 5750
Wire Wire Line
	9600 1650 10600 1650
Wire Wire Line
	7850 4600 7900 4600
Wire Wire Line
	7900 4400 7900 4650
Connection ~ 7900 4600
Wire Wire Line
	7750 5700 9800 5700
Connection ~ 7850 5700
Connection ~ 7950 5700
Wire Wire Line
	9300 5700 9300 5550
Connection ~ 8050 5700
Wire Wire Line
	8600 5550 8600 5700
Connection ~ 8600 5700
Wire Wire Line
	7750 5750 7750 5700
Wire Wire Line
	8500 5100 9300 5100
Wire Wire Line
	9300 5100 9300 5250
Wire Wire Line
	9100 5200 10000 5200
Connection ~ 9300 5200
Wire Wire Line
	8800 5200 8500 5200
Wire Wire Line
	8600 5250 8600 5200
Connection ~ 8600 5200
Wire Wire Line
	3700 5000 3800 5000
Wire Wire Line
	3750 5000 3750 4450
Connection ~ 3750 5000
Wire Wire Line
	3000 4400 3000 4500
Wire Wire Line
	3700 5100 3800 5100
Wire Wire Line
	3750 5100 3750 5400
Connection ~ 3750 5100
Connection ~ 3000 5700
Wire Wire Line
	4100 5100 4800 5100
Wire Wire Line
	4150 5000 4150 5300
Wire Wire Line
	4150 5000 4100 5000
Wire Wire Line
	4400 5100 4400 5150
Connection ~ 4150 5100
Wire Wire Line
	4400 5700 4400 5450
Connection ~ 3750 5700
Wire Wire Line
	4150 5300 4100 5300
Wire Wire Line
	3750 5300 3800 5300
Connection ~ 3750 5300
Wire Wire Line
	4800 5100 4800 5000
Connection ~ 4400 5100
Wire Wire Line
	4500 5000 4500 5100
Connection ~ 4500 5100
Wire Wire Line
	4400 5700 3000 5700
Wire Wire Line
	9800 5250 9800 5200
Connection ~ 9800 5200
Wire Wire Line
	9800 5700 9800 5550
Connection ~ 9300 5700
Connection ~ 9900 5200
Wire Wire Line
	3700 1550 3700 2050
Connection ~ 3700 1550
Wire Wire Line
	4300 2000 4300 1950
Wire Wire Line
	4300 2100 4300 2150
Wire Wire Line
	2950 4450 3450 4450
Connection ~ 3000 4450
Connection ~ 10350 1650
Wire Wire Line
	7650 2100 7650 3250
Connection ~ 7650 3250
Wire Wire Line
	7750 2100 7750 2400
Connection ~ 7750 2400
Wire Wire Line
	7850 2100 7850 2400
Connection ~ 7850 2400
Wire Wire Line
	7950 2100 7950 3050
Connection ~ 7950 3050
Wire Wire Line
	8050 3250 8050 2100
Wire Wire Line
	8150 3150 8100 3150
Wire Wire Line
	8100 3150 8100 3250
Connection ~ 8050 3250
Wire Wire Line
	7100 2400 8150 2400
$EndSCHEMATC
