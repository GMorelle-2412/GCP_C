#pragma once

#include "ui_Qt_main.h"
#include "BDD.h"

#include <QtWidgets/QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

class Qt_main : public QMainWindow{

    Q_OBJECT

public:
    Qt_main(QWidget *parent = nullptr);
    ~Qt_main();

private:
    Ui::Qt_mainClass * ui;

    BDD class_BDD;

private slots:
    void page_connection();

    void page_inscription();
    
    void bouton_connection();

    void bouton_inscription();

    void affiche_info_user();
};