#pragma once

#include <QObject>
#include "Qt_main.h"
#include "ui_Qt_main.h"

class select : public QObject {
    Q_OBJECT

public:
    explicit select(Qt_main* mainWindow);

    Ui::Qt_mainClass* ui;

    void get_projet();
};
