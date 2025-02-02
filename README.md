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
| git diff     | git diff       | git diff      |
| git status   | git status     | git status    |
| git diff     | git diff       | git diff      |
