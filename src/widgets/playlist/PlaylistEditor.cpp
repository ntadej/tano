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

#include "core/Common.h"
#include "core/File.h"
#include "core/playlist/containers/Channel.h"
#include "core/playlist/PlaylistModel.h"
#include "core/playlist/handlers/CSVHandler.h"
#include "core/settings/Settings.h"

#include "common/FileDialogs.h"
#include "dialogs/AboutDialog.h"
#include "dialogs/PrintDialog.h"
#include "playlist/PlaylistEditorHelp.h"
#include "playlist/PlaylistEditorScan.h"
#include "playlist/PlaylistExportTvheadend.h"
#include "playlist/PlaylistImportCSV.h"

#include "PlaylistEditor.h"
#include "ui_PlaylistEditor.h"

PlaylistEditor::PlaylistEditor(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::PlaylistEditor),
      _closeEnabled(false),
      _map(QHash<QString, QString>())
{
    ui->setupUi(this);
    ui->editWidget->setEnabled(false);
    ui->playlist->editMode();
    setTitle();

    _model = new PlaylistModel(this);
    ui->playlist->setModel(_model);

	createSettings();
	createConnections();

	ui->scan->setModel(_model);

    connect(ui->scan, SIGNAL(addItem(QString, QString)), this, SLOT(addItem(QString, QString)));
    connect(ui->scan, SIGNAL(scan(bool)), this, SLOT(scan(bool)));

    _menuExport = new QMenu();
    _menuExport->addAction(ui->actionExportTvheadend);
    _menuExport->addAction(ui->actionExportXmltvId);

    //ui->toolBar->insertAction(ui->actionClose, ui->actionAbout);
    //ui->toolBar->insertAction(ui->actionHelp, ui->actionSettings);
}

PlaylistEditor::~PlaylistEditor()
{
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
    connect(ui->actionAutoXmltv, SIGNAL(triggered()), this, SLOT(autoMapXmltv()));

    connect(ui->editName, SIGNAL(textChanged(QString)), this, SLOT(setTitle(QString)));

    connect(ui->buttonApplyNum, SIGNAL(clicked()), this, SLOT(editChannelNumber()));
    connect(ui->editType, SIGNAL(currentIndexChanged(int)), this, SLOT(editChannelType(int)));
    connect(ui->editChannelName, SIGNAL(textChanged(QString)), this, SLOT(editChannelName(QString)));
    connect(ui->editUrl, SIGNAL(textChanged(QString)), this, SLOT(editChannelUrl(QString)));
    connect(ui->editCategories, SIGNAL(textChanged(QString)), this, SLOT(editChannelCategories(QString)));
    connect(ui->editLanguage, SIGNAL(textChanged(QString)), this, SLOT(editChannelLanguage(QString)));
    connect(ui->editXmltvId, SIGNAL(textChanged(QString)), this, SLOT(editChannelXmltvId(QString)));
    connect(ui->editLogo, SIGNAL(textChanged(QString)), this, SLOT(editChannelLogo(QString)));

    connect(ui->actionUp, SIGNAL(triggered()), this, SLOT(moveUp()));
    connect(ui->actionDown, SIGNAL(triggered()), this, SLOT(moveDown()));

    connect(ui->playlist, SIGNAL(itemSelected(Channel *)), this, SLOT(editItem(Channel *)));
}

void PlaylistEditor::menuOpenExport()
{
    _menuExport->exec(QCursor::pos());
}

void PlaylistEditor::setTitle(const QString &title)
{
    if (title.isEmpty())
        setWindowTitle(Tano::name() + tr("Editor"));
    else
        setWindowTitle(tr("%1 - %2 Editor").arg(title, Tano::name()));
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
    //SettingsEditorDialog s(this);
    //s.exec();
    //createSettings();
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
        file.type = File::M3U;
    }

    if (file.path.isEmpty() || file.type == File::Unknown)
        return;

    ui->editWidget->setEnabled(false);

    QFile f(file.path);
    if (!f.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Editor"),
                            tr("Cannot read file %1:\n%2.")
                            .arg(file.path)
                            .arg(f.errorString()));
        return;
    }
    f.close();

    switch (file.type)
    {
    case File::M3U:
    case File::JS:
        _model->open(file.path, refresh, file.type);
        break;
    case File::CSV:
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
    if (!ui->playlist->currentChannel())
        return;

    ui->editNumber->setValue(1);
    ui->editType->setCurrentIndex(0);
    ui->editChannelName->setText("");
    ui->editUrl->setText("");
    ui->editCategories->setText("");
    ui->editLanguage->setText("");
    ui->editXmltvId->setCurrentIndex(0);
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
    ui->playlist->channelSelected(0);

    File file = FileDialogs::savePlaylist();

    if (file.path.isEmpty() || file.type == File::Unknown)
        return;

    QFile f(file.path);
    if (!f.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Editor"),
                            tr("Cannot write file %1:\n%2.")
                            .arg(file.path)
                            .arg(f.errorString()));
        return;
    }
    f.close();

    switch (file.type)
    {
    case File::M3U:
    case File::M3UClean:
    case File::CSV:
    case File::JS:
        _model->save(file.path, ui->editName->text(), file.type);
        break;
    case File::M3UUdpxy:
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

    _model->save(file, "", File::XmltvId);
}

