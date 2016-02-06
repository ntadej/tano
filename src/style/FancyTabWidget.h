/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2016 Tadej Novak <tadej@tano.si>
*
* Copyright (C) 2016 The Qt Company Ltd.
* Contact: https://www.qt.io/licensing/
*
* This file is based on file from Qt Creator.
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
#include <QWidget>

#include <QTimer>
#include <QPropertyAnimation>

class QPainter;
class QStackedLayout;
class QStatusBar;

class MiniSplitter;

class FancyTab : public QObject
{
    Q_OBJECT

    Q_PROPERTY(float fader READ fader WRITE setFader)
public:
    FancyTab(QWidget *tabbar) : enabled(false), full(false), tabbar(tabbar), m_fader(0) {
        animator.setPropertyName("fader");
        animator.setTargetObject(this);
    }
    float fader() { return m_fader; }
    void setFader(float value);

    void fadeIn();
    void fadeOut();

    QIcon icon;
    QString text;
    QString toolTip;
    bool enabled;
    bool full;

private:
    QPropertyAnimation animator;
    QWidget *tabbar;
    float m_fader;
};

class FancyTabBar : public QWidget
{
    Q_OBJECT

public:
    FancyTabBar(QWidget *parent = 0);
    ~FancyTabBar();

    bool event(QEvent *event);

    void paintEvent(QPaintEvent *event);
    void paintTab(QPainter *painter, int tabIndex) const;
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    bool validIndex(int index) const { return index >= 0 && index < m_tabs.count(); }

    QSize sizeHint() const;
    QSize minimumSizeHint() const;

    void setTabEnabled(int index, bool enable);
    bool isTabEnabled(int index) const;

    void insertTab(int index, const QIcon &icon, const QString &label, bool full = false) {
        FancyTab *tab = new FancyTab(this);
        tab->icon = icon;
        tab->text = label;
        tab->full = full;
        m_tabs.insert(index, tab);
        updateGeometry();
    }
    void setEnabled(int index, bool enabled);
    void removeTab(int index) {
        FancyTab *tab = m_tabs.takeAt(index);
        delete tab;
        updateGeometry();
    }
    void setCurrentIndex(int index);
    int currentIndex() const { return m_currentIndex; }

    void setTabToolTip(int index, QString toolTip) { m_tabs[index]->toolTip = toolTip; }
    QString tabToolTip(int index) const { return m_tabs.at(index)->toolTip; }

    bool tabFull(int index) const { return m_tabs.at(index)->full; }
    void setTabText(int index, const QString &text) const { m_tabs.at(index)->text = text; }
    QIcon tabIcon(int index) const { return m_tabs.at(index)->icon; }
    QString tabText(int index) const { return m_tabs.at(index)->text; }
    int count() const {return m_tabs.count(); }
    QRect tabRect(int index) const;

signals:
    void currentChanged(int);

public slots:
    void emitCurrentIndex();

private:
    static const int m_rounding;
    static const int m_textPadding;
    QRect m_hoverRect;
    int m_hoverIndex;
    int m_currentIndex;
    QList<FancyTab*> m_tabs;
    QTimer m_triggerTimer;
    QSize tabSizeHint(bool minimum = false) const;

};

class FancyTabWidget : public QWidget
{
    Q_OBJECT

public:
    FancyTabWidget(QWidget *parent = 0);

    void addStaticWidget(QWidget *widget);
    void insertTab(int index, QWidget *tab, const QIcon &icon, const QString &label, bool full = false);
    void removeTab(int index);
    void renameTab(int index, const QString &name);
    void setBackgroundBrush(const QBrush &brush);
    void addCornerWidget(QWidget *widget);
    void insertCornerWidget(int pos, QWidget *widget);
    int cornerWidgetCount() const;
    void setTabToolTip(int index, const QString &toolTip);

    void paintEvent(QPaintEvent *event);

    int tabCount() const;
    int currentIndex() const;
    QWidget *currentWidget() const;
    QStatusBar *statusBar() const;

    void setTabEnabled(int index, bool enable);
    bool isTabEnabled(int index) const;

    bool isSelectionWidgetVisible() const;

signals:
    void currentAboutToShow(int index);
    void currentChanged(int index);
    void currentChanged(QWidget *widget);

public slots:
    void setCurrentIndex(int index);
    void setCurrentWidget(QWidget *widget);
    void setLiteMode(bool enabled);
    void setSelectionWidgetVisible(bool visible);

private slots:
    void showWidget(int index);

private:
    FancyTabBar *m_tabBar;
    QWidget *m_containerWidget;
    QWidget *m_cornerWidgetContainer;
    QStackedLayout *m_modesStack;
    QWidget *m_selectionWidget;
    MiniSplitter *m_splitterWidget;
    QStatusBar *m_statusBar;
};

#endif // TANO_FANCYTABWIDGET_H_
