#ifndef TANOQML_VIDEO_H_
#define TANOQML_VIDEO_H_

#include <QGraphicsProxyWidget>
#include <QLabel>
#include <QDebug>
#include <QMutex>

// libvlc include
#include <vlc/vlc.h>
#include <vlc/libvlc.h>

#define VIDEO_WIDTH     500
#define VIDEO_HEIGHT    450


#include <QtGui/QGraphicsProxyWidget>


class VideoDisplay : public QGraphicsProxyWidget
{
Q_OBJECT
//Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)

public:
	VideoDisplay(QGraphicsItem* parent = 0);

private slots:
	void                    play();
	void                    processNewFrame( struct ctx* );

private:
	void                    initVLC();
	void                    catchException();

	// GUI
	QLabel*                 video;
	QString                 mrl;

	// VLC stuff
	static void             *lock(void *data, void **p_pixels);
	static void             unlock(void *data, void *id, void *const *p_pixels);
	static void				display(void *data, void *id);
	libvlc_instance_t*      m_vlcInstance;
	libvlc_media_t*         m_vlcMedia;
	libvlc_media_player_t*  m_vlcMediaplayer;

signals:
	void                    frameReady( struct ctx* );
};

struct ctx
{
	uchar*                  pixels;
	QMutex*                 mutex;
	VideoDisplay*             mainWindow;
};

#endif // MAINWINDOW_H
