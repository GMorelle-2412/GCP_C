#include "liste.h"

liste::liste(QObject* parent)
    : QObject(parent), class_BDD(new BDD()), class_sauvegarde(new sauvegarde)
{
}

QWidget* liste::Affiche_liste(QPushButton* bouton_liste, QStackedWidget* stackedWidget) {

    bouton_liste->disconnect();

    // Widget qui sera rempli AU MOMENT DU CLIC
    QWidget* projetWidget = new QWidget();
    QVBoxLayout* layout_principal = new QVBoxLayout(projetWidget);

    // On capture projetWidget et layout_principal dans la lambda
    connect(bouton_liste, &QPushButton::clicked, this,
        [=]() {

            qDebug() << "Bouton 'liste' cliqué pour le projet :" << class_sauvegarde->sauvegarde_element.nom;

            // Nettoyage si on reclique plusieurs fois
            QLayoutItem* item;
            while ((item = layout_principal->takeAt(0)) != nullptr) {
                delete item->widget();
                delete item;
            }

            // Titre
            AutoResizeTextBrowser* titre = new AutoResizeTextBrowser();
            titre->setText(class_sauvegarde->sauvegarde_element.nom);
            titre->setObjectName("titre");
            layout_principal->addWidget(titre);

            AutoResizeTextBrowser* description = new AutoResizeTextBrowser();
            description->setText(class_sauvegarde->sauvegarde_element.description);
            description->setObjectName("description");
            layout_principal->addWidget(description);

            // Récupération des données
            auto data_liste = class_BDD->Get_liste();
            auto data_contenue = class_BDD->Get_contenu();

            for (const auto& contenu : data_contenue) {

                if (contenu.id_element == class_sauvegarde->sauvegarde_element.id) {

                    for (const auto& liste : data_liste) {

                        if (liste.id == contenu.id_liste) {

                            QWidget* ligne = new QWidget(projetWidget);
                            QHBoxLayout* layout = new QHBoxLayout(ligne);

                            QCheckBox* validation = new QCheckBox(ligne);
                            validation->setChecked(liste.validation);
                            layout->addWidget(validation);

                            AutoResizeTextBrowser* contenuLabel = new AutoResizeTextBrowser();
                            contenuLabel->setText(liste.contenu);
                            contenuLabel->setStyleSheet(
                                liste.validation
                                ? "color: #606060; text-decoration: line-through;"
                                : "color: #606060;"
                            );
                            layout->addWidget(contenuLabel);

                            connect(validation, &QCheckBox::toggled, this,
                                [this, contenuLabel, id = liste.id](bool checked) {

                                    contenuLabel->setStyleSheet(
                                        checked
                                        ? "color: #606060; text-decoration: line-through;"
                                        : "color: #F0F0F0;"
                                    );

                                    class_BDD->modif_liste(id, contenuLabel->toPlainText(), checked);
                                });

                            layout_principal->addWidget(ligne);
                        }
                    }
                }
            }

            stackedWidget->setCurrentIndex(7);
        });

    return projetWidget;
}

