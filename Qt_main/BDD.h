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


class BDD : public QObject {

    Q_OBJECT

public:
    explicit BDD(QObject* parent = nullptr); // QObject* au lieu de QWidget*
    ~BDD();

    int id_user = 3;
    QString nom_user = "nul";
    int id_element = 0;
    std::vector<int> id_liste;

    struct LigneListe {
        int id;
        QString contenu;
        bool validation;
    };

    struct LigneContenueElement {
        int id;
        int id_element;
        int id_liste;
    };

    struct LigneElement {
        int id;
        QString nom;
        QString description;
    };

public slots:
    void Connect_BDD();

    void Connection(const QString& nom, const QString& mdp);
    void Connection_auto(const QString& nom);
    void Inscription(const QString& nom, const QString& mdp);

    std::vector<LigneElement> Get_element();
    void Poste_element(const QString& nom, const QString& description);
    void modif_element(int id, const QString& nom, const QString& description); // const QString&

    std::vector<LigneListe> Get_liste();
    int Poste_liste(const QString& contenu, bool validation);
    void modif_liste(int id, const QString& contenu, bool validation);         // const QString&
    void delete_liste(int id_liste);

    std::vector<LigneContenueElement> Get_contenu_liste();
    void Poste_contenu_liste(int id_element, int id_liste);
    void delete_contenu_liste(int id_liste);
};