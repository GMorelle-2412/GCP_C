#include "Qt_main.h"

Qt_main::Qt_main(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::Qt_mainClass) 
{
    ui->setupUi(this);

    /*Connection à la BDD*/
    class_BDD.Connect_BDD();

    ui->stackedWidget->setCurrentIndex(0);

    page_connection();
    bouton_connection();

    page_inscription();
    bouton_inscription();
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

Qt_main::~Qt_main()
{
    delete ui; 
}
