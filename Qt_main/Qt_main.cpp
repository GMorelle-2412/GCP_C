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

    std::vector<BDD::LigneListe> liste = class_BDD.Get_liste();

    for (int i = 0; i < liste.size(); i++) {

        QWidget* ligne = new QWidget(this);
        QHBoxLayout* layout = new QHBoxLayout(ligne);
        layout->setContentsMargins(0, 0, 0, 0);

        QCheckBox* validation = new QCheckBox(ligne);
        validation->setChecked(liste[i].validation == "1");
        layout->addWidget(validation);

        QLabel* contenu = new QLabel(liste[i].contenu, ligne);
        layout->addWidget(contenu);

        ui->verticalLayout_9->addWidget(ligne);
    }

}


Qt_main::~Qt_main()
{
    delete ui; 
}
