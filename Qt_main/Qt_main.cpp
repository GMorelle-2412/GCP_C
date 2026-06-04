#include "Qt_main.h"

Qt_main::Qt_main(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::Qt_mainClass) 
{
    ui->setupUi(this);

    /*Connection à la BDD*/
    class_BDD.Connect_BDD();

    /*Page de démarage*/
    ui->stackedWidget->setCurrentIndex(0);

    /*Appel de fonction*/
    //identification
    page_connection();
    bouton_connection();

    page_inscription();
    bouton_inscription();

    bouton_annulation();

    //Page principale
    bouton_deconection();
    bouton_creation_projet();

    //création de projet
    ajouter_liste();
    creation_projet();

}


void Qt_main::page_connection() {
    connect(ui->pushButton_2, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentIndex(2);

        });
}

void Qt_main::page_inscription() {
    connect(ui->pushButton, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentIndex(1);

        });
}

void Qt_main::bouton_connection(){
    connect(ui->pushButton_5, &QPushButton::clicked, this, [this]() {

        QString nom = ui->lineEdit_3->text();
        QString mdp = ui->lineEdit_4->text();

        if (nom.isEmpty() || mdp.isEmpty()) {
            qDebug() << "Champs vides";
            return;
        }

        class_BDD.Connection(nom, mdp);

        ui->stackedWidget->setCurrentIndex(3);

        affiche_info_user();

        affiche_element_liste();
        });
}

void Qt_main::bouton_inscription(){
    connect(ui->pushButton_3, &QPushButton::clicked, this, [this]() {

        QString nom = ui->lineEdit->text();
        QString mdp = ui->lineEdit_2->text();

        if (nom.isEmpty() || mdp.isEmpty()) {
            qDebug() << "Champs vides";
            return;
        }

        class_BDD.Inscription(nom, mdp);

        ui->stackedWidget->setCurrentIndex(3);
        });
}

void Qt_main::affiche_info_user() {
    ui->label_6->setText(class_BDD.nom_user);
}

void Qt_main::bouton_deconection() {
    connect(ui->pushButton_7, &QPushButton::clicked, this, [this]() {
       
        class_BDD.id_user = 0;
        class_BDD.nom_user = "nul";

        while (ui->verticalLayout_9->count() > 0) {
            QLayoutItem* item = ui->verticalLayout_9->takeAt(0);

            if (item->widget())
                item->widget()->deleteLater();

            delete item;
        }

        ui->stackedWidget->setCurrentIndex(0);
        });

}

void Qt_main::bouton_annulation() {
    //connection
    connect(ui->pushButton_6, &QPushButton::clicked, this, [this] (){
        ui->stackedWidget->setCurrentIndex(0);
    });

    //inscription
    connect(ui->pushButton_4, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentIndex(0);
    });

    //Création élément 
    connect(ui->pushButton_9, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentIndex(3);

        // Vider le layout
        while (ui->verticalLayout_8->count() > 0) {
            QLayoutItem* item = ui->verticalLayout_8->takeAt(0);

            if (item->widget())
                item->widget()->deleteLater();

            delete item;
        }
        });

    //Modif element
    connect(ui->pushButton_14, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentIndex(3);
        
        // Vider le layout
        while (ui->verticalLayout_12->count() > 0) {
            QLayoutItem* item = ui->verticalLayout_12->takeAt(0);

            if (item->widget())
                item->widget()->deleteLater();

            delete item;
        }
        });
}

void Qt_main::bouton_creation_projet() {
    connect(ui->pushButton_8, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentIndex(4);
        });
}

void Qt_main::ajouter_liste(){

    connect(ui->pushButton_11, &QPushButton::clicked, this, [this]() {

        QWidget* ligne = new QWidget(this);
        QHBoxLayout* layoutLigne = new QHBoxLayout(ligne);
        layoutLigne->setContentsMargins(0, 0, 0, 0);

        QCheckBox* check_box = new QCheckBox(ligne);
        QLineEdit* edit = new QLineEdit(ligne);
        QPushButton* btnSupprimer = new QPushButton("X", ligne);
        btnSupprimer->setFixedWidth(30);

        layoutLigne->addWidget(check_box);
        layoutLigne->addWidget(edit);
        layoutLigne->addWidget(btnSupprimer);

        ui->verticalLayout_8->addWidget(ligne);

        connect(btnSupprimer, &QPushButton::clicked, this, [this, ligne]() {
            ui->verticalLayout_8->removeWidget(ligne);
            ligne->deleteLater();
            });
        });
}

