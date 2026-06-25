#pragma once

#include "BDD.h"
#include "AutoResizeTextBrowser.h"
#include "sauvegarde.h"

#include <QObject>
#include <QPushButton>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QLineEdit>

class note : public QObject {
    Q_OBJECT

public:
    explicit note(QObject* parent = nullptr);

    BDD* class_BDD;
    sauvegarde* class_sauvegarde;

public slots:
    QWidget* affiche_note(QPushButton* bouton_affiche_note, QStackedWidget* stackedWidget,
        QVBoxLayout* verticalLayout);

    void zone_ajout_note(QPushButton* bouton_zone_ajout_note, QStackedWidget* stackedWidget);

    void ajout_note(QStackedWidget* stackedWidget, QPushButton* bouton_ajout_note,
        QLineEdit* lineEdit_9, QTextEdit* textEdit);

    void affiche_modif_note(QStackedWidget* stackedWidget,
        int id_note,
        AutoResizeTextBrowser* nom,
        AutoResizeTextBrowser* text);
};

