#include "Calculator_gui.h"
 
Calculator_gui::Calculator_gui(QWidget* parent): QMainWindow(parent){
    ui.setupUi(this);
 
    connect(ui.pushButton_0, SIGNAL(clicked(bool)), this, SLOT(value_clicked()));
    connect(ui.pushButton_1, SIGNAL(clicked(bool)), this, SLOT(value_clicked()));
    connect(ui.pushButton_2, SIGNAL(clicked(bool)), this, SLOT(value_clicked()));
    connect(ui.pushButton_3, SIGNAL(clicked(bool)), this, SLOT(value_clicked()));
    connect(ui.pushButton_4, SIGNAL(clicked(bool)), this, SLOT(value_clicked()));
    connect(ui.pushButton_5, SIGNAL(clicked(bool)), this, SLOT(value_clicked()));
    connect(ui.pushButton_6, SIGNAL(clicked(bool)), this, SLOT(value_clicked()));
    connect(ui.pushButton_7, SIGNAL(clicked(bool)), this, SLOT(value_clicked()));
    connect(ui.pushButton_8, SIGNAL(clicked(bool)), this, SLOT(value_clicked()));
    connect(ui.pushButton_9, SIGNAL(clicked(bool)), this, SLOT(value_clicked()));
    connect(ui.pushButton_abs, SIGNAL(clicked(bool)), this, SLOT(value_clicked()));
    connect(ui.pushButton_ac, SIGNAL(clicked(bool)), this, SLOT(ac_clicked()));
    connect(ui.pushButton_add, SIGNAL(clicked(bool)), this, SLOT(value_clicked()));
    connect(ui.pushButton_ans, SIGNAL(clicked(bool)), this, SLOT(ans_clicked()));
    connect(ui.pushButton_c, SIGNAL(clicked(bool)), this, SLOT(value_clicked()));
    connect(ui.pushButton_del, SIGNAL(clicked(bool)), this, SLOT(del_clicked()));
    connect(ui.pushButton_div, SIGNAL(clicked(bool)), this, SLOT(value_clicked()));
    connect(ui.pushButton_e, SIGNAL(clicked(bool)), this, SLOT(value_clicked()));
    connect(ui.pushButton_equ, SIGNAL(clicked(bool)), this, SLOT(equal_clicked()));
    connect(ui.pushButton_fac, SIGNAL(clicked(bool)), this, SLOT(value_clicked()));  // 连接阶乘键
    connect(ui.pushButton_l, SIGNAL(clicked(bool)), this, SLOT(value_clicked()));  
    connect(ui.pushButton_lpar, SIGNAL(clicked(bool)), this, SLOT(value_clicked()));  // 连接左小括号键
    connect(ui.pushButton_lsqb, SIGNAL(clicked(bool)), this, SLOT(value_clicked()));  // 连接左方括号键
    connect(ui.pushButton_shift, SIGNAL(clicked(bool)), this, SLOT(shift_clicked()));
    connect(ui.pushButton_mod, SIGNAL(clicked(bool)), this, SLOT(value_clicked()));
    connect(ui.pushButton_mul, SIGNAL(clicked(bool)), this, SLOT(value_clicked()));
    connect(ui.pushButton_pi, SIGNAL(clicked(bool)), this, SLOT(value_clicked()));
    connect(ui.pushButton_point, SIGNAL(clicked(bool)), this, SLOT(value_clicked()));
    connect(ui.pushButton_power, SIGNAL(clicked(bool)), this, SLOT(value_clicked()));
    connect(ui.pushButton_rpar, SIGNAL(clicked(bool)), this, SLOT(value_clicked()));
    connect(ui.pushButton_rsqb, SIGNAL(clicked(bool)), this, SLOT(value_clicked()));
    connect(ui.pushButton_s, SIGNAL(clicked(bool)), this, SLOT(value_clicked()));
    connect(ui.pushButton_sqr, SIGNAL(clicked(bool)), this, SLOT(value_clicked()));
    connect(ui.pushButton_sub, SIGNAL(clicked(bool)), this, SLOT(value_clicked()));
    connect(ui.pushButton_t, SIGNAL(clicked(bool)), this, SLOT(value_clicked()));   
}
 
Calculator_gui::~Calculator_gui()
{}
 
void Calculator_gui::value_clicked() {
        QPushButton * button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QString text = button->text();  // 获取按钮上的文本（数字）  
        currentExpression.append(text);  // 将数字添加到表达式末尾  
        ui.text_input->setText(currentExpression);  // 更新显示  
    }
}
 
void Calculator_gui::equal_clicked() {
    cal.infix = currentExpression.toStdString();
    result = cal.calculate();
    ui.text_output->setText(QString::number(result));  // 显示结果
    currentExpression.clear();
}
 
void Calculator_gui::ac_clicked() { 
    currentExpression.clear();
    ui.text_input->setText(currentExpression);  // 更新显示
}
 
void Calculator_gui::del_clicked() {
    currentExpression.chop(1);
    ui.text_input->setText(currentExpression);  // 更新显示
}
 
void Calculator_gui::ans_clicked() {
    currentExpression.append(QString("%1").arg(result, 0, 'f', 4));
    ui.text_input->setText(currentExpression);  // 更新显示
}
 
void Calculator_gui::shift_clicked() {
    on_shift = !on_shift;  // 切换shift状态
    if (on_shift) {
        ui.pushButton_s->setText("arcsin");
        ui.pushButton_c->setText("arccos");
        ui.pushButton_t->setText("arctan");
    }
    else {
        ui.pushButton_s->setText("sin");
        ui.pushButton_c->setText("cos");
        ui.pushButton_t->setText("tan");
    }
}
