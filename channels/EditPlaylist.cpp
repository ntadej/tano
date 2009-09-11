#include <QMessageBox>
#include <QHeaderView>
#include <QDir>
#include <QFileDialog>
#include <QCloseEvent>

#include "EditPlaylist.h"

EditPlaylist::EditPlaylist(QWidget *parent, QString fileName)
    : QMainWindow(parent)
{
	ui.setupUi(this);

	closeEnabled = false;

	add = new QMenu();
	add->addAction(ui.actionAddChannel);
	add->addAction(ui.actionAddCategory);
	add->addAction(ui.actionAddSubCategory);

	connect(ui.actionDelete, SIGNAL(triggered()), this, SLOT(deleteItem()));
	connect(ui.actionAddCategory, SIGNAL(triggered()), this, SLOT(addItemCategory()));
	connect(ui.actionAddSubCategory, SIGNAL(triggered()), this, SLOT(addItemSubCategory()));
	connect(ui.actionAddChannel, SIGNAL(triggered()), this, SLOT(addItemChannel()));
	connect(ui.actionReload, SIGNAL(triggered()), this, SLOT(open()));
	connect(ui.actionSave, SIGNAL(triggered()), this, SLOT(save()));
	connect(ui.actionAdd, SIGNAL(triggered()), this, SLOT(menuOpen()));
	connect(ui.actionClose, SIGNAL(triggered()), this, SLOT(exit()));

	ui.playlist->header()->setResizeMode(QHeaderView::Stretch);

	treeStyle();

	load = new TanoHandler(ui.playlist, true);

	fileN = fileName;

	open();
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

void EditPlaylist::treeStyle()
{
	categoryIcon.addPixmap(QPixmap(":/icons/images/folder.png"),
	                     QIcon::Normal, QIcon::Off);
	categoryIcon.addPixmap(QPixmap(":/icons/images/folder_video.png"),
	                     QIcon::Normal, QIcon::On);
	channelIcon = QIcon(":/icons/images/video.png");
}

void EditPlaylist::deleteItem()
{
	QTreeWidgetItem *parent = ui.playlist->currentItem()->parent();
	if(parent)
		ui.playlist->currentItem()->parent()->removeChild(ui.playlist->currentItem());
	else
		ui.playlist->takeTopLevelItem(ui.playlist->indexOfTopLevelItem(ui.playlist->currentItem()));
}

void EditPlaylist::addItemCategory()
{
	QStringList defaults;
	defaults << tr("Category");
	QTreeWidgetItem *item = new QTreeWidgetItem(defaults);
	item->setIcon(0,categoryIcon);
	item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled | Qt::ItemIsEditable);
	item->setData(0, Qt::UserRole, "category");
	ui.playlist->addTopLevelItem(item);
}

void EditPlaylist::addItemSubCategory()
{
	QStringList defaults;
	defaults << tr("Category");

	if(ui.playlist->currentItem()->data(0, Qt::UserRole).toString() == "category") {
		QTreeWidgetItem *item = new QTreeWidgetItem(ui.playlist->currentItem(), defaults);
		item->setIcon(0,categoryIcon);
		item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled | Qt::ItemIsEditable);
		item->setData(0, Qt::UserRole, "category");
	}
	else
		QMessageBox::warning(this, tr("Tano"),
									tr("Please, add subcategory to a category."));
}

void EditPlaylist::addItemChannel()
{
	QStringList defaults;
	defaults << tr("Channel") << tr("New channel") << tr("language") << "NI" << "URL";

	if(ui.playlist->currentItem()->data(0, Qt::UserRole).toString() == "category") {
		QTreeWidgetItem *item = new QTreeWidgetItem(ui.playlist->currentItem(), defaults);
		item->setIcon(0,channelIcon);
		item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled | Qt::ItemIsEditable);
		item->setData(0, Qt::UserRole, "channel");
	}
	else
		QMessageBox::warning(this, tr("Tano"),
									tr("Please, add channel to a category."));
}

void EditPlaylist::save()
{
	fileName =
		QFileDialog::getSaveFileName(this, tr("Save Channel list"),
									QDir::homePath(),
									tr("Tano TV Channel list Files (*.tano *.xml)"));
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

	generator = new TanoGenerator(ui.editName->text(), ui.playlist);
	generator->write(&file);
}

void EditPlaylist::open()
{
	load->clear();
	ui.playlist->clear();

	QXmlSimpleReader reader;
	reader.setContentHandler(load);
	reader.setErrorHandler(load);

	QFile file(fileN);
	if (!file.open(QFile::ReadOnly | QFile::Text)) {
		QMessageBox::warning(this, tr("Tano"),
							tr("Cannot read file %1:\n%2.")
							.arg(fileN)
							.arg(file.errorString()));
		return;
	}
	QXmlInputSource xmlInputSource(&file);
	reader.parse(xmlInputSource);

	ui.editName->setText(load->getName());
	setWindowTitle(load->getName());
}

void EditPlaylist::setFile(QString file)
{
	fileN = file;
	open();
}

void EditPlaylist::menuOpen()
{
	add->exec(QCursor::pos());
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
