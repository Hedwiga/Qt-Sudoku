/*
Filename:sudoku.cpp
*/
#include "sudoku.h"
#include "game.h"
#include "grid.h"
#include "ui_sudoku.h"
#include <iostream>
#include <fstream>
#include <string>
#include <QMessageBox>
#include <QTimer>
#include <QTime>
Sudoku::Sudoku(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Sudoku)
{
    ui->setupUi(this);

    currentRow = currentCol = UNDEFINED;
    timer = new QTimer(this);

    timer->setInterval(1000);
    QTime timeTemp(0,10,0);
    currentTimer = timeTemp;

    ui->time->setText(currentTimer.toString("m:ss"));


    for(int i = 1; i <= LEVELS; ++i)
    {
        QString levelName = "level" + QString::number(i);
        connect(findChild<QPushButton *>(levelName), SIGNAL(released()), this, SLOT(createSudoku()));
    }

    for(int row = 0; row  < SIZE_D; ++row )
        for(int col = 0; col < SIZE_D; ++col)
        {
            // numbering in the name of box starts from 1
            QString boxName = "box" + QString::number(row + 1) + "_" + QString::number(col + 1);
            QPushButton * box = findChild<QPushButton *>(boxName);
            connect(box, SIGNAL(released()), this, SLOT(clickedBox()));
        }

}

Sudoku::~Sudoku()
{
    delete ui;
}
void Sudoku::on_quit_clicked()
{
    close();
}
bool Sudoku::loadGrid()
{
    using namespace std;
    string path = "C:\\Users\\Maria\\University\\First_year\\Second_half\\OOP\\Labs\\Lab_OOP_4_new\\Sudoku";
    switch(level)
    {
    case Easy:
        path = path + "\\easy";
        break;
    case Medium:
        path = path + "\\medium";
        break;
    case Hard:
        path = path + "\\hard";
        break;
    }

    path = path + "\\sudoku_" +  to_string(0) + "\\sudoku.txt";
    ifstream file;
    file.open(path.c_str());
    if(!file.is_open())
        return false;
    for(int i = 0; i < SIZE_D; ++i)
        for(int j = 0; j < SIZE_D; ++j)
            file >> usersGrid[i][j];
    file.close();
    return true;
}
void Sudoku::createSudoku()
{
    QPushButton * currentLevel = (QPushButton *)sender();
    setLevel(currentLevel);
    if(!loadGrid())
        return;
    fillGrid();
    setTime();
}
void Sudoku::setLevel(QPushButton * currentLevel)
{
    if(currentLevel == ui->level1)
    {
        level = Easy;
        ui->level->setText("Easy");
    }
    else if(currentLevel == ui->level2)
    {
        level = Medium;
        ui->level->setText("Medium");
    }
    else if(currentLevel == ui->level3)
    {
        level = Hard;
        ui->level->setText("Hard");
    }
    ui->level1->setEnabled(false);
    ui->level2->setEnabled(false);
    ui->level3->setEnabled(false);


}
void Sudoku::setTime()
{
    connect(timer, SIGNAL(timeout()), this, SLOT(timeStart()));
    timer->start();
}

