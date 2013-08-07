/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2013 Tadej Novak <tadej@tano.si>
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
#if QT_VERSION >= 0x050000
    #include <QtWidgets/QAction>
    #include <QtWidgets/QComboBox>
    #include <QtWidgets/QLabel>
    #include <QtWidgets/QListView>
    #include <QtWidgets/QMenu>
    #include <QtWidgets/QPushButton>
    #include <QtWidgets/QStackedWidget>
    #include <QtWidgets/QToolBar>
    #include <QtWidgets/QVBoxLayout>
#else
    #include <QtGui/QAction>
    #include <QtGui/QComboBox>
    #include <QtGui/QLabel>
    #include <QtGui/QListView>
    #include <QtGui/QMenu>
    #include <QtGui/QPushButton>
    #include <QtGui/QStackedWidget>
    #include <QtGui/QToolBar>
    #include <QtGui/QVBoxLayout>
#endif

#include "core/xmltv/XmltvCommon.h"
#include "core/xmltv/containers/XmltvChannel.h"
#include "core/xmltv/containers/XmltvProgramme.h"
#include "core/xmltv/models/XmltvProgrammeFilterModel.h"
#include "core/xmltv/models/XmltvProgrammeModel.h"

#include "common/PlaylistDisplayWidget.h"
#include "style/FancyLineEdit.h"
#include "style/StyledBar.h"

#include "ScheduleTab.h"

ScheduleTab::ScheduleTab(QWidget *parent)
    : QMainWindow(parent),
      _model(0)
{
    _filterModel = new XmltvProgrammeFilterModel(this);
    _filterModel->setDynamicSortFilter(true);

    _main = new QStackedWidget(this);
    setCentralWidget(_main);

    _view = new QListView(this);
    _view->setAlternatingRowColors(true);
    _view->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _view->setModel(_filterModel);
    _view->setContextMenuPolicy(Qt::CustomContextMenu);
    _main->addWidget(_view);

    _rightMenu = new QMenu(_view);
    _change = new QAction(QIcon::fromTheme("view-refresh"), tr("Change"), this);
    _info = new QAction(QIcon::fromTheme("x-office-calendar"), tr("Show information"), this);
    _record = new QAction(QIcon::fromTheme("media-record"), tr("Record"), this);
    _rightMenu->addAction(_info);
#if FEATURE_RECORDER
    _rightMenu->addAction(_record);
#endif

    _toolbarTop = new QToolBar(this);
    _toolbarTop->setMovable(false);
    _toolbarTop->setFloatable(false);
    _toolbarTop->setIconSize(QSize(16, 16));
    addToolBar(Qt::TopToolBarArea, _toolbarTop);

    _toolbarBottomSearch = new QToolBar(this);
    _toolbarBottomSearch->setMovable(false);
    _toolbarBottomSearch->setFloatable(false);
    _toolbarBottomSearch->setProperty("topBorder", true);
    addToolBar(Qt::BottomToolBarArea, _toolbarBottomSearch);
    insertToolBarBreak(_toolbarBottomSearch);

    _toolbarBottomType = new QToolBar(this);
    _toolbarBottomType->setMovable(false);
    _toolbarBottomType->setFloatable(false);
    _toolbarBottomType->setProperty("topBorder", true);
    addToolBar(Qt::BottomToolBarArea, _toolbarBottomType);
    insertToolBarBreak(_toolbarBottomType);

    _playlistWidget = new PlaylistDisplayWidget(this);
    _main->addWidget(_playlistWidget);

    _labelTitle = new QLabel(this);
    _labelTitle->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    _labelTitle->setAlignment(Qt::AlignCenter);
    _toolbarTop->addWidget(_labelTitle);
    _toolbarTop->addAction(_change);

    _selectDate = new QComboBox(this);
    _selectDate->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    _selectDate->setProperty("hideborder", true);
    _toolbarBottomType->addWidget(_selectDate);

    _search = new FancyLineEdit(this);
    _toolbarBottomSearch->addWidget(_search);

    _noEpg = new QWidget(this);
    _noEpgIcon = new QLabel(_noEpg);
    _noEpgIcon->setAlignment(Qt::AlignCenter);
    _noEpgIcon->setPixmap(QIcon::fromTheme("list-remove").pixmap(32));
    _noEpgLabel = new QLabel(_noEpg);
    _noEpgLabel->setWordWrap(true);
    _noEpgLabel->setAlignment(Qt::AlignCenter);
    _noEpgButton = new QPushButton(_noEpg);
    _noEpgButton->setIcon(QIcon::fromTheme("video-x-generic"));
    _noEpgButton->setIconSize(QSize(24, 24));

    QVBoxLayout *layout = new QVBoxLayout(_noEpg);
    layout->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Preferred, QSizePolicy::Expanding));
    layout->addWidget(_noEpgIcon);
    layout->addWidget(_noEpgLabel);
    layout->addWidget(_noEpgButton);
    layout->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Preferred, QSizePolicy::Expanding));
    _noEpg->setLayout(layout);
    _main->addWidget(_noEpg);
    reset();

    connect(_view, SIGNAL(activated(QModelIndex)), this, SLOT(programmeClicked(QModelIndex)));
    connect(_view, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showMenu(QPoint)));
    connect(_noEpgButton, SIGNAL(clicked()), this, SLOT(change()));
    connect(_change, SIGNAL(triggered()), this, SLOT(change()));
    connect(_info, SIGNAL(triggered()), this, SLOT(info()));
    connect(_record, SIGNAL(triggered()), this, SLOT(record()));
    connect(_selectDate, SIGNAL(currentIndexChanged(QString)), this, SLOT(processFilters()));
    connect(_search, SIGNAL(textChanged(QString)), this, SLOT(processFilters()));
    connect(_playlistWidget, SIGNAL(itemSelected(Channel *)), this, SLOT(channel(Channel *)));
}

