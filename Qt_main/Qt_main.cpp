#include "Qt_main.h"
#include "select.h"

Qt_main::Qt_main(QWidget* parent)
    : QMainWindow(parent),
    ui(new Ui::Qt_mainClass)
{
    ui->setupUi(this);
    Style::appliquerStyle(this);

    // 1) Créer la BDD (instance unique)
    class_BDD = new BDD();
    class_BDD->Connect_BDD();

    // 2) Créer les objets métier
    class_projet = new projet(this);
    class_identification = new identification(this);
    class_select = new select(this);

    // 3) Câbler l'UI sur tous les objets
    class_projet->ui = ui;
    class_identification->ui = ui;
    // select récupère déjà ui dans son constructeur via Qt_main*

    // 4) Câbler la BDD partagée
    class_projet->class_BDD = class_BDD;
    class_identification->class_BDD = class_BDD;

    // 5) Câbler les pointeurs croisés
    class_identification->class_projet = class_projet;
    class_identification->class_select = class_select;

    // 6) Initialiser l'interface
    ui->stackedWidget->setCurrentIndex(0);

    class_identification->page_connection();
    class_identification->bouton_connection();
    class_identification->page_inscription();
    class_identification->bouton_inscription();

    bouton_annulation();

    class_identification->bouton_deconection();

    class_projet->bouton_creation_projet();
    class_projet->ajouter_liste();
    class_projet->creation_projet();

    // 7) Scroller (inchangé)
    auto configScrollerUltraSmooth = [](QScrollArea* area) {
        QScroller* scroller = QScroller::scroller(area->viewport());
        QScroller::grabGesture(area->viewport(), QScroller::TouchGesture);
        QScrollerProperties props = scroller->scrollerProperties();
        props.setScrollMetric(QScrollerProperties::DragStartDistance, 0.0001);
        props.setScrollMetric(QScrollerProperties::DragVelocitySmoothingFactor, 0.05);
        props.setScrollMetric(QScrollerProperties::AxisLockThreshold, 0.85);
        props.setScrollMetric(QScrollerProperties::DecelerationFactor, 0.06);
        props.setScrollMetric(QScrollerProperties::MaximumVelocity, 4.0);
        props.setScrollMetric(QScrollerProperties::MinimumVelocity, 0.02);
        props.setScrollMetric(QScrollerProperties::AcceleratingFlickMaximumTime, 0.35);
        props.setScrollMetric(QScrollerProperties::AcceleratingFlickSpeedupFactor, 1.8);
        props.setScrollMetric(QScrollerProperties::OvershootDragResistanceFactor, 0.15);
        props.setScrollMetric(QScrollerProperties::OvershootScrollDistanceFactor, 0.04);
        props.setScrollMetric(QScrollerProperties::OvershootScrollTime, 0.12);
        props.setScrollMetric(QScrollerProperties::ScrollingCurve,
            QVariant::fromValue(QEasingCurve(QEasingCurve::OutQuint)));
        scroller->setScrollerProperties(props);
        area->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        };

    configScrollerUltraSmooth(ui->scrollArea);
    configScrollerUltraSmooth(ui->scrollArea_2);
    configScrollerUltraSmooth(ui->scrollArea_3);
}

void Qt_main::bouton_annulation() {
    connect(ui->pushButton_6, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentIndex(0);
        });

    connect(ui->pushButton_4, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentIndex(0);
        });

    connect(ui->pushButton_9, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentIndex(3);
        class_identification->affiche_info_user();
        class_projet->affiche_element_liste();
        while (ui->verticalLayout_8->count() > 0) {
            QLayoutItem* item = ui->verticalLayout_8->takeAt(0);
            if (item->widget()) item->widget()->deleteLater();
            delete item;
        }
        });

    connect(ui->pushButton_14, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentIndex(3);
        class_identification->affiche_info_user();
        class_projet->affiche_element_liste();
        while (ui->verticalLayout_12->count() > 0) {
            QLayoutItem* item = ui->verticalLayout_12->takeAt(0);
            if (item->widget()) item->widget()->deleteLater();
            delete item;
        }
        conteur_ajout_liste = 0;
        });
}

Qt_main::~Qt_main() {
    delete ui;
    delete class_BDD;
}