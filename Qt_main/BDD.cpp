#include "BDD.h"

void BDD::Connect_BDD(){

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("BDD.db");

    //qDebug() << "Chemin courant =" << QDir::currentPath();

    if (!db.open()) {
        qDebug() << "Erreur ouverture DB:" << db.lastError().text();
    }
    else {
        qDebug() << "Ouverture DB";
    }
}

/*Users*/
void BDD::Connection(const QString& nom, const QString& mdp) {

    QSqlQuery query;
    query.prepare("SELECT id FROM users WHERE nom = :nom AND mot_de_passe = :mdp");

    query.bindValue(":nom", nom);
    query.bindValue(":mdp", mdp);

    if (!query.exec()) {
        qDebug() << "Erreur SQL:" << query.lastError().text();
        return;
    }

    if (query.next()) {
        int id = query.value(0).toInt();

        qDebug() << "ID =" << id;
        qDebug() << "nom" << nom;

        id_user = id;

        nom_user = nom;
    }

}

void BDD::Inscription(const QString& nom, const QString& mdp){

    QSqlQuery query;
    query.prepare("INSERT INTO users (nom, mot_de_passe) VALUES (:nom, :mdp)");

    query.bindValue(":nom", nom);
    query.bindValue(":mdp", mdp);

    if (!query.exec()) {
        qDebug() << "Erreur SQL:" << query.lastError().text();
        return;
    }

}
/*
void BDD::Modification_User(int id, const QString& nom, const QString& mdp) {
    QSqlQuery query;
    query.prepare("UPDATE users SET nom = :nom, mot_de_passe = :mdp WHERE id = :id");

    query.bindValue(":nom", nom);
    query.bindValue(":mdp", mdp);
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Erreur SQL:" << query.lastError().text();
    }
    else {
        qDebug() << "Utilisateur modifié avec succès";
    }
}

void BDD::Suppression_User(){

    if (id_user == 0) {
        qDebug() << "Aucun utilisateur connecté";
        return;
    }

    QSqlQuery query;
    query.prepare("DELETE FROM users WHERE id = :id");
    query.bindValue(":id", id_user);

    if (!query.exec()) {
        qDebug() << "Erreur SQL:" << query.lastError().text();
        return;
    }

    qDebug() << "Utilisateur supprimé avec succès";
    id_user = 0;
}*/
