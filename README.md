
This repository contains a C++ code of a Supaplex clone. It has a
brand new 3D art, but its game logic was recovered from an original
Supaplex executable via disassembly.

**Please read the Legal Information at the end of this document.
It contains important notices about the legal state of this code.**

The game is by no means complete. It does not have any menu or UI.
By default game starts in the first level. To choose a level, please
create a `level.cfg` file with a level number in the directory of
the executable file.

Building
========

Please use CMake (http://www.cmake.org/) to build the project.

I have tested build in the following environments:

  * Apple OSX Yosemite with Xcode 6.1.1.
  * Ubuntu Linux 14.10 with GCC 4.9.1.
  * Ubuntu Linux 14.10 with MinGW-w64 4.9.1 (cross-compile to Windows).

If you are using another system or compiler, your mileage may vary. Anyway,
the code is quite portable so it should build and run with very little efforts.
Feel free to contact me via e-mail zapolnov@gmail.com if you have any
problems.

On Ubuntu Linux you may use the following command to install required packages
before building:

    sudo apt-get install \
        build-essential \
        cmake \
        libgl1-mesa-dev \
        libglu1-mesa-dev \
        libx11-dev \
        libxrandr-dev \
        libxcursor-dev \
        libxi-dev \
        libxinerama-dev \
        libxxf86vm-dev

I have also provided Win32 and OSX Yosemite binaries in the `bin` directory for
your convenience.

Engine
======

The code uses 3D engine I was writing at that time. Originally engine code used
the Qt library. Later I have decided to make it more lightweight and have replaced
Qt classes with self-written implementations (you may find them in the
`engine/core/trolltech.h` file).

At the time when I was writing the engine, GLSL shaders were not
available on all platforms. So, I have included the GLSL compiler from
the Mesa library which compiles them to the ARB assembly and then
passes it to the OpenGL library.

Game logic
==========

All the reverse engineered game logic is in the `supaplex_game.cpp` file.
I have tried to keep it as close to the original, as possible.
The code is far from ideal: it has `goto`s, crypting variable names like
`byte_403C3` and all that stuff. But it is of very good quality for a disassembly.

In the `disassembly` directory you will find some scripts and tools I have used
to disassemble the Supaplex executable.

The `disassembly/decomp.c` file contains a decompressor
for the original, compressed executable - it is very interesting as it contains
(an incomplete) 8086 CPU emulator which runs decompressor instruction-by-instruction,
until a final decompressed executable is produced, then dumps it into a file.

The `disassembly/supaplex.idc` is a script for the Interactive Disassembler Pro
(https://www.hex-rays.com/products/ida/index.shtml). It sets human-readable
names for some symbols and adds useful comments to the code.

The `diassembly/supaplex_debugger.asm` was used to debug the Supaplex code. It
runs the original Supaplex executable and reports when various positions in the
code are hit. I don't remember exactly, but I believe that I have used this tool
to clarify purpose of some parts of the code.

The `disassembly/refactor` subdirectory contains an intermediate C++ code
I have built during the disassembly.

During the original disassembly process I have used a subversion repository.
I have preserved an original history using the svn-to-git import. Feel free to
navigate through history to see how the disassembly evolved (use the "follow
renames" option if you don't see the full history).

Legal information
=================

Artwork
-------

3D models and textures were created by the great guy Andrey Chuloshnikov
(http://growlart.daportfolio.com/, 4growl@gmail.com) as a freelance
job. He was paid for this job and copyright is owned by me (I have all the
necessary legal documents).

So all the models and textures are
Copyright (c) 2009-2014 Nikolay Zapolnov (zapolnov@gmail.com).
Feel free to use them under the terms of the
Creative Commons Attribution-NonCommercial-ShareAlike 4.0 License
(http://creativecommons.org/licenses/by-nc-sa/4.0/)

Engine code
-----------

Most of the engine code (everything in the `engine` subdirectory,
except contents of the `3rdparty` directory and some other files)
was written by me and was Copyright (c) 2009 Nikolay Zapolnov.

I hereby release all **my** code in this repository to the public domain.
Feel free to use it for any purpose. Should you need a more legalese
document, please use this one: https://creativecommons.org/publicdomain/zero/1.0

3rdparty libraries, sounds and other stuff that was not created (and are not owned)
by me are still copyrighted by their authors and are distributed under the
corresponding licenses. Please see comments at the top of source files for more
information.

Supaplex game logic
-------------------

**I believe, that you are not allowed to use the provided Supaplex game logic code
and disassembly scripts in any way, be that a commercial, free or
open-source project. Probably you are even not allowed to download and/or read
those files.**

Unfortunately, I didn't find a way to contact the original copyright
holder (the Digital Integration company) for a permission. But Maarten Egmond
did contact them long time ago and have received permission to redistribute
the original Supaplex for free (see http://www.elmerproductions.com/sp/faq.html#q_license
for more information). This fact, and also the fact that there are already a
lot of Supaplex clones available in the wild, makes me believe that I am not
doing nothing bad for nobody.

**If you are a copyright holder for the Supaplex and you believe that I infringe
your rights, please contact me immediately via e-mail zapolnov@gmail.com.**

Sounds
------

Sounds and music were downloaded for free from the Internet (via Google).
I do not own any copyright for them.
