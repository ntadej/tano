Tano - an open IP TV player
====================================
Tano is an open-source cross-platform IP TV player. It features
full IP TV playback with EPG and recorder. Project started
because of a need of a simple IP TV player on Linux providing EPG.
Now after more than three years of development, Tano reached its
stable release.

Tano runs on Linux and Windows,
other operating systems are currently not supported.
Playlists and other copyright material are not provided.


Get Tano
--------
You can download Tano from the official web page
(http://projects.tano.si) or using Git:

    $ git clone git://github.com/ntadej/tano.git

If you want to build with QML UI (Qt5 only) run

    $ git submodule init
    $ git submodule update


Dependencies
------------
- g++, make
- cmake >= 2.8.6
- Qt >= 4.8
- libvlc-qt >= 0.7.0


Compilation
-----------
    $ mkdir build
    $ cd build
    $ cmake .. -DCMAKE_INSTALL_PREFIX=prefix
    $ make

To build with QML UI (Qt5 only) replace cmake command with

    $ cmake .. -DCMAKE_INSTALL_PREFIX=prefix -DBUILD_QML=ON


Installation
------------
    $ make install


Copyright info
--------------
Copyright (C) 2008-2012 Tadej Novak

Tano is free (libre) software. This means that the application
source code is available to public, anyone is welcome to research
how the application works, participate in its development, freely
distribute the application and spread the word!

This project may be used under the terms of the
GNU General Public License version 3.0 as published by the
Free Software Foundation and appearing in the file LICENSE.GPL.
