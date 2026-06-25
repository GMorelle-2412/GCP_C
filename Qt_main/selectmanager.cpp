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
    projetWidget->setObjectName("widget_liste_contenu");

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
QWidget* SelectManager::bouton_modif_projet_clicked(
    QPushButton* bouton_modif_projet,
    QStackedWidget* stackedWidget,
    QLineEdit* nom,
    QLineEdit* description,
    QPushButton* bouton_ajouter,
    QVBoxLayout* verticalLayout,
    QPushButton* bouton_modif, QPushButton* bouton_liste)
{
    QWidget* WidgetModif = new QWidget();
    QVBoxLayout* layout_principal = new QVBoxLayout(WidgetModif);

    disconnect(bouton_modif_projet, nullptr, this, nullptr);

    connect(bouton_modif_projet, &QPushButton::clicked, this, [=]() {

        // --- Remplir les champs ---
        nom->setText(sauvegarde.nom);
        description->setText(sauvegarde.description);

        // --- Nettoyer layout_principal ---
        QLayoutItem* item;
        while ((item = layout_principal->takeAt(0)) != nullptr) {
            if (auto w = item->widget()) w->deleteLater();
            delete item;
        }

        // --- Nettoyer verticalLayout SAUF WidgetModif ---
        for (int i = verticalLayout->count() - 1; i >= 0; i--) {
            QLayoutItem* it = verticalLayout->itemAt(i);
            if (!it) continue;
            QWidget* w = it->widget();
            if (w && w == WidgetModif) continue;
            verticalLayout->takeAt(i);
            if (w) w->deleteLater();
            delete it;
        }

        // --- Charger les lignes existantes depuis la BDD ---
        auto data_liste = class_BDD->Get_liste();
        auto data_contenu = class_BDD->Get_contenu();

        for (const auto& contenu : data_contenu) {
            if (contenu.id_element == sauvegarde.id) {

                for (const auto& liste : data_liste) {
                    if (liste.id == contenu.id_liste) {

                        QWidget* ligne = new QWidget(WidgetModif);
                        ligne->setProperty("id_liste", liste.id); // ✅ tag BDD

                        QVBoxLayout* Vlayout = new QVBoxLayout(ligne);
                        QHBoxLayout* Hlayout = new QHBoxLayout();

                        QCheckBox* validation = new QCheckBox(ligne);
                        validation->setChecked(liste.validation);
                        Hlayout->addWidget(validation);

                        QLineEdit* contenuLabel = new QLineEdit(liste.contenu, ligne);
                        contenuLabel->setStyleSheet(
                            liste.validation
                            ? "color: #606060; text-decoration: line-through;"
                            : "color: #606060;"
                        );
                        Hlayout->addWidget(contenuLabel);

                        connect(validation, &QCheckBox::toggled, this,
                            [this, contenuLabel, id = liste.id](bool checked) {
                                contenuLabel->setStyleSheet(
                                    checked
                                    ? "color: #606060; text-decoration: line-through;"
                                    : "color: #606060;"
                                );
                                class_BDD->modif_liste(id, contenuLabel->text(), checked);
                            });

                        QPushButton* bouton_supprimer = new QPushButton("X", ligne);
                        connect(bouton_supprimer, &QPushButton::clicked, this,
                            [ligne]() { ligne->deleteLater(); });

                        Vlayout->addLayout(Hlayout);
                        Vlayout->addWidget(bouton_supprimer);

                        layout_principal->addWidget(ligne);
                    }
                }
            }
        }

        ajout_liste_modif(bouton_ajouter, layout_principal); // ✅ layout_principal, pas verticalLayout
        modif_projet(bouton_modif, stackedWidget, nom, description, layout_principal, bouton_liste);

        stackedWidget->setCurrentIndex(5);
        });

    return WidgetModif;
}

void SelectManager::ajout_liste_modif(QPushButton* bouton_ajouter, QVBoxLayout* verticalLayout)
{
    disconnect(bouton_ajouter, nullptr, this, nullptr);

    connect(bouton_ajouter, &QPushButton::clicked, this, [=]() {


        // --- 1) Retirer le stretch final s'il existe ---
        int count = verticalLayout->count();
        if (count > 0) {
            QLayoutItem* last = verticalLayout->itemAt(count - 1);
            if (last && last->spacerItem()) {
                verticalLayout->takeAt(count - 1);
                delete last;
            }
        }

        // --- 2) Création de la ligne ---
        QWidget* ligne = new QWidget();
        ligne->setMaximumHeight(120);

        ligne->setProperty("id_liste", -1); // ✅ -1 = nouvelle ligne

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

        // --- 3) Bouton supprimer ---
        connect(supp, &QPushButton::clicked, this, [=]() {
            ligne->deleteLater();
            });

        // --- 4) Remettre un seul stretch en bas ---
        verticalLayout->addStretch();
        });
}

