#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QDialogButtonBox>

class Dialog: public QDialog {
    Q_OBJECT
public:
    Dialog(QWidget* parent = 0);
    ~Dialog() {}
private:
    QLabel *label;
    QLineEdit *lineEdit;
    QPushButton *applyButton;
    QPushButton *cancelButton;
    QPushButton *saveButton;
    QDialogButtonBox *buttonBox;
};

#endif // DIALOG_H
