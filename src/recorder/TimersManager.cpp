#include <QDebug>
#include <QFile>
#include <QMessageBox>

#include "../Common.h"
#include "../xml/TimersGenerator.h"
#include "TimersManager.h"

TimersManager::TimersManager(QWidget *parent)
	:QMainWindow(parent)
{
	ui.setupUi(this);
	ui.timersWidget->header()->setResizeMode(QHeaderView::ResizeToContents);

	channel = 0;
	currentTimer = 0;
	currentItem = 0;
	ui.dockWidgetContents->setDisabled(true);

	connect(ui.timersWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(edit(QTreeWidgetItem*)));
	connect(ui.actionNew, SIGNAL(triggered()), this, SLOT(newItem()));
	connect(ui.buttonCreate, SIGNAL(clicked()), this, SLOT(addItem()));
	connect(ui.playlistWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(playlist(QTreeWidgetItem*)));
	connect(ui.buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(action(QAbstractButton*)));
}

TimersManager::~TimersManager() {

}

void TimersManager::action(QAbstractButton *button)
{
	switch(ui.buttonBox->standardButton(button)) {
	case 0x00000800:
		write();
		break;
	case 0x00200000:
		close();
		break;
	default:
		break;
	}
}

void TimersManager::openPlaylist(QString file)
{
	ui.playlistWidget->open(file);
}

void TimersManager::newItem()
{
	ui.dockWidgetContents->setDisabled(true);
	ui.mainWidget->setCurrentIndex(1);
}

void TimersManager::addItem()
{
	if(channel == 0 || ui.editNameNew->text() == "") {
		QMessageBox::warning(this, tr("Tano"),
							tr("Please enter a name and select a channel from the list."));
		return;
	}

	QTreeWidgetItem *newItem = new QTreeWidgetItem(ui.timersWidget, QStringList("Test"));
	newItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);

	currentTimer = new Timer(ui.editNameNew->text(),channel->name(),ui.playlistWidget->fileName(),channel->num());
	map.insert(newItem, currentTimer);

	ui.dockWidgetContents->setDisabled(false);
	edit(newItem);
	ui.mainWidget->setCurrentIndex(0);

	channel = 0;
}

void TimersManager::playlist(QTreeWidgetItem *item)
{
	channel = ui.playlistWidget->channelRead(item);
}

void TimersManager::edit(QTreeWidgetItem *item)
{
	if(currentTimer != 0) {
		disconnect(ui.editName, SIGNAL(textChanged(QString)), this, SLOT(applyName(QString)));
		disconnect(ui.editName, SIGNAL(textChanged(QString)), currentTimer, SLOT(setName(QString)));
		disconnect(ui.editDate, SIGNAL(dateChanged(QDate)), currentTimer, SLOT(setDate(QDate)));
		disconnect(ui.editStartTime, SIGNAL(timeChanged(QTime)), currentTimer, SLOT(setStartTime(QTime)));
		disconnect(ui.editEndTime, SIGNAL(timeChanged(QTime)), currentTimer, SLOT(setEndTime(QTime)));
	} else {
		ui.dockWidgetContents->setDisabled(false);
	}

	currentTimer = map[item];
	currentItem = item;

	connect(ui.editName, SIGNAL(textChanged(QString)), this, SLOT(applyName(QString)));
	connect(ui.editName, SIGNAL(textChanged(QString)), currentTimer, SLOT(setName(QString)));
	connect(ui.editDate, SIGNAL(dateChanged(QDate)), currentTimer, SLOT(setDate(QDate)));
	connect(ui.editStartTime, SIGNAL(timeChanged(QTime)), currentTimer, SLOT(setStartTime(QTime)));
	connect(ui.editEndTime, SIGNAL(timeChanged(QTime)), currentTimer, SLOT(setEndTime(QTime)));

	ui.editName->setText(currentTimer->name());
	ui.editChannel->setText(currentTimer->channel());
	ui.editNum->display(currentTimer->num());
	ui.editPlaylist->setText(currentTimer->playlist());
	ui.editDate->setDate(currentTimer->date());
	ui.editStartTime->setTime(currentTimer->startTime());
	ui.editEndTime->setTime(currentTimer->endTime());
}

void TimersManager::applyName(QString name)
{
	currentItem->setText(0,name);
}

void TimersManager::write()
{
	QString fileName;

	/*fileName =
		QFileDialog::getSaveFileName(this, tr("Save Timers"),
	  								QDir::homePath(),
	   									tr("Tano Timers (*.tano.xml)"));
	*/

	fileName = Common::settingsPath() + "timers.tano.xml";
	qDebug() << fileName;

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

	TanoGenerator *generator = new TanoGenerator(ui.timersWidget, map);
	generator->write(&file);
}
