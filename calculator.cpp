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
		if (length > 100) {//100글자보다 크면 오류 throw
			throw infixException[3];
		}
		char word;
		int code;
		int j = 0;
		char* changed = new char[100];
		char temp;
		loc_operator[0] = 0;
		int k = 0;//연산자 위치를 저장하기 위한 변수
		memset(changed, 0, sizeof(char) * 100);//메모리에 할당
		for (int i = 0; i < length; i++) {
			word = infix[i];
			code = GetPrecedence(word);
			switch (code) {
			case -1://처리할 수 없는 문자
				throw infixException[2];
			case 0://숫자
				changed[j] = word;
				j++;
				break;
			case 5://'x', 'b' ,16비트의 수의 경우
				changed[j] = word;
				j++;
				break;
			case 4://곱하기 나누기
			case 3://더하기 빼기
				while (!this->_stack.empty()) {//스택이 비어있지 않으면
					temp = this->_stack.top();
					if (ComparePre(temp, word)) {//스택에 있는 연산자 우선순위가 크거나 같은 경우 스택에서 뺀다
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
			case 2://'('의 경우
				this->_stack.push(word);
				break;
			case 1://')'의 경우
				while (1) {
					if (this->_stack.empty()) {//스택에 '('가 없는 경우
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
		while (!this->_stack.empty()) {//남은 연산자 pop
			temp = this->_stack.top();
			this->_stack.pop();
			if (temp == '(') {//스택에 '('가 남아있는 경우 오류 throw
				throw infixException[1];
			}
			changed[j] = temp;
			j++;
		}
		this->postfix_length = j;//class변수에 길이 저장
		this->_postfix = new char[100];
		strcpy_s(this->_postfix, 100, changed);
		free(changed);//changed 해제
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
		int j = 0;//loc_operator에 접근
		char current[100];
		int x = 0;//current에 접근
		char* formatPostfix[100];//postfix의 정수를 자릿수에 맞게 모아준다.
		int y = 0;//formatPostfix에 접근
		for (int i = 0; i < length; i++) {
			if (GetPrecedence(postfix()[i]) == 3 || GetPrecedence(postfix()[i]) == 4) {
				if (x > 0) {//문자가 하나라도 저장된 경우 그 문자를 빼낸다
					char* formatCurrent = new char[x + 1];
					for (int c = 0; c < x; c++) {
						formatCurrent[c] = current[c];
					}
					formatCurrent[x] = '\0';//문자열 current를 formatCurrent에 복사
					formatPostfix[y] = formatCurrent;//formatPostfix에 정수 저장
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
			if (GetPrecedence(postfix()[i]) == 5) {//16비트나 2비트인 경우
				while (1) {//나눠지는 부분까지 2bit나 16bit수를 저장
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
			if (i == this->loc_operator[j]) {//i가 기존 연산자 위치에 올 경우 정수를 분리한다.
				char* formatCurrent = new char[x + 1];
				for (int c = 0; c < x; c++) {
					formatCurrent[c] = current[c];
				}
				formatCurrent[x] = '\0';//문자열 current를 formatCurrent에 복사
				formatPostfix[y] = formatCurrent;//formatPostfix에 정수 저장
				y++;
				j++;//다음 loc_operator 참조
				x = 0;//x를 0으로 초기화
			}
			if (this->GetPrecedence(postfix()[i]) == 0) {//현재 postfix의 문자가 정수인 경우
				current[x] = postfix()[i];//current 배열에 저장한다.
				x++;
			}
		}

		stack<int> int_stack;
		for (int i = 0; i < y; i++) {
			if (this->GetPrecedence(formatPostfix[i][0]) == 4 ||
				this->GetPrecedence(formatPostfix[i][0]) == 3) {//접근한 값이 연산자인 경우
				char word = formatPostfix[i][0];//연산자
				if (int_stack.empty()) {//스택이 비어있는 경우 연산을 할 수 없다
					throw postfixException[1];
				}
				int op2 = int_stack.top();//두 번째 정수
				int_stack.pop();
				if (int_stack.empty()) {//스택이 비어있는 경우 연산을 할 수 없다
					throw postfixException[1];
				}
				int op1 = int_stack.top();//첫 번째 정수
				int_stack.pop();
				int result = this->Calc(op1, op2, word);//계산
				int_stack.push(result);
			}
			else if (GetPrecedence(formatPostfix[i][1]) == 5) {//저장된 수의 두 번째 수가 x나 b면 16비트 혹은 2비트를 변환해야한다.
				if (formatPostfix[i][1] == 'x') {//16비트
					calculatorAdapter = new CalculatorAdapter("sixteen");
					int_stack.push(calculatorAdapter->convert("sixteen", formatPostfix[i]));
				}
				else if (formatPostfix[i][1] == 'b') {//2비트
					calculatorAdapter = new CalculatorAdapter("two");
					int_stack.push(calculatorAdapter->convert("two", formatPostfix[i]));
				}
			}
			else {
				int_stack.push(atoi(formatPostfix[i]));//정수 스택에 삽입
			}
		}
		if (int_stack.size() > 1) {//스택에 정수값이 두 개 이상 있는 경우 오류 출력
			throw postfixException[0];
		}
		return int_stack.top();
	}
};