void Sudoku::timeStart()
{
    if(currentTimer.minute() == 0 && currentTimer.second() == 0)
    {
        timer->stop();
        lose();
    }
    else
    {
        currentTimer = currentTimer.addSecs(-1);
        ui->time->setText(currentTimer.toString("m:ss"));
    }
}
void Sudoku::fillGrid()
{
    for(int row = 0; row < SIZE_D; ++row)
        for(int col = 0; col < SIZE_D; ++col)
        {
            QString boxName = "box" + QString::number(row + 1) + "_" + QString::number(col + 1);
            QPushButton * box = findChild<QPushButton *>(boxName);
            if(usersGrid[row][col])
            {
                box->setStyleSheet("background-color: rgb(255, 170, 127);"
                                   "font: 12pt \"Comic Sans MS\"; ");
                box->setText(QString::number(usersGrid[row][col]));
            }
            else
                box->setEnabled(true);
        }
}
void Sudoku::clickedBox()
{
    if(currentRow != UNDEFINED || currentCol != UNDEFINED)
        clearBox();
    ui->clear->setEnabled(true);
    QPushButton* box = (QPushButton *)sender();
    setRowAndCol(box);
    box->setStyleSheet("background-color:rgb(170, 255, 127);"
                           "font: 12pt \"Comic Sans MS\"; ");
    showAvailableNumbers();
    for(int num = 1; num <= SIZE_D; ++num)
    {
        QString boxName = "num_" + QString::number(num);
        QPushButton * numButton = findChild<QPushButton *>(boxName);
        if(numButton->isEnabled())
            connect(numButton, SIGNAL(released()), this, SLOT(fillBox()));
    }
    if(usersGrid[currentRow][currentRow] == EMPTY)
        setStyleSheet("background-color:rgb(227, 227, 227);"
                      "font: 12pt \"Comic Sans MS\"; ");

}
void Sudoku::setRowAndCol(QPushButton * box)
{
    QString boxName = box->objectName();
    bool needCol = false;
    foreach(QChar ch, boxName)
    {
        if(ch.isNumber() && needCol)
        {
            currentCol = ch.digitValue() - 1;
            break;
        }
        if(ch.isNumber())
        {
            currentRow = ch.digitValue() - 1;
            needCol = true;
        }
        // currentCol and currentRow are substructed by 1, because indexation of boxes start from 1,
        // when indexation of matrix itself is typically starts from 0

    }
}

void Sudoku::showAvailableNumbers() const
{
    ui->available->setText("Available numbers:");
    for(int num = 1; num <= SIZE_D; ++num)
    {
        QString boxName = "num_" + QString::number(num);
        QPushButton * numButton = findChild<QPushButton *>(boxName);
        if(boxIsSafe(num) && currentRow != UNDEFINED && currentCol != UNDEFINED)
            enableButton(numButton);
        else
            disableButton(numButton);
    }
}
void Sudoku::enableButton(QPushButton * button) const
{
    button->setStyleSheet("font: 12pt \"Comic Sans MS\";"
                             "background-color: rgb(255, 255, 127);");
    button->setEnabled(true);
}



void Sudoku::assignRowAndCol(int row, int col)
{
    currentRow = row;
    currentCol = col;
}


void Sudoku::disableButton(QPushButton * button) const
{
    button->setStyleSheet("font: 12pt \"Comic Sans MS\";"
                             "background-color: rgb(227, 227, 255);");
    button->setEnabled(false);
}

void Sudoku::fillBox()
{
    if(currentRow == UNDEFINED && currentCol == UNDEFINED)
        return;
    QPushButton * numButton = (QPushButton *)sender();
    usersGrid[currentRow][currentCol] = getNum(numButton);
    QString boxName = "box" + QString::number(currentRow + 1) + "_" + QString::number(currentCol + 1);
    QPushButton * box = findChild<QPushButton *>(boxName);
    box->setStyleSheet("font: 12pt \"Comic Sans MS\";"
                             "background-color: rgb(170, 255, 255);");
    box->setText(QString::number(usersGrid[currentRow][currentCol]));
    ui->clear->setEnabled(false);
    clearNumbers();
    assignRowAndCol(UNDEFINED, UNDEFINED);
    if(findEmptyBox())
        return;
    else
        win();

}
int Sudoku::getNum(QPushButton * numButton)
{
    QString buttonName = numButton->objectName();
    foreach(QChar ch, buttonName)
    {
        if(ch.isNumber())
            return ch.digitValue();
    }
    return 0;
}


