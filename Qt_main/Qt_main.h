#pragma once

#include "ui_Qt_main.h"
#include "BDD.h"
#include "identification.h"
#include "Style.h"

#include <QScroller>
#include <QEasingCurve>
#include <QtWidgets/QMainWindow>
#include <QScrollBar>
#include <QScrollerProperties>
#include <QScrollArea>
#include "SelectManager.h"


class Qt_main : public QMainWindow {
    Q_OBJECT

public:
    Qt_main(QWidget* parent = nullptr);
    ~Qt_main();
    
    Ui::Qt_mainClass* ui;

    BDD* class_BDD;
    identification* class_identification;
    SelectManager* class_select;
	Style* class_style;

private slots:
	void select_affichage_projets();

    void bouton_annulation();
};
