#include <iostream>
#include <algorithm> 
#include <iomanip>
#include <cmath>
#include <string>
#include <vector>
#include<random>
using namespace std;

enum class enLevel { Easy = 1, Med, Hard, Mix };
enum class enOpType { Add = 1, Sub, Mult, Div, Mix };
enum class enResult { Right = 1, Wrong };

struct stQuestion {
	int questionNumber;
	enLevel level;
	enOpType opType;
	int firstNumber;
	int secondNumber;
	double questionAnswer;
};

struct stQuiz {
	int gameRounds;
	vector<stQuestion> questionsList;
	enLevel level;
	enOpType opType;
	int rightAnswers = 0;
	int wrongAnswers = 0;
	enResult quizResult;
};

int ReadNumber(const string& msg, int from, int to) {
	int number;
	do {
		cout << msg;
		cin >> number;
	} while (number < from || number > to);
	return number;
}

int RandomNumber(int from, int to) {
	static mt19937 gen(random_device{}());
	uniform_int_distribution<> dist(from, to);
	return dist(gen);
}

enLevel ReadLevel() {
	return(enLevel)ReadNumber("\nEnter Questions Level? [1]: Easy, [2]: Med, [3]: Hard, [4]: Mix ? ", 1, 4);
}

enLevel ResolveLevel(const enLevel& level) {
	if (level == enLevel::Mix) return(enLevel)RandomNumber(1, 3);
	return level;
}

enOpType ReadOpType() {
	return(enOpType)ReadNumber("\nEnter Operation Type? [1]: Add, [2]: Sub, [3]: Mult, [4]: Div, [5]: Mix ? ", 1, 5);
}

enOpType ResolveOpType(const enOpType& opType) {
	if (opType == enOpType::Mix) return(enOpType)RandomNumber(1, 4);
	return opType;
}

int GenerateNumber(const enLevel& level) {
	switch (level) {
	case enLevel::Easy: return(RandomNumber(1, 10));
	case enLevel::Med: return(RandomNumber(10, 50));
	case enLevel::Hard: return(RandomNumber(50, 100));
	default: return(RandomNumber(1, 10));
	}
}

double CalculateAnswer(int firstNumber, int secondNumber, const enOpType& opType) {
	switch (opType) {
	case enOpType::Add: return(firstNumber + secondNumber);
	case enOpType::Sub: return(firstNumber - secondNumber);
	case enOpType::Mult: return(firstNumber * secondNumber);
	case enOpType::Div: return(firstNumber / (double)secondNumber);
	default: return(firstNumber + secondNumber);
	}
}

stQuestion GenerateQuestion(int questionNumber, const enLevel& level, const enOpType& opType) {
	stQuestion question;
	question.questionNumber = questionNumber;
	question.level = ResolveLevel(level);
	question.opType = ResolveOpType(opType);
	question.firstNumber = GenerateNumber(question.level);
	question.secondNumber = GenerateNumber(question.level);
	question.questionAnswer = CalculateAnswer(question.firstNumber, question.secondNumber, question.opType);
	return question;
}

string GetOpSymbol(const enOpType& opType) {
	string symbols[]{ "+","-","*","/" };
	return symbols[int(opType) - 1];
}

void PrintQuestion(const stQuestion& question, int gameRounds) {
	cout << "\nQuestion [" << question.questionNumber << "/" << gameRounds << "]\n\n";
	cout << question.firstNumber << " "
		<< GetOpSymbol(question.opType) << " "
		<< question.secondNumber << " =? ";
}

double ReadPlayerAnswer() {
	double number;
	cin >> number;
	return number;
}

enResult IsEqual(double playerAnswer, double result) {
	if (abs(playerAnswer - result) < 0.00001) return enResult::Right;
	return enResult::Wrong;
}

void UpdateScore(const enResult& result, int& rightAnswers, int& wrongAnswers) {
	if (result == enResult::Right) rightAnswers++;
	else wrongAnswers++;
}

void ScreenColor(const enResult& result) {
	if (result == enResult::Right) system("color 2F");
	else system("color 4F");
}

void PrintQuestionResult(const enResult& result, double questionAnswer) {
	if (result == enResult::Right) {
		cout << "\nRight Answer :-)\n";
		cout << "\n-------------------------\n";
	}
	else {
		cout << "\nWrong Answer :-(\n";
		cout << "\nRight Answer =? " << questionAnswer << endl;
		cout << "\n-------------------------\n";


	}
}

