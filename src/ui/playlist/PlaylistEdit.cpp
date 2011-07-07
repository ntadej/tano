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

#include <QtCore/QDebug>
#include <QtGui/QCloseEvent>
#include <QtGui/QFileDialog>
#include <QtGui/QMenu>
#include <QtGui/QMessageBox>

#include "Config.h"
#include "container/Channel.h"
#include "container/File.h"
#include "core/Common.h"
#include "core/ConsoleOutput.h"
#include "core/Enums.h"
#include "core/Settings.h"
#include "ui/core/FileDialogs.h"
#include "ui/dialogs/AboutDialog.h"
#include "ui/dialogs/PrintDialog.h"
#include "ui/playlist/PlaylistExportTvheadend.h"
#include "ui/playlist/PlaylistImportCSV.h"
#include "ui/playlist/PlaylistImportWeb.h"

#if EDITOR
    #include "core/LocaleManager.h"
    #include "ui/dialogs/UpdateDialog.h"
    #include "ui/settings/SettingsEdit.h"
#endif

#if WITH_EDITOR_VLCQT
    #include <vlc-qt/Instance.h>
    #include <vlc-qt/MediaPlayer.h>
#endif

#include "PlaylistEdit.h"
#include "ui_PlaylistEdit.h"

PlaylistEdit::PlaylistEdit(const WId &video,
                           QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::PlaylistEdit),
      _closeEnabled(false)
{
#if EDITOR
    _locale = new LocaleManager();
#endif

    ui->setupUi(this);
    ui->editWidget->setEnabled(false);
    ui->playlist->editMode();

#if EDITOR
    _update = new UpdateDialog();
    _update->checkSilent();
#endif

	createSettings();
	createConnections();

#if WITH_EDITOR_VLCQT
    _instance = new VlcInstance(Tano::vlcQtArgs(), this);
    _player = new VlcMediaPlayer(video, this);
    _timer = new QTimer();
    connect(_player, SIGNAL(state(bool, bool, bool)), this, SLOT(setState(bool)));
    connect(_timer, SIGNAL(timeout()), this, SLOT(checkCurrentIp()));
#else
    ui->updateWidget->hide();
#endif

    _menuExport = new QMenu();
    _menuExport->addAction(ui->actionExportTvheadend);
    _menuExport->addAction(ui->actionExportXmltvId);

#if EDITOR
    ui->toolBar->insertAction(ui->actionClose, ui->actionAbout);
    ui->toolBar->insertAction(ui->actionAbout, ui->actionSettings);
#endif
}

PlaylistEdit::~PlaylistEdit()
{
    delete ui;

#if WITH_EDITOR_VLCQT
    delete _instance;
    delete _player;
    delete _timer;
#endif
}

void PlaylistEdit::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void PlaylistEdit::closeEvent(QCloseEvent *event)
{
    event->ignore();
    exit();
}

void PlaylistEdit::createSettings()
{
    Settings *settings = new Settings(this);
    ui->toolBar->setToolButtonStyle(Qt::ToolButtonStyle(settings->toolbarLook()));
    delete settings;
}

void PlaylistEdit::createConnections()
{
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(aboutTano()));
    connect(ui->actionSettings, SIGNAL(triggered()), this, SLOT(settings()));
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(open()));
    connect(ui->actionNew, SIGNAL(triggered()), this, SLOT(newPlaylist()));
    connect(ui->actionDelete, SIGNAL(triggered()), this, SLOT(deleteItem()));
    connect(ui->actionAdd, SIGNAL(triggered()), this, SLOT(addItem()));
    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(save()));
    connect(ui->actionClose, SIGNAL(triggered()), this, SLOT(exit()));
    connect(ui->actionExport, SIGNAL(triggered()), this, SLOT(menuOpenExport()));
    connect(ui->actionExportTvheadend, SIGNAL(triggered()), this, SLOT(exportTvheadend()));
    connect(ui->actionExportXmltvId, SIGNAL(triggered()), this, SLOT(exportXmltvId()));
    connect(ui->actionImportWeb, SIGNAL(triggered()), this, SLOT(importWeb()));
    connect(ui->actionPrint, SIGNAL(triggered()), this, SLOT(print()));

    connect(ui->editName, SIGNAL(textChanged(QString)), this, SLOT(setTitle(QString)));

    connect(ui->buttonApplyNum, SIGNAL(clicked()), this, SLOT(editChannelNumber()));
    connect(ui->editNumber, SIGNAL(returnPressed()), ui->buttonApplyNum, SLOT(click()));
    connect(ui->editChannelName, SIGNAL(textChanged(QString)), this, SLOT(editChannelName(QString)));
    connect(ui->editUrl, SIGNAL(textChanged(QString)), this, SLOT(editChannelUrl(QString)));
    connect(ui->editCategories, SIGNAL(textChanged(QString)), this, SLOT(editChannelCategories(QString)));
    connect(ui->editLanguage, SIGNAL(textChanged(QString)), this, SLOT(editChannelLanguage(QString)));
    connect(ui->editEpg, SIGNAL(textChanged(QString)), this, SLOT(editChannelEpg(QString)));
    connect(ui->editLogo, SIGNAL(textChanged(QString)), this, SLOT(editChannelLogo(QString)));

    connect(ui->actionUp, SIGNAL(triggered()), this, SLOT(moveUp()));
    connect(ui->actionDown, SIGNAL(triggered()), this, SLOT(moveDown()));

    connect(ui->playlist->treeWidget(), SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)), this, SLOT(editItem(QTreeWidgetItem*)));

