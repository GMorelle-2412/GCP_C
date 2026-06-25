#pragma once

#include "BDD.h"
#include "AutoResizeTextBrowser.h"
#include "sauvegarde.h"

#include <QObject>
#include <QPushbutton>
#include <QStackedWidget>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QCheckBox>

class liste : public QObject {
    Q_OBJECT

public:
    explicit liste(QObject* parent = nullptr);

    BDD* class_BDD;

    sauvegarde* class_sauvegarde;

public:
    QWidget* Affiche_liste(QPushButton* bouton_liste, QStackedWidget* stackedWidget);

    QWidget* zone_modif_liste(QPushButton* bouton_modif_projet, 
        QStackedWidget* stackedWidget, QLineEdit* nom, QLineEdit* description, 
        QPushButton* bouton_ajouter, QVBoxLayout* verticalLayout, QPushButton* bouton_modif);

    void ajout_liste(QPushButton* bouton_ajouter, QVBoxLayout* verticalLayout);

    void modif_projet(QPushButton* bouton_modif, QStackedWidget* stackedWidget,
        QLineEdit* nom, QLineEdit* description, QVBoxLayout* verticalLayout);
};

