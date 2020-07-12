#ifndef HISTORYWINDOW_H
#define HISTORYWINDOW_H

#include <QWidget>
#include <QTextStream>
#include <QFile>
#include <QFileInfo>
#include <QIODevice>
#include <QDir>
#include <QPoint>
#include <QMenu>
#include <QListWidget>

#include "qoolutils.h"

#include <QDebug>

namespace Ui {
class HistoryWindow;
}

class HistoryWindow : public QWidget
{
    Q_OBJECT

public:
    explicit HistoryWindow(QWidget *parent = 0);
    ~HistoryWindow();

private:
    Ui::HistoryWindow *ui;

    QoolUtils *util;
    QString mFilePath;

    //! Loads the currently saved history items to the window
    void loadHistory();


public slots:

    //! Creates a custom context menu on right click
    void openContextMenu(const QPoint&);

    //! Open bookmark in browser window
    void openItem();

    //!Delete bookmark from bookmarks list
    void deleteItem();

    //!Refresh bookmarks list
    void refreshList();

    //! Emits requestedUrl(QString) passing the selected history item to the browser main window
    void slotEmitUrlToMainWindow(QListWidgetItem*);

signals:
    void requestedUrl(QString);


};

#endif // HISTORYWINDOW_H
