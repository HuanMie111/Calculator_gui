// calculator.cpp: 实现文件
#define _USE_MATH_DEFINES
#include <cmath>
#include <stack>
#include <vector>
#include <map>
#include <cmath>
#include <string>
#include <algorithm>
#include <iostream>
#include <functional>
 
#include "calculator.h"
 
 
// 重要数学常量
const double pi = M_PI;  // 圆周率 pi
const double e = M_E;  // 自然常数 e
 
// 绝对值符号个数的奇偶性
enum ABS_ODEVITY {
	ABS_ODD = 1,
	ABS_EVEN = 2,
};
 
// 算术符号优先权等级
enum PRIO_LV {
	PRIO_LV0 = 0,
	PRIO_LV1 = 1,
	PRIO_LV2 = 2,
	PRIO_LV3 = 3,
	PRIO_LV4 = 4,
	PRIO_LV5 = 5,
	PRIO_LV6 = 6,
};
 
// 要替换的三角函数等长标识
std::map<std::string, std::string> tri_func_replace = { {"sin", "s"}, {"cos", "c" }, {"tan", "t" },
														{"arcsin", "S"}, {"arccos", "C"}, {"arctan", "T"},
														{"ln", "l"}, {"pi", "p"}, {"sqrt", "q"}
};
 
// 字符和要调用的函数名映射
std::map<std::string, std::function<double(double)>> c_func = {
		{"s", static_cast<double(*)(double)>(sin)},
		{"c", static_cast<double(*)(double)>(cos)},
		{"t", static_cast<double(*)(double)>(tan)},
		{"S", static_cast<double(*)(double)>(asin)},
		{"C", static_cast<double(*)(double)>(acos)},
		{"T", static_cast<double(*)(double)>(atan)},
		{"l", static_cast<double(*)(double)>(log)},
		{"q", static_cast<double(*)(double)>(sqrt)},
};
//std::map<std::string, std::function<double(double)>> c_func = { { "s", sin }, { "c", cos }, { "t", tan },
//																{ "S", asin }, { "C", acos }, { "T", atan }
//};
 
void replaceAll(std::string& str, const std::string& from, const std::string& to);
 
 
Calculator::Calculator() {				// 构造函数，初始化成员变量
 
	operatorSym = "支持运算符：+, -, *, /, %（取余）, ^（幂次方）, | |（绝对值）, !（阶乘）, sqrt（平方根）, ln（自然对数）, 括号 ( ) [ ] { }, 三角函数 sin cos tan, 反三角函数 arcsin arccos acrtan, 圆周率 pi, 自然常数 e";
	result = 0.0;
}
 
// 表达式判断合法性及自定义标准格式化
bool Calculator::getFormat() {
 
	stdInfix = infix;
 
	// 替换三角函数等长标识
	for (auto i = tri_func_replace.begin(); i != tri_func_replace.end(); i++) {
		replaceAll(stdInfix, i->first, i->second);
	}
 
	// for (int i = 0; i < stdInfix.length(); i++) {				// string 下标调用运算符时可能会导致类型溢出
	for (size_t i = 0; i < stdInfix.size(); i++) {					// string.size() 返回 size_type 类型，避免下标运算时的类型溢出
		if (legalCharacter.find(stdInfix[i]) == std::string::npos) {  // 判断字符合法性
			std::cout << "输入的表达式含非法字符 " << stdInfix[i] << "，返回默认结果 0。" << "\n";
			return false;
		}
		else if (stdInfix[i] == ' ') {  // 删除空格
			stdInfix.erase(i, 1);
		}
		else if (stdInfix[i] == '-' || stdInfix[i] == '+') {				// -x 转换为 0-x，+x 转化为 0+x
			if (i == 0)
				stdInfix.insert(0, 1, '0');
			else if (stdInfix[i - 1] == '(' || stdInfix[i - 1] == '[' || stdInfix[i - 1] == '{')
				stdInfix.insert(i, 1, '0');
			else if (stdInfix[i - 1] == '|' && count(stdInfix.begin(), stdInfix.begin() + i, '|') % 2)  // 根据之前的 | 符号个数的奇偶性判断是否添加 0
				stdInfix.insert(i, 1, '0');
		}
	}
	return true;
}
 
