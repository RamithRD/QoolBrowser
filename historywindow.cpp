#include "historywindow.h"
#include "ui_historywindow.h"

HistoryWindow::HistoryWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HistoryWindow)
{
    ui->setupUi(this);
    //ui->listWidget->setSelectionMode(QAbstractItemView::);
    QWidget::setWindowTitle("Qool History");
    QWidget::setWindowIcon(QIcon(":/Icons/Assets/browser_icon.png"));

    util = new QoolUtils();

    /* Context menu policy of the listwidget is set to CustomeContextMenu which emits
       the customeContextMenuRequested signal so I can create a custome contet menu on right click*/
    ui->listWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    //The signal is then connected to my custom slot
    connect(ui->listWidget, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(openContextMenu(QPoint)));

    connect(ui->listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
            this, SLOT(slotEmitUrlToMainWindow(QListWidgetItem*)));

    // Sets the absolute path of the current application directory to store the history file
    mFilePath = QDir::currentPath();

    // Loads the history list from file
    loadHistory();

}

HistoryWindow::~HistoryWindow()
{
    delete ui;
}


void HistoryWindow::loadHistory()
{

    if(QFileInfo(mFilePath+"/storage/history.qool").exists()){
        //The file exists


    }else{

        //The file doesn't exist

        QDir dir;
        dir.mkpath(mFilePath+"/storage");

        //if file not exists, txt file is created in the parent directory under storage folder
        QFile f(mFilePath+"/storage/history.qool");
        f.open(QIODevice::ReadWrite);

    }

    QString outputFilename = "history.qool";
    QFile historyFile(mFilePath+"/storage/"+outputFilename);
    historyFile.open(QIODevice::ReadWrite);

    /* Point a QTextStream object at the file */
    QTextStream txtStream(&historyFile);

    QString line = txtStream.readLine();

    while (!line.isNull())
    {
        new QListWidgetItem(line, ui->listWidget);
        line = txtStream.readLine();
    }

    historyFile.close();
}

void HistoryWindow::openContextMenu(const QPoint &pos)
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

void HistoryWindow::openItem()
{
    // Passes the url requested to be opened by the user to the browser main window
    emit requestedUrl(util->extractUrl(ui->listWidget->currentItem()->text()));
}

void HistoryWindow::deleteItem()
{
    //The selected bookmark from the list is deleted  permanently

    QFile historyFile(mFilePath+"/storage/history.qool");
    if(historyFile.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QString str;
        QTextStream txtStream(&historyFile);
        while(!txtStream.atEnd())
        {
            QString line = txtStream.readLine();

            //Loops through user's history and deletes the requested history item from file
            if(!line.contains(ui->listWidget->currentItem()->text()))
                str.append(line + "\n");
        }
        historyFile.resize(0);
        txtStream << str;
        historyFile.close();
    }
}

void HistoryWindow::refreshList()
{
    // Reloads the history list to the window

    ui->listWidget->clear();
    loadHistory();
}

void HistoryWindow::slotEmitUrlToMainWindow(QListWidgetItem* item)
{
   emit requestedUrl(util->extractUrl(item->text()));

}
