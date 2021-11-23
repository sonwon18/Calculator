#include<exception>
#include<string>
#include<stack>
#define _CRT_SEQURE_WARNINGS
using std::stack;
using std::string;
using std::exception;

static const string infixException[] = { "MissingLeftParen", "MissingRightParen", "UnknownOperator", "TooLongCalculation" };
static const string postfixException[] = { "LackOfValues", "TooManyValues", "DividedByZero",
"UnknownOperator", "WrongSixteen_Bit", "WrongTwo_Bit" };

class Converter {
public:
	virtual int sixteen_bit_Convert(char* str) = 0;
	virtual int two_bit_Convert(char* str) = 0;
};
class Sixteen_Bit_Converter : public Converter {
public:
	int sixteen_bit_Convert(char* str) override {
		if (str[0] != '0') {
			throw postfixException[4];
		}
		int value = 0;
		int i = strlen(str) - 1;
		for (i; i > 1; i--) {
			switch (str[i]) {
			case 'f':
				value += pow(16, (strlen(str) - 1 - i)) * 15;
				break;
			case 'e':
				value += pow(16, (strlen(str) - 1 - i)) * 14;
				break;
			case 'd':
				value += pow(16, (strlen(str) - 1 - i)) * 13;
				break;
			case 'c':
				value += pow(16, (strlen(str) - 1 - i)) * 12;
				break;
			case 'b':
				value += pow(16, (strlen(str) - 1 - i)) * 11;
				break;
			case 'a':
				value += pow(16, (strlen(str) - 1 - i)) * 10;
				break;
			case '9':
				value += pow(16, (strlen(str) - 1 - i)) * 9;
				break;
			case '8':
				value += pow(16, (strlen(str) - 1 - i)) * 8;
				break;
			case '7':
				value += pow(16, (strlen(str) - 1 - i)) * 7;
				break;
			case '6':
				value += pow(16, (strlen(str) - 1 - i)) * 6;
				break;
			case '5':
				value += pow(16, (strlen(str) - 1 - i)) * 5;
				break;
			case '4':
				value += pow(16, (strlen(str) - 1 - i)) * 4;
				break;
			case '3':
				value += pow(16, (strlen(str) - 1 - i)) * 3;
				break;
			case '2':
				value += pow(16, (strlen(str) - 1 - i)) * 2;
				break;
			case '1':
				value += pow(16, (strlen(str) - 1 - i)) * 1;
				break;
			case '0':
				break;
			default:
				throw postfixException[4];
			}
		}
		return value;
	}
	int two_bit_Convert(char* str) override {
		return 0;
	}
};
class Two_Bit_Converter : public Converter {
public:
	int sixteen_bit_Convert(char* str) override {
		return 0;
	}
	int two_bit_Convert(char* str) override {
		if (str[0] != '0') {
			throw postfixException[5];
		}
		int value = 0;
		int i = strlen(str) - 1;
		for (i; i > 1; i--) {
			switch (str[i]) {
			case '1':
				value += pow(2, (strlen(str) - 1 - i));
				break;
			case '0':
				break;
			default:
				throw postfixException[5];
			}
		}
		return value;
	}
};
class CalculatorAdapter {
public:
	Converter* converter = NULL;

	CalculatorAdapter(string calcType) {
		if (!calcType.compare("sixteen")) {
			converter = new Sixteen_Bit_Converter();
		}
		else if (!calcType.compare("two")) {
			converter = new Two_Bit_Converter();
		}
	}
	int convert(string calcType, char* str) {
		if (!calcType.compare("sixteen")) {
			return converter->sixteen_bit_Convert(str);
		}
		else if (!calcType.compare("two")) {
			return converter->two_bit_Convert(str);
		}
	}
};


