/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2012 Tadej Novak <tadej@tano.si>
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

#include <QtGui/QHBoxLayout>
#include <QtGui/QScrollBar>

#include "ScrollingWidget.h"

ScrollingWidget::ScrollingWidget(QWidget *parent)
    : QScrollArea(parent),
      _direction(true)
{
    _timer = new QTimer(this);

    QWidget *widget = new QWidget(this);
    _label = new QLabel(widget);
    QHBoxLayout *layout = new QHBoxLayout;
    layout->setContentsMargins(4,0,4,0);
    layout->setSpacing(10);
    layout->addWidget(_label);
    layout->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Fixed));
    widget->setLayout(layout);

    setWidget(widget);
    setWidgetResizable(true);
    setMaximumHeight(35);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    connect(_label, SIGNAL(linkActivated(QString)), this, SIGNAL(open(QString)));
    connect(_timer, SIGNAL(timeout()), this, SLOT(scroll()));

    _timer->start(50);
}

ScrollingWidget::~ScrollingWidget()
{
    delete _timer;
}

void ScrollingWidget::removeBorder()
{
    setMaximumHeight(_label->height());
    setFrameStyle(QFrame::NoFrame);
}

void ScrollingWidget::scroll()
{
    if(horizontalScrollBar()->maximum() != 0) {
        if(_direction && horizontalScrollBar()->value() < horizontalScrollBar()->maximum()) {
            horizontalScrollBar()->setValue(horizontalScrollBar()->value()+1);
            if(horizontalScrollBar()->maximum() == horizontalScrollBar()->value())
                _direction = false;
        } else if(!_direction && horizontalScrollBar()->value() > horizontalScrollBar()->minimum()) {
            horizontalScrollBar()->setValue(horizontalScrollBar()->value()-1);
            if(horizontalScrollBar()->value() == 0)
                _direction = true;
        }
    }
}

void ScrollingWidget::setText(const QString &text)
{
    _label->setText(text);
}