void SelectManager::modif_projet(
    QPushButton* bouton_modif,
    QStackedWidget* stackedWidget,
    QLineEdit* nom,
    QLineEdit* description,
    QVBoxLayout* layout_principal,
    QPushButton* bouton_liste)
{
    disconnect(bouton_modif, nullptr, this, nullptr);

    int id_element = sauvegarde.id;

    connect(bouton_modif, &QPushButton::clicked, this, [=]() {

        // --- 1) Mise à jour BDD ---
        class_BDD->modif_element(id_element, nom->text(), description->text());

        // --- 2) Mettre à jour sauvegarde ---
        sauvegarde.nom = nom->text();
        sauvegarde.description = description->text();

        // --- 3) Gestion des listes ---
        auto data_contenu = class_BDD->Get_contenu();
        std::vector<int> ids_en_bdd;
        for (const auto& c : data_contenu)
            if (c.id_element == id_element)
                ids_en_bdd.push_back(c.id_liste);

        std::vector<int> ids_vus_ui;

        for (int i = 0; i < layout_principal->count(); i++) {
            QLayoutItem* layoutItem = layout_principal->itemAt(i);
            if (!layoutItem) continue;
            QWidget* w = layoutItem->widget();
            if (!w) continue;

            QLineEdit* contenu = w->findChild<QLineEdit*>();
            QCheckBox* validation = w->findChild<QCheckBox*>();
            if (!contenu || !validation) continue;

            int id_liste = w->property("id_liste").toInt();

            if (id_liste == -1) {
                int nouvel_id = class_BDD->Poste_liste(
                    contenu->text(), validation->isChecked());
                if (nouvel_id != -1)
                    class_BDD->Poste_contenu(id_element, nouvel_id);
            }
            else {
                class_BDD->modif_liste(id_liste, contenu->text(), validation->isChecked());
                ids_vus_ui.push_back(id_liste);
            }
        }

        for (int id : ids_en_bdd) {
            bool encore_present = std::find(
                ids_vus_ui.begin(), ids_vus_ui.end(), id) != ids_vus_ui.end();
            if (!encore_present) {
                class_BDD->delete_liste(id);
                class_BDD->delete_contenu(id);
            }
        }

        // --- 4) Rafraîchir la page liste ---
        QWidget* widgetListe = stackedWidget->findChild<QWidget*>("widget_liste_contenu");

        if (widgetListe) {

            // ✅ Vider sans détruire le layout (évite le dangling pointer dans bouton_liste)
            if (QLayout* oldLayout = widgetListe->layout()) {
                QLayoutItem* item;
                while ((item = oldLayout->takeAt(0)) != nullptr) {
                    if (QWidget* w = item->widget()) {
                        w->hide();
                        delete w;
                    }
                    delete item;
                }
                // ❌ NE PAS faire delete oldLayout

                QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(oldLayout);

                // Titre
                AutoResizeTextBrowser* titre = new AutoResizeTextBrowser();
                titre->setText(sauvegarde.nom);
                titre->setObjectName("titre");
                layout->addWidget(titre);

                // Description
                AutoResizeTextBrowser* desc_label = new AutoResizeTextBrowser();
                desc_label->setText(sauvegarde.description);
                desc_label->setObjectName("description");
                layout->addWidget(desc_label);

                // Données fraîches
                auto data_liste_fresh = class_BDD->Get_liste();
                auto data_contenu_fresh = class_BDD->Get_contenu();

                for (const auto& c : data_contenu_fresh) {
                    if (c.id_element == sauvegarde.id) {
                        for (const auto& liste : data_liste_fresh) {
                            if (liste.id == c.id_liste) {

                                QWidget* ligne = new QWidget(widgetListe);
                                QHBoxLayout* Hlayout = new QHBoxLayout(ligne);
                                Hlayout->setContentsMargins(0, 0, 0, 0);
                                Hlayout->setSpacing(8);

                                QCheckBox* val = new QCheckBox(ligne);
                                val->setChecked(liste.validation);
                                Hlayout->addWidget(val);

                                AutoResizeTextBrowser* contenuLabel = new AutoResizeTextBrowser();
                                contenuLabel->setText(liste.contenu);
                                contenuLabel->setStyleSheet(
                                    liste.validation
                                    ? "color: #606060; text-decoration: line-through;"
                                    : "color: #606060;"
                                );
                                Hlayout->addWidget(contenuLabel);

                                connect(val, &QCheckBox::toggled, this,
                                    [this, contenuLabel, id = liste.id](bool checked) {
                                        contenuLabel->setStyleSheet(
                                            checked
                                            ? "color: #606060; text-decoration: line-through;"
                                            : "color: #F0F0F0;"
                                        );
                                        class_BDD->modif_liste(
                                            id, contenuLabel->toPlainText(), checked);
                                    });

                                layout->addWidget(ligne);
                            }
                        }
                    }
                }

                layout->addStretch();
            }
        }

        stackedWidget->setCurrentIndex(7);
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
        // Remonte la hiérarchie pour trouver un scroller actif
        QScroller* scroller = nullptr;
        QObject* o = obj;
        while (o) {
            scroller = QScroller::scroller(o);
            if (scroller) break;
            o = o->parent();
        }

        m_wasScrolling = scroller && scroller->state() != QScroller::Inactive;
        m_pressPos = static_cast<QMouseEvent*>(event)->globalPosition();
        m_pressedWidget = qobject_cast<QWidget*>(obj);

        // Si inertie en cours → avale le Press pour ne pas l'interrompre
        if (m_wasScrolling && scroller->state() == QScroller::Scrolling)
            return true;

        return QObject::eventFilter(obj, event);
    }

    if (event->type() == QEvent::MouseButtonRelease)
    {
        QPointF delta = static_cast<QMouseEvent*>(event)->globalPosition() - m_pressPos;
        bool didMove = delta.manhattanLength() > 5;

        if (m_wasScrolling || didMove)
        {
            m_wasScrolling = false;
            m_pressedWidget = nullptr;
            return QObject::eventFilter(obj, event);
        }

        // Tap propre → émettre le signal
        QWidget* w = m_pressedWidget;
        while (w) {
            if (qobject_cast<QPushButton*>(w)) {
                m_pressedWidget = nullptr;
                return QObject::eventFilter(obj, event);
            }
            if (w->objectName() == "zoneProjets") {
                BDD::LigneElement projet;
                projet.id = w->property("projet_id").toInt();
                projet.nom = w->property("projet_nom").toString();
                projet.description = w->property("projet_desc").toString();
                emit projetClicked(projet);
                m_pressedWidget = nullptr;
                return true;
            }
            if (w->objectName() == "zoneNotes") {
                int id_note = w->property("note_id").toInt();
                AutoResizeTextBrowser* nom = w->findChild<AutoResizeTextBrowser*>("titre");
                AutoResizeTextBrowser* text = w->findChild<AutoResizeTextBrowser*>("description");
                emit noteClicked(id_note, nom, text);
                m_pressedWidget = nullptr;
                return true;
            }
            w = w->parentWidget();
        }
        m_pressedWidget = nullptr;
    }

    return QObject::eventFilter(obj, event);
}

bool SelectManager::isScrolling(QObject* obj)
{
    for (QObject* o : { obj, obj->parent() }) {
        if (!o) continue;
        QScroller* s = QScroller::scroller(o);
        if (s && s->state() != QScroller::Inactive)
            return true;
    }
    return false;
}

/*Autre a trier*/
void SelectManager::bouton_supprimer_projet_clicked(
    QPushButton* bouton_supprimer,
    QStackedWidget* stackedWidget,
    std::function<void()> rafraichir_liste_projets)
{
    disconnect(bouton_supprimer, nullptr, this, nullptr);

    connect(bouton_supprimer, &QPushButton::clicked, this, [=]() {

        if (sauvegarde.id <= 0) return;

        // Confirmation avant suppression
        QMessageBox::StandardButton reponse = QMessageBox::question(
            nullptr,
            "Supprimer le projet",
            "Supprimer « " + sauvegarde.nom + " » et toutes ses listes ?",
            QMessageBox::Yes | QMessageBox::No
        );

        if (reponse != QMessageBox::Yes) return;

        // Suppression en cascade
        class_BDD->delete_element_complet(sauvegarde.id);

        // Reset sauvegarde
        sauvegarde.id = -1;
        sauvegarde.nom = "";
        sauvegarde.description = "";

        // Rafraîchir l'affichage des projets
        rafraichir_liste_projets();

        // Retour à la page principale
        stackedWidget->setCurrentIndex(3);
        });
}