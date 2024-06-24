#pragma once
// calculator.h: 头文件
#include <stack>
#include <vector>
#include <string>
 
 
//计算器类
class Calculator
{
public:
	Calculator();
	bool getFormat();					// 表达式判断合法性及自定义标准格式化
	int getPrior(char c);				// 获取算术符号优先级
	void getPostfix();					// 后缀表达式转换
	void calResult();					// 计算后缀表达式
	double calculate();					// 计算并返回结果
	void clearContainer();              // 清空用到的容器
 
	std::string operatorSym;			// 运算符号
	std::string infix;					// 表达式缓存
 
private:
	std::vector<std::string> postfix;	// 后缀表达式向量
	std::stack<char> symStack;			// 符号栈
	std::stack<double> figStack;		// 数字栈
	std::string stdInfix;				// 自定义标准格式化表达式
	double result;						// 最终计算结果
	std::string legalCharacter = "+-*/^lq%!|sctSCTep()[]{} 0123456789.";  // 合法字符
};
