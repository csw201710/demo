#include "main_window.h"
#include "utils.h"
#include <QApplication>
#include <QMouseEvent>
#include <QPainter>
#include <QThread>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    mainWidget = new QWidget();
    layout = new QHBoxLayout();
    networkLayout = new QVBoxLayout();
    memoryMonitor = new MemoryMonitor();
    downloadLabel = new QLabel("↓0.0K");
    uploadLabel = new QLabel("↑0.0K");
    thread = new Thread();

    QAction *quitAction = new QAction("退出", this);
    menu = new QMenu;
    menu->addAction(quitAction);

    downloadLabel->setStyleSheet("QLabel { font-size: 13px; color: #2E6843; }");
    uploadLabel->setStyleSheet("QLabel { font-size: 13px; color: #FF0000; }");

    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::X11BypassWindowManagerHint);

    networkLayout->setMargin(0);
    networkLayout->setSpacing(0);
    networkLayout->addStretch();
    networkLayout->addWidget(uploadLabel);
    networkLayout->addWidget(downloadLabel);
    networkLayout->addStretch();

    layout->setMargin(0);
    layout->addWidget(memoryMonitor);
    layout->addLayout(networkLayout);

    mainWidget->setLayout(layout);
    setCentralWidget(mainWidget);

    connect(quitAction, &QAction::triggered, this, &QApplication::quit);
    connect(thread, &Thread::updateNetworkSpeed, this, &MainWindow::updateNetworkSpeed);

    thread->start();
}

MainWindow::~MainWindow()
{

}

void MainWindow::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor("#F1F1F1"));
    painter.drawRect(rect());
}

void MainWindow::mouseMoveEvent(QMouseEvent *e)
{
    if (e->buttons() & Qt::LeftButton)
        move(e->globalPos() - p);
}

void MainWindow::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        p = e->globalPos() - frameGeometry().topLeft();
    }else if (e->button() == Qt::RightButton)
        menu->exec(QCursor::pos());
}

void MainWindow::updateNetworkSpeed(QString upload, QString download)
{
    uploadLabel->setText(upload);
    downloadLabel->setText(download);
}
