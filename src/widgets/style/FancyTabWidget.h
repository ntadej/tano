/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2013 Tadej Novak <tadej@tano.si>
*
* This file is part of Qt Creator.
* Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
* Contact: http://www.qt-project.org/legal
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#ifndef TANO_FANCYTABWIDGET_H_
#define TANO_FANCYTABWIDGET_H_

#include <QIcon>
#include <QTimer>
#include <QPropertyAnimation>
#include <QWidget>

class QPainter;
class QStackedLayout;
class QStatusBar;

class MiniSplitter;

class FancyTab : public QObject
{
Q_OBJECT
Q_PROPERTY(float fader READ fader WRITE setFader)
public:
    FancyTab(QWidget *tabbar) : enabled(false), tabbar(tabbar), _fader(0) {
        animator.setPropertyName("fader");
        animator.setTargetObject(this);
    }
    float fader() { return _fader; }
    void setFader(float value);

    void fadeIn();
    void fadeOut();

    QIcon icon;
    QString text;
    QString toolTip;
    bool enabled;

private:
    QPropertyAnimation animator;
    QWidget *tabbar;
    float _fader;
};

class FancyTabBar : public QWidget
{
Q_OBJECT
public:
    FancyTabBar(QWidget *parent = 0);
    ~FancyTabBar();

    bool event(QEvent *event);

    void paintEvent(QPaintEvent *event);
    void paintTab(QPainter *painter,
                  int tabIndex) const;
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    bool validIndex(int index) const { return index >= 0 && index < _tabs.count(); }

    QSize sizeHint() const;
    QSize minimumSizeHint() const;

    void setTabEnabled(int index,
                       bool enable);
    bool isTabEnabled(int index) const;

    void insertTab(int index,
                   const QIcon &icon,
                   const QString &label) {
        FancyTab *tab = new FancyTab(this);
        tab->icon = icon;
        tab->text = label;
        _tabs.insert(index, tab);
    }
    void setEnabled(int index, bool enabled);
    void removeTab(int index) {
        FancyTab *tab = _tabs.takeAt(index);
        delete tab;
    }
    void setCurrentIndex(int index);
    int currentIndex() const { return _currentIndex; }

    void setTabToolTip(int index, QString toolTip) { _tabs[index]->toolTip = toolTip; }
    QString tabToolTip(int index) const { return _tabs.at(index)->toolTip; }

    QIcon tabIcon(int index) const { return _tabs.at(index)->icon; }
    QString tabText(int index) const { return _tabs.at(index)->text; }
    int count() const {return _tabs.count(); }
    QRect tabRect(int index) const;

signals:
    void currentChanged(int);

public slots:
    void emitCurrentIndex();

private:
    static const int _rounding;
    static const int _textPadding;
    QRect _hoverRect;
    int _hoverIndex;
    int _currentIndex;
    QList<FancyTab*> _tabs;
    QTimer _triggerTimer;
    QSize tabSizeHint(bool minimum = false) const;

};

class FancyTabWidget : public QWidget
{
Q_OBJECT
public:
    FancyTabWidget(QWidget *parent = 0);

    void addStaticWidget(QWidget *widget);
    void insertTab(int index, QWidget *tab,
                   const QIcon &icon,
                   const QString &label);
    void removeTab(int index);
    void setBackgroundBrush(const QBrush &brush);
    void addCornerWidget(QWidget *widget);
    void insertCornerWidget(int pos,
                            QWidget *widget);
    int cornerWidgetCount() const;
    void setTabToolTip(int index,
                       const QString &toolTip);

    void paintEvent(QPaintEvent *event);

    int currentIndex() const;
    QStatusBar *statusBar() const;

    void setTabEnabled(int index,
                       bool enable);
    bool isTabEnabled(int index) const;

    bool isSelectionWidgetVisible() const;

signals:
    void currentAboutToShow(int index);
    void currentChanged(int index);

public slots:
    void setCurrentIndex(int index);
    void setLiteMode(bool enabled);
    void setSelectionWidgetVisible(bool visible);

private slots:
    void showWidget(int index);

private:
    FancyTabBar *_tabBar;
    QWidget *_containerWidget;
    QWidget *_cornerWidgetContainer;
    QStackedLayout *_modesStack;
    QWidget *_selectionWidget;
    MiniSplitter *_splitterWidget;
    QStatusBar *_statusBar;
};

#endif // TANO_FANCYTABWIDGET_H_
