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
astroGenerator is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

astroGenerator is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with astroGenerator.  If not, see <https://www.gnu.org/licenses/>.

## How to build
### Requirements
Since the only dependancy is Qt 5, this should build on any platform where Qt 5 is available.
However, it has mostly been tested on Window with MinGW.

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
windeployqt astroGenerator.exe
```

The `make install` step will copy required files and assets in the build directory.

### Linux, macOs and others

The only shortcoming is the handling of deployments.
On macOs you can replace `windeployqt` by `macdeployqt`.

On Linux, if you use your system installed Qt, you can skip this step.
