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

#include "FancyTabWidget.h"
#include "MiniSplitter.h"
#include "StyleHelper.h"
#include "StyledBar.h"

#include <QColorDialog>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QPainter>
#include <QStackedLayout>
#include <QStatusBar>
#include <QStyleFactory>
#include <QToolTip>
#include <QVBoxLayout>

const int FancyTabBar::_rounding = 22;
const int FancyTabBar::_textPadding = 4;

void FancyTab::fadeIn()
{
    animator.stop();
    animator.setDuration(80);
    animator.setEndValue(40);
    animator.start();
}

void FancyTab::fadeOut()
{
    animator.stop();
    animator.setDuration(160);
    animator.setEndValue(0);
    animator.start();
}

void FancyTab::setFader(float value)
{
    _fader = value;
    tabbar->update();
}

FancyTabBar::FancyTabBar(QWidget *parent)
    : QWidget(parent)
{
    _hoverIndex = -1;
    _currentIndex = -1;
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    setStyle(QStyleFactory::create(QLatin1String("windows")));
    setMinimumWidth(qMax(2 * _rounding, 40));
    setAttribute(Qt::WA_Hover, true);
    setFocusPolicy(Qt::NoFocus);
    setMouseTracking(true); // Needed for hover events
    _triggerTimer.setSingleShot(true);

    // We use a zerotimer to keep the sidebar responsive
    connect(&_triggerTimer, SIGNAL(timeout()), this, SLOT(emitCurrentIndex()));
}

FancyTabBar::~FancyTabBar()
{
    delete style();
}

QSize FancyTabBar::tabSizeHint(bool minimum) const
{
    QFont boldFont(font());
    boldFont.setPointSizeF(StyleHelper::sidebarFontSize());
    boldFont.setBold(true);
    QFontMetrics fm(boldFont);
    int spacing = 8;
    int width = 60 + spacing + 2;
    int maxLabelwidth = 0;
    for (int tab=0 ; tab<count() ;++tab) {
        int width = fm.width(tabText(tab));
        if (width > maxLabelwidth)
            maxLabelwidth = width;
    }
    int iconHeight = minimum ? 0 : 32;
    return QSize(qMax(width, maxLabelwidth + 4), iconHeight + spacing + fm.height());
}

void FancyTabBar::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter p(this);

    for (int i = 0; i < count(); ++i)
        if (i != currentIndex())
            paintTab(&p, i);

    // paint active tab last, since it overlaps the neighbors
    if (currentIndex() != -1)
        paintTab(&p, currentIndex());
}

// Handle hover events for mouse fade ins
void FancyTabBar::mouseMoveEvent(QMouseEvent *e)
{
    int newHover = -1;
    for (int i = 0; i < count(); ++i) {
        QRect area = tabRect(i);
        if (area.contains(e->pos())) {
            newHover = i;
            break;
        }
    }
    if (newHover == _hoverIndex)
        return;

    if (validIndex(_hoverIndex))
        _tabs[_hoverIndex]->fadeOut();

    _hoverIndex = newHover;

    if (validIndex(_hoverIndex)) {
        _tabs[_hoverIndex]->fadeIn();
        _hoverRect = tabRect(_hoverIndex);
    }
}

bool FancyTabBar::event(QEvent *event)
{
    if (event->type() == QEvent::ToolTip) {
        if (validIndex(_hoverIndex)) {
            QString tt = tabToolTip(_hoverIndex);
            if (!tt.isEmpty()) {
                QToolTip::showText(static_cast<QHelpEvent*>(event)->globalPos(), tt, this);
                return true;
            }
        }
    }
    return QWidget::event(event);
}

// Resets hover animation on mouse enter
void FancyTabBar::enterEvent(QEvent *e)
{
    Q_UNUSED(e)
    _hoverRect = QRect();
    _hoverIndex = -1;
}

// Resets hover animation on mouse enter
void FancyTabBar::leaveEvent(QEvent *e)
{
    Q_UNUSED(e)
    _hoverIndex = -1;
    _hoverRect = QRect();
    for (int i = 0 ; i < _tabs.count() ; ++i) {
        _tabs[i]->fadeOut();
    }
}

QSize FancyTabBar::sizeHint() const
{
    QSize sh = tabSizeHint();
    return QSize(sh.width(), sh.height() * _tabs.count());
}

