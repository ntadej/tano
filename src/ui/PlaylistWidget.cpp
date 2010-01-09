#include "PlaylistWidget.h"

PlaylistWidget::PlaylistWidget(QWidget *parent)
	:QWidget(parent)
{
	ui.setupUi(this);
	ui.treeWidget->header()->setResizeMode(QHeaderView::ResizeToContents);

	connect(ui.treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SIGNAL(itemClicked(QTreeWidgetItem*, int)));
	connect(ui.categoryBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(processCategories(QString)));
	connect(ui.searchEdit, SIGNAL(textChanged(QString)), this, SLOT(processSearch(QString)));
}

PlaylistWidget::~PlaylistWidget()
{

}

QTreeWidget* PlaylistWidget::treeWidget()
{
	return ui.treeWidget;
}

void PlaylistWidget::clear()
{
	ui.treeWidget->clear();
}

void PlaylistWidget::setCategories(QStringList c)
{
	ui.categoryBox->clear();
	ui.categoryBox->insertItem(0,tr("All channels"));
	ui.categoryBox->insertItems(1,c);
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
