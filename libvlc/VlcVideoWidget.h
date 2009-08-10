#ifndef VLCVIDEOWIDGET_H
#define VLCVIDEOWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QPoint>

class VlcVideoWidget : public QWidget
{
    Q_OBJECT

public:
    VlcVideoWidget(QWidget *parent = 0);
    ~VlcVideoWidget();

	WId getWinId();

protected:
	void mouseDoubleClickEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent *event);

signals:
	void full();
	void rightClick(QPoint);
	void wheel(bool);
	void mouseMove();
	void osd(bool);

public slots:
	void disableMove();
	void enableMove();
	void controlFull();

private slots:
	void hideMouse();

private:
	QWidget *widget;
	QTimer *timer;
	QPoint pos;
	bool move;
};

#endif // VLCVIDEOWIDGET_H
