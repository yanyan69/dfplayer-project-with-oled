# MP3 Player Project by yanyan

    This is an Arduino-based MP3 player using DFPlayer Mini, with smart shuffle, button controls, and autoplay. It includes a Python script to automatically rename MP3 files with numeric prefixes (required for DFPlayer) and embed track names into the Arduino code for debug printing.

## Project Structure
    - `MP3/`: Add your .mp3 files here. Run the Python script to rename them (e.g., "0001_Song.mp3").
    - `mp3_w_oled/mp3_w_oled.ino`: The Arduino sketch. Updated automatically by the Python script with track names.
    - `update_script.py`: Python script to rename new MP3s and inject track names into the INO file.
    - `setup.bat`: Windows batch file to run the setup (executes the Python script).
    - `requirements.txt`: Lists dependencies (none external).

## Setup Instructions
1. **Clone the Repo** (if from Git):
    git clone mp3-player-project.git
    cd mp3-player-project


2. **Add MP3 Files**: Place your .mp3 files in the `MP3/` folder.

3. **Run Setup**:
    - On Windows: Double-click `setup.bat` or run it from command prompt.
    - On other OS: Run `python update_script.py` manually.
        This will:
        - Rename new MP3 files with prefixes (e.g., "0001_YourSong.mp3").
        - Update `mp3_w_oled/mp3_w_oled.ino` with the track names array.

4. **Prepare SD Card**:
    - Copy the entire `MP3/` folder to the root of your SD card (formatted as FAT32).
    - Insert the SD card into the DFPlayer Mini.

5. **Upload Arduino Code**:
    - Open `mp3_w_oled/mp3_w_oled.ino` in Arduino IDE.
    - Select your board (e.g., Arduino Nano) and port.
    - Upload the sketch.

6. **Hardware**:
    - Connect DFPlayer: TX to Nano D8, RX to D9 (and a 1k ohms resistor), BUSY to D4.
    - Buttons: Play/Pause to D3, Next/Vol+ to D2 (with pullups).
    - Power and speakers as per DFPlayer docs. gnd2 to audkey_01, gnd1 to arduino gnd and then arduino 5v to dfplayer vcc.

## Usage
    - Short press Play: Toggle play/pause.
    - Long press Play: Volume down.
    - Short press Next: Skip to next track.
    - Long press Next: Volume up.
    - Tracks shuffle smartly and autoplay.

## Notes
    - MP3 files must be renamed with 4-digit prefixes for DFPlayer to recognize order.
    - If adding new MP3s, re-run `update_script.py` (or `setup.bat`) to update names and Arduino code.
    - Git: MP3 files are large; consider adding to `.gitignore` if not committing them.

## Troubleshooting
    - DFPlayer not detected? Check wiring and SD card format.
    - No sound? Adjust volume in code or check speakers.
    - Script errors? Ensure Python 3+ and files in correct paths.

For issues, check serial monitor output in Arduino IDE.