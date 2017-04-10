#ifndef OPTIMIZATION_H
#define OPTIMIZATION_H

#include <QVector>

class Optimization
{
    double params[4];
    double getM(const QVector<double>& x, const double& r) const;
    double calculateRPiavskii(double after, double before, double m) const;
    QVector<double>::iterator insertToRPiavskii(double before, double value, double after, int pos, double m, QVector<double>& array) const;
    double calculateRStrongin(double after, double before, double m) const;
    QVector<double>::iterator insertToRStrongin(double before, double value, double after, int pos, double m, QVector<double>& array) const;
public:
    double calculateFunc(double x) const;
    QVector<double> getFuncData(QVector<double> &x, double start, double end, int n) const;
    void setParameters(double params[4]);
    QPair<double, double> bruteforceMethod(QVector<QPair<double, double>>& iterations,
                                           double start, double end, int maxIterCount, double eps) const;
    QPair<double, double> PiavskiiMethod(QVector<QPair<double, double>>& iterations,
                                         double start, double end, int maxIterCount, double eps, double r)  const;
    QPair<double, double> StronginMethod(QVector<QPair<double, double>>& iterations,
                                         double start, double end, int maxIterCount, double eps, double r)  const;

};

#endif // OPTIMIZATION_H
