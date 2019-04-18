QPoint Utils::Misc::screenCenter(const QWidget *w)
{
    // Returns the QPoint which the widget will be placed center on screen (where parent resides)

    QWidget *parent = w->parentWidget();
    QDesktopWidget *desktop = QApplication::desktop();
    int scrn = desktop->screenNumber(parent);  // fallback to `primaryScreen` when parent is invalid
    QRect r = desktop->availableGeometry(scrn);
    return QPoint(r.x() + (r.width() - w->frameSize().width()) / 2, r.y() + (r.height() - w->frameSize().height()) / 2);
}