void Qt_main::creation_projet(){
    connect(ui->pushButton_10, &QPushButton::clicked, this, [this]() {

        // 1) Enregistrer l’élément principal
        QString nom = ui->lineEdit_5->text();
        QString description = ui->lineEdit_6->text();

        class_BDD.Poste_element(nom, description);   // met à jour id_element

        class_BDD.id_liste.clear(); // IMPORTANT : vider avant de remplir

        // 2) Enregistrer chaque ligne
        for (int i = 0; i < ui->verticalLayout_8->count(); i++) {

            QWidget* ligne = ui->verticalLayout_8->itemAt(i)->widget();
            if (!ligne) continue;

            QLineEdit* edit = ligne->findChild<QLineEdit*>();
            QCheckBox* check = ligne->findChild<QCheckBox*>();

            if (!edit || !check) continue;

            QString contenu = edit->text();
            bool validation = check->isChecked();

            // Récupérer l’ID de la ligne insérée
            int id = class_BDD.Poste_liste(contenu, validation);

            // Stocker l’ID
            class_BDD.id_liste.push_back(id);
        }

        // 3) Créer les relations dans contenu_liste
        for (int id_l : class_BDD.id_liste) {
            class_BDD.Poste_contenu_liste(class_BDD.id_element, id_l);
        }

        // 4) Nettoyer l’interface
        while (ui->verticalLayout_8->count() > 0) {
            QLayoutItem* item = ui->verticalLayout_8->takeAt(0);
            if (item->widget()) item->widget()->deleteLater();
            delete item;
        }

        ui->stackedWidget->setCurrentIndex(3);
        });
    }

void Qt_main::affiche_element_liste() {

    std::vector<BDD::LigneListe> data_liste = class_BDD.Get_liste();
    std::vector<BDD::LigneContenueElement> data_contenue_liste = class_BDD.Get_contenu_liste();
    std::vector<BDD::LigneElement> data_element = class_BDD.Get_element();

    int verification_element = 0;

    for (int i = 0; i < data_liste.size(); i++) {

        QWidget* projet = new QWidget(this);
        QVBoxLayout* zone_projet = new QVBoxLayout(projet);
        zone_projet->setContentsMargins(0, 0, 0, 0);


        for (int j = 0; j < data_contenue_liste.size(); j++) {

            if (data_liste[i].id == data_contenue_liste[j].id_liste) {

                for (int k = 0; k < data_element.size(); k++) {

                    if (data_contenue_liste[j].id_element == data_element[k].id) {

                        if (verification_element != data_contenue_liste[j].id_element) {

                            QLabel* nom = new QLabel(data_element[k].nom, projet);
                            zone_projet->addWidget(nom);

                            QLabel* description = new QLabel(data_element[k].description, projet);
                            zone_projet->addWidget(description);

                            verification_element = data_contenue_liste[j].id_element;

                            QPushButton* modif = new QPushButton("Modification", projet);
                            zone_projet->addWidget(modif);

                            // Appel correct
                            affichage_modif_projet(data_element[k].id, data_element, data_liste, data_contenue_liste, modif);

                        }
                    }
                }
            }
        }

        // Ligne contenant checkbox + nom de la liste
        QWidget* ligne = new QWidget(projet);
        QHBoxLayout* layout = new QHBoxLayout(ligne);
        layout->setContentsMargins(0, 0, 0, 0);

        QCheckBox* validation = new QCheckBox(ligne);
        validation->setChecked(data_liste[i].validation);
        layout->addWidget(validation);

        QLabel* contenu = new QLabel(data_liste[i].contenu, ligne);
        layout->addWidget(contenu);

        QPushButton* supp = new QPushButton("Suppression", ligne);
        layout->addWidget(supp);

        // On ajoute la ligne dans le projet
        zone_projet->addWidget(ligne);

        // On ajoute le projet dans le layout principal
        ui->verticalLayout_9->addWidget(projet);
    }
}

