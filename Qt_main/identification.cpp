#include "select.h"
#include "identification.h"


void identification::page_connection() {
    connect(ui->pushButton_2, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentIndex(2);
        });
}

void identification::page_inscription() {
    connect(ui->pushButton, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentIndex(1);
        });
}

void identification::bouton_connection()
{
    QSettings settings("MonApp", "Auth");

    bool auto_login = settings.value("auto_login", false).toBool();

    QString saved_user = settings.value("username", "").toString();

    if (auto_login && !saved_user.isEmpty()) {
        class_BDD->Connection_auto(saved_user);
        
        ui->stackedWidget->setCurrentIndex(0);
        
        affiche_info_user();
        
        class_projet->affiche_element_liste();
    }

    connect(ui->pushButton_5, &QPushButton::clicked, this, [this]() {
        QString nom = ui->lineEdit_3->text();
        QString mdp = ui->lineEdit_4->text();
        
        if (nom.isEmpty() || mdp.isEmpty()) {
            qDebug() << "Champs vides";
            return;
        }
        
        if (class_BDD->Connection(nom, mdp)) {
            QSettings settings("MonApp", "Auth");
            settings.setValue("auto_login", true);
            settings.setValue("username", nom);
            ui->stackedWidget->setCurrentIndex(7);
            class_select->get_projet();
            affiche_info_user();
            class_projet->affiche_element_liste();
        }
        else {
            qDebug() << "Identifiants incorrects";
        }
        });
}

void identification::bouton_inscription() {
    connect(ui->pushButton_3, &QPushButton::clicked, this, [this]() {
        QString nom = ui->lineEdit->text();
        QString mdp = ui->lineEdit_2->text();
        
        if (nom.isEmpty() || mdp.isEmpty()) {
            qDebug() << "Champs vides";
            return;
        }
        
        class_BDD->Inscription(nom, mdp);
        
        ui->stackedWidget->setCurrentIndex(7);
        });
}

void identification::affiche_info_user() {
    ui->label_6->setText(class_BDD->nom_user);
}

void identification::bouton_deconection() {
    connect(ui->pushButton_7, &QPushButton::clicked, this, [this]() {
        QSettings settings("MonApp", "Auth");
        settings.setValue("auto_login", false);
        settings.remove("username");
        
        class_BDD->id_user = 0;
        class_BDD->nom_user = "nul";
        
        while (ui->verticalLayout_9->count() > 0) {
            QLayoutItem* item = ui->verticalLayout_9->takeAt(0);
            
            if (item->widget())
                item->widget()->deleteLater();
            
            delete item;
        }
        ui->stackedWidget->setCurrentIndex(0);
        });
}