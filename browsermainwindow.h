#ifndef BROWSERMAINWINDOW_H
#define BROWSERMAINWINDOW_H

#include <QMainWindow>
#include <QtWebEngineWidgets>
#include <QWebEngineView>
#include <QWebEngineHistory>
#include <QMovie>
#include <QTextStream>
#include <QFile>
#include <QUrl>
#include <QIODevice>
#include <QDebug>

#include "qoollineedit.h"
#include "bookmarks.h"
#include "addbookmarkdialog.h"
#include "historywindow.h"

namespace Ui {
class BrowserMainWindow;
}

class BrowserMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit BrowserMainWindow(QWidget *parent = 0);
    ~BrowserMainWindow();

private:
    Ui::BrowserMainWindow *ui;

    //! Holds the current index of the tabwidget
    int currentTabIndex;

    //! Holds the amount of tabs opened in the tabwidget
    int tabCount;

    //! Holds whether the browser is in fullscreen mode or not
    bool windowFullscreen = false;

    //! Contains the file path (absolute path of the application directory) which stores the browsing history.
    QString mHistoryFilePath;

    //!Dynamic object of QWebEngineView
    QWebEngineView *webView;

    //!Dynamic object of QTabWidget
    QTabWidget *tabView;

    //!Dynamic object of AddBookmarkDialog Class
    AddBookmarkDialog *bDialog;

    //!Dynamic object of HistoryWindow Class
    HistoryWindow *historyWindow;

    //!Dynamic object of Bookmarks Class
    Bookmarks *allBookMarks;

    //Used to load and animate the spinning .gif
    QMovie *movLoad;
    QLabel *lblLoad;

    //Set of dynamic objects of QPushButtons used in browser GUI
    QPushButton *backButton;
    QPushButton *forwardButton;
    QPushButton *refreshButton;
    QPushButton *stopButton;
    QPushButton *searchButton;
    QPushButton *favButton;

    //! Custom QLineEdit with double click signal implementation.
    QoolLineEdit *UrlBar;

    /*!
     * A Qlist of type QWebEngineView, holds references to each active webView in the browser.
     * Whenever a tab is closed, the corresdponding webView is removed from the list.
    */
    QList<QWebEngineView *> webViewList;

    /*!
        Builds up the initial user interface inside the tab.
        @param QString url: Contains the url to be loaded, if empty google.com is loaded.
    */
    void setupTabUI(QString);

    /*!
        Creates a new tab within tabwidget.
        @param QString url: Contains the url to be loaded, if empty google.com is loaded.
    */
    void createNewTab(QString);

    /*!
        This method checks if the passed parameter is a valid url or not.
        It connects to the host of the passed URL throught a QTcpSocket and checks if the host returns a "200 OK" status code, which specifies validity of the url.

        Credit for the algorithm goes to the author of this answer in StackOverflow : http://stackoverflow.com/a/28498623/5208124
        I have done minimal modifications to suit my application needs.

        @param QUrl url: contains the url that is checked for validity.
        @returns bool: true if the url is valid or false if invalid.
    */
    bool isUrlValid(QUrl);

signals:

    /*!
        Initiates the process of adding a bookmark.
        Emitted when the "Add to bookmarks" button is pressed.
        @param QString title : title of web page
        @param QString url : url of web page
    */
    void newBookmarkAdded(QString,QString);


    /*!
        Emitted when a HTML stream of a web page is requested.
        @param QString: Contains the HTML stream which is converted to a string.
    */
    void newPageSrcReceived(QString);


    /*!
        Emitted when a new web page is accessed, the information of the access web page is stored in the history.
        @param QString title : title of web page
        @param QString url : url of web page
        @param QString dataTime : the date and time which the web page was accessed, converted to string from QDateTime.
    */
    void newHistoryItem(QString, QString, QString);

