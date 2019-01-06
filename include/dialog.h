#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QKeyEvent>
#include "mainwindow.h"
#include "components.hpp"

// Dialog opens on right click for resistance and dc voltage
class Dialog: public QDialog {
    Q_OBJECT

public:
	explicit Dialog(Component* component = nullptr, QWidget* parent = nullptr);
	~Dialog() override;

protected:
	void keyPressEvent(QKeyEvent *) override;

private slots:
	void onCancelButtonInDialog();
	void onOkButtonInDialog();

private:
	Component* component;
	QLabel *nameLabel;
	QLabel *errorLabel;
    QLineEdit *lineEdit;
	QPushButton *cancelButton;
	QPushButton *okButton;
    QDialogButtonBox *buttonBox;

	double oldResistanceValue;
	double oldVoltageValue;
	int oldTimeIntervalValue;

	Resistor* r;
	DCVoltage* v;
	Clock* cl;

	bool applyHappened = false;
	bool isResistor = false;
	bool isDCVoltage = false;
	bool isClock = false;

	bool isSaveFile = false;
};

#endif // DIALOG_H
