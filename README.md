ECE 218 Project 2

Brandon Mori and Aidan Munns

PROJECT DESCRIPTION:
This project implements an enhanced driver safety system design for Driver's ED instructors and their students. The system manages two primary subsytems: vehicle ignition safety controls and automated headlight management. The ignition system ensures safe vehicle operation by monitoring driver/passenger presence and seatbelt status, while the headlight system provides automatic light control based on ambient light conditions. The system includes safety features such as requiring both driver and passenger to be seated with fastened seatbelts before allowing engine start, and implements a sophisticated headlight control system with manual and automatic modes featuring light-level dependent activation delays.

STARTING REPOSITORY:
https://github.com/MollyFry/218-Project-1/blob/main/Our%20Raw%20Code

IGNITION SUBSYSTEM:
| Specifications | Test Result | Comments |
| :---         |     :---:      |          ---: |
|Enable engine start while both seats are occupied and seatbelts fastened. Otherwise print appropriate error message.| Pass | All error messages are displayed on the serial monitor depending on which error occurred. |
|Start the engine (light the blue LED, turn off Green) when ignition is enabled (green LED) and ignition button is pressed  (before the button is released).|Pass| Serial Monitor will display: Engine Started  |
| Keep the engine running even if the driver/passenger should unfasten belt and exit the vehicle.|  Pass | |
| When the engine is running, stop the engine once the ignition button has been pushed and then released. | Pass | Serial Monitor will display: Engine Stopped |

HEADLIGHT SUBSYSTEM:
| Left-aligned | Center-aligned | Right-aligned |
| :---         |     :---:      |          ---: |
| Headlights are only enabled when engine is running (engineLed is ON). | Pass |  |
| If the engine is running, and the user selects: ON, turn on both low beam lamps; if they select OFF, turn off all lamps. |Pass |OFF position is 0 on the potentiometer, ON position is anything past 0.66 on potentiometer. 
|In the AUTO position: When the light level is above a “daylight” level, switch headlights off after a delay of 2 seconds.When the light level is below a “dusk” level, switch headlights on after a delay of 1 second.|
| Pass| This system functioned as intended. The time delay was difficult to capture accurately in a demo video, but there was an obvious time delay present for both “daylight” and “dusk” levels. |
| When the engine is running, if a passenger unbuckles the belt and leaves the vehicle, the headlight system will remain on.  | Pass   | As long as engineLed is on, the headlight system will function as intended.|
