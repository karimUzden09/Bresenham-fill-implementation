#include "CV_test_GUI.h"
#include <QtWidgets/QApplication>
/*
����������� ������ ����� �.�. ����������� ������ ������� ������� �������� ������������ �����
� ����������� ��������
*/
#pragma comment(linker, "/STACK:100000000000")

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CV_test_GUI w;
    w.show();
    return a.exec();
}