QSize FancyTabBar::minimumSizeHint() const
{
    QSize sh = tabSizeHint(true);
    return QSize(sh.width(), sh.height() * _tabs.count());
}

QRect FancyTabBar::tabRect(int index) const
{
    QSize sh = tabSizeHint();

    if (sh.height() * _tabs.count() > height())
        sh.setHeight(height() / _tabs.count());

    return QRect(0, index * sh.height(), sh.width(), sh.height());

}

// This keeps the sidebar responsive since
// we get a repaint before loading the
// mode itself
void FancyTabBar::emitCurrentIndex()
{
    emit currentChanged(_currentIndex);
}

void FancyTabBar::mousePressEvent(QMouseEvent *e)
{
    e->accept();
    for (int index = 0; index < _tabs.count(); ++index) {
        if (tabRect(index).contains(e->pos())) {

            if (isTabEnabled(index)) {
                _currentIndex = index;
                update();
                _triggerTimer.start(0);
            }
            break;
        }
    }
}

void FancyTabBar::paintTab(QPainter *painter,
                           int tabIndex) const
{
    if (!validIndex(tabIndex)) {
        qWarning("invalid index");
        return;
    }
    painter->save();

    QRect rect = tabRect(tabIndex);
    bool selected = (tabIndex == _currentIndex);
    bool enabled = isTabEnabled(tabIndex);

    if (selected) {
        //background
        painter->save();
        QLinearGradient grad(rect.topLeft(), rect.topRight());
        grad.setColorAt(0, QColor(255, 255, 255, 140));
        grad.setColorAt(1, QColor(255, 255, 255, 210));
        painter->fillRect(rect.adjusted(0, 0, 0, -1), grad);
        painter->restore();

        //shadows
        painter->setPen(QColor(0, 0, 0, 110));
        painter->drawLine(rect.topLeft() + QPoint(1,-1), rect.topRight() - QPoint(0,1));
        painter->drawLine(rect.bottomLeft(), rect.bottomRight());
        painter->setPen(QColor(0, 0, 0, 40));
        painter->drawLine(rect.topLeft(), rect.bottomLeft());

        //highlights
        painter->setPen(QColor(255, 255, 255, 50));
        painter->drawLine(rect.topLeft() + QPoint(0, -2), rect.topRight() - QPoint(0,2));
        painter->drawLine(rect.bottomLeft() + QPoint(0, 1), rect.bottomRight() + QPoint(0,1));
        painter->setPen(QColor(255, 255, 255, 40));
        painter->drawLine(rect.topLeft() + QPoint(0, 0), rect.topRight());
        painter->drawLine(rect.topRight() + QPoint(0, 1), rect.bottomRight() - QPoint(0, 1));
        painter->drawLine(rect.bottomLeft() + QPoint(0,-1), rect.bottomRight()-QPoint(0,1));
    }

    QString tabText(this->tabText(tabIndex));
    QRect tabTextRect(rect);
    const bool drawIcon = rect.height() > 36;
    QRect tabIconRect(tabTextRect);
    tabTextRect.translate(0, drawIcon ? -2 : 1);
    QFont boldFont(painter->font());
    boldFont.setPointSizeF(StyleHelper::sidebarFontSize());
    boldFont.setBold(true);
    painter->setFont(boldFont);
    painter->setPen(selected ? QColor(255, 255, 255, 160) : QColor(0, 0, 0, 110));
    const int textFlags = Qt::AlignCenter | (drawIcon ? Qt::AlignBottom : Qt::AlignVCenter) | Qt::TextWordWrap;
    if (enabled) {
        painter->drawText(tabTextRect, textFlags, tabText);
        painter->setPen(selected ? QColor(60, 60, 60) : StyleHelper::panelTextColor());
    } else {
        painter->setPen(selected ? StyleHelper::panelTextColor() : QColor(255, 255, 255, 120));
    }

#if !defined(Q_OS_MAC)
    if (!selected && enabled) {
        painter->save();
        int fader = int(_tabs[tabIndex]->fader());
        QLinearGradient grad(rect.topLeft(), rect.topRight());
        grad.setColorAt(0, Qt::transparent);
        grad.setColorAt(0.5, QColor(255, 255, 255, fader));
        grad.setColorAt(1, Qt::transparent);
        painter->fillRect(rect, grad);
        painter->setPen(QPen(grad, 1.0));
        painter->drawLine(rect.topLeft(), rect.topRight());
        painter->drawLine(rect.bottomLeft(), rect.bottomRight());
        painter->restore();
    }
#endif

    if (!enabled)
        painter->setOpacity(0.7);

    if (drawIcon) {
        int textHeight = painter->fontMetrics().boundingRect(QRect(0, 0, width(), height()), Qt::TextWordWrap, tabText).height();
        tabIconRect.adjust(0, 4, 0, -textHeight);
        StyleHelper::drawIconWithShadow(tabIcon(tabIndex), tabIconRect, painter, enabled ? QIcon::Normal : QIcon::Disabled);
    }

    painter->translate(0, -1);
    painter->drawText(tabTextRect, textFlags, tabText);
    painter->restore();
}

