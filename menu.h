/*
Filename:menu.h
*/
#ifndef MENU_H
#define MENU_H
#include "sudoku.h"
#include "game.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class Menu; }
QT_END_NAMESPACE

class Menu : public QMainWindow
{
    Q_OBJECT
    Ui::Menu *ui;

private slots:

    void on_quit_clicked();
    void on_pushButton_clicked();

public:
    explicit Menu(QWidget *parent = nullptr);
    ~Menu();

};
#endif // MENU_H
