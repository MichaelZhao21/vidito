# Vidito

WIP. Simple video editor, built using [SDL](https://wiki.libsdl.org/SDL3/FrontPage), [Clay](https://github.com/nicbarker/clay), and [ffmpeg](https://ffmpeg.org/).

## Setup

Install the following packages:

- [SDL3](https://github.com/libsdl-org/SDL)
- [SDL3_ttf](https://github.com/libsdl-org/SDL_ttf)

To configure using cmake, run `cmake -S . -B build`. This will download the required Clay library files (see `lib` after running the command) as well as generating build files (see `build` after running the command).

## Build and Run

Run the following:

```bash
cd build
make
```

Then you can run the `Vidito` executable.

> Note: Currently I have the `Roboto-Regular.ttf` file copied manually to the `build` folder. This should be changed in the future
