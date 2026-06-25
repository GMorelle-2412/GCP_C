#pragma once

#include <QString>
#include <QWidget>
#include <QVBoxLayout>

class sauvegarde {

public:
    /*structure*/
    struct sauvegarde_element{
        int id;
        QString nom;
        QString description;
    };
    sauvegarde_element sauvegarde_element;

    struct LigneListe {
        int id;
        QString contenu;
        bool validation;
    };
    LigneListe sauvegarde_liste;

    struct LigneContenueElement {
        int id;
        int id_element;
        int id_liste;
        int id_note;
    };
    LigneContenueElement sauvegarde_contenue_liste_Element;

    struct Contenu_element_notes {
        int id;
        int id_element;
        int id_notes;
    };
    Contenu_element_notes Sauvegarde_Contenu_element_notes;

    struct LigneElement {
        int id;
        QString nom;
        QString description;
    };
    LigneElement sauvegarde_LigneElement;

    struct LigneNote {
        int id;
        QString text;
        QString nom;
    };
    LigneNote sauvegarde_note;


    /*vector*/
    std::vector<int> table_id_liste_modif;
    std::vector<int> id_liste;


    /*variable*/
    int id_notes = 0;              
    int verife_reste_liste = -1;
    int sauvegarde_nb_liste_valider = 0;
    int sauvegarde_nb_liste_max = 0;
    int nb_projet = 0;
    int nb_projet_platine = 0;
    int id_user = 3;
    int id_element = 0;
    bool m_wasScrolling = false;


    /*QObject*/
    QWidget* sauve_modif_notes;
    QVBoxLayout* layout_sauve_modif_notes;
    QString nom_user = "nul";
    QPointF m_pressPos;
    QWidget* m_pressedWidget = nullptr;
};