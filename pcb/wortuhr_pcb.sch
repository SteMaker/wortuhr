EESchema Schematic File Version 4
EELAYER 30 0
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
L RF_Module:ESP-12F U2
U 1 1 5E7F6779
P 3050 2200
F 0 "U2" H 3150 3075 50  0000 C CNN
F 1 "ESP-12F" H 3275 2975 50  0000 C CNN
F 2 "RF_Module:ESP-12E" H 3050 2200 50  0001 C CNN
F 3 "http://wiki.ai-thinker.com/_media/esp8266/esp8266_series_modules_user_manual_v1.1.pdf" H 2700 2300 50  0001 C CNN
	1    3050 2200
	1    0    0    -1  
$EndComp
$Comp
L Regulator_Linear:LM1117-3.3 U1
U 1 1 5E7F7098
P 2525 3775
F 0 "U1" H 2525 4017 50  0000 C CNN
F 1 "LM1117-3.3" H 2525 3926 50  0000 C CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Vertical" H 2525 3775 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/lm1117.pdf" H 2525 3775 50  0001 C CNN
	1    2525 3775
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x02_Female X1
U 1 1 5E7F6F31
P 800 3900
F 0 "X1" H 793 3559 50  0000 C CNN
F 1 "250-x02" H 793 3650 50  0000 C CNN
F 2 "stemaker_connectoren:WAGO_250-402" H 650 3575 50  0001 L BNN
F 3 "" H 800 3400 50  0001 L BNN
	1    800  3900
	-1   0    0    1   
$EndComp
$Comp
L Device:Fuse F1
U 1 1 5E7F7F60
P 1375 3775
F 0 "F1" V 1178 3775 50  0000 C CNN
F 1 "Fuse" V 1269 3775 50  0000 C CNN
F 2 "stemaker_fuse:RND_fuse_holder_5x20mm_horizontal" V 1305 3775 50  0001 C CNN
F 3 "~" H 1375 3775 50  0001 C CNN
	1    1375 3775
	0    1    1    0   
$EndComp
$Comp
L Device:CP C2
U 1 1 5E7FA606
P 2975 3925
F 0 "C2" H 3093 3971 50  0000 L CNN
F 1 "1µF" H 3093 3880 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D5.0mm_P2.50mm" H 3013 3775 50  0001 C CNN
F 3 "~" H 2975 3925 50  0001 C CNN
	1    2975 3925
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole_Pad H11
U 1 1 5E7FC38B
P 950 3350
F 0 "H11" V 1187 3353 50  0000 C CNN
F 1 "MountingHole_Pad" V 1096 3353 50  0000 C CNN
F 2 "TestPoint:TestPoint_THTPad_2.5x2.5mm_Drill1.2mm" H 950 3350 50  0001 C CNN
F 3 "~" H 950 3350 50  0001 C CNN
	1    950  3350
	0    -1   -1   0   
$EndComp
$Comp
L Mechanical:MountingHole_Pad H10
U 1 1 5E7FCD66
P 950 3100
F 0 "H10" V 1187 3103 50  0000 C CNN
F 1 "MountingHole_Pad" V 1096 3103 50  0000 C CNN
F 2 "TestPoint:TestPoint_THTPad_2.5x2.5mm_Drill1.2mm" H 950 3100 50  0001 C CNN
F 3 "~" H 950 3100 50  0001 C CNN
	1    950  3100
	0    -1   -1   0   
$EndComp
$Comp
L Mechanical:MountingHole_Pad H9
U 1 1 5E7FD08A
P 950 2850
F 0 "H9" V 1187 2853 50  0000 C CNN
F 1 "MountingHole_Pad" V 1096 2853 50  0000 C CNN
F 2 "TestPoint:TestPoint_THTPad_2.5x2.5mm_Drill1.2mm" H 950 2850 50  0001 C CNN
F 3 "~" H 950 2850 50  0001 C CNN
	1    950  2850
	0    -1   -1   0   
