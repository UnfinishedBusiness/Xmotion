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
Sheet 7 9
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
L CONN_01X03 P4
U 1 1 5702A181
P 5450 2800
F 0 "P4" H 5528 2838 50  0000 L CNN
F 1 "serial bus" H 5528 2747 50  0000 L CNN
F 2 "Socket_Strips:Socket_Strip_Straight_1x03" H 5450 2800 50  0001 C CNN
F 3 "" H 5450 2800 50  0000 C CNN
	1    5450 2800
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR061
U 1 1 5702A1E8
P 5200 2950
F 0 "#PWR061" H 5200 2700 50  0001 C CNN
F 1 "GND" H 5208 2777 50  0000 C CNN
F 2 "" H 5200 2950 50  0000 C CNN
F 3 "" H 5200 2950 50  0000 C CNN
	1    5200 2950
	1    0    0    -1  
$EndComp
Wire Wire Line
	5200 2950 5200 2900
Wire Wire Line
	5200 2900 5250 2900
Wire Wire Line
	4550 2800 5250 2800
Wire Wire Line
	4550 2700 5250 2700
Text HLabel 4550 2700 0    60   Input ~ 0
rx
Text HLabel 4550 2800 0    60   Output ~ 0
tx
$EndSCHEMATC
