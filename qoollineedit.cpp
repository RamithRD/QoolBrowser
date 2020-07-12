#include "qoollineedit.h"

QoolLineEdit::QoolLineEdit(QWidget *parent)
    :QLineEdit(parent)
{

}

QoolLineEdit::~QoolLineEdit()
{

}

//Custom LineEdit implemented with DoubleClick signal, used in URL bar
void QoolLineEdit::mouseDoubleClickEvent(QMouseEvent*)
{
   emit doubleClicked();
}
