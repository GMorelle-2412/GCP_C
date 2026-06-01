#include "Qt_main.h"
#include <QtWidgets/QApplication>
#include "BDD.h"

int main(int argc, char *argv[]){

    QApplication app(argc, argv);
    Qt_main window;
    window.show();
    return app.exec();
}
