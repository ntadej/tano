#include <QMessageBox>
#include <QHeaderView>
#include <QDir>
#include <QFileDialog>
#include <QCloseEvent>

#include "../xml/M3UGenerator.h"
#include "../xml/M3UHandler.h"
#include "../xml/tanohandler.h"

#include "EditPlaylist.h"

EditPlaylist::EditPlaylist(QWidget *parent)
    : QMainWindow(parent)
{
	ui.setupUi(this);

	closeEnabled = false;

	connect(ui.actionDelete, SIGNAL(triggered()), this, SLOT(deleteItem()));
	connect(ui.actionAddChannel, SIGNAL(triggered()), this, SLOT(addItemChannel()));
	connect(ui.actionReload, SIGNAL(triggered()), this, SLOT(open()));
	connect(ui.actionSave, SIGNAL(triggered()), this, SLOT(save()));
	connect(ui.actionClose, SIGNAL(triggered()), this, SLOT(exit()));
	connect(ui.actionImport, SIGNAL(triggered()), this, SLOT(import()));

	ui.playlist->header()->setResizeMode(QHeaderView::ResizeToContents);

	channelIcon = QIcon(":/icons/images/video.png");
}

EditPlaylist::~EditPlaylist()
{

}

void EditPlaylist::closeEvent(QCloseEvent *event)
{
	if(!closeEnabled) {
		event->ignore();
		//ui.actionClose->trigger();
	} else {
		closeEnabled = false;
	}
}

void EditPlaylist::deleteItem()
{
	if(ui.playlist->currentItem()->parent())
		ui.playlist->currentItem()->parent()->removeChild(ui.playlist->currentItem());
	else
		ui.playlist->takeTopLevelItem(ui.playlist->indexOfTopLevelItem(ui.playlist->currentItem()));
}

void EditPlaylist::addItemChannel()
{
	QStringList defaults;
	defaults << "#" << tr("Channel") << tr("Categories") << "URL" << tr("language") << "";

	QTreeWidgetItem *item = new QTreeWidgetItem(ui.playlist, ui.playlist->currentItem());
	item->setIcon(0,channelIcon);
	item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled | Qt::ItemIsEditable);
	item->setData(0, Qt::UserRole, "channel");

	item->setText(0,defaults.at(0));
	item->setText(1,defaults.at(1));
	item->setText(2,defaults.at(2));
	item->setText(3,defaults.at(3));
	item->setText(4,defaults.at(4));
	item->setText(5,defaults.at(5));
}

void EditPlaylist::save()
{
	fileName =
		QFileDialog::getSaveFileName(this, tr("Save Channel list"),
									QDir::homePath(),
									tr("Tano TV Channel list Files (*.m3u)"));
	if (fileName.isEmpty())
		return;

	QFile file(fileName);
	if (!file.open(QFile::WriteOnly | QFile::Text)) {
		QMessageBox::warning(this, tr("Tano"),
							tr("Cannot write file %1:\n%2.")
							.arg(fileName)
							.arg(file.errorString()));
		return;
	}

	M3UGenerator *generator = new M3UGenerator(ui.editName->text(), ui.playlist);
	generator->write(&file);
	delete generator;
}

void EditPlaylist::open()
{
	M3UHandler *load = new M3UHandler(ui.playlist, true);

	load->clear();
	ui.playlist->clear();

	QFile file(fileN);
	if (!file.open(QFile::ReadOnly | QFile::Text)) {
		QMessageBox::warning(this, tr("Tano"),
							tr("Cannot read file %1:\n%2.")
							.arg(fileN)
							.arg(file.errorString()));
		return;
	}
	load->processFile(fileN);

	ui.editName->setText(load->getName());
	setWindowTitle(load->getName());

	show();

	delete load;
}

void EditPlaylist::import()
{
	TanoHandler *handler = new TanoHandler(ui.playlist,true);

	QString fileNameI =
			QFileDialog::getOpenFileName(this, tr("Open Channel list File"),
										QDir::homePath(),
										tr("Tano TV Old Channel list Files(*.tano *.xml)"));
	if (fileNameI.isEmpty())
		return;

	ui.playlist->clear();

	QXmlSimpleReader reader;
	reader.setContentHandler(handler);
	reader.setErrorHandler(handler);

	QFile file(fileNameI);
	if (!file.open(QFile::ReadOnly | QFile::Text)) {
		QMessageBox::warning(this, tr("Tano"),
							tr("Cannot read file %1:\n%2.")
							.arg(fileNameI)
							.arg(file.errorString()));
		return;
	}

	QXmlInputSource xmlInputSource(&file);
	if (!reader.parse(xmlInputSource))
		return;

	ui.editName->setText(handler->getName());
	setWindowTitle(handler->getName());

	delete handler;
}

void EditPlaylist::setFile(QString file)
{
	fileN = file;
}

void EditPlaylist::exit()
{
	if(closeEnabled) {
		close();
		return;
	}

	int ret;
	ret = QMessageBox::warning(this, ui.editName->text(),
								   tr("Do you want close the editor?\nYou will lose any unsaved settings."),
								   QMessageBox::Close | QMessageBox::Cancel,
								   QMessageBox::Close);

	switch (ret) {
		case QMessageBox::Close:
			closeEnabled = true;
			ui.actionClose->trigger();
			break;
		case QMessageBox::Cancel:
			break;
		default:
			break;
	}
}
