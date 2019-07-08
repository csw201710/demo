#include "memory_monitor.h"
#include <QPainter>
#include <QTimer>
#include "utils.h"

MemoryMonitor::MemoryMonitor(QWidget *parent)
    : QWidget(parent)
{
    QTimer *timer = new QTimer();
    percent = 0;

    timer->start(1000);

    connect(timer, &QTimer::timeout, this, [=]{
        Utils::getMemoryPercent(percent);
        repaint();
    });

    setFixedWidth(52);
}

void MemoryMonitor::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    painter.setPen(Qt::NoPen);

    if (percent < 60)
        painter.setBrush(QColor("#1C834D"));
    else if (percent >= 60 && percent < 80)
        painter.setBrush(QColor("#A44200"));
    else if (percent >= 80)
        painter.setBrush(QColor("#940000"));

    painter.drawRect(rect());

    if (percent < 60)
        painter.setBrush(QColor("#24CF60"));
    else if (percent >= 60 && percent < 80)
        painter.setBrush(QColor("#FF6F00"));
    else if (percent >= 80)
        painter.setBrush(QColor("#FF1E1F"));

    painter.drawRect(QRect(0, 51 - percent / 2, width(), percent / 2));

    painter.setPen(QColor("#FFFFFF"));
    painter.drawText(rect(), Qt::AlignCenter, QString::number(percent, 'r', 0) + "%");
}