$EndComp
$Comp
L Mechanical:MountingHole_Pad H8
U 1 1 5E7FD2E6
P 950 2600
F 0 "H8" V 1187 2603 50  0000 C CNN
F 1 "MountingHole_Pad" V 1096 2603 50  0000 C CNN
F 2 "TestPoint:TestPoint_THTPad_2.5x2.5mm_Drill1.2mm" H 950 2600 50  0001 C CNN
F 3 "~" H 950 2600 50  0001 C CNN
	1    950  2600
	0    -1   -1   0   
$EndComp
$Comp
L Mechanical:MountingHole_Pad H7
U 1 1 5E7FE6F2
P 950 2350
F 0 "H7" V 1187 2353 50  0000 C CNN
F 1 "MountingHole_Pad" V 1096 2353 50  0000 C CNN
F 2 "TestPoint:TestPoint_THTPad_2.5x2.5mm_Drill1.2mm" H 950 2350 50  0001 C CNN
F 3 "~" H 950 2350 50  0001 C CNN
	1    950  2350
	0    -1   -1   0   
$EndComp
$Comp
L Mechanical:MountingHole_Pad H6
U 1 1 5E7FEA4C
P 950 2100
F 0 "H6" V 1187 2103 50  0000 C CNN
F 1 "MountingHole_Pad" V 1096 2103 50  0000 C CNN
F 2 "TestPoint:TestPoint_THTPad_2.5x2.5mm_Drill1.2mm" H 950 2100 50  0001 C CNN
F 3 "~" H 950 2100 50  0001 C CNN
	1    950  2100
	0    -1   -1   0   
$EndComp
$Comp
L Mechanical:MountingHole_Pad H5
U 1 1 5E8044AF
P 950 1850
F 0 "H5" V 1187 1853 50  0000 C CNN
F 1 "MountingHole_Pad" V 1096 1853 50  0000 C CNN
F 2 "TestPoint:TestPoint_THTPad_2.5x2.5mm_Drill1.2mm" H 950 1850 50  0001 C CNN
F 3 "~" H 950 1850 50  0001 C CNN
	1    950  1850
	0    -1   -1   0   
$EndComp
$Comp
L Mechanical:MountingHole_Pad H4
U 1 1 5E80476C
P 950 1600
F 0 "H4" V 1187 1603 50  0000 C CNN
F 1 "MountingHole_Pad" V 1096 1603 50  0000 C CNN
F 2 "TestPoint:TestPoint_THTPad_2.5x2.5mm_Drill1.2mm" H 950 1600 50  0001 C CNN
F 3 "~" H 950 1600 50  0001 C CNN
	1    950  1600
	0    -1   -1   0   
$EndComp
$Comp
L Mechanical:MountingHole_Pad H3
U 1 1 5E804CD0
P 950 1350
F 0 "H3" V 1187 1353 50  0000 C CNN
F 1 "MountingHole_Pad" V 1096 1353 50  0000 C CNN
F 2 "TestPoint:TestPoint_THTPad_2.5x2.5mm_Drill1.2mm" H 950 1350 50  0001 C CNN
F 3 "~" H 950 1350 50  0001 C CNN
	1    950  1350
	0    -1   -1   0   
$EndComp
$Comp
L Mechanical:MountingHole_Pad H2
U 1 1 5E804FDD
P 950 1100
F 0 "H2" V 1187 1103 50  0000 C CNN
F 1 "MountingHole_Pad" V 1096 1103 50  0000 C CNN
F 2 "TestPoint:TestPoint_THTPad_2.5x2.5mm_Drill1.2mm" H 950 1100 50  0001 C CNN
F 3 "~" H 950 1100 50  0001 C CNN
	1    950  1100
	0    -1   -1   0   
