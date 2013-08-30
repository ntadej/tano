#!/bin/bash
#############################################################################
# Tano - An Open IP TV Player
# Copyright (C) 2013 Tadej Novak <tadej@tano.si>
#
# This library is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published
# by the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this library. If not, see <http://www.gnu.org/licenses/>.
#############################################################################

set -eu

DEVEL=""
TARGET=""
BUILD="1"
CUSTOM=""
VLCQT=""
while getopts "hdt:b:c:i:" OPTION
do
  case $OPTION in
      h)
          echo "Quick build system for Tano."
          echo ""
          echo "Use -t to specify target (in targets directory)"
          echo "Use -d to ignore dependency requests (Linux only)"
          echo "Use -b to specify build number (defaults to 1, Linux only)"
          echo "Use -c to specify branded build name (defaults to disabled)"
          echo "Use -i to specify VLC-QT include (OS X only)"
          exit 0
          ;;
      d)
          DEVEL="-d"
          ;;
      t)
          TARGET="$OPTARG"
          ;;
      b)
          BUILD="$OPTARG"
          ;;
      c)
          CUSTOM="$OPTARG"
          ;;
      i)
          VLCQT="../../../$OPTARG"
          ;;
  esac
done


if [[ -z $TARGET ]]; then
    echo "Target (-t) not specified"
    exit 1
fi


if [ -f targets/${TARGET}.sh ]; then
    source targets/${TARGET}.sh
else
    echo "Target $TARGET not supported"
    exit 1
fi