void Sudoku::defineLimitsForBlock(int &limitRow, int &limitCol) const
{
    if(currentRow < SIZE_)
        limitRow = SIZE_;
    else if(currentRow < SIZE_ * 2)
        limitRow = SIZE_ * 2;
    else
        limitRow = SIZE_ * 3;

    if(currentCol < SIZE_)
        limitCol = SIZE_;
    else if(currentCol < SIZE_ * 2)
        limitCol = SIZE_ * 2;
    else
        limitCol = SIZE_ * 3;
}

bool Sudoku::findEmptyBox() const
{
    for(int row = 0; row < SIZE_D; ++row)
        for(int col = 0; col < SIZE_D; ++col)
            if(usersGrid[row][col] == EMPTY)
                return true;
    return false;
}

bool Sudoku::rowIsSafe(int num) const
{
    for(int col = 0; col < SIZE_D; ++col)
        if(usersGrid[currentRow][col] == num ) return false;
    return true;
}

bool Sudoku::colIsSafe(int num) const
{
    for(int row = 0; row < SIZE_D; ++row)
        if(usersGrid[row][currentCol] == num ) return false;
    return true;
}

bool Sudoku::blockIsSafe(int num) const
{
    int limitRow = 0, limitCol = 0;
    defineLimitsForBlock(limitRow, limitCol);
    for(int row = limitRow - SIZE_; row < limitRow; ++row)
        for(int col = limitCol - SIZE_; col < limitCol; ++col)
            if(usersGrid[row][col] == num)
                return false;
    return true;
}

bool Sudoku::boxIsSafe(int num) const
{
    return
        rowIsSafe(num) &&
        colIsSafe(num) &&
        blockIsSafe(num);
}


void Sudoku::clearNumbers()
{
    for(int num = 1; num <= SIZE_D; ++num)
    {
        QString numName = "num_" + QString::number(num);
        QPushButton * numButton = findChild<QPushButton *>(numName);
        disableButton(numButton);
    }
}

void Sudoku::win()
{
    for(int row = 0; row  < SIZE_D; ++row )
        for(int col = 0; col < SIZE_D; ++col)
        {
            // numbering in the name of box starts from 1
            QString boxName = "box" + QString::number(row + 1) + "_" + QString::number(col + 1);
            QPushButton * box = findChild<QPushButton *>(boxName);
            box->setStyleSheet("font: 12pt \"Comic Sans MS\";"
                            "background-color: rgb(85, 0, 127);"
                            "color: rgb(255, 255, 255);");
            box->setEnabled(false);
        }
    ui->available->setText("CONGRATULATIONS! YOU WON!");
}

void Sudoku::lose()
{
    for(int row = 0; row  < SIZE_D; ++row )
        for(int col = 0; col < SIZE_D; ++col)
        {
            // numbering in the name of box starts from 1
            QString boxName = "box" + QString::number(row + 1) + "_" + QString::number(col + 1);
            QPushButton * box = findChild<QPushButton *>(boxName);
            box->setEnabled(false);
        }
    for(int num = 1; num <= SIZE_D; ++num)
    {
        QString boxName = "num_" + QString::number(num);
        QPushButton * numButton = findChild<QPushButton *>(boxName);
        numButton->setEnabled(false);
    }
    ui->available->setText("OH NO! YOU LOST");
}


void Sudoku::clearBox()
{
    usersGrid[currentRow][currentCol] = EMPTY;
    QString boxName = "box" + QString::number(currentRow + 1) + "_" + QString::number(currentCol + 1);
    QPushButton * box = findChild<QPushButton *>(boxName);
    box->setStyleSheet("font: 12pt \"Comic Sans MS\";"
                       "background-color: rgb(227, 227, 227);");
    box->setText(" ");
    assignRowAndCol(UNDEFINED, UNDEFINED);
}
void Sudoku::on_clear_clicked()
{
    if(currentRow == UNDEFINED || currentCol == UNDEFINED)
        return;
    clearNumbers();
    clearBox();
    ui->clear->setEnabled(false);
}
