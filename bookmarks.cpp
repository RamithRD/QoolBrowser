#include "bookmarks.h"
#include "ui_bookmarks.h"

Bookmarks::Bookmarks(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Bookmarks)
{
    ui->setupUi(this);
    QWidget::setWindowTitle("Qool Bookmarks");
    QWidget::setWindowIcon(QIcon(":/Icons/Assets/browser_icon.png"));


    util = new QoolUtils();

    // Sets the absolute path of the current application directory to store the bookmarks file
    mFilePath = QDir::currentPath();

    /* Context menu policy of the listwidget is set to CustomeContextMenu which emits
       the customeContextMenuRequested signal so I can create a custom context menu on right click*/
    ui->listWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    //The signal is then connected to my custom slot
    connect(ui->listWidget, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(openContextMenu(QPoint)));

    connect(ui->listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
            this, SLOT(slotEmitUrlToMainWindow(QListWidgetItem*)));

    // Loads the bookmarks list from file
    loadBookmarks();

}

Bookmarks::~Bookmarks()
{
    delete ui;
}

void Bookmarks::slotEmitUrlToMainWindow(QListWidgetItem* item)
{
    emit requestedUrl(util->extractUrl(item->text()));
}

void Bookmarks::loadBookmarks()
{
    if(QFileInfo(mFilePath+"/storage/bookmarks.qool").exists()){
        //The file exists


    }else{

        //The file doesn't exist

        QDir dir;
        dir.mkpath(mFilePath+"/storage");

        //if file not exists, txt file is created in the parent directory under storage folder
        QFile f(mFilePath+"/storage/bookmarks.qool");
        f.open(QIODevice::ReadWrite);

    }


    QString outputFilename = "bookmarks.qool";
    QFile bookmarkFile(mFilePath+"/storage/"+outputFilename);
    bookmarkFile.open(QIODevice::ReadWrite);

    /* Point a QTextStream object at the file */
    QTextStream txtStream(&bookmarkFile);

    QString line = txtStream.readLine();

    while (!line.isNull())
    {
        new QListWidgetItem(line, ui->listWidget);
        line = txtStream.readLine();
    }

    bookmarkFile.close();
}

void Bookmarks::openContextMenu(const QPoint &pos)
{
    // Handle global position
    QPoint globalPos = ui->listWidget->mapToGlobal(pos);

    // Create menu and insert some actions
    QMenu rightClickMenu;
    rightClickMenu.addAction("Open", this, SLOT(openItem()));
    rightClickMenu.addAction("Delete",  this, SLOT(deleteItem()));
    rightClickMenu.addAction("Refresh",  this, SLOT(refreshList()));

    // Show context menu at handling position
    rightClickMenu.exec(globalPos);
}

void Bookmarks::openItem()
{
    // Passes the url requested to be opened by the user to the browser main window
    emit requestedUrl(util->extractUrl(ui->listWidget->currentItem()->text()));
}

void Bookmarks::deleteItem()
{
    //The selected bookmark from the list is deleted  permanently

    QFile bookmarkFile(mFilePath+"/storage/bookmarks.qool");
    if(bookmarkFile.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QString str;
        QTextStream txtStream(&bookmarkFile);
        while(!txtStream.atEnd())
        {
            QString line = txtStream.readLine();

            //Loops through all bookmark entries and deletes the requested bookmark from file
            if(!line.contains(ui->listWidget->currentItem()->text()))
                str.append(line + "\n");
        }
        bookmarkFile.resize(0);
        txtStream << str;
        bookmarkFile.close();
    }
}

void Bookmarks::refreshList()
{
    //Reloads the bookmark list to the window

    ui->listWidget->clear();
    loadBookmarks();
}



