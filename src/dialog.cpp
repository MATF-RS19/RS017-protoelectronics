#include "dialog.h"
#include <QVBoxLayout>
#include <QDebug>

Dialog::~Dialog() {}

Dialog::Dialog(Component* component, QWidget* parent)
	: QDialog (parent), component(component)
{
	// Making dialog for resistor
	if(component->componentType()=="resistor") {
		nameLabel = new QLabel(tr("Resistance:"));
		setWindowTitle(tr("Resistor edit"));
		r = dynamic_cast<Resistor*>(component);
		isResistor = true;
	}

	// Making dialog for dc voltage
	else if(component->componentType() == "voltage") {
		nameLabel = new QLabel(tr("Voltage:"));
		setWindowTitle(tr("DC Voltage edit"));
		v = dynamic_cast<DCVoltage*>(component);
		isDCVoltage = true;
	}

	// Making dialog for clock
	else if(component->componentType() == "clock") {
		nameLabel = new QLabel(tr("Clock:"));
		setWindowTitle(tr("Clock's time interval edit"));
		cl = dynamic_cast<Clock*>(component);
		isClock = true;
	}

	// Everything else is the same for all
	// Labels
    lineEdit = new QLineEdit;
	nameLabel->setBuddy(lineEdit);

	errorLabel = new QLabel(tr(""));
	errorLabel->setStyleSheet("QLabel{color:red;}");

	// Buttons
	buttonBox = new QDialogButtonBox(Qt::Horizontal);

	cancelButton = new QPushButton(tr("&Cancel"));
	cancelButton->setDefault(true);

	okButton = new QPushButton(tr("&Ok"));
	okButton->setDefault(true);

	buttonBox->addButton(cancelButton, QDialogButtonBox::RejectRole);
	buttonBox->addButton(okButton, QDialogButtonBox::ApplyRole);

	// Aligning buttons on center and setting stylesheet
	buttonBox->setCenterButtons(true);
	buttonBox->setStyleSheet("background-color: rgb(8, 246, 242); border-radius: 10px; padding: 6px; ");

	// Connecting buttons with slots
	connect(this->cancelButton, SIGNAL(clicked(bool)), this, SLOT(onCancelButtonInDialog()));
	connect(this->okButton, SIGNAL(clicked(bool)), this, SLOT(onOkButtonInDialog()));

	// Layouts
    QHBoxLayout *topLayout = new QHBoxLayout;
	topLayout->addWidget(nameLabel);
    topLayout->addWidget(lineEdit);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(buttonBox);
	mainLayout->addSpacing(8);

    setLayout(mainLayout);

	// Only resistor and clock need errorLabel
	if(isResistor || isClock)
		mainLayout->addWidget(errorLabel);
}

void Dialog::keyPressEvent(QKeyEvent *event) {
	// On esc button we close dialog
	if(event->key() == Qt::Key_Escape){
		this->close();
	}
	QDialog::keyPressEvent(event);
}


void Dialog::onOkButtonInDialog() {
	// We save old value in case after apply happeneds cancel
	// For resistance we have to check new value
	if(isResistor) {
		double newResistanceValue = this->lineEdit->text().toDouble();
		if(newResistanceValue <= 0) {
			errorLabel->setText("Resistance value must be positive!");
			update();
		}
		else {
			applyHappened = true;
			oldResistanceValue = r->resistance();
			r->setResistance(newResistanceValue);
            r->update();
			this->close();
		}

	} else if(isDCVoltage) {
		applyHappened = true;
		oldVoltageValue = v->voltage();
		v->setVoltage(this->lineEdit->text().toDouble());
        v->update();
		this->close();
	}

	// For clock we have to check new value
	else if(isClock) {
		int newTimeIntervalValue = this->lineEdit->text().toInt();
		if(newTimeIntervalValue <= 0) {
			errorLabel->setText("Time interval value must be positive!");
			update();
		}
		else {
			applyHappened = true;
			oldTimeIntervalValue = cl->timeInterval();
			cl->setTimeInterval(newTimeIntervalValue);
			cl->update();
			this->close();
		}
	}
}

void Dialog::onCancelButtonInDialog() {
	// If apply button is not clicked before cancel then values on oldResistance and oldVoltage and oldTimeInterval are unknown!
	if(!applyHappened && isResistor)
		oldResistanceValue = r->resistance();
	else if(!applyHappened && isDCVoltage)
		oldVoltageValue = v->voltage();
	else if(!applyHappened && isClock)
		oldTimeIntervalValue = cl->timeInterval();

	// Either way on cancel we should set old value
	if(isResistor)
		r->setResistance(oldResistanceValue);
	else if(isDCVoltage)
		v->setVoltage(oldVoltageValue);
	else if(isClock)
		cl->setTimeInterval(oldTimeIntervalValue);

	this->close();
}
