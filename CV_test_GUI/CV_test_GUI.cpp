
#include "CV_test_GUI.h"

#include<qthread.h>
#include<filesystem>
CV_test_GUI::CV_test_GUI(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this); // запускаем основное окно
	set_enable_disable(false); // когда окно в первый раз запускается все виджеты кроме кнопки открыть изоброжение не активны
	set_enable_disable_save_widgets(false);
}


void CV_test_GUI::on_open_imge_button_clicked()
{
	fileName = QFileDialog::getOpenFileName(0, "Open Image", QString(), "BMP File(*.bmp)"); // Открытия диологового окна для загрузки изоброжения
	if (!fileName.isEmpty()) // если мы выбрали изоброжение 
	{
		image = QImage(fileName); // инициализируем поле image нашим выбранным изоброжением
		set_enable_disable(true); // все виджеты кроме кнопки сохронить делаем активными делаем октивными
		std::filesystem::path std_path{fileName.toStdWString()}; //используется библиотека filesystem из C++17 для того чтобы корректно передать путь до файла в функцию imread 
		cv_image = cv::imread(std_path.string()); // ницилизируем поле cv_image в котором хранится нашим выбранным изоброжением
		cv::cvtColor(cv_image, cv_image_grey_version, cv::COLOR_BGR2GRAY); // иницилизируем поле  cv_image_grey_version в котором будет хранится полутоновое версия изоброжения
		columns = cv_image.cols; //инициализируем поле columns  в котором хранится  количество строк в изоброжении
		rows = cv_image.rows; // инициализируем поле rows  в котором хранится  количество строк в изоброжении
		filler.set_rows_columns(rows, columns); // даем информацию об количестве столбцов и строк объекту класса Filler
		ui.x_point_spinBox->setMaximum(rows); // в виджете x_point_spinBox максимально допустимое значение будет равно значению rows
		ui.y_point_spinBox->setMaximum(columns); // в виджете y_point_spinBox максимально допустимое значение будет равно значению columns
		ui.x_size_lable->setText(QString::number(columns)); //отоброжение в x_size_lable клоичество строк в изоброжении
		ui.y_size_lable->setText(QString::number(rows)); //отоброжение в y_size_lable клоичество столбцов в изоброжении
		ui.Drow_palce->resize(image.width(), image.height()); // lable Подгоняется под размер изоброжения
		ui.Drow_palce->setPixmap(QPixmap::fromImage(image)); // отоброжется изоброжение в окне с помощю виджет label

	}

}

/*

В функиции все почти тоже самое что в функции fill_image в которая обявленна в зоголовочном файле Filler.hpp
и релизованна в файле Fillel.cpp за исключением пару вещей которые рассматриаются ниже

*/

void CV_test_GUI::fill_image_interactive(cv::Mat cv_image_arg, const Pixel& pixel_pos, const uint32& pixel_brightness, const uint32& filler_color)
{
	std::queue<Pixel> queue;
	queue.push(pixel_pos);
	Pixel temp{ 0,0 };
	while (!queue.empty())
	{
		temp = queue.front();
		queue.pop();

		if (temp.x + 1 < rows)
		{
			if (image_data[(temp.x + 1) + temp.y * rows] <= pixel_brightness && 
				image_data[(temp.x + 1) + temp.y * rows] != filler_color)
			{
				image_data[(temp.x + 1) + temp.y * rows] = filler_color;
				temp.x = temp.x + 1;
				queue.push(temp);
			}
		}

		if (temp.x > 0)
		{
			if (image_data[(temp.x - 1) + temp.y * rows] <= pixel_brightness &&
				image_data[(temp.x - 1) + temp.y * rows] != filler_color)
			{
				image_data[(temp.x - 1) + temp.y * rows] = filler_color;
				temp.x = temp.x - 1;
				queue.push(temp);
			}
		}

		if (temp.y > 0)
		{
			if (image_data[temp.x + (temp.y - 1) * rows] <= pixel_brightness &&
				image_data[temp.x + (temp.y - 1) * rows] != filler_color)
			{
				image_data[temp.x + (temp.y - 1) * rows] = filler_color;
				temp.y = temp.y - 1;
				queue.push(temp);
			}

		}
		if (temp.y + 1 < rows)
		{
			if (image_data[temp.x + (temp.y + 1) * rows] <= pixel_brightness &&
				image_data[temp.x + (temp.y + 1) * rows] != filler_color)
			{
				image_data[temp.x + (temp.y + 1) * rows] = filler_color;
				temp.y = temp.y + 1;
				queue.push(temp);
			}
		}

		cv_saved_image = cv::Mat(rows, columns, image_type, image_data); // обновляем output_image в соответсвии с изменениям данных изоброжения в результате приминения аллгоритма заливки
		cv::cvtColor(cv_saved_image, cv_saved_image, cv::COLOR_BGR2RGB);  // конвертируем тип изоброжения в COLOR_BGR2RGB для того чтобы совместить с QImage
		buff_image = QImage((uchar*)cv_saved_image.data, cv_saved_image.cols,
			cv_saved_image.rows, cv_saved_image.step, QImage::Format_RGB888); // из объекта cv::Mat переводим в тип QImage для того чтобы выводить на изменения изоброажение на экране Qt
		ui.Drow_palce->setPixmap(QPixmap::fromImage(buff_image)); // Отрисовываем изоброжение в окне qt точнее в  lable
		QThread::msleep(interctive_speed); // Приостонавливаем выполнение текущего потока чем выше значение переменной  interctive_speed тем медленей будет происходить отрисовка
		QCoreApplication::processEvents(); // Обновляем все виджеты на окне
	}
}

