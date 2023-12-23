#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QDesktopServices>

mainwindow::mainwindow(QWidget *parent, GameService * game) :
    QMainWindow(parent), ui(new Ui::mainwindow) {
    this->setWindowTitle("Cops and monsters");

    ui->setupUi(this);
    game_ = game;

    int w = 32;
    int h = 32;

    ui->tableWidget->setRowCount(game->getLevel().size().first);
    ui->tableWidget->setColumnCount(game->getLevel().size().second);
    ui->tableWidget->resize(w * game->getLevel().size().second,h * game->getLevel().size().first);

    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setFocusPolicy(Qt::NoFocus);
    ui->tableWidget->setSelectionMode(QAbstractItemView::NoSelection);

    textures.insert("floor", QPixmap("/Users/matusha/Desktop/ЯП/C++/Лабораторные/Семестр_3/lab3Game/textures/floor.png").
        scaled(w, h, Qt::KeepAspectRatio));
    textures.insert("wall", QPixmap("/Users/matusha/Desktop/ЯП/C++/Лабораторные/Семестр_3/lab3Game/textures/wall.png").
        scaled(w, h, Qt::KeepAspectRatio));
    textures.insert("window", QPixmap("/Users/matusha/Desktop/ЯП/C++/Лабораторные/Семестр_3/lab3Game/textures/window.png").
        scaled(w, h, Qt::KeepAspectRatio));
    textures.insert("storage", QPixmap("/Users/matusha/Desktop/ЯП/C++/Лабораторные/Семестр_3/lab3Game/textures/storage.png").
        scaled(w, h, Qt::KeepAspectRatio));
    textures.insert("noType", QPixmap("/Users/matusha/Desktop/ЯП/C++/Лабораторные/Семестр_3/lab3Game/textures/noType.png").
        scaled(w, h, Qt::KeepAspectRatio));
    textures.insert("barrier", QPixmap("/Users/matusha/Desktop/ЯП/C++/Лабораторные/Семестр_3/lab3Game/textures/barrier.png").
        scaled(w, h, Qt::KeepAspectRatio));

    drawMap();
}

void mainwindow::drawMap() {
    for (int i = 0; i < game_->getLevel().size().first; i++) {
        for (int j = 0; j < game_->getLevel().size().second; j++) {
            QString squareTypeName;
                switch (game_->getLevel().getGameField()[i][j]->getSquareType()) {
                    case SquareType::Wall : {
                        squareTypeName = "wall";
                        break;
                    }
                    case SquareType::Floor : {
                        squareTypeName = "floor";
                        break;
                    }
                    case SquareType::Storage : {
                        squareTypeName = "storage";
                        break;
                    }
                    case SquareType::Window : {
                        squareTypeName = "window";
                        break;
                    }
                    case SquareType::Barrier : {
                        squareTypeName = "barrier";
                        break;
                    }
                    case SquareType::NoType : {
                        squareTypeName = "noType";
                        break;
                    }
                }

            auto tbl = new QTableWidgetItem();
            tbl->setData(Qt::BackgroundRole, textures[squareTypeName]);
            ui->tableWidget->setItem(i, j, tbl);
        }
    }
    ui->tableWidget->show();
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(game_->getLevel().getGameField()[6][5]->getEntity()->getMaxTime());
    ui->progressBar->setValue(game_->getLevel().getGameField()[6][5]->getEntity()->getCurrentTime());


}

mainwindow::~mainwindow() {
    delete ui;
}
