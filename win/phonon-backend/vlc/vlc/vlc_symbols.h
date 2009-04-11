/*
 * VLC_Backend backend for the Phonon library
 * Copyright (C) 2007-2008  Tanguy Krotoff <tkrotoff@gmail.com>
 * 					2009	Tadej Novak <tadej@pfusion.co.cc>
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

#ifndef VLC_SYMBOLS_H
#define VLC_SYMBOLS_H

#include <vlc/vlc.h>

#include <QtCore/QString>

/**
 * Loads libvlc-control.(dll|so|dylib), determines and returns the VLC_Backend path.
 *
 * Examples:
 * C:/Progam Files/VideoLAN/VLC_Backend
 * /usr/local/lib
 *
 * Under Windows, in order to determine libvlc-control.dll,
 * we use Win32 API function SetCurrentDirectory()
 *
 * @return the VLE path
 * @see changeBackToCurrentDirectory()
 */
QString getVLCPath();

/**
 * Gets the VLC_Backend plugin path.
 *
 * Examples:
 * C:/Program Files/VideoLAN/VLC_Backend/plugins
 * /usr/local/lib/vlc
 *
 * @return the VLC_Backend plugin path
 */
QString getVLCPluginPath();

/**
 * Unloads libvlc.
 */
void unloadLibVLC();

/**
 * Internal, resolves a libvlc function name.
 */
void * resolve(const char * name);

/**
 * Macro inspired from Qt-4.4.0, files dbus_symbols.h and dbus_symbols.cpp
 */
