# QIX Style Line Drawing Game

## Introduction

This project is a QIX-style line drawing game implemented using C and GTK 4 on Debian Linux. The game features a moving line that bounces around within the window boundaries, leaving trails as it moves. The player, represented as a movable dot cloud, can move around the edges of the window and can move off the border when the control key is held.

More to follow as game is completed.

## Dependencies

### Linux

These are the requirements for Linux but also tested configurations

- **Operating System:**
  - Debian-based Linux distribution (developed on Debian Sid Trixie)
- **Compiler:**
  - GCC 13 (GNU Compiler Collection)
- **Libraries:**
  - GTK 4
  - Cairo
  - GLib 2

To install the required dependencies on a Debian-based system, you can use the following command:

```sh
sudo apt update
sudo apt install build-essential autoconf automake libgtk-4-dev libcairo2-dev libglib2.0-dev
```

## Game Instructions

### Game Controls

- **Arrow Keys:** Move the player around the edges of the window.
- **Shift Key:** Allow the player to move off the border in one direction.

### Rules

TBD

## Install and Build Instructions

### Clone the Repository

``` sh
git clone git@github.com:Augmentedjs/gtk-qix-like-game.git
cd gtk-qix-like-game
```

### Generate the Build System Files

Run the autogen.sh script to generate the configure script and other necessary build files:

``` sh
./autogen.sh
```

### Configure the Project

Run the configure script to set up the project for your system:

```sh
./configure
```

### Build the Project

Use make to compile the project:

``` sh
make
```

### Clean Up

You can clean up the build artifacts using the following commands:

```sh
make clean
```

Removes the compiled object files and the executable.

```sh
make distclean
```

Removes all files generated by the build system, including Makefile, config.status, config.log, configure, aclocal.m4, compile, depcomp, install-sh, Makefile.in, missing, and the autom4te.cache and .deps directories.

### Run the Game

After building the project, you can run the game using the following command:

``` sh
./qix_line
```

## TODO

Stuff to work on with QIX

* Shapes only fill away from QIX location
  - Defect on some cases filling over the monster
* ~~Shapes should use area fill to close~~
* Shape fills happen when touching a wall
* Shapes should calculate area
* Player should be able to move on borders of drawn shapes
* Player doesn't look correct, should be a diamond shape
  - Maybe
* Player can change speed for drawing
* ~~QIX Monster doesn't move correctly, needs face/movement algorithm~~
* ~~QIX Monster trails are too close and mirror direction vs a 'tail'~~
* ~~QIX Monster doesn't bounce off walls~~
* QIX Monster doesn't kill player if touched
* Need a title area on field
  - lives
  - area
  - time
  - level
  - score
* Size of playfield should be a param (default to 640x480)
  - Maybe not, but perhaps a zoom?  Or aspect change?
* Player animations
  - start
  - explode
  - complete
* Sparks on higher levels (post level 3)
* Multiple QIX Monsters (level 3 on)
* Sounds
  - Moving
  - QIX
  - Fill
  - Explode
  - Moving
  - Drawing
* Implement rules of game - time limit
* Implement rules of game - lives
* Implement rules of game - area score
* Implement rules of game - win from 75% or higher area captured
* Implement rules of game - Levels

## License

This project is licensed under the Apache 2.0 License. See the LICENSE file for details.
