#include "Qt_main.h"
#include "identification.h"
#include "BDD.h"
#include "SelectManager.h"

#include <QIcon>
#include <QtWidgets/QApplication>

int main(int argc, char *argv[]){

    QApplication app(argc, argv);

    app.setWindowIcon(QIcon(":/icon/image/icon.PNG"));

    Qt_main window;

    window.show();

    return app.exec();
}
