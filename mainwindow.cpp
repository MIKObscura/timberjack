#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QtWidgets/QTreeWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QTreeWidget* MainWindow::getTree(){
    return ui->treeWidget;
}
