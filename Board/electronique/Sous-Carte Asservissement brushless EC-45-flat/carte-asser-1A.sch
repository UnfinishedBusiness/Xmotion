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
Sheet 1 9
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Sheet
S 8000 2350 900  1100
U 57002469
F0 "brushless-part" 60
F1 "brushless-part.sch" 60
F2 "motor1_enable" I L 8000 2650 60 
F3 "motor1_direction" I L 8000 2750 60 
F4 "motor2_enable" I L 8000 3150 60 
F5 "motor2_direction" I L 8000 3250 60 
F6 "motor1_ready" O L 8000 2550 60 
F7 "motor2_ready" O L 8000 3050 60 
F8 "motor1_speed" I L 8000 2850 60 
F9 "motor2_speed" I L 8000 3350 60 
$EndSheet
$Sheet
S 5000 2150 2100 2500
U 57016C72
F0 "uC-part" 60
F1 "uC-part.sch" 60
F2 "usb_dp" B L 5000 3450 60 
F3 "usb_dm" B L 5000 3350 60 
F4 "i2c_sda" B L 5000 2950 60 
F5 "i2c_scl" B L 5000 2850 60 
F6 "tim2_ch2_encoder" I R 7100 3950 60 
F7 "tim2_ch1_encoder" I R 7100 3850 60 
F8 "tim3_ch2_encoder" I R 7100 4250 60 
F9 "tim3_ch1_encoder" I R 7100 4150 60 
F10 "usart2_rx" I L 5000 3900 60 
F11 "usart2_tx" O L 5000 4000 60 
F12 "gpio_direction1" O R 7100 2750 60 
F13 "gpio_direction2" O R 7100 3250 60 
F14 "gpio_enable" O R 7100 2650 60 
F15 "can_tx" O L 5000 2550 60 
F16 "can_rx" I L 5000 2450 60 
F17 "gpio_ready1" I R 7100 2550 60 
F18 "gpio_ready2" I R 7100 3050 60 
F19 "pwm2" O R 7100 3350 60 
F20 "pwm1" O R 7100 2850 60 
$EndSheet
$Sheet
S 3400 2350 550  300 
U 57018916
F0 "can-part" 60
F1 "can-part.sch" 60
F2 "rx" I R 3950 2450 60 
F3 "tx" O R 3950 2550 60 
$EndSheet
$Sheet
S 3400 3250 550  300 
U 57018934
F0 "usb-part" 60
F1 "usb-part.sch" 60
F2 "dm" B R 3950 3350 60 
F3 "dp" B R 3950 3450 60 
$EndSheet
$Sheet
S 5000 1300 2100 400 
U 57013891
F0 "power-part" 60
F1 "power-part.sch" 60
$EndSheet
Wire Wire Line
	7100 2550 8000 2550
Wire Wire Line
	7100 2650 8000 2650
Wire Wire Line
	7100 2750 8000 2750
Wire Wire Line
	7100 2850 8000 2850
Wire Wire Line
	7100 3050 8000 3050
Wire Wire Line
	7100 3250 8000 3250
Wire Wire Line
	7100 3350 8000 3350
Wire Wire Line
	7550 3150 8000 3150
Wire Wire Line
	3950 3350 5000 3350
Wire Wire Line
	5000 3450 3950 3450
Wire Wire Line
	7550 3150 7550 2650
Connection ~ 7550 2650
$Sheet
S 3400 3800 550  300 
U 5702837B
F0 "serial-part" 60
F1 "serial-part.sch" 60
F2 "rx" I R 3950 3900 60 
F3 "tx" O R 3950 4000 60 
$EndSheet
Wire Wire Line
	3950 2450 4450 2450
Wire Wire Line
	4450 2450 4550 2550
Wire Wire Line
	3950 2550 4450 2550
Wire Wire Line
	4450 2550 4550 2450
Wire Wire Line
	4550 2550 5000 2550
Wire Wire Line
	4550 2450 5000 2450
Wire Wire Line
	3950 3900 4500 3900
Wire Wire Line
	4500 3900 4600 4000
Wire Wire Line
	3950 4000 4500 4000
Wire Wire Line
	4500 4000 4600 3900
Wire Wire Line
	4600 4000 5000 4000
Wire Wire Line
	4600 3900 5000 3900
$Sheet
S 8000 3700 750  850 
U 5707AF1F
F0 "encoder-part" 60
F1 "encoder-part.sch" 60
F2 "encoder1_1" O L 8000 3850 60 
F3 "encoder1_2" O L 8000 3950 60 
F4 "encoder2_1" O L 8000 4150 60 
F5 "encoder2_2" O L 8000 4250 60 
$EndSheet
Wire Wire Line
	7100 3850 8000 3850
Wire Wire Line
	7100 3950 8000 3950
Wire Wire Line
	7100 4150 8000 4150
Wire Wire Line
	7100 4250 8000 4250
Wire Wire Line
	5000 2950 3950 2950
Wire Wire Line
	3950 2850 5000 2850
$Sheet
S 3400 2800 550  200 
U 570700AB
F0 "i2c-part" 60
F1 "i2c-part.sch" 60
F2 "sda" B R 3950 2950 60 
F3 "scl" B R 3950 2850 60 
$EndSheet
$EndSCHEMATC