$EndComp
$Comp
L Mechanical:MountingHole_Pad H1
U 1 1 5E805395
P 950 850
F 0 "H1" V 1187 853 50  0000 C CNN
F 1 "MountingHole_Pad" V 1096 853 50  0000 C CNN
F 2 "TestPoint:TestPoint_THTPad_2.5x2.5mm_Drill1.2mm" H 950 850 50  0001 C CNN
F 3 "~" H 950 850 50  0001 C CNN
	1    950  850 
	0    -1   -1   0   
$EndComp
$Comp
L Switch:SW_Push SW1
U 1 1 5E81272C
P 2150 1600
F 0 "SW1" H 2150 1885 50  0000 C CNN
F 1 "SW_Push" H 2150 1794 50  0000 C CNN
F 2 "Button_Switch_THT:SW_PUSH_6mm_H4.3mm" H 2150 1800 50  0001 C CNN
F 3 "~" H 2150 1800 50  0001 C CNN
	1    2150 1600
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW2
U 1 1 5E813AD7
P 3975 1600
F 0 "SW2" H 3975 1885 50  0000 C CNN
F 1 "SW_Push" H 3975 1794 50  0000 C CNN
F 2 "Button_Switch_THT:SW_PUSH_6mm_H4.3mm" H 3975 1800 50  0001 C CNN
F 3 "~" H 3975 1800 50  0001 C CNN
	1    3975 1600
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x03_Male J2
U 1 1 5E81500F
P 4475 1800
F 0 "J2" H 4447 1732 50  0000 R CNN
F 1 "Conn_01x03_Male" H 4447 1823 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 4475 1800 50  0001 C CNN
F 3 "~" H 4475 1800 50  0001 C CNN
	1    4475 1800
	-1   0    0    1   
$EndComp
$Comp
L Mechanical:MountingHole_Pad H23
U 1 1 5E81A48A
P 4075 2200
F 0 "H23" V 4029 2350 50  0000 L CNN
F 1 "MountingHole_Pad" V 4120 2350 50  0000 L CNN
F 2 "TestPoint:TestPoint_THTPad_2.5x2.5mm_Drill1.2mm" H 4075 2200 50  0001 C CNN
F 3 "~" H 4075 2200 50  0001 C CNN
	1    4075 2200
	0    1    1    0   
$EndComp
Wire Wire Line
	1100 3775 1225 3775
Wire Wire Line
	1525 3775 1675 3775
Connection ~ 1675 3775
Wire Wire Line
	1675 3775 1675 3350
Wire Wire Line
	1675 3350 1050 3350
Wire Wire Line
	1050 3100 1675 3100
Wire Wire Line
	1675 3100 1675 3350
Connection ~ 1675 3350
Wire Wire Line
	1050 2850 1675 2850
Wire Wire Line
	1675 2850 1675 3100
Connection ~ 1675 3100
Wire Wire Line
	1050 2600 1675 2600
Wire Wire Line
	1675 2600 1675 2850
Connection ~ 1675 2850
Wire Wire Line
	1050 2350 1675 2350
Wire Wire Line
	1675 2350 1675 2600
Connection ~ 1675 2600
Wire Wire Line
	1050 2100 1675 2100
Wire Wire Line
	1675 2100 1675 2350
Connection ~ 1675 2350
Wire Wire Line
	1050 1850 1675 1850
Wire Wire Line
	1675 1850 1675 2100
Connection ~ 1675 2100
Wire Wire Line
	1050 1600 1675 1600
Wire Wire Line
	1675 1600 1675 1850
Connection ~ 1675 1850
Wire Wire Line
	1050 1350 1675 1350
Wire Wire Line
	1675 1350 1675 1600
Connection ~ 1675 1600
Wire Wire Line
	1050 1100 1675 1100
Wire Wire Line
	1675 1100 1675 1350
Connection ~ 1675 1350
Wire Wire Line
	1050 850  1675 850 
Wire Wire Line
	1675 850  1675 1100
Connection ~ 1675 1100
Wire Wire Line
	2450 1600 2350 1600
