#include "mainwindow.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QPainter>
#include <QDesktopServices>

mainwindow::mainwindow(QWidget *parent, std::shared_ptr<GameService> const& game) :
    QMainWindow(parent), ui(new Ui::mainwindow) {
    this->setWindowTitle("Cops and monsters");

    highlightSquare = {0, 0};
    isHightlited = false;

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

    QString basePath = "/Users/matusha/Desktop/ЯП/C++/Лабораторные/Семестр_3/lab3Game/textures/";

    textures.insert("floor", QPixmap(basePath + "floor.png").scaled(w, h, Qt::KeepAspectRatio));
    textures.insert("wall", QPixmap(basePath + "wall.png").scaled(w, h, Qt::KeepAspectRatio));
    textures.insert("window", QPixmap(basePath + "window.png").scaled(w, h, Qt::KeepAspectRatio));
    textures.insert("storage", QPixmap(basePath + "storage.png").scaled(w, h, Qt::KeepAspectRatio));
    textures.insert("noType", QPixmap(basePath + "noType.png").scaled(w, h, Qt::KeepAspectRatio));
    textures.insert("barrier", QPixmap(basePath + "barrier.png").scaled(w, h, Qt::KeepAspectRatio));
    textures.insert("pointer_up", QPixmap(basePath + "pointer_up.png").scaled(w, h, Qt::KeepAspectRatio));
    textures.insert("pointer_down", QPixmap(basePath + "pointer_down.png").scaled(w, h, Qt::KeepAspectRatio));
    textures.insert("pointer_left", QPixmap(basePath + "pointer_left.png").scaled(w, h, Qt::KeepAspectRatio));
    textures.insert("pointer_right", QPixmap(basePath + "pointer_right.png").scaled(w, h, Qt::KeepAspectRatio));
    textures.insert("operative", QPixmap(basePath + "operative.png").scaled(w, h, Qt::KeepAspectRatio));
    textures.insert("smart_entity", QPixmap(basePath + "smart_entity.png").scaled(w, h, Qt::KeepAspectRatio));
    textures.insert("highlight", QPixmap(basePath + "highlight.png").scaled(w, h, Qt::KeepAspectRatio));
    textures.insert("wild_entity", QPixmap(basePath + "wild_entity.png").scaled(w, h, Qt::KeepAspectRatio));

    moveService_ = new MoveService(game);
    attackService_ = new AttackService(game);
    timer = new QTimer();
    //connect(timer, SIGNAL(timeout()), this, SLOT(drawMap()));
    timer->start(1000);
    connect(ui->tableWidget, SIGNAL(cellClicked(int, int)), this, SLOT(cellSelected(int, int)));
    ui->frame->setVisible(false);
    drawMap();
}

QString mainwindow::getSquareName(size_t x, size_t y) {
    QString squareTypeName;
    switch (game_->getLevel().getGameField()[x][y]->getSquareType()) {
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
    return squareTypeName;
}

QString getEntityType(Entity * entity) {
    if(dynamic_cast<Operative*>(entity)) {return "operative"; }
    if(dynamic_cast<SmartEntity*>(entity)) {return "smart_entity"; }
    if(dynamic_cast<wildEntity*>(entity)) {return "wild_entity"; }
    return "";
}


void mainwindow::drawMap() {
    for (int i = 0; i < game_->getLevel().size().first; i++) {
        for (int j = 0; j < game_->getLevel().size().second; j++) {
            QString squareTypeName = getSquareName(i, j);
            auto tbl = new QTableWidgetItem();
            QPixmap n(textures[squareTypeName]);
            auto painter = new QPainter(&n);
            painter->drawPixmap(QPoint(), n);
            Entity * entity =  game_->getLevel().getGameField()[i][j]->getEntity();
            if(entity) {
                painter->drawPixmap(QPoint(), textures[getEntityType(entity)]);
            }
            if(i == highlightSquare.first && j == highlightSquare.second && isHightlited) {
                if(entity) { isHightlited = true; painter->drawPixmap(QPoint(), textures["highlight"]); }
                else {isHightlited = false;}
            }
                painter->end();
                tbl->setData(Qt::BackgroundRole, n);
                ui->tableWidget->setItem(i, j, tbl);
                ui->tableWidget->show();
        }
    }
    if(isHightlited) {
        Entity * entity = game_->getLevel().getGameField()[highlightSquare.first][highlightSquare.second]->getEntity();
        ui->label_3->setText(QString::fromStdString(entity->getName()));

        ui->progressBar->setMinimum(0);
        ui->progressBar->setMaximum(entity->getMaxHealth());
        ui->progressBar->setValue(entity->getCurrentHealth());

        ui->progressBar_2->setMinimum(0);
        ui->progressBar_2->setMaximum(entity->getMaxTime());
        ui->progressBar_2->setValue(entity->getCurrentTime());

        ui->frame->setVisible(true);
    } else {ui->frame->setVisible(false);}
}

void mainwindow::cellSelected(int x, int y) {
    Entity * entity = game_->getLevel().getGameField()[x][y]->getEntity();
    if(dynamic_cast<Operative*>(entity)) {
        isHightlited = true;
        highlightSquare = {x, y};
    } else {
        isHightlited = false;
    }
    drawMap();
}

void mainwindow::keyPressEvent(QKeyEvent * button) {
    QMap<Qt::Key, Directions> dirsAndButtons = {
        {Qt::Key_W, Directions::north},
        {Qt::Key_S, Directions::south},
        {Qt::Key_A, Directions::west},
        {Qt::Key_D, Directions::east}
    };
    QMap<Qt::Key, Directions> attackButtons = {
        {Qt::Key_Up, Directions::north},
        {Qt::Key_Down, Directions::south},
        {Qt::Key_Left, Directions::west},
        {Qt::Key_Right, Directions::east}
    };
    if(isHightlited) {
        if(dirsAndButtons.count(static_cast<Qt::Key>(button->key()))) {
            try {
                Entity *entity = game_->getLevel().getGameField()[highlightSquare.first][highlightSquare.second]->getEntity();
                moveService_->move(entity, dirsAndButtons[static_cast<Qt::Key>(button->key())]);
                std::cout << entity->getCurrentTime() << std::endl;
                highlightSquare = findPos(entity, game_->getLevel().getGameField());

            } catch (std::exception const& ) { return; }
        }
        if(attackButtons.count(static_cast<Qt::Key>(button->key()))) {
            try {
                Entity *entity = game_->getLevel().getGameField()[highlightSquare.first][highlightSquare.second]->getEntity();
                attackService_->attack(entity, attackButtons[static_cast<Qt::Key>(button->key())]);
                highlightSquare = findPos(entity, game_->getLevel().getGameField());
            } catch (std::exception const& ) { return; }
        }
    }

    drawMap();
}


void mainwindow::checkButtons() {

}

mainwindow::~mainwindow() {
    delete ui;
}
