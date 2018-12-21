#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "components.hpp"
#include "log_component.hpp"
#include "scene.h"

#include <QMainWindow>
#include <QListWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QHBoxLayout>
#include <QKeyEvent>


class MainWindow : public QMainWindow
{
public:
    explicit MainWindow(QWidget *parent = nullptr);
protected:
    void keyPressEvent(QKeyEvent *);
private:
    QGraphicsView* view;
    QGraphicsScene* scene;
    QListWidget* itemListWidget;

    void createListWidget();
    void createSceneAndView();
    void createGraphicsItems();
    void createLayout();
};

#endif // MAINWINDOW_H
