#include "BDD.h"

BDD::BDD(QObject* parent) : QObject(parent) {}
BDD::~BDD() {}

/*Connexion BDD*/
void BDD::Connect_BDD()
{
    QString dbPath;

#ifdef Q_OS_ANDROID
    dbPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/BDD.db";

    if (!QFile::exists(dbPath)) {
        QDir().mkpath(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
        QFile::copy(":/BDD/BDD.db", dbPath);
        QFile::setPermissions(dbPath, QFile::ReadOwner | QFile::WriteOwner);
    }
#else
    dbPath = "BDD.db";
#endif

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbPath);

    if (!db.open())
        qDebug() << "Erreur ouverture DB:" << db.lastError().text();
    else
        qDebug() << "Ouverture DB succès :" << dbPath;
}


/*Users*/
void BDD::Connection(const QString& nom, const QString& mdp)
{
    QSqlQuery query(QSqlDatabase::database());
    query.prepare("SELECT id FROM users WHERE nom = :nom AND mot_de_passe = :mdp");
    query.bindValue(":nom", nom);
    query.bindValue(":mdp", mdp);

    if (!query.exec()) {
        qDebug() << "Erreur SQL Connection:" << query.lastError().text();
        return;
    }

    if (query.next()) {
        id_user = query.value(0).toInt();
        nom_user = nom;
        return;
    }

    return;
}

void BDD::Connection_auto(const QString& nom)
{
    QSqlQuery query(QSqlDatabase::database());
    query.prepare("SELECT id FROM users WHERE nom = :nom");
    query.bindValue(":nom", nom);

    if (!query.exec()) {
        qDebug() << "Erreur SQL Connection_auto:" << query.lastError().text();
        return;
    }

    if (query.next()) {
        id_user = query.value(0).toInt();
        nom_user = nom;
    }
    else {
        qDebug() << "Auto-login échoué : utilisateur introuvable";
    }
}

void BDD::Inscription(const QString& nom, const QString& mdp)
{
    QSqlQuery query(QSqlDatabase::database());
    query.prepare("INSERT INTO users (nom, mot_de_passe) VALUES (:nom, :mdp)");
    query.bindValue(":nom", nom);
    query.bindValue(":mdp", mdp);

    if (!query.exec())
        qDebug() << "Erreur SQL Inscription:" << query.lastError().text();
}


/*Liste*/
std::vector<BDD::LigneListe> BDD::Get_liste()
{
    std::vector<LigneListe> resultat;

    QSqlQuery query(QSqlDatabase::database());
    query.prepare("SELECT * FROM liste WHERE id_user = :id_user");
    query.bindValue(":id_user", 3);

    if (!query.exec()) {
        qDebug() << "Erreur SQL Get_liste:" << query.lastError().text();
        return resultat;
    }

    while (query.next()) {
        LigneListe ligne;
        ligne.id = query.value("id").toInt();
        ligne.contenu = query.value("contenu").toString();
        ligne.validation = query.value("validation").toBool();
        resultat.push_back(ligne);
    }

    return resultat;
}

int BDD::Poste_liste(const QString& contenu, bool validation)
{
    QSqlQuery query(QSqlDatabase::database());
    query.prepare("INSERT INTO liste (contenu, validation, id_user) VALUES (:contenu, :validation, :id_user)");
    query.bindValue(":contenu", contenu);
    query.bindValue(":validation", validation);
    query.bindValue(":id_user", 3);

    if (!query.exec()) {
        qDebug() << "Erreur SQL Poste_liste:" << query.lastError().text();
        return -1;
    }

    return query.lastInsertId().toInt();
}

void BDD::modif_liste(int id, const QString& contenu, bool validation)
{
    QSqlQuery query(QSqlDatabase::database());
    query.prepare("UPDATE liste SET contenu = :contenu, validation = :validation WHERE id = :id");
    query.bindValue(":id", id);
    query.bindValue(":contenu", contenu);
    query.bindValue(":validation", validation);

    if (!query.exec())
        qDebug() << "Erreur SQL modif_liste:" << query.lastError().text();
}

void BDD::delete_liste(int id_liste)
{
    QSqlQuery query(QSqlDatabase::database());
    query.prepare("DELETE FROM liste WHERE id = :id");
    query.bindValue(":id", id_liste);

    if (!query.exec())
        qDebug() << "Erreur SQL delete_liste:" << query.lastError().text();
}


/*Contenu_element_listes*/
std::vector<BDD::LigneContenueElement> BDD::Get_contenu() {

    std::vector<LigneContenueElement> resultat;

        for (const LigneListe& item : Get_liste()) {
            QSqlQuery query(QSqlDatabase::database());
            query.prepare("SELECT * FROM contenu_element_listes WHERE id_liste = :id_liste");
            query.bindValue(":id_liste", item.id);

            if (!query.exec()) {
                qDebug() << "Erreur SQL Get_contenu:" << query.lastError().text();
                continue;
            }

            while (query.next()) {
                LigneContenueElement ligne;
                ligne.id = query.value("id").toInt();
                ligne.id_element = query.value("id_element").toInt();
                ligne.id_liste = query.value("id_liste").toInt();
                resultat.push_back(ligne);
            }
        }
    
    return resultat;
}

