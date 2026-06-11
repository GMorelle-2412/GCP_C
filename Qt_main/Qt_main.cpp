#include "Qt_main.h"
#include "Style.h"

#include <QScroller>
#include <QEasingCurve>

Qt_main::Qt_main(QWidget* parent)
    : QMainWindow(parent),
    ui(new Ui::Qt_mainClass)
{
    ui->setupUi(this);

    Style::appliquerStyle(this);

    class_BDD.Connect_BDD();

    ui->stackedWidget->setCurrentIndex(0);

    page_connection();
    bouton_connection();

    page_inscription();
    bouton_inscription();

    bouton_annulation();

    bouton_deconection();
    bouton_creation_projet();

    ajouter_liste();
    creation_projet();

    // Fonction helper pour appliquer les propriétés sur une scrollArea
#include <QEasingCurve>

    auto configScrollerUltraSmooth = [](QScrollArea* area) {
        QScroller* scroller = QScroller::scroller(area->viewport());
        QScroller::grabGesture(area->viewport(), QScroller::TouchGesture);

        QScrollerProperties props = scroller->scrollerProperties();

        // --- Sensation immédiate au toucher ---
        props.setScrollMetric(QScrollerProperties::DragStartDistance, 0.0001);
        props.setScrollMetric(QScrollerProperties::DragVelocitySmoothingFactor, 0.05);
        props.setScrollMetric(QScrollerProperties::AxisLockThreshold, 0.85);

        // --- Inertie ultra fluide ---
        props.setScrollMetric(QScrollerProperties::DecelerationFactor, 0.06);
        props.setScrollMetric(QScrollerProperties::MaximumVelocity, 4.0);
        props.setScrollMetric(QScrollerProperties::MinimumVelocity, 0.02);

        // --- Flick très dynamique ---
        props.setScrollMetric(QScrollerProperties::AcceleratingFlickMaximumTime, 0.35);
        props.setScrollMetric(QScrollerProperties::AcceleratingFlickSpeedupFactor, 1.8);

        // --- Micro-overshoot (effet premium) ---
        props.setScrollMetric(QScrollerProperties::OvershootDragResistanceFactor, 0.15);
        props.setScrollMetric(QScrollerProperties::OvershootScrollDistanceFactor, 0.04);
        props.setScrollMetric(QScrollerProperties::OvershootScrollTime, 0.12);

        // --- Courbe d’animation ultra smooth ---
        props.setScrollMetric(
            QScrollerProperties::ScrollingCurve,
            QVariant::fromValue(QEasingCurve(QEasingCurve::OutQuint))
        );

        scroller->setScrollerProperties(props);

        area->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        };

    configScrollerUltraSmooth(ui->scrollArea);
    configScrollerUltraSmooth(ui->scrollArea_2);
    configScrollerUltraSmooth(ui->scrollArea_3);
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

void Qt_main::bouton_connection()
{
    // ── Auto-login au démarrage ──────────────────────────────────────────
    QSettings settings("MonApp", "Auth");
    bool auto_login = settings.value("auto_login", false).toBool();
    QString saved_user = settings.value("username", "").toString();

    if (auto_login && !saved_user.isEmpty()) {
        class_BDD.Connection_auto(saved_user); // connexion sans mdp
        ui->stackedWidget->setCurrentIndex(3);
        affiche_info_user();
        affiche_element_liste();
    }

    // ── Connexion manuelle ───────────────────────────────────────────────
    connect(ui->pushButton_5, &QPushButton::clicked, this, [this]() {
        QString nom = ui->lineEdit_3->text();
        QString mdp = ui->lineEdit_4->text();

        if (nom.isEmpty() || mdp.isEmpty()) {
            qDebug() << "Champs vides";
            return;
        }

        if (class_BDD.Connection(nom, mdp)) {
            // Sauvegarde pour auto-login la prochaine fois
            QSettings settings("MonApp", "Auth");
            settings.setValue("auto_login", true);
            settings.setValue("username", nom);

            ui->stackedWidget->setCurrentIndex(3);
            affiche_info_user();
            affiche_element_liste();
        }
        else {
            qDebug() << "Identifiants incorrects";
        }
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

void Qt_main::bouton_deconection() {
    connect(ui->pushButton_7, &QPushButton::clicked, this, [this]() {

        // ── Supprime l'auto-login ────────────────────────────────────────
        QSettings settings("MonApp", "Auth");
        settings.setValue("auto_login", false);
        settings.remove("username");

        // ── Reset BDD ───────────────────────────────────────────────────
        class_BDD.id_user = 0;
        class_BDD.nom_user = "nul";

        // ── Vide la liste ───────────────────────────────────────────────
        while (ui->verticalLayout_9->count() > 0) {
            QLayoutItem* item = ui->verticalLayout_9->takeAt(0);
            if (item->widget())
                item->widget()->deleteLater();
            delete item;
        }

        ui->stackedWidget->setCurrentIndex(0);
        });
}

void Qt_main::bouton_annulation() {
    //connection
    connect(ui->pushButton_6, &QPushButton::clicked, this, [this] (){
        ui->stackedWidget->setCurrentIndex(0);
    });

    //inscription
    connect(ui->pushButton_4, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentIndex(0);
    });

    //Création élément 
    connect(ui->pushButton_9, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentIndex(3);
        affiche_info_user();
        affiche_element_liste();

        // Vider le layout
        while (ui->verticalLayout_8->count() > 0) {
            QLayoutItem* item = ui->verticalLayout_8->takeAt(0);

            if (item->widget())
                item->widget()->deleteLater();

            delete item;
        }
        });

    //Modif element
    connect(ui->pushButton_14, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentIndex(3);
        affiche_info_user();
        affiche_element_liste();
        
        // Vider le layout
        while (ui->verticalLayout_12->count() > 0) {
            QLayoutItem* item = ui->verticalLayout_12->takeAt(0);

            if (item->widget())
                item->widget()->deleteLater();

            delete item;
        }

        conteur_ajout_liste = 0;

        });
}

void Qt_main::bouton_creation_projet() {
    connect(ui->pushButton_8, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentIndex(4);
        });
}

