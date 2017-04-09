#ifndef OPTIMIZATION_H
#define OPTIMIZATION_H

#include <QVector>

class Optimization
{
    double alpha, betta, gamma, delta;
public:
    double calculateFunc(double x);
    QVector<double> getFuncData(QVector<double> &x, double start, double end, int n);
    void setParameters(double alpha, double betta, double gamma, double delta);
    QPair<double, double> bruteforceMethod(QVector<QPair<double, double>>& iterations, double start, double end, int maxIterCount, double eps);
};

#endif // OPTIMIZATION_H
