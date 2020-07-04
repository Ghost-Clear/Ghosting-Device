# Ghosting-Device

[![Codacy Badge](https://app.codacy.com/project/badge/Grade/4d49067b5e7546a598f560febd2e2aeb)](https://www.codacy.com/manual/varunchitturi/Ghosting-Device?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=varunchitturi/Ghosting-Device&amp;utm_campaign=Badge_Grade)
[![CodeFactor](https://www.codefactor.io/repository/github/varunchitturi/ghosting-device/badge)](https://www.codefactor.io/repository/github/varunchitturi/ghosting-device)
[![Actions Status](https://github.com/varunchitturi/Ghosting-Device/workflows/Build/badge.svg)](https://github.com/varunchitturi/Ghosting-Device/actions)

## The official source code for the ghosting devices that connect to the Ghost Connect App

These ghosting devices connect to the ghost connect app and relay information between it using BLE technology.
They have sensors attached that will transmit a detection signal to the app when the sensors detect something.
They also have an led that indicates which corner to ghost to.

## Ghosting App

For the repository for the ghosting app that this device connects to, refer to https://github.com/varunchitturi/Ghosting-Connector

## Notes

1. Peripheral name references
  1. Front Right is referred to as FR in the code
  2. Front Left is referred to as FL in the code
  3. Center Right is referred to as CR in the code
  4. Center Left is referred to as CL in the code
  5. Back Right is referred to as LR in the code
  6. Back Left is referred to as LL in the code
2. The Internal_FS folder and the ultrasonic_test folder are just used for testing the features of the device and are not loaded onto the device.
3. For all purposes of contributing, the devices are Adafruit NRF52 Bluefruit using the Arduino board scheme.
4. To transmit a detected signal, write "detected" using the tx characteristics.
5. All input from the app is received using the rx characteristic.
  1. If "1" is received, turn the led on.
  2. If "0" is received, turn the led off.
6. The project folder was made using PlatformIO in VScode

## Libraries

1. Arduino
2. Bluefruit
3. NewPing
4. Adafruit LittleFS
5. Internal File System

## File Descriptions
 
 All files are included in the src folder of each device folder.
 Files from the 6 different folders are pretty much the same except for the naming command for the devices.
 The files had to be placed in different folders to be able to compile correctly and be loaded onto each device.