$Comp
L power:GND #PWR0101
U 1 1 5E81F0E3
P 1900 1650
F 0 "#PWR0101" H 1900 1400 50  0001 C CNN
F 1 "GND" H 1905 1477 50  0000 C CNN
F 2 "" H 1900 1650 50  0001 C CNN
F 3 "" H 1900 1650 50  0001 C CNN
	1    1900 1650
	1    0    0    -1  
$EndComp
Wire Wire Line
	1950 1600 1900 1600
Wire Wire Line
	1900 1600 1900 1650
Wire Wire Line
	2975 4075 2525 4075
Connection ~ 2525 4075
Wire Wire Line
	1675 4075 1450 4075
Wire Wire Line
	1450 4075 1450 3950
Connection ~ 1675 4075
Wire Wire Line
	2825 3775 2975 3775
$Comp
L power:+3.3V #PWR0103
U 1 1 5E822B85
P 2975 3650
F 0 "#PWR0103" H 2975 3500 50  0001 C CNN
F 1 "+3.3V" H 2990 3823 50  0000 C CNN
F 2 "" H 2975 3650 50  0001 C CNN
F 3 "" H 2975 3650 50  0001 C CNN
	1    2975 3650
	1    0    0    -1  
$EndComp
Wire Wire Line
	2975 3650 2975 3775
Connection ~ 2975 3775
$Comp
L Connector:Conn_01x05_Male J1
U 1 1 5E826608
P 4325 3650
F 0 "J1" H 4297 3582 50  0000 R CNN
F 1 "Conn_01x05_Male" H 4297 3673 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x05_P2.54mm_Vertical" H 4325 3650 50  0001 C CNN
F 3 "~" H 4325 3650 50  0001 C CNN
	1    4325 3650
	-1   0    0    1   
$EndComp
Wire Wire Line
	4125 3450 3650 3450
$Comp
L power:+3.3V #PWR0104
U 1 1 5E829247
P 3650 3450
F 0 "#PWR0104" H 3650 3300 50  0001 C CNN
F 1 "+3.3V" H 3665 3623 50  0000 C CNN
F 2 "" H 3650 3450 50  0001 C CNN
F 3 "" H 3650 3450 50  0001 C CNN
	1    3650 3450
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0105
U 1 1 5E8297F5
P 3650 3550
F 0 "#PWR0105" H 3650 3300 50  0001 C CNN
F 1 "GND" H 3655 3377 50  0000 C CNN
F 2 "" H 3650 3550 50  0001 C CNN
F 3 "" H 3650 3550 50  0001 C CNN
	1    3650 3550
	1    0    0    -1  
$EndComp
Wire Wire Line
	4125 3550 3650 3550
Wire Wire Line
	4125 3650 3750 3650
Wire Wire Line
	4125 3750 3750 3750
Text Label 3750 3650 0    50   ~ 0
SCL
Text Label 3750 3750 0    50   ~ 0
SDA
$Comp
L power:GND #PWR0106
U 1 1 5E82DAD0
P 3650 3850
F 0 "#PWR0106" H 3650 3600 50  0001 C CNN
F 1 "GND" H 3655 3677 50  0000 C CNN
F 2 "" H 3650 3850 50  0001 C CNN
F 3 "" H 3650 3850 50  0001 C CNN
	1    3650 3850
	1    0    0    -1  
$EndComp
Wire Wire Line
	4125 3850 3650 3850
$Comp
L power:GND #PWR0107
U 1 1 5E83240A
P 4250 1500
F 0 "#PWR0107" H 4250 1250 50  0001 C CNN
F 1 "GND" H 4255 1327 50  0000 C CNN
F 2 "" H 4250 1500 50  0001 C CNN
F 3 "" H 4250 1500 50  0001 C CNN
	1    4250 1500
	-1   0    0    1   
$EndComp
Wire Wire Line
	4250 1500 4250 1600
