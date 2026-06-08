#include "Style.h"

void Style::appliquerStyle(QMainWindow* window)
{
    window->setStyleSheet(R"(

/* Fenêtre principale */
QMainWindow,
QWidget {
    background-color: #232323;
    color: white;
    font-family: "Segoe UI";
}

/* Titre GCP */
QLabel#label_titre {
    color: #E67E22;
    font-size: 50px;
    font-weight: bold;
}

/* Labels */
QLabel {
    color: white;
    font-size: 14px;
}

/* Champs texte */
QLineEdit {
    background-color: #2F2F2F;
    border: 2px solid #E67E22;
    border-radius: 5px;
    padding: 6px;
    color: white;
    min-height: 28px;
}

QLineEdit:focus {
    border: 2px solid #F39C12;
}

/* Boutons principaux */
QPushButton {
    background-color: #232323;
    color: #E67E22;
    border: 2px solid #E67E22;
    border-radius: 5px;
    min-height: 35px;
    padding: 5px;
}

QPushButton:hover {
    background-color: #E67E22;
    color: black;
}

QPushButton:pressed {
    background-color: #D35400;
    color: white;
}

/* Cases à cocher */
QCheckBox {
    color: white;
}

QCheckBox::indicator {
    width: 18px;
    height: 18px;
}

QCheckBox::indicator:unchecked {
    border: 2px solid #E67E22;
    background: transparent;
}

QCheckBox::indicator:checked {
    border: 2px solid #E67E22;
    background: #E67E22;
}

/* ScrollArea */
QScrollArea {
    border: 1px solid #444;
    border-radius: 5px;
    background-color: #2F2F2F;
}

/* Barre de défilement verticale */
QScrollBar:vertical {
    background: #2F2F2F;
    width: 12px;
}

QScrollBar::handle:vertical {
    background: #E67E22;
    border-radius: 5px;
}

QScrollBar::add-line:vertical,
QScrollBar::sub-line:vertical {
    height: 0px;
}

/* Zone projet */
#scrollAreaWidgetContents,
#scrollAreaWidgetContents_2,
#scrollAreaWidgetContents_3 {
    background-color: #2F2F2F;
}

)");
}