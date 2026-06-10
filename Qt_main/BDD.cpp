#include "BDD.h"
#include <QStandardPaths>
#include <QFile>
#include <QDir>

void BDD::Connect_BDD() {
    QString dbPath;

#ifdef Q_OS_ANDROID
    // Chemin writeable sur Android
    dbPath = QStandardPaths::writableLocation(
        QStandardPaths::AppDataLocation) + "/BDD.db";

    // Copier la BDD depuis les ressources si elle n'existe pas
    if (!QFile::exists(dbPath)) {
        QDir dir;
        dir.mkpath(QStandardPaths::writableLocation(
            QStandardPaths::AppDataLocation));

        QFile::copy(":/BDD/BDD.db", dbPath);
        QFile::setPermissions(dbPath,
            QFile::ReadOwner | QFile::WriteOwner);
    }
#else
    // Windows — chemin local
    dbPath = "BDD.db";
#endif

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbPath);

    if (!db.open()) {
        qDebug() << "Erreur ouverture DB:" << db.lastError().text();
    }
    else {
        qDebug() << "Ouverture DB succès :" << dbPath;
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


/*Liste*/
int BDD::Poste_liste(const QString& contenu, bool validation)
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

std::vector<BDD::LigneListe> BDD::Get_liste() {

    std::vector<LigneListe> resultat;

    QSqlQuery query;
    query.prepare("SELECT * FROM liste WHERE id_user = :id_user");
    query.bindValue(":id_user", id_user);

    if (!query.exec()) {
        qDebug() << "Erreur SQL:" << query.lastError().text();
        return resultat;
    }

    while (query.next()) {

        LigneListe ligne;
        ligne.id = query.value("id").toInt();
        ligne.contenu = query.value("contenu").toString();
        ligne.validation = query.value("validation").toBool();

        resultat.push_back(ligne);

        qDebug() << "ID:" << ligne.id
            << "Contenu:" << ligne.contenu
            << "Validation:" << ligne.validation;
    }

    return resultat;
}

void BDD::modif_liste(int id, QString contenu, bool validation) {
    
    QSqlQuery query;
    query.prepare("UPDATE liste SET contenu = :contenu, validation = :validation WHERE id = :id");
    query.bindValue(":id", id);
    query.bindValue(":contenu", contenu);
    query.bindValue(":validation", validation);

    if (!query.exec()) {
        qDebug() << "Erreur SQL Update_liste :" << query.lastError();
        return;
    }
}

void BDD::delete_liste(int id_liste) {
    
    QSqlQuery query;
    query.prepare("DELETE FROM liste WHERE id = :id");
    query.bindValue(":id", id_liste);

    if (!query.exec()) {
         qDebug() << "Erreur delete_liste :" << query.lastError().text();
    }

}

/*Contenu_liste*/
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

std::vector<BDD::LigneContenueElement>BDD::Get_contenu_liste() {

    std::vector<BDD::LigneContenueElement> data_contenu_liste;

    std::vector<BDD::LigneListe> liste = Get_liste();

    for (int i = 0; i < liste.size(); i++) {

        QSqlQuery query;
        query.prepare("SELECT * FROM contenu_liste WHERE id_liste = :id_liste");
        query.bindValue(":id_liste", liste[i].id);

        if (!query.exec()) {
            qDebug() << "Erreur SQL Get_contenu_liste :" << query.lastError();
            continue;
        }

        while (query.next()) {
            LigneContenueElement Ligne;

            Ligne.id = query.value("id").toInt();
            Ligne.id_element = query.value("id_element").toInt();
            Ligne.id_liste = query.value("id_liste").toInt();

            qDebug() << Ligne.id_element;

            data_contenu_liste.push_back(Ligne);
        }
    }
    return data_contenu_liste;
}

void BDD::delete_contenu_liste(int id_liste) {
    QSqlQuery query;
    query.prepare("DELETE FROM contenu_liste WHERE id_liste = :id");
    query.bindValue(":id", id_liste);
    if (!query.exec())
        qDebug() << "Erreur delete_contenu_liste :" << query.lastError().text();
}

/*Element*/
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

std::vector<BDD::LigneElement> BDD::Get_element()
{
    // Récupère tous les id_element depuis contenu_liste
    std::vector<BDD::LigneContenueElement> data_liste = Get_contenu_liste();

    std::vector<BDD::LigneElement> data_element;

    for (int j = 0; j < data_liste.size(); j++) {

        QSqlQuery query;
        query.prepare("SELECT id, nom, description FROM element WHERE id = :id_element");
        query.bindValue(":id_element", data_liste[j].id_element);

        if (!query.exec()) {
            qDebug() << "Erreur SQL Get_element :" << query.lastError();
            continue;
        }

        while (query.next()) {

            LigneElement elem;

            elem.id = query.value("id").toInt();
            elem.nom = query.value("nom").toString();
            elem.description = query.value("description").toString();

            qDebug() << elem.id;
            qDebug() << elem.nom;
            qDebug() << elem.description;

            data_element.push_back(elem);
        }
    }

    return data_element;
}

void BDD::modif_element(int id, QString nom, QString description) {
    
    QSqlQuery query;
    query.prepare("UPDATE element SET nom = :nom, description = :description WHERE id = :id");
    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":description", description);

    if (!query.exec()) {
        qDebug() << "Erreur SQL Update_liste :" << query.lastError();
        return;
    }

    while (query.next()) {

        qDebug() << "id = " << id;
        qDebug() << "nom = " << nom;
        qDebug() << "description = " << description;
    }
}