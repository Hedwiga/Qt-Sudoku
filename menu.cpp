/*
Filename:menu.cpp
*/
#include "menu.h"
#include "game.h"
#include "ui_menu.h"
#include "sudoku.h"
Menu::Menu(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Menu)
{

    ui->setupUi(this);

}

Menu::~Menu()
{
    delete ui;
}

void Menu::on_quit_clicked()
{
    close();
}

void Menu::on_pushButton_clicked()
{
    Sudoku newGame;
    newGame.setModal(true);
    newGame.exec();
}
