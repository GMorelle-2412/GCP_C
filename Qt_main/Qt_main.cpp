#include "Qt_main.h"

Qt_main::Qt_main(QWidget* parent)
    : QMainWindow(parent),
      ui(new Ui::Qt_mainClass),
      class_BDD(new BDD()),
      class_select(new select()),
      class_identification(new identification()),
      class_style(new Style())
{
    ui->setupUi(this);     


    class_BDD->Connect_BDD();


    ui->stackedWidget->setCurrentIndex(3);

    
    select_affichage_projets();


    class_select->zone_creation_projet(ui->stackedWidget, ui->pushButton_8);

    class_select->ajouter_liste(ui->verticalLayout_8, ui->pushButton_11);

    class_select->bouton_creation_projet_clicked(ui->pushButton_10, ui->stackedWidget, ui->lineEdit_5, ui->lineEdit_6, ui->verticalLayout_8);


    bouton_annulation();
}


void Qt_main::select_affichage_projets() {
    auto data_element = class_BDD->Get_element();
    
    std::vector<int> tab_id_element;  
    for (int i = 0; i < data_element.size(); i++) {
        //Filtre doublon id_element
        bool deja_present = false;
        
        for (int j = 0; j < tab_id_element.size(); j++) {
            
            if (tab_id_element[j] == data_element[i].id) {
                deja_present = true;
                break;
            }
        }
        if (deja_present == true)continue;
        tab_id_element.push_back(data_element[i].id);

        // Création + affichage du widget
        QWidget* widgetProjet = class_select->affichage_projets(data_element[i]);
        ui->verticalLayout_9->addWidget(widgetProjet);

        // Récupération du bouton "modifier" dans le widget créé
        QPushButton* bouton_modifier = widgetProjet->findChild<QPushButton*>("bouton_modifier");

        if (bouton_modifier) class_select->bouton_ouvrir_clicked(data_element[i], bouton_modifier, ui->stackedWidget);

        QWidget* widgetListe = class_select->bouton_liste_clicked(ui->pushButton_19, ui->stackedWidget);
        ui->verticalLayout_21->addWidget(widgetListe);


		//QWidget* widgetNote = class_select->bouton_note_clicked(ui->pushButton_18, ui->stackedWidget);
		//ui->verticalLayout_14->addWidget(widgetNote);
    }

    QWidget* widgetListe = class_select->bouton_modif_projet_clicked(ui->pushButton_23, ui->stackedWidget, ui->lineEdit_7, ui->lineEdit_8, ui->pushButton_12, ui->verticalLayout_12, ui->pushButton_13);
	ui->verticalLayout_12->addWidget(widgetListe);

    QWidget* widgetNote = class_select->affiche_note(ui->pushButton_18, ui->stackedWidget);
    ui->verticalLayout_20->addWidget(widgetNote);

    class_select->zone_ajout_note(ui->pushButton_26, ui->stackedWidget);

    class_select->ajout_note(ui->stackedWidget, ui->pushButton_28, ui->lineEdit_9, ui->textEdit);
}

void Qt_main::bouton_annulation() {    

    ui->pushButton_14->disconnect();
    connect(ui->pushButton_14, &QPushButton::clicked, this, [=]() {

        ui->stackedWidget->setCurrentIndex(7);
        });

	ui->pushButton_16->disconnect();
    connect(ui->pushButton_16, &QPushButton::clicked, this, [=]() {

        ui->stackedWidget->setCurrentIndex(3);
		});

    ui->pushButton_22->disconnect();
    connect(ui->pushButton_22, &QPushButton::clicked, this, [=]() {

        ui->stackedWidget->setCurrentIndex(6);
        });

	ui->pushButton_9->disconnect();
    connect(ui->pushButton_9, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentIndex(3);
		});

    ui->pushButton_25->disconnect();
    connect(ui->pushButton_25, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentIndex(6);
        });

    ui->pushButton_29->disconnect();
    connect(ui->pushButton_29, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentIndex(8);
        });
}


Qt_main::~Qt_main() {
    
}