#include <QMouseEvent>
#include <QApplication>
#include <QDesktopWidget>
#include <QHBoxLayout>
#include <QDebug>

#include "VlcVideoWidget.h"
#include "VlcInstance.h"

VlcVideoWidget::VlcVideoWidget(QWidget *parent)
    : QWidget(parent)
{
	setMouseTracking(true);

	move = true;

	widget = new QWidget(this);
	widget->setMouseTracking(true);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(widget);
    setLayout(layout);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(hideMouse()));
}

VlcVideoWidget::~VlcVideoWidget()
{
	delete timer;
	delete widget;
}

WId VlcVideoWidget::getWinId()
{
	return widget->winId();
}

//Events:
void VlcVideoWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
	event->ignore();
	emit full();
}
void VlcVideoWidget::mouseMoveEvent(QMouseEvent *event)
{
	event->ignore();

	int h = QApplication::desktop()->height();

	if(this->isFullScreen() && move) {
		qApp->setOverrideCursor(Qt::ArrowCursor);
		pos = event->globalPos();

		if(event->globalPos().y() > h-115) {
			emit osd(false);
			timer->stop();
		} else {
			emit osd(true);
			timer->start(1000);
		}
	}
}
void VlcVideoWidget::mousePressEvent(QMouseEvent *event)
{
	event->ignore();

	if(event->button() == Qt::RightButton) {
		qApp->setOverrideCursor(Qt::ArrowCursor);
		emit rightClick(event->globalPos());
	}
}
void VlcVideoWidget::wheelEvent(QWheelEvent *event)
{
	event->ignore();

	if(event->delta()>0)
		emit wheel(true);
	else
		emit wheel(false);
}

void VlcVideoWidget::hideMouse()
{
	if(this->isFullScreen() && move) {
		qApp->setOverrideCursor(Qt::BlankCursor);
		timer->stop();
	}
}

//Move
void VlcVideoWidget::disableMove() { move = false; }
void VlcVideoWidget::enableMove() {	move = true; }

void VlcVideoWidget::controlFull()
{
	Qt::WindowFlags flags = windowFlags();
	if (!isFullScreen()) {
		flags |= Qt::Window;
		flags ^= Qt::SubWindow;
		setWindowFlags(flags);
#ifdef Q_WS_X11
		// This works around a bug with Compiz
		// as the window must be visible before we can set the state
		show();
		raise();
		setWindowState( windowState() | Qt::WindowFullScreen );
#else
		setWindowState( windowState() | Qt::WindowFullScreen );
		show();
#endif
	} else if (isFullScreen()) {
		flags ^= (Qt::Window | Qt::SubWindow);
		setWindowFlags(flags);
		setWindowState( windowState()  ^ Qt::WindowFullScreen );
		qApp->setOverrideCursor(Qt::ArrowCursor);
		show();
	}
}

void VlcVideoWidget::teletext()
{
	libvlc_toggle_teletext(_vlcCurrentMediaPlayer, _vlcException);
}
int VlcVideoWidget::teletextPage()
{
#if VLC_TRUNK
	return 0;
#else
	return libvlc_video_get_teletext(_vlcCurrentMediaPlayer, _vlcException);
#endif
}
void VlcVideoWidget::setTeletextPage(const int &page)
{
#if VLC_TRUNK

#else
	libvlc_video_set_teletext(_vlcCurrentMediaPlayer, page, _vlcException);
#endif
}

void VlcVideoWidget::setRatioOriginal()
{
	if (_vlcCurrentMediaPlayer) {
		libvlc_video_set_aspect_ratio(_vlcCurrentMediaPlayer, QString("").toAscii().data(), _vlcException);
	}
}
void VlcVideoWidget::setRatio1_1()
{
	if (_vlcCurrentMediaPlayer) {
		libvlc_video_set_aspect_ratio(_vlcCurrentMediaPlayer, QString("1:1").toAscii().data(), _vlcException);
	}
}
void VlcVideoWidget::setRatio4_3()
{
	if (_vlcCurrentMediaPlayer) {
		libvlc_video_set_aspect_ratio(_vlcCurrentMediaPlayer, QString("4:3").toAscii().data(), _vlcException);
	}
}
void VlcVideoWidget::setRatio16_9()
{
	if (_vlcCurrentMediaPlayer) {
		libvlc_video_set_aspect_ratio(_vlcCurrentMediaPlayer, QString("16:9").toAscii().data(), _vlcException);
	}
}
void VlcVideoWidget::setRatio16_10()
{
	if (_vlcCurrentMediaPlayer) {
		libvlc_video_set_aspect_ratio(_vlcCurrentMediaPlayer, QString("16:10").toAscii().data(), _vlcException);
	}
}
void VlcVideoWidget::setRatio2_21_1()
{
	if (_vlcCurrentMediaPlayer) {
		libvlc_video_set_aspect_ratio(_vlcCurrentMediaPlayer, QString("221:100").toAscii().data(), _vlcException);
	}
}
void VlcVideoWidget::setRatio5_4()
{
	if (_vlcCurrentMediaPlayer) {
		libvlc_video_set_aspect_ratio(_vlcCurrentMediaPlayer, QString("5:4").toAscii().data(), _vlcException);
	}
}

