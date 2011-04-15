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
#include "core/Common.h"
#include "core/ConsoleOutput.h"
#include "core/Enums.h"
#include "core/Settings.h"
#include "ui/dialogs/AboutDialog.h"
#include "ui/dialogs/PrintDialog.h"
#include "ui/playlist/PlaylistImportCSV.h"
#include "ui/playlist/PlaylistImportWeb.h"

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
	_closeEnabled(false),
	_standalone(false)
{
	ui->setupUi(this);
	ui->editWidget->setEnabled(false);
	ui->playlist->editMode();

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
	_menuExport->addAction(ui->actionExportM3UClean);
	_menuExport->addAction(ui->actionExportCSV);
	_menuExport->addAction(ui->actionExportJs);

	_menuImport = new QMenu();
	_menuImport->addAction(ui->actionImportDownload);
	_menuImport->addAction(ui->actionImportCSV);
	_menuImport->addAction(ui->actionImportJs);
	_menuImport->addAction(ui->actionImportTanoOld);
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
	connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(aboutTano()));
	connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(open()));
	connect(ui->actionNew, SIGNAL(triggered()), this, SLOT(newPlaylist()));
	connect(ui->actionDelete, SIGNAL(triggered()), this, SLOT(deleteItem()));
	connect(ui->actionAdd, SIGNAL(triggered()), this, SLOT(addItem()));
	connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(save()));
	connect(ui->actionClose, SIGNAL(triggered()), this, SLOT(exit()));
	connect(ui->actionExport, SIGNAL(triggered()), this, SLOT(menuOpenExport()));
	connect(ui->actionExportM3UClean, SIGNAL(triggered()), this, SLOT(exportM3UClean()));
	connect(ui->actionExportCSV, SIGNAL(triggered()), this, SLOT(exportCSV()));
	connect(ui->actionExportJs, SIGNAL(triggered()), this, SLOT(exportJs()));
	connect(ui->actionImport, SIGNAL(triggered()), this, SLOT(menuOpenImport()));
	connect(ui->actionImportDownload, SIGNAL(triggered()), this, SLOT(importWeb()));
	connect(ui->actionImportCSV, SIGNAL(triggered()), this, SLOT(importCSV()));
	connect(ui->actionImportJs, SIGNAL(triggered()), this, SLOT(importJs()));
	connect(ui->actionImportTanoOld, SIGNAL(triggered()), this, SLOT(importTanoOld()));
	connect(ui->actionPrint, SIGNAL(triggered()), this, SLOT(print()));

	connect(ui->editName, SIGNAL(textChanged(QString)), this, SLOT(setTitle(QString)));

	connect(ui->buttonApplyNum, SIGNAL(clicked()), this, SLOT(editChannelNumber()));
	connect(ui->editNumber, SIGNAL(returnPressed()), ui->buttonApplyNum, SLOT(click()));
	connect(ui->editChannelName, SIGNAL(textChanged(QString)), this, SLOT(editChannelName(QString)));
	connect(ui->editUrl, SIGNAL(textChanged(QString)), this, SLOT(editChannelUrl(QString)));
	connect(ui->editCategories, SIGNAL(textChanged(QString)), this, SLOT(editChannelCategories(QString)));
	connect(ui->editLanguage, SIGNAL(textChanged(QString)), this, SLOT(editChannelLanguage(QString)));
	connect(ui->editEpg, SIGNAL(textChanged(QString)), this, SLOT(editChannelEpg(QString)));

	connect(ui->actionUp, SIGNAL(triggered()), this, SLOT(moveUp()));
	connect(ui->actionDown, SIGNAL(triggered()), this, SLOT(moveDown()));

	connect(ui->playlist->treeWidget(), SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)), this, SLOT(editItem(QTreeWidgetItem*)));

#if WITH_EDITOR_VLCQT
	connect(ui->buttonUpdate, SIGNAL(toggled(bool)), this, SLOT(refreshPlaylist(bool)));
#endif
}

