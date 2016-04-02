/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2016 Tadej Novak <tadej@tano.si>
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
#include <QtWidgets/QMenu>
#include <QtWidgets/QMessageBox>

#include "application/Common.h"
#include "common/File.h"
#include "common/widgets/FileDialogs.h"
#include "channels/containers/Channel.h"
#include "channels/editor/ChannelsEditorHelp.h"
#include "channels/editor/ChannelsImportCSV.h"
#include "channels/editor/ChannelsPrintDialog.h"
#include "channels/editor/ChannelsScan.h"
#include "channels/models/ChannelsModel.h"

#include "ChannelsEditor.h"
#include "ui_ChannelsEditor.h"

ChannelsEditor::ChannelsEditor(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::ChannelsEditor),
      _closeEnabled(false),
      _map(QHash<QString, QString>())
{
    ui->setupUi(this);
    ui->toolBar->setProperty("custom_style_disabled", true);
    ui->editWidget->setEnabled(false);
    setTitle();

    _model = new ChannelsModel(this);
    ui->channels->setChannelsModel(_model);

	createConnections();

    ui->scan->setChannelsModel(_model);

    connect(ui->scan, SIGNAL(addItem(QString, QString)), this, SLOT(addItem(QString, QString)));
    connect(ui->scan, SIGNAL(scan(bool)), this, SLOT(scan(bool)));

    _menuExport = new QMenu();
    _menuExport->addAction(ui->actionExportXmltvId);
}

ChannelsEditor::~ChannelsEditor()
{
    delete ui;
}

void ChannelsEditor::changeEvent(QEvent *e)
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

void ChannelsEditor::closeEvent(QCloseEvent *event)
{
    event->ignore();
    exit();
}

void ChannelsEditor::createConnections()
{
    connect(ui->actionHelp, SIGNAL(triggered()), this, SLOT(help()));
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(open()));
    connect(ui->actionNew, SIGNAL(triggered()), this, SLOT(newChannels()));
    connect(ui->actionDelete, SIGNAL(triggered()), this, SLOT(deleteItem()));
    connect(ui->actionAdd, SIGNAL(triggered()), this, SLOT(addItem()));
    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(save()));
    connect(ui->actionClose, SIGNAL(triggered()), this, SLOT(exit()));
    connect(ui->actionExport, SIGNAL(triggered()), this, SLOT(menuOpenExport()));
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
    connect(ui->editXmltvId, SIGNAL(currentTextChanged(QString)), this, SLOT(editChannelXmltvId(QString)));
    connect(ui->editLogo, SIGNAL(textChanged(QString)), this, SLOT(editChannelLogo(QString)));

    connect(ui->actionUp, SIGNAL(triggered()), this, SLOT(moveUp()));
    connect(ui->actionDown, SIGNAL(triggered()), this, SLOT(moveDown()));

    connect(ui->channels, SIGNAL(itemSelected(Channel *)), this, SLOT(editItem(Channel *)));
}

void ChannelsEditor::menuOpenExport()
{
    _menuExport->exec(QCursor::pos());
}

void ChannelsEditor::setTitle(const QString &title)
{
    if (title.isEmpty())
        setWindowTitle(Tano::name() + tr("Editor"));
    else
        setWindowTitle(tr("%1 - %2 Editor").arg(title, Tano::name()));
}

void ChannelsEditor::help()
{
    ChannelsEditorHelp help(this);
    help.exec();
}

