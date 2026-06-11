#pragma once

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <qdir.h>
#include <vector>
#include <QString>
#include <QDir>
#include <QFile>
#include <QStandardPaths>


class BDD{

public:

    int id_user = 0;

    QString nom_user = "nul";

    int id_element = 0;

    std::vector<int> id_liste;

    //Capture liste
    struct LigneListe {
        int id;
        QString contenu;
        bool validation;
    };

    //Capture contenu_liste
    struct LigneContenueElement {
        int id;
        int id_element;
        int id_liste;
    };

    //Capture element
    struct LigneElement {
        int id;
        QString nom;
        QString description;
    };


public slots:
    void Connect_BDD();

    /*users*/
    bool Connection(const QString& nom, const QString& mdp); // void ? bool

    void Connection_auto(const QString& nom);

    void Inscription(const QString& nom, const QString& mdp);

    //void Modification_User(int id, const QString& nom, const QString& mdp);

    //void Suppression_User();

    /*element*/
    std::vector<LigneElement>Get_element();

    void Poste_element(const QString& nom, const QString& description);

    void modif_element(int id, QString nom, QString description);

    /*liste*/
    std::vector<LigneListe>Get_liste();

    int Poste_liste(const QString& contenu, bool validation);

    void modif_liste(int id, QString contenu, bool validation);

    void delete_liste(int id_liste);

    /*contenu_liste*/
    std::vector<LigneContenueElement>Get_contenu_liste();

    void Poste_contenu_liste(int id_element, int id_liste);

    void delete_contenu_liste(int id_liste);

};