#define DEFINEFUNC(ret, func, args, argcall, funcret)		\
	typedef ret (* _p_PTR_##func) args;			\
	static inline ret p_##func args {			\
		_p_PTR_##func ptr = 0;				\
		ptr = (_p_PTR_##func) resolve(#func);		\
		funcret ptr argcall;				\
	}

/*
Macro expanded:

	typedef const char * (* _p_PTR_libvlc_exception_get_message) (const libvlc_exception_t * p_e);

	static inline const char * p_libvlc_exception_get_message(const libvlc_exception_t * p_e) {
		_p_PTR_libvlc_exception_get_message ptr = 0;
		ptr = (_p_PTR_libvlc_exception_get_message) resolve("libvlc_exception_get_message");
		return ptr(p_e);
	}
*/

/* Definitions from libvlc.h */

DEFINEFUNC(void, libvlc_exception_init, (libvlc_exception_t * p_e), (p_e), )

DEFINEFUNC(int, libvlc_exception_raised, (const libvlc_exception_t * p_e), (p_e), return)

DEFINEFUNC(void, libvlc_exception_raise, (libvlc_exception_t * p_e, const char * psz_format, ...), (p_e, psz_format), )

DEFINEFUNC(void, libvlc_exception_clear, (libvlc_exception_t * p_e), (p_e), )

DEFINEFUNC(const char *, libvlc_exception_get_message, (
						const libvlc_exception_t * p_e),
						(p_e), return)

DEFINEFUNC(libvlc_instance_t *, libvlc_new, (int argc, const char * const * argv, libvlc_exception_t * p_e), (argc, argv, p_e), return)

DEFINEFUNC(int, libvlc_get_vlc_id, (libvlc_instance_t * p_i), (p_i), return)

DEFINEFUNC(void, libvlc_release, (libvlc_instance_t * p_i), (p_i), )

DEFINEFUNC(void, libvlc_retain, (libvlc_instance_t * p_i), (p_i), )

DEFINEFUNC(const char *, libvlc_get_version, (), (), return)

DEFINEFUNC(const char *, libvlc_get_compiler, (), (), return)

DEFINEFUNC(const char *, libvlc_get_changeset, (), (), return)

DEFINEFUNC(libvlc_media_t *, libvlc_media_new, (
				libvlc_instance_t * p_i,
				const char * psz_mrl,
				libvlc_exception_t * p_e),
				(p_i, psz_mrl, p_e), return)

DEFINEFUNC(libvlc_media_t *, libvlc_media_new_as_node, (
				libvlc_instance_t * p_i,
				const char * psz_name,
				libvlc_exception_t * p_e),
				(p_i, psz_name, p_e), return)

DEFINEFUNC(void, libvlc_media_add_option, (
				libvlc_media_t * p_md,
				const char * ppsz_options,
				libvlc_exception_t * p_e),
				(p_md, ppsz_options, p_e), )

DEFINEFUNC(void, libvlc_media_retain, (
				libvlc_media_t * p_md),
				(p_md), )

DEFINEFUNC(void, libvlc_media_release, (
				libvlc_media_t * p_md),
				(p_md), )

DEFINEFUNC(char *, libvlc_media_get_mrl, (
						libvlc_media_t * p_md,
						libvlc_exception_t * p_e),
						(p_md, p_e), return)

DEFINEFUNC(libvlc_media_t *, libvlc_media_duplicate, (libvlc_media_t * p_md), (p_md), return)

DEFINEFUNC(char *, libvlc_media_get_meta, (
				libvlc_media_t * p_md,
				libvlc_meta_t e_meta,
				libvlc_exception_t * p_e),
				(p_md, e_meta, p_e), return)

DEFINEFUNC(libvlc_state_t, libvlc_media_get_state, (
				libvlc_media_t * p_md,
				libvlc_exception_t * p_e),
				(p_md, p_e), return)

DEFINEFUNC(int, libvlc_media_tags_count_for_key, (
						libvlc_media_t * p_md,
						const char * key,
						libvlc_exception_t * p_e),
						(p_md, key, p_e), return)

DEFINEFUNC(libvlc_media_list_t *, libvlc_media_subitems, (
				libvlc_media_t * p_md,
				libvlc_exception_t * p_e),
				(p_md, p_e), return)

DEFINEFUNC(libvlc_event_manager_t *, libvlc_media_event_manager, (
					libvlc_media_t * p_md,
					libvlc_exception_t * p_e),
					(p_md, p_e), return)

DEFINEFUNC(libvlc_time_t, libvlc_media_get_duration, (
					libvlc_media_t * p_md,
					libvlc_exception_t * p_e),
					(p_md, p_e), return)

DEFINEFUNC(int, libvlc_media_is_preparsed, (
					libvlc_media_t * p_md,
					libvlc_exception_t * p_e),
					(p_md, p_e), return)

DEFINEFUNC(void, libvlc_media_set_user_data, (
					libvlc_media_t * p_md,
					void * p_new_user_data,
					libvlc_exception_t * p_e),
					(p_md, p_new_user_data, p_e), )

DEFINEFUNC(void *, libvlc_media_get_user_data, (
					libvlc_media_t * p_md,
					libvlc_exception_t * p_e),
					(p_md, p_e), return)

DEFINEFUNC(void, libvlc_playlist_loop, (
					libvlc_instance_t * p_i, int loop,
					libvlc_exception_t * p_e),
					(p_i, loop, p_e), )

DEFINEFUNC(void, libvlc_playlist_play, (
					libvlc_instance_t * p_i, int i_id, int i_options,
					char ** ppsz_options, libvlc_exception_t * p_e),
					(p_i, i_id, i_options, ppsz_options, p_e), )

DEFINEFUNC(void, libvlc_playlist_pause, (
					libvlc_instance_t * p_i,
					libvlc_exception_t * p_e),
					(p_i, p_e), )

DEFINEFUNC(int, libvlc_playlist_isplaying, (
					libvlc_instance_t * p_i,
					libvlc_exception_t * p_e),
					(p_i, p_e), return)

DEFINEFUNC(int, libvlc_playlist_items_count, (
						libvlc_instance_t * p_i,
						libvlc_exception_t * p_e),
						(p_i, p_e), return)

DEFINEFUNC(void, libvlc_playlist_lock, (libvlc_instance_t * p_i), (p_i), )

DEFINEFUNC(void, libvlc_playlist_unlock, (libvlc_instance_t * p_i), (p_i), )

DEFINEFUNC(void, libvlc_playlist_stop, (
					libvlc_instance_t * p_i,
					libvlc_exception_t * p_e),
					(p_i, p_e), )

DEFINEFUNC(void, libvlc_playlist_next, (
					libvlc_instance_t * p_i,
					libvlc_exception_t * p_e),
					(p_i, p_e), )

DEFINEFUNC(void, libvlc_playlist_prev, (
					libvlc_instance_t * p_i,
					libvlc_exception_t * p_e),
					(p_i, p_e), )

DEFINEFUNC(void, libvlc_playlist_clear, (
					libvlc_instance_t * p_i,
					libvlc_exception_t * p_e),
					(p_i, p_e), )

DEFINEFUNC(int, libvlc_playlist_add, (
					libvlc_instance_t * p_i, const char * psz_uri,
					const char * psz_name, libvlc_exception_t * p_e),
					(p_i, psz_uri, psz_name, p_e), return)

DEFINEFUNC(int, libvlc_playlist_add_extended, (
						libvlc_instance_t * p_i, const char * psz_uri,
						const char * psz_name, int i_options, const char ** ppsz_options,
						libvlc_exception_t * p_e),
						(p_i, psz_uri, psz_name, i_options, ppsz_options, p_e), return)

DEFINEFUNC(int, libvlc_playlist_delete_item, (
						libvlc_instance_t * p_i, int i_id,
						libvlc_exception_t * p_e),
						(p_i, i_id, p_e), return)

DEFINEFUNC(libvlc_media_player_t *, libvlc_playlist_get_media_instance, (
				libvlc_instance_t * p_i, libvlc_exception_t * p_e), (p_i, p_e), return)

DEFINEFUNC(int, libvlc_media_player_is_seekable, (
				libvlc_media_player_t * p_mi,
				libvlc_exception_t * p_e),
				(p_mi, p_e), return)

DEFINEFUNC(int, libvlc_media_player_can_pause, (
				libvlc_media_player_t * p_mi,
				libvlc_exception_t * p_e),
				(p_mi, p_e), return)

DEFINEFUNC(libvlc_media_player_t *, libvlc_media_player_new, (libvlc_instance_t * p_i, libvlc_exception_t * p_e), (p_i, p_e), return)

DEFINEFUNC(libvlc_media_player_t *, libvlc_media_player_new_from_media, (
						libvlc_media_t * p_md,
						libvlc_exception_t * p_e),
						(p_md, p_e), return)

DEFINEFUNC(void, libvlc_media_player_release, (libvlc_media_player_t * p_mi), (p_mi), )

DEFINEFUNC(void, libvlc_media_player_retain, (libvlc_media_player_t * p_mi), (p_mi), )

DEFINEFUNC(void, libvlc_media_player_set_media, (libvlc_media_player_t * p_mi, libvlc_media_t * p_md, libvlc_exception_t * p_e), (p_mi, p_md, p_e), )

DEFINEFUNC(libvlc_media_t *, libvlc_media_player_get_media, (libvlc_media_player_t * p_mi, libvlc_exception_t * p_e), (p_mi, p_e), return)

DEFINEFUNC(libvlc_event_manager_t *, libvlc_media_player_event_manager, (
						libvlc_media_player_t * p_mi,
						libvlc_exception_t * p_e),
						(p_mi, p_e), return)

DEFINEFUNC(void, libvlc_media_player_play, (libvlc_media_player_t * p_mi, libvlc_exception_t * p_e), (p_mi, p_e), )

DEFINEFUNC(void, libvlc_media_player_pause, (libvlc_media_player_t * p_mi, libvlc_exception_t * p_e), (p_mi, p_e), )

DEFINEFUNC(void, libvlc_media_player_stop, (libvlc_media_player_t * p_mi, libvlc_exception_t * p_e), (p_mi, p_e), )

DEFINEFUNC(void, libvlc_media_player_set_drawable, (
				libvlc_media_player_t * p_mi,
				libvlc_drawable_t drawable,
				libvlc_exception_t * p_e),
				(p_mi, drawable, p_e), )

DEFINEFUNC(libvlc_drawable_t, libvlc_media_player_get_drawable, (libvlc_media_player_t * p_mi, libvlc_exception_t * p_e), (p_mi, p_e), return)

DEFINEFUNC(libvlc_time_t, libvlc_media_player_get_length, (libvlc_media_player_t * p_mi, libvlc_exception_t * p_e), (p_mi, p_e), return)

DEFINEFUNC(libvlc_time_t, libvlc_media_player_get_time, (libvlc_media_player_t * p_mi, libvlc_exception_t * p_e), (p_mi, p_e), return)

DEFINEFUNC(void, libvlc_media_player_set_time, (libvlc_media_player_t * p_mi, libvlc_time_t time, libvlc_exception_t * p_e), (p_mi, time, p_e), return)

DEFINEFUNC(float, libvlc_media_player_get_position, (libvlc_media_player_t * p_mi, libvlc_exception_t * p_e), (p_mi, p_e), return)

DEFINEFUNC(void, libvlc_media_player_set_position, (libvlc_media_player_t * p_mi, float pos, libvlc_exception_t * p_e), (p_mi, pos, p_e), )

DEFINEFUNC(void, libvlc_media_player_set_chapter, (libvlc_media_player_t * p_mi, int chapter, libvlc_exception_t * p_e), (p_mi, chapter, p_e), )

DEFINEFUNC(int, libvlc_media_player_get_chapter, (libvlc_media_player_t * p_mi, libvlc_exception_t * p_e), (p_mi, p_e), return)

DEFINEFUNC(int, libvlc_media_player_get_chapter_count, (libvlc_media_player_t * p_mi, libvlc_exception_t * p_e), (p_mi, p_e), return)

DEFINEFUNC(int, libvlc_media_player_will_play, (libvlc_media_player_t * p_mi, libvlc_exception_t * p_e), (p_mi, p_e), return)

DEFINEFUNC(float, libvlc_media_player_get_rate, (libvlc_media_player_t * p_mi, libvlc_exception_t * p_e), (p_mi, p_e), return)

DEFINEFUNC(void, libvlc_media_player_set_rate, (libvlc_media_player_t * p_mi, float rate, libvlc_exception_t * p_e), (p_mi, rate, p_e), return)

DEFINEFUNC(libvlc_state_t, libvlc_media_player_get_state, (libvlc_media_player_t * p_mi, libvlc_exception_t * p_e), (p_mi, p_e), return)

DEFINEFUNC(int, libvlc_media_player_has_vout, (libvlc_media_player_t * p_mi, libvlc_exception_t * p_e), (p_mi, p_e), return)

DEFINEFUNC(float, libvlc_media_player_get_fps, (libvlc_media_player_t * p_mi, libvlc_exception_t * p_e), (p_mi, p_e), return)

DEFINEFUNC(libvlc_media_list_t *, libvlc_media_list_new, (libvlc_instance_t * p_i, libvlc_exception_t * p_e), (p_i, p_e), return)

DEFINEFUNC(void, libvlc_media_list_release, (libvlc_media_list_t * p_mlist), (p_mlist), )

DEFINEFUNC(void, libvlc_media_list_retain, (libvlc_media_list_t * p_mlist), (p_mlist), )

DEFINEFUNC(void, libvlc_media_list_add_file_content, (
					libvlc_media_list_t * p_mlist,
					const char * psz_uri,
					libvlc_exception_t * p_e),
					(p_mlist, psz_uri, p_e), )

DEFINEFUNC(void, libvlc_media_list_set_media, (
					libvlc_media_list_t * p_mlist,
					libvlc_media_t * p_md,
					libvlc_exception_t * p_e),
					(p_mlist, p_md, p_e), )

DEFINEFUNC(libvlc_media_t *, libvlc_media_list_media, (
					libvlc_media_list_t * p_mlist,
					libvlc_exception_t * p_e),
					(p_mlist, p_e), return)

DEFINEFUNC(void, libvlc_media_list_add_media, (
					libvlc_media_list_t * p_mlist,
					libvlc_media_t * p_md,
					libvlc_exception_t * p_e),
					(p_mlist, p_md, p_e), )

DEFINEFUNC(void, libvlc_media_list_insert_media, (
					libvlc_media_list_t * p_mlist,
					libvlc_media_t * p_md,
					int index,
					libvlc_exception_t * p_e),
					(p_mlist, p_md, index, p_e), )

DEFINEFUNC(void, libvlc_media_list_remove_index, (
				libvlc_media_list_t * p_mlist,
				int index,
				libvlc_exception_t * p_e),
				(p_mlist, index, p_e), )

DEFINEFUNC(int, libvlc_media_list_count, (
			libvlc_media_list_t * p_mlist,
			libvlc_exception_t * p_e),
			(p_mlist, p_e), return)

DEFINEFUNC(libvlc_media_t *, libvlc_media_list_item_at_index, (
				libvlc_media_list_t * p_mlist,
				int index,
				libvlc_exception_t * p_e),
				(p_mlist, index, p_e), return)

DEFINEFUNC(int, libvlc_media_list_index_of_item, (
				libvlc_media_list_t * p_mlist,
				libvlc_media_t * p_md,
				libvlc_exception_t * p_e),
				(p_mlist, p_md, p_e), return)

DEFINEFUNC(int, libvlc_media_list_is_readonly, (libvlc_media_list_t * p_mlist), (p_mlist), return)

DEFINEFUNC(void, libvlc_media_list_lock, (libvlc_media_list_t * p_mlist), (p_mlist), )

DEFINEFUNC(void, libvlc_media_list_unlock, (libvlc_media_list_t * p_mlist), (p_mlist), )

DEFINEFUNC(libvlc_media_list_view_t *, libvlc_media_list_flat_view, (
				libvlc_media_list_t * p_mlist,
				libvlc_exception_t * p_e),
				(p_mlist, p_e), return)

DEFINEFUNC(libvlc_media_list_view_t *, libvlc_media_list_hierarchical_view, (
					libvlc_media_list_t * p_mlist,
					libvlc_exception_t * p_e),
					(p_mlist, p_e), return)

DEFINEFUNC(libvlc_media_list_view_t *, libvlc_media_list_hierarchical_node_view, (
					libvlc_media_list_t * p_mlist,
					libvlc_exception_t * p_e),
					(p_mlist, p_e), return)

DEFINEFUNC(libvlc_event_manager_t *, libvlc_media_list_event_manager, (
				libvlc_media_list_t * p_mlist,
				libvlc_exception_t * p_e),
				(p_mlist, p_e), return)

DEFINEFUNC(void, libvlc_media_list_view_retain, (libvlc_media_list_view_t * p_mlv), (p_mlv), )

DEFINEFUNC(void, libvlc_media_list_view_release, (libvlc_media_list_view_t * p_mlv), (p_mlv), )

DEFINEFUNC(libvlc_event_manager_t *, libvlc_media_list_view_event_manager, (libvlc_media_list_view_t * p_mlv), (p_mlv), return)

DEFINEFUNC(int, libvlc_media_list_view_count, (
				libvlc_media_list_view_t * p_mlv,
				libvlc_exception_t * p_e),
				(p_mlv, p_e), return)

DEFINEFUNC(libvlc_media_t *, libvlc_media_list_view_item_at_index, (
					libvlc_media_list_view_t * p_mlv,
					int index,
					libvlc_exception_t * p_e),
					(p_mlv, index, p_e), return)

DEFINEFUNC(libvlc_media_list_view_t *, libvlc_media_list_view_children_at_index, (
					libvlc_media_list_view_t * p_mlv,
					int index,
					libvlc_exception_t * p_e),
					(p_mlv, index, p_e), return)

DEFINEFUNC(libvlc_media_list_view_t *, libvlc_media_list_view_children_for_item, (
					libvlc_media_list_view_t * p_mlv,
					libvlc_media_t * p_md,
					libvlc_exception_t * p_e),
					(p_mlv, p_md, p_e), return)

DEFINEFUNC(int, libvlc_media_list_view_index_of_item, (
					libvlc_media_list_view_t * p_mlv,
					libvlc_media_t * p_md,
					libvlc_exception_t * p_e),
					(p_mlv, p_md, p_e), return)

DEFINEFUNC(void, libvlc_media_list_view_insert_at_index, (
					libvlc_media_list_view_t * p_mlv,
					libvlc_media_t * p_md,
					int index,
					libvlc_exception_t * p_e),
					(p_mlv, p_md, index, p_e), )

DEFINEFUNC(void, libvlc_media_list_view_remove_at_index, (
					libvlc_media_list_view_t * p_mlv,
					int index,
					libvlc_exception_t * p_e),
					(p_mlv, index, p_e), return)

DEFINEFUNC(void, libvlc_media_list_view_add_item, (
				libvlc_media_list_view_t * p_mlv,
				libvlc_media_t * p_md,
				libvlc_exception_t * p_e),
				(p_mlv, p_md, p_e), )

DEFINEFUNC(libvlc_media_list_t *, libvlc_media_list_view_parent_media_list, (
					libvlc_media_list_view_t * p_mlv,
					libvlc_exception_t * p_e),
					(p_mlv, p_e), return)

DEFINEFUNC(libvlc_media_library_t *, libvlc_media_library_new, (
							libvlc_instance_t * p_i,
							libvlc_exception_t * p_e),
							(p_i, p_e), return)

DEFINEFUNC(void, libvlc_media_library_release, (libvlc_media_library_t * p_mlib), (p_mlib), )

DEFINEFUNC(void, libvlc_media_library_retain, (libvlc_media_library_t * p_mlib), (p_mlib), )

DEFINEFUNC(void, libvlc_media_library_load, (
			libvlc_media_library_t * p_mlib,
			libvlc_exception_t * p_e),
			(p_mlib, p_e), )

DEFINEFUNC(void, libvlc_media_library_save, (
			libvlc_media_library_t * p_mlib,
			libvlc_exception_t * p_e),
			(p_mlib, p_e), )

DEFINEFUNC(libvlc_media_list_t *, libvlc_media_library_media_list, (
				libvlc_media_library_t * p_mlib,
				libvlc_exception_t * p_e),
				(p_mlib, p_e), return)

DEFINEFUNC(libvlc_media_list_player_t *, libvlc_media_list_player_new, (
				libvlc_instance_t * p_i,
				libvlc_exception_t * p_e),
				(p_i, p_e), return)

DEFINEFUNC(void, libvlc_media_list_player_release, (libvlc_media_list_player_t * p_mlp), (p_mlp), )

DEFINEFUNC(void, libvlc_media_list_player_set_media_instance, (
				libvlc_media_list_player_t * p_mlp,
				libvlc_media_player_t * p_mi,
				libvlc_exception_t * p_e),
				(p_mlp, p_mi, p_e), )

DEFINEFUNC(void, libvlc_media_list_player_set_media_list, (
				libvlc_media_list_player_t * p_mlp,
				libvlc_media_list_t * p_mlist,
				libvlc_exception_t * p_e),
				(p_mlp, p_mlist, p_e), )

DEFINEFUNC(void, libvlc_media_list_player_play, (
				libvlc_media_list_player_t * p_mlp,
				libvlc_exception_t * p_e),
				(p_mlp, p_e), )

DEFINEFUNC(void, libvlc_media_list_player_pause, (
				libvlc_media_list_player_t * p_mlp,
				libvlc_exception_t * p_e),
				(p_mlp, p_e), )

DEFINEFUNC(int, libvlc_media_list_player_is_playing, (
					libvlc_media_list_player_t * p_mlp,
					libvlc_exception_t * p_e),
					(p_mlp, p_e), return)

DEFINEFUNC(libvlc_state_t, libvlc_media_list_player_get_state, (
					libvlc_media_list_player_t * p_mlp,
					libvlc_exception_t * p_e),
					(p_mlp, p_e), return)

DEFINEFUNC(void, libvlc_media_list_player_play_item_at_index, (
				libvlc_media_list_player_t * p_mlp,
				int i_index,
				libvlc_exception_t * p_e),
				(p_mlp, i_index, p_e), )

DEFINEFUNC(void, libvlc_media_list_player_play_item, (
				libvlc_media_list_player_t * p_mlp,
				libvlc_media_t * p_md,
				libvlc_exception_t * p_e),
				(p_mlp, p_md, p_e), )

DEFINEFUNC(void, libvlc_media_list_player_stop, (
				libvlc_media_list_player_t * p_mlp,
				libvlc_exception_t * p_e),
				(p_mlp, p_e), )

DEFINEFUNC(void, libvlc_media_list_player_next, (
				libvlc_media_list_player_t * p_mlp,
				libvlc_exception_t * p_e),
				(p_mlp, p_e), )

DEFINEFUNC(void, libvlc_toggle_fullscreen, (libvlc_media_player_t * p_mi, libvlc_exception_t * p_e), (p_mi, p_e), )

DEFINEFUNC(void, libvlc_set_fullscreen, (libvlc_media_player_t * p_mi, int fullscreen, libvlc_exception_t * p_e), (p_mi, fullscreen, p_e), )

DEFINEFUNC(int, libvlc_get_fullscreen, (libvlc_media_player_t * p_mi, libvlc_exception_t * p_e), (p_mi, p_e), return)

DEFINEFUNC(int, libvlc_video_get_height, (libvlc_media_player_t * p_mi, libvlc_exception_t * p_e), (p_mi, p_e), return)

DEFINEFUNC(int, libvlc_video_get_width, (libvlc_media_player_t * p_mi, libvlc_exception_t * p_e), (p_mi, p_e), return)

DEFINEFUNC(char *, libvlc_video_get_aspect_ratio, (libvlc_media_player_t * p_mi, libvlc_exception_t * p_e), (p_mi, p_e), return)

DEFINEFUNC(void, libvlc_video_set_aspect_ratio, (libvlc_media_player_t * p_mi, char * psz_aspect, libvlc_exception_t * p_e), (p_mi, psz_aspect, p_e), )

DEFINEFUNC(int, libvlc_video_get_spu, (libvlc_media_player_t * p_mi, libvlc_exception_t * p_e), (p_mi, p_e), return)

DEFINEFUNC(void, libvlc_video_set_spu, (libvlc_media_player_t * p_mi, int i_spu, libvlc_exception_t * p_e), (p_mi, i_spu, p_e), )

DEFINEFUNC(bool, libvlc_video_set_subtitle_file, (libvlc_media_player_t * p_mi, char * psz_subtitle, libvlc_exception_t * p_e), (p_mi, psz_subtitle, p_e), return)

DEFINEFUNC(char *, libvlc_video_get_crop_geometry, (libvlc_media_player_t * p_mi, libvlc_exception_t * p_e), (p_mi, p_e), return)

DEFINEFUNC(void, libvlc_video_set_crop_geometry, (libvlc_media_player_t * p_mi, char * psz_geometry, libvlc_exception_t * p_e), (p_mi, psz_geometry, p_e), )

DEFINEFUNC(void, libvlc_video_filter_set_hue, (libvlc_media_player_t * p_mi, int i_value, libvlc_exception_t * p_e), (p_mi, i_value, p_e), )

DEFINEFUNC(void, libvlc_toggle_teletext, (libvlc_media_player_t * p_mi, libvlc_exception_t * p_e), (p_mi, p_e), )

DEFINEFUNC(int, libvlc_video_get_teletext, (libvlc_media_player_t * p_mi, libvlc_exception_t * p_e), (p_mi, p_e), return)

DEFINEFUNC(void, libvlc_video_set_teletext, (libvlc_media_player_t * p_mi, int i_page, libvlc_exception_t * p_e), (p_mi, i_page, p_e), )

DEFINEFUNC(void, libvlc_video_take_snapshot, (libvlc_media_player_t * p_mi, char * psz_filepath, unsigned int i_width, unsigned int i_height, libvlc_exception_t * p_e), (p_mi, psz_filepath, i_width, i_height, p_e), )

DEFINEFUNC(int, libvlc_video_destroy, (libvlc_media_player_t * p_mi, libvlc_exception_t * p_e), (p_mi, p_e), return)

DEFINEFUNC(void, libvlc_video_resize, (libvlc_media_player_t * p_mi, int i_width, int i_height, libvlc_exception_t * p_e), (p_mi, i_width, i_height, p_e), )

DEFINEFUNC(int, libvlc_video_reparent, (libvlc_media_player_t * p_mi, libvlc_drawable_t drawable, libvlc_exception_t * p_e), (p_mi, drawable, p_e), return)

DEFINEFUNC(void, libvlc_video_redraw_rectangle, (libvlc_media_player_t * p_mi, const libvlc_rectangle_t * area, libvlc_exception_t * p_e), (p_mi, area, p_e), )

DEFINEFUNC(void, libvlc_video_set_parent, (
				libvlc_instance_t * p_i,
				libvlc_drawable_t drawable,
				libvlc_exception_t * p_e),
				(p_i, drawable, p_e), )

DEFINEFUNC(libvlc_drawable_t, libvlc_video_get_parent, (libvlc_instance_t * p_i, libvlc_exception_t * p_e), (p_i, p_e), return)

DEFINEFUNC(void, libvlc_video_set_size, (libvlc_instance_t * p_i, int i_width, int i_height, libvlc_exception_t * p_e), (p_i, i_width, i_height, p_e), )

DEFINEFUNC(void, libvlc_video_set_viewport, (libvlc_instance_t * p_i, const libvlc_rectangle_t * view, const libvlc_rectangle_t * clip, libvlc_exception_t * p_e), (p_i, view, clip, p_e), )

DEFINEFUNC(void, libvlc_audio_toggle_mute, (libvlc_instance_t * p_i, libvlc_exception_t * p_e), (p_i, p_e), )

DEFINEFUNC(int, libvlc_audio_get_mute, (libvlc_instance_t * p_i, libvlc_exception_t * p_e), (p_i, p_e), return)

DEFINEFUNC(void, libvlc_audio_set_mute, (libvlc_instance_t * p_i, int status, libvlc_exception_t * p_e), (p_i, status, p_e), )

DEFINEFUNC(int, libvlc_audio_get_volume, (libvlc_instance_t * p_i, libvlc_exception_t * p_e), (p_i, p_e), return)

DEFINEFUNC(void, libvlc_audio_set_volume, (libvlc_instance_t * p_i, int i_volume, libvlc_exception_t * p_e), (p_i, i_volume, p_e), )

DEFINEFUNC(int, libvlc_audio_get_track_count, (libvlc_media_player_t * p_mi, libvlc_exception_t * p_e), (p_mi, p_e), return)

DEFINEFUNC(int, libvlc_audio_get_track, (libvlc_media_player_t * p_mi, libvlc_exception_t * p_e), (p_mi, p_e), return)

DEFINEFUNC(void, libvlc_audio_set_track, (libvlc_media_player_t * p_mi, int i_track, libvlc_exception_t * p_e), (p_mi, i_track, p_e), )

DEFINEFUNC(int, libvlc_audio_get_channel, (libvlc_instance_t * p_i, libvlc_exception_t * p_e), (p_i, p_e), return)

DEFINEFUNC(void, libvlc_audio_set_channel, (libvlc_instance_t * p_i, int i_channel, libvlc_exception_t * p_e), (p_i, i_channel, p_e), )

DEFINEFUNC(libvlc_media_discoverer_t *, libvlc_media_discoverer_new_from_name, (
				libvlc_instance_t * p_i,
				const char * psz_name,
				libvlc_exception_t * p_e),
				(p_i, psz_name, p_e), return)

DEFINEFUNC(void, libvlc_media_discoverer_release, (libvlc_media_discoverer_t * p_md), (p_md), )

DEFINEFUNC(char *, libvlc_media_discoverer_localized_name, (libvlc_media_discoverer_t * p_md), (p_md), return)

DEFINEFUNC(libvlc_media_list_t *, libvlc_media_discoverer_media_list, (libvlc_media_discoverer_t * p_md), (p_md), return)

DEFINEFUNC(libvlc_event_manager_t *, libvlc_media_discoverer_event_manager, (libvlc_media_discoverer_t * p_md), (p_md), return)

DEFINEFUNC(int, libvlc_media_discoverer_is_running, (libvlc_media_discoverer_t * p_md), (p_md), return)

DEFINEFUNC(void, libvlc_vlm_add_broadcast, (
					libvlc_instance_t * p_i, char * psz_name, char * psz_input, char * psz_output,
					int i_options, char ** ppsz_options, int b_enabled, int b_loop, libvlc_exception_t * p_e),
					(p_i, psz_name, psz_input, psz_output, i_options, ppsz_options, b_enabled, b_loop, p_e), )

DEFINEFUNC(void, libvlc_vlm_del_media, (libvlc_instance_t * p_i, char * psz_name, libvlc_exception_t * p_e), (p_i, psz_name, p_e), )

DEFINEFUNC(void, libvlc_vlm_set_enabled, (
					libvlc_instance_t * p_i, char * psz_name, int b_enabled,
					libvlc_exception_t * p_e), (p_i, psz_name, b_enabled, p_e), )

DEFINEFUNC(void, libvlc_vlm_set_output, (
					libvlc_instance_t * p_i, char * psz_name, char * psz_output,
					libvlc_exception_t * p_e), (p_i, psz_name, psz_output, p_e), )

DEFINEFUNC(void, libvlc_vlm_set_input, (
					libvlc_instance_t * p_i, char * psz_name, char * psz_input,
					libvlc_exception_t * p_e), (p_i, psz_name, psz_input, p_e), )

DEFINEFUNC(void, libvlc_vlm_add_input, (
					libvlc_instance_t * p_i, char * psz_name, char * psz_input,
					libvlc_exception_t * p_e), (p_i, psz_name, psz_input, p_e), )

DEFINEFUNC(void, libvlc_vlm_set_loop, (
					libvlc_instance_t * p_i, char * psz_name, int b_loop,
					libvlc_exception_t * p_e), (p_i, psz_name, b_loop, p_e), )

DEFINEFUNC(void, libvlc_vlm_change_media, (
					libvlc_instance_t * p_i, char * psz_name, char * psz_input, char * psz_output,
					int i_options, char ** ppsz_options, int b_enabled, int b_loop, libvlc_exception_t * p_e),
					(p_i, psz_name, psz_input, psz_output, i_options, ppsz_options, b_enabled, b_loop, p_e), )

DEFINEFUNC(void, libvlc_vlm_play_media, (libvlc_instance_t * p_i, char * psz_name, libvlc_exception_t * p_e), (p_i, psz_name, p_e), )

DEFINEFUNC(void, libvlc_vlm_stop_media, (libvlc_instance_t * p_i, char * psz_name, libvlc_exception_t * p_e), (p_i, psz_name, p_e), )

DEFINEFUNC(void, libvlc_vlm_pause_media, (libvlc_instance_t * p_i, char * psz_name, libvlc_exception_t * p_e), (p_i, psz_name, p_e), )

DEFINEFUNC(void, libvlc_vlm_seek_media, (
					libvlc_instance_t * p_i, char * psz_name,
					float f_percentage, libvlc_exception_t * p_e),
					(p_i, psz_name, f_percentage, p_e), )

DEFINEFUNC(char *, libvlc_vlm_show_media, (libvlc_instance_t * p_i, char * psz_name, libvlc_exception_t * p_e), (p_i, psz_name, p_e), return)

DEFINEFUNC(unsigned, libvlc_get_log_verbosity, (
						const libvlc_instance_t * p_i,
						libvlc_exception_t * p_e),
						(p_i, p_e), return)

DEFINEFUNC(void, libvlc_set_log_verbosity, (
					libvlc_instance_t * p_i, unsigned level,
					libvlc_exception_t * p_e),
					(p_i, level, p_e), )

DEFINEFUNC(libvlc_log_t *, libvlc_log_open, (libvlc_instance_t * p_i, libvlc_exception_t * p_e), (p_i, p_e), return)

DEFINEFUNC(void, libvlc_log_close, (libvlc_log_t * p_log, libvlc_exception_t * p_e), (p_log, p_e), )

DEFINEFUNC(unsigned, libvlc_log_count, (const libvlc_log_t * p_log, libvlc_exception_t * p_e), (p_log, p_e), return)

DEFINEFUNC(void, libvlc_log_clear, (libvlc_log_t * p_log, libvlc_exception_t * p_e), (p_log, p_e), )

DEFINEFUNC(libvlc_log_iterator_t *, libvlc_log_get_iterator, (const libvlc_log_t * p_log, libvlc_exception_t * p_e), (p_log, p_e), return)

DEFINEFUNC(void, libvlc_log_iterator_free, (libvlc_log_iterator_t * p_iter, libvlc_exception_t * p_e), (p_iter, p_e), )

DEFINEFUNC(int, libvlc_log_iterator_has_next, (const libvlc_log_iterator_t * p_iter, libvlc_exception_t * p_e), (p_iter, p_e), return)

DEFINEFUNC(libvlc_log_message_t *, libvlc_log_iterator_next, (
							libvlc_log_iterator_t * p_iter,
							struct libvlc_log_message_t * buffer,
							libvlc_exception_t * p_e),
							(p_iter, buffer, p_e), return)

DEFINEFUNC(void, libvlc_event_attach, (
					libvlc_event_manager_t * p_event_manager,
					libvlc_event_type_t i_event_type,
					libvlc_callback_t f_callback,
					void * p_user_data,
					libvlc_exception_t * p_e),
					(p_event_manager, i_event_type, f_callback, p_user_data, p_e), )

DEFINEFUNC(void, libvlc_event_detach, (
					libvlc_event_manager_t * p_event_manager,
					libvlc_event_type_t i_event_type,
					libvlc_callback_t f_callback,
					void * p_user_data,
					libvlc_exception_t * p_e),
					(p_event_manager, i_event_type, f_callback, p_user_data, p_e), )

DEFINEFUNC(const char *, libvlc_event_type_name, (
						libvlc_event_type_t i_event_type),
						(i_event_type), return)

#endif	//VLC_SYMBOLS_H
