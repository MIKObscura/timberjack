#include "mainwindow.h"
#include "vdf_parser.hpp"
#include <QApplication>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QTreeWidgetItem>
#include <QList>
#include <QString>
#include <iostream>
#include <sstream>
#include <string>

/*
* Recursively traverses the tree and puts the items found in the QTreeWidget
* params:
*  kv: vdf object that will be traversed
*  list: top level items list
*  parent: parent of the items currently being initialized (nullptr if top level item)
*/
void traverseTree(tyti::vdf::multikey_object &kv, QList<QTreeWidgetItem *> &list, QTreeWidgetItem *parent = nullptr){
    for(auto it = kv.attribs.begin(); it != kv.attribs.end(); ++it){
        QStringList KVPair = {it->first.c_str(), it->second.c_str()};
        if(parent == nullptr){
            list.append(new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr), KVPair));
        }
        else{
            new QTreeWidgetItem(parent, KVPair);
        }
    }
    for(auto it = kv.childs.begin(); it != kv.childs.end(); ++it){
        QStringList sectionName = {it->first.c_str(), " "};
        QList<QTreeWidgetItem *> section;
        QTreeWidgetItem *item;
        if(parent == nullptr){
            item = new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr), sectionName);
            list.append(item);
        }
        else{
            item = new QTreeWidgetItem(parent, sectionName);
        }
        traverseTree(*it->second, section, item);
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    std::ostringstream out;
    std::ifstream file(argv[1]);
    auto tree = tyti::vdf::read<tyti::vdf::multikey_object>(file);
    out << "Timberjack - " << tree.name;
    w.setWindowTitle(QString(out.str().c_str()));
    QList<QTreeWidgetItem *> items;
    traverseTree(tree, items);
    w.getTree()->insertTopLevelItems(0, items);
    w.show();
    return a.exec();
}
