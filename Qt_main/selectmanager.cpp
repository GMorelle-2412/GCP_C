#include "SelectManager.h"

SelectManager::SelectManager(QObject* parent)
    : QObject(parent), class_BDD(new BDD())
{

    sauve_modif_notes = new QWidget();
    layout_sauve_modif_notes = new QVBoxLayout(sauve_modif_notes);
}


/*zone principale*/
QWidget* SelectManager::affichage_projets(const BDD::LigneElement& projet)
{
    // Calcul du pourcentage
    auto data_liste = class_BDD->Get_liste();
    auto data_contenue = class_BDD->Get_contenu();

    for (int i = 0; i < data_contenue.size(); i++) {
        for (int j = 0; j < data_liste.size(); j++) {

            if (data_contenue[i].id_element == projet.id &&
                data_contenue[i].id_liste == data_liste[j].id) {

                sauvegarde_nb_liste_max++;

                if (data_liste[j].validation == 1)
                    sauvegarde_nb_liste_valider++;
            }
        }
    }

    int pourcentage = 0;
    if (sauvegarde_nb_liste_max > 0)
        pourcentage = (sauvegarde_nb_liste_valider * 100) / sauvegarde_nb_liste_max;

    // Widget principal cliquable
    QWidget* projetWidget = new QWidget();
    projetWidget->setMaximumWidth(500);
    projetWidget->setObjectName("zoneProjets");
    projetWidget->setCursor(Qt::PointingHandCursor); // curseur main au survol
    projetWidget->installEventFilter(this);          // capture les clics

    // Stocker le projet associé au widget
    projetWidget->setProperty("projet_id", projet.id);
    projetWidget->setProperty("projet_nom", projet.nom);
    projetWidget->setProperty("projet_desc", projet.description);

    QVBoxLayout* layout_principal = new QVBoxLayout(projetWidget);

    // Zone titre + description
    QVBoxLayout* zone_titre_description = new QVBoxLayout;

    // Après la création de titre et description, installez l'eventFilter
    AutoResizeTextBrowser* titre = new AutoResizeTextBrowser();
    titre->setText(projet.nom);
    titre->setObjectName("titre");
    titre->installEventFilter(this);          // ← ajout
    titre->setTextInteractionFlags(Qt::NoTextInteraction); // ← désactive sélection/clic texte
    zone_titre_description->addWidget(titre);

    AutoResizeTextBrowser* description = new AutoResizeTextBrowser();
    description->setText(projet.description);
    description->setObjectName("description");
    description->installEventFilter(this);    // ← ajout
    description->setTextInteractionFlags(Qt::NoTextInteraction); // ← désactive sélection/clic texte
    zone_titre_description->addWidget(description);

    layout_principal->addLayout(zone_titre_description);

    // Zone état
    QHBoxLayout* zone_etat = new QHBoxLayout;

    QWidget* GANT = new QWidget;
    zone_etat->addWidget(GANT);

    QLabel* imageLabel = new QLabel();
    imageLabel->setFixedSize(100, 100);
    imageLabel->setScaledContents(true);

    if (pourcentage == 100) {
        imageLabel->setPixmap(QPixmap(":/completion/image/platine.png"));
        nb_projet_platine++;
    }
    else if (pourcentage >= 75) {
        imageLabel->setPixmap(QPixmap(":/completion/image/or.png"));
    }
    else if (pourcentage >= 50) {
        imageLabel->setPixmap(QPixmap(":/completion/image/fer.png"));
    }
    else if (pourcentage > 0) {
        imageLabel->setPixmap(QPixmap(":/completion/image/bronze.png"));
    }
    else {
        imageLabel->setPixmap(QPixmap(":/completion/image/noir.png"));
    }

    zone_etat->addWidget(imageLabel);

    QLabel* pourcentage_de_completion =
        new QLabel(QString::number(pourcentage) + " %");
    zone_etat->addWidget(pourcentage_de_completion);

    layout_principal->addLayout(zone_etat);

    // Reset compteurs
    sauvegarde_nb_liste_max = 0;
    sauvegarde_nb_liste_valider = 0;

    return projetWidget;
}

