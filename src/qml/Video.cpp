#include <QDebug>

#include "core/Common.h"
#include "qml/Video.h"

VideoDisplay::VideoDisplay(QGraphicsItem* parent)
	: QGraphicsProxyWidget(parent)
{
	// This is the widget where the video will be drawn
	// The advantage of a QLabel over a QWidget is that
	// we can easily push a QPixmap into.
	video = new QLabel( );
	video->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
	setWidget(video);

	// Forget that for the moment ;-)
	connect( this, SIGNAL( frameReady( struct ctx* ) ),
			 this, SLOT( processNewFrame( struct ctx* ) ) );


	// Finally init libvlc
	initVLC();
	play();
}

void VideoDisplay::initVLC()
{
	// List of parameters used to initialize libvlc.
	// These arguments are same as those you can pass
	// the the VLC command line.
	char const* vlc_argv[] =
	{
		//"--verbose", "3",
		// Edit this line if libvlc can't locate your plugins directory
		//"--plugin-path", "/path/to/vlc",
	};
	int vlc_argc = sizeof(vlc_argv) / sizeof(*vlc_argv);

	// Create a libvlc instance
	m_vlcInstance = libvlc_new( vlc_argc, vlc_argv );
	// This for catching and printing exceptions
	// raised by libvlc
	catchException();
	// Create the mediaplayer used to play a media
	m_vlcMediaplayer = libvlc_media_player_new( m_vlcInstance );
	// Re-checking for exceptions
	catchException();

	// We're done with the initialization!
}

void VideoDisplay::play()
{
	// This method is called, when the play button is
	// clicked.


		// Show the open dialog
		mrl = "udp://@232.4.1.1:5002";
		if ( mrl.isEmpty() ) return;

		// Create a new media from the Media Resource Locator
		m_vlcMedia = libvlc_media_new_location( m_vlcInstance, mrl.toAscii() );
		// Catching exception
		catchException();

		// We now need a struct for storing the video buffer
		// and a mutex to protect it.
		// The structure will be given as an arguments for the
		// lock/unlock callbacks.
		struct ctx* context;
		// Allocating the space for the structure
		context = ( struct ctx* )malloc( sizeof( *context ) );
		// Allocating the video buffer
		context->pixels = ( uchar* )malloc( ( sizeof( *( context->pixels ) ) * VIDEO_WIDTH * VIDEO_HEIGHT ) * 4 );
		// Allocating the mutex
		context->mutex = new QMutex();
		context->mainWindow = this;


		// Put the media into the mediaplayer
		libvlc_media_player_set_media( m_vlcMediaplayer, m_vlcMedia );
		catchException();




	// Finally, start the playback.
	libvlc_video_set_callbacks(m_vlcMediaplayer, lock, unlock, display, context);
	libvlc_video_set_format(m_vlcMediaplayer, "RV32", VIDEO_WIDTH, VIDEO_HEIGHT, VIDEO_WIDTH*4);
	libvlc_media_player_play( m_vlcMediaplayer );
	catchException();
}

void *VideoDisplay::lock(void *data, void **p_pixels)
{
	struct ctx *ctx = (struct ctx*) data;
	// Lock the buffer (to avoid concurrent access and data corruption)
	ctx->mutex->lock();
	// Tell libvlc to write the next frame into our pre-allocated buffer
	*p_pixels = ctx->pixels;

	return NULL;
}

void VideoDisplay::unlock(void *data, void *id, void *const*p_pixels)
{
	struct ctx *ctx = (struct ctx*) data;
	// As we are in a static method we don't have an instance
	// of the VideoDisplay here. Fortunately we created a copy
	// of our instance pointer into the ctx structure,
	// do you remember ?
	ctx->mainWindow->emit frameReady( ctx );
	// That way we are able to emit a signal containing our
	// new video frame !
}

void VideoDisplay::display(void *data, void *id)
{
	/* VLC wants to display the video */
	(void) data;
	Q_ASSERT(id == NULL);
}

void VideoDisplay::processNewFrame( struct ctx* ctx )
{
	// WARNING:
	// This part of the code is extremely slow and inefficient
	// (but sufficient for our example).

	// The bottleneck starts here ;)

	// Conversion of the frame from our pixel buffer to a QImage
	QImage px( ctx->pixels, VIDEO_WIDTH, VIDEO_HEIGHT, QImage::Format_RGB32 );
	// Then we're creating a new QPixmap from our QImage
	QPixmap pix = QPixmap::fromImage( px );

	// Set the current frame into the video container
	video->setPixmap( pix );

	// Releasing the mutex for the upcoming frame.
	ctx->mutex->unlock();
}

void VideoDisplay::catchException()
{
	qDebug() << libvlc_errmsg();
}
