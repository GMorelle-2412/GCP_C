#pragma once

#include "ui_Qt_main.h"
#include "BDD.h"
#include "identification.h"
#include "projet.h"
#include "Style.h"

#include <QScroller>
#include <QEasingCurve>
#include <QtWidgets/QMainWindow>

// Forward declaration pour éviter la boucle d'inclusion
class select;

class Qt_main : public QMainWindow {
    Q_OBJECT

public:
    Qt_main(QWidget* parent = nullptr);
    ~Qt_main();

    Ui::Qt_mainClass* ui;

    BDD* class_BDD;
    identification* class_identification;
    projet* class_projet;
    select* class_select;

private:
    int conteur_ajout_liste = 0;

private slots:
    void bouton_annulation();
};
