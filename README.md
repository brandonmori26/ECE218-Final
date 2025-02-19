ECE 218 Project 3

Brandon Mori and Leo Weisberger

PROJECT DESCRIPTION:
This project involves the design and implementation of a windshield wiper control system as part of a driver-ed smart car system. The system includes a driver seat occupancy sensor, ignition push button, wiper mode selector, intermittent delay time selector, windshield wiper motor, and a user LCD display. The ignition subsystem ensures safe ignition by monitoring seat occupancy and seatbelt status, providing appropriate feedback on the display. When the engine is running, the wiper subsystem allows the user to select HI, LO, INT, or OFF modes, with intermittent mode delays set to SHORT, MEDIUM, or LONG. The system ensures the wipers return to the starting position before stopping and displays the selected mode on the LCD. This design prioritizes safety and convenience, providing feedback for each system state through visual and auditory indicators.

STARTING REPOSITORY:


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
|In the AUTO position: When the light level is above a “daylight” level, switch headlights off after a delay of 2 seconds.When the light level is below a “dusk” level, switch headlights on after a delay of 1 second.| Pass|This system functioned as intended. The time delay was difficult to capture accurately in a demo video, but there was an obvious time delay present for both “daylight” and “dusk” levels.|
| When the engine is running, if a passenger unbuckles the belt and leaves the vehicle, the headlight system will remain on.  | Pass   | As long as engineLed is on, the headlight system will function as intended.|
