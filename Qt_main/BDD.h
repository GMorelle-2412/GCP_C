#pragma once

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <QString>
#include <vector>
#include <QLabel.h>


class BDD : public QObject {

    Q_OBJECT

public:
    explicit BDD(QObject* parent = nullptr); // QObject* au lieu de QWidget*
    ~BDD();

    int id_user = 3;

    QString nom_user = "nul";

    int id_element = 0;

    std::vector<int> id_liste;

    int id_notes = 0;

    struct LigneListe {
        int id;
        QString contenu;
        bool validation;
    };

    struct LigneContenueElement {
        int id;
        int id_element;
        int id_liste;
        int id_note;
    };

    struct Sauvegarde_Contenu_element_notes {
        int id;
        int id_element;
        int id_notes;
    };

    struct LigneElement {
        int id;
        QString nom;
        QString description;
    };

    struct LigneNote {
        int id;
        QString text;
        QString nom;
    };

public slots:
    /*Connexion BDD*/
    void Connect_BDD();


    /*Users*/
    void Connection(const QString& nom, const QString& mdp);
    void Connection_auto(const QString& nom);
    void Inscription(const QString& nom, const QString& mdp);


    /*Element*/
    std::vector<LigneElement> Get_element();
    int Poste_element(const QString& nom, const QString& description);
    void modif_element(int id, const QString& nom, const QString& description); 


    /*Listes*/
    std::vector<LigneListe> Get_liste();
    int Poste_liste(const QString& contenu, bool validation);
    void modif_liste(int id, const QString& contenu, bool validation);        
    void delete_liste(int id_liste);


    /*Contenu_element_listes*/
    std::vector<LigneContenueElement> Get_contenu();
    void Poste_contenu(int id_element, int id_liste);
    void delete_contenu(int id_liste);


    /*Contenu_element_notes*/
    std::vector<Sauvegarde_Contenu_element_notes> Get_Contenu_element_notes(int id_element);
    int Poste_Contenu_element_notes(int id_element, int id_notes);
    int modif_note(int id_note, const QString& nom, const QString& texte);


    /*Notes*/
    std::vector<LigneNote> Get_note(int id_note);
    int Poste_note(const QString& text, const QString& nom);

    /*A trier*/
    // BDD.h — ajouter dans la classe
    void delete_element(int id_element);
    void delete_element_complet(int id_element); // supprime tout en cascade
};