void FancyTabBar::setCurrentIndex(int index)
{
    if (isTabEnabled(index)) {
        _currentIndex = index;
        update();
        emit currentChanged(_currentIndex);
    }
}

void FancyTabBar::setTabEnabled(int index,
                                bool enable)
{
    Q_ASSERT(index < _tabs.size());
    Q_ASSERT(index >= 0);

    if (index < _tabs.size() && index >= 0) {
        _tabs[index]->enabled = enable;
        update(tabRect(index));
    }
}

bool FancyTabBar::isTabEnabled(int index) const
{
    Q_ASSERT(index < _tabs.size());
    Q_ASSERT(index >= 0);

    if (index < _tabs.size() && index >= 0)
        return _tabs[index]->enabled;

    return false;
}

class FancyColorButton : public QWidget
{
public:
    FancyColorButton(QWidget *parent)
      : _parent(parent)
    {
        setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    }

    void mousePressEvent(QMouseEvent *ev)
    {
        if (ev->modifiers() & Qt::ShiftModifier) {
            QColor color = QColorDialog::getColor(StyleHelper::requestedBaseColor(), _parent);
            if (color.isValid())
                StyleHelper::setBaseColor(color);
        }
    }
private:
    QWidget *_parent;
};

FancyTabWidget::FancyTabWidget(QWidget *parent)
    : QWidget(parent)
{
    _tabBar = new FancyTabBar(this);

    _selectionWidget = new QWidget(this);
    QVBoxLayout *selectionLayout = new QVBoxLayout;
    selectionLayout->setSpacing(0);
    selectionLayout->setMargin(0);

    StyledBar *bar = new StyledBar;
    QHBoxLayout *layout = new QHBoxLayout(bar);
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->addWidget(new FancyColorButton(this));
    selectionLayout->addWidget(bar);

    selectionLayout->addWidget(_tabBar, 1);
    _selectionWidget->setLayout(selectionLayout);
    _selectionWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

    _cornerWidgetContainer = new QWidget(this);
    _cornerWidgetContainer->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    _cornerWidgetContainer->setAutoFillBackground(false);

    QVBoxLayout *cornerWidgetLayout = new QVBoxLayout;
    cornerWidgetLayout->setSpacing(0);
    cornerWidgetLayout->setMargin(0);
    cornerWidgetLayout->addStretch();
    _cornerWidgetContainer->setLayout(cornerWidgetLayout);

    selectionLayout->addWidget(_cornerWidgetContainer, 0);

    _containerWidget = new QWidget(this);
    _modesStack = new QStackedLayout;
    _splitterWidget = new MiniSplitter(this);
    _statusBar = new QStatusBar;
    _statusBar->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);

    _containerWidget->setLayout(_modesStack);
    _splitterWidget->addWidget(_containerWidget);

    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->setMargin(0);
    vlayout->setSpacing(0);
    vlayout->addWidget(_splitterWidget);
    vlayout->addWidget(_statusBar);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->setMargin(0);
    mainLayout->setSpacing(1);
    mainLayout->addWidget(_selectionWidget);
    mainLayout->addLayout(vlayout);
    setLayout(mainLayout);

    connect(_tabBar, SIGNAL(currentChanged(int)), this, SLOT(showWidget(int)));
}

