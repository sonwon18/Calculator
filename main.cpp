#include<iostream>
#include "calculator.cpp""

int main() {
	string str;
	std::cout << "�������� �Է�(100���� �̸�) : ";
	std::cin >> str;
	MainCalculator* cal = new MainCalculator();
	try {
		cal->Convert(str);
		int result = cal->Result();
		std::cout << "����� : " << result << std::endl;
	}
	catch (string ex) {
		std::cout << "�����߻�! : " << ex << std::endl;
	}
}