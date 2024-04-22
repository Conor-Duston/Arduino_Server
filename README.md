# Arduino MEGA File Server

## Description
This project aims to create a working file sharing server on an Arduino MEGA 2560 board with an ethernet shield.
The Arduino MEGA was selected because it is a deciently powerfull board, but not so powerfull that it had an OS due to resource constraints.
The project could be used in conjunction with wifi sensors to have data read from multiple sources to one low power board.

## General Information

### Materials
This project was made for the Arudiuno Mega 3 with the Ethernet Shield R2.
The SD card used is an ExFat mini sd card, inserted into the Ethernet Shield's SD slot.
The server code will not work for **any** other device combinations, **THE BOARD, SHIELD, AND SD CARD** need to be the same. 

### Networking
The hardcoded MAC on [line 28 of Arduino_file_server.ino](/Arduino_file_server/Arduino_file_server.ino)  ***needs*** to be changed based on your sheilds IP address. Check [Arudiuno shield documentation](https://docs.arduino.cc/hardware/ethernet-shield-rev2/) for more info.
The IP address is done based on the internet router's range of IP adresses on the same network. This can vary between routers and IP types.
***CAUTION*** it is **NOT** recommended to use this project over publicly accesible networks. There are **NO** security features currently added to the project. 

### Setting up and Running
This project requires either Visual Studio Code or Arduino IDE to run.
If you use Visual Studio Code, make sure to have the Arduino extention installed as well as the CPP extention.

- Before compiling the project, install the following Arduino libraries:
    - Ethernet
    - SdFat
- Attach your Arduino Ethernet Hat to the Arduino Mega board
- Aquire an SD card with more than 32 GB of space and transfer index.html file to that card
- Place the SD card into the Ethernet hat
- Compile and run the project

You should be able to access the SD card by typing the IP address into the search bar of your computer
