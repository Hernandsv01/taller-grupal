#include "matchendedgui.h"

#include "./ui_matchendedgui.h"

MatchEndedGui::MatchEndedGui(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MatchEndedGui) {
    ui->setupUi(this);

    // Create a new QLabel

    for (int i = 0; i < 50; i++) {
        std::string label_text = "Player" + std::to_string(i);

        QLabel *label = new QLabel(label_text.c_str());

        // Add the label to the scrollable list
        ui->scrollAreaWidgetContents->layout()->addWidget(label);
    }
    // QLabel *label = new QLabel("Hello, World!");

    QSpacerItem *spacer =
        new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    ui->scrollAreaWidgetContents->layout()->addItem(spacer);
}

MatchEndedGui::~MatchEndedGui() { delete ui; }
