#include "utils.h"
#include <QFile>
#include <QTime>
#include <QApplication>

void Utils::getMemoryPercent(float &percent)
{
    QFile file("/proc/meminfo");
    file.open(QIODevice::ReadOnly);

    QString buffer = file.readAll();
    QStringList lines = buffer.split("\n").filter(QRegExp("^MemTotal|^MemAvailable|^SwapTotal|^SwapFree"));
    QRegExp sep("\\s+");

    unsigned long long memTotal = lines.at(0).split(sep).at(1).toLong();
    unsigned long long memAvailable = lines.at(1).split(sep).at(1).toLong();
    unsigned long long swapTotal = lines.at(2).split(sep).at(1).toLong();
    unsigned long long swapFree = lines.at(3).split(sep).at(1).toLong();

    percent = (memTotal - memAvailable) * 100.0 / memTotal;

    file.close();
}

void Utils::getNetworkBandWidth(unsigned long long &receiveBytes, unsigned long long &sendBytes)
{
    QFile file("/proc/net/dev");
    file.open(QIODevice::ReadOnly);

    file.readLine();
    file.readLine();

    QString buffer;
    receiveBytes = 0;
    sendBytes = 0;

    while ((buffer = file.readLine()) != nullptr)
    {
        QStringList lines = buffer.trimmed().split(QRegExp("\\s+"));

        if (lines.first() != "lo:") {
            receiveBytes += lines.at(1).toLong();
            sendBytes += lines.at(9).toLong();
        }
    }

    file.close();
}

QString Utils::networkConversion(long bytes)
{
    if (bytes < 1024)
        return QString::number(bytes) + "B";

    if (bytes / 1024 < 1024)
        return QString::number(bytes / 1024, 'r', 1) + "K";

    if (bytes / 1024 / 1024 < 1024)
        return QString::number(bytes / 1024 / 1024, 'r', 1) + "M";
}
