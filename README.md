# Super Mario 64

This repo contains a full decompilation of Super Mario 64 (U).
The source and data have been decompiled but complete naming and documentation
all of the code and data is still a work in progress. Decompiling the (E) ROM
is also an ongoing effort.

It builds the following ROMs:

This repo does not include all assets necessary to run the game.
**Any binaries posted here do not include required assets from the base ROM
A prior copy of the game is required to extract the required assets.**

# Purpose

The purpose of this project is to extend the original SM64 decompilation to focus on non-N64 platforms, to not be contrained by the limitations of N64 hardware.  One of the main ways this is accomplished is by loading game assets externally at runtime, rather than compile them in.

# PAK Files

Pak is a mod package format created for this project, to create easily managed mod packages.

## Loading Pak Files
All pak files placed in the romfs/ directory will be loaded at application startup.  Pak files placed in sub-directories of romfs/ can be enabled or disabled at runtime.  Directories which start with the tilde character are disabled by default, otherwise the mod directory is enabled by default.  Multiple pak files can be placed in a single directory and toggled on/off together.

## Disabling Pak Files
Once in game, access the start menu, and press R on the gamepad until you reach the mod section.  There will be one entry here for each directory under romfs/.  You can enable or disable them by highlighting the mod and then pressing A.  Mod changes require an application restart to take effect.

## Asset conflict resolution
In the event an asset exists in more than one pak file, the pak files are sorted in alphabetical order, and the highest value pak will take precedence.  THis means zzz.pak will override aaa.pak in the event of an asset conflict.

# Supported Platforms
Only 64-bit little endian platforms which have SDL2, opengl, and a C++ compiler are supported.  This includes, but is not limited to, X64 Windows, X64 Linux, and Nintendo Switch.  It should work with a little work with: X64 OSX, iOS devices, Android devices, Raspberry Pi, nearly all 64bit little endian arm devices.

Big endian support is on the roadmap.

# 1. Copy baserom(s) for asset extraction

Place an existing big endian Z64 US ROM in this source root directory for asset extraction, and then run import/build (posix) or import/build.bat to extract assets and then place them into a pak file at romfs/!!base.pak

**The app will crash if it is attempted to be launched without romfs/!!base.pak being present.**

# Python 
Ensure you have Python 3.6+ installed, and the following python packages:

pip3 install pillow zstandard tqdm xxhash


## Contributing

Pull requests are welcome. For major changes, please open an issue first to
discuss what you would like to change.

Run clang-format on your code to ensure it meets the project's coding standards.

Official discord: https://discord.gg/27JtCWs
