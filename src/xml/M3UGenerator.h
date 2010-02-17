#ifndef TANO_M3UGENERATOR_H_
#define TANO_M3UGENERATOR_H_

#include <QtCore/QTextStream>
#include <QtGui/QTreeWidget>
#include <QtGui/QTreeWidgetItem>

class M3UGenerator
{
public:
	M3UGenerator(QTreeWidget *treeWidget, const QString &n);

    bool write(QIODevice *device);

private:
	void generateItem(QTreeWidgetItem *item, const int &depth);

    QTreeWidget *treeWidget;
    QString name;
    QTextStream out;
};

#endif // TANO_M3UGENERATOR_H_
