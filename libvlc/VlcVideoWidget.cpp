#include <QMouseEvent>
#include <QApplication>
#include <QDesktopWidget>
#include <QHBoxLayout>
#include <QDebug>

#include "VlcVideoWidget.h"

VlcVideoWidget::VlcVideoWidget(QWidget *parent)
    : QWidget(parent)
{
	setMouseTracking(true);
	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(hideMouse()));

	pos = QPoint();
	move = true;

	widget = new QWidget(this);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(widget);
    setLayout(layout);
}

VlcVideoWidget::~VlcVideoWidget()
{

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
		//emit mouseMove();
		pos = event->globalPos();

		if(event->globalPos().y() > h-105) {
			emit osd(false);
			timer->stop();
			qDebug() << "false move";
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
void VlcVideoWidget::disableMove()
{
	move = false;
}
void VlcVideoWidget::enableMove()
{
	move = true;
}

void VlcVideoWidget::controlFull()
{
	Qt::WindowFlags flags = windowFlags();
	if (!isFullScreen()) {
		//we only update that value if it is not already fullscreen
		//this->setWindowFlags(flags & (Qt::Window | Qt::SubWindow));
		flags |= Qt::Window;
		flags ^= Qt::SubWindow;
		setWindowFlags(flags);
#ifdef Q_WS_X11
		// This works around a bug with Compiz
		// as the window must be visible before we can set the state
		show();
		raise();
		setWindowState( windowState() | Qt::WindowFullScreen ); // set
#else
		setWindowState( windowState() | Qt::WindowFullScreen ); // set
		show();
#endif
	} else if (isFullScreen()) {
		flags ^= (Qt::Window | Qt::SubWindow); //clear the flags...
		//flags |= this->windowFlags(); //then we reset the flags (window and subwindow)
		setWindowFlags(flags);
		setWindowState( windowState()  ^ Qt::WindowFullScreen ); // reset
		qApp->setOverrideCursor(Qt::ArrowCursor);
		show();
	}
}