ScheduleTab::~ScheduleTab() { }

void ScheduleTab::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type())
    {
    case QEvent::LanguageChange:
        _change->setText(tr("Change"));
        _info->setText(tr("Show information"));
        _record ->setText(tr("Record"));
        _noEpgLabel->setText("<h3>" + tr("Current channel selected doesn't have any schedule information") + "</h3>");
        _noEpgButton->setText(tr("Select channel"));
        break;
    default:
        break;
    }
}

void ScheduleTab::change()
{
    _toolbarBottomSearch->show();
    _toolbarBottomType->hide();
    _search->clear();

    _main->setCurrentWidget(_playlistWidget);
    _labelTitle->setText(tr("Select channel"));
}

void ScheduleTab::channel(Channel *channel)
{
    reset();

    emit requestEpg(channel->xmltvId());
    emit changeTo(this);
}

void ScheduleTab::info()
{
    programmeClicked(_view->indexAt(_currentPos));
}

void ScheduleTab::processFilters()
{
    if (_main->currentWidget() == _playlistWidget) {
        _playlistWidget->setFilters(_search->text());
    } else {
        QRegExp regExp(_search->text(), Qt::CaseInsensitive);
        _filterModel->setFilterRegExp(regExp);
        _filterModel->setDate(QDate::fromString(_selectDate->currentText(), Tano::Xmltv::dateFormatDisplay()));
    }
}

void ScheduleTab::programmeClicked(const QModelIndex &index)
{
    emit itemSelected(_model->value(_filterModel->mapToSource(index).row(), 0).toString());
}

void ScheduleTab::record()
{
    emit requestRecord(_model->value(_filterModel->mapToSource(_view->indexAt(_currentPos)).row(), 0).toString());
}

void ScheduleTab::reset()
{
    _main->setCurrentWidget(_noEpg);
    _labelTitle->setText(tr("No information"));

    _toolbarBottomSearch->hide();
    _toolbarBottomType->hide();
}

void ScheduleTab::setEpg(const QString &channel,
                         XmltvProgrammeModel *epg)
{
    if (_model)
        delete _model;

    _toolbarBottomSearch->show();
    _toolbarBottomType->show();
    _search->clear();

    _main->setCurrentWidget(_view);

    _model = epg;
    _filterModel->setProgrammeModel(_model);
    _labelTitle->setText(channel);

    QList<QDate> date;
    for (int i = 0; i < _model->rowCount(); i++) {
        QDate d = QDateTime::fromString(_model->value(i, 2).toString(), Tano::Xmltv::dateFormat()).date();
        if (!date.contains(d) && d >= QDate::currentDate()) {
            date << d;
        }
    }

    _selectDate->clear();
    foreach (const QDate &d, date) {
        _selectDate->addItem(d.toString(Tano::Xmltv::dateFormatDisplay()));
    }
}

void ScheduleTab::setPage(int id)
{
    _main->setCurrentIndex(id);
}

void ScheduleTab::showMenu(const QPoint &pos)
{
    _currentPos = pos;

    _rightMenu->exec(QCursor::pos());
}
