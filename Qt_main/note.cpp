#include "note.h"

note::note(QObject* parent)
    : QObject(parent), class_BDD(nullptr), class_sauvegarde(new sauvegarde())
{
}

/*Notes*/
QWidget* note::affiche_note(QPushButton* bouton_affiche_note,
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

        auto data_contenue_note = class_BDD->Get_Contenu_element_notes(class_sauvegarde->sauvegarde_element.id);

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

void note::zone_ajout_note(QPushButton* bouton_zone_ajout_note, QStackedWidget* stackedWidget) {
    bouton_zone_ajout_note->disconnect();

    connect(bouton_zone_ajout_note, &QPushButton::clicked, this, [=]() {

        stackedWidget->setCurrentIndex(9);
        });
}

void note::ajout_note(QStackedWidget* stackedWidget, QPushButton* bouton_ajout_note, QLineEdit* lineEdit_9, QTextEdit* textEdit) {

    bouton_ajout_note->disconnect();

    connect(bouton_ajout_note, &QPushButton::clicked, this, [=]() {

        qDebug() << class_sauvegarde->sauvegarde_element.nom;

        qDebug() << class_sauvegarde->sauvegarde_element.id;

        QString text = textEdit->toPlainText();

        QString nom = lineEdit_9->text();

        class_sauvegarde->id_notes = class_BDD->Poste_note(text, nom);

        class_BDD->Poste_Contenu_element_notes(class_sauvegarde->sauvegarde_element.id, class_sauvegarde->id_notes);

        stackedWidget->setCurrentIndex(8);
        });
}

void note::affiche_modif_note(QStackedWidget* stackedWidget,
    int id_note,
    AutoResizeTextBrowser* nom,
    AutoResizeTextBrowser* text)
{
    // Nettoyage du layout
    QLayoutItem* item;
    while ((item = class_sauvegarde->layout_sauve_modif_notes->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    QLineEdit* editNom = new QLineEdit();
    editNom->setText(nom->toPlainText());
    class_sauvegarde->layout_sauve_modif_notes->addWidget(editNom);

    QTextEdit* editText = new QTextEdit();
    editText->setPlainText(text->toPlainText());
    class_sauvegarde->layout_sauve_modif_notes->addWidget(editText);

    QPushButton* save = new QPushButton("Enregistrer");
    class_sauvegarde->layout_sauve_modif_notes->addWidget(save);

    connect(save, &QPushButton::clicked, this, [=]() {
        class_BDD->modif_note(id_note, editNom->text(), editText->toPlainText());
        nom->setText(editNom->text());
        text->setText(editText->toPlainText());
        stackedWidget->setCurrentIndex(8);
        });

    stackedWidget->setCurrentIndex(10);
}
