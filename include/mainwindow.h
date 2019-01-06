#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//#include "components.hpp"
#include "log_component.hpp"
#include "scene.h" // for itemChange

#include <QMainWindow>
#include <QListWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QJsonDocument>

class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
	QLabel* propertiesMessage;
	static MainWindow* getMainWindow()
	{
		foreach (QWidget *w, qApp->topLevelWidgets())
			if (MainWindow* mainWin = dynamic_cast<MainWindow*>(w))
				return mainWin;
		return nullptr;
	}
	~MainWindow() override;

protected:
    void keyPressEvent(QKeyEvent *) override;

private slots:
	void onOpenFile();
	void onSaveFile();

private:
    QGraphicsView* view;
    QGraphicsScene* scene;
    QListWidget* itemListWidget;

	QPushButton *openFileButton;
	QPushButton *saveFileButton;
	QDialogButtonBox *buttonBox;

    void createListWidget();
    void createSceneAndView();
    void createLayout();

    void saveFile(QJsonDocument);
	QString currentFile;
};

#endif // MAINWINDOW_H