void VlcVideoWidget::setCropOriginal()
{
	if (_vlcCurrentMediaPlayer) {
		libvlc_video_set_crop_geometry(_vlcCurrentMediaPlayer, QString("").toAscii().data(), _vlcException);
	}
}
void VlcVideoWidget::setCrop16_9()
{
	if (_vlcCurrentMediaPlayer) {
		libvlc_video_set_crop_geometry(_vlcCurrentMediaPlayer, QString("16:9").toAscii().data(), _vlcException);
	}
}
void VlcVideoWidget::setCrop16_10()
{
	if (_vlcCurrentMediaPlayer) {
		libvlc_video_set_crop_geometry(_vlcCurrentMediaPlayer, QString("16:10").toAscii().data(), _vlcException);
	}
}
void VlcVideoWidget::setCrop1_85_1()
{
	if (_vlcCurrentMediaPlayer) {
		libvlc_video_set_crop_geometry(_vlcCurrentMediaPlayer, QString("185:100").toAscii().data(), _vlcException);
	}
}
void VlcVideoWidget::setCrop2_21_1()
{
	if (_vlcCurrentMediaPlayer) {
		libvlc_video_set_crop_geometry(_vlcCurrentMediaPlayer, QString("221:100").toAscii().data(), _vlcException);
	}
}
void VlcVideoWidget::setCrop2_35_1()
{
	if (_vlcCurrentMediaPlayer) {
		libvlc_video_set_crop_geometry(_vlcCurrentMediaPlayer, QString("235:100").toAscii().data(), _vlcException);
	}
}
void VlcVideoWidget::setCrop2_39_1()
{
	if (_vlcCurrentMediaPlayer) {
		libvlc_video_set_crop_geometry(_vlcCurrentMediaPlayer, QString("239:100").toAscii().data(), _vlcException);
	}
}
void VlcVideoWidget::setCrop5_4()
{
	if (_vlcCurrentMediaPlayer) {
		libvlc_video_set_crop_geometry(_vlcCurrentMediaPlayer, QString("5:4").toAscii().data(), _vlcException);
	}
}
void VlcVideoWidget::setCrop5_3()
{
	if (_vlcCurrentMediaPlayer) {
		libvlc_video_set_crop_geometry(_vlcCurrentMediaPlayer, QString("5:3").toAscii().data(), _vlcException);
	}
}
void VlcVideoWidget::setCrop4_3()
{
	if (_vlcCurrentMediaPlayer) {
		libvlc_video_set_crop_geometry(_vlcCurrentMediaPlayer, QString("4:3").toAscii().data(), _vlcException);
	}
}
void VlcVideoWidget::setCrop1_1()
{
	if (_vlcCurrentMediaPlayer) {
		libvlc_video_set_crop_geometry(_vlcCurrentMediaPlayer, QString("1:1").toAscii().data(), _vlcException);
	}
}

void VlcVideoWidget::setFilterDisabled()
{
	if (_vlcCurrentMediaPlayer) {
#if VLC_TRUNK
		libvlc_video_set_deinterlace(_vlcCurrentMediaPlayer, 0,	QString("discard").toAscii().data(), _vlcException);
		libvlc_video_set_deinterlace(_vlcCurrentMediaPlayer, 0,	QString("blend").toAscii().data(), _vlcException);
		libvlc_video_set_deinterlace(_vlcCurrentMediaPlayer, 0,	QString("mean").toAscii().data(), _vlcException);
		libvlc_video_set_deinterlace(_vlcCurrentMediaPlayer, 0,	QString("bob").toAscii().data(), _vlcException);
		libvlc_video_set_deinterlace(_vlcCurrentMediaPlayer, 0,	QString("linear").toAscii().data(), _vlcException);
		libvlc_video_set_deinterlace(_vlcCurrentMediaPlayer, 0,	QString("x").toAscii().data(), _vlcException);
#endif
	}
}
void VlcVideoWidget::setFilterDiscard()
{
	setFilterDisabled();

	if (_vlcCurrentMediaPlayer) {
#if VLC_TRUNK
		libvlc_video_set_deinterlace(_vlcCurrentMediaPlayer, 1,	QString("discard").toAscii().data(), _vlcException);
#endif
	}
}
void VlcVideoWidget::setFilterBlend()
{
	setFilterDisabled();

	if (_vlcCurrentMediaPlayer) {
#if VLC_TRUNK
		libvlc_video_set_deinterlace(_vlcCurrentMediaPlayer, 1,	QString("blend").toAscii().data(), _vlcException);
#endif
	}
}
void VlcVideoWidget::setFilterMean()
{
	setFilterDisabled();

	if (_vlcCurrentMediaPlayer) {
#if VLC_TRUNK
		libvlc_video_set_deinterlace(_vlcCurrentMediaPlayer, 1,	QString("mean").toAscii().data(), _vlcException);
#endif
	}
}
void VlcVideoWidget::setFilterBob()
{
	setFilterDisabled();

	if (_vlcCurrentMediaPlayer) {
#if VLC_TRUNK
		libvlc_video_set_deinterlace(_vlcCurrentMediaPlayer, 1,	QString("bob").toAscii().data(), _vlcException);
#endif
	}
}
void VlcVideoWidget::setFilterLinear()
{
	setFilterDisabled();

	if (_vlcCurrentMediaPlayer) {
#if VLC_TRUNK
		libvlc_video_set_deinterlace(_vlcCurrentMediaPlayer, 1,	QString("linear").toAscii().data(), _vlcException);
#endif
	}
}
void VlcVideoWidget::setFilterX()
{
	setFilterDisabled();

	if (_vlcCurrentMediaPlayer) {
#if VLC_TRUNK
		libvlc_video_set_deinterlace(_vlcCurrentMediaPlayer, 1,	QString("x").toAscii().data(), _vlcException);
#endif
	}
}
