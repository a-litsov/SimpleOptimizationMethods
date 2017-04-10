#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "optimization.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    void setupUI();
    void getPlotData(double params[4], double& startX, double& endX);
    void getData(double params[4], double& startX, double& endX, double& eps, double& iterCount, double& r, int& methodNum);

public:
    explicit MainWindow(QWidget *parent = 0);
    void createGraph(const Optimization& optObj, int n, double startX, double endX, double startY, double endY);
    void drawLine(QPair<double, double> iteration, double startY, int iterNum, bool isFinal, bool showIter, bool showX);
    void showIterations(const QVector<QPair<double, double>>& iterations, const QPair<double, double>& min, double startY);
    ~MainWindow();

private slots:
    void on_startButton_clicked();

    void on_drawButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
