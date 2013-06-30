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

#include "core/File.h"

#include "SettingsPlaylist.h"
#include "ui_SettingsPlaylist.h"

SettingsPlaylist::SettingsPlaylist(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::SettingsPlaylist)
{
    ui->setupUi(this);
    ui->playlist->setType(File::M3U);
}

SettingsPlaylist::~SettingsPlaylist()
{
    delete ui;
}

void SettingsPlaylist::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

QString SettingsPlaylist::playlist() const
{
    return ui->playlist->value();
}

void SettingsPlaylist::setPlaylist(const QString &playlist)
{
    ui->playlist->setValue(playlist);
}

bool SettingsPlaylist::playlistUpdate() const
{
    return ui->checkRefresh->isChecked();
}

void SettingsPlaylist::setPlaylistUpdate(bool enabled)
{
    ui->checkRefresh->setChecked(enabled);
}

QString SettingsPlaylist::playlistUpdateUrl() const
{
    return ui->editUrl->text();
}

void SettingsPlaylist::setPlaylistUpdateUrl(const QString &playlist)
{
    ui->editUrl->setText(playlist);
}

QString SettingsPlaylist::radioCategory() const
{
    return ui->editRadio->text();
}

void SettingsPlaylist::setRadioCategory(const QString &category)
{
    ui->editRadio->setText(category);
}

QString SettingsPlaylist::hdCategory() const
{
    return ui->editHd->text();
}

void SettingsPlaylist::setHdCategory(const QString &category)
{
    ui->editHd->setText(category);
}

bool SettingsPlaylist::udpxy() const
{
    return ui->checkUdpxy->isChecked();
}

void SettingsPlaylist::setUdpxy(bool enabled)
{
    ui->checkUdpxy->setChecked(enabled);
}

QString SettingsPlaylist::udpxyUrl() const
{
    return ui->editUdpxyUrl->text();
}

void SettingsPlaylist::setUdpxyUrl(const QString &url)
{
    ui->editUdpxyUrl->setText(url);
}

int SettingsPlaylist::udpxyPort() const
{
    return ui->editUdpxyPort->value();
}

void SettingsPlaylist::setUdpxyPort(int port)
{
    ui->editUdpxyPort->setValue(port);
}
