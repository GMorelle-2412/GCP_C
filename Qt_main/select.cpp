#include "select.h"

select::select(Qt_main* mainWindow)
    : QObject(mainWindow)
{
    ui = mainWindow->ui;   // On récupère l’UI existante
}



void select::get_projet() {
    /*std::vector<BDD::LigneListe> data_liste = class_BDD->Get_liste();
    std::vector<BDD::LigneContenueElement> data_contenue = class_BDD->Get_contenu_liste();
    std::vector<BDD::LigneElement> data_element = class_BDD->Get_element();*/

    qDebug() << "la fonction get_projet est appellet";

    QWidget* projet = new QWidget();

    // Layout principal du widget "projet"
    QVBoxLayout* layout_principal = new QVBoxLayout(projet);

    // Zone titre + description
    QVBoxLayout* zone_titre_description = new QVBoxLayout;

    QLabel* titre = new QLabel("test");
    zone_titre_description->addWidget(titre);

    QLabel* description = new QLabel("test");
    zone_titre_description->addWidget(description);

    // Zone état
    QHBoxLayout* zone_etat = new QHBoxLayout;

    QLabel* pourcentage_de_completion = new QLabel("0 %");
    zone_etat->addWidget(pourcentage_de_completion);

    // On assemble tout
    layout_principal->addLayout(zone_titre_description);
    layout_principal->addLayout(zone_etat);

    // On ajoute le widget final dans ton layout UI
    ui->verticalLayout_20->addWidget(projet);
    
    /*
    for (int i = 0; i < data_element.size(); i++) {

        for (int j = 0; j < data_contenue.size(); j++) {
            
            if (data_element[i].id == data_contenue[j].id_element) {

                

            }
        }
        
    }*/
};