#include "addbookmarkdialog.h"
#include "ui_addbookmarkdialog.h"

AddBookmarkDialog::AddBookmarkDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddBookmarkDialog)
{
    ui->setupUi(this);
    QWidget::setWindowIcon(QIcon(":/Icons/Assets/browser_icon.png"));

    mFilePath = QDir::currentPath();

    if(QFileInfo(mFilePath+"/storage/bookmarks.qool").exists()){
        //The file exists

        qDebug() << "File exists";

    }else{

        //The file doesn't exist

        qDebug() << "File not found";

        QDir dir;
        dir.mkpath(mFilePath+"/storage");

        //if file not exists, txt file is created in the parent directory under storage folder
        QFile f(mFilePath+"/storage/bookmarks.qool");
        f.open(QIODevice::ReadWrite);

    }
}

AddBookmarkDialog::~AddBookmarkDialog()
{
    delete ui;
}

void AddBookmarkDialog::on_buttonBox_accepted()
{
    saveBookmark();
}

void AddBookmarkDialog::slotSetBookmarkInfo(QString title, QString url)
{
    ui->bookmarkName->setText(title);
    ui->bookmarkUrl->setText(url);

    mBookmarkTitle = title;
    mBookmarkUrl = url;
}

void AddBookmarkDialog::saveBookmark()
{
    /* Try and open a file for output */
       QString outputFilename = "bookmarks.qool";
       QFile outputFile(mFilePath+"/storage/"+outputFilename);
       outputFile.open(QIODevice::Append);

       /* Check it opened OK */
       if(!outputFile.isOpen()){
           qDebug() << "- Error, unable to open";

       }

       /* Point a QTextStream object at the file */
       QTextStream outStream(&outputFile);

       /* Write the line to the file */
       outStream << ui->bookmarkName->text() + " | " + ui->bookmarkUrl->text()+"\n";

       /* Close the file */
       outputFile.close();

}



