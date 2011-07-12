/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2011 Tadej Novak <tadej@tano.si>
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

#ifndef TANO_BROWSEWIDGET_H_
#define TANO_BROWSEWIDGET_H_

#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QToolButton>
#include <QtGui/QWidget>

#include "core/Enums.h"

class BrowseWidget : public QWidget
{
Q_OBJECT
public:
    BrowseWidget(QWidget *parent = 0);
    ~BrowseWidget();

    void setResetValue(const QString &value) { _resetValue = value; }
    void setType(const Tano::FileType &type) { _type = type; }
    void setValue(const QString &value);
    QString value() const;

private slots:
    void browse();
    void reset();

private:
    QString _resetValue;

    QLineEdit *_edit;
    QPushButton *_reset;
    QToolButton *_browse;

    Tano::FileType _type;
};

#endif // TANO_BROWSEWIDGET_H_
