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
Sheet 9 9
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
L CONN_01X03 P9
U 1 1 57070354
P 5600 3300
F 0 "P9" H 5678 3338 50  0000 L CNN
F 1 "i2c bus" H 5678 3247 50  0000 L CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x03" H 5600 3300 50  0001 C CNN
F 3 "" H 5600 3300 50  0000 C CNN
	1    5600 3300
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR066
U 1 1 570703B8
P 5350 3450
F 0 "#PWR066" H 5350 3200 50  0001 C CNN
F 1 "GND" H 5358 3277 50  0000 C CNN
F 2 "" H 5350 3450 50  0000 C CNN
F 3 "" H 5350 3450 50  0000 C CNN
	1    5350 3450
	1    0    0    -1  
$EndComp
Wire Wire Line
	5350 3450 5350 3400
Wire Wire Line
	5350 3400 5400 3400
Text HLabel 5000 3200 0    60   BiDi ~ 0
sda
Text HLabel 5000 3300 0    60   BiDi ~ 0
scl
Wire Wire Line
	5000 3300 5400 3300
Wire Wire Line
	5000 3200 5400 3200
$EndSCHEMATC
