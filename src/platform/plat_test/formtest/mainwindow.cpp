#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTableView>
#include <QStandardItemModel>
#include <QAbstractItemDelegate>

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

