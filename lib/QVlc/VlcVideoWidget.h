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

	void setRatioOriginal();
	void setRatio1_1();
	void setRatio4_3();
	void setRatio16_9();
	void setRatio16_10();
	void setRatio2_21_1();
	void setRatio5_4();

	void setCropOriginal();
	void setCrop16_9();
	void setCrop16_10();
	void setCrop1_85_1();
	void setCrop2_21_1();
	void setCrop2_35_1();
	void setCrop2_39_1();
	void setCrop5_4();
	void setCrop5_3();
	void setCrop4_3();
	void setCrop1_1();

	void setFilterDisabled();
	void setFilterDiscard();
	void setFilterBlend();
	void setFilterMean();
	void setFilterBob();
	void setFilterLinear();
	void setFilterX();

private slots:
	void hideMouse();

private:
	QWidget *widget;
	QTimer *timer;
	QPoint pos;
	bool move;
};

#endif // VLCVIDEOWIDGET_H