void Qt_main::affichage_modif_projet(int id_element, std::vector<BDD::LigneElement> data_element, std::vector<BDD::LigneListe> data_liste, const std::vector<BDD::LigneContenueElement> data_contenue_liste, QPushButton* modif){

    connect(modif, &QPushButton::clicked, this, [this, id_element, data_element, data_liste, data_contenue_liste]() {

            ui->stackedWidget->setCurrentIndex(5);

            std::vector<BDD::LigneListe> data_liste = class_BDD.Get_liste();

            //vector_ligne_contenu.clear();

            int sauve_num = 0;

            for (int n = 0; n < data_liste.size(); n++) {

                if (data_contenue_liste[n].id_element == id_element &&
                    data_contenue_liste[n].id_liste == data_liste[n].id) {

                    //ligne_contenu ligne;

                    int id_liste = data_contenue_liste[n].id_liste;

                    QWidget* projet = new QWidget(this);
                    QVBoxLayout* zone_projet = new QVBoxLayout(projet);

                    QWidget* ligneWidget = new QWidget(projet);
                    QHBoxLayout* layout = new QHBoxLayout(ligneWidget);

                    QCheckBox* validation = new QCheckBox(ligneWidget);
                    validation->setChecked(data_liste[n].validation);
                    qDebug() << validation;
                    //ligne.validation = validation;
                    validation->setObjectName("validation_" + QString::number(id_liste));
                    layout->addWidget(validation);

                    QLineEdit* contenu = new QLineEdit(data_liste[n].contenu, ligneWidget);
                    //ligne.contenu = contenu;
                    contenu->setObjectName("contenu_" + QString::number(id_liste));
                    layout->addWidget(contenu);

                    QPushButton* supp = new QPushButton("Suppression", ligneWidget);
                    layout->addWidget(supp);

                    modif_projet(id_element, data_element, data_liste, data_contenue_liste);

                    zone_projet->addWidget(ligneWidget);
                    ui->verticalLayout_12->addWidget(projet);

                    //vector_ligne_contenu.push_back(ligne);

                    sauve_num++;
                }
            }

            ajout_liste_modif(id_element, data_contenue_liste);
        });
}

