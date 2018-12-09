#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "component.h"
#include "logcomponent.h"
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
