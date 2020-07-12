#ifndef ADDBOOKMARKDIALOG_H
#define ADDBOOKMARKDIALOG_H

#include <QDialog>
#include <QTextStream>
#include <QFile>
#include <QUrl>
#include <QIODevice>
#include <QDir>
#include <QDebug>


namespace Ui {
class AddBookmarkDialog;
}

class AddBookmarkDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddBookmarkDialog(QWidget *parent = 0);
    ~AddBookmarkDialog();

private slots:

    //! Triggered when "OK" button of the dialog box is pressed. Calls the saveBookmark() method.
    void on_buttonBox_accepted();

    /*!
        Receives the title and the url of the bookmarked web page and these values are set to the member variables.
        @param QString title : title of web page
        @param QString url : url of web page
    */
    void slotSetBookmarkInfo(QString,QString);

private:
    Ui::AddBookmarkDialog *ui;

    //private member variables
    QString mBookmarkTitle;
    QString mBookmarkUrl;
    QString mFilePath;
    QUrl mBookmarkIconUrl;

    //! Writes a user added bookmark to bookmarks file
    void saveBookmark();

};

#endif // ADDBOOKMARKDIALOG_H
