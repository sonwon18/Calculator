#include<iostream>
#include "calculator.cpp""

int main() {
	string str;
	std::cout << "중위연산 입력(100글자 미만) : ";
	std::cin >> str;
	MainCalculator* cal = new MainCalculator();
	try {
		cal->Convert(str);
		int result = cal->Result();
		std::cout << "계산결과 : " << result << std::endl;
	}
	catch (string ex) {
		std::cout << "오류발생! : " << ex << std::endl;
	}
}