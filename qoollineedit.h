#ifndef QOOLLINEEDIT_H
#define QOOLLINEEDIT_H

#include <QLineEdit>


class QoolLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit QoolLineEdit(QWidget *parent = Q_NULLPTR);
    ~QoolLineEdit();

signals:
    void doubleClicked();

protected:
    //! Gives double click ability to LineEdit
    void  mouseDoubleClickEvent(QMouseEvent*);
};

#endif // QOOLLINEEDIT_H
