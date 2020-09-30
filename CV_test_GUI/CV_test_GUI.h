#pragma once

#include <QtWidgets/QMainWindow>
#include<qfiledialog.h>
#include<qimage.h>
#include<qpixmap.h>
#include<qmessagebox.h>
#include<opencv2/core.hpp>
#include<opencv2/imgcodecs.hpp>
#include<opencv2/imgproc.hpp>

#include"include/Filler.hpp"
#include"version.hpp"


#include "ui_CV_test_GUI.h"
class CV_test_GUI : public QMainWindow
{
	Q_OBJECT

public:
	CV_test_GUI(QWidget *parent = Q_NULLPTR);

private:
	QString fileName; // ���� �������� �����
	int32 columns; // ���������� ����� � �����������
	int32 rows; // ���������� �������� � �����������
	uchar* image_data; // �������� �� ������ �����������
	int32 image_type; // ��� �����������
	cv::Mat cv_image; // �������� �������� ���������� �  ���� ������ ������ Mat �� ���������� opencv 4.1.1
	cv::Mat cv_image_grey_version; // ����� �������� ����������� ������ �����������
	cv::Mat cv_saved_image; // ����������� ������ �����������
	cv::Mat cv_buffer;
	Filler filler; // ������ ��� ������������� � ��������� �������
	Ui::CV_test_GUIClass ui; // ������ ����
	Pixel pos{ 0,0 };// ������ ��������� ��� �������� ��������� ������� 
	uint32 pixel_brightness; // �������� ������� �������
	uint32 filler_color;  // ���� �������
	QImage image; // � ������� ������� QImage ����������� ����� ��������� � ���� Qt
	QImage buff_image;
	size_t interctive_speed = 0; // �������� ������ ������� ������� � ������������� ������

	void fill_image_interactive // ������������� ������ ����������� �������
	(
		cv::Mat cv_image_arg,// �������� �������� ���������� �  ���� ������ ������ Mat �� ���������� opencv 4.1.1

		const Pixel& pixel_pos, // ��������� ��� �������� ��������� x , y

		const uint32& pixel_brightness,// �������� ������� �������
		const uint32& filler_color	   //���� �������
	);

	void fill_image_rec_interactive_version // ������������� ������ ����������� �������
	(
		cv::Mat cv_image_arg,// �������� �������� ���������� �  ���� ������ ������ Mat �� ���������� opencv 4.1.1
		int32 x,
		int32 y,
		const uint32& pixel_brightness,// �������� ������� �������
		const uint32& filler_color	   //���� �������
	);

	/*
		������� � ������� ��������� ������� ����������� � �������� 
		����� ���� state==true � � �� �������� ����� state == false 
	*/
	void set_enable_disable(bool state) noexcept; 
	/*
		������� set_enable_disable_save_widgets(bool state) � ������� ������� ��� ���������� ����������� 
		����������� � �������� ����� ���� state==true � � �� �������� ���� state == false
*/
	void set_enable_disable_save_widgets(bool state) noexcept;

private slots:
	void on_open_imge_button_clicked();			// ���� ��� ������ ������� �����������
	void on_to_fill_clicked();					// ���� ��� ������ ������� �����������
	void on_save_pushButton_clicked();			// ���� ��� ������ ����������� �����������
	void on_open_image_action_triggered();		// ���� ��� ������ ���� ������� �����������
	void on_save_image_action_triggered();		// C��� ��� ������ ���� ��������� �����������
	void on_exit_action_triggered();			// ���� ��� ������ ���� �����
	void on_about_action_triggered();			// ���� ��� ������ ���� � ���������
};
