#! /bin/bash
#############################################################################
# Tano - An Open IP TV Player
# Copyright (C) 2016 Tadej Novak <tadej@tano.si>
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

inkscape --export-png ../volume/volumeslider-handle.png -w 16 volumeslider-handle.svg
inkscape --export-png ../volume/volumeslider-handle@2x.png -w 32 volumeslider-handle.svg

inkscape --export-png ../volume/volumeslider-handle_glow.png -w 16 volumeslider-handle_glow.svg
inkscape --export-png ../volume/volumeslider-handle_glow@2x.png -w 32 volumeslider-handle_glow.svg

inkscape --export-png ../volume/volumeslider-gradient.png -w 112 volumeslider-gradient.svg
