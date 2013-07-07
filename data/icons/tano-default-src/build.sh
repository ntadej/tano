#! /bin/bash
#############################################################################
# Tano - An Open IP TV Player
# Copyright (C) 2013 Tadej Novak <tadej@tano.si>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>./
#############################################################################

for t in *
do
    if test -d "$t"
    then
        pushd $t
        pushd "normal"
        for i in *
        do
            if test -f "$i"
            then
                filename=$(basename "$i")
                extension="${filename##*.}"
                filename="${filename%.*}"

                $TANO_INKSCAPE --export-png ../../../tano-default/$t/32/$filename.png -w 32 -h 32 $filename.svg
                $TANO_INKSCAPE --export-png ../../../tano-default@2x/$t/64/$filename.png -w 64 -h 64 $filename.svg
                $TANO_INKSCAPE --export-png ../../../tano-default/$t/48/$filename.png -w 48 -h 48 $filename.svg
                $TANO_INKSCAPE --export-png ../../../tano-default@2x/$t/96/$filename.png -w 96 -h 96 $filename.svg

                $TANO_INKSCAPE --export-png ../../../tano-default/$t/16/$filename.png -w 16 -h 16 ../extrasmall/$filename.svg
                $TANO_INKSCAPE --export-png ../../../tano-default@2x/$t/32/$filename.png -w 32 -h 32 ../extrasmall/$filename.svg

                $TANO_INKSCAPE --export-png ../../../tano-default/$t/22/$filename.png -w 22 -h 22 ../small/$filename.svg
                $TANO_INKSCAPE --export-png ../../../tano-default@2x/$t/44/$filename.png -w 44 -h 44 ../small/$filename.svg
                $TANO_INKSCAPE --export-png ../../../tano-default/$t/24/$filename.png -w 24 -h 24 ../small/$filename.svg
                $TANO_INKSCAPE --export-png ../../../tano-default@2x/$t/48/$filename.png -w 48 -h 48 ../small/$filename.svg
            fi
        done
        popd
        popd
    fi
done
