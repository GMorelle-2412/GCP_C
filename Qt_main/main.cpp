#include "Qt_main.h"
#include <QtWidgets/QApplication>
#include "BDD.h"
#include <QIcon>

int main(int argc, char *argv[]){

    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/icon/image/icon.PNG"));
    Qt_main window;
    window.show();
    return app.exec();
}
