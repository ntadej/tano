#ifndef TANOGENERATOR_H
#define TANOGENERATOR_H

#include <QTextStream>

class QTreeWidget;
class QTreeWidgetItem;

class TanoGenerator
{
public:
    TanoGenerator(QTreeWidget *treeWidget);

    bool write(QIODevice *device);

private:
    static QString indent(int indentLevel);
    static QString escapedText(const QString &str);
    static QString escapedAttribute(const QString &str);
    void generateItem(QTreeWidgetItem *item, int depth);

    QTreeWidget *treeWidget;
    QTextStream out;
};

#endif