#if EDITOR
    connect(_update, SIGNAL(newUpdate()), this, SLOT(updateAvailable()));
    connect(ui->actionUpdate, SIGNAL(triggered()), _update, SLOT(check()));
#endif

#if WITH_EDITOR_VLCQT
    connect(ui->buttonUpdate, SIGNAL(toggled(bool)), this, SLOT(refreshPlaylist(bool)));
#endif
}

void PlaylistEdit::menuOpenExport()
{
    _menuExport->exec(QCursor::pos());
}

void PlaylistEdit::setTitle(const QString &title)
{
    if(title.isEmpty())
        setWindowTitle(tr("Tano Editor"));
    else
        setWindowTitle(tr("%1 - Tano Editor").arg(title));
}


void PlaylistEdit::aboutTano()
{
    AboutDialog about(Tano::Editor, this);
    about.exec();
}

void PlaylistEdit::settings()
{
#if EDITOR
    SettingsEdit s(0, this);
    s.exec();
    _locale->setLocale();
    createSettings();
#endif
}

void PlaylistEdit::updateAvailable()
{
#if EDITOR
    ui->toolBar->insertAction(ui->actionAbout, ui->actionUpdate);
#endif
}

void PlaylistEdit::open(const QString &playlist,
                        const bool &refresh)
{
    File file;
    PlaylistImportCSV dialog;
    if(playlist.isNull()) {
        file = FileDialogs::openPlaylist();
    } else {
        file = File(playlist, FileDialogs::M3U);
    }

    ui->editWidget->setEnabled(false);

    switch (file.type())
    {
    case FileDialogs::M3U:
    case FileDialogs::JS:
    case FileDialogs::TanoOld:
        ui->playlist->open(file.path(), refresh, FileDialogs::Type(file.type()));
        break;
    case FileDialogs::CSV:
        dialog.exec();
        if(!dialog.proceed())
            return;

        ui->playlist->openCSV(file.path(), dialog.separator(), dialog.header(), dialog.columns());
        break;
    default:
        break;
    }

    ui->editName->setText(ui->playlist->name());
    ui->number->display(ui->playlist->treeWidget()->topLevelItemCount());
}

void PlaylistEdit::newPlaylist()
{
    if(ui->playlist->treeWidget()->topLevelItemCount() == 0)
        return;

    int ret;
    ret = QMessageBox::warning(this, tr("Playlist Editor"),
                                   tr("Do you want to create new playlist?\nYou will lose any unsaved changes."),
                                   QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
                                   QMessageBox::Discard);

    switch (ret)
    {
    case QMessageBox::Save:
        ui->actionSave->trigger();

        ui->editWidget->setEnabled(false);
        ui->playlist->clear();
        ui->editName->setText(tr("New playlist"));
        ui->number->display(ui->playlist->treeWidget()->topLevelItemCount());
        break;
    case QMessageBox::Discard:
        ui->editWidget->setEnabled(false);
        ui->playlist->clear();
        ui->editName->setText(tr("New playlist"));
        ui->number->display(ui->playlist->treeWidget()->topLevelItemCount());
        break;
    case QMessageBox::Cancel:
    default:
        break;
    }
}

