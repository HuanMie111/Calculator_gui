#pragma once
 
#include <QtWidgets/QMainWindow>
#include "ui_Calculator_gui.h"
#include "calculator.h"
 
class Calculator_gui : public QMainWindow
{
    Q_OBJECT
 
    public:
        Calculator_gui(QWidget *parent = nullptr);
        ~Calculator_gui();
 
    private slots:
        void value_clicked();  // 数字和操作符的槽函数
        void equal_clicked();  // 等号的槽函数
        void ac_clicked();  // ac 的槽函数
        void del_clicked();  // del 的槽函数
        void ans_clicked();  // ans 的槽函数
        void shift_clicked();  // shift 的槽函数
 
    private:
        Ui::Calculator_guiClass ui;
        Calculator cal;  // Calculator 子对象
        QString currentExpression;
        double result = 0;
        bool on_shift = false;  // shift 是否按下
};