void Qt_main::ajouter_liste() {

    connect(ui->pushButton_11, &QPushButton::clicked, this, [this]() {

        // Retirer le stretch s'il existe déjà (toujours en dernière position)
        int last = ui->verticalLayout_8->count() - 1;
        if (last >= 0) {
            QLayoutItem* lastItem = ui->verticalLayout_8->itemAt(last);
            if (lastItem && !lastItem->widget()) {
                ui->verticalLayout_8->takeAt(last);
                delete lastItem;
            }
        }

        QWidget* ligne = new QWidget(this);
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

        ui->verticalLayout_8->addWidget(ligne);
        ui->verticalLayout_8->addStretch();  // ← toujours en dernier

        connect(supp, &QPushButton::clicked, this, [ligne]() {
            ligne->deleteLater();
            });
        });
}

void Qt_main::creation_projet() {
    connect(ui->pushButton_10, &QPushButton::clicked, this, [this]() {

        // 1) Enregistrer l'élément principal
        QString nom = ui->lineEdit_5->text();
        QString description = ui->lineEdit_6->text();

        class_BDD.Poste_element(nom, description);

        class_BDD.id_liste.clear();

        // 2) Enregistrer chaque ligne
        for (int i = 0; i < ui->verticalLayout_8->count(); i++) {

            QWidget* ligne = ui->verticalLayout_8->itemAt(i)->widget();
            if (!ligne) continue;  // ← ignore le stretch (qui n'est pas un widget)

            QLineEdit* edit = ligne->findChild<QLineEdit*>();
            QCheckBox* check = ligne->findChild<QCheckBox*>();

            if (!edit || !check) continue;

            QString contenu = edit->text();
            bool validation = check->isChecked();

            int id = class_BDD.Poste_liste(contenu, validation);
            class_BDD.id_liste.push_back(id);
        }

        // 3) Créer les relations dans contenu_liste
        for (int id_l : class_BDD.id_liste) {
            class_BDD.Poste_contenu_liste(class_BDD.id_element, id_l);
        }

        // 4) Nettoyer l'interface
        while (ui->verticalLayout_8->count() > 0) {
            QLayoutItem* item = ui->verticalLayout_8->takeAt(0);
            if (item->widget()) item->widget()->deleteLater();
            delete item;
        }

        ui->stackedWidget->setCurrentIndex(3);
        affiche_element_liste();
        });
}

