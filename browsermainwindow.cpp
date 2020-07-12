#include "browsermainwindow.h"
#include "ui_browsermainwindow.h"

BrowserMainWindow::BrowserMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BrowserMainWindow)
{
    ui->setupUi(this);

    //Sets the title of the browser window
    QWidget::setWindowTitle("Qool Browser");
    //Sets the icon of the browser window
    QWidget::setWindowIcon(QIcon(":/Icons/Assets/browser_icon.png"));
    //Opens browser window as maximized at startup
    QWidget::showMaximized();

    //Initializes tab index to 0
    currentTabIndex = 0;

    /*
     * QTabWidget object is created and some preferences are set:
     * 1. Close buttons appear on each tab
     * 2. Tabs are movable within the tabber area
     * 3. Shape of tabs are set to triangular
     * 4. The height of a tab is increased to 22px using a stylesheet
    */
    tabView = new QTabWidget();
    tabView->setTabsClosable(true);
    tabView->setMovable(true);
    tabView->setTabShape(QTabWidget::Triangular);
    tabView->setStyleSheet("QTabBar::tab { height: 22px;}");

    //Sets the tabView as the central widget of the mainwindow
    setCentralWidget(tabView);

    //A new tab is created on browser launch. By default Google.com is loaded.
    createNewTab("");

}

BrowserMainWindow::~BrowserMainWindow()
{
    delete ui;
}

void BrowserMainWindow::setupTabUI(QString mUrl)
{

    // the loading gif is set to the label as a QMovie
    lblLoad = new QLabel;
    movLoad = new QMovie(":/Loader/Assets/loader.gif");
    lblLoad->setMovie(movLoad);
    lblLoad->show();

    UrlBar = new QoolLineEdit();
    webView = new QWebEngineView();

    bDialog = new AddBookmarkDialog();
    //Only one instance of addbookmark dialog will be displayed
    bDialog->setModal(true);

    backButton = new QPushButton();
    backButton->setIcon(QIcon(":/Icons/Assets/back.png"));
    backButton->setToolTip("Click to go back");
    backButton->setEnabled(false);

    forwardButton = new QPushButton();
    forwardButton->setIcon(QIcon(":/Icons/Assets/forward.png"));
    forwardButton->setToolTip("Click to go forward");
    forwardButton->setEnabled(false);

    refreshButton = new QPushButton();
    refreshButton->setIcon(QIcon(":/Icons/Assets/refresh.png"));
    refreshButton->setToolTip("Click to refresh");

    searchButton = new QPushButton();
    searchButton->setIcon(QIcon(":/Icons/Assets/search.png"));
    searchButton->setToolTip("Search");

    favButton = new QPushButton();
    favButton->setIcon(QIcon(":/Icons/Assets/favourite.png"));
    favButton->setToolTip("Add to favourites");

    //Setting UrlBar properties
    UrlBar->setClearButtonEnabled(true);

    //Setting WebView properties
    if(mUrl.isEmpty())
    {
        webView->load(QUrl("http://google.com"));

    }else{
        webView->load(QUrl(mUrl));
        mUrl.clear();
    }

    webView->show();
}

void BrowserMainWindow::createNewTab(QString nUrl)
{
    if(nUrl.isEmpty())
    {
        setupTabUI("");

    }else{
        setupTabUI(nUrl);
        nUrl.clear();
    }

    //Finalizing new tab UI placements
    QHBoxLayout * hLayout = new QHBoxLayout;
    hLayout->addWidget(backButton);
    hLayout->addWidget(forwardButton);
    hLayout->addWidget(refreshButton);
    hLayout->addWidget(UrlBar);
    hLayout->addWidget(searchButton);
    hLayout->addWidget(favButton);
    hLayout->addWidget(lblLoad);

    QWidget *hLayWidget = new QWidget();
    hLayWidget->setLayout(hLayout);

    QVBoxLayout *finalVLayout = new QVBoxLayout;
    finalVLayout->addWidget(hLayWidget);
    finalVLayout->addWidget(webView);
    finalVLayout->setMargin(0);

    QWidget *newTab = new QWidget();
    newTab->setLayout(finalVLayout);

    //New tab will load google.com by default
    tabView->addTab(newTab,"New Tab");
    //Sets focus to newly created tab
    tabView->setCurrentWidget(newTab);
    //Adds the newly created instance of thewebView to the list of webViews
    webViewList.append(webView);

    /* Signal and Slots connections - start */

    connect(tabView,SIGNAL(tabCloseRequested(int)),
            this,SLOT(slotCloseTabOnRequest(int)));

    connect(webView,SIGNAL(loadStarted()),
            this,SLOT(slotRunSpinner()));

    connect(webView,SIGNAL(loadFinished(bool)),
            this,SLOT(slotSetUrlOnBar(bool)));

    connect(UrlBar,SIGNAL(returnPressed()),
            this,SLOT(slotLoadWebPage()));

    connect(UrlBar,SIGNAL(doubleClicked()),
            UrlBar,SLOT(selectAll()));

    connect(searchButton,SIGNAL(pressed()),
            this,SLOT(slotLoadWebPage()));

    connect(backButton,SIGNAL(pressed()),
            webView,SLOT(back()));

    connect(forwardButton,SIGNAL(pressed()),
            webView,SLOT(forward()));

    connect(refreshButton,SIGNAL(pressed()),
            webView,SLOT(reload()));

    connect(favButton,SIGNAL(pressed()),
            this,SLOT(slotAddBookmark()));

    connect(webView,SIGNAL(iconChanged(QIcon)),
            this,SLOT(slotSetIconOnTab(QIcon)));

    connect(webView,SIGNAL(titleChanged(QString)),
            this,SLOT(slotSetTitleOnTab(QString)));

    connect(tabView,SIGNAL(currentChanged(int)),
            this,SLOT(slotSetFocusToNewTab(int)));

    //sets URL and title on add bookmark dialog's linedits
    connect(this,SIGNAL(newBookmarkAdded(QString,QString)),
            bDialog,SLOT(slotSetBookmarkInfo(QString,QString)));

    connect(this,SIGNAL(newHistoryItem(QString,QString,QString)),
            this,SLOT(slotSaveHistoryItem(QString,QString,QString)));

    /* Signal and Slots connections - end */

    //updates the tabCount with the updated count of tabs
    tabCount = tabView->count();

}

