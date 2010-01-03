#ifndef M3UGENERATOR_H
#define M3UGENERATOR_H

#include <QTextStream>
#include <QTreeWidget>
#include <QTreeWidgetItem>

class M3UGenerator
{
public:
    M3UGenerator(QString n, QTreeWidget *treeWidget);

    bool write(QIODevice *device);

private:
    void generateItem(QTreeWidgetItem *item, int depth);

    QTreeWidget *treeWidget;
    QString name;
    QTextStream out;
};

#endif