// 获取算术符号优先级
// 单目预算符优先级应高于双目运算符（大概）
int Calculator::getPrior(char c) {
 
	if (c == '+' || c == '-') {
		return PRIO_LV1;
	}
	else if (c == '*' || c == '/') {
		return PRIO_LV2;
	}
	else if (c == '%' || c == '^') {
		return PRIO_LV3;
	}
	else if (c == 's' || c == 'c' || c == 't' || c == 'S' || c == 'C' || c == 'T' || c == 'l' || c == 'q') {
		return PRIO_LV4;
	}
	else if (c == '!') {
		return PRIO_LV5;
	}
	else if (c == 'p' || c == 'e') {  // pi 和 e 也当作运算符处理，优先级最高
		return PRIO_LV6;
	}
	else {
		return PRIO_LV0;
	}
	// else { cout << c << " 非法符号！" << endl; }
}
 
// 后缀表达式转换
void Calculator::getPostfix() {
 
	int absNumeber = ABS_ODD;  // 绝对值符号个数的奇偶性
	std::string tmp;
 
	// for (int i = 0; i < stdInfix.length(); i++) {
	for (size_t i = 0; i < stdInfix.size(); i++) {  // string.size() 返回 size_type 类型，避免下标运算时的类型溢出
		tmp = "";
		switch (stdInfix[i]) {
		case '+':
		case '-':
		case '*':
		case '/':
		case '%':
		case '^':
		case 's':
		case 'c':
		case 't':
		case 'S':
		case 'C':
		case 'T':
		case 'l':
		case 'p':
		case 'e':
		case 'q':
		case '!':
			if (symStack.empty() || symStack.top() == '(' || symStack.top() == '[' || symStack.top() == '{' || (symStack.top() == '|' && absNumeber == ABS_ODD)) {
				symStack.push(stdInfix[i]);
			}
			else {
				while (!symStack.empty() && (getPrior(symStack.top()) >= getPrior(stdInfix[i]))) {
					tmp += symStack.top();
					postfix.push_back(tmp);
					symStack.pop();
					tmp = "";
				}
				symStack.push(stdInfix[i]);
			}
			break;
		case '|':
			if (absNumeber == ABS_ODD) {
				symStack.push(stdInfix[i]);
				absNumeber = ABS_EVEN;
			}
			else {
				while (!symStack.empty() && symStack.top() != '|') {
					tmp += symStack.top();
					postfix.push_back(tmp);
					symStack.pop();
					tmp = "";
				}
				if (!symStack.empty() && symStack.top() == '|') {
					tmp += symStack.top();
					postfix.push_back(tmp);						// 左绝对值符号 '|' 加入后缀表达式，用于绝对值的检测计算
					symStack.pop();
					absNumeber = ABS_ODD;
				}
			}
			break;
		case '(':
		case '[':
		case '{':
			symStack.push(stdInfix[i]);
			break;
		case ')':
			while (!symStack.empty() && symStack.top() != '(') {
				tmp += symStack.top();
				postfix.push_back(tmp);
				symStack.pop();
				tmp = "";
			}
			if (!symStack.empty() && symStack.top() == '(') {
				symStack.pop();							// 将左括号出栈丢弃
			}
			break;
		case ']':
			while (!symStack.empty() && symStack.top() != '[') {
				tmp += symStack.top();
				postfix.push_back(tmp);
				symStack.pop();
				tmp = "";
			}
			if (!symStack.empty() && symStack.top() == '[') {
				symStack.pop();							// 将左括号出栈丢弃
			}
			break;
		case '}':
			while (!symStack.empty() && symStack.top() != '{') {
				tmp += symStack.top();
				postfix.push_back(tmp);
				symStack.pop();
				tmp = "";
			}
			if (!symStack.empty() && symStack.top() == '{') {
				symStack.pop();							// 将左括号出栈丢弃
			}
			break;
		default:
			if ((stdInfix[i] >= '0' && stdInfix[i] <= '9')) {
				tmp += stdInfix[i];
				while (i + 1 < stdInfix.length() && (stdInfix[i + 1] >= '0' && stdInfix[i + 1] <= '9' || stdInfix[i + 1] == '.')) {	 // 小数处理
					tmp += stdInfix[i + 1];			// 是连续的数字，则追加
					i++;
				}
				if (tmp[tmp.length() - 1] == '.') {
					tmp += '0';						// 将 x. 做 x.0 处理
				}
				postfix.push_back(tmp);
			}
			break;
		}  // end switch
	}  // end for
 
	// if(!symStack.empty()) {
	while (!symStack.empty()) {						//将栈中剩余符号加入后缀表达式
		tmp = "";
		tmp += symStack.top();
		postfix.push_back(tmp);
		symStack.pop();
	}
}
 
