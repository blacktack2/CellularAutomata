# CellularAutomata

## About

This project is a c++ and OpenGL based front-end for playing around with
various types of Cellular Automata (see
[the wiki](https://en.wikipedia.org/wiki/Cellular_automaton) for more
information). I am writing this in my own time with no formal specification.
A specific set of goals for this project can be seen in the TODO section,
which will be changed and updated as I come up with new ideas and complete
old ones.

**Epilepsy Warning! May cause seizures!**

Some configurations of 2D Lifelike Cellular Automata can result in flashing images.
This can be avoided by slowing down the simulation using the `Iteration Delay`
parameter. A safe setting would be 300ms (about 3fps) though you can adjust this
according to your own judgement (the default configuration of `Conway's Game
of Life` should be fairly safe to experiment with).

## Installation

### Dependencies

This project makes use of the following external libraries:

- [SDL2](https://www.libsdl.org/)
- [Dear ImGui](https://github.com/ocornut/imgui)
- [OpenGL](https://www.khronos.org/) | [useful tutorial](https://learnopengl.com/)
- [GLM](http://glm.g-truc.net/)

All credit for the afformentioned libraries goes to their respective author/authors.

The source code for ImGui, OpenGL, and GLM is included within the project
repository, and so they do not need further steps to install.

#### SDL2

##### Linux

Follow the instructions at
(https://lazyfoo.net/tutorials/SDL/01_hello_SDL/linux/index.php)
to download SDL2. If you download using `apt-get` SDL2 should be placed in the
expected directory by default. If you download manually you will have to ensure
SDL2 is located somewhere CMake can find it, otherwise you may want to edit the
`CMakeLists.txt` file to look in the correct directory.

##### Windows

Follow the instructions at
(https://lazyfoo.net/tutorials/SDL/01_hello_SDL/linux/index.php)
to download SDL2. CMake expects to find SDL2 in the `c:/programs/sdl/`
directory.

### Building

#### Linux

Change to the project root and type the following:

```bash
cmake --build .
```

This should create a new executable file called `CellularAutomata`. Running this
should start the application.

**Note**: You will need cmake version 3.17 or greater to build the application this
way.

#### Windows

This project can be built using cmake with VisualStudio (I have only tried using
version 2022 and do not know if other versions will work). It should be possible
to build using CLion, though as of yet I have not been able to get this working
due to issues with linking SDL2.

## Usage

Currently I have only implemented 2D Lifelike Cellular Automata, and the GUI layout
is still a work in progress, so this section of the readme will be limited for now.

## Definitions

### B/S Rule Notation

Throughout this readme I will be using the B/S rule notation to describe the rules
of a 2D Lifelike Cellular Atomata. The notation is formatted as `B<0-8>/S<0-8>` where
`<0-8>` is replaced by a sequence of numbers. The numbers following the `B` (Birth)
dictate how many neighbours a dead cell must have to become alive. The numbers
following the `S` (Survive) dictate how many neighbours a live cell must have to
stay alive.

An example of this notation would be with `Conway's Game of Life` which is notated as
`B3S32`. This means that a dead cell will become alive if it has exactly 3 neighbours,
and a live cell will stay alive if it has 2 or 3 neighbours.

### Cellular Automata

Cellular Automata describes a set of simulations in which you apply simple rules to
a grid of cells with a finite set of states. These rules will change the state of
each cell based on the state of other cells (usually of the neighbouring cells).

See lifelike Cellular Automata for more information

### Lifelike Cellular Automata

Due to the sheer number of rules a Cellular Automata simulation can have in 2
dimensions and above, the classification of 'Lifelike' has been created to simplify
the process to a more reasonable subset of rules.

A Lifelike Cellular Automata only considers the number of neighbours within the moore
neighbourhood (the eight immediate neighbours in a square grid, including diagonals)
and the state of the current cell. The well known Cellular Automata `Conway's Game of
Life` fits the definition of Lifelike.

## License

MIT License

Copyright (c) 2022 Stuart Manfred Lewis

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
