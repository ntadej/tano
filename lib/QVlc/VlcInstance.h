#ifndef QVLC_VLCINSTANCE_H_
#define QVLC_VLCINSTANCE_H_

#include <QtCore/QObject>
#include <QtGui/QWidget>

#include <vlc/vlc.h>

extern libvlc_exception_t *_vlcException;
extern libvlc_instance_t *_vlcInstance;
extern libvlc_media_player_t *_vlcCurrentMediaPlayer;

class VlcInstance : public QObject {
Q_OBJECT
public:
	VlcInstance(QStringList args, WId widget = NULL);
	virtual ~VlcInstance();

	void openMedia(QString media);
	static void checkException();
	static QString version();

public slots:
	void init();
	void play();
	void pause();
	void stop();
	void mute();

private:
	int fatalError();
	void unloadMedia();

	libvlc_media_player_t * _vlcMediaPlayer;
	libvlc_media_t * _vlcMedia;
	WId _widgetId;
	QStringList _args;
};

#endif // QVLC_VLCINSTANCE_H_
