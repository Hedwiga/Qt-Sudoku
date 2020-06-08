/*
Filename:sudoku.h
*/
#ifndef SUDOKU_H
#define SUDOKU_H
#include "menu.h"
#include "game.h"
#include "grid.h"
#include <QDialog>
#include <QMap>
#include <QTime>
namespace Ui {
class Sudoku;
}

class Sudoku : public QDialog
{
    Q_OBJECT

    Ui::Sudoku *ui;
    Levels level;
    int currentRow;
    int currentCol;
    int usersGrid[SIZE_D][SIZE_D];
    QTimer * timer;
    QTime currentTimer;

    void setRowAndCol(QPushButton *);
    int getNum(QPushButton *);
    void setLevel(QPushButton *);
    void defineLimitsForBlock(int & limitRow, int & limitCol) const;
    bool findEmptyBox() const;
    bool rowIsSafe(int num) const;
    bool colIsSafe(int num) const;
    bool blockIsSafe(int num) const;
    bool boxIsSafe(int num) const;
    void showAvailableNumbers() const;
    void disableButton(QPushButton *) const;
    void enableButton(QPushButton *) const;
    void enableAllButtons() const;
    void assignRowAndCol(int row, int col);
    void clearBox();
    void clearNumbers();
    void win();
    void lose();
    void setTime();

private slots:
    void timeStart();
    void fillBox();
    void createSudoku();
    void fillGrid();
    void on_quit_clicked();
    void clickedBox();


//    bool findEmptyBox();
    void on_clear_clicked();

public:
    explicit Sudoku(QWidget *parent = nullptr);
    bool loadGrid();
    ~Sudoku();



};

#endif // SUDOKU_H
