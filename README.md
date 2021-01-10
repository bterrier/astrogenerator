# astroGenerator
This project is an attempt to bring astroGenerator back to life.

astroGenerator is a program written by Vincent Gallouedec to organize sky-watching night.

The original version was available from:
- http://www.univers-astronomie.fr/
- http://www.univers-astronomie.fr/generateur-soiree/astrogenerator

However, the website is often offline and the latest release is version 2.0 published in 2013.

At this time the only way to get the program and its sources is to download the setup from the Internet Archive Wayback Machine:
- https://web.archive.org/web/20160910193551/http://www.univers-astronomie.fr/generateur-soiree/astrogenerator/telecharger.php?name=astroGenerator-2.0-setup.exe&version=2.0

## Goals, planning and versions
### Version 1
This repo starts at version 1.1.1 which is the version I was able to retrieve the sources from astroGenerator-2.0-setup.exe
The code is taken as-is without any improvements, fixes or changes.
The only files that are added are:
- AstroGenerator.pro
- REAMDE.md
- LICENSE

Version 1.2 sources are lost for now.
This version number is reserved fo future use when sources might be retrieved.

### Version 2
Version 2.0 sources are lost for now.
This version number is reserved fo future use when sources might be retrieved.

### Version 3
Versions 3.x are based on version 1.1.1 because I could not retrieve sources for 2.0 or 1.2

astroGenerator version 3.0 is astroGenerator 1.1.1 ported to Qt 5.5.

The goal is to have astroGenerator working with Qt 5. The choice of 5.5 is to accomodate for Qt WebKit. The code itself is modified only to allow building with Qt 5 and to rename the software.

astroGenerator version 3.1+ is the same as 3.0, but ported to Qt 5.15.
The code is modified to remove dependancies to Qt WebKit

## Authors
Original author: Valentin Gallouedec

Translators: Vincent Hourdin

## License
This work is licensed under the Creative Commons Attribution-NonCommercial 3.0 Unported License. To view a copy of this license, visit http://creativecommons.org/licenses/by-nc/3.0/ or send a letter to Creative Commons, PO Box 1866, Mountain View, CA 94042, USA.

A copy is also provided in the LICENSE file.

## How to build
### Requirements

This version only works on Windows with MinGW.

- Qt 4.8
- MinGW 4.8.2

### Build steps

Generate translations:
```cmd
lrelease astrogenerator.pro
```

Build:
```cmd
cd ..
mkdir build-astrogenerator
cd build-astrogenerator
qmake ../astrogenerator/astrogenerator.pro
mingw32-make
mingw32-make install
```

The `make install` step will copy required files and assets in the build directory. This includes Qt DLLs, Qt plugins, images, ...

Finally you will need to copy MinGW DLLs in the build directory:

```cmd
copy C:\Qt\Tools\mingw482_32\bin\libgcc_s_dw2-1.dll C:\Users\Bob\build-astrogenerator\.
copy C:\Qt\Tools\mingw482_32\bin\libstdc++-6.dll C:\Users\Bob\build-astrogenerator\.
copy C:\Qt\Tools\mingw482_32\bin\libwinpthread-1.dll C:\Users\Bob\build-astrogenerator\.
```

### Linux, macOs and others

Since the only dependancy is Qt 4, this should build on any platform where Qt 4 is available.

The only shortcoming is the handling of deployments. There are 2 solutions:

1. Update the .pro so that it does not deploy DLLs and plugins during the install step.
2. Do not run the install step and manually copy-paste to the build directory:
   - icones/
   - images/
   - VSOP87/
   - *.txt
   - dbastrogenerator