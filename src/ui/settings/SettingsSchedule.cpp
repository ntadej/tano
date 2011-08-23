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

#include "core/Settings.h"

#if !EDITOR
    #include "core/PluginLoader.h"
    #include "epg/XmltvController.h"
#endif

#include "SettingsSchedule.h"
#include "ui_SettingsSchedule.h"

SettingsSchedule::SettingsSchedule(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::SettingsSchedule)
{
    ui->setupUi(this);

    ui->location->setType(Tano::XmltvFile);
    ui->location->setResetValue(Settings::DEFAULT_LOCATION);

    connect(ui->comboGrabber, SIGNAL(currentIndexChanged(QString)), this, SLOT(processGrabber(QString)));

#if !EDITOR
    _controller = new XmltvController(this);
    listGrabbers();
    listPlugins();
#endif
}

SettingsSchedule::~SettingsSchedule()
{
    delete ui;
#if !EDITOR
    delete _controller;
#endif
}

void SettingsSchedule::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
            break;
        default:
            break;
    }
}

void SettingsSchedule::listGrabbers()
{
#if !EDITOR
    QStringList grabbers = _controller->grabbers();

    ui->comboGrabber->clear();

    for(int i = 0; i < grabbers.size(); i++) {
        ui->comboGrabber->addItem(grabbers[i].split("|")[1], grabbers[i].split("|")[0]);
    }

    ui->comboGrabber->addItem(tr("Custom"), "custom");
#endif
}

void SettingsSchedule::listPlugins()
{
#if !EDITOR
    PluginLoader *loader = new PluginLoader();
    for(int i = 0; i < loader->epgPlugin().size(); i++) {
        ui->comboPlugin->addItem(loader->epgName()[i]);
    }
    delete loader;
#endif
}

void SettingsSchedule::processGrabber(const QString &grabber)
{
    ui->labelCustom->setEnabled(grabber == tr("Custom"));
    ui->location->setEnabled(grabber == tr("Custom"));

    if(grabber != tr("Custom"))
        ui->location->reset();
}

Tano::EpgType SettingsSchedule::epgType() const
{
    if(ui->radioXmltv->isChecked())
        return Tano::EpgXmltv;
    else if(ui->radioPlugin->isChecked())
        return Tano::EpgPlugin;
}

void SettingsSchedule::setEpgType(const Tano::EpgType &type)
{
    if(type == Tano::EpgXmltv)
        ui->radioXmltv->setChecked(true);
    else if(type == Tano::EpgPlugin)
        ui->radioPlugin->setChecked(true);
}

QString SettingsSchedule::grabber() const
{
    return ui->comboGrabber->itemData(ui->comboGrabber->currentIndex()).toString();
}

void SettingsSchedule::setGrabber(const QString &grabber)
{
    ui->comboGrabber->setCurrentIndex(ui->comboGrabber->findData(grabber));
}

QString SettingsSchedule::location() const
{
    return ui->location->value();
}

void SettingsSchedule::setLocation(const QString &location)
{
    ui->location->setValue(location);
}

QString SettingsSchedule::plugin() const
{
    return ui->comboPlugin->currentText();
}

void SettingsSchedule::setPlugin(const QString &plugin)
{
    ui->comboPlugin->setCurrentIndex(ui->comboPlugin->findText(plugin));
}
