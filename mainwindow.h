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

public:
    explicit MainWindow(QWidget *parent = 0);
    void createGraph(const Optimization& optObj, int n , double eps, double startX, double endX, double startY, double endY);
    void drawLine(QPair<double, double> iteration, double startY, int iterNum, bool isFinal, bool showIter, bool showX);
    void showIterations(const QVector<QPair<double, double>>& iterations, const QPair<double, double>& min, double startY);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