Wire Wire Line
	4250 1600 4175 1600
Wire Wire Line
	3775 1600 3650 1600
Wire Wire Line
	3650 1700 4275 1700
Wire Wire Line
	3650 1900 3750 1900
Wire Wire Line
	3750 1900 3750 1800
Wire Wire Line
	3750 1800 4275 1800
$Comp
L power:GND #PWR0108
U 1 1 5E838261
P 4150 1900
F 0 "#PWR0108" H 4150 1650 50  0001 C CNN
F 1 "GND" V 4125 1775 50  0000 R CNN
F 2 "" H 4150 1900 50  0001 C CNN
F 3 "" H 4150 1900 50  0001 C CNN
	1    4150 1900
	0    1    1    0   
$EndComp
Wire Wire Line
	4275 1900 4150 1900
Text Notes 4650 1700 0    50   ~ 0
serial
Wire Wire Line
	3650 2200 3975 2200
Text Notes 4375 2175 0    50   ~ 0
LED data
$Comp
L power:GND #PWR0109
U 1 1 5E83B3B0
P 3050 2975
F 0 "#PWR0109" H 3050 2725 50  0001 C CNN
F 1 "GND" H 3055 2802 50  0000 C CNN
F 2 "" H 3050 2975 50  0001 C CNN
F 3 "" H 3050 2975 50  0001 C CNN
	1    3050 2975
	1    0    0    -1  
$EndComp
Wire Wire Line
	3050 2900 3050 2975
$Comp
L power:+3.3V #PWR0110
U 1 1 5E83C8D5
P 3050 1250
F 0 "#PWR0110" H 3050 1100 50  0001 C CNN
F 1 "+3.3V" H 3065 1423 50  0000 C CNN
F 2 "" H 3050 1250 50  0001 C CNN
F 3 "" H 3050 1250 50  0001 C CNN
	1    3050 1250
	1    0    0    -1  
$EndComp
Wire Wire Line
	3050 1250 3050 1400
$Comp
L power:+3.3V #PWR0111
U 1 1 5E83E97B
P 2350 1800
F 0 "#PWR0111" H 2350 1650 50  0001 C CNN
F 1 "+3.3V" V 2365 1928 50  0000 L CNN
F 2 "" H 2350 1800 50  0001 C CNN
F 3 "" H 2350 1800 50  0001 C CNN
	1    2350 1800
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2350 1800 2450 1800
Wire Wire Line
	3650 2000 4075 2000
Wire Wire Line
	3650 2100 4075 2100
Text Label 4050 2000 2    50   ~ 0
SDA
Text Label 4075 2100 2    50   ~ 0
SCL
$Comp
L power:GND #PWR0112
U 1 1 5E843945
P 3850 2550
F 0 "#PWR0112" H 3850 2300 50  0001 C CNN
F 1 "GND" V 3825 2425 50  0000 R CNN
F 2 "" H 3850 2550 50  0001 C CNN
F 3 "" H 3850 2550 50  0001 C CNN
	1    3850 2550
	1    0    0    -1  
$EndComp
Wire Wire Line
	3650 2500 3850 2500
Wire Wire Line
	3850 2500 3850 2550
$Comp
L Mechanical:MountingHole_Pad H12
U 1 1 5E84EFF1
P 950 4475
F 0 "H12" V 1187 4478 50  0000 C CNN
F 1 "MountingHole_Pad" V 1096 4478 50  0000 C CNN
F 2 "TestPoint:TestPoint_THTPad_2.5x2.5mm_Drill1.2mm" H 950 4475 50  0001 C CNN
F 3 "~" H 950 4475 50  0001 C CNN
	1    950  4475
	0    -1   -1   0   
