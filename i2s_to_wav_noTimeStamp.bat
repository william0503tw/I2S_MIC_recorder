rem commad: ffmpeg -f s32le -ar 44100 -ac 1 -i track.i2s track.wav

@echo on

for %%i in (*.i2s) do (
    setlocal enabledelayedexpansion
    for /f "tokens=2 delims==" %%a in ('wmic OS Get localdatetime /value') do set "dt=%%a"
    set "datetime=!dt:~0,8!_!dt:~8,6!"
    ffmpeg -f s32le -ar 44100 -ac 1 -i %%i  %%~ni.wav
)