bool BrowserMainWindow::isUrlValid(QUrl url)
{
    QString a = "a";

     QTcpSocket socket;
     socket.connectToHost(url.host(), 80);
     if (socket.waitForConnected()) {
         socket.write("HEAD " + url.path().toUtf8() + " HTTP/1.1\r\n"
                      "Host: " + url.host().toUtf8() + "\r\n\r\n");
         if (socket.waitForReadyRead()) {
             QByteArray bytes = socket.readAll();
             if (bytes.contains("200 OK")) {
                 return true;
             }
         }
     }
     return false;
}

void BrowserMainWindow::slotLoadWebPage()
{
    QUrl loadUrl(UrlBar->text());

    if(isUrlValid(loadUrl)){

         webView->load(loadUrl);

    }else{

        QString searchKey = UrlBar->text();
        webView->load(QUrl("http://www.google.com/search?q="+searchKey));
    }

}

void BrowserMainWindow::slotCloseTabOnRequest(int tabIndex)
{
    tabView->removeTab(tabIndex);
    webViewList.removeAt(tabIndex);
}

void BrowserMainWindow::slotSetIconOnTab(QIcon icon)
{
    tabView->setTabIcon(currentTabIndex,icon);
}

void BrowserMainWindow::slotSetTitleOnTab(QString titleStr)
{
    tabView->setTabText(currentTabIndex,titleStr);
}

void BrowserMainWindow::slotSetFocusToNewTab(int index)
{
    currentTabIndex = index;
}

void BrowserMainWindow::slotAddBookmark()
{
    QWebEngineView *wview = new QWebEngineView();

    wview = webViewList[currentTabIndex];

    emit newBookmarkAdded(wview->title(),wview->url().toString());
    bDialog->show();

}

void BrowserMainWindow::slotReceivePageSrc(QString src)
{
    // Converted HTML stream is displayed in a new window

    QPlainTextEdit *view = new QPlainTextEdit;
    view->setWindowIcon(QIcon(":/Icons/Assets/browser_icon.png"));
    view->setMinimumWidth(640);
    view->setMinimumHeight(480);
    view->setAttribute(Qt::WA_DeleteOnClose);
    view->setPlainText(src);
    view->show();
}

void BrowserMainWindow::slotSaveHistoryItem(QString title, QString url, QString dateTime)
{
    mHistoryFilePath = QDir::currentPath();

    if(QFileInfo(mHistoryFilePath+"/storage/history.qool").exists()){
        //The file exists

    }else{

        //The file doesn't exist

        QDir dir;
        dir.mkpath(mHistoryFilePath+"/storage");

        //if file not exists, txt file is created in the parent directory under storage folder
        QFile f(mHistoryFilePath+"/storage/history.qool");
        f.open(QIODevice::ReadWrite);

    }

    /* Try and open a file for output */
       QString outputFilename = "history.qool";
       QFile outputFile(mHistoryFilePath+"/storage/"+outputFilename);
       outputFile.open(QIODevice::Append);

       /* Check it opened OK */
       if(!outputFile.isOpen()){
           qDebug() << "- Error, unable to open";

       }

       /* Point a QTextStream object at the file */
       QTextStream outStream(&outputFile);

       /* Write the line to the file */
       outStream << dateTime + " | " + title + " | " + url +"\n";

       /* Close the file */
       outputFile.close();
}

