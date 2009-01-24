/*
 * MPlayer backend for the Phonon library
 * Copyright (C) 2006-2008  Ricardo Villalba <rvm@escomposlinux.org>
 * Copyright (C) 2007-2008  Tanguy Krotoff <tkrotoff@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "MediaSettings.h"

MediaSettings::MediaSettings() {
	clear();
}

MediaSettings::~MediaSettings() {
}

void MediaSettings::clear() {
	//From MPlayer manpage: a value of -1 (the default) will not change the volume
	volume = -1;

	brightness = 0;
	contrast = 0;
	hue = 0;
	saturation = 0;

	audioFilters.clear();
	videoFilters.clear();

	opticalDeviceName.clear();


//FIXME Everything after this point is not clean and not used!

	gamma = 0;

	aspect_ratio_id = AspectAuto;

	//fullscreen = FALSE;

	external_subtitles.clear();
	external_audio.clear();
	sub_delay = 0;
	audio_delay = 0;
	sub_pos = 100; // 100% by default
	sub_scale = 5;

	sub_scale_ass = 1;

	speed = 1.0;

	add_letterbox = false;

	audioFilters.clear();
	videoFilters.clear();

	audio_use_channels = ChDefault; // (0)
	stereo_mode = Stereo; // (0)

	panscan_factor = 1.0;

	// Not set yet.
	starting_time = -1;

	is264andHD = false;

	forced_demuxer.clear();
	forced_video_codec.clear();
	forced_audio_codec.clear();

	original_demuxer.clear();
	original_video_codec.clear();
	original_audio_codec.clear();

	mplayer_additional_options.clear();
	mplayer_additional_video_filters.clear();
	mplayer_additional_audio_filters.clear();

	win_width = 400;
	win_height = 300;
}

double MediaSettings::win_aspect() {
	return (double) win_width / win_height;
}
