//
// Created by Матвей Галимьянов on 14.12.2023.
//

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "gameServices.h"
#include <QMap>

QT_BEGIN_NAMESPACE
namespace Ui { class mainwindow; }
QT_END_NAMESPACE

class mainwindow : public QMainWindow {
Q_OBJECT

public:
    mainwindow(QWidget *parent = nullptr, GameService * game = nullptr);
    void drawMap();
    ~mainwindow() override;
private:
    Ui::mainwindow *ui;
    GameService * game_ = nullptr;
    QMap<QString, QPixmap> textures;
private slots:

};


#endif //MAINWINDOW_H
