import os

mydir = "Core/Src/App/Commands"

file_list = []

for entry in os.scandir(mydir):
    if not entry.is_file() or entry.name == "script.py": continue

    path = f"${{CMAKE_SOURCE_DIR}}/{mydir}/{entry.name}"
    file_list.append(path)

for item in file_list:
    print(item)
    print()
