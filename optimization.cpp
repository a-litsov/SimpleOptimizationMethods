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

int insertInOrder(double value, QVector<double>& array)
{
    QVector<double>::iterator it = qLowerBound(array.begin(), array.end(), value); // find proper position in descending order
    QVector<double>::iterator valueIt = array.insert(it, value); // insert before iterator it
    return valueIt - array.begin(); // return our element's index
}

void insertAtPos(double value, int pos, QVector<double>& array)
{
    if( array.begin()+pos > array.end() || array.begin()+pos < array.begin())
        int a= 7;
    array.insert(array.begin()+pos, value);
}

QVector<double>::iterator insertToR(double before, double value, double after, int pos, QVector<double>& array)
{
    QVector<double>::iterator valIt = array.erase(array.begin() + pos - 1);
    valIt = array.insert(valIt, after - value);
    return array.insert(valIt, value - before);
}

QPair<double, double> Optimization::bruteforceMethod(QVector<QPair<double, double>>& iterations, double start, double end, int maxIterCount, double eps)
{
    double globalMin = 10000000;
    int iter = 0; double currentError = 1000000;
    QVector<double> x; QVector<double> y;
    x.push_back(start); x.push_back(end);
    y.push_back(calculateFunc(start)); y.push_back(calculateFunc(end));
    QVector<double> R;
    R.push_back(fabs(end - start));
    double newPoint, newValue;
    while (iter < maxIterCount && currentError > eps)
    {
        int maxIndex = 0;
        double maxValue = 0;
        for (int i = 0; i < R.size(); i++)
            if (R[i] > maxValue)
            {
                maxValue = R[i];
                maxIndex = i;
            }
        currentError = fabs(y[maxIndex+1]-y[maxIndex]);
        // Поправить
//        if (currentError < eps)
//            break;
        newPoint = 0.5*(x[maxIndex]+x[maxIndex+1]);
        newValue = calculateFunc(newPoint);
        if (newValue < globalMin)
            globalMin = newValue;
        int pos = insertInOrder(newPoint, x);
        insertAtPos(newValue, pos, y);
        QVector<double>::iterator tmpIt = insertToR(x[pos-1], newPoint, x[pos+1], pos, R);
        iterations.push_back(QPair<double, double>(newPoint, newValue));
        iter++;
    }
    return QPair<double, double>(newPoint, y[y.size()-1]);
}