enResult FinalResult(int rightAnswers, int wrongAnswers) {
	if (rightAnswers >= wrongAnswers) return enResult::Right;
	return enResult::Wrong;
}

stQuiz BuildQuiz(int gameRounds, const enLevel& level, const enOpType& opType) {
	stQuiz quiz;
	quiz.gameRounds = gameRounds;
	quiz.level = level;
	quiz.opType = opType;
	quiz.questionsList.resize(gameRounds);
	return quiz;
}

void CreateQuestion(stQuiz& quiz, int i) {
	quiz.questionsList[i] = GenerateQuestion(i + 1, quiz.level, quiz.opType);
}

void ProcessQuestion(stQuiz& quiz, int i) {

	CreateQuestion(quiz, i);

	PrintQuestion(quiz.questionsList[i], quiz.gameRounds);

	double playerAnswer = ReadPlayerAnswer();

	enResult result = IsEqual(playerAnswer, quiz.questionsList[i].questionAnswer);

	ScreenColor(result);

	PrintQuestionResult(result, quiz.questionsList[i].questionAnswer);

	UpdateScore(result, quiz.rightAnswers, quiz.wrongAnswers);


}

stQuiz GenerateQuiz(int gameRounds, const enLevel& level, const enOpType& opType) {
	stQuiz quiz = BuildQuiz(gameRounds, level, opType);

	for (int i = 0; i < gameRounds; i++) {

		ProcessQuestion(quiz, i);
	}

	quiz.quizResult = FinalResult(quiz.rightAnswers, quiz.wrongAnswers);

	return quiz;
}

string Tabs(int number) {
	string tabs = "";

	for (int i = 1; i <= number; i++) {
		tabs += "\t";
	}
	return tabs;
}

void ShowQuizResult() {
	cout << Tabs(4) << "-------------------------------------------------------\n\n";
	cout << Tabs(4) << "                     [ Quiz Result ] \n\n";
	cout << Tabs(4) << "-------------------------------------------------------\n";

}

string GetQuestionsLevel(const enLevel& level) {
	string levels[]{ "Easy","Med","Hard","Mix" };
	return levels[int(level) - 1];
}

string GetOpType(const enOpType& opType) {
	string operationType[]{ "Add","Sub","Mult","Div","Mix" };
	return operationType[int(opType) - 1];
}

string GetQuizResult(const enResult& result) {
	string quizResult[]{ "Pass","Fail" };
	return quizResult[int(result) - 1];
}

void PrintQuizResult(const stQuiz& quiz) {
	cout << Tabs(4) << "-------------------------------------------------------\n\n";
	cout << Tabs(4) << "Number Of Questions       : " << quiz.gameRounds << endl << endl;
	cout << Tabs(4) << "Questions Level           : " << GetQuestionsLevel(quiz.level) << endl << endl;
	cout << Tabs(4) << "Operation Type            : " << GetOpType(quiz.opType) << endl << endl;
	cout << Tabs(4) << "Number Of Right Answers   : " << quiz.rightAnswers << endl << endl;
	cout << Tabs(4) << "Number Of Wrong Answers   : " << quiz.wrongAnswers << endl << endl;
	cout << Tabs(4) << "Final Result              : " << GetQuizResult(quiz.quizResult) << endl << endl;
	cout << Tabs(4) << "-------------------------------------------------------\n";

}

void ResetScreen() {
	system("cls");
	system("color 0F");
}

void StartQuiz() {
	char playAgain = 'Y';
	int gameRounds;
	enLevel level;
	enOpType opType;
	stQuiz quiz;

	do {

		ResetScreen();

		gameRounds = ReadNumber("\nHow Many Rounds (1-10)? ", 1, 10);

		level = ReadLevel();

		opType = ReadOpType();

		quiz = GenerateQuiz(gameRounds, level, opType);

		ShowQuizResult();

		PrintQuizResult(quiz);

		ScreenColor(quiz.quizResult);

		cout << endl << Tabs(4) << "Do You Want To Play Again? (Y/N)? ";
		cin >> playAgain;

	} while (playAgain == 'Y' || playAgain == 'y');
}



int main() {

	StartQuiz();


}