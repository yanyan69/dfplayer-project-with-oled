import os
import sys
import re

# Allow proper UTF-8 printing in Windows console
sys.stdout.reconfigure(encoding='utf-8')

# Configuration
mp3_folder = "MP3"
arduino_folder = "mp3_w_oled" 
arduino_file = os.path.join(arduino_folder, "mp3_w_oled.ino")  # Full path example: 'player/player.ino'
marker_start = "// BEGIN_TRACK_NAMES"
marker_end = "// END_TRACK_NAMES"

# Step 1: Rename MP3 files
# Match filenames that already start with 4 digits and underscore
pattern = re.compile(r"^\d{4}_")

files = [f for f in os.listdir(mp3_folder) if f.lower().endswith(".mp3")]

# Filter out files that already have numeric prefixes
new_files = [f for f in files if not pattern.match(f)]
new_files.sort()

# Rename only new files, starting from the next available number
existing_files = [f for f in files if pattern.match(f)]
next_index = len(existing_files) + 1 if existing_files else 1

for i, filename in enumerate(new_files, start=next_index):
    new_name = f"{i:04d}_{filename}"
    old_path = os.path.join(mp3_folder, filename)
    new_path = os.path.join(mp3_folder, new_name)
    os.rename(old_path, new_path)
    print(f"Renamed: {filename} -> {new_name}")

print("Done renaming new MP3 files!")

# Step 2: Generate track names array (using original names without prefixes)
all_files = [f for f in os.listdir(mp3_folder) if f.lower().endswith(".mp3")]
all_files.sort()  # Sort alphabetically, which will order by prefix numerically

track_names = []
for f in all_files:
    # Strip the "0001_" prefix to get the original name
    if pattern.match(f):
        original_name = f[5:]  # Skip first 5 chars: 4 digits + '_'
    else:
        original_name = f  # Fallback, though should all have prefixes now
    track_names.append(original_name)

# Generate the Arduino array code
array_code = "const char* trackNames[] = {\n"
for name in track_names:
    array_code += f'    "{name}",\n'
array_code = array_code.rstrip(',\n') + "\n};"  # Remove trailing comma

# Step 3: Update the Arduino code
if os.path.exists(arduino_file):
    with open(arduino_file, 'r', encoding='utf-8') as f:
        content = f.read()

    # Find the markers and replace the content between them
    pattern = re.compile(f"{re.escape(marker_start)}.*?{re.escape(marker_end)}", re.DOTALL)
    replacement = f"{marker_start}\n{array_code}\n{marker_end}"
    new_content = pattern.sub(replacement, content)

    if new_content == content:
        print("Warning: Markers not found in Arduino file. Please add them around where the array should go.")
    else:
        with open(arduino_file, 'w', encoding='utf-8') as f:
            f.write(new_content)
        print(f"Updated Arduino code in '{arduino_file}' with new track names array!")
else:
    print(f"Error: Arduino file '{arduino_file}' not found. Please check the path or adjust 'arduino_folder' in this script.")
