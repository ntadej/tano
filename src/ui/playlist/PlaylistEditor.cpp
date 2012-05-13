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

#include <QtGui/QCloseEvent>

#if defined(Qt5)
    #include <QtWidgets/QMenu>
    #include <QtWidgets/QMessageBox>
#elif defined(Qt4)
    #include <QtGui/QMenu>
    #include <QtGui/QMessageBox>
#endif

#include "Config.h"
#include "container/core/Channel.h"
#include "core/Common.h"
#include "core/Settings.h"
#include "playlist/PlaylistModel.h"
#include "playlist/handlers/CSVHandler.h"
#include "ui/core/FileDialogs.h"
#include "ui/dialogs/AboutDialog.h"
#include "ui/dialogs/PrintDialog.h"
#include "ui/playlist/PlaylistEditorHelp.h"
#include "ui/playlist/PlaylistExportTvheadend.h"
#include "ui/playlist/PlaylistImportCSV.h"

#ifdef EDITOR
    #include "core/LocaleManager.h"
    #include "ui/settings-editor/SettingsEditorDialog.h"
#if UPDATE
    #include "update/UpdateDialog.h"
#endif
#endif

#if WITH_EDITOR_VLCQT
    #include "ui/playlist/PlaylistEditorScan.h"
#endif

#include "PlaylistEditor.h"
#include "ui_PlaylistEditor.h"

PlaylistEditor::PlaylistEditor(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::PlaylistEditor),
      _closeEnabled(false)
{
#ifdef EDITOR
    _locale = new LocaleManager();
#endif

    ui->setupUi(this);
    ui->editWidget->setEnabled(false);
    ui->playlist->editMode();

    _model = new PlaylistModel(this);
    ui->playlist->setModel(_model);

#if defined(EDITOR) && UPDATE
    _update = new UpdateDialog();
    _update->checkSilent();
#endif

	createSettings();
	createConnections();

#if WITH_EDITOR_VLCQT
    _scan = new PlaylistEditorScan();
    _scan->setModel(_model);
    ui->updateContents->addWidget(_scan);

    connect(_scan, SIGNAL(addItem(QString, QString)), this, SLOT(addItem(QString, QString)));
    connect(_scan, SIGNAL(scan(bool)), this, SLOT(scan(bool)));
#else
    ui->updateWidget->hide();
#endif

    _menuExport = new QMenu();
    _menuExport->addAction(ui->actionExportTvheadend);
    _menuExport->addAction(ui->actionExportXmltvId);

#ifdef EDITOR
    ui->toolBar->insertAction(ui->actionClose, ui->actionAbout);
    ui->toolBar->insertAction(ui->actionHelp, ui->actionSettings);
#endif
}

PlaylistEditor::~PlaylistEditor()
{
#if WITH_EDITOR_VLCQT
    ui->updateContents->removeWidget(_scan);
    delete _scan;
#endif

    delete ui;
}

void PlaylistEditor::changeEvent(QEvent *e)
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

void PlaylistEditor::closeEvent(QCloseEvent *event)
{
    event->ignore();
    exit();
}

void PlaylistEditor::createSettings()
{
    QScopedPointer<Settings> settings(new Settings(this));
    ui->toolBar->setToolButtonStyle(Qt::ToolButtonStyle(settings->toolbarLook()));
}

