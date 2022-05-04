# Alarm for Getting Up in the Morning
5780 Mini Project

Louis Vocatura
James Scholz

Alarm for Getting Up in the Morning

The purpose of this project is to make an alarm that wakes you up in the morning but must be shut off from a separate STM board. This second STM board should be a fair bit away from the alarm so the person waking up must get out of bed to shut it off. This allows the person to get up in order to wake up which helps prevent falling back to sleep.

The functionality for this project is relatively simple. There will be one STM board which controls the actual alarm. That alarm is a simple piezo buzzer. This buzzer is connected to the STM digital to analogue converter. The code provides a sine wave in the form of an array which the DAC cycles through at a specific frequency which generates a voltage wave. When put through the buzzer, an alarm will sound. It's more annoying than loud which was the point.

This STM will only cycle through this wave when receiving a uart signal from the second STM. This second stm controls the timer for which the buzzer goes off after, along with the communication to the first STM. This is done using Syc tic operations. After a certain number of milliseconds (which can be as high as 24 hours or 8.67e7 milliseconds) the timer will begin to send a constant uart signal to the first STM, turning on the alarm. 

The way the alarm is then shut off is using the second STM’s user button. By pressing this user button, an interrupt will occur and change the signal being sent through the UART which shuts off the buzzer. After the same amount of time, the alarm will automatically go off again, insisting on constant wake up times for work or school.

-------------------------------------------------------------------------------------------------------------------------------

FILE AND SETUP INFORMATION:

The drivers for the STM in the UART_Timer_Code file are omitted due to size. 

The two files are for both the two STM’s, one file should be flashed onto one STM and the other on to the second STM. 

The piezo buzzer is linked to PA4 in the Alarm_STM and outputted to ground.

The UART connection goes from PC4 on the UART_Timer_STM to PC5 on the Alarm_STM.

To change the alarm’s interval period, change the number of milliseconds in the stm32f0xx_it.c file on line 132 to the desired number.