void ChannelsEditor::open(const QString &channelsList,
                          bool refresh)
{
    File file;
    CSVInfo info;
    ChannelsImportCSV dialog;
    if (channelsList.isNull()) {
        file = FileDialogs::openChannelsList();
    } else {
        file.path = channelsList;
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

void ChannelsEditor::newChannels()
{
    if (!_model->rowCount())
        return;

    int ret;
    ret = QMessageBox::warning(this, tr("Channels Editor"),
                               tr("Do you want to create new channels list?\nYou will lose any unsaved changes."),
                               QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
                               QMessageBox::Discard);

    switch (ret)
    {
    case QMessageBox::Save:
        ui->actionSave->trigger();
    case QMessageBox::Discard:
        ui->editWidget->setEnabled(false);
        _model->clear();
        ui->editName->setText(tr("New channels list"));
        ui->number->display(_model->rowCount());
        break;
    case QMessageBox::Cancel:
    default:
        break;
    }
}

void ChannelsEditor::deleteItem()
{
    if (!ui->channels->currentChannel())
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

    ui->channels->channelSelected(_model->deleteChannel(ui->channels->currentChannel()));

    ui->number->display(_model->rowCount());
}

void ChannelsEditor::addItem()
{
    Channel *channel = _model->createChannel();
    ui->channels->channelSelected(channel);
    ui->number->display(_model->rowCount());
}

void ChannelsEditor::addItem(const QString &name,
                             const QString &url)
{
    _model->createChannel(name, url);
    ui->number->display(_model->rowCount());
}

void ChannelsEditor::save()
{
    ui->channels->channelSelected(0);

    File file = FileDialogs::saveChannelsList();

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
        _model->save(file.path, ui->editName->text(), file.type);
        break;
    case File::M3UUdpxy:
        int s;
        s = QMessageBox::warning(this, tr("Export to M3U format with Udpxy URLs"),
                                 tr("You need to have valid Udpxy settings or the exported channels list will contain classic URLs."),
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

void ChannelsEditor::exportXmltvId()
{
    QString file = FileDialogs::saveXmltv();

    if (file.isEmpty())
        return;

    _model->save(file, "", File::XmltvId);
}

void ChannelsEditor::exit()
{
    if (_closeEnabled) {
        hide();
        return;
    }

    int ret;
    ret = QMessageBox::warning(this, tr("Channels Editor"),
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

void ChannelsEditor::print()
{
    ChannelsPrintDialog dialog(ui->editName->text(), _model);
    dialog.exec();
}

void ChannelsEditor::setMediaInstance(VlcInstance *instance)
{
    ui->scan->setMediaInstance(instance);
}

void ChannelsEditor::setXmltvMap(const QHash<QString, QString> &map)
{
    ui->editXmltvId->clear();

    QStringList values = map.values();
    values.sort();

    ui->editXmltvId->addItem("");
    ui->editXmltvId->addItems(values);
    ui->editXmltvId->addItem(tr("Other"));

    _map = map;
}

void ChannelsEditor::scan(bool status)
{
    if (status) {
        ui->channels->setEnabled(false);
    } else {
        ui->channels->setEnabled(true);
    }
}

void ChannelsEditor::mapXmltv()
{
    QString newId;
    QString id = ui->channels->currentChannel()->xmltvId();
    QString name = ui->channels->currentChannel()->name();

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

    newId = id;

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

void ChannelsEditor::autoMapXmltv()
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

void ChannelsEditor::editItem(Channel *channel)
{
    if (!channel) {
        ui->editWidget->setEnabled(false);
        return;
    }

    if (ui->channels->currentChannel() != channel)
        ui->channels->channelSelected(channel);

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

void ChannelsEditor::editChannelNumber()
{
    if (!ui->channels->currentChannel())
        return;

    QString text = ui->editNumber->text();
    if (text.toInt() != ui->channels->currentChannel()->number())
        _model->processNumber(ui->channels->currentChannel(), text.toInt());
    else
        QMessageBox::warning(this, tr("Editor"),
                            tr("A channel with this number already exists!"));
    ui->editNumber->setValue(ui->channels->currentChannel()->number());
}

void ChannelsEditor::editChannelType(int type)
{
    if (!ui->channels->currentChannel())
        return;

    ui->channels->currentChannel()->setType(Channel::Type(type));
}

void ChannelsEditor::editChannelName(const QString &text)
{
    if (!ui->channels->currentChannel())
        return;

    ui->channels->currentChannel()->setName(text);
}

void ChannelsEditor::editChannelUrl(const QString &text)
{
    if (!ui->channels->currentChannel())
        return;

    ui->channels->currentChannel()->setUrl(text);
}

void ChannelsEditor::editChannelCategories(const QString &text)
{
    if (!ui->channels->currentChannel())
        return;

    ui->channels->currentChannel()->setCategories(text.split(","));
}

void ChannelsEditor::editChannelLanguage(const QString &text)
{
    if (!ui->channels->currentChannel())
        return;

    ui->channels->currentChannel()->setLanguage(text);
}

void ChannelsEditor::editChannelXmltvId(const QString &text)
{
    if (!ui->channels->currentChannel())
        return;

    ui->channels->currentChannel()->setXmltvId(text);
}

void ChannelsEditor::editChannelLogo(const QString &text)
{
    if (!ui->channels->currentChannel())
        return;

    ui->channels->currentChannel()->setLogo(text);
}

void ChannelsEditor::moveUp()
{
    if (!ui->channels->currentChannel())
        return;

    _model->moveUp(ui->channels->currentChannel());
    ui->editNumber->setValue(ui->channels->currentChannel()->number());
}

void ChannelsEditor::moveDown()
{
    if (!ui->channels->currentChannel())
        return;

    _model->moveDown(ui->channels->currentChannel());
    ui->editNumber->setValue(ui->channels->currentChannel()->number());
}