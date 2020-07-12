#ifndef BOOKMARKS_H
#define BOOKMARKS_H

#include <QWidget>
#include <QTextStream>
#include <QFile>
#include <QFileInfo>
#include <QIODevice>
#include <QDir>
#include <QPoint>
#include <QMenu>
#include <QListWidget>

#include <QDebug>

#include "qoolutils.h"

namespace Ui {
class Bookmarks;
}

class Bookmarks : public QWidget
{
    Q_OBJECT

public:
    explicit Bookmarks(QWidget *parent = 0);
    ~Bookmarks();

public slots:

    //! Emits requestedUrl(QString) passing the selected bookmark to the browser main window
    void slotEmitUrlToMainWindow(QListWidgetItem*);

    //! Creates a custom context menu on right click
    void openContextMenu(const QPoint&);

    //! Open bookmark in browser window
    void openItem();

    //!Delete bookmark from bookmarks list
    void deleteItem();

    //!Refresh bookmarks list
    void refreshList();


private:
    Ui::Bookmarks *ui;

    QString mFilePath;
    QoolUtils *util;

    //! Loads all the bookmarks added by the user and loaded in to a list widget
    void loadBookmarks();


signals:
    void requestedUrl(QString);


};

#endif // BOOKMARKS_H
