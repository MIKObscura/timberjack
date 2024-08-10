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

void traverseTree(tyti::vdf::object &kv, QList<QTreeWidgetItem *> &list, QTreeWidgetItem *parent = nullptr){
    
    for(auto it = kv.attribs.begin(); it != kv.attribs.end(); ++it){
        QStringList KVPair = {it->first.c_str(), it->second.c_str()};
        if(parent == nullptr){
            list.append(new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr), KVPair));
        }
        else{
            new QTreeWidgetItem(parent, KVPair);
        }
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    std::ostringstream out;
    std::ifstream file(argv[1]);
    auto tree = tyti::vdf::read(file);
    out << "Timberjack - " << tree.name;
    w.setWindowTitle(QString(out.str().c_str()));
    QList<QTreeWidgetItem *> items;
    traverseTree(tree, items);
    w.getTree()->insertTopLevelItems(0, items);
    w.show();
    return a.exec();
}