private slots:

    /*!
        Connected with loadStarted() Signal in QWebEngineView.
        Loading spinner animation is started when signal emits a new load of a web page.
    */
    void slotRunSpinner();


    /*!
        Connected with loadFinished(bool) Signal in QWebEngineView.
        Once a web page is loaded successfully, the URL of the loaded web page is set to the UrlBar (type: QoolLineEdit).
        @param bool loaded : argument which contains true if web page loaded successfully or false if an error occured.
    */
    void slotSetUrlOnBar(bool);


    /*!
        This slot is connected to two signals, returnPressed() which is emitted from QoolLineEdit (UrlBar) when return key is pressed and QPushButton (searchButton) pressed() signal.
        Whenever these signals are emitted, the text from UrlBar is retrieved and checks if the user entered a valid url or not using the isUrlValid() method.
        If it's valid the Url is loaded to the webView or else the text is entered into a Google search.
    */
    void slotLoadWebPage();


    /*!
        Closes a tab when the close button on a tab is pressed.
        Connected with tabCloseRequested(int) signal in QTabWidget.
        @param int index : contains the index of the tab that should be removed.
    */
    void slotCloseTabOnRequest(int);


    /*!
        Retrieves the web icon of the loaded web page (favicon) from the webView and sets it on the tab it's hosted on.
        Connected with iconChanged(QIcon) signal in QWebEngineView.
        @param QIcon icon : contains the icon of the web page.
    */
    void slotSetIconOnTab(QIcon);


    /*!
        Retrieves the title of the loaded web page from the webView and sets it as the title of the tab it's hosted on.
        Connected with titleChanged(const QString &title) signal in QWebEngineView.
        @param QString title : contains the title of the web page.
    */
    void slotSetTitleOnTab(QString);


    /*!
        Used to update the currentTabIndex integer variable with currently opened tab index in the tab widget.
        Connected with currentChanged(int) signal in QTabWidget.
        @param int index : contains parameter of the new current tab index position.
    */
    void slotSetFocusToNewTab(int);


    /*!
        Connected with QPushButton favButton pressed() signal.
        When user press "Add to Bookmarks" button, this slot emits the newBookmarkAdded(QString, QString) signal, which passes the web page title and url from the webView to the signal.
        newBookmarkAdded signal is connected with slotSetBookmarkInfo public slot implemented in AddBookmarkDialog class which continues the process of successfully adding a bookmark.
    */
    void slotAddBookmark();


    /*!
        Connected with newPageSrcReceived(QString) custom signal which is emitted when PageSource action is triggered.
        The slot receives the page source from the signal which contains the original text and HTML tags of the web page loaded inside the webView.
        @param QString result : contains the HTML stream which is converted to text.
    */
    void slotReceivePageSrc(QString);


    /*!
        When a user enters a web page, a web browser usually saves the title, url and date/time visited of the web page which can be later view using the history window.
        Connected with newHistoryItem(QString, QString, QString) signal which is emitted whenever a new web page is successfully loaded.
        The received parameters from the signal is then written to the history file which is located in the current application directory.
        @param QString title : title of web page
        @param QString url : url of web page
        @param QString dataTime : the date and time which the web page was accessed, converted to string from QDateTime.
    */
    void slotSaveHistoryItem(QString, QString, QString);


    /*!
        Connected with requestedUrl(QString) signals in bookmarks window and history window.
        Whenever a user double clicks or right click -> open an url from bookmarks window or history window,
        the url is passed to the slot and a new tab is created in the browser loading the url in it.
        @param QString url : contains the url requested to be opened in the browser.
    */
    void slotOpenRequestedUrl(QString);


    /*!
        Calls the method createNewTab(), which will create a new tab inside browser window and load google.com
    */
    void on_actionNew_Tab_triggered();


    /*!
        Opens another browser window by creating a new instance of BrowserMainWindow class.
    */
    void on_actionNew_Window_triggered();


    /*!
        Exits the application.
    */
    void on_actionQuit_triggered();


    /*!
        Stops loading the current web page.
    */
    void on_actionStop_triggered();


    /*!
        Reloads the current web page.
    */
    void on_actionReload_Page_triggered();


    /*!
        The zoom factor of the webView is increased by 0.1
    */
    void on_actionZoom_in_triggered();


    /*!
        The zoom factor of the webView is decreased by 0.1
    */
    void on_actionZoom_out_triggered();


    /*!
        Resets the zoom factor of the webView to the default value (1.0)
    */
    void on_actionReset_Zoom_triggered();


    /*!
        Opens the page source of the current web page in a new window.
    */
    void on_actionPage_Source_triggered();


    /*!
        Browser window toggles between maximized and fullscreen state.
    */
    void on_actionFull_Screen_triggered();


    /*!
        Opens a list of bookmarks added by the user in a new window.
    */
    void on_actionShow_all_bookmarks_triggered();


    /*!
        Opens the browsing history in a new window.
    */
    void on_actionShow_all_history_triggered();


    /*!
        Opens an about QMessageBox.
    */
    void on_actionAbout_Qool_Browser_triggered();
};

#endif // BROWSERMAINWINDOW_H
