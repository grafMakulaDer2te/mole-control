# Mole Control for the Gamebuino Classic
## Description
This is a simple Mole Whacking game for the gamebuino classic.
## Setup
Copy the build/arduino.avr.uno/mole-control.ino.hex onto your SD-card (not tested jet because my DIY gambuino has no SD-card).
Alternatively open the mole-control.ino in your Arduino IDE and upload it directly.
## Game play
Select a mole/hole with the D-Pad then press A. If you hit a mole it will disappear. If there was no mole Your lives will degreased by 1.

If you wait too long (2.5 seconds at the beginning) the mole will also disappear and Your lives will also be degreased by 1. This time shortens every level by 0.2 seconds. The appearing rate also increases with every level.

You have 10 lives at the beginning. If it reaches 0 the game is over.
Press Button C to restart the game.