/*

В функиции все почти тоже самое что в функции fill_image_rec_interactive_version в которая обявленна в зоголовочном файле Filler.hpp
и релизованна в файле Fillel.cpp за исключением пару вещей которые рассматриаются ниже

*/
void CV_test_GUI::fill_image_rec_interactive_version
(
	cv::Mat cv_image_arg,
	int32 x,
	int32 y,
	const uint32& pixel_brightness,
	const uint32& filler_color
)
{
	if (image_data[x + y * rows] >= pixel_brightness)
	{
		return;
	}
	if (image_data[x + y * columns] == filler_color)
	{
		return;
	}
	
	image_data[x + y * rows] = filler_color;

	cv_saved_image = cv::Mat(rows, columns, image_type, image_data);  // обновляем output_image в соответсвии с изменениями данных изоброжения в результате приминения аллгоритма заливки
	cv::cvtColor(cv_image_arg, cv_saved_image, cv::COLOR_BGR2RGB);	// конвертируем тип изоброжения в COLOR_BGR2RGB для того чтобы совместить с QImage
	buff_image = QImage((uchar*)cv_saved_image.data, cv_saved_image.cols,
		cv_saved_image.rows, cv_saved_image.step, QImage::Format_RGB888); // конвертируем тип изоброжения в COLOR_BGR2RGB для того чтобы совместить с QImage
	ui.Drow_palce->setPixmap(QPixmap::fromImage(buff_image)); // Отрисовываем изоброжение в окне qt точнее в  lable
	QThread::msleep(interctive_speed); // Приостонавливаем выполнение текущего потока чем выше значение переменной  interctive_speed тем медленей будет происходить отрисовка
	QCoreApplication::processEvents(); // Обновляем все виджеты на окне
	if (x + 1 < rows)
	{
		fill_image_rec_interactive_version(cv_image_arg, x + 1, y, pixel_brightness, filler_color);
	}
	if (x > 0)
	{
		fill_image_rec_interactive_version(cv_image_arg, x - 1, y, pixel_brightness, filler_color);
	}
	if (y + 1 < rows)
	{
		fill_image_rec_interactive_version(cv_image_arg, x, y + 1, pixel_brightness, filler_color);
	}
	if (y > 0)
	{
		fill_image_rec_interactive_version(cv_image_arg, x, y - 1, pixel_brightness, filler_color);
	}
	return;
}

/*
		Функция set_enable_disable(bool state) в которой некоторые виджеты переводятся в активный
		режим если state==true и в не активный если state == false
*/
void CV_test_GUI::set_enable_disable(bool state) noexcept
{
	ui.to_fill->setEnabled(state);
	ui.pixel_brightness_spinBox->setEnabled(state);
	ui.filler_color_spinBox->setEnabled(state);
	ui.x_point_spinBox->setEnabled(state);
	ui.y_point_spinBox->setEnabled(state);
	ui.interactiv_ver_checkBox->setEnabled(state);
	ui.interactive_speed_spinBox->setEnabled(state);
	
	ui.iterative_radioButton->setEnabled(state);
	ui.recursive_radioButton->setEnabled(state);
	
}
/*
		Функция set_enable_disable_save_widgets(bool state) в которой виджеты для сохронения изображения переводятся в активный
		режим если state==true и в не активный если state == false
*/
void CV_test_GUI::set_enable_disable_save_widgets(bool state) noexcept
{
	ui.save_image_action->setEnabled(state);
	ui.save_pushButton->setEnabled(state);
}


