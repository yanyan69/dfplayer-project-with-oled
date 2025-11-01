@echo off
echo Setting up MP3 Player Project...

REM Check if Python is installed
python --version >nul 2>&1
if %errorlevel% neq 0 (
    echo Python is not installed or not in PATH. Please install Python 3.6+ from https://www.python.org/
    pause
    exit /b 1
)

REM No requirements to install, as all are standard libs.

REM Run the update script to rename MP3s and update Arduino code
echo Running update_script.py...
python update_script.py

echo Setup complete! Copy the MP3/ folder to your SD card, then upload D:\mp3_w_oled\mp3_w_oled.ino to your Arduino.
pause