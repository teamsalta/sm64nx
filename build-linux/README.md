# Install build dependencies
The build system has the following package requirements:

python3 >= 3.6

pip3 install tqdm zstandard xxhash

libsdl2-dev

libglew-dev

# Compiling
## Debian / Ubuntu - targeting 64 bits

sudo apt install build-essential git python3 libglew-dev libsdl2-dev zstd

## Fedora - targeting 64 bits
sudo dnf install make gcc python3 glew-devel SDL2-devel zstd


## If you want to build it yourself:
sudo pacman -S base-devel python sdl2 glew zstd

### Void Linux - targeting 64 bits
sudo xbps-install -S base-devel python3 SDL2-devel glew-devel zstd

# Build the executable.
Run make to build.  ensure than romfs/!!base.pak is available to the executable before you attempt to execute it.