void PlaylistEditor::createConnections()
{
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(aboutTano()));
    connect(ui->actionHelp, SIGNAL(triggered()), this, SLOT(help()));
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
    connect(ui->actionPrint, SIGNAL(triggered()), this, SLOT(print()));

    connect(ui->editName, SIGNAL(textChanged(QString)), this, SLOT(setTitle(QString)));

    connect(ui->buttonApplyNum, SIGNAL(clicked()), this, SLOT(editChannelNumber()));
    connect(ui->editNumber, SIGNAL(returnPressed()), ui->buttonApplyNum, SLOT(click()));
    connect(ui->editRadio, SIGNAL(toggled(bool)), this, SLOT(editChannelRadio(bool)));
    connect(ui->editChannelName, SIGNAL(textChanged(QString)), this, SLOT(editChannelName(QString)));
    connect(ui->editUrl, SIGNAL(textChanged(QString)), this, SLOT(editChannelUrl(QString)));
    connect(ui->editCategories, SIGNAL(textChanged(QString)), this, SLOT(editChannelCategories(QString)));
    connect(ui->editLanguage, SIGNAL(textChanged(QString)), this, SLOT(editChannelLanguage(QString)));
    connect(ui->editXmltvId, SIGNAL(textChanged(QString)), this, SLOT(editChannelXmltvId(QString)));
    connect(ui->editLogo, SIGNAL(textChanged(QString)), this, SLOT(editChannelLogo(QString)));

    connect(ui->actionUp, SIGNAL(triggered()), this, SLOT(moveUp()));
    connect(ui->actionDown, SIGNAL(triggered()), this, SLOT(moveDown()));

    connect(ui->playlist, SIGNAL(itemSelected(Channel *)), this, SLOT(editItem(Channel *)));

#if defined(EDITOR) && UPDATE
    connect(_update, SIGNAL(newUpdate()), this, SLOT(updateAvailable()));
    connect(ui->actionUpdate, SIGNAL(triggered()), _update, SLOT(check()));
#endif
}

void PlaylistEditor::menuOpenExport()
{
    _menuExport->exec(QCursor::pos());
}

void PlaylistEditor::setTitle(const QString &title)
{
    if (title.isEmpty())
        setWindowTitle(tr("Tano Editor"));
    else
        setWindowTitle(tr("%1 - Tano Editor").arg(title));
}


void PlaylistEditor::aboutTano()
{
    AboutDialog about(this);
    about.exec();
}

void PlaylistEditor::help()
{
    PlaylistEditorHelp help(this);
    help.exec();
}

void PlaylistEditor::settings()
{
#ifdef EDITOR
    SettingsEditorDialog s(this);
    s.exec();
    _locale->setLocale();
    createSettings();
#endif
}

void PlaylistEditor::updateAvailable()
{
#if defined(EDITOR) && UPDATE
    ui->toolBar->insertAction(ui->actionAbout, ui->actionUpdate);
#endif
}

void PlaylistEditor::open(const QString &playlist,
                          const bool &refresh)
{
    File file;
    CSVInfo info;
    PlaylistImportCSV dialog;
    if (playlist.isNull()) {
        file = FileDialogs::openPlaylist();
    } else {
        file.path = playlist;
        file.type = Tano::M3U;
    }

    if (file.path.isEmpty() || file.type == Tano::Unknown)
        return;

    ui->editWidget->setEnabled(false);

    QFile f(file.path);
    if (!f.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Tano"),
                            tr("Cannot read file %1:\n%2.")
                            .arg(file.path)
                            .arg(f.errorString()));
        return;
    }
    f.close();

    switch (file.type)
    {
    case Tano::M3U:
    case Tano::JS:
        _model->open(file.path, refresh, file.type);
        break;
    case Tano::CSV:
        dialog.exec();
        if (!dialog.proceed())
            return;

        info.separator = dialog.separator();
        info.header = dialog.header();
        info.columns = dialog.columns();
        _model->open(file.path, refresh, file.type, info);
        break;
    default:
        break;
    }

    ui->editName->setText(_model->name());
    ui->number->display(_model->rowCount());
}

