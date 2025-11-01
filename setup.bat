@echo off
echo.
echo    MP3 Player Setup - Yanyan
echo.

REM Check Python
python --version >nul 2>&1
if %errorlevel% neq 0 (
    echo [ERROR] Python not found! Install from https://python.org
    pause
    exit /b 1
)

echo [INFO] Renaming MP3 files with 0001_, 0002_...
python update_script.py

echo.
echo [SUCCESS] Setup complete!
echo.
echo Next steps:
echo   1. Copy the entire MP3/ folder to your SD card (FAT32)
echo   2. Open mp3_player.ino in Arduino IDE
echo   3. Upload to your Arduino
echo.
pause