$EndComp
$Comp
L Mechanical:MountingHole_Pad H13
U 1 1 5E84F333
P 950 4725
F 0 "H13" V 1187 4728 50  0000 C CNN
F 1 "MountingHole_Pad" V 1096 4728 50  0000 C CNN
F 2 "TestPoint:TestPoint_THTPad_2.5x2.5mm_Drill1.2mm" H 950 4725 50  0001 C CNN
F 3 "~" H 950 4725 50  0001 C CNN
	1    950  4725
	0    -1   -1   0   
$EndComp
$Comp
L Mechanical:MountingHole_Pad H14
U 1 1 5E84F77B
P 950 4975
F 0 "H14" V 1187 4978 50  0000 C CNN
F 1 "MountingHole_Pad" V 1096 4978 50  0000 C CNN
F 2 "TestPoint:TestPoint_THTPad_2.5x2.5mm_Drill1.2mm" H 950 4975 50  0001 C CNN
F 3 "~" H 950 4975 50  0001 C CNN
	1    950  4975
	0    -1   -1   0   
$EndComp
$Comp
L Mechanical:MountingHole_Pad H15
U 1 1 5E84FF9B
P 950 5225
F 0 "H15" V 1187 5228 50  0000 C CNN
F 1 "MountingHole_Pad" V 1096 5228 50  0000 C CNN
F 2 "TestPoint:TestPoint_THTPad_2.5x2.5mm_Drill1.2mm" H 950 5225 50  0001 C CNN
F 3 "~" H 950 5225 50  0001 C CNN
	1    950  5225
	0    -1   -1   0   
$EndComp
$Comp
L Mechanical:MountingHole_Pad H16
U 1 1 5E85028D
P 950 5475
F 0 "H16" V 1187 5478 50  0000 C CNN
F 1 "MountingHole_Pad" V 1096 5478 50  0000 C CNN
F 2 "TestPoint:TestPoint_THTPad_2.5x2.5mm_Drill1.2mm" H 950 5475 50  0001 C CNN
F 3 "~" H 950 5475 50  0001 C CNN
	1    950  5475
	0    -1   -1   0   
$EndComp
$Comp
L Mechanical:MountingHole_Pad H17
U 1 1 5E85051C
P 950 5725
F 0 "H17" V 1187 5728 50  0000 C CNN
F 1 "MountingHole_Pad" V 1096 5728 50  0000 C CNN
F 2 "TestPoint:TestPoint_THTPad_2.5x2.5mm_Drill1.2mm" H 950 5725 50  0001 C CNN
F 3 "~" H 950 5725 50  0001 C CNN
	1    950  5725
	0    -1   -1   0   
$EndComp
$Comp
L Mechanical:MountingHole_Pad H18
U 1 1 5E850708
P 950 5975
F 0 "H18" V 1187 5978 50  0000 C CNN
F 1 "MountingHole_Pad" V 1096 5978 50  0000 C CNN
F 2 "TestPoint:TestPoint_THTPad_2.5x2.5mm_Drill1.2mm" H 950 5975 50  0001 C CNN
F 3 "~" H 950 5975 50  0001 C CNN
	1    950  5975
	0    -1   -1   0   
$EndComp
$Comp
L Mechanical:MountingHole_Pad H19
U 1 1 5E850AAE
P 950 6225
F 0 "H19" V 1187 6228 50  0000 C CNN
F 1 "MountingHole_Pad" V 1096 6228 50  0000 C CNN
F 2 "TestPoint:TestPoint_THTPad_2.5x2.5mm_Drill1.2mm" H 950 6225 50  0001 C CNN
F 3 "~" H 950 6225 50  0001 C CNN
	1    950  6225
	0    -1   -1   0   
$EndComp
$Comp
L Mechanical:MountingHole_Pad H20
U 1 1 5E850CFC
P 950 6475
F 0 "H20" V 1187 6478 50  0000 C CNN
F 1 "MountingHole_Pad" V 1096 6478 50  0000 C CNN
F 2 "TestPoint:TestPoint_THTPad_2.5x2.5mm_Drill1.2mm" H 950 6475 50  0001 C CNN
F 3 "~" H 950 6475 50  0001 C CNN
	1    950  6475
	0    -1   -1   0   
