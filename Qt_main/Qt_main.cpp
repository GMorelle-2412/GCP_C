#include "Qt_main.h"
#include "Style.h"

Qt_main::Qt_main(QWidget* parent)
    : QMainWindow(parent),
    ui(new Ui::Qt_mainClass)
{
    ui->setupUi(this);

    Style::appliquerStyle(this);

    class_BDD.Connect_BDD();

    ui->stackedWidget->setCurrentIndex(0);

    page_connection();
    bouton_connection();

    page_inscription();
    bouton_inscription();

    bouton_annulation();

    bouton_deconection();
    bouton_creation_projet();

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

        conteur_ajout_liste = 0;

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

void Qt_main::affichage_modif_projet(int id_element, std::vector<BDD::LigneElement> data_element, std::vector<BDD::LigneListe> data_liste, const std::vector<BDD::LigneContenueElement> data_contenue_liste, QPushButton* modif)
{
    connect(modif, &QPushButton::clicked, this,
        [this, id_element, data_element, data_liste, data_contenue_liste]()
        {
            ui->stackedWidget->setCurrentIndex(5);

            // 1) Nettoyer le layout avant d'ajouter
            while (ui->verticalLayout_12->count() > 0) {
                QLayoutItem* item = ui->verticalLayout_12->takeAt(0);
                if (item->widget())
                    item->widget()->deleteLater();
                delete item;
            }

            // 2) Affichage des infos de l’élément
            for (auto& e : data_element) {
                if (e.id == id_element) {
                    ui->lineEdit_7->setText(e.nom);
                    ui->lineEdit_8->setText(e.description);
                    break;
                }
            }

            // 3) Création des lignes
            for (auto& c : data_contenue_liste) {
                if (c.id_element != id_element)
                    continue;

                for (auto& l : data_liste) {
                    if (l.id == c.id_liste) {

                        table_id_liste_modif.push_back(l.id);

                        QWidget* projet = new QWidget(this);
                        QVBoxLayout* zone_projet = new QVBoxLayout(projet);

                        QWidget* ligneWidget = new QWidget(projet);
                        QHBoxLayout* layout = new QHBoxLayout(ligneWidget);

                        QCheckBox* validation = new QCheckBox(ligneWidget);
                        validation->setChecked(l.validation);
                        layout->addWidget(validation);

                        QLineEdit* contenu = new QLineEdit(l.contenu, ligneWidget);
                        layout->addWidget(contenu);

                        QPushButton* supp = new QPushButton("Suppression", ligneWidget);
                        layout->addWidget(supp);

                        zone_projet->addWidget(ligneWidget);
                        ui->verticalLayout_12->addWidget(projet);

                        connect(supp, &QPushButton::clicked, this, [projet]() {
                            projet->deleteLater();
                            });
                    }
                }
            }

            // 4) Connecter UNE SEULE FOIS le bouton d’ajout
            ajout_liste_modif(id_element, data_contenue_liste);

            // 5) Connecter UNE SEULE FOIS le bouton de modification
            modif_projet(id_element, data_element, data_liste, data_contenue_liste);
        });
}

void Qt_main::ajout_liste_modif(int id_element, const std::vector<BDD::LigneContenueElement>data_contenue_liste){

    // Empêche les connexions multiples
    disconnect(ui->pushButton_12, nullptr, this, nullptr);

    connect(ui->pushButton_12, &QPushButton::clicked, this, [this, id_element, data_contenue_liste](){

            QWidget* projet = new QWidget(this);
            QVBoxLayout* zone_projet = new QVBoxLayout(projet);

            QWidget* ligneWidget = new QWidget(projet);
            QHBoxLayout* layout = new QHBoxLayout(ligneWidget);

            QCheckBox* validation = new QCheckBox(ligneWidget);
            layout->addWidget(validation);

            QLineEdit* contenu = new QLineEdit("", ligneWidget);
            layout->addWidget(contenu);

            QPushButton* supp = new QPushButton("X", ligneWidget);
            layout->addWidget(supp);

            zone_projet->addWidget(ligneWidget);
            ui->verticalLayout_12->addWidget(projet);

            connect(supp, &QPushButton::clicked, this, [projet]() {
                projet->deleteLater();
                });
        });
}

void Qt_main::modif_projet(int id_element, std::vector<BDD::LigneElement> data_element, std::vector<BDD::LigneListe>data_liste, const std::vector<BDD::LigneContenueElement> data_contenue_liste){

    disconnect(ui->pushButton_13, nullptr, this, nullptr);

    connect(ui->pushButton_13, &QPushButton::clicked, this, [this, id_element, data_element, data_liste, data_contenue_liste]() {
        
        //element
        for (int y = 0; y < data_element.size(); y++) {

            if (id_element == data_element[y].id) {
                class_BDD.modif_element(id_element, ui->lineEdit_7->text(), ui->lineEdit_8->text());
            }

        }

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

        //reste
        if (verife_reste_liste == -1) {
            int reste = ui->verticalLayout_12->count() - max_liste;
            if (reste < 0)reste = reste * (-1);

            verife_reste_liste = reste;
        }

        //liste
        //update
        int compteur_update = 0;
        int nb = ui->verticalLayout_12->count();

        for (int i = 0; i < max_liste; i++) {

            QLayoutItem* item = ui->verticalLayout_12->itemAt(i);
            if (!item) continue;

            QWidget* ligne = item->widget();
            if (!ligne) continue;

            QLineEdit* contenu = ligne->findChild<QLineEdit*>();
            QCheckBox* validation = ligne->findChild<QCheckBox*>();

            if (!contenu || !validation)
                continue;

            QString texte = contenu->text();
            bool valide = validation->isChecked();

            qDebug() << "Ligne" << i << "Texte:" << texte << "Valide:" << valide;

            // Mise à jour BDD
            class_BDD.modif_liste(table_id_liste_modif[i], texte, valide);
        
            compteur_update++;
        }

        //poste
        if (nb > max_liste) {

            for (int i = 0; i < nb - max_liste; i++) {

                QLayoutItem* item = ui->verticalLayout_12->itemAt(i + compteur_update);
                if (!item) continue;

                QWidget* ligne = item->widget();
                if (!ligne) continue;

                QLineEdit* contenu = ligne->findChild<QLineEdit*>();
                QCheckBox* validation = ligne->findChild<QCheckBox*>();

                if (!contenu || !validation) continue;

                QString texte = contenu->text();
                bool valide = validation->isChecked();

                qDebug() << "Nouvelle Ligne" << i << "Texte:" << texte << "Valide:" << valide;
            
                int id_liste = class_BDD.Poste_liste(texte, valide);

                class_BDD.Poste_contenu_liste(id_element, id_liste);
            }
        }

        // delete
        if (nb < max_liste) {

            int nb_delete = max_liste - nb;  // nombre de lignes à supprimer

            qDebug() << "Suppression de" << nb_delete << "lignes dans la BDD";

            // On supprime les dernières lignes de la BDD
            for (int i = 0; i < nb_delete; i++) {

                // Récupérer le dernier id_liste dans table_id_liste_modif
                int id_liste = table_id_liste_modif.back();
                table_id_liste_modif.pop_back();

                qDebug() << "Suppression id_liste =" << id_liste;

                // Suppression dans la BDD
                class_BDD.delete_liste(id_liste);
                class_BDD.delete_contenu_liste(id_liste);
            }
        }
    });

}


Qt_main::~Qt_main()
{
    delete ui; 
}