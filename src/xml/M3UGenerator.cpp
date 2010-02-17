#include "M3UGenerator.h"

M3UGenerator::M3UGenerator(QTreeWidget *treeWidget, const QString &n)
	: treeWidget(treeWidget), name(n)
{

}

bool M3UGenerator::write(QIODevice *device)
{
	out.setDevice(device);
	out.setCodec("UTF-8");
	out << "#EXTM3U\n"
		<< "#EXTNAME:"
		<< name
		<< "\n\n";
	for (int i = 0; i < treeWidget->topLevelItemCount(); ++i)
		generateItem(treeWidget->topLevelItem(i), 1);
	return true;
}

void M3UGenerator::generateItem(QTreeWidgetItem *item, const int &depth)
{
	QString tagName = item->data(0, Qt::UserRole).toString();
	if (tagName == "category") {
		name = item->text(0);
		for (int i = 0; i < item->childCount(); ++i)
			generateItem(item->child(i), depth + 1);
	} else if (tagName == "channel") {
		out	<< "#EXTINF:"
			<< item->text(0) << ","
			<< item->text(1) << "\n";
		if(!item->text(2).isEmpty() || !item->text(3).isEmpty() || !item->text(4).isEmpty()) {
			out	<< "#EXTTV:"
				<< item->text(2) << ";"
				<< item->text(4) << ";"
				<< item->text(5)
				<< "\n";
		}
		out	<< item->text(3);
		out	<< "\n\n";
	}
}