$EndComp
$Comp
L Mechanical:MountingHole_Pad H21
U 1 1 5E850F93
P 950 6725
F 0 "H21" V 1187 6728 50  0000 C CNN
F 1 "MountingHole_Pad" V 1096 6728 50  0000 C CNN
F 2 "TestPoint:TestPoint_THTPad_2.5x2.5mm_Drill1.2mm" H 950 6725 50  0001 C CNN
F 3 "~" H 950 6725 50  0001 C CNN
	1    950  6725
	0    -1   -1   0   
$EndComp
$Comp
L Mechanical:MountingHole_Pad H22
U 1 1 5E85118D
P 950 6975
F 0 "H22" V 1187 6978 50  0000 C CNN
F 1 "MountingHole_Pad" V 1096 6978 50  0000 C CNN
F 2 "TestPoint:TestPoint_THTPad_2.5x2.5mm_Drill1.2mm" H 950 6975 50  0001 C CNN
F 3 "~" H 950 6975 50  0001 C CNN
	1    950  6975
	0    -1   -1   0   
$EndComp
Wire Wire Line
	1675 4075 1675 4475
Wire Wire Line
	1675 4475 1050 4475
Wire Wire Line
	1050 4725 1675 4725
Wire Wire Line
	1675 4725 1675 4475
Connection ~ 1675 4475
Wire Wire Line
	1050 4975 1675 4975
Wire Wire Line
	1675 4975 1675 4725
Connection ~ 1675 4725
Wire Wire Line
	1050 5225 1675 5225
Wire Wire Line
	1675 5225 1675 4975
Connection ~ 1675 4975
Wire Wire Line
	1050 5475 1675 5475
Wire Wire Line
	1675 5475 1675 5225
Connection ~ 1675 5225
Wire Wire Line
	1050 5725 1675 5725
Wire Wire Line
	1675 5725 1675 5475
Connection ~ 1675 5475
Wire Wire Line
	1050 5975 1675 5975
Wire Wire Line
	1675 5975 1675 5725
Connection ~ 1675 5725
Wire Wire Line
	1050 6225 1675 6225
Wire Wire Line
	1675 6225 1675 5975
Connection ~ 1675 5975
Wire Wire Line
	1050 6475 1675 6475
Wire Wire Line
	1675 6475 1675 6225
Connection ~ 1675 6225
Wire Wire Line
	1050 6725 1675 6725
Wire Wire Line
	1675 6725 1675 6475
Connection ~ 1675 6475
Wire Wire Line
	1050 6975 1675 6975
Wire Wire Line
	1675 6975 1675 6725
Connection ~ 1675 6725
Wire Wire Line
	1100 3775 1100 3800
Wire Wire Line
	1100 3800 1000 3800
Wire Wire Line
	1100 3950 1100 3900
Wire Wire Line
	1100 3900 1000 3900
$Comp
L Switch:SW_Push SW3
U 1 1 5E807EA9
P 4375 2675
F 0 "SW3" H 4375 2960 50  0000 C CNN
F 1 "SW_Push" H 4375 2869 50  0000 C CNN
F 2 "Button_Switch_THT:SW_PUSH_6mm_H4.3mm" H 4375 2875 50  0001 C CNN
F 3 "~" H 4375 2875 50  0001 C CNN
	1    4375 2675
	1    0    0    -1  
$EndComp
Wire Wire Line
	3650 2400 4050 2400
Wire Wire Line
	4050 2400 4050 2675
Wire Wire Line
	4050 2675 4175 2675
