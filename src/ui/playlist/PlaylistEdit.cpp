/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2008-2010 Tadej Novak <ntadej@users.sourceforge.net>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#include "PlaylistEdit.h"
#include "ui_PlaylistEdit.h"

#include <QtCore/QDebug>
#include <QtGui/QCloseEvent>
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>

#include "core/Common.h"
#include "core/Settings.h"
#include "ui/dialogs/PrintDialog.h"

PlaylistEdit::PlaylistEdit(const QString &playlist,
						   const WId &video,
						   QWidget *parent)
	: QMainWindow(parent),
	ui(new Ui::PlaylistEdit),
	_closeEnabled(false),
	_playlist(playlist),
	_channelIcon(QIcon(":/icons/128x128/video.png"))
{
	ui->setupUi(this);

	createSettings();
	createConnections();

	_instance = new VlcInstance(Common::libvlcArgs(), this);
	_player = new VlcMediaPlayer(video, this);
	_timer = new QTimer();
	connect(_player, SIGNAL(state(bool, bool, bool)), this, SLOT(setState(bool)));
	connect(_timer, SIGNAL(timeout()), this, SLOT(checkCurrentIp()));

	ui->editWidget->setEnabled(false);
	ui->playlist->editMode();
	ui->playlist->open(_playlist);
	ui->editName->setText(ui->playlist->name());
	ui->number->display(ui->playlist->treeWidget()->topLevelItemCount());

	ui->epgCombo->addItem(Tano::epgType(Tano::Slovenia));
	ui->epgCombo->addItem(Tano::epgType(Tano::XMLTV));
	for(int i=0; i < ui->epgCombo->count(); i++) {
		if(ui->epgCombo->itemText(i) == Tano::epgType(ui->playlist->epgType())) {
			ui->epgCombo->setCurrentIndex(i);
			break;
		}
	}
}

PlaylistEdit::~PlaylistEdit()
{
	delete ui;
	delete _instance;
	delete _player;
	delete _timer;
}

void PlaylistEdit::changeEvent(QEvent *e)
{
	QMainWindow::changeEvent(e);
	switch (e->type()) {
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
	connect(ui->actionDelete, SIGNAL(triggered()), this, SLOT(deleteItem()));
	connect(ui->actionAdd, SIGNAL(triggered()), this, SLOT(addItem()));
	connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(save()));
	connect(ui->actionClose, SIGNAL(triggered()), this, SLOT(exit()));
	connect(ui->actionImport, SIGNAL(triggered()), this, SLOT(import()));
	connect(ui->actionPrint, SIGNAL(triggered()), this, SLOT(print()));

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

	connect(ui->buttonUpdate, SIGNAL(toggled(bool)), this, SLOT(refreshPlaylist(bool)));
}

void PlaylistEdit::deleteItem()
{
	ui->editNumber->setText("");
	ui->editChannelName->setText("");
	ui->editUrl->setText("");
	ui->editCategories->setText("");
	ui->editLanguage->setText("");
	ui->editEpg->setText("");

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
	QString fileName =
		QFileDialog::getSaveFileName(this, tr("Save Channel list"),
									QDir::homePath(),
									tr("Tano TV Channel list Files (*.m3u)"));
	if (fileName.isEmpty())
		return;

	ui->playlist->save(ui->editName->text(), ui->epgCombo->currentText(), fileName);

	_closeEnabled = true;
	exit();
}

void PlaylistEdit::import()
{
	QString fileName =
			QFileDialog::getOpenFileName(this, tr("Open Channel list File"),
										QDir::homePath(),
										tr("Tano TV Old Channel list Files(*.tano *.xml)"));
	if (fileName.isEmpty())
		return;

	ui->playlist->import(fileName);
	ui->number->display(ui->playlist->treeWidget()->topLevelItemCount());
}

void PlaylistEdit::exit()
{
	if(_closeEnabled) {
		hide();
		return;
	}

	int ret;
	ret = QMessageBox::warning(this, tr("Playlist Editor"),
								   tr("Do you want close the editor?\nYou will lose any unsaved settings."),
								   QMessageBox::Save | QMessageBox::Close | QMessageBox::Cancel,
								   QMessageBox::Close);

	switch (ret) {
		case QMessageBox::Save:
			ui->actionSave->trigger();
			break;
		case QMessageBox::Close:
			_closeEnabled = true;
			ui->actionClose->trigger();
			break;
		case QMessageBox::Cancel:
			break;
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
	if(!refresh) {
		_timer->stop();
		ui->progressBar->setValue(1);
		ui->playlist->setEnabled(true);
	} else {
		ui->playlist->setEnabled(false);

		qDebug() << ui->ipFrom->text() << ui->ipPort->value() << ui->ipTimeout->value();

		QStringList ipFrom = ui->ipFrom->text().split(".");
		_currentIp[0] = ipFrom[0].toInt();
		_currentIp[1] = ipFrom[1].toInt();
		_currentIp[2] = ipFrom[2].toInt();
		_currentIp[3] = 1;

		_currentPort = ui->ipPort->value();
		_currentTimeout = ui->ipTimeout->value();

		checkIp();
	}
}

void PlaylistEdit::checkIp()
{
	ui->progressBar->setValue(_currentIp[3]);
	_player->open(currentIp());

	_timer->start(_currentTimeout);
}

void PlaylistEdit::checkCurrentIp()
{
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
			qDebug() << "New";
			addItem(tr("New channel from scan"), currentIp());
		}
	}

	if(_currentIp[3] != 255) {
		_currentIp[3]++;
		checkIp();
	} else {
		ui->buttonUpdate->setChecked(false);
	}
}

QString PlaylistEdit::currentIp()
{
	QString ip = "udp://@";
	ip.append(QString().number(_currentIp[0])+".");
	ip.append(QString().number(_currentIp[1])+".");
	ip.append(QString().number(_currentIp[2])+".");
	ip.append(QString().number(_currentIp[3])+":");
	ip.append(QString().number(_currentPort));

	return ip;
}

void PlaylistEdit::setState(const bool &playing)
{
	_currentIpPlaying = playing;
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
