#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "optimization.h"
#include <vector>

#define customPlot ui->customPlot

QVector<QCPItemLine*> lines;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupUI();


}

void MainWindow::getData(double params[4], double& startX, double& endX, double& eps, double& iterCount, double& r, int& methodNum)
{
    getPlotData(params, startX, endX);
    eps = ui->errorBox->value();
    iterCount = ui->itersBox->value();
    r = ui->rBox->value();
    methodNum = ui->comboBox->currentIndex();
}

void MainWindow::getPlotData(double params[4], double& startX, double& endX)
{
    QDoubleSpinBox* boxes[] = {ui->alphaBox, ui->bettaBox, ui->gammaBox, ui->deltaBox};
    for (int i = 0; i < 4; i++)
        params[i] = boxes[i]->value();
    startX = ui->startBox->value();
    endX = ui->endBox->value();
}

void MainWindow::setupUI()
{
    QRect rec = QApplication::desktop()->screenGeometry();
    this->setGeometry(rec);

    customPlot->setMinimumWidth(this->width());
    customPlot->setMinimumHeight(this->height()-110);
}

void MainWindow::createGraph(const Optimization& optObj, int n, double startX, double endX, double startY, double endY)
{

    QVector<double> x(n);
    QVector<double> y = optObj.getFuncData(x, startX, endX, n);

    // create graph and assign data to it:
    customPlot->addGraph();
    QPen pen;
    pen.setStyle(Qt::SolidLine);
    pen.setColor(Qt::blue);
    pen.setWidth(2);
    customPlot->graph(0)->setPen(pen);
    customPlot->graph(0)->setData(x, y);

    // give the axes some labels:
    customPlot->xAxis->setLabel("x");
    customPlot->yAxis->setLabel("y");

    // set axes ranges, so we see all data:
    customPlot->xAxis->setRange(startX, endX);
    customPlot->yAxis->setRange(startY, endY);

    // set ticks count
    const int maxTicksCount = 70;

    customPlot->xAxis->ticker()->setTickCount(maxTicksCount);

    customPlot->xAxis->setTickLabelRotation(40);

    // remove lines if some method already worked
    for (int i = 0; i < lines.size(); i++)
        if (customPlot->hasItem(lines[i]))
            customPlot->removeItem(lines[i]);

    customPlot->replot();
}

void MainWindow::showIterations(const QVector<QPair<double, double>>& iterations, const QPair<double, double>& min, double startY)
{
    for (int i = 0; i < lines.size(); i++)
    {
        if (customPlot->hasItem(lines[i]))
            customPlot->removeItem(lines[i]);
    }
    lines.clear();
    for (int i = 0; i < iterations.size(); i++)
    {
        drawLine(iterations[i], startY, i, iterations[i].first == min.first, false, false);
    }
    customPlot->replot();
}

void MainWindow::drawLine(QPair<double, double> iteration, double startY, int iterNum, bool isFinal, bool showIter, bool showX)
{
    QPen pen;
    pen.setStyle(Qt::DashLine);
    pen.setColor(isFinal ? Qt::green : Qt::red);
    const int finalWidth = 2;
    if (isFinal)
        pen.setWidth(finalWidth);

    QCPItemLine *min1 = new QCPItemLine(customPlot);
    min1->setPen(pen);
    min1->start->setCoords(iteration.first, startY);
    min1->end->setCoords(iteration.first, iteration.second);
    lines.push_back(min1);

    QCPItemLine *min2 = new QCPItemLine(customPlot);
    min2->setPen(pen);
    const float shiftY = 0.1;
    min2->start->setCoords(iteration.first, iteration.second);
    min2->end->setCoords(iteration.first, iteration.second+shiftY);
    lines.push_back(min2);

    if (showX)
    {
        QCPItemText *textLabel = new QCPItemText(customPlot);
        textLabel->setPositionAlignment(Qt::AlignBottom|Qt::AlignHCenter);
        textLabel->position->setType(QCPItemPosition::ptPlotCoords);
        textLabel->position->setCoords(iteration.first, iteration.second+shiftY);
        textLabel->setText(QString::number(iteration.first));
    }

    if (showIter)
    {
        QCPItemText *textLabel2 = new QCPItemText(customPlot);
        textLabel2->setPositionAlignment(Qt::AlignBottom|Qt::AlignHCenter);
        textLabel2->position->setType(QCPItemPosition::ptPlotCoords);
        textLabel2->position->setCoords(iteration.first+shiftY/10, iteration.second/2);
        textLabel2->setText(QString::number(iterNum));
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_startButton_clicked()
{
    Optimization optObj;
    const double startY = -2, endY = 2;
    double params[4], startX, endX, eps, iterCount, r;
    int methodNum = 0;
    getData(params, startX, endX, eps, iterCount, r, methodNum);
    optObj.setParameters(params);
    QVector<QPair<double, double>> iterations;
    QPair<double, double> min;
    switch(methodNum)
    {
    case 0:
        min = optObj.bruteforceMethod(iterations, startX, endX, iterCount, eps);
        break;
    case 1:
        min = optObj.PiavskiiMethod(iterations, startX, endX, iterCount, eps, r);
        break;
    case 2:
        min = optObj.StronginMethod(iterations, startX, endX, iterCount, eps, r);
        break;
    }

    ui->statusBar->showMessage(QString("Готово, точка глобального минимума: (" + QString::number(min.first) + ";" + QString::number(min.second)
                                       + "), количество итераций: " + QString::number(iterations.size())));
    showIterations(iterations, min, startY);
}

void MainWindow::on_drawButton_clicked()
{
    Optimization optObj;
    const double startY = -2, endY = 2;
    double params[4], startX, endX;
    getPlotData(params, startX, endX);
    optObj.setParameters(params);

    const int n = 50 * fabs(endX - startX);
    createGraph(optObj, n, startX, endX, startY, endY);
}
