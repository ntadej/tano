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

#include <QtGui/QStyledItemDelegate>

#include "MobilePlaylistHandler.h"

#include "container/Channel.h"
#include "core/Common.h"
#include "core/LocaleManager.h"
#include "playlist/PlaylistModel.h"

MobilePlaylistHandler::MobilePlaylistHandler(QObject *parent)
    : QObject(parent)
{
    _model = new PlaylistModel(this);
    openPlaylist();
}

MobilePlaylistHandler::~MobilePlaylistHandler()
{
    delete _model;
}

QVariantList MobilePlaylistHandler::categories()
{
    QVariantList list;
    for(int i = 0; i < _model->categories().size(); i++) {
        if(!_model->categories()[i].isEmpty())
            list << _model->categories()[i];
    }
    return list;
}

QVariantList MobilePlaylistHandler::languages()
{
    QVariantList list;
    for(int i = 0; i < _model->languages().size(); i++) {
        if(!_model->languages()[i].isEmpty())
            list << _model->languages()[i];
    }
    return list;
}

void MobilePlaylistHandler::openPlaylist()
{
    _model->openM3UFile(Tano::locateResource("playlists/sl/siol-mpeg4.m3u"));
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
