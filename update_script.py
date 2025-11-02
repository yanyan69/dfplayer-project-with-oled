# update_script.py
# Renames MP3 files in each folder (01, 02, ...) to 001_songname.mp3
# For DFPlayer Mini compatibility

import os

BASE_DIR = "MP3"  # main directory containing folders 01, 02, ...
PREFIX_PATTERN = "{:03d}_"

if not os.path.exists(BASE_DIR):
    print(f"ERROR: '{BASE_DIR}' folder not found.")
    exit(1)

total_renamed = 0

for folder in sorted(os.listdir(BASE_DIR)):
    folder_path = os.path.join(BASE_DIR, folder)
    if not os.path.isdir(folder_path) or not folder.isdigit():
        continue  # skip non-numeric folders

    print(f"\n--- Processing Folder: {folder} ---")
    files = [f for f in os.listdir(folder_path) if f.lower().endswith(".mp3")]
    files.sort()

    renamed = 0
    for i, old_name in enumerate(files, 1):
        # Skip if already in the format 001_...
        if len(old_name) > 4 and old_name[:3].isdigit() and old_name[3] == '_':
            continue

        prefix = PREFIX_PATTERN.format(i)
        new_name = prefix + old_name
        old_path = os.path.join(folder_path, old_name)
        new_path = os.path.join(folder_path, new_name)

        os.rename(old_path, new_path)
        print(f"Renamed: {old_name} -> {new_name}")
        renamed += 1

    if renamed == 0:
        print("No changes needed.")
    else:
        print(f"Renamed {renamed} file(s).")
        total_renamed += renamed

print(f"\nDone. Total renamed files: {total_renamed}")