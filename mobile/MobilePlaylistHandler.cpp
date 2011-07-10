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

#include "MobilePlaylistHandler.h"

#include "container/Channel.h"
#include "core/Common.h"
#include "core/ListModel.h"
#include "core/LocaleManager.h"
#include "ui/playlist/PlaylistWidget.h"

MobilePlaylistHandler::MobilePlaylistHandler(QObject *parent)
    : QObject(parent)
{
    _playlist = new PlaylistWidget();
    _playlist->hide();
    openPlaylist();

    _model = new ListModel(new Channel, this);
    _model->appendRows(_items);
}

MobilePlaylistHandler::~MobilePlaylistHandler()
{
    delete _model;
    delete _playlist;
}

QVariantList MobilePlaylistHandler::categories()
{
    QVariantList list;
    for(int i = 0; i < _playlist->categories().size(); i++) {
        if(!_playlist->categories()[i].isEmpty())
            list << _playlist->categories()[i];
    }
    return list;
}

QVariantList MobilePlaylistHandler::languages()
{
    QVariantList list;
    for(int i = 0; i < _playlist->languages().size(); i++) {
        if(!_playlist->languages()[i].isEmpty())
            list << _playlist->languages()[i];
    }
    return list;
}

void MobilePlaylistHandler::openPlaylist()
{
    _channels.clear();
    _items.clear();

    _playlist->open(Tano::locateResource("playlists/sl/siol-mpeg4.m3u"));
    _channels = _playlist->channels();

    for(int i = 0; i < _channels.size(); i++) {
        _items << _channels[i];
    }
}

void MobilePlaylistHandler::processPlaylist()
{
    /*for(int i = 0; i < ui->treeWidget->topLevelItemCount(); i++) {
        ui->treeWidget->topLevelItem(i)->setHidden(false);
    }

    if(ui->categoryBox->currentText() != tr("All categories")) {
        for(int i = 0; i<ui->treeWidget->topLevelItemCount(); i++) {
            if(!ui->treeWidget->topLevelItem(i)->text(2).contains(ui->categoryBox->currentText(), Qt::CaseInsensitive)) {
                ui->treeWidget->topLevelItem(i)->setHidden(true);
            }
        }
    }

    if(ui->languageBox->currentText() != tr("All languages")) {
        for(int i = 0; i<ui->treeWidget->topLevelItemCount(); i++) {
            if(!ui->treeWidget->topLevelItem(i)->text(3).contains(ui->languageBox->currentText(), Qt::CaseInsensitive)) {
                ui->treeWidget->topLevelItem(i)->setHidden(true);
            }
        }
    }

    if(ui->searchEdit->text() != "") {
        for(int i  =0; i<ui->treeWidget->topLevelItemCount(); i++) {
            if(!ui->treeWidget->topLevelItem(i)->text(1).contains(ui->searchEdit->text(), Qt::CaseInsensitive)) {
                    ui->treeWidget->topLevelItem(i)->setHidden(true);
                }
        }
    }
*/
}
