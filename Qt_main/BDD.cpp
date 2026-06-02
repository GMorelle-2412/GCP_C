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

std::vector<BDD::LigneListe> BDD::Get_liste(){

    std::vector<LigneListe> resultat;

    QSqlQuery query;
    query.prepare("SELECT * FROM liste WHERE id_user = :id_user");
    query.bindValue(":id_user", id_user);

    if (!query.exec()) {
        qDebug() << "Erreur SQL:" << query.lastError().text();
        return resultat; // retourne un tableau vide
    }

    while (query.next()) {

        LigneListe ligne;
        ligne.id = query.value("id").toInt();
        ligne.contenu = query.value("contenu").toString();
        ligne.validation = query.value("validation").toString();

        resultat.push_back(ligne);

        qDebug() << "ID:" << ligne.id
            << "Contenu:" << ligne.contenu
            << "Validation:" << ligne.validation;
    }

    return resultat;
}


int BDD::Poste_liste(const QString& contenu, const QString& validation)
{
    QSqlQuery query;
    query.prepare("INSERT INTO liste (contenu, validation, id_user) VALUES (:contenu, :validation, :id_user)");
    
    qDebug() << id_user;

    query.bindValue(":contenu", contenu);
    query.bindValue(":validation", validation);
    query.bindValue(":id_user", id_user);

    if (!query.exec()) {
        qDebug() << "Erreur SQL:" << query.lastError().text();
        return -1;
    }

    return query.lastInsertId().toInt();
}

void BDD::Poste_contenu_liste(int id_element, int id_liste) {

    QSqlQuery query;
    query.prepare("INSERT INTO contenu_liste (id_element, id_liste) VALUES (:id_element, :id_liste)");

    query.bindValue(":id_element", id_element);
    query.bindValue(":id_liste", id_liste);

    if (!query.exec()) {
        qDebug() << "Erreur SQL:" << query.lastError().text();
        return;
    }
}

void BDD::Poste_element(const QString& nom, const QString& description) {

    QSqlQuery query;
    query.prepare("INSERT INTO element (nom, description) VALUES (:nom, :description)");

    query.bindValue(":nom", nom);
    query.bindValue(":description", description);

    if (!query.exec()) {
        qDebug() << "Erreur SQL:" << query.lastError().text();
        return;
    }

    id_element = query.lastInsertId().toInt();
}