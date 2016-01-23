# Tano [![Build status][appveyor-img]][appveyor]
[![Homepage][web-img]][web]
[![Latest release][release-img]][release]
[![Download][download-img]][download]
[![License][license-img]][license]

Tano is an open-source cross-platform IP TV player. It features full IP TV
playback with EPG and recorder. Project started because of a need of a simple
IP TV player on Linux providing EPG.

Tano runs on supported versions of Linux, OS X and Windows. It is developed
in cross-platform spirit so it may work on other systems but is not supported.
Playlists and other copyright material are not provided.


## Binaries and source code

You can download Tano binaries and sources from the [official web page](https://tano.si/tano-player/)
or [GitHub releases](https://github.com/ntadej/tano/releases).

Latest source is also available via Git:
```bash
  git clone git://github.com/ntadej/tano.git
```

Master branch is considered stable and working on all supported platforms.
All development is made in branches. You can also find specific releases by
browsing tags.


## Requirements

Tano can be built with the following compilers: g++, clang, MSVC.
Build files are generated using [CMake](http://www.cmake.org) (3.0.2 or later).
It requires [VLC-Qt](https://vlc-qt.tano.si) 1.0 or later and Qt 5.


## Building

After running CMake, build with your compiler, depending on makefile generator:
Unix based: `make`, MSVC: `nmake` or `jom`

There are two platform specific targets. You can make DMG file on OS X by running
`make dmg` and you can copy required Qt libraries with `make windows`.


## Copyright info

Copyright (C) 2010-2016 Tadej Novak

Tano is free (libre) software. This means that the application
source code is available to public, anyone is welcome to research
how the application works, participate in its development, freely
distribute the application and spread the word!

This project may be used under the terms of the
GNU General Public License version 3.0 as published by the
Free Software Foundation and appearing in the file LICENSE.md.


[web]: https://tano.si/tano-player/
[release]: https://github.com/ntadej/tano/releases/latest
[download]: https://github.com/ntadej/tano/releases
[license]: https://github.com/ntadej/tano/blob/master/LICENSE.md
[appveyor]: https://ci.appveyor.com/project/ntadej/tano

[web-img]: https://img.shields.io/badge/web-tano.si%2Ftano--player%2F-green.svg
[license-img]: https://img.shields.io/github/license/ntadej/tano.svg
[release-img]: https://img.shields.io/github/release/ntadej/tano.svg
[download-img]: https://img.shields.io/github/downloads/ntadej/tano/total.svg
[appveyor-img]: https://ci.appveyor.com/api/projects/status/0uq1tnrnju2i7twa/branch/master?svg=true
