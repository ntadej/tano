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

#ifndef MEDIASETTINGS_H
#define MEDIASETTINGS_H

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QSize>

/**
 * Settings for the MPlayer process.
 *
 * MPlayer uses command line arguments.
 * Here are all the arguments that we be given to MPlayer command line.
 *
 * FIXME This class is still dirty and some code needs to be removed.
 * All javadoc commented variables are clean and tested, others are not.
 *
 * @see MPlayerLoader
 * @author Tanguy Krotoff
 */
class MediaSettings {
public:

//Not clean and not used
	enum Aspect { AspectAuto = 1, Aspect43 = 2, Aspect169 = 3, Aspect235 = 4,
		Aspect149 = 8, Aspect1610 = 9, Aspect54 = 10 };

	enum AudioChannels { ChDefault = 0, ChStereo = 2, ChSurround = 4,
		ChFull51 = 6 };

	enum StereoMode { Stereo = 0, Left = 1, Right = 2 };

	enum IDs { NoneSelected = -1000, SubNone = 90000 };
//!Not clean and not used

	MediaSettings();
	~MediaSettings();

	void clear();

	/**
	 * MPlayer volume, see MPlayer manpage.
	 *
	 * -volume <-1-100> (also see -af volume)
	 * Sets the startup volume in the mixer, either hardware or software (if used with -softvol).
	 * A value of -1 (the default) will not change the volume.
	 */
	int volume;

	/** Video contrast. */
	int contrast;

	/** Video brightness. */
	int brightness;

	/** Video hue. */
	int hue;

	/** Video saturation. */
	int saturation;

	/**
	 * Audio filters.
	 *
	 * - "karaoke"
	 * - "extrastereo"
	 * - "volnorm=2"
	 */
	QStringList audioFilters;

	/**
	 * Video filters.
	 */
	QStringList videoFilters;

	/**
	 * Optial device name for MPlayer.
	 *
	 * -dvd-device argument for MPlayer
	 * <pre>mplayer dvd://<title> -dvd-device d:</pre>
	 * <pre>mplayer dvd://1 -dvd-device /dev/dvd</pre>
	 *
	 * -cdrom-device argument for MPlayer
	 * <pre>mplayer vcd://<track> -cdrom-device d:</pre>
	 * <pre>mplayer vcd://1 -cdrom-device /dev/cdrom</pre>
	 * <pre>mplayer cdda://1 -cdrom-device /dev/cdrom</pre>
	 */
	QString opticalDeviceName;


//FIXME Everything after this point is not clean and is not used!

	int gamma;

	double current_sec;
	int current_sub_id;

	int current_audio_id;

	int current_title_id;
	int current_chapter_id;
	int current_angle_id;

	int aspect_ratio_id;

	//bool fullscreen;

	QString external_subtitles;
	QString external_audio; // external audio file

	int sub_delay;
	int audio_delay;

	// Subtitles position (0-100)
	int sub_pos;
	double sub_scale;

	double sub_scale_ass;


	double speed; // Speed of playback: 1.0 = normal speed

	int current_deinterlacer;

	bool add_letterbox;

	int audio_use_channels;
	int stereo_mode;

	double panscan_factor; // mplayerwindow zoom

	// This a property of the video and it should be
	// in mediadata, but we have to save it to preserve
	// this data among restarts.
	double starting_time; // Some videos don't start at 0

	//! The codec of the video is ffh264 and it's high definition
	bool is264andHD;

	// Advanced settings
	QString forced_demuxer;
	QString forced_video_codec;
	QString forced_audio_codec;

	// A copy of the original values, so we can restore them.
	QString original_demuxer;
	QString original_video_codec;
	QString original_audio_codec;

	// Options to mplayer (for this file only)
	QString mplayer_additional_options;
	QString mplayer_additional_video_filters;
	QString mplayer_additional_audio_filters;

	// Some things that were before in mediadata
	// They can vary, because of filters, so better here

	//Resolution used by mplayer
	//Can be bigger that video resolution
	//because of the aspect ratio or expand filter
	int win_width;
	int win_height;
	double win_aspect();
};

#endif	//MEDIASETTINGS_H