$Comp
L power:GND #PWR0113
U 1 1 5E80AB25
P 4700 2675
F 0 "#PWR0113" H 4700 2425 50  0001 C CNN
F 1 "GND" H 4705 2502 50  0000 C CNN
F 2 "" H 4700 2675 50  0001 C CNN
F 3 "" H 4700 2675 50  0001 C CNN
	1    4700 2675
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4575 2675 4700 2675
Text Notes 4575 2600 0    50   ~ 0
cfg
Text Notes 3700 1350 0    50   ~ 0
flash
Text Notes 2225 1350 0    50   ~ 0
reset
Text Notes 4400 3500 0    50   ~ 0
ambient light
NoConn ~ 2450 2700
NoConn ~ 2450 2600
NoConn ~ 2450 2500
NoConn ~ 2450 2400
NoConn ~ 2450 2300
NoConn ~ 2450 2200
NoConn ~ 3650 2600
NoConn ~ 3650 2300
NoConn ~ 2450 2000
NoConn ~ 3650 1800
Text Label 1675 3475 0    50   ~ 0
LED_5V
Text Label 1675 4250 0    50   ~ 0
LED_GND
Text Label 1150 3775 1    50   ~ 0
MAIN_SUPPLY
$Comp
L Device:Net-Tie_2 NT1
U 1 1 5E8474F6
P 1875 3775
F 0 "NT1" H 1875 3956 50  0000 C CNN
F 1 "Net-Tie_2" H 1875 3865 50  0000 C CNN
F 2 "NetTie:NetTie-2_SMD_Pad2.0mm" H 1875 3775 50  0001 C CNN
F 3 "~" H 1875 3775 50  0001 C CNN
	1    1875 3775
	1    0    0    -1  
$EndComp
$Comp
L Device:Net-Tie_2 NT2
U 1 1 5E847A2B
P 1875 4075
F 0 "NT2" H 1875 4256 50  0000 C CNN
F 1 "Net-Tie_2" H 1875 4165 50  0000 C CNN
F 2 "NetTie:NetTie-2_SMD_Pad2.0mm" H 1875 4075 50  0001 C CNN
F 3 "~" H 1875 4075 50  0001 C CNN
	1    1875 4075
	1    0    0    -1  
$EndComp
Wire Wire Line
	1675 3775 1775 3775
Wire Wire Line
	1975 3775 2100 3775
Wire Wire Line
	1975 4075 2100 4075
Wire Wire Line
	1675 4075 1775 4075
$Comp
L Device:CP C1
U 1 1 5E857778
P 2100 3925
F 0 "C1" H 2218 3971 50  0000 L CNN
F 1 "1µF" H 2218 3880 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D5.0mm_P2.50mm" H 2138 3775 50  0001 C CNN
F 3 "~" H 2100 3925 50  0001 C CNN
	1    2100 3925
	1    0    0    -1  
$EndComp
Connection ~ 2100 3775
Wire Wire Line
	2100 3775 2225 3775
Connection ~ 2100 4075
Wire Wire Line
	2100 4075 2300 4075
Wire Wire Line
	1100 3950 1450 3950
$Comp
L power:GND #PWR0102
U 1 1 5E859418
P 2300 4125
F 0 "#PWR0102" H 2300 3875 50  0001 C CNN
F 1 "GND" H 2305 3952 50  0000 C CNN
F 2 "" H 2300 4125 50  0001 C CNN
F 3 "" H 2300 4125 50  0001 C CNN
	1    2300 4125
	1    0    0    -1  
$EndComp
Wire Wire Line
	2300 4125 2300 4075
Connection ~ 2300 4075
Wire Wire Line
	2300 4075 2525 4075
$Comp
L power:+5V #PWR0114
U 1 1 5E85CA8F
P 2100 3600
F 0 "#PWR0114" H 2100 3450 50  0001 C CNN
F 1 "+5V" H 2115 3773 50  0000 C CNN
F 2 "" H 2100 3600 50  0001 C CNN
F 3 "" H 2100 3600 50  0001 C CNN
	1    2100 3600
	1    0    0    -1  
$EndComp
Wire Wire Line
	2100 3600 2100 3775
$EndSCHEMATC
