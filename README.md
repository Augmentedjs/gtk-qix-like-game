# gtk-qix-demo - QIX Style Line Drawing Game

## Introduction

This project is a QIX-style line drawing game implemented using C and GTK 4 on Debian Linux. The game features a moving line that bounces around within the window boundaries, leaving trails as it moves. The player, represented as a movable dot cloud, can move around the edges of the window and can move off the border when the control key is held.

More to follow as game is completed.

## Dependencies

### Linux

- **Operating System:** Debian-based Linux distribution (e.g., Ubuntu, Debian)
- **Compiler:** GCC (GNU Compiler Collection)
- **Libraries:**
  - GTK 4
  - Cairo
  - GLib

To install the required dependencies on a Debian-based system, you can use the following command:

```sh
sudo apt update
sudo apt install build-essential autoconf automake libgtk-4-dev libcairo2-dev libglib2.0-dev
```

## Instructions

1. Clone the Repository

``` sh
git clone git@github.com:Augmentedjs/gtk-qix-demo.git
cd qix-line-drawing-game
```

2 Generate the Build System Files

Run the autogen.sh script to generate the configure script and other necessary build files:

``` sh
./autogen.sh
```

3 Configure the Project

Run the configure script to set up the project for your system:

```sh
./configure
```

4 Build the Project

Use make to compile the project:

``` sh
make
```

5 Clean Up

You can clean up the build artifacts using the following commands:

make clean: Removes the compiled object files and the executable.
make distclean: Removes all files generated by the build system, including Makefile, config.status, config.log, configure, aclocal.m4, compile, depcomp, install-sh, Makefile.in, missing, and the autom4te.cache and .deps directories.

6 Run the Game

After building the project, you can run the game using the following command:

``` sh
./qix_line
```

## Makefile Targets

- make: Compiles the project.
- make clean: Removes the compiled object files and the executable.
- make distclean: Removes all files generated by the build system, including Makefile, config.status, config.log, configure, aclocal.m4, compile, depcomp, install-sh, Makefile.in, missing, and the autom4te.cache and .deps directories.

## Game Controls

Arrow Keys: Move the player around the edges of the window.
Control Key: Allow the player to move off the border in one direction.

## TODO

- Add game rules and objectives.
- Implement additional game features.
- Improve graphics and animations.

## License

This project is licensed under the Apache 2.0 License. See the LICENSE file for details.
