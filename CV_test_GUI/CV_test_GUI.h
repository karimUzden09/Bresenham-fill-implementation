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
	QString fileName; // Путь текущего файла
	int32 columns; // Количество строк в изоброжении
	int32 rows; // Количество столбцов в изоброжении
	uchar* image_data; // казатель на данные изображения
	int32 image_type; // тип изображения
	cv::Mat cv_image; // Исходная картинка передается в  виде обекта класса Mat из библиотеки opencv 4.1.1
	cv::Mat cv_image_grey_version; // Здесь хранится полутоновое версия изоброжения
	cv::Mat cv_saved_image; // сохроняемая версия изображения
	cv::Mat cv_buffer;
	Filler filler; // обеъкт для взаимоействия с функциями заливки
	Ui::CV_test_GUIClass ui; // Объект окна
	Pixel pos{ 0,0 };// Объект структуру для хронения координат пикселя 
	uint32 pixel_brightness; // Порговая яркость пикселя
	uint32 filler_color;  // Цвет заливки
	QImage image; // С помощью объекта QImage изоброжение будет выводится в окне Qt
	QImage buff_image;
	size_t interctive_speed = 0; // Скорость работы функции заливки в интерактивном режиме

	void fill_image_interactive // Интерактивная версия итеративной функции
	(
		cv::Mat cv_image_arg,// Исходная картинка передается в  виде обекта класса Mat из библиотеки opencv 4.1.1

		const Pixel& pixel_pos, // Структура для хронения кординать x , y

		const uint32& pixel_brightness,// Порговая яркость пикселя
		const uint32& filler_color	   //Цвет заливки
	);

	void fill_image_rec_interactive_version // Интерактивная версия рекурсивной функции
	(
		cv::Mat cv_image_arg,// Исходная картинка передается в  виде обекта класса Mat из библиотеки opencv 4.1.1
		int32 x,
		int32 y,
		const uint32& pixel_brightness,// Порговая яркость пикселя
		const uint32& filler_color	   //Цвет заливки
	);

	/*
		Функция в которой некоторые виджеты переводятся в активный 
		режим если state==true и в не активный ексли state == false 
	*/
	void set_enable_disable(bool state) noexcept; 
	/*
		Функция set_enable_disable_save_widgets(bool state) в которой виджеты для сохронения изображения 
		переводятся в активный режим если state==true и в не активный если state == false
*/
	void set_enable_disable_save_widgets(bool state) noexcept;

private slots:
	void on_open_imge_button_clicked();			// Слот для кнопки открыть изображение
	void on_to_fill_clicked();					// Слот для кнопки заливки изображения
	void on_save_pushButton_clicked();			// Слот для кнопки сохроненеия изображения
	void on_open_image_action_triggered();		// Слот для пункта меню открыть изображение
	void on_save_image_action_triggered();		// Cлот для пункта меню сохронить изображение
	void on_exit_action_triggered();			// Слот для пункта меню выход
	void on_about_action_triggered();			// Слот для пункта меню о программе
};