// 计算后缀表达式
void Calculator::calResult() {
 
	std::string tmp;
	double number = 0;
	double op1 = 0, op2 = 0;
 
	for (int i = 0; i < postfix.size(); i++) {
		tmp = postfix[i];
		if (tmp[0] >= '0' && tmp[0] <= '9') {
			number = atof(tmp.c_str());
			figStack.push(number);
		}
		else if (postfix[i] == "+") {
			if (!figStack.empty()) {
				op2 = figStack.top();
				figStack.pop();
			}
			if (!figStack.empty()) {
				op1 = figStack.top();
				figStack.pop();
			}
			figStack.push(op1 + op2);
		}
		else if (postfix[i] == "-") {
			if (!figStack.empty()) {
				op2 = figStack.top();
				figStack.pop();
			}
			if (!figStack.empty()) {
				op1 = figStack.top();
				figStack.pop();
			}
			figStack.push(op1 - op2);
		}
		else if (postfix[i] == "*") {
			if (!figStack.empty()) {
				op2 = figStack.top();
				figStack.pop();
			}
			if (!figStack.empty()) {
				op1 = figStack.top();
				figStack.pop();
			}
			figStack.push(op1 * op2);
		}
		else if (postfix[i] == "/") {
			if (!figStack.empty()) {
				op2 = figStack.top();
				figStack.pop();
			}
			if (!figStack.empty()) {
				op1 = figStack.top();
				figStack.pop();
			}
			if (op2 != 0) {
				// 除数不为0，未做处理，默认
			}
			figStack.push(op1 / op2);
		}
		else if (postfix[i] == "%") {
			if (!figStack.empty()) {
				op2 = figStack.top();
				figStack.pop();
			}
			if (!figStack.empty()) {
				op1 = figStack.top();
				figStack.pop();
			}
			figStack.push(fmod(op1, op2));	// 可进行小数求余
		}
		else if (postfix[i] == "^") {
			if (!figStack.empty()) {
				op2 = figStack.top();
				figStack.pop();
			}
			if (!figStack.empty()) {
				op1 = figStack.top();
				figStack.pop();
			}
			figStack.push(pow(op1, op2));
		}
		else if (postfix[i] == "|") {
			if (!figStack.empty()) {
				op1 = figStack.top();
				figStack.pop();
			}
			figStack.push(abs(op1));
		}
		else if (postfix[i] == "!") {
			if (!figStack.empty()) {
				op1 = figStack.top();
				figStack.pop();
			}
			if (op1 > 0) {
				// 阶乘数应大于 0；阶乘数为小数时，求其向下取整的数的阶
				double factorial = 1;
				for (int i = 1; i <= op1; ++i)
				{
					factorial *= i;
				}
				op1 = factorial;
			}
			figStack.push(op1);
		}
		else if (postfix[i] == "s" || postfix[i] == "c" ||
			postfix[i] == "t" || postfix[i] == "S" ||
			postfix[i] == "C" || postfix[i] == "T" ||
			postfix[i] == "l" || postfix[i] == "q") {
			if (!figStack.empty()) {
				op1 = figStack.top();
				figStack.pop();
			}
			figStack.push(c_func[postfix[i]](op1));
		}
		else if (postfix[i] == "p") {
			figStack.push(pi);
		}
		else if (postfix[i] == "e") {
			figStack.push(e);
		}
		else
			std::cout << "后缀表达式中该操作符" << postfix[i] << "未编写处理情况" << "\n";
	}
 
	if (!figStack.empty()) {
		result = figStack.top();
	}
	else
		std::cout << "结果未正确计算" << "\n";
 
}
 
// 单个计算结束，清空用到的容器
void Calculator::clearContainer() {
	std::stack<double>().swap(figStack);  // 和空栈交换，实现清空栈效果
	std::stack<char>().swap(symStack);
	postfix.clear();
}
 
// 计算并返回结果
double Calculator::calculate() {
 
	if (getFormat()) {				// 表达式判断合法性及自定义标准格式化
		getPostfix();				// 后缀表达式转换
		calResult();				// 获取算术结果
		clearContainer();           // 清空每次计算用到的容器
		return result;
	}
	return 0;
}
 
 
void replaceAll(std::string& str, const std::string& from, const std::string& to) {
	// 把 string 中所有的 from 替换为 to
	size_t pos = 0;
	while ((pos = str.find(from, pos)) != std::string::npos) {
		str.replace(pos, from.length(), to);
		pos += to.length();  // 更新起始位置，避免重复替换  
	}
}
