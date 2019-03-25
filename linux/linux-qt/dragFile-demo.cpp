#ifndef XDROPFILEWIDGET_H
#define XDROPFILEWIDGET_H
 
#include <QWidget>
 
class XDropFileWidget : public QWidget
{
    Q_OBJECT
public:
    explicit XDropFileWidget(QWidget *parent = 0);
 
 
protected:
    ///判断是否为有效的文件
    virtual bool IsValidDragFile(QDropEvent *e);
 
    ///接受目录
    /// @note 遍例目录，调用AcceptFile
    virtual void AcceptFolder(QString folder);
 
    ///接受文件
    virtual void AcceptFile(QString pathfile);
 
// QWidget interface
protected:
    virtual void dragEnterEvent(QDragEnterEvent *e) override;
    virtual void dragMoveEvent(QDragMoveEvent *e) override;
    virtual void dropEvent(QDropEvent *e) override;
 
 
 
};
 
#endif // XDROPFILEWIDGET_H




#include "xdropfilewidget.h"
 
#include <QDir>
#include <QFile>
#include <QDropEvent>
#include <QMimeData>
 
XDropFileWidget::XDropFileWidget(QWidget *parent) : QWidget(parent)
{
    setAcceptDrops(true);
}
 
bool XDropFileWidget::IsValidDragFile(QDropEvent *e)
{
    QString text = e->mimeData()->text();
    bool bre = false;
    do
    {
        // 存在多个文件时，只判断第一个
        QStringList files = text.split("\n");
        if(files.size() < 1)
        {
            break;
        }
 
        int pos = files[0].indexOf("file:///");
        if(0 != pos)
        {
            break;
        }
 
        //去掉拖拽头标 file:///
        QString filename = files[0].mid(8);
 
        // 接受目录
        QDir dir(filename);
        if (dir.exists())
        {
            bre = true;
            break;
        }
 
        if(QFile::exists(filename))
        {
            bre = true;
            break;
        }
 
    }while(false);
 
    return bre;
}
 
void XDropFileWidget::AcceptFolder(QString folder)
{
    QDir dir(folder);
 
    //文件
    QFileInfoList file_list = dir.entryInfoList(QDir::Files | QDir::NoSymLinks);
 
    for(int i = 0; i < file_list.size(); i++)
    {
        QString fileName = file_list[i].filePath();
        AcceptFile(fileName);
    }
 
    //目录
    QFileInfoList folder_list = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
    for(int i = 0; i < folder_list.size(); i++)
    {
        AcceptFolder(folder_list[i].absoluteFilePath());
    }
}
 
void XDropFileWidget::AcceptFile(QString pathfile)
{
 
}
 
void XDropFileWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if(IsValidDragFile(event))
    {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}
 
void XDropFileWidget::dragMoveEvent(QDragMoveEvent *event)
{
    if(IsValidDragFile(event))
    {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}
 
void XDropFileWidget::dropEvent(QDropEvent *event)
{
    QStringList files = event->mimeData()->text().split("\n");
    for(int i = 0; i < files.size(); i++)
    {
        QString fileName = files[i].mid(8); //  去掉头file:///
 
        // 接受目录
        QDir dir(fileName);
        if (dir.exists())
        {
            AcceptFolder(fileName);
            continue;
        }
 
        if(QFile::exists(fileName))
        {
            AcceptFile(fileName);
            continue;
        }
    }
}


