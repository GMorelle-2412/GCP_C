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
            QString validation = check->isChecked() ? "1" : "0";

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

                            modif_projet();

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
        validation->setChecked(data_liste[i].validation == "1");
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




    /*for (int i = 0; i < data_liste.size(); i++) {

        QWidget* ligne = new QWidget(this);
        QHBoxLayout* layout = new QHBoxLayout(ligne);
        layout->setContentsMargins(0, 0, 0, 0);

        QCheckBox* validation = new QCheckBox(ligne);
        validation->setChecked(data_liste[i].validation == "1");
        layout->addWidget(validation);

        QLabel* contenu = new QLabel(data_liste[i].contenu, ligne);
        layout->addWidget(contenu);

        ui->verticalLayout_9->addWidget(ligne);
    }*/

    

}

void Qt_main::affichage_modif_projet(int id_element, std::vector<BDD::LigneElement> data_element, std::vector<BDD::LigneListe> data_liste, const std::vector<BDD::LigneContenueElement> data_contenue_liste, QPushButton* modif){

    connect(modif, &QPushButton::clicked, this, [this, id_element, data_element, data_liste, data_contenue_liste]() {

            ui->stackedWidget->setCurrentIndex(5);

            std::vector<BDD::LigneListe> data_liste = class_BDD.Get_liste();

            for (int i = 0; i < data_element.size(); i++) {
                if (id_element == data_element[i].id) {
                    ui->lineEdit_7->setText(data_element[i].nom);
                    ui->lineEdit_8->setText(data_element[i].description);
                }
            }

            for (int n = 0; n < data_liste.size(); n++) {

                if (data_contenue_liste[n].id_element == id_element) {
                    if (data_contenue_liste[n].id_liste == data_liste[n].id) {

                        QWidget* projet = new QWidget(this);
                        QVBoxLayout* zone_projet = new QVBoxLayout(projet);
                        zone_projet->setContentsMargins(0, 0, 0, 0);

                        QWidget* ligne = new QWidget(projet);
                        QHBoxLayout* layout = new QHBoxLayout(ligne);
                        layout->setContentsMargins(0, 0, 0, 0);

                        QCheckBox* validation = new QCheckBox(ligne);
                        validation->setChecked(data_liste[n].validation == "1");
                        layout->addWidget(validation);

                        QLabel* contenu = new QLabel(data_liste[n].contenu, ligne);
                        layout->addWidget(contenu);

                        QPushButton* supp = new QPushButton("Suppression", ligne);
                        layout->addWidget(supp);

                        zone_projet->addWidget(ligne);
                        ui->verticalLayout_12->addWidget(projet);
                    }
                }
            }
            
        });
}

void Qt_main::modif_projet() {

}

Qt_main::~Qt_main()
{
    delete ui; 
}
