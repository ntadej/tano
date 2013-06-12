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

#ifndef TANO_FANCYLINEEDIT_H_
#define TANO_FANCYLINEEDIT_H_

#include <QLineEdit>

class FancyLineEditPrivate;

class FancyLineEdit : public QLineEdit
{
Q_OBJECT
Q_ENUMS(Side)
public:
    enum Side {Left = 0, Right = 1};

    explicit FancyLineEdit(QWidget *parent = 0);

    QPixmap buttonPixmap(Side side) const;
    void setButtonPixmap(Side side,
                         const QPixmap &pixmap);

    QMenu *buttonMenu(Side side) const;
    void setButtonMenu(Side side,
                       QMenu *menu);

    void setButtonVisible(Side side,
                          bool visible);
    bool isButtonVisible(Side side) const;

    void setButtonToolTip(Side side,
                          const QString &);
    void setButtonFocusPolicy(Side side,
                              Qt::FocusPolicy policy);

    // Set whether tabbing in will trigger the menu.
    void setMenuTabFocusTrigger(Side side,
                                bool v);
    bool hasMenuTabFocusTrigger(Side side) const;

    // Set if icon should be hidden when text is empty
    void setAutoHideButton(Side side,
                           bool h);
    bool hasAutoHideButton(Side side) const;

signals:
    void buttonClicked(FancyLineEdit::Side side);
    void leftButtonClicked();
    void rightButtonClicked();

private slots:
    void checkButtons(const QString &);
    void iconClicked();

protected:
    virtual void resizeEvent(QResizeEvent *e);

private:
    // Unimplemented, to force the user to make a decision on
    // whether to use setHistoryKey() or setSpecialCompleter().
    void setCompleter(QCompleter *);

    void updateMargins();
    void updateButtonPositions();
    friend class FancyLineEditPrivate;

    FancyLineEditPrivate *d;
    QString _oldText;
};

#endif // TANO_FANCYLINEEDIT_H_
