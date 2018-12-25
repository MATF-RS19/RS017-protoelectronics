#include "dialog.h"
#include <QVBoxLayout>

Dialog::Dialog(QWidget* parent)
    : QDialog (parent){

    label = new QLabel(tr("New resistance value:"));
    lineEdit = new QLineEdit;
    label->setBuddy(lineEdit);

    buttonBox = new QDialogButtonBox(Qt::Horizontal);
    buttonBox->addButton(QDialogButtonBox::Apply);
    buttonBox->addButton(QDialogButtonBox::Cancel);
    buttonBox->addButton(QDialogButtonBox::Save);

    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addWidget(label);
    topLayout->addWidget(lineEdit);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(buttonBox);

    setLayout(mainLayout);
    setWindowTitle(tr("Resistor edit"));
}