void PlaylistEditor::newPlaylist()
{
    if (!_model->rowCount())
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

void PlaylistEditor::deleteItem()
{
    ui->editNumber->setText("");
    ui->editRadio->setChecked(false);
    ui->editChannelName->setText("");
    ui->editUrl->setText("");
    ui->editCategories->setText("");
    ui->editLanguage->setText("");
    ui->editXmltvId->setText("");
    ui->editLogo->setText("");

    ui->editWidget->setEnabled(false);

    ui->playlist->channelSelected(_model->deleteChannel(ui->playlist->currentChannel()));

    ui->number->display(_model->rowCount());
}

void PlaylistEditor::addItem()
{
    Channel *channel = _model->createChannel();
    ui->playlist->channelSelected(channel);
    ui->number->display(_model->rowCount());
}

void PlaylistEditor::addItem(const QString &name,
                             const QString &url)
{
    _model->createChannel(name, url);
    ui->number->display(_model->rowCount());
}

void PlaylistEditor::save()
{
    File file = FileDialogs::savePlaylist();

    if (file.path.isEmpty() || file.type == Tano::Unknown)
        return;

    QFile f(file.path);
    if (!f.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Tano"),
                            tr("Cannot write file %1:\n%2.")
                            .arg(file.path)
                            .arg(f.errorString()));
        return;
    }
    f.close();

    switch (file.type)
    {
    case Tano::M3U:
    case Tano::M3UClean:
    case Tano::CSV:
    case Tano::JS:
        _model->save(file.path, ui->editName->text(), file.type);
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
            _model->save(file.path, ui->editName->text(), file.type);
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

void PlaylistEditor::exportTvheadend()
{
    PlaylistExportTvheadend dialog;
    dialog.exec();
    if (!dialog.proceed())
        return;

    _model->exportTvheadend(dialog.location(), dialog.interface());
}

void PlaylistEditor::exportXmltvId()
{
    QString file = FileDialogs::saveXmltv();

    if (file.isEmpty())
        return;

    _model->save(file, "", Tano::XmltvId);
}

void PlaylistEditor::exit()
{
    if (_closeEnabled) {
        hide();
#ifdef EDITOR
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

void PlaylistEditor::print()
{
    PrintDialog dialog(ui->editName->text(), _model);
    dialog.exec();
}

void PlaylistEditor::setMediaInstance(VlcInstance *instance)
{
#if WITH_EDITOR_VLCQT
    _scan->setMediaInstance(instance);
#else
    Q_UNUSED(instance)
#endif
}

void PlaylistEditor::scan(const bool &status)
{
    if (status) {
        ui->playlist->setEnabled(false);
    } else {
        ui->playlist->setEnabled(true);
    }
}

void PlaylistEditor::editItem(Channel *channel)
{
    if (!channel) {
        ui->editWidget->setEnabled(false);
        return;
    }

    if (!ui->editWidget->isEnabled())
        ui->editWidget->setEnabled(true);

    ui->editNumber->setText(channel->numberString());
    ui->editRadio->setChecked(channel->radio());
    ui->editChannelName->setText(channel->name());
    ui->editUrl->setText(channel->url());
    ui->editCategories->setText(channel->categories().join(","));
    ui->editLanguage->setText(channel->language());
    ui->editXmltvId->setText(channel->xmltvId());
    ui->editLogo->setText(channel->logo());
}

void PlaylistEditor::editChannelNumber()
{
    QString text = ui->editNumber->text();
    if (text.toInt() != ui->playlist->currentChannel()->number())
        _model->processNumber(ui->playlist->currentChannel(), text.toInt());
    else
        QMessageBox::warning(this, tr("Tano"),
                            tr("A channel with this number already exists!"));
    ui->editNumber->setText(QString().number(ui->playlist->currentChannel()->number()));
}

void PlaylistEditor::editChannelRadio(const bool &radio)
{
    ui->playlist->currentChannel()->setRadio(radio);
}

void PlaylistEditor::editChannelName(const QString &text)
{
    ui->playlist->currentChannel()->setName(text);
}

void PlaylistEditor::editChannelUrl(const QString &text)
{
    ui->playlist->currentChannel()->setUrl(text);
}

void PlaylistEditor::editChannelCategories(const QString &text)
{
    ui->playlist->currentChannel()->setCategories(text.split(","));
}

void PlaylistEditor::editChannelLanguage(const QString &text)
{
    ui->playlist->currentChannel()->setLanguage(text);
}

void PlaylistEditor::editChannelXmltvId(const QString &text)
{
    ui->playlist->currentChannel()->setXmltvId(text);
}

void PlaylistEditor::editChannelLogo(const QString &text)
{
    ui->playlist->currentChannel()->setLogo(text);
}

void PlaylistEditor::moveUp()
{
    _model->moveUp(ui->playlist->currentChannel());
    ui->editNumber->setText(ui->playlist->currentChannel()->numberString());
}

void PlaylistEditor::moveDown()
{
    _model->moveDown(ui->playlist->currentChannel());
    ui->editNumber->setText(ui->playlist->currentChannel()->numberString());
}
