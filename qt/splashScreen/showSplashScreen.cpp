#include <QPainter>
#include <QSplashScreen>
#include <QTimer>
void showSplashScreen()
{
    QPixmap splashImg(":/icons/logo/splash");
    QPainter painter(&splashImg);
    QString version = "version 1.2.3";
    painter.setPen(QPen(Qt::white));
    painter.setFont(QFont("Arial", 22, QFont::Black));
    painter.drawText(224 - painter.fontMetrics().width(version), 270, version);
    QSplashScreen *splash = new QSplashScreen(splashImg);
    splash->show();
    QTimer::singleShot(2500, splash, &QObject::deleteLater);
    qApp->processEvents();
}

