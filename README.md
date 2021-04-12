# Embedded-Systems
Software simulation of embedded systems using IAR<br><br>

## Lab 1
Write a register level program to calculate each
bit address of Port F Data register, where the red,
green, blue LEDs are connected. Also light each
led with different delay 1 sec, 2sec, 3 sec.

## Lab 2
Write a program to get different colour with
combination of led lighting. This operation you
have carried out using a user switch SW1 on
PF4 pin.

## Lab 3
Activate the locked switch also from LAB 2 program. Now use both the user switches to operate the LED in different cobination. 

## Lab 4
DAC design using R-2R ladder circuit. 

## Lab 5
ADC to Digital conversion.

## Lab 6
### A
Write a PWM program to gradually increase the LED contrast and gradually decrease. Once one LED
contrast gradually deceased anther colour LED do the same. (Repeat it same for all onboard LED on
TM4C123GH). (Also try to create different duty cycle PWM using LOAD register).

### B
Write an Analog Input Based Control to glow each LED at different threshold of Analog Input Value.

### C
Write a PWM Control to control the motor speed by using the PWM signalAdd onboard two switches, SW1 and SW2, to the project.
-SW1 is used for changing the motor speed - when the user presses the SW1, the duty cycle changes
from 0%, 30%, 60% to 100%, and then back to 0%.
-SW2 is used for changing the motor direction - when the user presses the SW2, change the motor
rotation direction. (Do not change the rotation direction if the duty cycle is 0%)

## Lab 7
UART communication between TM4C and PC. In this practice problem, you have to implement a GUI application using any programming language. On the GUI front end, you have keep three buttons and one 2-D plot. Buttons controls the onboard LEDS and 2-D plot shows the analog voltage in line graph.

## Lab 8
UART communication on same board or two different devices. 

## Lab 9
SPI communication on same board or two different devices. Master send a message to control an LED on slave device. If same board wants to be used SPI, then use SPI models 0 and model 1 and drive the onboard LEDS. Use UART to show the message send from master to a slave device.