void Qt_main::affiche_element_liste() {

    std::vector<BDD::LigneListe> data_liste = class_BDD.Get_liste();
    std::vector<BDD::LigneContenueElement> data_contenue = class_BDD.Get_contenu_liste();
    std::vector<BDD::LigneElement> data_element = class_BDD.Get_element();

    // On vide le layout avant de recréer l'affichage
    QLayoutItem* item;
    while ((item = ui->verticalLayout_9->takeAt(0)) != nullptr) {
        if (item->widget()) delete item->widget();
        delete item;
    }

    // Pour éviter de créer plusieurs fois le même projet
    std::vector<int> elements_deja_crees;

    for (int i = 0; i < data_element.size(); i++) {

        int id_element = data_element[i].id;

        // Vérifier si cet élément a déjà été traité
        bool deja_cree = false;
        for (int d = 0; d < elements_deja_crees.size(); d++) {
            if (elements_deja_crees[d] == id_element) {
                deja_cree = true;
                break;
            }
        }
        if (deja_cree) continue;

        // Vérifier si cet élément a au moins une ligne associée
        bool a_au_moins_une_ligne = false;
        for (int j = 0; j < data_contenue.size(); j++) {
            if (data_contenue[j].id_element == id_element) {
                a_au_moins_une_ligne = true;
                break;
            }
        }
        if (!a_au_moins_une_ligne) continue;

        // On marque cet élément comme déjà créé
        elements_deja_crees.push_back(id_element);

        // Séparateur entre projets (sauf avant le tout premier)
        if (ui->verticalLayout_9->count() > 0) {
            QFrame* sep = new QFrame(this);
            sep->setFrameShape(QFrame::HLine);
            sep->setFixedHeight(1);
            sep->setStyleSheet("background-color: #3F3F3F; margin: 6px 0px;");
            ui->verticalLayout_9->addWidget(sep);
        }

        // Carte projet
        QWidget* projet = new QWidget(this);
        projet->setObjectName("carte_projet");
        projet->setStyleSheet(
            "QWidget#carte_projet {"
            "   background-color: #2A2A2A;"
            "   border-left: 3px solid #E67E22;"
            "   border-radius: 4px;"
            "   padding: 4px;"
            "}"
        );

        QVBoxLayout* zone_projet = new QVBoxLayout(projet);
        zone_projet->setContentsMargins(10, 8, 8, 8);
        zone_projet->setSpacing(4);

        // Nom + description
        QLabel* nom = new QLabel(data_element[i].nom, projet);
        nom->setStyleSheet("font-size: 18pt; font-weight: 600;");
        zone_projet->addWidget(nom);

        QLabel* description = new QLabel(data_element[i].description, projet);
        description->setStyleSheet("color: #A0A0A0; font-size: 14pt;");
        zone_projet->addWidget(description);

        // Bouton modification
        QPushButton* modif = new QPushButton("Modification", projet);
        zone_projet->addWidget(modif);

        affichage_modif_projet(id_element, data_element, data_liste, data_contenue, modif);

        // Lignes associées à ce projet
        for (int j = 0; j < data_contenue.size(); j++) {

            if (data_contenue[j].id_element == id_element) {

                int id_liste = data_contenue[j].id_liste;

                for (int k = 0; k < data_liste.size(); k++) {

                    if (data_liste[k].id == id_liste) {
                        int id_liste_capture = data_liste[k].id;  // ← capture

                        QWidget* ligne = new QWidget(projet);
                        QHBoxLayout* layout = new QHBoxLayout(ligne);

                        QCheckBox* validation = new QCheckBox(ligne);
                        validation->setChecked(data_liste[k].validation);
                        layout->addWidget(validation);

                        QLabel* contenu = new QLabel(data_liste[k].contenu, ligne);
                        if (data_liste[k].validation) {
                            contenu->setStyleSheet("color: #606060; text-decoration: line-through;");
                        }
                        else {
                            contenu->setStyleSheet("color: #F0F0F0;");
                        }
                        layout->addWidget(contenu);

                        connect(validation, &QCheckBox::toggled, this, [this, contenu, id_liste_capture](bool checked) {
                            if (checked) {
                                contenu->setStyleSheet("color: #606060; text-decoration: line-through;");
                            }
                            else {
                                contenu->setStyleSheet("color: #F0F0F0;");
                            }
                            class_BDD.modif_liste(id_liste_capture, contenu->text(), checked);
                            });

                        zone_projet->addWidget(ligne);
                    }
                }
            }
        }

        ui->verticalLayout_9->addWidget(projet);
    }
}

