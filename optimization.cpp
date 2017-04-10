#include <QVector>
#include <QtAlgorithms>
#include <math.h>
#include "optimization.h"
#include <assert.h>

const double INF = 1000000;

double Optimization::calculateFunc(double x)  const
{
    return params[0] * sin(params[1]*x) + params[2] * cos(params[3]*x);
//    return sin(x);
//    return x*x;
//    return cos(x) + sin(x);
}

QVector<double> Optimization::getFuncData(QVector<double> &x, double start, double end, int n) const
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

void Optimization::setParameters(double params[4])
{
    for (int i = 0; i < 4; i++)
        this->params[i] = params[i];
}

int insertInOrder(double value, QVector<double>& array)
{
    QVector<double>::iterator it = qLowerBound(array.begin(), array.end(), value); // find proper position in descending order
    QVector<double>::iterator valueIt = array.insert(it, value); // insert before iterator it
    return valueIt - array.begin(); // return our element's index
}

void insertAtPos(double value, int pos, QVector<double>& array)
{
    array.insert(array.begin()+pos, value);
}

// Inserts with neighbour items redefinition
QVector<double>::iterator insertToR(double before, double value, double after, int pos, QVector<double>& array)
{
    QVector<double>::iterator valIt = array.erase(array.begin() + pos - 1);
    valIt = array.insert(valIt, after - value);
    return array.insert(valIt, value - before);
}

int findMax(QVector<double> array, double& maxValue)
{
    maxValue = 0;
    int maxIndex = 0;
    for (int i = 0; i < array.size(); i++)
        if (array[i] > maxValue)
        {
            maxValue = array[i];
            maxIndex = i;
        }
    return maxIndex;
}

QPair<double, double> Optimization::bruteforceMethod(QVector<QPair<double, double>>& iterations, double start, double end, int maxIterCount, double eps)  const
{
    // Initialization
    QPair<double, double> globalMin(0, INF);
    QVector<double> x; QVector<double> y;
    x.push_back(start); x.push_back(end);
    y.push_back(calculateFunc(start)); y.push_back(calculateFunc(end));
    QVector<double> R;
    R.push_back(fabs(end - start));

    // Main work
    for (int iter = 0; iter < maxIterCount; iter++)
    {
        double maxValue = 0;
        int maxIndex = findMax(R, maxValue);
        // Isolates currentError variable
        {
            double currentError = fabs(x[maxIndex+1]-x[maxIndex]);
            if (currentError < eps)
                break;
        }
        double newPoint = 0.5*(x[maxIndex]+x[maxIndex+1]);
        double newValue = calculateFunc(newPoint);
        if (newValue < globalMin.second)
        {
            globalMin.first = newPoint;
            globalMin.second = newValue;
        }
        int pos = insertInOrder(newPoint, x);
        insertAtPos(newValue, pos, y);
        insertToR(x[pos-1], newPoint, x[pos+1], pos, R);
        iterations.push_back(QPair<double, double>(newPoint, newValue));
    }
    return globalMin;
}

double Optimization::getM(const QVector<double>& x, const double& r) const
{
    double M = 0;
    for (int i = 0; i < x.size() - 1; i++)
    {
        double value = fabs((calculateFunc(x[i+1]) - calculateFunc(x[i])) / (x[i+1] - x[i]));
        if (value > M)
            M = value;
    }
    assert(M >= 0);
    double m;
    if (M > 0)
    {
        m = r * M;
    } else {
        if (M == 0)
            m = 1;
    }
    return m;
}

double Optimization::calculateRPiavskii(double after, double before, double m) const
{
    return 0.5*m*(after-before) - (calculateFunc(after)+calculateFunc(before))/2;
}

QVector<double>::iterator Optimization::insertToRPiavskii(double before, double value, double after, int pos, double m, QVector<double>& array) const
{
    QVector<double>::iterator valIt = array.erase(array.begin() + pos - 1);
    valIt = array.insert(valIt, calculateRPiavskii(after, value, m));
    return array.insert(valIt, calculateRPiavskii(value, before, m));
}