class MainCalculator {
private:
	CalculatorAdapter* calculatorAdapter = NULL;
	stack<char> _stack;
	int loc_operator[100] = { 0 };
	char* _postfix;
	int postfix_length;
public:
	MainCalculator() {
		this->_postfix = nullptr;
		postfix_length = 0;
	}
	char* postfix() {
		return this->_postfix;
	}
	int GetPrecedence(char t) {
		if ((int)t >= 48 && (int)t <= 57) {
			return 0;
		}
		switch (t) {
		case 'a':
		case 'c':
		case 'd':
		case 'e':
		case 'f':
		case 'x':
		case 'b': return 5;
		case '(': return 2;
		case ')': return 1;
		case '*':
		case '/': return 4;
		case '+':
		case '-': return 3;
		default:
			return -1;
		}
	}
	int ComparePre(char op1, char op2) {
		int op1Pre = GetPrecedence(op1);
		int op2Pre = GetPrecedence(op2);
		if (op1Pre >= op2Pre) {
			return 1;
		}
		else {
			return 0;
		}
	}
	void Convert(string infix) {
		int length = infix.length();
		if (length > 100) {//100���ں��� ũ�� ���� throw
			throw infixException[3];
		}
		char word;
		int code;
		int j = 0;
		char* changed = new char[100];
		char temp;
		loc_operator[0] = 0;
		int k = 0;//������ ��ġ�� �����ϱ� ���� ����
		memset(changed, 0, sizeof(char) * 100);//�޸𸮿� �Ҵ�
		for (int i = 0; i < length; i++) {
			word = infix[i];
			code = GetPrecedence(word);
			switch (code) {
			case -1://ó���� �� ���� ����
				throw infixException[2];
			case 0://����
				changed[j] = word;
				j++;
				break;
			case 5://'x', 'b' ,16��Ʈ�� ���� ���
				changed[j] = word;
				j++;
				break;
			case 4://���ϱ� ������
			case 3://���ϱ� ����
				while (!this->_stack.empty()) {//������ ������� ������
					temp = this->_stack.top();
					if (ComparePre(temp, word)) {//���ÿ� �ִ� ������ �켱������ ũ�ų� ���� ��� ���ÿ��� ����
						this->_stack.pop();
						changed[j] = temp;
						j++;
					}
					else {
						break;
					}
				}
				this->_stack.push(word);
				loc_operator[k] = j;
				k++;
				break;
			case 2://'('�� ���
				this->_stack.push(word);
				break;
			case 1://')'�� ���
				while (1) {
					if (this->_stack.empty()) {//���ÿ� '('�� ���� ���
						throw infixException[0];
					}
					temp = this->_stack.top();
					this->_stack.pop();
					if (temp == '(')
						break;
					changed[j] = temp;
					j++;
				}
			default:
				break;
			}
		}
		while (!this->_stack.empty()) {//���� ������ pop
			temp = this->_stack.top();
			this->_stack.pop();
			if (temp == '(') {//���ÿ� '('�� �����ִ� ��� ���� throw
				throw infixException[1];
			}
			changed[j] = temp;
			j++;
		}
		this->postfix_length = j;//class������ ���� ����
		this->_postfix = new char[100];
		strcpy_s(this->_postfix, 100, changed);
		free(changed);//changed ����
		for (int v = 0; v < j; v++) {
			printf("%c", this->postfix()[v]);
		}
		printf("\n");
	}
	int Calc(int op1, int op2, char word) {

		switch (word) {
		case '+':
			return op1 + op2;
		case '-':
			return op1 - op2;
		case '*':
			return op1 * op2;
		case '/':
			if (op2 == 0) {
				throw postfixException[2];
			}
			return op1 / op2;
			break;
		}
	}
	int Result() {
		int length = this->postfix_length;
		int j = 0;//loc_operator�� ����
		char current[100];
		int x = 0;//current�� ����
		char* formatPostfix[100];//postfix�� ������ �ڸ����� �°� ����ش�.
		int y = 0;//formatPostfix�� ����
		for (int i = 0; i < length; i++) {
			if (GetPrecedence(postfix()[i]) == 3 || GetPrecedence(postfix()[i]) == 4) {
				if (x > 0) {//���ڰ� �ϳ��� ����� ��� �� ���ڸ� ������
					char* formatCurrent = new char[x + 1];
					for (int c = 0; c < x; c++) {
						formatCurrent[c] = current[c];
					}
					formatCurrent[x] = '\0';//���ڿ� current�� formatCurrent�� ����
					formatPostfix[y] = formatCurrent;//formatPostfix�� ���� ����
					y++;
					x = 0;
				}
				char* formatCurrent = new char[2];
				formatCurrent[0] = postfix()[i];
				formatCurrent[1] = '\0';
				formatPostfix[y] = formatCurrent;
				y++;
				j++;
				continue;
			}
			if (GetPrecedence(postfix()[i]) == 5) {//16��Ʈ�� 2��Ʈ�� ���
				while (1) {//�������� �κб��� 2bit�� 16bit���� ����
					if (i == this->loc_operator[j]) {
						break;
					}
					if (GetPrecedence(postfix()[i]) == 3 || GetPrecedence(postfix()[i]) == 4) {
						break;
					}
					current[x] = postfix()[i];
					x++;
					i++;
				}
				char* formatCurrent = new char[x + 1];
				for (int c = 0; c < x; c++) {
					formatCurrent[c] = current[c];
				}
				formatCurrent[x] = '\0';
				formatPostfix[y] = formatCurrent;
				y++;
				x = 0;
				j++;
				if (GetPrecedence(postfix()[i]) == 3 || GetPrecedence(postfix()[i]) == 4) {
					char* formatCurrent = new char[2];
					formatCurrent[0] = postfix()[i];
					formatCurrent[1] = '\0';
					formatPostfix[y] = formatCurrent;
					y++;
				}
			}
			if (i == this->loc_operator[j]) {//i�� ���� ������ ��ġ�� �� ��� ������ �и��Ѵ�.
				char* formatCurrent = new char[x + 1];
				for (int c = 0; c < x; c++) {
					formatCurrent[c] = current[c];
				}
				formatCurrent[x] = '\0';//���ڿ� current�� formatCurrent�� ����
				formatPostfix[y] = formatCurrent;//formatPostfix�� ���� ����
				y++;
				j++;//���� loc_operator ����
				x = 0;//x�� 0���� �ʱ�ȭ
			}
			if (this->GetPrecedence(postfix()[i]) == 0) {//���� postfix�� ���ڰ� ������ ���
				current[x] = postfix()[i];//current �迭�� �����Ѵ�.
				x++;
			}
		}

		stack<int> int_stack;
		for (int i = 0; i < y; i++) {
			if (this->GetPrecedence(formatPostfix[i][0]) == 4 ||
				this->GetPrecedence(formatPostfix[i][0]) == 3) {//������ ���� �������� ���
				char word = formatPostfix[i][0];//������
				if (int_stack.empty()) {//������ ����ִ� ��� ������ �� �� ����
					throw postfixException[1];
				}
				int op2 = int_stack.top();//�� ��° ����
				int_stack.pop();
				if (int_stack.empty()) {//������ ����ִ� ��� ������ �� �� ����
					throw postfixException[1];
				}
				int op1 = int_stack.top();//ù ��° ����
				int_stack.pop();
				int result = this->Calc(op1, op2, word);//���
				int_stack.push(result);
			}
			else if (GetPrecedence(formatPostfix[i][1]) == 5) {//����� ���� �� ��° ���� x�� b�� 16��Ʈ Ȥ�� 2��Ʈ�� ��ȯ�ؾ��Ѵ�.
				if (formatPostfix[i][1] == 'x') {//16��Ʈ
					calculatorAdapter = new CalculatorAdapter("sixteen");
					int_stack.push(calculatorAdapter->convert("sixteen", formatPostfix[i]));
				}
				else if (formatPostfix[i][1] == 'b') {//2��Ʈ
					calculatorAdapter = new CalculatorAdapter("two");
					int_stack.push(calculatorAdapter->convert("two", formatPostfix[i]));
				}
			}
			else {
				int_stack.push(atoi(formatPostfix[i]));//���� ���ÿ� ����
			}
		}
		if (int_stack.size() > 1) {//���ÿ� �������� �� �� �̻� �ִ� ��� ���� ���
			throw postfixException[0];
		}
		return int_stack.top();
	}
};
