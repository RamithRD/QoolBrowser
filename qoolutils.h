#ifndef QOOLUTILS_H
#define QOOLUTILS_H

#include <QString>
#include <QTextStream>


class QoolUtils
{
public:
    QoolUtils();

    //! Extracts an Url from a line of QString
    QString extractUrl(QString);
};

#endif // QOOLUTILS_H
