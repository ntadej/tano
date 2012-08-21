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

#if defined(Qt5)
    #include <QtWidgets/QAction>
    #include <QtWidgets/QMenu>
#elif defined(Qt4)
    #include <QtGui/QAction>
    #include <QtGui/QMenu>
#endif

#include "PlaylistDisplayWidget.h"
#include "ui_PlaylistDisplayWidget.h"

#include "playlist/PlaylistFilterModel.h"
#include "playlist/PlaylistModel.h"

PlaylistDisplayWidget::PlaylistDisplayWidget(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::PlaylistDisplayWidget),
      _current(0)
{
    ui->setupUi(this);
    ui->filters->hide();

    _filterModel = new PlaylistFilterModel(this);
    _filterModel->setDynamicSortFilter(true);

    ui->playlistView->setModel(_filterModel);
    ui->playlistView->setContextMenuPolicy(Qt::CustomContextMenu);

	_rightMenu = new QMenu(ui->playlistView);
	_play = new QAction(QIcon(":/icons/24x24/media-playback-start.png"), tr("Play"), this);
	_schedule = new QAction(QIcon(":/icons/24x24/calendar.png"), tr("Schedule"), this);
	_rightMenu->addAction(_play);
	_rightMenu->addAction(_schedule);

    connect(ui->playlistView, SIGNAL(activated(QModelIndex)), this, SLOT(channelSelected(QModelIndex)));
    connect(ui->filters, SIGNAL(filters(QString, QString, QString, QList<Channel::Type>)), this, SLOT(processFilters(QString, QString, QString, QList<Channel::Type>)));
    connect(_play, SIGNAL(triggered()), this, SLOT(play()));
    connect(_schedule, SIGNAL(triggered()), this, SLOT(schedule()));
}

PlaylistDisplayWidget::~PlaylistDisplayWidget()
{
    delete ui;
    delete _filterModel;
}

void PlaylistDisplayWidget::changeEvent(QEvent *e)
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

void PlaylistDisplayWidget::channelSelected(Channel *channel)
{
    _current = channel;
    emit itemSelected(_current);
    updateSelection(_current);
}

void PlaylistDisplayWidget::channelSelected(const QModelIndex &index)
{
    _current = _model->row(_filterModel->mapToSource(index).row());
    emit itemSelected(_current);
    updateSelection(_current);
}

void PlaylistDisplayWidget::channelSelected(const int &channel)
{
    if (!channel) {
        _current = 0;
        emit itemSelected(_current);
        return;
    }

    _current = _model->number(channel);
    updateSelection(_current);
}

void PlaylistDisplayWidget::channelSelected(const QString &xmltvId)
{
    _current = _model->xmltvId(xmltvId);
    emit itemSelected(_current);
    updateSelection(_current);
}

void PlaylistDisplayWidget::editMode()
{
    ui->filters->editMode();
    ui->filters->show();
}

PlaylistFilterWidget *PlaylistDisplayWidget::filter()
{
    return ui->filters;
}

void PlaylistDisplayWidget::play()
{
    channelSelected(ui->playlistView->indexAt(_currentPos));
}

void PlaylistDisplayWidget::playMode()
{
    connect(ui->playlistView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showMenu(QPoint)));
}

void PlaylistDisplayWidget::processFilters(const QString &search,
                                           const QString &category,
                                           const QString &language,
                                           const QList<Channel::Type> &types)
{
    QRegExp regExp(search, Qt::CaseInsensitive);
    _filterModel->setFilterRegExp(regExp);
    _filterModel->setCategory(category);
    _filterModel->setLanguage(language);
    _filterModel->setTypes(types);
}

void PlaylistDisplayWidget::refreshModel()
{
    ui->filters->refreshModel(_model->categories(), _model->languages());
}

void PlaylistDisplayWidget::schedule()
{
    emit scheduleRequested(_model->row(_filterModel->mapToSource(ui->playlistView->indexAt(_currentPos)).row()));
}

void PlaylistDisplayWidget::setModel(PlaylistModel *model)
{
    _model = model;
    _filterModel->setSourceModel(model);
}

void PlaylistDisplayWidget::showMenu(const QPoint &pos)
{
	_currentPos = pos;

	_rightMenu->exec(QCursor::pos());
}

void PlaylistDisplayWidget::updateSelection(Channel *channel)
{
    ui->playlistView->selectionModel()->select(_model->indexFromItem(channel), QItemSelectionModel::SelectCurrent);

    if (!visibleChannels().contains(channel))
        ui->playlistView->scrollTo(_model->indexFromItem(channel), QAbstractItemView::PositionAtTop);
}

QList<Channel *> PlaylistDisplayWidget::visibleChannels()
{
    QList<Channel *> list;

    QModelIndex index = ui->playlistView->indexAt(QPoint(0, 0));
    if (index.isValid()) {
        list << _model->row(_filterModel->mapToSource(index).row());
    }

    while (ui->playlistView->viewport()->rect().contains(QPoint(0, ui->playlistView->visualRect(index).y() + ui->playlistView->visualRect(index).height() + 1))) {
        index = ui->playlistView->indexAt(QPoint(0, ui->playlistView->visualRect(index).y() + ui->playlistView->visualRect(index).height() + 1));
        if (!index.isValid())
            break;
        list << _model->row(_filterModel->mapToSource(index).row());
    }

    return list;
}