void Qt_main::affichage_modif_projet(int id_element, std::vector<BDD::LigneElement> data_element, std::vector<BDD::LigneListe> data_liste, const std::vector<BDD::LigneContenueElement> data_contenue_liste, QPushButton* modif)
{
    connect(modif, &QPushButton::clicked, this,
        [this, id_element, data_element, data_liste, data_contenue_liste]()
        {
            ui->stackedWidget->setCurrentIndex(5);

            // 1) Nettoyer le layout
            while (ui->verticalLayout_12->count() > 0) {
                QLayoutItem* item = ui->verticalLayout_12->takeAt(0);
                if (item->widget())
                    item->widget()->deleteLater();
                delete item;
            }

            // 2) Affichage des infos de l'élément
            for (auto& e : data_element) {
                if (e.id == id_element) {
                    ui->lineEdit_7->setText(e.nom);
                    ui->lineEdit_8->setText(e.description);
                    break;
                }
            }

            // 3) Création des lignes
            for (auto& c : data_contenue_liste) {
                if (c.id_element != id_element) continue;

                for (auto& l : data_liste) {
                    if (l.id != c.id_liste) continue;

                    table_id_liste_modif.push_back(l.id);

                    QWidget* ligne = new QWidget(this);
                    ligne->setMaximumHeight(120);
                    QVBoxLayout* zone = new QVBoxLayout(ligne);
                    zone->setContentsMargins(8, 8, 8, 8);
                    zone->setSpacing(6);

                    QWidget* rangee = new QWidget(ligne);

                    QHBoxLayout* layoutRangee = new QHBoxLayout(rangee);
                    layoutRangee->setContentsMargins(0, 0, 0, 0);
                    layoutRangee->setSpacing(8);

                    QCheckBox* validation = new QCheckBox(rangee);
                    validation->setChecked(l.validation);

                    QLineEdit* contenu = new QLineEdit(l.contenu, rangee);
                    layoutRangee->addWidget(validation);
                    layoutRangee->addWidget(contenu);

                    QPushButton* supp = new QPushButton("X", ligne);
                    supp->setFixedHeight(36);
                    supp->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

                    zone->addWidget(rangee);
                    zone->addWidget(supp);

                    ui->verticalLayout_12->addWidget(ligne);  // ← widget d'abord

                    connect(supp, &QPushButton::clicked, this, [ligne]() {
                        ligne->deleteLater();
                        });
                }
            }

            // ← addStretch UNE SEULE FOIS après la boucle
            ui->verticalLayout_12->addStretch();

            // 4) Bouton ajout
            ajout_liste_modif(id_element, data_contenue_liste);

            // 5) Bouton modification
            modif_projet(id_element, data_element, data_liste, data_contenue_liste);
        });
}

void Qt_main::ajout_liste_modif(int id_element, const std::vector<BDD::LigneContenueElement> data_contenue_liste) {

    disconnect(ui->pushButton_12, nullptr, this, nullptr);

    connect(ui->pushButton_12, &QPushButton::clicked, this, [this, id_element, data_contenue_liste]() {

        // Retirer le stretch avant d'ajouter la nouvelle ligne
        int last = ui->verticalLayout_12->count() - 1;
        if (last >= 0) {
            QLayoutItem* lastItem = ui->verticalLayout_12->itemAt(last);
            if (lastItem && !lastItem->widget()) {
                ui->verticalLayout_12->takeAt(last);
                delete lastItem;
            }
        }

        QWidget* ligne = new QWidget(this);
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

        ui->verticalLayout_12->addWidget(ligne);
        ui->verticalLayout_12->addStretch();  // ← remet le stretch en dernier

        connect(supp, &QPushButton::clicked, this, [ligne]() {
            ligne->deleteLater();
            });
        });
}

