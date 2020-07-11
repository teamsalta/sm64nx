# Install Visual Studio
Download Visual Studio Community from https://visualstudio.microsoft.com/downloads/

Execute the installer

Click Continue

Wait for the download and install to complete.

Check Game development with C++ and Desktop development with C++ and Python development

Under Individual Components Check Windows 10 SDK (10.0.17763)

Click Install

*Restart your machine if prompted, on reboot start Visual Studio and got through setup process

Click Sign in

Sign into your Microsoft Account

Click Clone a repository

Paste https://github.com/teamsalta/sm64nx.git into Repository location

Click Clone

Wait for the process to finish, it may finish with an error, this is not unusual

Close Visual Studio

# Install Python
Download Python from https://www.python.org/downloads/

Execute the installer as Administrator

Check the Add Python 3.8 to PATH and Install launcher

Click Customize installation

Make sure all boxes are checked and click Next

Make sure all boxes are checked and click Install

Wait for install

Click Disable path length limit

Click Close

Restart your computer

Open a cmd window as Administrator and paste in:
```
python -m pip install --upgrade pip 
```

and press Enter

Once that finishes paste in

```
pip3 install pillow zstandard tqdm xxhash
```

press Enter

# Configure Assets
Paste your baserom.us.z64 into C:\Users\username\source\repos\sm64nx

In the cmd windows navigate to C:\Users\username\source\repos\sm64nx\import and execute:
```
build.bat
```

If this runs two lines and quits:
Paste into the cmd window: 
```
python extract_assets.py
```

Wait for it to complete

Paste into the cmd window: 
```
python ..\scripts\pak.py --source . --output "..\romfs\!!base.pak"
```

Navigate to C:\Users\username\source\repos\sm64nx\romfs in file explorer

!!base.pak should exist and be ~3588KB

# Compile
Navigate to C:\Users\username\source\repos\sm64nx\build-windows-visual-studio

Double click sm64.sln

It should open in Visual Studio

In the solutions configuration drop down change from Debug to Release

In the Build menu click Build Solution

Wait for it to complete

Navigate to C:\Users\username\source\repos\sm64nx\

Copy the romfs folder, sdl2.dll and baserom.us.z64 from C:\Users\username\source\repos\sm64nx to C:\Users\username\source\repos\sm64nx\build-windows-visual-studio\x64\Release

Launch sm64.exe from C:\Users\username\source\repos\sm64nx\build-windows-visualstudio\x64\Release
