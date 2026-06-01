#pragma once

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <qdir.h>

class BDD{

public:

    int id_user = 0;

    QString nom_user = "nul";

public slots:
    void Connect_BDD();

    /*users*/
    void Connection(const QString& nom, const QString& mdp);

    void Inscription(const QString& nom, const QString& mdp);

    void Modification_User(int id, const QString& nom, const QString& mdp);

    void Suppression_User();
};
