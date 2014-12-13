
This repository contains a C++ code of a supaplex clone. It has a
brand new 3D art, but it's game logic was recovered from an original
Supaplex executable via disassembly.

**Please read the Legal Information at the end of this document.
It contains important notices about the legal state of this code.**

The game is by no means complete. It does not have any menu or UI.
By default game starts in the first level. To choose a level, please
create a `level.cfg` file with a level number in the directory of
the executable file.

Building
========

Please use CMake (http://www.cmake.org/) to build the project. I have
only tested the build in OSX Yosemite. But I believe it should build
under Windows and Linux with minimum efforts.

I have also provided OSX Yosemite binary in the `bin` directory for
your convenience.

Engine
======

The code is using the 3D engine I was writing at the time. Originally
engine code was using the Qt library. Then I have decided to make it more
lightweight and have replaced Qt classes with self-written implementations
(you may find them in the `engine/core/trolltech.h` file).

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
to disassemble the Supaplex executable. Unfortunately, I do not remember the exact
meaning/usage of all this stuff. If you are curious, I have used subversion repository
during the original disassembly process, so I have preserved an original history. Feel
free to navigate through history to see how the disassembly evolved (use the "follow
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

I hereby release all my code in this repository to the public domain.
Feel free to use it for any purpose. Should you need a more legalese
document, please use this one: https://creativecommons.org/publicdomain/zero/1.0

3rdparty libraries, artwork, sounds and other stuff that was not created by me
are still copyrighted by their authors and are distributed under the corresponding
licenses. Please see comments at the top of source files for more information.

Supaplex game logic
-------------------

The Supaplex game logic and disassembly scripts are provided
**only for educational purposes**.

Unfortunately, I didn't find a way to contact the original copyright
holder (the Digital Integration company) for a permission. But Maarten Egmond
did contact them long time ago and have received permission to redistribute
the original Supaplex for free (see
http://www.elmerproductions.com/sp/faq.html#q_license for more information).
This fact, and also the fact that there are already a lot of Supaplex clones
available in the wild, makes me believe that I am not doing nothing bad for nobody.

**If you are a copyright holder for the Supaplex and you believe that I infinge
your rights, please contact me immediately via e-mail zapolnov@gmail.com.**

Sounds
------

Sounds and music were downloaded for free from the Internet (via Google).
I do not own any copyright for them.
