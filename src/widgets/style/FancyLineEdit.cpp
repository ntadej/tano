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

#include "FancyLineEdit.h"
#include "IconButton.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QMenu>
#include <QStyle>

static void execMenuAtWidget(QMenu *menu, QWidget *widget)
{
    QPoint p;
    QRect screen = qApp->desktop()->availableGeometry(widget);
    QSize sh = menu->sizeHint();
    QRect rect = widget->rect();
    if (widget->isRightToLeft()) {
        if (widget->mapToGlobal(QPoint(0, rect.bottom())).y() + sh.height() <= screen.height())
            p = widget->mapToGlobal(rect.bottomRight());
        else
            p = widget->mapToGlobal(rect.topRight() - QPoint(0, sh.height()));
        p.rx() -= sh.width();
    } else {
        if (widget->mapToGlobal(QPoint(0, rect.bottom())).y() + sh.height() <= screen.height())
            p = widget->mapToGlobal(rect.bottomLeft());
        else
            p = widget->mapToGlobal(rect.topLeft() - QPoint(0, sh.height()));
    }
    p.rx() = qMax(screen.left(), qMin(p.x(), screen.right() - sh.width()));
    p.ry() += 1;

    menu->exec(p);
}

class FancyLineEditPrivate : public QObject
{
public:
    explicit FancyLineEditPrivate(FancyLineEdit *parent);

    virtual bool eventFilter(QObject *obj,
                             QEvent *event);

    FancyLineEdit  *_lineEdit;
    QPixmap _pixmap[2];
    QMenu *_menu[2];
    bool _menuTabFocusTrigger[2];
    IconButton *_iconbutton[2];
    bool _iconEnabled[2];
};

FancyLineEditPrivate::FancyLineEditPrivate(FancyLineEdit *parent)
    : QObject(parent),
      _lineEdit(parent)
{
    for (int i = 0; i < 2; ++i) {
        _menu[i] = 0;
        _menuTabFocusTrigger[i] = false;
        _iconbutton[i] = new IconButton(parent);
        _iconbutton[i]->installEventFilter(this);
        _iconbutton[i]->hide();
        _iconbutton[i]->setAutoHide(false);
        _iconEnabled[i] = false;
    }
}

bool FancyLineEditPrivate::eventFilter(QObject *obj,
                                       QEvent *event)
{
    int buttonIndex = -1;
    for (int i = 0; i < 2; ++i) {
        if (obj == _iconbutton[i]) {
            buttonIndex = i;
            break;
        }
    }
    if (buttonIndex == -1)
        return QObject::eventFilter(obj, event);
    switch (event->type()) {
    case QEvent::FocusIn:
        if (_menuTabFocusTrigger[buttonIndex] && _menu[buttonIndex]) {
            _lineEdit->setFocus();
            execMenuAtWidget(_menu[buttonIndex], _iconbutton[buttonIndex]);
            return true;
        }
    default:
        break;
    }
    return QObject::eventFilter(obj, event);
}

FancyLineEdit::FancyLineEdit(QWidget *parent)
    : QLineEdit(parent),
      d(new FancyLineEditPrivate(this))
{
    ensurePolished();
    updateMargins();

    connect(this, SIGNAL(textChanged(QString)), this, SLOT(checkButtons(QString)));
    connect(d->_iconbutton[Left], SIGNAL(clicked()), this, SLOT(iconClicked()));
    connect(d->_iconbutton[Right], SIGNAL(clicked()), this, SLOT(iconClicked()));
}

void FancyLineEdit::checkButtons(const QString &text)
{
    if (_oldText.isEmpty() || text.isEmpty()) {
        for (int i = 0; i < 2; ++i) {
            if (d->_iconbutton[i]->hasAutoHide())
                d->_iconbutton[i]->animateShow(!text.isEmpty());
        }
        _oldText = text;
    }
}

void FancyLineEdit::setButtonVisible(Side side,
                                     bool visible)
{
    d->_iconbutton[side]->setVisible(visible);
    d->_iconEnabled[side] = visible;
    updateMargins();
}

bool FancyLineEdit::isButtonVisible(Side side) const
{
    return d->_iconEnabled[side];
}

