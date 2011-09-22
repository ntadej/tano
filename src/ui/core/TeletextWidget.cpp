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

#include <vlc-qt/MediaPlayer.h>
#include <vlc-qt/Video.h>

#include "TeletextWidget.h"

TeletextWidget::TeletextWidget(QWidget *parent)
    : QWidget(parent),
      _backend(0)
{
    setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

    _button = new QPushButton(this);
    _button->setCheckable(true);
    _button->setChecked(false);
    _button->setIcon(QIcon(":/icons/48x48/teletext.png"));
    _button->setIconSize(QSize(48, 48));
    _button->setMaximumSize(QSize(48, 48));
    _button->setMinimumSize(QSize(48, 48));
    _button->setToolTip(tr("Teletext"));
    _button->setStatusTip(tr("Teletext"));

    _buttonTransparency = new QPushButton(this);
    _buttonTransparency->setVisible(false);
    _buttonTransparency->setCheckable(true);
    _buttonTransparency->setChecked(false);
    _buttonTransparency->setIcon(QIcon(":/icons/48x48/teletext-transparency.png"));
    _buttonTransparency->setIconSize(QSize(48, 48));
    _buttonTransparency->setMaximumSize(QSize(48, 48));
    _buttonTransparency->setMinimumSize(QSize(48, 48));
    _buttonTransparency->setToolTip(tr("Teletext Transparency"));
    _buttonTransparency->setStatusTip(tr("Teletext Transparency"));

    _page = new QSpinBox(this);
    _page->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    _page->setVisible(false);
    _page->setMinimumHeight(48);
    _page->setMinimum(100);
    _page->setMaximum(999);

    //connect(_button, SIGNAL(toggled(bool)), _buttonTransparency, SLOT(setVisible(bool)));
    connect(_button, SIGNAL(toggled(bool)), _page, SLOT(setVisible(bool)));
    connect(_button, SIGNAL(toggled(bool)), this, SLOT(teletext(bool)));
    connect(_buttonTransparency, SIGNAL(toggled(bool)), this, SLOT(teletextTransparency(bool)));
    connect(_page, SIGNAL(valueChanged(int)), this, SLOT(page(int)));

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(_button);
    layout->addWidget(_buttonTransparency);
    layout->addWidget(_page);
    layout->setSpacing(0);
    layout->setMargin(0);
    setLayout(layout);
}

TeletextWidget::~TeletextWidget()
{
    delete _button;
    delete _page;
}

void TeletextWidget::setBackend(VlcMediaPlayer *player)
{
    _backend = player->video();
}

void TeletextWidget::page(const int &p)
{
    if(_backend)
        _backend->setTeletextPage(p);
}

void TeletextWidget::teletext(const bool &on)
{
    if(on) {
        if(_backend) {
            _backend->setTeletextPage(100);
            _page->setValue(100);
        } else {
            _button->setChecked(false);
        }
    } else {
        if(_backend)
            _backend->setTeletextPage(99);
    }
}

void TeletextWidget::teletextTransparency(const bool &on)
{
    if(_backend)
        _backend->toggleTeletextTransparency();
}

void TeletextWidget::toggleTeletext(const bool &on)
{
    _button->setChecked(on);
}
