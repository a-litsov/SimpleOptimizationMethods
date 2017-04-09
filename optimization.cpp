#include "optimization.h"
#include <QVector>
#include <QtAlgorithms>
#include <math.h>
double Optimization::calculateFunc(double x)
{
//    return alpha * sin(betta*x) + gamma * cos(betta*x);
//    return sin(x); - некорректно, т.к. синус заметно медленнее растёт с ростом аргумента
    return x*x; // корректно, т.к. здесь обратная ситуация
// Если же брать за условие останова x[.]-x[..] < eps, то получим полный рандом
// Всё это говорит о неэффективности метода полного перебора, разве что в конце еще раз искать
// минимум среди найденных значений
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

void insertInOrder(double value, QVector<double>& array)
{
    QVector<double>::iterator it = qLowerBound(array.begin(), array.end(), value); // find proper position in descending order
    array.insert(it, value); // insert before iterator it
}

double Optimization::bruteforceMethod(QVector<QPair<double, double>>& iterations, double start, double end, int maxIterCount, double eps)
{
    int iter = 0; double currentError = 1000000;
    QVector<double> x; QVector<double> y;
    x.push_back(start); x.push_back(end);
    y.push_back(calculateFunc(start)); y.push_back(calculateFunc(end));
    QVector<double> R;
    double newPoint, newValue;
    while (iter < maxIterCount && currentError > eps)
    {
        y.clear();
        for (int i = 0; i < x.size(); i++)
            y.push_back(calculateFunc(x[i]));
        R.clear();
        for (int i = 0; i < x.size()-1; i++)
            R.push_back(fabs(x[i+1] - x[i]));
        int maxIndex = 0;
        double maxValue = 0;
        for (int i = 0; i < R.size(); i++)
            if (R[i] > maxValue)
            {
                maxValue = R[i];
                maxIndex = i;
            }
        currentError = fabs(y[maxIndex+1]-y[maxIndex]);
        newPoint = 0.5*(x[maxIndex]+x[maxIndex+1]);
        newValue = calculateFunc(newPoint);
        insertInOrder(newPoint, x);
//        insertInOrder(newValue, y);
        iterations.push_back(QPair<double, double>(newPoint, newValue));
        iter++;
    }
    y.push_back(calculateFunc(newPoint));
    return y[y.size()-1];
}