void BDD::Poste_contenu(int id_element, int id_liste){

    QSqlQuery query(QSqlDatabase::database());

    query.prepare("INSERT INTO contenu_element_listes (id_element, id_liste) VALUES (:id_element, :id_liste)");
    query.bindValue(":id_element", id_element);
    query.bindValue(":id_liste", id_liste);

    if (!query.exec())qDebug() << "Erreur SQL Poste_contenu:" << query.lastError().text();
    
    return;
}

void BDD::delete_contenu(int id_liste)
{
    QSqlQuery query(QSqlDatabase::database());
    query.prepare("DELETE FROM contenu_element_listes WHERE id_liste = :id");
    query.bindValue(":id", id_liste);

    if (!query.exec())
        qDebug() << "Erreur SQL delete_contenu:" << query.lastError().text();
}


/*Contenu_element_notes*/
std::vector<BDD::Sauvegarde_Contenu_element_notes> BDD::Get_Contenu_element_notes(int id_element) {
    
    std::vector<BDD::Sauvegarde_Contenu_element_notes> sauvegarde;


        QSqlQuery query(QSqlDatabase::database());
        query.prepare("SELECT * FROM contenu_element_notes WHERE id_element = :id_element");
        query.bindValue(":id_element", id_element);

        if (!query.exec()) {
            qDebug() << "Erreur SQL Get_contenu:" << query.lastError().text();
            return sauvegarde;
        }

        while (query.next()) {
            Sauvegarde_Contenu_element_notes ligne;

            ligne.id = query.value("id").toInt();
            ligne.id_element = query.value("id_element").toInt();
            ligne.id_notes = query.value("id_notes").toInt();

            sauvegarde.push_back(ligne);
        }
    
    return sauvegarde;
}

int BDD::Poste_Contenu_element_notes(int id_element, int id_notes) {
    QSqlQuery query(QSqlDatabase::database());

    query.prepare("INSERT INTO contenu_element_notes (id_element, id_notes) VALUES (:id_element, :id_notes)");

    query.bindValue(":id_element", id_element);
    query.bindValue(":id_notes", id_notes);

    if (!query.exec()) {
        qDebug() << "Erreur SQL Poste_liste:" << query.lastError().text();
        return -1;
    }
}

int BDD::modif_note(int id_note, const QString& nom, const QString& texte) {
    
    QSqlQuery query;
    query.prepare("UPDATE notes SET nom = :nom, text = :text WHERE id = :id");
    query.bindValue(":nom", nom);
    query.bindValue(":text", texte);
    query.bindValue(":id", id_note);

    if (!query.exec()) {
        qDebug() << "Erreur modif_note:" << query.lastError().text();
        return -1;
    }

    return 0;
}


/*Element*/
std::vector<BDD::LigneElement> BDD::Get_element() {

    std::vector<LigneElement> resultat;

    for (const LigneContenueElement& item : Get_contenu()) {
        QSqlQuery query(QSqlDatabase::database());
        query.prepare("SELECT id, nom, description FROM element WHERE id = :id");
        query.bindValue(":id", item.id_element);

        if (!query.exec()) {
            qDebug() << "Erreur SQL Get_element:" << query.lastError().text();
            continue;
        }

        if (query.next()) {
            LigneElement elem;
            elem.id = query.value("id").toInt();
            elem.nom = query.value("nom").toString();
            elem.description = query.value("description").toString();
            resultat.push_back(elem);
        }
    }

    return resultat;
}

int BDD::Poste_element(const QString& nom, const QString& description){

    QSqlQuery query(QSqlDatabase::database());

    query.prepare("INSERT INTO element (nom, description) VALUES (:nom, :description)");
    query.bindValue(":nom", nom);
    query.bindValue(":description", description);

    if (!query.exec()) {
        qDebug() << "Erreur SQL Poste_element:" << query.lastError().text();
        return 0;
    }

    return id_element = query.lastInsertId().toInt();
}

void BDD::modif_element(int id, const QString& nom, const QString& description)
{
    QSqlQuery query(QSqlDatabase::database());
    query.prepare("UPDATE element SET nom = :nom, description = :description WHERE id = :id");
    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":description", description);

    if (!query.exec())
        qDebug() << "Erreur SQL modif_element:" << query.lastError().text();
}


/*Notes*/
std::vector<BDD::LigneNote> BDD::Get_note(int id_note) {
    
    std::vector<LigneNote> Note;

    QSqlQuery query(QSqlDatabase::database());
    query.prepare("SELECT * FROM notes WHERE id = :id");

    query.bindValue(":id", id_note);

    if (!query.exec()) {
        qDebug() << "Erreur SQL Get_element:" << query.lastError().text();
        return Note;
    }

    if (query.next()) {
        LigneNote note_info;

        note_info.id = query.value("id").toInt();
        note_info.text = query.value("text").toString();
        note_info.nom = query.value("nom").toString();

        Note.push_back(note_info);
    }

    return Note;
}

int BDD::Poste_note(const QString& text, const QString& nom) {

    QSqlQuery query(QSqlDatabase::database());
    query.prepare("INSERT INTO notes (text, nom) VALUES (:text, :nom)");
    query.bindValue(":text", text);
    query.bindValue(":nom", nom);

    if (!query.exec())
        qDebug() << "Erreur SQL Poste_note :" << query.lastError().text();

    return query.lastInsertId().toInt();
}
