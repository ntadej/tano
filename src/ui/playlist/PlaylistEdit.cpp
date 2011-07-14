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
#include "container/core/Channel.h"
#include "container/core/File.h"
#include "container/playlist/CSVInfo.h"
#include "core/Common.h"
#include "core/ConsoleOutput.h"
#include "core/Enums.h"
#include "core/Settings.h"
#include "playlist/PlaylistModel.h"
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

    _model = new PlaylistModel(this);
    ui->playlist->setModel(_model);

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

    connect(ui->playlist, SIGNAL(itemClicked(Channel *)), this, SLOT(editItem(Channel *)));

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
        file = File(playlist, Tano::M3U);
    }

    if(file.path().isEmpty() || file.type() == -1)
        return;

    ui->editWidget->setEnabled(false);

    QFile f(file.path());
    if (!f.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Tano"),
                            tr("Cannot read file %1:\n%2.")
                            .arg(file.path())
                            .arg(f.errorString()));
        return;
    }
    f.close();

    switch (file.type())
    {
    case Tano::M3U:
    case Tano::JS:
    case Tano::TanoOld:
        _model->open(file.path(), refresh, Tano::FileType(file.type()));
        break;
    case Tano::CSV:
        dialog.exec();
        if(!dialog.proceed())
            return;

        _model->open(file.path(), refresh, Tano::FileType(file.type()), CSVInfo(dialog.separator(), dialog.header(), dialog.columns()));
        break;
    default:
        break;
    }

    ui->editName->setText(_model->name());
    ui->number->display(_model->rowCount());
}

void PlaylistEdit::newPlaylist()
{
    if(_model->rowCount() == 0)
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
    case QMessageBox::Discard:
        ui->editWidget->setEnabled(false);
        _model->clear();
        ui->editName->setText(tr("New playlist"));
        ui->number->display(_model->rowCount());
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

    _model->deleteChannel(ui->playlist->currentChannel());

    ui->number->display(_model->rowCount());
}

void PlaylistEdit::addItem()
{
    editItem(_model->createChannel());
    ui->number->display(_model->rowCount());
}

void PlaylistEdit::addItem(const QString &name,
                           const QString &url)
{
    _model->createChannel(name, url);
    ui->number->display(_model->rowCount());
}

void PlaylistEdit::save()
{
    File file = FileDialogs::savePlaylist();

    if (file.path().isEmpty() || file.type() == -1)
        return;

    QFile f(file.path());
    if (!f.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Tano"),
                            tr("Cannot write file %1:\n%2.")
                            .arg(file.path())
                            .arg(f.errorString()));
        return;
    }
    f.close();

    switch (file.type())
    {
    case Tano::M3U:
    case Tano::M3UClean:
    case Tano::CSV:
    case Tano::JS:
        _model->save(file.path(), ui->editName->text(), Tano::FileType(file.type()));
        break;
    case Tano::M3UUdpxy:
        int s;
        s = QMessageBox::warning(this, tr("Export to M3U format with Udpxy URLs"),
                                 tr("You need to have valid Udpxy settings or the exported playlist will contain classic URLs."),
                                 QMessageBox::Save | QMessageBox::Cancel,
                                 QMessageBox::Cancel);

        switch (s)
        {
        case QMessageBox::Save:
            _model->save(file.path(), ui->editName->text(), Tano::FileType(file.type()));
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

    _model->exportTvheadend(dialog.location(), dialog.interface(), dialog.xmltv());
}

void PlaylistEdit::exportXmltvId()
{
    QString file = FileDialogs::saveXmltv();

    if (file.isEmpty())
        return;

    _model->save(file, "", Tano::XmltvId);
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
    PrintDialog dialog(ui->editName->text(), _model);
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
        for(int i = 0; i < _model->rowCount(); i++) {
            if(_model->row(i)->url() == currentIp()) {
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

void PlaylistEdit::editItem(Channel *channel)
{
    if(channel == 0) {
        ui->editWidget->setEnabled(false);
        return;
    }

    if(!ui->editWidget->isEnabled())
        ui->editWidget->setEnabled(true);

    ui->playlist->setCurrentChannel(channel);

    ui->editNumber->setText(channel->numberString());
    ui->editChannelName->setText(channel->name());
    ui->editUrl->setText(channel->url());
    ui->editCategories->setText(channel->categories().join(","));
    ui->editLanguage->setText(channel->language());
    ui->editEpg->setText(channel->epg());
    ui->editLogo->setText(channel->logo());
}

void PlaylistEdit::editChannelNumber()
{
    QString text = ui->editNumber->text();
    if(text.toInt() != ui->playlist->currentChannel()->number())
        _model->processNumber(ui->playlist->currentChannel(), text.toInt());
    else
        QMessageBox::warning(this, tr("Tano"),
                            tr("A channel with this number already exists!"));
    ui->editNumber->setText(QString().number(ui->playlist->currentChannel()->number()));
}

void PlaylistEdit::editChannelName(const QString &text)
{
    ui->playlist->currentChannel()->setName(text);
}

void PlaylistEdit::editChannelUrl(const QString &text)
{
    ui->playlist->currentChannel()->setUrl(text);
}

void PlaylistEdit::editChannelCategories(const QString &text)
{
    ui->playlist->currentChannel()->setCategories(text.split(","));
}

void PlaylistEdit::editChannelLanguage(const QString &text)
{
    ui->playlist->currentChannel()->setLanguage(text);
}

void PlaylistEdit::editChannelEpg(const QString &text)
{
    ui->playlist->currentChannel()->setEpg(text);
}

void PlaylistEdit::editChannelLogo(const QString &text)
{
    ui->playlist->currentChannel()->setLogo(text);
}

void PlaylistEdit::moveUp()
{
    _model->moveUp(ui->playlist->currentChannel());
    ui->editNumber->setText(ui->playlist->currentChannel()->numberString());
}

void PlaylistEdit::moveDown()
{
    _model->moveDown(ui->playlist->currentChannel());
    ui->editNumber->setText(ui->playlist->currentChannel()->numberString());
}