void SelectManager::bouton_ouvrir_clicked(const BDD::LigneElement& projet,
    QStackedWidget* stackedWidget)
{
    sauvegarde.id = projet.id;
    sauvegarde.nom = projet.nom;
    sauvegarde.description = projet.description;

    stackedWidget->setCurrentIndex(6);
}


/*création projet*/
void SelectManager::zone_creation_projet(QStackedWidget* stackedWidget, QPushButton* bouton_modifier) {
    bouton_modifier->disconnect();

    connect(bouton_modifier, &QPushButton::clicked, this, [=]() {

        stackedWidget->setCurrentIndex(4);
        });

    return;
}

void SelectManager::ajouter_liste(QVBoxLayout* verticalLayout_8, QPushButton* bouton_ajouter) {

    connect(bouton_ajouter, &QPushButton::clicked, this, [=]() {
        int last = verticalLayout_8->count() - 1;

        if (last >= 0) {
            QLayoutItem* lastItem = verticalLayout_8->itemAt(last);

            if (lastItem && !lastItem->widget()) {
                verticalLayout_8->takeAt(last);
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

        verticalLayout_8->addWidget(ligne);
        verticalLayout_8->addStretch();

        connect(supp, &QPushButton::clicked, this, [ligne]() {
            ligne->deleteLater();
            });
        });
}

void SelectManager::bouton_creation_projet_clicked(QPushButton* bouton_creation_projet, QStackedWidget* stackedWidget, QLineEdit* nom_cre, QLineEdit* description_cre, QVBoxLayout* verticalLayout_8) {
    connect(bouton_creation_projet, &QPushButton::clicked, this, [=]() {
        QString nom = nom_cre->text();
        QString description = description_cre->text();

        class_BDD->Poste_element(nom, description);
        class_BDD->id_liste.clear();

        for (int i = 0; i < verticalLayout_8->count(); i++) {
            QWidget* ligne = verticalLayout_8->itemAt(i)->widget();

            if (!ligne) continue;

            QLineEdit* edit = ligne->findChild<QLineEdit*>();
            QCheckBox* check = ligne->findChild<QCheckBox*>();

            if (!edit || !check) continue;

            int id = class_BDD->Poste_liste(edit->text(), check->isChecked());

            class_BDD->id_liste.push_back(id);
        }

        for (int id_l : class_BDD->id_liste)
            class_BDD->Poste_contenu(class_BDD->id_element, id_l);

        while (verticalLayout_8->count() > 0) {
            QLayoutItem* item = verticalLayout_8->takeAt(0);

            if (item->widget()) item->widget()->deleteLater();

            delete item;
        }

        stackedWidget->setCurrentIndex(3);

        //affiche_element_liste();
        });
}


/*Liste*/
QWidget* SelectManager::bouton_liste_clicked(QPushButton* bouton_liste, QStackedWidget* stackedWidget) {

    bouton_liste->disconnect();

    // Widget qui sera rempli AU MOMENT DU CLIC
    QWidget* projetWidget = new QWidget();
    QVBoxLayout* layout_principal = new QVBoxLayout(projetWidget);

    // On capture projetWidget et layout_principal dans la lambda
    connect(bouton_liste, &QPushButton::clicked, this,
        [=]() {

            qDebug() << "Bouton 'liste' cliqué pour le projet :" << sauvegarde.nom;

            // Nettoyage si on reclique plusieurs fois
            QLayoutItem* item;
            while ((item = layout_principal->takeAt(0)) != nullptr) {
                delete item->widget();
                delete item;
            }

            // Titre
            AutoResizeTextBrowser* titre = new AutoResizeTextBrowser();
            titre->setText(sauvegarde.nom);
			titre->setObjectName("titre");
            layout_principal->addWidget(titre);

            AutoResizeTextBrowser* description = new AutoResizeTextBrowser();
            description->setText(sauvegarde.description);
			description->setObjectName("description");
            layout_principal->addWidget(description);

            // Récupération des données
            auto data_liste = class_BDD->Get_liste();
            auto data_contenue = class_BDD->Get_contenu();

            for (const auto& contenu : data_contenue) {

                if (contenu.id_element == sauvegarde.id) {

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

//Affichage de la page de modification du projet
QWidget* SelectManager::bouton_modif_projet_clicked(QPushButton* bouton_modif_projet, QStackedWidget* stackedWidget, QLineEdit* nom, QLineEdit* description, QPushButton* bouton_ajouter, QVBoxLayout* verticalLayout, QPushButton* bouton_modif) {

    bouton_modif_projet->disconnect();

    QWidget* WidgetModif = new QWidget();
    QVBoxLayout* layout_principal = new QVBoxLayout(WidgetModif);

    connect(bouton_modif_projet, &QPushButton::clicked, this, [=]() {
        nom->setText(sauvegarde.nom);

        description->setText(sauvegarde.description);

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

            if (contenu.id_element == sauvegarde.id) {

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

        ajout_liste_modif(bouton_ajouter, verticalLayout);

        modif_projet(bouton_modif, stackedWidget, nom, description, verticalLayout);

        stackedWidget->setCurrentIndex(5);
        });

    return WidgetModif;
}

void SelectManager::ajout_liste_modif(QPushButton* bouton_ajouter, QVBoxLayout* verticalLayout) {
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

void SelectManager::modif_projet(QPushButton* bouton_modif, QStackedWidget* stackedWidget, QLineEdit* nom, QLineEdit* description, QVBoxLayout* verticalLayout) {

    disconnect(bouton_modif, nullptr, this, nullptr);

    int id_element = sauvegarde.id;

    std::vector<BDD::LigneElement> data_element;
    std::vector<BDD::LigneListe> data_liste;
    std::vector<BDD::LigneContenueElement> data_contenue_liste;

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

        if (verife_reste_liste == -1) {
            int reste = verticalLayout->count() - max_liste;

            if (reste < 0) reste *= -1;

            verife_reste_liste = reste;
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

            class_BDD->modif_liste(table_id_liste_modif[i], contenu->text(), validation->isChecked());

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
                int id_liste = table_id_liste_modif.back();

                table_id_liste_modif.pop_back();

                qDebug() << "Suppression id_liste =" << id_liste;

                class_BDD->delete_liste(id_liste);

                class_BDD->delete_contenu(id_liste);
            }
        }

        stackedWidget->setCurrentIndex(3);
        //affiche_element_liste();
        });
}

//QWidget* SelectManager::bouton_note_clicked(QPushButton* bouton_note, QStackedWidget* stackedWidget) {}


/*Notes*/
QWidget* SelectManager::affiche_note(QPushButton* bouton_affiche_note,
    QStackedWidget* stackedWidget,
    QVBoxLayout* verticalLayout)
{
    bouton_affiche_note->disconnect();

    QWidget* note = new QWidget();
    QVBoxLayout* layout_principal = new QVBoxLayout(note);
    layout_principal->setSpacing(75);

    connect(bouton_affiche_note, &QPushButton::clicked, this, [=]() {

        // Nettoyage du layout principal
        QLayoutItem* item;
        while ((item = layout_principal->takeAt(0)) != nullptr) {
            if (item->widget())
                delete item->widget();
            delete item;
        }

        auto data_contenue_note = class_BDD->Get_Contenu_element_notes(sauvegarde.id);

        for (const auto& contenu : data_contenue_note) {

            auto data_note = class_BDD->Get_note(contenu.id_notes);

            for (const auto& noteData : data_note) {

                QWidget* zone_note = new QWidget();
                zone_note->setObjectName("zoneNotes");  // ← était "zoneProjets", corrigé
                zone_note->setCursor(Qt::PointingHandCursor);
                zone_note->installEventFilter(this);

                zone_note->setProperty("note_id", noteData.id);
                zone_note->setProperty("note_nom", noteData.nom);
                zone_note->setProperty("note_text", noteData.text);

                QVBoxLayout* layout_zone_note = new QVBoxLayout(zone_note);

                AutoResizeTextBrowser* nom = new AutoResizeTextBrowser();
                nom->setText(noteData.nom);
                nom->setObjectName("titre");
                nom->installEventFilter(this);
                nom->setTextInteractionFlags(Qt::NoTextInteraction);
                layout_zone_note->addWidget(nom);

                AutoResizeTextBrowser* text = new AutoResizeTextBrowser();
                text->setText(noteData.text);
                text->setObjectName("description");
                text->installEventFilter(this);
                text->setTextInteractionFlags(Qt::NoTextInteraction);
                layout_zone_note->addWidget(text);

                layout_principal->addWidget(zone_note);
            }
        }

        stackedWidget->setCurrentIndex(8);
        });

    return note;
}

void SelectManager::zone_ajout_note(QPushButton* bouton_zone_ajout_note, QStackedWidget* stackedWidget) {
    bouton_zone_ajout_note->disconnect();

    connect(bouton_zone_ajout_note, &QPushButton::clicked, this, [=]() {

        stackedWidget->setCurrentIndex(9);
        });
}

void SelectManager::ajout_note(QStackedWidget* stackedWidget, QPushButton* bouton_ajout_note, QLineEdit* lineEdit_9, QTextEdit* textEdit) {

    bouton_ajout_note->disconnect();

    connect(bouton_ajout_note, &QPushButton::clicked, this, [=]() {

        qDebug() << sauvegarde.nom;

        qDebug() << sauvegarde.id;

        QString text = textEdit->toPlainText();

        QString nom = lineEdit_9->text();

        id_notes = class_BDD->Poste_note(text, nom);

        class_BDD->Poste_Contenu_element_notes(sauvegarde.id, id_notes);

        stackedWidget->setCurrentIndex(8);
        });
}

void SelectManager::affiche_modif_note(QStackedWidget* stackedWidget,
    int id_note,
    AutoResizeTextBrowser* nom,
    AutoResizeTextBrowser* text)
{
    // Nettoyage du layout
    QLayoutItem* item;
    while ((item = layout_sauve_modif_notes->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    QLineEdit* editNom = new QLineEdit();
    editNom->setText(nom->toPlainText());
    layout_sauve_modif_notes->addWidget(editNom);

    QTextEdit* editText = new QTextEdit();
    editText->setPlainText(text->toPlainText());
    layout_sauve_modif_notes->addWidget(editText);

    QPushButton* save = new QPushButton("Enregistrer");
    layout_sauve_modif_notes->addWidget(save);

    connect(save, &QPushButton::clicked, this, [=]() {
        class_BDD->modif_note(id_note, editNom->text(), editText->toPlainText());
        nom->setText(editNom->text());
        text->setText(editText->toPlainText());
        stackedWidget->setCurrentIndex(8);
        });

    stackedWidget->setCurrentIndex(10);
}



bool SelectManager::eventFilter(QObject* obj, QEvent* event)
{
    if (event->type() == QEvent::MouseButtonPress)
    {
        if (qobject_cast<QPushButton*>(obj))
            return QObject::eventFilter(obj, event);

        QWidget* w = qobject_cast<QWidget*>(obj);
        while (w) {
            if (qobject_cast<QPushButton*>(w))
                return QObject::eventFilter(obj, event);

            if (w->objectName() == "zoneProjets") {
                BDD::LigneElement projet;
                projet.id = w->property("projet_id").toInt();
                projet.nom = w->property("projet_nom").toString();
                projet.description = w->property("projet_desc").toString();
                emit projetClicked(projet);
                return true;
            }

            if (w->objectName() == "zoneNotes") {  // ← nouveau cas
                int id_note = w->property("note_id").toInt();
                AutoResizeTextBrowser* nom = w->findChild<AutoResizeTextBrowser*>("titre");
                AutoResizeTextBrowser* text = w->findChild<AutoResizeTextBrowser*>("description");
                emit noteClicked(id_note, nom, text);
                return true;
            }

            w = w->parentWidget();
        }
    }
    return QObject::eventFilter(obj, event);
}