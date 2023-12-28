//
// Created by Матвей Галимьянов on 14.12.2023.
//

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "gameServices.h"
#include <QMap>
#include <QPushButton>
#include <ui_mainwindow.h>
#include <QTimer>
#include <QContextMenuEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class mainwindow; }
QT_END_NAMESPACE

class mainwindow : public QMainWindow {
Q_OBJECT

public:
    mainwindow(QWidget *parent = nullptr, std::shared_ptr<GameService> const& game = nullptr);
    void drawMap();
    ~mainwindow() override;
private:
    bool isHightlited;
    QPair<size_t, size_t> highlightSquare;
    QString getSquareName(size_t x, size_t y);
    Ui::mainwindow *ui;
    std::shared_ptr<GameService> game_ = nullptr;
    QMap<QString, QPixmap> textures;
    QTimer *timer;
    MoveService * moveService_;
    AttackService * attackService_;
protected:
    void keyPressEvent(QKeyEvent *) override;
public slots:
    void cellSelected(int, int);
    void checkButtons();
};

#endif //MAINWINDOW_H
