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
LIBS:2
LIBS:stm32
LIBS:carte-asser-1A-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 8 9
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
L CONN_01X04 P7
U 1 1 5707E3EA
P 5400 2550
F 0 "P7" H 5478 2588 50  0000 L CNN
F 1 "encoder 1" H 5478 2497 50  0000 L CNN
F 2 "Socket_Strips:Socket_Strip_Straight_1x04" H 5400 2550 50  0001 C CNN
F 3 "" H 5400 2550 50  0000 C CNN
	1    5400 2550
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR062
U 1 1 5707E412
P 5150 2350
F 0 "#PWR062" H 5150 2200 50  0001 C CNN
F 1 "+5V" H 5168 2523 50  0000 C CNN
F 2 "" H 5150 2350 50  0000 C CNN
F 3 "" H 5150 2350 50  0000 C CNN
	1    5150 2350
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR063
U 1 1 5707E44F
P 5150 2750
F 0 "#PWR063" H 5150 2500 50  0001 C CNN
F 1 "GND" H 5158 2577 50  0000 C CNN
F 2 "" H 5150 2750 50  0000 C CNN
F 3 "" H 5150 2750 50  0000 C CNN
	1    5150 2750
	1    0    0    -1  
$EndComp
Wire Wire Line
	5150 2750 5150 2700
Wire Wire Line
	5150 2700 5200 2700
Wire Wire Line
	5200 2400 5150 2400
Wire Wire Line
	5150 2400 5150 2350
Text HLabel 4800 2500 0    60   Output ~ 0
encoder1_1
Text HLabel 4800 2600 0    60   Output ~ 0
encoder1_2
Wire Wire Line
	4800 2500 5200 2500
Wire Wire Line
	5200 2600 4800 2600
$Comp
L CONN_01X04 P8
U 1 1 5707E4D2
P 5400 3400
F 0 "P8" H 5478 3438 50  0000 L CNN
F 1 "encoder 2" H 5478 3347 50  0000 L CNN
F 2 "Socket_Strips:Socket_Strip_Straight_1x04" H 5400 3400 50  0001 C CNN
F 3 "" H 5400 3400 50  0000 C CNN
	1    5400 3400
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR064
U 1 1 5707E4D8
P 5150 3200
F 0 "#PWR064" H 5150 3050 50  0001 C CNN
F 1 "+5V" H 5168 3373 50  0000 C CNN
F 2 "" H 5150 3200 50  0000 C CNN
F 3 "" H 5150 3200 50  0000 C CNN
	1    5150 3200
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR065
U 1 1 5707E4DE
P 5150 3600
F 0 "#PWR065" H 5150 3350 50  0001 C CNN
F 1 "GND" H 5158 3427 50  0000 C CNN
F 2 "" H 5150 3600 50  0000 C CNN
F 3 "" H 5150 3600 50  0000 C CNN
	1    5150 3600
	1    0    0    -1  
$EndComp
Wire Wire Line
	5150 3600 5150 3550
Wire Wire Line
	5150 3550 5200 3550
Wire Wire Line
	5200 3250 5150 3250
Wire Wire Line
	5150 3250 5150 3200
Text HLabel 4800 3350 0    60   Output ~ 0
encoder2_1
Text HLabel 4800 3450 0    60   Output ~ 0
encoder2_2
Wire Wire Line
	4800 3350 5200 3350
Wire Wire Line
	5200 3450 4800 3450
$EndSCHEMATC
