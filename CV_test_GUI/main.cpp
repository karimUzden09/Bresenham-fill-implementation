#include "CV_test_GUI.h"
#include <QtWidgets/QApplication>
/*
Увиличиваем размер стека т.к. рекурсивная версия функции заливки вызывает переполнение стека
с стандартным размером 
*/
#pragma comment(linker, "/STACK:100000000000")

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	CV_test_GUI w;
	w.show();
	return a.exec();
}