void FancyLineEdit::iconClicked()
{
    IconButton *button = qobject_cast<IconButton *>(sender());
    int index = -1;
    for (int i = 0; i < 2; ++i)
        if (d->_iconbutton[i] == button)
            index = i;
    if (index == -1)
        return;
    if (d->_menu[index]) {
        execMenuAtWidget(d->_menu[index], button);
    } else {
        emit buttonClicked((Side)index);
        if (index == Left)
            emit leftButtonClicked();
        else if (index == Right)
            emit rightButtonClicked();
    }
}

void FancyLineEdit::updateMargins()
{
    bool leftToRight = (layoutDirection() == Qt::LeftToRight);
    Side realLeft = (leftToRight ? Left : Right);
    Side realRight = (leftToRight ? Right : Left);

    int leftMargin = d->_iconbutton[realLeft]->pixmap().width() + 8;
    int rightMargin = d->_iconbutton[realRight]->pixmap().width() + 8;
    // Note KDE does not reserve space for the highlight color
    if (style()->inherits("OxygenStyle")) {
        leftMargin = qMax(24, leftMargin);
        rightMargin = qMax(24, rightMargin);
    }

    QMargins margins((d->_iconEnabled[realLeft] ? leftMargin : 0), 0,
                     (d->_iconEnabled[realRight] ? rightMargin : 0), 0);

    setTextMargins(margins);
}

void FancyLineEdit::updateButtonPositions()
{
    QRect contentRect = rect();
    for (int i = 0; i < 2; ++i) {
        Side iconpos = (Side)i;
        if (layoutDirection() == Qt::RightToLeft)
            iconpos = (iconpos == Left ? Right : Left);

        if (iconpos == FancyLineEdit::Right) {
            const int iconoffset = textMargins().right() + 4;
            d->_iconbutton[i]->setGeometry(contentRect.adjusted(width() - iconoffset, 0, 0, 0));
        } else {
            const int iconoffset = textMargins().left() + 4;
            d->_iconbutton[i]->setGeometry(contentRect.adjusted(0, 0, -width() + iconoffset, 0));
        }
    }
}

void FancyLineEdit::resizeEvent(QResizeEvent *)
{
    updateButtonPositions();
}

void FancyLineEdit::setButtonPixmap(Side side,
                                    const QPixmap &buttonPixmap)
{
    d->_iconbutton[side]->setPixmap(buttonPixmap);
    updateMargins();
    updateButtonPositions();
    update();
}

QPixmap FancyLineEdit::buttonPixmap(Side side) const
{
    return d->_pixmap[side];
}

void FancyLineEdit::setButtonMenu(Side side,
                                  QMenu *buttonMenu)
{
     d->_menu[side] = buttonMenu;
     d->_iconbutton[side]->setIconOpacity(1.0);
 }

QMenu *FancyLineEdit::buttonMenu(Side side) const
{
    return  d->_menu[side];
}

bool FancyLineEdit::hasMenuTabFocusTrigger(Side side) const
{
    return d->_menuTabFocusTrigger[side];
}

void FancyLineEdit::setMenuTabFocusTrigger(Side side, bool v)
{
    if (d->_menuTabFocusTrigger[side] == v)
        return;

    d->_menuTabFocusTrigger[side] = v;
    d->_iconbutton[side]->setFocusPolicy(v ? Qt::TabFocus : Qt::NoFocus);
}

bool FancyLineEdit::hasAutoHideButton(Side side) const
{
    return d->_iconbutton[side]->hasAutoHide();
}

void FancyLineEdit::setAutoHideButton(Side side,
                                      bool h)
{
    d->_iconbutton[side]->setAutoHide(h);
    if (h)
        d->_iconbutton[side]->setIconOpacity(text().isEmpty() ?  0.0 : 1.0);
    else
        d->_iconbutton[side]->setIconOpacity(1.0);
}

void FancyLineEdit::setButtonToolTip(Side side,
                                     const QString &tip)
{
    d->_iconbutton[side]->setToolTip(tip);
}

void FancyLineEdit::setButtonFocusPolicy(Side side,
                                         Qt::FocusPolicy policy)
{
    d->_iconbutton[side]->setFocusPolicy(policy);
}