void PlaylistEditor::exit()
{
    if (_closeEnabled) {
        hide();
        //qApp->quit();
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
    ui->scan->setMediaInstance(instance);
}

void PlaylistEditor::setXmltvMap(const QHash<QString, QString> &map)
{
    ui->editXmltvId->clear();

    QStringList values = map.values();
    values.sort();

    ui->editXmltvId->addItem("");
    ui->editXmltvId->addItems(values);
    ui->editXmltvId->addItem(tr("Other"));

    _map = map;
}

void PlaylistEditor::scan(const bool &status)
{
    if (status) {
        ui->playlist->setEnabled(false);
    } else {
        ui->playlist->setEnabled(true);
    }
}

void PlaylistEditor::mapXmltv()
{
    ui->editXmltvId->setItemText(ui->editXmltvId->count() - 1, tr("Other"));

    QString newId;
    QString id = ui->playlist->currentChannel()->xmltvId();
    QString name = ui->playlist->currentChannel()->name();

    if (!id.isEmpty() && _map.values().contains(id)) {
        newId = id;
    } else if (!id.isEmpty() && _map.values().isEmpty()) {
        newId = id;
    } else {
        QStringList keys = _map.keys();
        keys = keys.filter(name, Qt::CaseInsensitive);
        if (!keys.isEmpty()) {
            newId = _map[keys[0]];
        } else {
            newId = id;
        }
    }

    for (int i = 0; i < ui->editXmltvId->count(); i++) {
        if (ui->editXmltvId->itemText(i) == newId) {
            ui->editXmltvId->setCurrentIndex(i);
            break;
        } else if (i == ui->editXmltvId->count() - 1) {
            ui->editXmltvId->setItemText(i, newId);
            ui->editXmltvId->setCurrentIndex(i);
        }
    }
}

void PlaylistEditor::autoMapXmltv()
{
    if (_map.keys().isEmpty()) {
        QMessageBox::warning(this, tr("Editor"),
                            tr("Please wait for XMLTV file to parse."));
        return;
    }

    for (int i = 0; i < _model->rowCount(); i++) {
        editItem(_model->row(i));
    }
}

void PlaylistEditor::editItem(Channel *channel)
{
    if (!channel) {
        ui->editWidget->setEnabled(false);
        return;
    }

    if (ui->playlist->currentChannel() != channel)
        ui->playlist->channelSelected(channel);

    if (!ui->editWidget->isEnabled())
        ui->editWidget->setEnabled(true);

    ui->editNumber->setValue(channel->number());
    ui->editType->setCurrentIndex(channel->type());
    ui->editChannelName->setText(channel->name());
    ui->editUrl->setText(channel->url());
    ui->editCategories->setText(channel->categories().join(","));
    ui->editLanguage->setText(channel->language());
    ui->editLogo->setText(channel->logo());
    mapXmltv();
}

void PlaylistEditor::editChannelNumber()
{
    if (!ui->playlist->currentChannel())
        return;

    QString text = ui->editNumber->text();
    if (text.toInt() != ui->playlist->currentChannel()->number())
        _model->processNumber(ui->playlist->currentChannel(), text.toInt());
    else
        QMessageBox::warning(this, tr("Editor"),
                            tr("A channel with this number already exists!"));
    ui->editNumber->setValue(ui->playlist->currentChannel()->number());
}

void PlaylistEditor::editChannelType(const int &type)
{
    if (!ui->playlist->currentChannel())
        return;

    ui->playlist->currentChannel()->setType(Channel::Type(type));
}

void PlaylistEditor::editChannelName(const QString &text)
{
    if (!ui->playlist->currentChannel())
        return;

    ui->playlist->currentChannel()->setName(text);
}

void PlaylistEditor::editChannelUrl(const QString &text)
{
    if (!ui->playlist->currentChannel())
        return;

    ui->playlist->currentChannel()->setUrl(text);
}

void PlaylistEditor::editChannelCategories(const QString &text)
{
    if (!ui->playlist->currentChannel())
        return;

    ui->playlist->currentChannel()->setCategories(text.split(","));
}

void PlaylistEditor::editChannelLanguage(const QString &text)
{
    if (!ui->playlist->currentChannel())
        return;

    ui->playlist->currentChannel()->setLanguage(text);
}

void PlaylistEditor::editChannelXmltvId(const QString &text)
{
    if (!ui->playlist->currentChannel())
        return;

    ui->playlist->currentChannel()->setXmltvId(text);
}

void PlaylistEditor::editChannelLogo(const QString &text)
{
    if (!ui->playlist->currentChannel())
        return;

    ui->playlist->currentChannel()->setLogo(text);
}

void PlaylistEditor::moveUp()
{
    if (!ui->playlist->currentChannel())
        return;

    _model->moveUp(ui->playlist->currentChannel());
    ui->editNumber->setValue(ui->playlist->currentChannel()->number());
}

void PlaylistEditor::moveDown()
{
    if (!ui->playlist->currentChannel())
        return;

    _model->moveDown(ui->playlist->currentChannel());
    ui->editNumber->setValue(ui->playlist->currentChannel()->number());
}
