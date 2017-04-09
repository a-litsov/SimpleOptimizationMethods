#include "optimization.h"
#include <QVector>
#include <QtAlgorithms>

double Optimization::calculateFunc(double x)
{
//    return alpha * sin(betta*x) + gamma * cos(betta*x);
    return x*x;
}

QVector<double> Optimization::getFuncData(QVector<double> &x, double start, double end, int n)
{
    double interval = (end - start) / (n-1);
    QVector<double> values(n);
    for (int i = 0; i < n; i++)
    {
        x[i] = start + interval * i;
        values[i] = calculateFunc(x[i]);
    }
    return values;
}

void Optimization::setParameters(double alpha, double betta, double gamma, double delta)
{
    this->alpha = alpha;
    this->betta = betta;
    this->gamma = gamma;
    this->delta = delta;
}

double Optimization::bruteforceMethod(QVector<double> &x, double start, double end, int maxIterCount, double eps)
{
    int iter = 0; double currentError = 1000000;
    x.clear();
    x.push_back(start); x.push_back(end);
    QVector<double> R;
    R.push_back(end - start);
    QVector<double> values;
    while (iter < maxIterCount && currentError > eps)
    {
        qSort(x);
        for (int i = 0; i < x.size(); i++)
            values[i] = calculateFunc(x[i]);
        for (int i = 0; i < x.size()-1; i++)
            R.push_back(x[i+1] - x[i]);
        int maxIndex = 0;
        int maxValue = 0;
        for (int i = 0; i < R.size(); i++)
            if (R[i] > maxValue)
            {
                maxValue = R[i];
                maxIndex = i;
            }
        x.push_back(0.5*(x[maxIndex]+x[maxIndex+1]));
    }
}