void BrowserMainWindow::slotOpenRequestedUrl(QString url)
{
    //url is passed into the method which sets the url of the webview to the requested url
    QWidget::activateWindow();
    createNewTab(url);
}

void BrowserMainWindow::slotRunSpinner()
{
    // Starts the loading animation
    movLoad->start();
}

void BrowserMainWindow::slotSetUrlOnBar(bool loaded)
{
    if(loaded){

        //sets the current URL on the URL bar
        UrlBar->setText(webView->url().toString());
        movLoad->stop();

        //checks if it's possible to go to previous web page in history
        if(webView->history()->canGoBack()){
            backButton->setEnabled(true);
        }else{
            backButton->setEnabled(false);
        }

        if(webView->history()->canGoForward()){
            forwardButton->setEnabled(true);
        }else{
            forwardButton->setEnabled(false);
        }

        /* When a new web page is accessed, it's information such as title, url and date accessed is
         * stored in the history permanently until deleted.*/

        QWebEngineHistoryItem hItem = webView->history()->currentItem();

        emit newHistoryItem(hItem.title(),hItem.url().toString(),hItem.lastVisited().toString());        
    }
}

void BrowserMainWindow::on_actionNew_Tab_triggered()
{
    createNewTab("");
}

void BrowserMainWindow::on_actionNew_Window_triggered()
{
    //New browser window instance created
    BrowserMainWindow *mw = new BrowserMainWindow;
    mw->setAttribute(Qt::WA_DeleteOnClose);
    mw->show();
}

void BrowserMainWindow::on_actionQuit_triggered()
{
    this->close();
}

void BrowserMainWindow::on_actionStop_triggered()
{
    QWebEngineView *wview = new QWebEngineView();

    wview = webViewList[currentTabIndex];
    wview->stop();
}

void BrowserMainWindow::on_actionReload_Page_triggered()
{
    QWebEngineView *wview = new QWebEngineView();

    wview = webViewList[currentTabIndex];
    wview->reload();
}

void BrowserMainWindow::on_actionZoom_in_triggered()
{
    QWebEngineView *wview = new QWebEngineView();

    //increase zoom
    wview = webViewList[currentTabIndex];
    wview->setZoomFactor(wview->zoomFactor() + 0.1);
}

void BrowserMainWindow::on_actionZoom_out_triggered()
{
    QWebEngineView *wview = new QWebEngineView();

    //decrease zoom
    wview = webViewList[currentTabIndex];
    wview->setZoomFactor(wview->zoomFactor() - 0.1);
}

void BrowserMainWindow::on_actionReset_Zoom_triggered()
{
    QWebEngineView *wview = new QWebEngineView();

    //resets zoom to original level of webview in the current tab
    wview = webViewList[currentTabIndex];
    wview->setZoomFactor(1.0);
}

void BrowserMainWindow::on_actionPage_Source_triggered()
{
    QWebEngineView *wview = new QWebEngineView();
    wview = webViewList[currentTabIndex];

    connect(this,SIGNAL(newPageSrcReceived(QString)),
            this,SLOT(slotReceivePageSrc(QString)));

    wview->page()->toHtml([this](const QString& result) mutable {emit newPageSrcReceived(result);});
}

void BrowserMainWindow::on_actionFull_Screen_triggered()
{

    if (isFullScreen())
      {
        // Show the window normal or maximized (depending on how it was shown before)
        if (windowFullscreen)
          showMaximized();
        else
          showNormal();
      }
      else
      {
        // Save if the window is currently maximized
        windowFullscreen = isMaximized();

        showFullScreen();
      }

}

void BrowserMainWindow::on_actionShow_all_bookmarks_triggered()
{
    allBookMarks = new Bookmarks();

    /*When a link is double clicked or opened from bookmarks window,  url is opened in the browser*/
    connect(allBookMarks, SIGNAL(requestedUrl(QString)),
            this,SLOT(slotOpenRequestedUrl(QString)));

    allBookMarks->setAttribute(Qt::WA_QuitOnClose, false);//closes bookmarks window if main window is closed
    allBookMarks->show();
}

void BrowserMainWindow::on_actionShow_all_history_triggered()
{
    historyWindow = new HistoryWindow();

    /*When a link is double clicked or opened from history window,  url is opened in the browser*/
    connect(historyWindow, SIGNAL(requestedUrl(QString)),
            this,SLOT(slotOpenRequestedUrl(QString)));

    historyWindow->setAttribute(Qt::WA_QuitOnClose, false); //closes history window if main window is closed
    historyWindow->show();
}

void BrowserMainWindow::on_actionAbout_Qool_Browser_triggered()
{
    QMessageBox::about(this,
                       "About Qool Browser",
                       "Developed by: Ramith D Randeniya\n\nProject developed for Cross Platform Application Development in C++\n"
                       "Special Thanks to Dr.Davide Maracco for continuous guidance and support.\n\n2016-2017");
}
