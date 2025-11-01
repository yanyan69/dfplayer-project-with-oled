# update_script.py
# Only renames MP3 files with 0001_, 0002_... for DFPlayer
import os

MP3_DIR = "MP3"
PREFIX = "{:04d}_"

if not os.path.exists(MP3_DIR):
    print(f"ERROR: '{MP3_DIR}' folder not found!")
    exit(1)

files = [f for f in os.listdir(MP3_DIR) if f.lower().endswith(".mp3")]
files.sort()

renamed = 0
for i, old_name in enumerate(files, 1):
    if old_name[:5].isdigit() and old_name[4] == '_':
        continue  # Already prefixed
    new_name = PREFIX.format(i) + old_name
    old_path = os.path.join(MP3_DIR, old_name)
    new_path = os.path.join(MP3_DIR, new_name)
    os.rename(old_path, new_path)
    print(f"Renamed: {old_name} → {new_name}")
    renamed += 1

if renamed == 0:
    print("All files already renamed.")
else:
    print(f"Renamed {renamed} file(s). Ready for SD card!")