void PlaylistEdit::menuOpenExport()
{
	_menuExport->exec(QCursor::pos());
}

void PlaylistEdit::menuOpenImport()
{
	_menuImport->exec(QCursor::pos());
}

void PlaylistEdit::setStandalone(const bool &standalone)
{
	_standalone = standalone;
	if(_standalone) {
		ui->toolBar->insertAction(ui->actionClose, ui->actionAbout);
	}
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

void PlaylistEdit::open(const QString &playlist,
						const bool &refresh)
{
	QString p;
	if(playlist == 0) {
		p = QFileDialog::getOpenFileName(this, tr("Open channel list"),
										 QDir::homePath(),
										 tr("Tano TV channel list files(*.m3u)"));
	} else {
		p = playlist;
	}

	ui->editWidget->setEnabled(false);
	ui->playlist->open(p, refresh);
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

	switch (ret) {
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
			break;
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
		QFileDialog::getSaveFileName(this, tr("Save channel list"),
									QDir::homePath(),
									tr("Tano TV channel list files (*.m3u)"));
	if (fileName.isEmpty())
		return;

	ui->playlist->save(ui->editName->text(), fileName);

	_closeEnabled = true;
	exit();
}

void PlaylistEdit::exportM3UClean()
{
	QString fileName =
		QFileDialog::getSaveFileName(this, tr("Export to original M3U format"),
									QDir::homePath(),
									tr("M3U (original) list files (*.m3u)"));
	if (fileName.isEmpty())
		return;

	ui->playlist->exportM3UClean(fileName);
}

void PlaylistEdit::exportCSV()
{
	QString fileName =
		QFileDialog::getSaveFileName(this, tr("Export to Comma-separated values file"),
									QDir::homePath(),
									tr("Comma-separated values file (*.csv *.txt)"));
	if (fileName.isEmpty())
		return;

	ui->playlist->exportCSV(fileName);
}

void PlaylistEdit::importCSV()
{
	QString fileName =
			QFileDialog::getOpenFileName(this, tr("Import Comma-separated values file"),
										QDir::homePath(),
										tr("Comma-separated values file (*.csv *.txt)"));
	if (fileName.isEmpty())
		return;

	PlaylistImportCSV dialog;
	dialog.exec();
	if(!dialog.proceed())
		return;

	ui->playlist->importCSV(fileName, dialog.separator(), dialog.header(), dialog.columns());
	ui->number->display(ui->playlist->treeWidget()->topLevelItemCount());
	ui->editName->setText(ui->playlist->name());
}

void PlaylistEdit::exportJs()
{
	QString fileName =
		QFileDialog::getSaveFileName(this, tr("Export to Sagem JS channel list"),
									QDir::homePath(),
									tr("Sagem JS channel list files (*.js)"));
	if (fileName.isEmpty())
		return;

	ui->playlist->exportJs(fileName);
}

void PlaylistEdit::importJs()
{
	QString fileName =
			QFileDialog::getOpenFileName(this, tr("Import Sagem JS channel list"),
										QDir::homePath(),
										tr("Sagem JS channel list files (*.js)"));
	if (fileName.isEmpty())
		return;

	ui->playlist->importJs(fileName);
	ui->number->display(ui->playlist->treeWidget()->topLevelItemCount());
	ui->editName->setText(ui->playlist->name());
}

void PlaylistEdit::importTanoOld()
{
	QString fileName =
			QFileDialog::getOpenFileName(this, tr("Import Tano TV old channel list"),
										QDir::homePath(),
										tr("Tano TV old channel list files(*.tano *.xml)"));
	if (fileName.isEmpty())
		return;

	ui->playlist->importTanoOld(fileName);
	ui->number->display(ui->playlist->treeWidget()->topLevelItemCount());
	ui->editName->setText(ui->playlist->name());
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
		if(_standalone)
			qApp->quit();
		return;
	}

	int ret;
	ret = QMessageBox::warning(this, tr("Playlist Editor"),
								   tr("Do you want close the editor?\nYou will lose any unsaved changes."),
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
