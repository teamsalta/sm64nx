# sm64_mod_tools
Tools for manipulating SM64 mod formats, for externally loading assets (models, textures, behavior scripts, etc).

# installation
It is recommended to git clone out this repository, and then add the directory to your environment PATH variable and PYTHONPATH variable.

# pak.py
a compressed package format for storing and transferring mods.  will package all *.geo, *.bhv, *.gfx, *.png, *.lvl, and *.mcr files it finds.

## Example
pak.py --source /home/chad/owo/ --output owo.pak

## Usage
Build sm64 mod packs

optional arguments:
  -h, --help            show this help message and exit
  --output OUTPUT, -o OUTPUT
                        output file name or directory
  --source SOURCE, -s SOURCE
                        source directory containing files to compare
  --extract EXTRACT, -e EXTRACT
                        file to unpack, use --output to specify directory
  --scale SCALE         texture resolution divisor. 2 means half quality, 1 is
                        original quality
  --usepaths            export textures using filename paths


# sm64.py
builds python scripts which, when ran, build external files for use with packaging into pak files.  When the python script is ran, it  will generate quite a few excess files, typically you would not want to distribute the gfx files, and the geo files contain all gfx and textures referenced inside of them.

## Example

sm64.py actors/myactor/model.inc.c actors/myactor/geo.inc.c > myactor.py

./myactor.py

# encode_images.py
converts all png's in the cwd to c files used by sm64 mods
