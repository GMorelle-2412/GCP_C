#include "Style.h"

void Style::appliquerStyle(QMainWindow* fenetre)
{
    const QString styleGlobal = R"(

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

        QLabel#label_titre, QLabel#label_5 {
            color: #E67E22;
            font-size: 28pt;
            font-weight: 700;
            letter-spacing: 4px;
        }

        QLabel#label_6 {
            color: rgb(180, 180, 180);
            font-size: 18pt;
            font-weight: 600;
        }

        QLabel#label,  QLabel#label_2,
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
            background-color: #2E2E2E;
        }

        QLineEdit:hover {
            border-bottom: 2px solid #C0691A;
        }

        /* ── Boutons principaux ─────────────────────────────────────────────── */
        QPushButton#pushButton,
        QPushButton#pushButton_2 {
            background-color: transparent;
            color: #E67E22;
            border: 2px solid #E67E22;
            border-radius: 4px;
            padding: 12px 28px;
            font-size: 14pt;
            font-weight: 600;
            letter-spacing: 1px;
            min-height: 24px;
        }

        QPushButton#pushButton:hover,
        QPushButton#pushButton_2:hover {
            background-color: #E67E22;
            color: #232323;
        }

        QPushButton#pushButton:pressed,
        QPushButton#pushButton_2:pressed {
            background-color: #C0691A;
            border-color: #C0691A;
            color: #F0F0F0;
        }

        /* ── Boutons validation formulaires ─────────────────────────────────── */
        QPushButton#pushButton_3,
        QPushButton#pushButton_5,
        QPushButton#pushButton_8,
        QPushButton#pushButton_10,
        QPushButton#pushButton_13 {
            background-color: transparent;
            color: #E67E22;
            border: 2px solid #E67E22;
            border-radius: 4px;
            padding: 12px 28px;
            font-size: 14pt;
            font-weight: 600;
            letter-spacing: 1px;
            min-height: 48px;
        }

        QPushButton#pushButton_3:hover,
        QPushButton#pushButton_5:hover,
        QPushButton#pushButton_8:hover,
        QPushButton#pushButton_10:hover,
        QPushButton#pushButton_13:hover {
            background-color: #E67E22;
            color: #232323;
        }

        QPushButton#pushButton_3:pressed,
        QPushButton#pushButton_5:pressed,
        QPushButton#pushButton_8:pressed,
        QPushButton#pushButton_10:pressed,
        QPushButton#pushButton_13:pressed {
            background-color: #C0691A;
            border-color: #C0691A;
            color: #F0F0F0;
        }

        /* ── Boutons annulation ──────────────────────────────────────────────── */
        QPushButton#pushButton_4,
        QPushButton#pushButton_6,
        QPushButton#pushButton_9,
        QPushButton#pushButton_14 {
            background-color: transparent;
            color: #A0A0A0;
            border: 2px solid #3F3F3F;
            border-radius: 4px;
            padding: 12px 28px;
            font-size: 14pt;
            min-height: 48px;
        }

        QPushButton#pushButton_4:hover,
        QPushButton#pushButton_6:hover,
        QPushButton#pushButton_9:hover,
        QPushButton#pushButton_14:hover {
            background-color: #3A3A3A;
            color: #F0F0F0;
            border-color: #606060;
        }

        /* ── Bouton Déconnexion ──────────────────────────────────────────────── */
        QPushButton#pushButton_7 {
            background-color: transparent;
            color: #A0A0A0;
            border: 1px solid #3F3F3F;
            border-radius: 4px;
            font-size: 13pt;
            padding: 10px 24px;
        }

        QPushButton#pushButton_7:hover {
            background-color: #3A3A3A;
            color: #F0F0F0;
            border-color: #606060;
        }

        /* ── Boutons Ajouter ─────────────────────────────────────────────────── */
        QPushButton#pushButton_11,
        QPushButton#pushButton_12 {
            background-color: #2E2E2E;
            color: #E67E22;
            border: 1px dashed #E67E22;
            border-radius: 4px;
            padding: 8px 20px;
            font-size: 13pt;
        }

        QPushButton#pushButton_11:hover,
        QPushButton#pushButton_12:hover {
            background-color: #3A3A3A;
        }

        /* ── Boutons dynamiques ──────────────────────────────────────────────── */
        QPushButton[text="Modification"] {
            background-color: transparent;
            color: #E67E22;
            border: 1px solid #E67E22;
            border-radius: 3px;
            padding: 5px 7px;
            font-size: 14pt;
            min-height: 44px;
        }

        QPushButton[text="Modification"]:hover {
            background-color: #E67E22;
            color: #232323;
        }

        QPushButton[text="Suppression"] {
            background-color: transparent;
            color: #A0A0A0;
            border: 1px solid #3F3F3F;
            border-radius: 3px;
            padding: 10px 14px;
            font-size: 14pt;
            min-height: 44px;
        }

        QPushButton[text="Suppression"]:hover {
            background-color: #3A3A3A;
            color: #F0F0F0;
        }

        QPushButton[text="X"] {
            background-color: transparent;
            color: #606060;
            border: 1px solid #3F3F3F;
            border-radius: 3px;
            font-size: 13pt;
            min-height: 36px;
            margin-top: 4px;

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

        /* Règle universelle — couvre aussi les checkboxes créées dynamiquement */
        QCheckBox::indicator,
        QWidget QCheckBox::indicator {
            width: 22px;
            height: 22px;
            border: 2px solid #3F3F3F;
            border-radius: 3px;
            background-color: #2E2E2E;
            image: none;
        }

        QCheckBox::indicator:hover,
        QWidget QCheckBox::indicator:hover {
            border-color: #E67E22;
        }

        QCheckBox::indicator:checked,
        QWidget QCheckBox::indicator:checked {
            background-color: #E67E22;
            border-color: #E67E22;
            image: none;
        }

        QCheckBox::indicator:unchecked,
        QWidget QCheckBox::indicator:unchecked {
            background-color: #2E2E2E;
            border-color: #3F3F3F;
            image: none;
        }

        /* ── ScrollArea principale (page liste projets) ──────────────────────── */
        QScrollArea#scrollArea {
            background-color: #232323;
            border: none;
        }

        QScrollArea#scrollArea > QWidget > QWidget {
            background-color: #232323;
        }

        /* ── ScrollAreas secondaires (création / modification) ───────────────── */
        QScrollArea#scrollArea_2,
        QScrollArea#scrollArea_3 {
            background-color: #2E2E2E;
            border: 1px solid #3F3F3F;
            border-radius: 4px;
        }

        QScrollArea#scrollArea_2 > QWidget > QWidget,
        QScrollArea#scrollArea_3 > QWidget > QWidget {
            background-color: #2E2E2E;
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