QWidget* liste::zone_modif_liste(QPushButton* bouton_modif_projet, QStackedWidget* stackedWidget, QLineEdit* nom, QLineEdit* description, QPushButton* bouton_ajouter, QVBoxLayout* verticalLayout, QPushButton* bouton_modif) {

    bouton_modif_projet->disconnect();

    QWidget* WidgetModif = new QWidget();
    QVBoxLayout* layout_principal = new QVBoxLayout(WidgetModif);

    connect(bouton_modif_projet, &QPushButton::clicked, this, [=]() {
        nom->setText(class_sauvegarde->sauvegarde_element.nom);

        description->setText(class_sauvegarde->sauvegarde_element.description);

        // Nettoyage si on reclique plusieurs fois
        QLayoutItem* item;
        while ((item = layout_principal->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }

        // Récupération des données
        auto data_liste = class_BDD->Get_liste();
        auto data_contenue = class_BDD->Get_contenu();

        for (const auto& contenu : data_contenue) {

            if (contenu.id_element == class_sauvegarde->sauvegarde_element.id) {

                for (const auto& liste : data_liste) {

                    if (liste.id == contenu.id_liste) {

                        QWidget* ligne = new QWidget(WidgetModif);
                        QHBoxLayout* layout = new QHBoxLayout(ligne);

                        QCheckBox* validation = new QCheckBox(ligne);
                        validation->setChecked(liste.validation);
                        layout->addWidget(validation);

                        QLabel* contenuLabel = new QLabel(liste.contenu, ligne);
                        contenuLabel->setStyleSheet(
                            liste.validation
                            ? "color: #606060; text-decoration: line-through;"
                            : "color: #606060;"
                        );
                        layout->addWidget(contenuLabel);

                        connect(validation, &QCheckBox::toggled, this,
                            [this, contenuLabel, id = liste.id](bool checked) {

                                contenuLabel->setStyleSheet(
                                    checked
                                    ? "color: #606060; text-decoration: line-through;"
                                    : "color: #F0F0F0;"
                                );

                                class_BDD->modif_liste(id, contenuLabel->text(), checked);
                            });

                        QPushButton* bouton_supprimer = new QPushButton("X", ligne);
                        layout->addWidget(bouton_supprimer);

                        layout_principal->addWidget(ligne);
                    }
                }
            }
        }

        ajout_liste(bouton_ajouter, verticalLayout);

        modif_projet(bouton_modif, stackedWidget, nom, description, verticalLayout);

        stackedWidget->setCurrentIndex(5);
        });

    return WidgetModif;
}

void liste::ajout_liste(QPushButton* bouton_ajouter, QVBoxLayout* verticalLayout) {
    disconnect(bouton_ajouter, nullptr, this, nullptr);

    connect(bouton_ajouter, &QPushButton::clicked, this, [=]() {
        int last = verticalLayout->count() - 1;
        if (last >= 0) {
            QLayoutItem* lastItem = verticalLayout->itemAt(last);

            if (lastItem && !lastItem->widget()) {
                verticalLayout->takeAt(last);
                delete lastItem;
            }
        }

        QWidget* ligne = new QWidget();
        ligne->setMaximumHeight(120);

        QVBoxLayout* zone = new QVBoxLayout(ligne);
        zone->setContentsMargins(8, 8, 8, 8);
        zone->setSpacing(6);

        QWidget* rangee = new QWidget(ligne);

        QHBoxLayout* layoutRangee = new QHBoxLayout(rangee);
        layoutRangee->setContentsMargins(0, 0, 0, 0);
        layoutRangee->setSpacing(8);

        QCheckBox* validation = new QCheckBox(rangee);

        QLineEdit* contenu = new QLineEdit("", rangee);
        layoutRangee->addWidget(validation);
        layoutRangee->addWidget(contenu);

        QPushButton* supp = new QPushButton("X", ligne);
        supp->setFixedHeight(36);
        supp->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

        zone->addWidget(rangee);
        zone->addWidget(supp);

        verticalLayout->addWidget(ligne);
        verticalLayout->addStretch();

        connect(supp, &QPushButton::clicked, this, [ligne]() { ligne->deleteLater(); });
        });
}

void liste::modif_projet(QPushButton* bouton_modif, QStackedWidget* stackedWidget, QLineEdit* nom, QLineEdit* description, QVBoxLayout* verticalLayout) {

    disconnect(bouton_modif, nullptr, this, nullptr);

    int id_element = class_sauvegarde->sauvegarde_element.id;

    std::vector<sauvegarde::LigneElement> data_element;
    std::vector<sauvegarde::LigneListe> data_liste;
    std::vector<sauvegarde::LigneContenueElement> data_contenue_liste;

    connect(bouton_modif, &QPushButton::clicked, this, [=]() {

        for (int y = 0; y < data_element.size(); y++) {
            if (id_element == data_element[y].id)
                class_BDD->modif_element(id_element, nom->text(), description->text());
        }

        int max_liste = 0;
        for (int f = 0; f < data_contenue_liste.size(); f++)

            for (int h = 0; h < data_liste.size(); h++)

                if (id_element == data_contenue_liste[f].id_element &&
                    data_contenue_liste[f].id_liste == data_liste[h].id)
                    max_liste++;

        if (class_sauvegarde->verife_reste_liste == -1) {
            int reste = verticalLayout->count() - max_liste;

            if (reste < 0) reste *= -1;

            class_sauvegarde->verife_reste_liste = reste;
        }

        int compteur_update = 0;
        int nb = verticalLayout->count();

        for (int i = 0; i < max_liste; i++) {
            QLayoutItem* item = verticalLayout->itemAt(i);

            if (!item) continue;

            QWidget* ligne = item->widget();

            if (!ligne) continue;

            QLineEdit* contenu = ligne->findChild<QLineEdit*>();
            QCheckBox* validation = ligne->findChild<QCheckBox*>();

            if (!contenu || !validation) continue;

            qDebug() << "Ligne" << i << "Texte:" << contenu->text() << "Valide:" << validation->isChecked();

            class_BDD->modif_liste(class_sauvegarde->table_id_liste_modif[i], contenu->text(), validation->isChecked());

            compteur_update++;
        }

        if (nb > max_liste) {
            for (int i = 0; i < nb - max_liste; i++) {

                QLayoutItem* item = verticalLayout->itemAt(i + compteur_update);

                if (!item) continue;

                QWidget* ligne = item->widget();

                if (!ligne) continue;

                QLineEdit* contenu = ligne->findChild<QLineEdit*>();
                QCheckBox* validation = ligne->findChild<QCheckBox*>();

                if (!contenu || !validation) continue;

                qDebug() << "Nouvelle Ligne" << i << "Texte:" << contenu->text() << "Valide:" << validation->isChecked();

                int id_liste = class_BDD->Poste_liste(contenu->text(), validation->isChecked());

                class_BDD->Poste_contenu(id_element, id_liste);
            }
        }

        if (nb < max_liste) {
            int nb_delete = max_liste - nb;

            qDebug() << "Suppression de" << nb_delete << "lignes dans la BDD";

            for (int i = 0; i < nb_delete; i++) {
                int id_liste = class_sauvegarde->table_id_liste_modif.back();

                class_sauvegarde->table_id_liste_modif.pop_back();

                qDebug() << "Suppression id_liste =" << id_liste;

                class_BDD->delete_liste(id_liste);

                class_BDD->delete_contenu(id_liste);
            }
        }

        stackedWidget->setCurrentIndex(3);
        //affiche_element_liste();
        });
}