void Qt_main::modif_projet(int id_element, std::vector<BDD::LigneElement> data_element, std::vector<BDD::LigneListe>data_liste, const std::vector<BDD::LigneContenueElement> data_contenue_liste){

    disconnect(ui->pushButton_13, nullptr, this, nullptr);

    connect(ui->pushButton_13, &QPushButton::clicked, this, [this, id_element, data_element, data_liste, data_contenue_liste]() {
        
        //element
        for (int y = 0; y < data_element.size(); y++) {

            if (id_element == data_element[y].id) {
                class_BDD.modif_element(id_element, ui->lineEdit_7->text(), ui->lineEdit_8->text());
            }

        }

        //NB Liste
        int max_liste = 0;
        for (int f = 0; f < data_contenue_liste.size(); f++) {

            for (int h = 0; h < data_liste.size(); h++) {

                if (id_element == data_contenue_liste[f].id_element) {

                    if (data_contenue_liste[f].id_liste == data_liste[h].id) {
                        max_liste++;
                    }
                }
            }
        }

        //reste
        if (verife_reste_liste == -1) {
            int reste = ui->verticalLayout_12->count() - max_liste;
            if (reste < 0)reste = reste * (-1);

            verife_reste_liste = reste;
        }

        //liste
        //update
        int compteur_update = 0;
        int nb = ui->verticalLayout_12->count();

        for (int i = 0; i < max_liste; i++) {

            QLayoutItem* item = ui->verticalLayout_12->itemAt(i);
            if (!item) continue;

            QWidget* ligne = item->widget();
            if (!ligne) continue;

            QLineEdit* contenu = ligne->findChild<QLineEdit*>();
            QCheckBox* validation = ligne->findChild<QCheckBox*>();

            if (!contenu || !validation)
                continue;

            QString texte = contenu->text();
            bool valide = validation->isChecked();

            qDebug() << "Ligne" << i << "Texte:" << texte << "Valide:" << valide;

            // Mise à jour BDD
            class_BDD.modif_liste(table_id_liste_modif[i], texte, valide);
        
            compteur_update++;
        }

        //poste
        if (nb > max_liste) {

            for (int i = 0; i < nb - max_liste; i++) {

                QLayoutItem* item = ui->verticalLayout_12->itemAt(i + compteur_update);
                if (!item) continue;

                QWidget* ligne = item->widget();
                if (!ligne) continue;

                QLineEdit* contenu = ligne->findChild<QLineEdit*>();
                QCheckBox* validation = ligne->findChild<QCheckBox*>();

                if (!contenu || !validation) continue;

                QString texte = contenu->text();
                bool valide = validation->isChecked();

                qDebug() << "Nouvelle Ligne" << i << "Texte:" << texte << "Valide:" << valide;
            
                int id_liste = class_BDD.Poste_liste(texte, valide);

                class_BDD.Poste_contenu_liste(id_element, id_liste);
            }
        }

        // delete
        if (nb < max_liste) {

            int nb_delete = max_liste - nb;  // nombre de lignes à supprimer

            qDebug() << "Suppression de" << nb_delete << "lignes dans la BDD";

            // On supprime les dernières lignes de la BDD
            for (int i = 0; i < nb_delete; i++) {

                // Récupérer le dernier id_liste dans table_id_liste_modif
                int id_liste = table_id_liste_modif.back();
                table_id_liste_modif.pop_back();

                qDebug() << "Suppression id_liste =" << id_liste;

                // Suppression dans la BDD
                class_BDD.delete_liste(id_liste);
                class_BDD.delete_contenu_liste(id_liste);
            }
        }

        ui->stackedWidget->setCurrentIndex(3);
        affiche_info_user();
        affiche_element_liste();
    });

}


Qt_main::~Qt_main()
{
    delete ui; 
}