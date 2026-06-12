#pragma once

#include "BDD.h"
#include "Style.h"
#include "ui_Qt_main.h"
#include "projet.h"

#include <QObject>
#include <QPushButton>
#include <QSettings>

// Forward declarations pour éviter les boucles d'inclusion
class projet;
class select;

class identification : public QObject {
    Q_OBJECT

public:
    explicit identification(QObject* parent = nullptr) : QObject(parent) {}

    Ui::Qt_mainClass* ui;

    BDD* class_BDD;
    projet* class_projet;
    select* class_select;

    void page_connection();
    void page_inscription();
    void bouton_connection();
    void bouton_inscription();
    void affiche_info_user();
    void bouton_deconection();
};
