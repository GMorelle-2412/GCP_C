#include "projet.h"

/*
void projet::bouton_creation_projet() {
    connect(ui->pushButton_8, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentIndex(4);
        });
}

void projet::ajouter_liste() {
    connect(ui->pushButton_11, &QPushButton::clicked, this, [this]() {
        int last = ui->verticalLayout_8->count() - 1;
        
        if (last >= 0) {
            QLayoutItem* lastItem = ui->verticalLayout_8->itemAt(last);
            
            if (lastItem && !lastItem->widget()) {
                ui->verticalLayout_8->takeAt(last);
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
        
        ui->verticalLayout_8->addWidget(ligne);
        ui->verticalLayout_8->addStretch();
        
        connect(supp, &QPushButton::clicked, this, [ligne]() {
            ligne->deleteLater();
            });
        });
}

void projet::creation_projet() {
    connect(ui->pushButton_10, &QPushButton::clicked, this, [this]() {
        QString nom = ui->lineEdit_5->text();
        QString description = ui->lineEdit_6->text();
        
        class_BDD->Poste_element(nom, description);
        class_BDD->id_liste.clear();
        
        for (int i = 0; i < ui->verticalLayout_8->count(); i++) {
            QWidget* ligne = ui->verticalLayout_8->itemAt(i)->widget();
            
            if (!ligne) continue;
            
            QLineEdit* edit = ligne->findChild<QLineEdit*>();
            QCheckBox* check = ligne->findChild<QCheckBox*>();
            
            if (!edit || !check) continue;
            
            int id = class_BDD->Poste_liste(edit->text(), check->isChecked());
            
            class_BDD->id_liste.push_back(id);
        }
        
        for (int id_l : class_BDD->id_liste)
            class_BDD->Poste_contenu_liste(class_BDD->id_element, id_l);
        
        while (ui->verticalLayout_8->count() > 0) {
            QLayoutItem* item = ui->verticalLayout_8->takeAt(0);
            
            if (item->widget()) item->widget()->deleteLater();
            
            delete item;
        }
        
        ui->stackedWidget->setCurrentIndex(3);
        
        affiche_element_liste();
        });
}

void projet::affiche_element_liste() {
    std::vector<BDD::LigneListe> data_liste = class_BDD->Get_liste();
    std::vector<BDD::LigneContenueElement> data_contenue = class_BDD->Get_contenu_liste();
    std::vector<BDD::LigneElement> data_element = class_BDD->Get_element();

    QLayoutItem* item;
    while ((item = ui->verticalLayout_9->takeAt(0)) != nullptr) {
        if (item->widget()) delete item->widget();
        delete item;
    }

    std::vector<int> elements_deja_crees;

    for (int i = 0; i < data_element.size(); i++) {
        int id_element = data_element[i].id;
        
        bool deja_cree = false;
        
        for (int d = 0; d < elements_deja_crees.size(); d++) {
            if (elements_deja_crees[d] == id_element) { deja_cree = true; break; }
        }
        
        if (deja_cree) continue;
        
        bool a_au_moins_une_ligne = false;
        
        for (int j = 0; j < data_contenue.size(); j++) {
            if (data_contenue[j].id_element == id_element) { a_au_moins_une_ligne = true; break; }
        }
        
        if (!a_au_moins_une_ligne) continue;
        
        elements_deja_crees.push_back(id_element);

        if (ui->verticalLayout_9->count() > 0) {
            QFrame* sep = new QFrame();
            sep->setFrameShape(QFrame::HLine);
            sep->setFixedHeight(1);
            sep->setStyleSheet("background-color: #3F3F3F; margin: 6px 0px;");
            ui->verticalLayout_9->addWidget(sep);
        }

        QWidget* projet = new QWidget();
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

        QLabel* nom = new QLabel(data_element[i].nom, projet);
        nom->setStyleSheet("font-size: 18pt; font-weight: 600;");
        zone_projet->addWidget(nom);

        QLabel* description = new QLabel(data_element[i].description, projet);
        description->setStyleSheet("color: #A0A0A0; font-size: 14pt;");
        zone_projet->addWidget(description);

        QPushButton* modif = new QPushButton("Modification", projet);
        zone_projet->addWidget(modif);
        affichage_modif_projet(id_element, data_element, data_liste, data_contenue, modif);

        for (int j = 0; j < data_contenue.size(); j++) {
            
            if (data_contenue[j].id_element == id_element) {
                int id_liste = data_contenue[j].id_liste;
                
                for (int k = 0; k < data_liste.size(); k++) {
                    
                    if (data_liste[k].id == id_liste) {
                        int id_liste_capture = data_liste[k].id;
                        
                        QWidget* ligne = new QWidget(projet);
                        
                        QHBoxLayout* layout = new QHBoxLayout(ligne);
                        
                        QCheckBox* validation = new QCheckBox(ligne);
                        validation->setChecked(data_liste[k].validation);
                        layout->addWidget(validation);
                        
                        QLabel* contenu = new QLabel(data_liste[k].contenu, ligne);
                        contenu->setStyleSheet(data_liste[k].validation
                            ? "color: #606060; text-decoration: line-through;"
                            : "color: #F0F0F0;");
                        
                        layout->addWidget(contenu);
                        
                        connect(validation, &QCheckBox::toggled, this, [this, contenu, id_liste_capture](bool checked) {
                            contenu->setStyleSheet(checked
                                ? "color: #606060; text-decoration: line-through;"
                                : "color: #F0F0F0;");
                            class_BDD->modif_liste(id_liste_capture, contenu->text(), checked);
                            });
                        zone_projet->addWidget(ligne);
                    }
                }
            }
        }
        ui->verticalLayout_9->addWidget(projet);
    }
}

void projet::affichage_modif_projet(int id_element, std::vector<BDD::LigneElement> data_element, std::vector<BDD::LigneListe> data_liste, const std::vector<BDD::LigneContenueElement> data_contenue_liste, QPushButton* modif)
{
    connect(modif, &QPushButton::clicked, this,
        [this, id_element, data_element, data_liste, data_contenue_liste]() {
            ui->stackedWidget->setCurrentIndex(5);
            
            while (ui->verticalLayout_12->count() > 0) {
                QLayoutItem* item = ui->verticalLayout_12->takeAt(0);
                if (item->widget()) item->widget()->deleteLater();
                delete item;
            }
            
            for (auto& e : data_element) {
                if (e.id == id_element) {
                    ui->lineEdit_7->setText(e.nom);
                    ui->lineEdit_8->setText(e.description);
                    break;
                }
            }
            
            for (auto& c : data_contenue_liste) {
                
                if (c.id_element != id_element) continue;
                
                for (auto& l : data_liste) {
                    
                    if (l.id != c.id_liste) continue;
                    
                    table_id_liste_modif.push_back(l.id);
                    
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
                    validation->setChecked(l.validation);
                    
                    QLineEdit* contenu = new QLineEdit(l.contenu, rangee);
                    layoutRangee->addWidget(validation);
                    layoutRangee->addWidget(contenu);
                    
                    QPushButton* supp = new QPushButton("X", ligne);
                    supp->setFixedHeight(36);
                    
                    supp->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
                    zone->addWidget(rangee);
                    zone->addWidget(supp);
                    
                    ui->verticalLayout_12->addWidget(ligne);
                    
                    connect(supp, &QPushButton::clicked, this, [ligne]() { ligne->deleteLater(); });
                }
            }

            ui->verticalLayout_12->addStretch();
            ajout_liste_modif(id_element, data_contenue_liste);
            modif_projet(id_element, data_element, data_liste, data_contenue_liste);
        });
}

void projet::ajout_liste_modif(int id_element, const std::vector<BDD::LigneContenueElement> data_contenue_liste) {
    disconnect(ui->pushButton_12, nullptr, this, nullptr);
    
    connect(ui->pushButton_12, &QPushButton::clicked, this, [this, id_element, data_contenue_liste]() {
        int last = ui->verticalLayout_12->count() - 1;
        
        if (last >= 0) {
            QLayoutItem* lastItem = ui->verticalLayout_12->itemAt(last);
            
            if (lastItem && !lastItem->widget()) {
                ui->verticalLayout_12->takeAt(last);
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
        
        ui->verticalLayout_12->addWidget(ligne);
        ui->verticalLayout_12->addStretch();
        
        connect(supp, &QPushButton::clicked, this, [ligne]() { ligne->deleteLater(); });
        });
}

void projet::modif_projet(int id_element, std::vector<BDD::LigneElement> data_element, std::vector<BDD::LigneListe> data_liste, const std::vector<BDD::LigneContenueElement> data_contenue_liste) {
    disconnect(ui->pushButton_13, nullptr, this, nullptr);
    
    connect(ui->pushButton_13, &QPushButton::clicked, this, [this, id_element, data_element, data_liste, data_contenue_liste]() {

        for (int y = 0; y < data_element.size(); y++) {
            if (id_element == data_element[y].id)
                class_BDD->modif_element(id_element, ui->lineEdit_7->text(), ui->lineEdit_8->text());
        }

        int max_liste = 0;
        for (int f = 0; f < data_contenue_liste.size(); f++)
            
            for (int h = 0; h < data_liste.size(); h++)
                
                if (id_element == data_contenue_liste[f].id_element &&
                    data_contenue_liste[f].id_liste == data_liste[h].id)
                    max_liste++;

        if (verife_reste_liste == -1) {
            int reste = ui->verticalLayout_12->count() - max_liste;
            
            if (reste < 0) reste *= -1;
            
            verife_reste_liste = reste;
        }

        int compteur_update = 0;
        int nb = ui->verticalLayout_12->count();

        for (int i = 0; i < max_liste; i++) {
            QLayoutItem* item = ui->verticalLayout_12->itemAt(i);
            
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
                
                QLayoutItem* item = ui->verticalLayout_12->itemAt(i + compteur_update);
                
                if (!item) continue;
                
                QWidget* ligne = item->widget();
                
                if (!ligne) continue;
                
                QLineEdit* contenu = ligne->findChild<QLineEdit*>();
                QCheckBox* validation = ligne->findChild<QCheckBox*>();
                
                if (!contenu || !validation) continue;
                
                qDebug() << "Nouvelle Ligne" << i << "Texte:" << contenu->text() << "Valide:" << validation->isChecked();
                
                int id_liste = class_BDD->Poste_liste(contenu->text(), validation->isChecked());
                
                class_BDD->Poste_contenu_liste(id_element, id_liste);
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
                
                class_BDD->delete_contenu_liste(id_liste);
            }
        }

        ui->stackedWidget->setCurrentIndex(3);
        affiche_element_liste();
        });
}*/