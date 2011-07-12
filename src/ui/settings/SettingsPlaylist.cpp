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

#include <QtGui/QFileDialog>

#include "SettingsPlaylist.h"
#include "ui_SettingsPlaylist.h"

#include "core/Enums.h"
#include "ui/playlist/PlaylistImportWeb.h"
#include "ui/playlist/PlaylistSelect.h"

SettingsPlaylist::SettingsPlaylist(QWidget *parent)
    : QWidget(parent),
    ui(new Ui::SettingsPlaylist)
{
    ui->setupUi(this);
    ui->browseCustom->setType(Tano::M3U);
    ui->select->open("playlists/playlists.xml");

    connect(ui->buttonDownload, SIGNAL(clicked()), this, SLOT(playlistDownload()));
}

SettingsPlaylist::~SettingsPlaylist()
{
    delete ui;
}

void SettingsPlaylist::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
            break;
        default:
            break;
    }
}

QString SettingsPlaylist::playlist() const
{
    if(ui->radioCustom->isChecked())
        return ui->browseCustom->value();
    else
        return ui->select->playlist();
}

void SettingsPlaylist::setPlaylist(const QString &playlist)
{
    if(!ui->select->setPlaylist(playlist)) {
        ui->radioCustom->setChecked(true);
        ui->browseCustom->setValue(playlist);
    }
}

void SettingsPlaylist::playlistDownload()
{
    PlaylistImportWeb web;
    web.save();

    if(web.playlist().isEmpty())
        return;

    ui->radioCustom->setChecked(true);
    ui->browseCustom->setValue(web.playlist());
}
