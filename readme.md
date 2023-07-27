# SPH0645 I2S MEMS Microphone to .wav file using ESP32

This code help users to record sound

## Prerequisite

1. ESP32 I2S devices 
2. FFmpeg in %PATH% (Windows system in this case) [Tutorial link by wikihow](https://www.wikihow.com/Install-FFmpeg-on-Windows)
3. SD Card

## Pinout

|SPH0645|ESP32|
|-------|-----|
|WS     |IO15 |
|SD     |IO32 |
|BCK    |IO14 |
|VCC    |3.3V |
|GND    |GND  |

|SD Card|ESP32|
|-------|-----|
|Chip select     |IO26 |

## Usage

![Usage1](../I2S_MIC_recorder/picture/usage_1.png)

1. Continue outputing 's' means in "stand by mode", waiting for start command 'a' input.

2. Enter file name

3. Enter recording time

4. Insert SD Card into PC

5. Run "i2s_to_wav.bat" in folder where .i2s file locates (etc. SD Card)

6. File would be format into wav file with timestamp

## Know Issue

- [ ] 2023/07/27 recording result sounds slightly faster, maybe due to mismatch of sample rate cause by SD card write time. 