void Qt_main::modif_projet(int id_element, std::vector<BDD::LigneElement> data_element, std::vector<BDD::LigneListe> data_liste, const std::vector<BDD::LigneContenueElement> data_contenue_liste) {


    connect(ui->pushButton_13, &QPushButton::clicked, this, [this, id_element, data_element, data_liste, data_contenue_liste]() {
        
        //NB Liste
        int max_liste = 0;
        for (int f = 0; f < data_contenue_liste.size(); f++) {

            for (int h = 0; h < data_liste.size(); h++) {

                if (id_element == data_contenue_liste[f].id_element) {

                    if (data_contenue_liste[f].id_liste == data_liste[h].id) {
                        max_liste++;
                    }
                }
            }
        }

        //element
        for (int y = 0; y < data_element.size(); y++) {

            if (id_element == data_element[y].id) {
                class_BDD.modif_element(id_element, ui->lineEdit_7->text(), ui->lineEdit_8->text());
            }

        }

        //liste
        if (ui->verticalLayout_12->count() == max_liste) {

            for (int i = 0; i < data_liste.size(); i++) {

                int id_liste = data_liste[i].id;

                // Retrouver les widgets dynamiques
                QLineEdit* contenu = this->findChild<QLineEdit*>("contenu_" + QString::number(id_liste));
                QCheckBox* validation = this->findChild<QCheckBox*>("validation_" + QString::number(id_liste));

                if (!contenu || !validation)
                    continue; // Le widget n'existe pas → on ignore


                QString texte = contenu->text();
                bool valide = validation->isChecked();

                if (!validation) {
                    qDebug() << "QCheckBox introuvable pour id_liste =" << id_liste;
                }
                else {
                    qDebug() << "État =" << validation->isChecked();
                }

                class_BDD.modif_liste(id_liste, texte, valide);
            }
        }

        if (ui->verticalLayout_12->count() < max_liste){
            int reste = max_liste - ui->verticalLayout_12->count();

            int max_update = 0;

            //update
            if (max_update == ui->verticalLayout_12->count()) {
                qDebug() << "fin de update";
            }
            else {
                for (int i = 0; i < data_liste.size(); i++) {

                    int id_liste = data_liste[i].id;

                    // Retrouver les widgets dynamiques
                    QLineEdit* contenu = this->findChild<QLineEdit*>("contenu_" + QString::number(id_liste));
                    QCheckBox* validation = this->findChild<QCheckBox*>("validation_" + QString::number(id_liste));

                    if (!contenu || !validation)
                        continue;


                    QString texte = contenu->text();
                    bool valide = validation->isChecked();

                    if (!validation) {
                        qDebug() << "QCheckBox introuvable pour id_liste =" << id_liste;
                    }
                    else {
                        qDebug() << "État =" << validation->isChecked();
                    }

                    class_BDD.modif_liste(id_liste, texte, valide);

                    max_update++;
                }
            }

            //delete
            // delete
            if (reste != 0) {

                // On supprime les derniers éléments de la BDD
                // Exemple : si reste = 2 → on supprime les 2 derniers ID
                for (int i = 0; i < reste; i++) {

                    // Récupérer le dernier ID existant dans la BDD
                    int last_id = data_liste.back().id;

                    // Suppression dans la BDD
                    class_BDD.delete_liste(last_id);

                    // Retirer aussi de ton vecteur local
                    //data_liste.pop_back();

                    qDebug() << "Suppression de l'ID =" << last_id;
                }
            }

        }

        if (ui->verticalLayout_12->count() > max_liste){

            int reste = ui->verticalLayout_12->count() - max_liste;

            int max_update = 0;

            //update
            if (max_update == max_liste) {
                qDebug() << "fin de update";
            }
            else {
                for (int i = 0; i < data_liste.size(); i++) {

                    int id_liste = data_liste[i].id;

                    // Retrouver les widgets dynamiques
                    QLineEdit* contenu = this->findChild<QLineEdit*>("contenu_" + QString::number(id_liste));
                    QCheckBox* validation = this->findChild<QCheckBox*>("validation_" + QString::number(id_liste));

                    if (!contenu || !validation)
                        continue; // Le widget n'existe pas → on ignore


                    QString texte = contenu->text();
                    bool valide = validation->isChecked();

                    if (!validation) {
                        qDebug() << "QCheckBox introuvable pour id_liste =" << id_liste;
                    }
                    else {
                        qDebug() << "État =" << validation->isChecked();
                    }

                    class_BDD.modif_liste(id_liste, texte, valide);

                    max_update++;
                }
            }

            //poste
            if (reste != 0) {

                // Création d’un nouvel ID (exemple)
                int new_id = data_liste.back().id + 1;

                // Retrouver les widgets dynamiques du dernier élément
                QLineEdit* contenu = this->findChild<QLineEdit*>("contenu_" + QString::number(new_id));
                QCheckBox* validation = this->findChild<QCheckBox*>("validation_" + QString::number(new_id));

                if (!contenu || !validation) {
                    qDebug() << "Impossible de trouver les widgets pour new_id =" << new_id;
                    return;
                }

                QString texte = contenu->text();
                bool valide = validation->isChecked();

                // Enregistrer dans la base
                class_BDD.Poste_liste(texte, valide);

                // décrémenter reste
                reste -= 1;
            }

        }
    });

}

void Qt_main::ajout_liste_modif(int id_element, const std::vector<BDD::LigneContenueElement> data_contenue_liste) {

    connect(ui->pushButton_12, &QPushButton::clicked, this, [this, id_element, data_contenue_liste]() {

        std::vector<BDD::LigneListe> data_liste = class_BDD.Get_liste();

        int new_id = data_liste.back().id + 1;

        // 3) Création du widget principal
        QWidget* projet = new QWidget(this);
        QVBoxLayout* zone_projet = new QVBoxLayout(projet);

        QWidget* ligneWidget = new QWidget(projet);
        QHBoxLayout* layout = new QHBoxLayout(ligneWidget);

        // 4) Checkbox
        QCheckBox* validation = new QCheckBox(ligneWidget);
        validation->setObjectName("validation_" + QString::number(new_id));
        layout->addWidget(validation);

        // 5) LineEdit (contenu vide ou valeur par défaut)
        QLineEdit* contenu = new QLineEdit("", ligneWidget);
        contenu->setObjectName("contenu_" + QString::number(new_id));
        layout->addWidget(contenu);

        // 6) Bouton suppression
        QPushButton* supp = new QPushButton("Suppression", ligneWidget);
        layout->addWidget(supp);

        // 7) Ajout au layout principal
        zone_projet->addWidget(ligneWidget);
        ui->verticalLayout_12->addWidget(projet);

        // 8) Incrémenter ton compteur
        conteur_ajout_liste++;
        });
}


Qt_main::~Qt_main()
{
    delete ui; 
}