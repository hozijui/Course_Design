#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtGui>
#include <QtGui/QtGui>
#include <QtCore>
#include <QtCore/QtCore>

MainWindow::MainWindow(QWidget *parent)
    :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
