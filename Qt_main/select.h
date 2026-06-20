#pragma once

#include "BDD.h"
#include "AutoResizeTextBrowser.h"

#include <QObject>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <qstackedwidget.h>
#include <QDebug>
#include <QList>
#include <QVector>
#include <QScrollArea>
#include <QCheckBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QTextBrowser>

class select : public QObject {
    Q_OBJECT

public:
    explicit select(QObject* parent = nullptr);

    BDD* class_BDD;

    struct sauvegarde_element {
        int id;
        QString nom;
		QString description;
    };
    sauvegarde_element sauvegarde;

    int verife_reste_liste = -1;

    int id_notes = -1;

    std::vector<int> table_id_liste_modif;

    QWidget* sauve_modif_notes;
    QVBoxLayout* layout_sauve_modif_notes;

    int sauvegarde_nb_liste_valider = 0;    
    int sauvegarde_nb_liste_max = 0;

    int nb_projet = 0;
    int nb_projet_platine = 0;


public slots:

    /*zone principale*/
    QWidget* affichage_projets(const BDD::LigneElement& projet);

	void zone_creation_projet(QStackedWidget* stackedWidget, QPushButton* bouton_modifier);


    /*création projet*/
	void ajouter_liste(QVBoxLayout* verticalLayout_8, QPushButton* bouton_ajouter);

	void bouton_creation_projet_clicked(QPushButton* bouton_creation_projet, QStackedWidget* stackedWidget, QLineEdit* nom_cre, QLineEdit* description_cre, QVBoxLayout* verticalLayout_8);

    void bouton_ouvrir_clicked(const BDD::LigneElement& projet, QPushButton* bouton_modifier, QStackedWidget* stackedWidget);
    
    
    /*Liste*/
    QWidget* bouton_liste_clicked(QPushButton* bouton_liste, QStackedWidget* stackedWidget);

	QWidget* bouton_modif_projet_clicked(QPushButton* bouton_modif_projet, QStackedWidget* stackedWidget, QLineEdit* nom, QLineEdit* description, QPushButton* bouton_ajouter, QVBoxLayout* verticalLayout, QPushButton* bouton_modif);

    void ajout_liste_modif(QPushButton* bouton_ajouter, QVBoxLayout* verticalLayout);

    void modif_projet(QPushButton* bouton_modif, QStackedWidget* stackedWidget, QLineEdit* nom, QLineEdit* description, QVBoxLayout* verticalLayout);

	//QWidget* bouton_note_clicked(QPushButton* bouton_note, QStackedWidget* stackedWidget);


    /*Notes*/
    QWidget* affiche_note(QPushButton* bouton_affiche_note, QStackedWidget* stackedWidget, QVBoxLayout* verticalLayout);

    void zone_ajout_note(QPushButton* bouton_zone_ajout_note, QStackedWidget* stackedWidget);

    void ajout_note(QStackedWidget* stackedWidget, QPushButton* bouton_ajout_note, QLineEdit* lineEdit_9, QTextEdit* textEdit);

    void affiche_modif_note(QStackedWidget* stackedWidget, QPushButton* bouton_modif_note, int notes, QLabel *nom, QLabel *text, QVBoxLayout* verticalLayout);
};
