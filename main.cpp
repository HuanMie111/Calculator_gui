#include "Calculator_gui.h"
#include <QtWidgets/QApplication>
 
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Calculator_gui w;
    w.show();
    return a.exec();
}
