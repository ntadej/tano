#include <QFile>
#include <QMessageBox>

#include "PlaylistWidget.h"

PlaylistWidget::PlaylistWidget(QWidget *parent)
	:QWidget(parent)
{
	ui.setupUi(this);
	ui.treeWidget->header()->setResizeMode(QHeaderView::ResizeToContents);

	handler = new M3UHandler(ui.treeWidget);

	connect(ui.treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SIGNAL(itemClicked(QTreeWidgetItem*, int)));
	connect(ui.categoryBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(processCategories(QString)));
	connect(ui.searchEdit, SIGNAL(textChanged(QString)), this, SLOT(processSearch(QString)));
}

PlaylistWidget::~PlaylistWidget()
{
	delete handler;
}

void PlaylistWidget::clear()
{
	ui.treeWidget->clear();
}

void PlaylistWidget::open(QString file)
{
	handler->clear();

    if (file.isEmpty())
        return;

    _fileName = file;

    ui.treeWidget->clear();

    QFile f(_fileName);
    if (!f.open(QFile::ReadOnly | QFile::Text)) {
    	QMessageBox::warning(this, tr("Tano"),
							tr("Cannot read file %1:\n%2.")
							.arg(_fileName)
							.arg(f.errorString()));
        return;
    }

    handler->processFile(_fileName);

	ui.categoryBox->clear();
	ui.categoryBox->insertItem(0,tr("All channels"));
	ui.categoryBox->insertItems(1,handler->getCategories());
}

void PlaylistWidget::processCategories(QString cat)
{
	if(cat == tr("All channels"))
		for(int i=0; i<ui.treeWidget->topLevelItemCount(); i++)
			ui.treeWidget->topLevelItem(i)->setHidden(false);
	else
		for(int i=0; i<ui.treeWidget->topLevelItemCount(); i++)
			if(ui.treeWidget->topLevelItem(i)->text(2).contains(cat))
				ui.treeWidget->topLevelItem(i)->setHidden(false);
			else
				ui.treeWidget->topLevelItem(i)->setHidden(true);
}

void PlaylistWidget::processSearch(QString search)
{
	if(search == "")
		for(int i=0; i<ui.treeWidget->topLevelItemCount(); i++)
			ui.treeWidget->topLevelItem(i)->setHidden(false);
	else
		for(int i=0; i<ui.treeWidget->topLevelItemCount(); i++)
			if(ui.treeWidget->topLevelItem(i)->text(1).contains(search, Qt::CaseInsensitive))
				ui.treeWidget->topLevelItem(i)->setHidden(false);
			else
				ui.treeWidget->topLevelItem(i)->setHidden(true);
}

QString PlaylistWidget::name()
{
	return handler->getName();
}

QString PlaylistWidget::fileName()
{
	return _fileName;
}

QList<int> PlaylistWidget::nums()
{
	return handler->nums();
}

Channel *PlaylistWidget::channelRead(QTreeWidgetItem* clickedChannel)
{
	return handler->channelRead(clickedChannel);
}

Channel *PlaylistWidget::channelReadNum(int clickedChannel)
{
	return handler->channelReadNum(clickedChannel);
}

QTreeWidget *PlaylistWidget::treeWidget()
{
	return ui.treeWidget;
}
