# ECE 218 Project 3

Brandon Mori and Leo Weisberger

## PROJECT DESCRIPTION:
This project involves the design and implementation of a windshield wiper control system as part of a driver-ed smart car system. The system includes a driver seat occupancy sensor, ignition push button, wiper mode selector, intermittent delay time selector, windshield wiper motor, and a user LCD display. The ignition subsystem ensures safe ignition by monitoring seat occupancy and seatbelt status, while also providing appropriate feedback on the serial display; whether that be a specific error message or a successful ignition. When the engine is running, the wiper subsystem allows the user to select HI, LO, INT, or OFF modes, with intermittent mode delays set to 3 seconds (short), 6 seconds (medium), or 8 seconds (long). The system ensures the wipers return to the starting position before stopping and displays the selected mode on the LCD. This design prioritizes safety and convenience, providing feedback for each system state through visual and auditory indicators.

## DESIGN ALTERNATIVES:
While most of the hardware was predetermined, there was flexibility in selecting the type of motor to simulate the windshield wiper system. The choice is between a continuous servo motor and a positional servo motor. After consideration, we chose the positional servo due to its precise control over angular positioning, which was critical for ensuring the wipers return to their starting position after each cycle. This accuracy helps simulate realistic wiper behavior, making the positional servo the ideal option for us.

## IGNITION SUBSYSTEM:
| Specifications | Test Result | Comments |
| :---         |     :---:      |          ---: |
|Enable engine start while both seats are occupied and seatbelts fastened. Welcome Message prints. Otherwise print appropriate error message.| Pass | All error messages are displayed on the serial monitor depending on which error occurred. If error, Alarm buzzer triggers |
|Start the engine (light the blue LED, turn off Green) when ignition is enabled (green LED) and ignition button is pressed  (before the button is released).|Pass| Serial Monitor will display: Engine Started  |
| Keep the engine running even if the driver/passenger should unfasten belt and exit the vehicle.|  Pass | |
| When the engine is running, stop the engine once the ignition button has been pushed and then released. | Pass | Serial Monitor will display: Engine Stopped |

## Windshield Wiper Subsystem

| Specifications | Test Result | Comments |
| :--- | :---: | ---: |
| Wipers remain OFF when the engine is off. | Pass | Wipers do not activate unless the engine is running. |
| Wipers start in LO mode when selected and continuously run at a low speed. | Pass | Wipers cycle continuously at a low speed. |
| Wipers start in HI mode when selected and continuously run at a high speed. | Pass | Wipers cycle continuously at a high speed. |
| Wipers start in INT mode when selected, cycling at the set intermittent delay (3s, 6s, or 8s). | Pass | Wipers pause between cycles according to the selected delay. |
| Wipers return to the start position before stopping when turned OFF. | Pass | Wipers always complete a full cycle before stopping at their initial position. |
| Wiper mode selection updates on the LCD display. | Pass | LCD correctly updates to show the selected mode (HI, LO, INT, or OFF). |
| Wipers stop when the engine is turned off, regardless of mode. | Pass | Wipers immediately stop when the engine turns off. |

## MODULAR  DIAGRAM


![Flowchart (1)](https://github.com/user-attachments/assets/4b12fd3d-975b-4952-8f51-714bfe83732e)


