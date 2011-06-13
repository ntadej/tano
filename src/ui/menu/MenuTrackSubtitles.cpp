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

#include "MenuTrackSubtitles.h"

MenuTrackSubtitles::MenuTrackSubtitles(QWidget *parent)
    : MenuCore(parent)
{
    setTitle(tr("Subtitles"));
    setIcon(QIcon(":/icons/24x24/subtitle.png"));
    actionNext()->setText(tr("Next subtitles"));
    setType(Vlc::Subtitles);

    QAction *open = new QAction(tr("Open subtitle file"), this);
    connect(open, SIGNAL(triggered()), this, SLOT(openSubtitles()));
    addAction(open);
}

MenuTrackSubtitles::~MenuTrackSubtitles() { }

void MenuTrackSubtitles::openSubtitles()
{
    QString file = QFileDialog::getOpenFileName(this, tr("Open Subtitles file"),
                                                QDir::homePath(),
                                                tr("Subtitles files(*.sub *.srt *.txt)"));

    if (file.isEmpty())
        return;

    emit subtitles(file);
}
