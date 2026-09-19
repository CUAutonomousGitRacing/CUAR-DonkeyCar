# Software Team - CU Autonomous Racing

### Overview

This is the documentation for the Software Team of the CU Autonomous Racing club at the University of Colorado Boulder.

The goal of this program is to allow a full size go kart to drive autonomously using a driving-focused neural network known as DonkeyCar. The AI is managed by our Jetson Nano, receiving data from two seperate Teensy 4.1 and a camera.

## Current Files
### Sensors

This folder contains the config and run files for our Teensy controlling all our sensors. The goal is to detect any voltage or temperature abnormalities to ensure the cart is safe. All config variables are currently contained within the teensy_functions.h file to minimize number of files needed. All function declarations are contained in teensy_functions, which are the heartbeat with the other teensy, temperature check, and packet parsing and verifying.

### Training Data

All training data images and catalogs are contained in this folder. Store all further data for training the AI within this folder.

### mycar

The DonkeyCar AI program itself is contained here. All config variables are set up for our system, with the goal of removing and altering files to minimize number of unused/unneeded files. 

TestingUART.py: Contains scripts to ensure UART communication is functional

arduino_drive.py: Contains function to allow driving 

calibrate.py: Contains script to calibrate controls before driving/testing

manage.py: The heart of the AI. Containts scripts for driving, training, weight declaration, and overall AI implementation. **Do not edit without explicit permission from the Software Team, as it may brick the AI**

config.py: Driving config file for data collection and training

myconfig.py: Driving config file for manual driving

train.py: Contains script to enable training

weather_part.py: Enables weather config depending on what weather needs to be trained in