void FancyTabWidget::addStaticWidget(QWidget *widget)
{
    _splitterWidget->addWidget(widget);
    _splitterWidget->setStretchFactor(1, 1);
    _splitterWidget->setStretchFactor(0, 0);
}

void FancyTabWidget::setSelectionWidgetVisible(bool visible)
{
    _selectionWidget->setVisible(visible);
}

bool FancyTabWidget::isSelectionWidgetVisible() const
{
    return _selectionWidget->isVisible();
}

void FancyTabWidget::insertTab(int index,
                               QWidget *tab,
                               const QIcon &icon,
                               const QString &label,
                               bool full)
{
    _modesStack->insertWidget(index, tab);
    _tabBar->insertTab(index, icon, label, full);
}

void FancyTabWidget::removeTab(int index)
{
    _modesStack->removeWidget(_modesStack->widget(index));
    _tabBar->removeTab(index);
}

void FancyTabWidget::renameTab(int index,
                               const QString &name)
{
    _tabBar->setTabText(index, name);
}

void FancyTabWidget::setBackgroundBrush(const QBrush &brush)
{
    QPalette pal = _tabBar->palette();
    pal.setBrush(QPalette::Mid, brush);
    _tabBar->setPalette(pal);
    pal = _cornerWidgetContainer->palette();
    pal.setBrush(QPalette::Mid, brush);
    _cornerWidgetContainer->setPalette(pal);
}

void FancyTabWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    if (_selectionWidget->isVisible()) {
        QPainter painter(this);

        QRect rect = _selectionWidget->rect().adjusted(0, 0, 1, 0);
        rect = style()->visualRect(layoutDirection(), geometry(), rect);
        StyleHelper::verticalGradient(&painter, rect, rect);
        painter.setPen(StyleHelper::borderColor());
        painter.drawLine(rect.topRight(), rect.bottomRight());

        QColor light = StyleHelper::sidebarHighlight();
        painter.setPen(light);
        painter.drawLine(rect.bottomLeft(), rect.bottomRight());
    }
}

void FancyTabWidget::insertCornerWidget(int pos,
                                        QWidget *widget)
{
    QVBoxLayout *layout = static_cast<QVBoxLayout *>(_cornerWidgetContainer->layout());
    layout->insertWidget(pos, widget);
}

int FancyTabWidget::cornerWidgetCount() const
{
    return _cornerWidgetContainer->layout()->count();
}

void FancyTabWidget::addCornerWidget(QWidget *widget)
{
    _cornerWidgetContainer->layout()->addWidget(widget);
}

int FancyTabWidget::currentIndex() const
{
    return _tabBar->currentIndex();
}

QWidget *FancyTabWidget::currentWidget() const
{
    return _modesStack->currentWidget();
}

QStatusBar *FancyTabWidget::statusBar() const
{
    return _statusBar;
}

void FancyTabWidget::setCurrentIndex(int index)
{
    if (_tabBar->isTabEnabled(index))
        _tabBar->setCurrentIndex(index);
}

void FancyTabWidget::setCurrentWidget(QWidget *widget)
{
    int index = -1;
    for (int i = 0; i < _modesStack->count(); i++) {
        if (_modesStack->widget(i) == widget) {
            index = i;
        }
    }

    if (_tabBar->isTabEnabled(index))
        _tabBar->setCurrentIndex(index);
}

void FancyTabWidget::setLiteMode(bool enabled)
{
    _selectionWidget->setHidden(enabled);
    _containerWidget->setHidden(enabled);
    _statusBar->setHidden(enabled);
}

void FancyTabWidget::showWidget(int index)
{
    emit currentAboutToShow(index);
    _modesStack->setCurrentIndex(index);
    if (_splitterWidget->count() > 1)
        _splitterWidget->widget(1)->setHidden(_tabBar->tabFull(index));
    emit currentChanged(index);
    emit currentChanged(_modesStack->widget(index));
}

void FancyTabWidget::setTabToolTip(int index,
                                   const QString &toolTip)
{
    _tabBar->setTabToolTip(index, toolTip);
}

void FancyTabWidget::setTabEnabled(int index,
                                   bool enable)
{
    _tabBar->setTabEnabled(index, enable);
}

bool FancyTabWidget::isTabEnabled(int index) const
{
    return _tabBar->isTabEnabled(index);
}
