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

#include <QtCore/QEvent>

#include "common/widgets/FileDialogs.h"

#include "MenuTrackSubtitles.h"

MenuTrackSubtitles::MenuTrackSubtitles(QWidget *parent)
    : MenuCore(parent)
{
    setTitle(tr("Subtitles"));
    setIcon(QIcon::fromTheme("font-x-generic"));
    actionNext()->setText(tr("Next subtitles"));
    setType(Vlc::Subtitles);

    QAction *open = new QAction(tr("Open subtitle file"), this);
    connect(open, SIGNAL(triggered()), this, SLOT(openSubtitles()));
    addAction(open);
}

MenuTrackSubtitles::~MenuTrackSubtitles() { }

void MenuTrackSubtitles::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type())
    {
    case QEvent::LanguageChange:
        setTitle(tr("Subtitles"));
        actionNext()->setText(tr("Next subtitles"));
        actions()[0]->setText(tr("Open subtitle file"));
        break;
    default:
        break;
    }
}

void MenuTrackSubtitles::openSubtitles()
{
    QString file = FileDialogs::openSubtitles();

    if (file.isEmpty())
        return;

    emit subtitles(file);
}
