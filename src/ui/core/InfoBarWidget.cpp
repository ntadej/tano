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

#include <QtGui/QHBoxLayout>
#include <QtGui/QScrollBar>

#include "core/GetFile.h"
#include "InfoBarWidget.h"

InfoBarWidget::InfoBarWidget(QWidget *parent)
    : QScrollArea(parent),
      _direction(true),
      _image(new GetFile())
{
    _timer = new QTimer(this);

    QWidget *widget = new QWidget(this);
    _labelChannel = new QLabel(widget);
    _labelLanguage = new QLabel(widget);
    _labelLogo = new QLabel(widget);
    _labelNext = new QLabel(widget);
    _labelNow = new QLabel(widget);
    QHBoxLayout *layout = new QHBoxLayout;
    layout->setContentsMargins(4,0,4,0);
    layout->setSpacing(10);
    layout->addWidget(_labelLogo);
    layout->addWidget(_labelChannel);
    layout->addWidget(_labelLanguage);
    layout->addWidget(_labelNow);
    layout->addWidget(_labelNext);
    layout->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Fixed));
    widget->setLayout(layout);

    setWidget(widget);
    setWidgetResizable(true);
    setMaximumHeight(35);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    connect(_labelNow, SIGNAL(linkActivated(QString)), this, SIGNAL(open(QString)));
    connect(_labelNext, SIGNAL(linkActivated(QString)), this, SIGNAL(open(QString)));
    connect(_image, SIGNAL(file(QString)), this, SLOT(image(QString)));
    connect(_timer, SIGNAL(timeout()), this, SLOT(scroll()));

    _timer->start(50);
}

InfoBarWidget::~InfoBarWidget()
{
    delete _image;
    delete _timer;
}

void InfoBarWidget::scroll()
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

void InfoBarWidget::clear()
{
    _labelNow->setText("");
    _labelNext->setText("");
    _labelChannel->setText("");
    _labelLanguage->setText("");
    _labelLogo->clear();
    _labelLogo->hide();
}

void InfoBarWidget::setInfo(const QString &channel,
                            const QString &language)
{
    _labelChannel->setText("<b>" + channel + "</b>");
    if(!language.isEmpty() && !language.isNull())
        _labelLanguage->setText(tr("Language: %1").arg(language));
}

void InfoBarWidget::setEpg(const QString &now,
                           const QString &next)
{
    _labelNow->setText(tr("Now: %1").arg(now));
    _labelNext->setText(tr("Next: %1").arg(next));
}

void InfoBarWidget::setLogo(const QString &logo)
{
    if(logo.isEmpty())
        return;

    if(logo.contains("http"))
        _image->getFile(logo);
    else
        image(logo);
}

void InfoBarWidget::image(const QString &image)
{
    _labelLogo->setPixmap(QPixmap(image));
    _labelLogo->show();
}
