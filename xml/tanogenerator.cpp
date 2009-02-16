#include <QtGui>

#include "tanogenerator.h"

TanoGenerator::TanoGenerator(QTreeWidget *treeWidget)
    : treeWidget(treeWidget)
{
}

bool TanoGenerator::write(QIODevice *device)
{
    out.setDevice(device);
    out.setCodec("UTF-8");
    out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        << "<!DOCTYPE tano>\n"
        << "<tano version=\"1.1\">\n";
    for (int i = 0; i < treeWidget->topLevelItemCount(); ++i)
        generateItem(treeWidget->topLevelItem(i), 1);
    out << "</tano>\n";
    return true;
}

QString TanoGenerator::indent(int depth)
{
    const int IndentSize = 4;
    return QString(IndentSize * depth, ' ');
}

QString TanoGenerator::escapedText(const QString &str)
{
    QString result = str;
    result.replace("&", "&amp;");
    result.replace("<", "&lt;");
    result.replace(">", "&gt;");
    return result;
}

QString TanoGenerator::escapedAttribute(const QString &str)
{
    QString result = escapedText(str);
    result.replace("\"", "&quot;");
    result.prepend("\"");
    result.append("\"");
    return result;
}

void TanoGenerator::generateItem(QTreeWidgetItem *item, int depth)
{
    QString tagName = item->data(0, Qt::UserRole).toString();
    if (tagName == "category") {
        bool expanded = !treeWidget->isItemExpanded(item);
        bool type;
        if(treeWidget->indexOfTopLevelItem(item) == -1)
        	type = false;
        else
        	type = true;

        out << indent(depth) << "<category "
							 << "expanded=\"" << (expanded ? "false" : "true") << "\""
							 << "type=\"" << (type ? "main" : "sub")
                             << "\">\n"
            << indent(depth + 1) << "<title>" << escapedText(item->text(0))
                                 << "</title>\n";
        for (int i = 0; i < item->childCount(); ++i)
            generateItem(item->child(i), depth + 1);
        out << indent(depth) << "</category>\n";
    } else if (tagName == "channel") {
        out << indent(depth) << "<channel>\n"
            << indent(depth + 1) << "<title>" << escapedText(item->text(0))
                                 << "</title>\n"
            << indent(depth + 1) << "<name>" << escapedText(item->text(1))
                                 << "</name>\n"
            << indent(depth + 1) << "<epg>" << escapedText(item->text(3))
                                 << "</epg>\n"
            << indent(depth + 1) << "<language>" << escapedText(item->text(2))
                                 << "</language>\n"
            << indent(depth + 1) << "<url>" << escapedText(item->text(4))
                                 << "</url>\n"
            << indent(depth) << "</channel>\n";
    }
}
