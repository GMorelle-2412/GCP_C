#pragma once

#include "BDD.h"
#include "Style.h"
#include "ui_Qt_main.h"

#include <QObject>
#include <QPushButton>
#include <vector>
#include <QCheckBox>

class projet : public QObject {
    Q_OBJECT

public:
    explicit projet(QObject* parent = nullptr) : QObject(parent) {}

    int verife_reste_liste = -1;

    std::vector<int> table_id_liste_modif;

    Ui::Qt_mainClass* ui = nullptr;

    BDD* class_BDD = nullptr;

    void bouton_creation_projet();

    void ajouter_liste();

    void creation_projet();

    void affiche_element_liste();

    void affichage_modif_projet(int id_element,
        std::vector<BDD::LigneElement> data_element,
        std::vector<BDD::LigneListe> data_liste,
        const std::vector<BDD::LigneContenueElement> data_contenue_liste,
        QPushButton* modif);

    void ajout_liste_modif(int id_element,
        const std::vector<BDD::LigneContenueElement> data_contenue_liste);

    void modif_projet(int id_element,
        std::vector<BDD::LigneElement> data_element,
        std::vector<BDD::LigneListe> data_liste,
        const std::vector<BDD::LigneContenueElement> data_contenue_liste);
};