/**
 * Set the image hue, between 0 and 360. Defaults to 0.
 *
 * \param p_mediaplayer the media player
 * \param i_value new filter value
 * \param p_e an initialized exception pointer
 */
VLC_PUBLIC_API void libvlc_video_filter_set_hue( libvlc_media_player_t *, int, libvlc_exception_t * );

/**
 * Set the image contrast, between 0 and 2. Defaults to 1.
 *
 * \param p_mediaplayer the media player
 * \param i_value new filter value
 * \param p_e an initialized exception pointer
 */
VLC_PUBLIC_API void libvlc_video_filter_set_contrast( libvlc_media_player_t *, int, libvlc_exception_t * );

/**
 * Set the image saturation, between 0 and 3. Defaults to 1.
 *
 * \param p_mediaplayer the media player
 * \param i_value new filter value
 * \param p_e an initialized exception pointer
 */
VLC_PUBLIC_API void libvlc_video_filter_set_saturation( libvlc_media_player_t *, int, libvlc_exception_t * );

/**
 * Set the image brightness, between 0 and 2. Defaults to 1.
 *
 * \param p_mediaplayer the media player
 * \param i_value new filter value
 * \param p_e an initialized exception pointer
 */
VLC_PUBLIC_API void libvlc_video_filter_set_brightness( libvlc_media_player_t *, int, libvlc_exception_t * );
