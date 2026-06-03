#pragma once

#include "ui_Qt_main.h"
#include "BDD.h"

#include <QtWidgets/QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QCheckBox>

class Qt_main : public QMainWindow{

    Q_OBJECT

public:
    Qt_main(QWidget *parent = nullptr);
    ~Qt_main();

private:
    Ui::Qt_mainClass * ui;

    BDD class_BDD;

private slots:
    void page_connection();

    void page_inscription();
    
    void bouton_connection();

    void bouton_inscription();

    void affiche_info_user();

    void bouton_deconection();

    void bouton_annulation();

    void bouton_creation_projet();

    void ajouter_liste();

    void creation_projet();

    void affiche_element_liste();

    void affichage_modif_projet(int id_element, std::vector<BDD::LigneElement> data_element, std::vector<BDD::LigneListe> data_liste, const std::vector<BDD::LigneContenueElement> data_contenue_liste, QPushButton* modif);

    void modif_projet();
};