void PlaylistEdit::deleteItem()
{
    ui->editNumber->setText("");
    ui->editChannelName->setText("");
    ui->editUrl->setText("");
    ui->editCategories->setText("");
    ui->editLanguage->setText("");
    ui->editEpg->setText("");
    ui->editLogo->setText("");

    ui->editWidget->setEnabled(false);

    ui->playlist->deleteItem();

    ui->number->display(ui->playlist->treeWidget()->topLevelItemCount());
}

void PlaylistEdit::addItem()
{
    editItem(ui->playlist->createItem());
    ui->number->display(ui->playlist->treeWidget()->topLevelItemCount());
}

void PlaylistEdit::addItem(const QString &name,
                           const QString &url)
{
    ui->playlist->createItem(name, url);
    ui->number->display(ui->playlist->treeWidget()->topLevelItemCount());
}

void PlaylistEdit::save()
{
    File file = FileDialogs::savePlaylist();

    if (file.path().isEmpty() || file.type() == -1)
        return;

    qDebug() << file.path() << file.type();

    switch (file.type())
    {
    case FileDialogs::M3U:
    case FileDialogs::M3UClean:
    case FileDialogs::CSV:
    case FileDialogs::JS:
        ui->playlist->save(file.path(), ui->editName->text(), FileDialogs::Type(file.type()));
        break;
    case FileDialogs::M3UUdpxy:
        int s;
        s = QMessageBox::warning(this, tr("Export to M3U format with Udpxy URLs"),
                                 tr("You need to have valid Udpxy settings or the exported playlist will contain classic URLs."),
                                 QMessageBox::Save | QMessageBox::Cancel,
                                 QMessageBox::Cancel);

        switch (s)
        {
        case QMessageBox::Save:
            ui->playlist->save(file.path(), ui->editName->text(), FileDialogs::Type(file.type()));
            break;
        case QMessageBox::Cancel:
        default:
            break;
        }
        break;
    default:
        break;
    }
}

void PlaylistEdit::exportTvheadend()
{
    PlaylistExportTvheadend dialog;
    dialog.exec();
    if(!dialog.proceed())
        return;

    ui->playlist->exportTvheadend(dialog.location(), dialog.interface(), dialog.xmltv());
}

void PlaylistEdit::exportXmltvId()
{
    QString fileName = FileDialogs::saveXmltv();

    if (fileName.isEmpty())
        return;

    ui->playlist->exportXmltvId(fileName);
}

void PlaylistEdit::importWeb()
{
    PlaylistImportWeb web;
    web.download();

    if(!web.playlist().isEmpty())
        open(web.playlist(), web.refresh());
}

void PlaylistEdit::exit()
{
    if(_closeEnabled) {
        hide();
#if EDITOR
        qApp->quit();
#endif
        return;
    }

    int ret;
    ret = QMessageBox::warning(this, tr("Playlist Editor"),
                                   tr("Do you want close the editor?\nYou will lose any unsaved changes."),
                                   QMessageBox::Save | QMessageBox::Close | QMessageBox::Cancel,
                                   QMessageBox::Close);

    switch (ret)
    {
    case QMessageBox::Save:
        ui->actionSave->trigger();
        break;
    case QMessageBox::Close:
        _closeEnabled = true;
        ui->actionClose->trigger();
        break;
    case QMessageBox::Cancel:
    default:
        break;
    }
}

void PlaylistEdit::print()
{
    PrintDialog dialog(ui->editName->text(), ui->playlist);
    dialog.exec();
}

void PlaylistEdit::refreshPlaylist(const bool &refresh)
{
#if WITH_EDITOR_VLCQT
    if(!refresh) {
        _timer->stop();
        ui->progressBar->setValue(1);
        ui->playlist->setEnabled(true);
    } else {
        ui->playlist->setEnabled(false);

        if(ConsoleOutput::debug())
            qDebug() << "Scanning:" << ui->ipFrom->text() << ui->ipPort->value() << ui->ipTimeout->value();

        QStringList ipFrom = ui->ipFrom->text().split(".");
        _currentIp[0] = ipFrom[0].toInt();
        _currentIp[1] = ipFrom[1].toInt();
        _currentIp[2] = ipFrom[2].toInt();
        _currentIp[3] = 1;

        _currentPort = ui->ipPort->value();
        _currentTimeout = ui->ipTimeout->value();

        checkIp();
    }
#endif
}

void PlaylistEdit::checkIp()
{
#if WITH_EDITOR_VLCQT
    ui->progressBar->setValue(_currentIp[3]);
    _player->open(currentIp());

    _timer->start(_currentTimeout);
#endif
}

