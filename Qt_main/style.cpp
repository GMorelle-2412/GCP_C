#include "Style.h"

void Style::appliquerStyle(QMainWindow* fenetre)
{
    const QString styleGlobal = R"(

        /* ════════════════════════════════════════════════════════════════════
           Palette : #232323 fond | #2E2E2E champs | #3F3F3F bordures
                      #E67E22 accent | #C0691A accent pressed | #F0F0F0 texte
                      #A0A0A0 texte secondaire | #606060 texte désactivé
           ════════════════════════════════════════════════════════════════════ */

        /* ── Base ──────────────────────────────────────────────────────────── */
        QMainWindow, QWidget {
            background-color: #232323;
            color: #F0F0F0;
            font-family: "Segoe UI", "Arial", sans-serif;
            font-size: 14pt;
        }

        /* ── Labels ─────────────────────────────────────────────────────────── */
        QLabel {
            color: #F0F0F0;
            background-color: transparent;
        }

        /* Titre principal (page d'accueil uniquement) */
        QLabel#label_titre {
            color: #E67E22;
            font-size: 28pt;
            font-weight: 700;
            letter-spacing: 4px;
        }

        /* Petite marque "GCP" répétée dans l'en-tête de chaque page */
        QLabel#label_5, QLabel#label_7, QLabel#label_9,
        QLabel#label_12, QLabel#label_13, QLabel#label_16 {
            color: #E67E22;
            font-size: 50pt;
            font-weight: 700;
            letter-spacing: 2px;
        }

        /* Sous-titre dynamique d'en-tête de page (nom du projet en cours, etc.) */
        QLabel#label_6, QLabel#label_8, QLabel#label_10,
        QLabel#label_11, QLabel#label_14, QLabel#label_15 {
            color: rgb(180, 180, 180);
            font-size: 18pt;
            font-weight: 600;
        }

        /* Libellés de formulaire */
        QLabel#label,   QLabel#label_2,
        QLabel#label_3, QLabel#label_4 {
            color: #A0A0A0;
            font-size: 12pt;
            font-weight: 600;
            letter-spacing: 1px;
        }

        /* ── Champs de saisie ───────────────────────────────────────────────── */
        QLineEdit {
            background-color: #2E2E2E;
            color: #F0F0F0;
            border: none;
            border-bottom: 2px solid #3F3F3F;
            border-radius: 0px;
            padding: 8px 6px;
            selection-background-color: #E67E22;
            selection-color: #232323;
            min-height: 36px;
        }

        QLineEdit:focus {
            border-bottom: 2px solid #E67E22;
        }

        QLineEdit:hover {
            border-bottom: 2px solid #C0691A;
        }

        QLineEdit:disabled {
            color: #606060;
            border-bottom: 2px solid #3F3F3F;
        }

        /* ── Zone de texte multi-lignes (contenu des notes) ───────────────────── */
        QTextEdit {
            background-color: #2E2E2E;
            color: #F0F0F0;
            border: 2px solid #3F3F3F;
            border-radius: 4px;
            padding: 10px;
            selection-background-color: #E67E22;
            selection-color: #232323;
        }

        QTextEdit:focus {
            border: 2px solid #E67E22;
        }

        /* ── Boutons : style par défaut ────────────────────────────────────────
           Sert de filet de sécurité pour TOUS les boutons, y compris ceux créés
           dynamiquement en C++ sans objectName (ex: "ouvrire" dans select.cpp).
           Les blocs ci-dessous ne font qu'ajuster taille/teinte par-dessus. ──── */
        QPushButton {
            background-color: transparent;
            color: #E67E22;
            border: 2px solid #E67E22;
            border-radius: 4px;
            padding: 10px 24px;
            font-size: 13pt;
            font-weight: 600;
            letter-spacing: 1px;
            min-height: 30px;
        }

        QPushButton:hover {
            background-color: #E67E22;
            color: #232323;
        }

        QPushButton:pressed {
            background-color: #C0691A;
            border-color: #C0691A;
            color: #F0F0F0;
        }

        QPushButton:disabled {
            color: #606060;
            border-color: #3F3F3F;
        }

        /* ── Boutons principaux (page d'accueil) ──────────────────────────────── */
        QPushButton#pushButton,
        QPushButton#pushButton_2 {
            min-width: 150px;
            min-height: 50px;
            font-size: 14pt;
            padding: 12px 28px;
        }

        /* ── Boutons d'action / validation ─────────────────────────────────────
           Soumission de formulaires + actions principales de navigation. ──────── */
        QPushButton#pushButton_3,  QPushButton#pushButton_5,
        QPushButton#pushButton_8,  QPushButton#pushButton_10,
        QPushButton#pushButton_13, QPushButton#pushButton_18,
        QPushButton#pushButton_19, QPushButton#pushButton_23,
        QPushButton#pushButton_26, QPushButton#pushButton_28,
        QPushButton#bouton_modifier,
        QPushButton[text="Enregistrer"] {
            min-height: 48px;
            padding: 12px 28px;
        }

        /* ── Boutons secondaires (annulation / retour) ─────────────────────────── */
        QPushButton#pushButton_4,  QPushButton#pushButton_6,
        QPushButton#pushButton_9,  QPushButton#pushButton_14,
        QPushButton#pushButton_16, QPushButton#pushButton_22,
        QPushButton#pushButton_25, QPushButton#pushButton_29,
        QPushButton#pushButton_31 {
            color: #A0A0A0;
            border-color: #3F3F3F;
            min-height: 48px;
            padding: 12px 28px;
        }

        QPushButton#pushButton_4:hover,  QPushButton#pushButton_6:hover,
        QPushButton#pushButton_9:hover,  QPushButton#pushButton_14:hover,
        QPushButton#pushButton_16:hover, QPushButton#pushButton_22:hover,
        QPushButton#pushButton_25:hover, QPushButton#pushButton_29:hover,
        QPushButton#pushButton_31:hover {
            background-color: #3A3A3A;
            color: #F0F0F0;
            border-color: #606060;
        }

        QPushButton#pushButton_4:pressed,  QPushButton#pushButton_6:pressed,
        QPushButton#pushButton_9:pressed,  QPushButton#pushButton_14:pressed,
        QPushButton#pushButton_16:pressed, QPushButton#pushButton_22:pressed,
        QPushButton#pushButton_25:pressed, QPushButton#pushButton_29:pressed,
        QPushButton#pushButton_31:pressed {
            background-color: #2E2E2E;
            border-color: #606060;
        }

        /* ── Boutons Déconnexion (un par page) ─────────────────────────────────── */
        QPushButton#pushButton_7,  QPushButton#pushButton_15,
        QPushButton#pushButton_17, QPushButton#pushButton_24,
        QPushButton#pushButton_27, QPushButton#pushButton_30 {
            color: #A0A0A0;
            border: 1px solid #3F3F3F;
            font-size: 13pt;
            padding: 10px 24px;
        }

        QPushButton#pushButton_7:hover,  QPushButton#pushButton_15:hover,
        QPushButton#pushButton_17:hover, QPushButton#pushButton_24:hover,
        QPushButton#pushButton_27:hover, QPushButton#pushButton_30:hover {
            background-color: #3A3A3A;
            color: #F0F0F0;
            border-color: #606060;
        }

        /* ── Boutons "Ajouter" (nouvelle ligne de liste) ───────────────────────── */
        QPushButton#pushButton_11, QPushButton#pushButton_12 {
            background-color: #2E2E2E;
            border: 1px dashed #E67E22;
            padding: 8px 20px;
            font-size: 13pt;
        }

        QPushButton#pushButton_11:hover, QPushButton#pushButton_12:hover {
            background-color: #3A3A3A;
        }

        /* ── Boutons créés dynamiquement, identifiés par leur texte ────────────── */
        QPushButton[text="Modification"] {
            border: 1px solid #E67E22;
            padding: 5px 7px;
            min-height: 44px;
        }

        QPushButton[text="Modification"]:hover {
            background-color: #E67E22;
            color: #232323;
        }

        QPushButton[text="Suppression"] {
            color: #A0A0A0;
            border: 1px solid #3F3F3F;
            padding: 10px 14px;
            min-height: 44px;
        }

        QPushButton[text="Suppression"]:hover {
            background-color: #3A3A3A;
            color: #F0F0F0;
        }

        QPushButton[text="X"] {
            color: #606060;
            border: 1px solid #3F3F3F;
            font-size: 13pt;
            min-height: 36px;
            margin-top: 4px;
            padding: 4px;
        }

        QPushButton[text="X"]:hover {
            color: #E67E22;
            border-color: #E67E22;
        }

        /* ── Cases à cocher ──────────────────────────────────────────────────── */
        QCheckBox {
            color: #F0F0F0;
            spacing: 8px;
            background-color: transparent;
        }

        QCheckBox::indicator {
            width: 22px;
            height: 22px;
            border: 2px solid #3F3F3F;
            border-radius: 3px;
            background-color: #2E2E2E;
            image: none;
        }

        QCheckBox::indicator:hover {
            border-color: #E67E22;
        }

        QCheckBox::indicator:checked {
            background-color: #E67E22;
            border-color: #E67E22;
            image: none;
        }

        QCheckBox::indicator:unchecked {
            background-color: #2E2E2E;
            border-color: #3F3F3F;
            image: none;
        }

        /* ── Zones de défilement ────────────────────────────────────────────────
           Style appliqué par défaut à TOUTES les QScrollArea, plus une variante
           "encadrée" pour celles qui contiennent un formulaire. ────────────────── */
        QScrollArea {
            background-color: #232323;
            border: none;
        }

        QScrollArea > QWidget > QWidget {
            background-color: #232323;
        }

        QScrollArea#scrollArea_2, QScrollArea#scrollArea_3, QScrollArea#scrollArea_7 {
            background-color: #2E2E2E;
            border: 1px solid #3F3F3F;
            border-radius: 4px;
        }

        QScrollArea#scrollArea_2 > QWidget > QWidget,
        QScrollArea#scrollArea_3 > QWidget > QWidget,
        QScrollArea#scrollArea_7 > QWidget > QWidget {
            background-color: #2E2E2E;
            
        }

        #zoneProjets {
            background-color: #232323;
            border-bottom: 2px solid #444;
            border-left: 2px solid #E67E22;
            padding: 10px;
        }


        /* ── Scrollbars ──────────────────────────────────────────────────────── */
        QScrollBar:vertical {
            background-color: #232323;
            width: 8px;
            border-radius: 4px;
        }

        QScrollBar::handle:vertical {
            background-color: #3F3F3F;
            border-radius: 4px;
            min-height: 30px;
        }

        QScrollBar::handle:vertical:hover {
            background-color: #E67E22;
        }

        QScrollBar::add-line:vertical,
        QScrollBar::sub-line:vertical {
            height: 0px;
        }

        QScrollBar:horizontal {
            background-color: #232323;
            height: 8px;
            border-radius: 4px;
        }

        QScrollBar::handle:horizontal {
            background-color: #3F3F3F;
            border-radius: 4px;
            min-width: 30px;
        }

        QScrollBar::handle:horizontal:hover {
            background-color: #E67E22;
        }

        QScrollBar::add-line:horizontal,
        QScrollBar::sub-line:horizontal {
            width: 0px;
        }

        /* ── StackedWidget ───────────────────────────────────────────────────── */
        QStackedWidget {
            background-color: #232323;
        }

    )";

    fenetre->setStyleSheet(styleGlobal);
}