void CV_test_GUI::on_to_fill_clicked()
{
	pos.x = ui.x_point_spinBox->value(); // считываем число которое укзанна в виджете ui.x_point_spinBox в поле X обекта Структуры Pxiel
	pos.y = ui.y_point_spinBox->value(); // считываем число которое укзанна в виджете ui.y_point_spinBox в поле Y обекта Структуры Pxiel
	filler_color = ui.filler_color_spinBox->value(); // считываем число которое укзанна в виджете ui.filler_color_spinBox в поле filler_color
	pixel_brightness = ui.pixel_brightness_spinBox->value(); // считываем число которое укзанна в виджете ui.pixel_brightness_spinBox в поле pixel_brightness
	interctive_speed = ui.interactive_speed_spinBox->value(); // Считываем число которое указана в виджете interactive_speed_spinBox где задется скорость выполнения интерактивной версий фукеции заливки чем выше щначение тем меделенней она будет выполнятся
	image_data = cv_image_grey_version.data; // В поле image_data буде хранится указатель на данные изоброжения
	image_type = cv_image_grey_version.type(); // В поле image_type будет хранится тип изображения
	filler.set_rows_columns(rows, columns);   // отпровляем данные ширины и высоты изобоажения для объекта класса Filler 

	if (ui.recursive_radioButton->isChecked()) // если у нас выбран рекурсивный режим
	{
		if (ui.interactiv_ver_checkBox->isChecked()) // если у нас выбран интерактивный режим
		{
			fill_image_rec_interactive_version(cv_image_grey_version, pos.x, pos.y, pixel_brightness, filler_color); // выполняем интерактивную версию рекурсивной функции заливки
		}
		else
		{
			
			auto filled_data = filler.fill_image_rec_ver(image_data, pos.x, pos.y, pixel_brightness, filler_color);								// Вызыаем рекурсивную версию заливки
			cv_saved_image = cv::Mat(rows, columns, cv_image_grey_version.type(), filled_data);													// В сохроняемой версии унас храниться изиененная версия изображения
			cv::cvtColor(cv_saved_image, cv_saved_image, cv::COLOR_BGR2RGB);																	// Переводим цвет в cv::COLOR_BGR2RGB
			image = QImage((uchar*)cv_saved_image.data, cv_saved_image.cols, cv_saved_image.rows, cv_saved_image.step, QImage::Format_RGB888);	// из объекта cv::Mat переводим в тип QImage
			ui.Drow_palce->setPixmap(QPixmap::fromImage(image));																				// Отрисовываем изоброжение в окне qt точнее в  lable
		}
	}
	if (ui.iterative_radioButton->isChecked()) // Если выбран итеративный режим
	{
		if (ui.interactiv_ver_checkBox->isChecked()) // Если  интерактвный режим включен
		{
			fill_image_interactive(cv_image_grey_version, pos, pixel_brightness, filler_color); // Вызов интерактивной версий функций заливки
		}
		else
		{
			
			auto filled_data = filler.fill_image(image_data, pos, pixel_brightness, filler_color);												// Вызыаем рекурсивную версию заливки
			cv_saved_image = cv::Mat(rows, columns, cv_image_grey_version.type(), filled_data);													// В сохроняемой версии унас храниться изиененная версия изображения
			cv::cvtColor(cv_saved_image, cv_saved_image, cv::COLOR_BGR2RGB);																	// Переводим цвет в cv::COLOR_BGR2RGB
			image = QImage((uchar*)cv_saved_image.data, cv_saved_image.cols, cv_saved_image.rows, cv_saved_image.step, QImage::Format_RGB888);	// из объекта cv::Mat переводим в тип QImage
			ui.Drow_palce->setPixmap(QPixmap::fromImage(image));																				// Отрисовываем изоброжение в окне qt точнее в  lable
		}
	}
	set_enable_disable_save_widgets(true); // Делаем активным кнопки для сохронения файла
	
}

void CV_test_GUI::on_save_pushButton_clicked()
{
	auto saved_file_name = QFileDialog::getSaveFileName(0, "Save Image", QString(), "BMP File(*.bmp)"); // Открывается диалог сохроненеия файла
	std::filesystem::path std_path{ saved_file_name.toStdWString() }; //используется библиотека filesystem из C++17 для того чтобы корректно передать путь сохронения файла в функцию imwrite 
	cv::imwrite(std_path.string(), cv_saved_image); // Сохроняем изображение
}

void CV_test_GUI::on_open_image_action_triggered()
{
	on_open_imge_button_clicked(); // Вызываем слот открытия изоброжение
}

void CV_test_GUI::on_save_image_action_triggered()
{
	on_save_pushButton_clicked(); // Вызываем слот сохронееия изоброжение
}

void CV_test_GUI::on_exit_action_triggered()
{
	QApplication::exit(); // Выходим из программы
}

void CV_test_GUI::on_about_action_triggered()
{
	QMessageBox msg;
	std::wstring W_Title{ L"О программе" };

	msg.setWindowTitle(QString::fromWCharArray(W_Title.c_str()));
	auto majot =  QString::number(CV_TEST_VER_MAJOR);
	auto minor = QString::number(CV_TEST_VER_MINOR);
	auto patch = QString::number(CV_TEST_VER_PATCH);
	QString text = { "Version " + majot + "." + minor +  "." + patch + " Copright(c) 2019-present, Karim Uzdenov & CV_TEST_GUI & contributs"};
	msg.setText(text);
	msg.exec();
}