void PlaylistEdit::checkCurrentIp()
{
#if WITH_EDITOR_VLCQT
    if(_currentIpPlaying) {
        _player->stop();

        bool newChannel = true;
        for(int i=0; i<ui->playlist->treeWidget()->topLevelItemCount(); i++) {
            if(ui->playlist->channelRead(ui->playlist->treeWidget()->topLevelItem(i))->url() == currentIp()) {
                newChannel = false;
                break;
            }
        }

        if(newChannel) {
            if(ConsoleOutput::debug())
                qDebug() << "Scanning:" << "Channel Found";
            addItem(tr("New channel from scan %1").arg(currentIp()), currentIp());
        }
    }

    if(_currentIp[3] != 255) {
        _currentIp[3]++;
        checkIp();
    } else {
        ui->buttonUpdate->setChecked(false);
    }
#endif
}

QString PlaylistEdit::currentIp()
{
#if WITH_EDITOR_VLCQT
    QString ip = "udp://@";
    ip.append(QString().number(_currentIp[0])+".");
    ip.append(QString().number(_currentIp[1])+".");
    ip.append(QString().number(_currentIp[2])+".");
    ip.append(QString().number(_currentIp[3])+":");
    ip.append(QString().number(_currentPort));

    return ip;
#endif
}

void PlaylistEdit::setState(const bool &playing)
{
#if WITH_EDITOR_VLCQT
    _currentIpPlaying = playing;
#endif
}

void PlaylistEdit::editItem(QTreeWidgetItem *item)
{
    if(item == 0) {
        ui->editWidget->setEnabled(false);
        return;
    }

    if(!ui->editWidget->isEnabled())
        ui->editWidget->setEnabled(true);

    ui->playlist->treeWidget()->setCurrentItem(item);

    ui->editNumber->setText(ui->playlist->channelRead(item)->numberString());
    ui->editChannelName->setText(ui->playlist->channelRead(item)->name());
    ui->editUrl->setText(ui->playlist->channelRead(item)->url());
    ui->editCategories->setText(ui->playlist->channelRead(item)->categories().join(","));
    ui->editLanguage->setText(ui->playlist->channelRead(item)->language());
    ui->editEpg->setText(ui->playlist->channelRead(item)->epg());
    ui->editLogo->setText(ui->playlist->channelRead(item)->logo());
}

void PlaylistEdit::editChannelNumber()
{
    QString text = ui->editNumber->text();
    if(text.toInt() != ui->playlist->channelRead(ui->playlist->treeWidget()->currentItem())->number())
        ui->editNumber->setText(QString().number(ui->playlist->processNum(ui->playlist->treeWidget()->currentItem(), text.toInt())));
    ui->playlist->treeWidget()->sortByColumn(0, Qt::AscendingOrder);
}

void PlaylistEdit::editChannelName(const QString &text)
{
    ui->playlist->channelRead(ui->playlist->treeWidget()->currentItem())->setName(text);
    ui->playlist->treeWidget()->currentItem()->setText(1, text);
}

void PlaylistEdit::editChannelUrl(const QString &text)
{
    ui->playlist->channelRead(ui->playlist->treeWidget()->currentItem())->setUrl(text);
}

void PlaylistEdit::editChannelCategories(const QString &text)
{
    ui->playlist->channelRead(ui->playlist->treeWidget()->currentItem())->setCategories(text.split(","));
}

void PlaylistEdit::editChannelLanguage(const QString &text)
{
    ui->playlist->channelRead(ui->playlist->treeWidget()->currentItem())->setLanguage(text);
}

void PlaylistEdit::editChannelEpg(const QString &text)
{
    ui->playlist->channelRead(ui->playlist->treeWidget()->currentItem())->setEpg(text);
}

void PlaylistEdit::editChannelLogo(const QString &text)
{
    ui->playlist->channelRead(ui->playlist->treeWidget()->currentItem())->setLogo(text);
}

void PlaylistEdit::moveUp()
{
    ui->playlist->moveUp(ui->playlist->treeWidget()->currentItem());
    ui->editNumber->setText(ui->playlist->channelRead(ui->playlist->treeWidget()->currentItem())->numberString());
}

void PlaylistEdit::moveDown()
{
    ui->playlist->moveDown(ui->playlist->treeWidget()->currentItem());
    ui->editNumber->setText(ui->playlist->channelRead(ui->playlist->treeWidget()->currentItem())->numberString());
}
