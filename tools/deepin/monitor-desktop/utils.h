#ifndef UTILS_H
#define UTILS_H

#include <QObject>

namespace Utils
{
    void getMemoryPercent(float &percent);
    void getNetworkBandWidth(unsigned long long int &receiveBytes, unsigned long long int &sendBytes);
    QString networkConversion(long bytes);
}

#endif // UTILS_H
