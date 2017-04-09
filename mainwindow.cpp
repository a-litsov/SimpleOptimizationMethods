#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->customPlot->setMinimumWidth(this->width());
    ui->customPlot->setMinimumHeight(this->height()-100);

}

MainWindow::~MainWindow()
{
    delete ui;
}