QPair<double, double> Optimization::PiavskiiMethod(QVector<QPair<double, double>>& iterations, double start, double end, int maxIterCount, double eps, double r)  const
{
    assert(r > 1);
    // Initialization
    QPair<double, double> globalMin(0, INF);
    QVector<double> x; QVector<double> y;
    x.push_back(start); x.push_back(end);
    y.push_back(calculateFunc(start)); y.push_back(calculateFunc(end));
    QVector<double> R;
    double m = getM(x, r);
    R.push_back(calculateRPiavskii(end, start, m));


    // Main work
    for (int iter = 0; iter < maxIterCount; iter++)
    {
        double maxValue = 0;
        int maxIndex = findMax(R, maxValue);
        // Isolates currentError variable
        {
            double currentError = fabs(x[maxIndex+1]-x[maxIndex]);
            if (currentError < eps)
                break;
        }
        double newPoint = (x[maxIndex+1]+x[maxIndex])/2 - (calculateFunc(x[maxIndex+1]) - calculateFunc(x[maxIndex])) / (2 * m);
        double newValue = calculateFunc(newPoint);
        if (newValue < globalMin.second)
        {
            globalMin.first = newPoint;
            globalMin.second = newValue;
        }
        int pos = insertInOrder(newPoint, x);
        insertAtPos(newValue, pos, y);
        m = getM(x, r);
        insertToRPiavskii(x[pos-1], newPoint, x[pos+1], pos, m, R);
        iterations.push_back(QPair<double, double>(newPoint, newValue));
    }
    return globalMin;
}

double Optimization::calculateRStrongin(double after, double before, double m) const
{
    double afterValue = calculateFunc(after), beforeValue = calculateFunc(before);
    return m*(after-before)+pow(afterValue - beforeValue, 2)/(m*(after-before)) - 2*(afterValue + beforeValue);
}

QVector<double>::iterator Optimization::insertToRStrongin(double before, double value, double after, int pos, double m, QVector<double>& array) const
{
    QVector<double>::iterator valIt = array.erase(array.begin() + pos - 1);
    valIt = array.insert(valIt, calculateRStrongin(after, value, m));
    return array.insert(valIt, calculateRStrongin(value, before, m));
}

QPair<double, double> Optimization::StronginMethod(QVector<QPair<double, double>>& iterations, double start, double end, int maxIterCount, double eps, double r)  const
{
    assert(r > 1);
    // Initialization
    QPair<double, double> globalMin(0, INF);
    QVector<double> x; QVector<double> y;
    x.push_back(start); x.push_back(end);
    y.push_back(calculateFunc(start)); y.push_back(calculateFunc(end));
    QVector<double> R;
    double m = getM(x, r);
    R.push_back(calculateRStrongin(end, start, m));


    // Main work
    for (int iter = 0; iter < maxIterCount; iter++)
    {
        double maxValue = 0;
        int maxIndex = findMax(R, maxValue);
        // Isolates currentError variable
        {
            double currentError = fabs(x[maxIndex+1]-x[maxIndex]);
            if (currentError < eps)
                break;
        }
        double newPoint = (x[maxIndex+1]+x[maxIndex])/2 - (calculateFunc(x[maxIndex+1]) - calculateFunc(x[maxIndex])) / (2 * m);
        double newValue = calculateFunc(newPoint);
        if (newValue < globalMin.second)
        {
            globalMin.first = newPoint;
            globalMin.second = newValue;
        }
        int pos = insertInOrder(newPoint, x);
        insertAtPos(newValue, pos, y);
        m = getM(x, r);
        insertToRStrongin(x[pos-1], newPoint, x[pos+1], pos, m, R);
        iterations.push_back(QPair<double, double>(newPoint, newValue));
    }
    return globalMin;
}
