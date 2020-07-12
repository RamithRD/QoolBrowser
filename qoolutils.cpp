#include "qoolutils.h"

QoolUtils::QoolUtils()
{

}

QString QoolUtils::extractUrl(QString item)
{
    /* In both history file and bookmarks file, an entry row is seperated with: | AKA seperator charactor
     * for example in bookmark list: Google | https://www.google.lk/
     * after the last seperator charactor the URL of the web page is available
     * this method takes out the URL from string item and returns it.
    */

    QString url = item.split('|').last();
    //converts whitespace characters to ASCII 32
    url = url.simplified();
    //removes white spaces from string
    url.replace( " ", "" );

    return url;
}
