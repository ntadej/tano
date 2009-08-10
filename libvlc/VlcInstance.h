#ifndef VLCINSTANCE_H_
#define VLCINSTANCE_H_

#include <QObject>
#include <vlc/vlc.h>

extern libvlc_exception_t *_vlcException;
extern libvlc_instance_t *_vlcInstance;
extern libvlc_media_player_t *_vlcMediaPlayer;

class VlcInstance : public QObject {
Q_OBJECT
public:
	VlcInstance();
	virtual ~VlcInstance();

	void openMedia(QString media);

public slots:
	void pause();
	void stop();

private:
	void playInternal();
	void unloadMedia();
	void checkException();
	bool _isPlaying;

	libvlc_media_player_t * _vlcMediaPlayer;
	libvlc_media_t * _vlcMedia;
};

#endif /* VLCINSTANCE_H_ */
