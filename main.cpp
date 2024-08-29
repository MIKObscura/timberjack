#include "mainwindow.h"
#include "vdf_parser.hpp"
#include <QApplication>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QTreeWidgetItem>
#include <QList>
#include <QString>
#include <QShortcut>
#include <QKeySequence>
#include <QObject>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <utility>

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
        QTreeWidgetItem *item;
        if(parent == nullptr){
            item = new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr), KVPair);
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsEditable);
            list.append(item);
        }
        else{
            item = new QTreeWidgetItem(parent, KVPair);
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsEditable);
        }
    }
    for(auto it = kv.childs.begin(); it != kv.childs.end(); ++it){
        QStringList sectionName = {it->first.c_str(), " "};
        QList<QTreeWidgetItem *> section;
        QTreeWidgetItem *item;
        if(parent == nullptr){
            item = new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr), sectionName);
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsEditable);
            list.append(item);
        }
        else{
            item = new QTreeWidgetItem(parent, sectionName);
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsEditable);
        }
        traverseTree(*it->second, section, item);
    }
}

void writeChanges(QTreeWidgetItem &item, tyti::vdf::multikey_object &parent){
    int children = item.childCount();
    if(children == 0){
        parent.add_attribute(item.text(0).toStdString(), item.text(1).toStdString());
        return;
    }
    for(int i = 0; i < children; i++){
        QTreeWidgetItem* child = item.child(i);
        if(child->childCount() > 0){
            tyti::vdf::multikey_object child_object;
            child_object.set_name(child->text(0).toStdString());
            writeChanges(*child, child_object);
            parent.add_child(std::make_unique<tyti::vdf::multikey_object>(child_object));
        } else{
            parent.add_attribute(child->text(0).toStdString(), child->text(1).toStdString());
        }
    }
}

void save(QTreeWidget* tree, auto name, char* filename){
    tyti::vdf::multikey_object root;
    root.set_name(name);
    int itemCount = tree->topLevelItemCount();
    for(int i = 0; i < itemCount; i++){
        auto item = tree->topLevelItem(i);
        if(item->childCount() > 0){
            tyti::vdf::multikey_object child_object;
            child_object.set_name(item->text(0).toStdString());
            writeChanges(*item, child_object);
            root.add_child(std::make_unique<tyti::vdf::multikey_object>(child_object));
        } else {
            root.add_attribute(item->text(0).toStdString(), item->text(1).toStdString());
        }
    }
    std::stringstream ss;
    ss << "tj_" << filename;
    std::string outputFilename = ss.str();
    std::ofstream fout(outputFilename);
    tyti::vdf::write(fout, root);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    std::ostringstream title;
    std::ifstream file(argv[1]);
    auto tree = tyti::vdf::read<tyti::vdf::multikey_object>(file);
    if(tree.name.empty()){ // if the vdf tree has no name, use the file name
        title << "Timberjack - " << argv[1];
    }
    else{
        title << "Timberjack - " << tree.name;
    }
    w.setWindowTitle(QString(title.str().c_str()));
    QList<QTreeWidgetItem *> items;
    traverseTree(tree, items);
    w.getTree()->insertTopLevelItems(0, items);
    QShortcut saveShortcut(&w);
    saveShortcut.setKeys(QKeySequence::Save);
    QObject::connect(&saveShortcut, &QShortcut::activated, &saveShortcut, [&]() {save(w.getTree(), tree.name, argv[1]);});
    w.show();
    return a.exec();
}
