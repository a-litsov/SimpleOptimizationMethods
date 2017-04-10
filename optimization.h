#ifndef OPTIMIZATION_H
#define OPTIMIZATION_H

#include <QVector>

class Optimization
{
    double alpha, betta, gamma, delta;
    double getM(const QVector<double>& x, const double& r) const;
    QVector<double>::iterator insertToRPiavskii(double before, double value, double after, int pos, double m, QVector<double>& array) const;
public:
    double calculateFunc(double x) const;
    QVector<double> getFuncData(QVector<double> &x, double start, double end, int n) const;
    void setParameters(double alpha, double betta, double gamma, double delta);
    QPair<double, double> bruteforceMethod(QVector<QPair<double, double>>& iterations,
                                           double start, double end, int maxIterCount, double eps) const;
    QPair<double, double> PiavskiiMethod(QVector<QPair<double, double>>& iterations,
                   double start, double end, int maxIterCount, double eps, double r)  const;
};

#endif // OPTIMIZATION_H
