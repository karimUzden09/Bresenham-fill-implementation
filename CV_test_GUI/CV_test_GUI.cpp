
#include "CV_test_GUI.h"

#include<qthread.h>
#include<filesystem>
CV_test_GUI::CV_test_GUI(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this); // ��������� �������� ����
	set_enable_disable(false); // ����� ���� � ������ ��� ����������� ��� ������� ����� ������ ������� ����������� �� �������
	set_enable_disable_save_widgets(false);
}


void CV_test_GUI::on_open_imge_button_clicked()
{
	fileName = QFileDialog::getOpenFileName(0, "Open Image", QString(), "BMP File(*.bmp)"); // �������� ����������� ���� ��� �������� �����������
	if (!fileName.isEmpty()) // ���� �� ������� ����������� 
	{
		image = QImage(fileName); // �������������� ���� image ����� ��������� ������������
		set_enable_disable(true); // ��� ������� ����� ������ ��������� ������ ��������� ������ ���������
		std::filesystem::path std_path{fileName.toStdWString()}; //������������ ���������� filesystem �� C++17 ��� ���� ����� ��������� �������� ���� �� ����� � ������� imread 
		cv_image = cv::imread(std_path.string()); // ������������ ���� cv_image � ������� �������� ����� ��������� ������������
		cv::cvtColor(cv_image, cv_image_grey_version, cv::COLOR_BGR2GRAY); // ������������� ����  cv_image_grey_version � ������� ����� �������� ����������� ������ �����������
		columns = cv_image.cols; //�������������� ���� columns  � ������� ��������  ���������� ����� � �����������
		rows = cv_image.rows; // �������������� ���� rows  � ������� ��������  ���������� ����� � �����������
		filler.set_rows_columns(rows, columns); // ���� ���������� �� ���������� �������� � ����� ������� ������ Filler
		ui.x_point_spinBox->setMaximum(rows); // � ������� x_point_spinBox ����������� ���������� �������� ����� ����� �������� rows
		ui.y_point_spinBox->setMaximum(columns); // � ������� y_point_spinBox ����������� ���������� �������� ����� ����� �������� columns
		ui.x_size_lable->setText(QString::number(columns)); //����������� � x_size_lable ���������� ����� � �����������
		ui.y_size_lable->setText(QString::number(rows)); //����������� � y_size_lable ���������� �������� � �����������
		ui.Drow_palce->resize(image.width(), image.height()); // lable ����������� ��� ������ �����������
		ui.Drow_palce->setPixmap(QPixmap::fromImage(image)); // ����������� ����������� � ���� � ������ ������ label

	}

}

/*

� �������� ��� ����� ���� ����� ��� � ������� fill_image � ������� ��������� � ������������ ����� Filler.hpp
� ����������� � ����� Fillel.cpp �� ����������� ���� ����� ������� �������������� ����

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

		cv_saved_image = cv::Mat(rows, columns, image_type, image_data); // ��������� output_image � ����������� � ���������� ������ ����������� � ���������� ���������� ���������� �������
		cv::cvtColor(cv_saved_image, cv_saved_image, cv::COLOR_BGR2RGB);  // ������������ ��� ����������� � COLOR_BGR2RGB ��� ���� ����� ���������� � QImage
		buff_image = QImage((uchar*)cv_saved_image.data, cv_saved_image.cols,
			cv_saved_image.rows, cv_saved_image.step, QImage::Format_RGB888); // �� ������� cv::Mat ��������� � ��� QImage ��� ���� ����� �������� �� ��������� ������������ �� ������ Qt
		ui.Drow_palce->setPixmap(QPixmap::fromImage(buff_image)); // ������������ ����������� � ���� qt ������ �  lable
		QThread::msleep(interctive_speed); // ���������������� ���������� �������� ������ ��� ���� �������� ����������  interctive_speed ��� �������� ����� ����������� ���������
		QCoreApplication::processEvents(); // ��������� ��� ������� �� ����
	}
}

/*

� �������� ��� ����� ���� ����� ��� � ������� fill_image_rec_interactive_version � ������� ��������� � ������������ ����� Filler.hpp
� ����������� � ����� Fillel.cpp �� ����������� ���� ����� ������� �������������� ����

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

	cv_saved_image = cv::Mat(rows, columns, image_type, image_data);  // ��������� output_image � ����������� � ����������� ������ ����������� � ���������� ���������� ���������� �������
	cv::cvtColor(cv_image_arg, cv_saved_image, cv::COLOR_BGR2RGB);	// ������������ ��� ����������� � COLOR_BGR2RGB ��� ���� ����� ���������� � QImage
	buff_image = QImage((uchar*)cv_saved_image.data, cv_saved_image.cols,
		cv_saved_image.rows, cv_saved_image.step, QImage::Format_RGB888); // ������������ ��� ����������� � COLOR_BGR2RGB ��� ���� ����� ���������� � QImage
	ui.Drow_palce->setPixmap(QPixmap::fromImage(buff_image)); // ������������ ����������� � ���� qt ������ �  lable
	QThread::msleep(interctive_speed); // ���������������� ���������� �������� ������ ��� ���� �������� ����������  interctive_speed ��� �������� ����� ����������� ���������
	QCoreApplication::processEvents(); // ��������� ��� ������� �� ����
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
		������� set_enable_disable(bool state) � ������� ��������� ������� ����������� � ��������
		����� ���� state==true � � �� �������� ���� state == false
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
		������� set_enable_disable_save_widgets(bool state) � ������� ������� ��� ���������� ����������� ����������� � ��������
		����� ���� state==true � � �� �������� ���� state == false
*/
void CV_test_GUI::set_enable_disable_save_widgets(bool state) noexcept
{
	ui.save_image_action->setEnabled(state);
	ui.save_pushButton->setEnabled(state);
}


void CV_test_GUI::on_to_fill_clicked()
{
	pos.x = ui.x_point_spinBox->value(); // ��������� ����� ������� ������� � ������� ui.x_point_spinBox � ���� X ������ ��������� Pxiel
	pos.y = ui.y_point_spinBox->value(); // ��������� ����� ������� ������� � ������� ui.y_point_spinBox � ���� Y ������ ��������� Pxiel
	filler_color = ui.filler_color_spinBox->value(); // ��������� ����� ������� ������� � ������� ui.filler_color_spinBox � ���� filler_color
	pixel_brightness = ui.pixel_brightness_spinBox->value(); // ��������� ����� ������� ������� � ������� ui.pixel_brightness_spinBox � ���� pixel_brightness
	interctive_speed = ui.interactive_speed_spinBox->value(); // ��������� ����� ������� ������� � ������� interactive_speed_spinBox ��� ������� �������� ���������� ������������� ������ ������� ������� ��� ���� �������� ��� ���������� ��� ����� ����������
	image_data = cv_image_grey_version.data; // � ���� image_data ���� �������� ��������� �� ������ �����������
	image_type = cv_image_grey_version.type(); // � ���� image_type ����� �������� ��� �����������
	filler.set_rows_columns(rows, columns);   // ���������� ������ ������ � ������ ����������� ��� ������� ������ Filler 

	if (ui.recursive_radioButton->isChecked()) // ���� � ��� ������ ����������� �����
	{
		if (ui.interactiv_ver_checkBox->isChecked()) // ���� � ��� ������ ������������� �����
		{
			fill_image_rec_interactive_version(cv_image_grey_version, pos.x, pos.y, pixel_brightness, filler_color); // ��������� ������������� ������ ����������� ������� �������
		}
		else
		{
			
			auto filled_data = filler.fill_image_rec_ver(image_data, pos.x, pos.y, pixel_brightness, filler_color);								// ������� ����������� ������ �������
			cv_saved_image = cv::Mat(rows, columns, cv_image_grey_version.type(), filled_data);													// � ����������� ������ ���� ��������� ���������� ������ �����������
			cv::cvtColor(cv_saved_image, cv_saved_image, cv::COLOR_BGR2RGB);																	// ��������� ���� � cv::COLOR_BGR2RGB
			image = QImage((uchar*)cv_saved_image.data, cv_saved_image.cols, cv_saved_image.rows, cv_saved_image.step, QImage::Format_RGB888);	// �� ������� cv::Mat ��������� � ��� QImage
			ui.Drow_palce->setPixmap(QPixmap::fromImage(image));																				// ������������ ����������� � ���� qt ������ �  lable
		}
	}
	if (ui.iterative_radioButton->isChecked()) // ���� ������ ����������� �����
	{
		if (ui.interactiv_ver_checkBox->isChecked()) // ����  ������������ ����� �������
		{
			fill_image_interactive(cv_image_grey_version, pos, pixel_brightness, filler_color); // ����� ������������� ������ ������� �������
		}
		else
		{
			
			auto filled_data = filler.fill_image(image_data, pos, pixel_brightness, filler_color);												// ������� ����������� ������ �������
			cv_saved_image = cv::Mat(rows, columns, cv_image_grey_version.type(), filled_data);													// � ����������� ������ ���� ��������� ���������� ������ �����������
			cv::cvtColor(cv_saved_image, cv_saved_image, cv::COLOR_BGR2RGB);																	// ��������� ���� � cv::COLOR_BGR2RGB
			image = QImage((uchar*)cv_saved_image.data, cv_saved_image.cols, cv_saved_image.rows, cv_saved_image.step, QImage::Format_RGB888);	// �� ������� cv::Mat ��������� � ��� QImage
			ui.Drow_palce->setPixmap(QPixmap::fromImage(image));																				// ������������ ����������� � ���� qt ������ �  lable
		}
	}
	set_enable_disable_save_widgets(true); // ������ �������� ������ ��� ���������� �����
	
}

void CV_test_GUI::on_save_pushButton_clicked()
{
	auto saved_file_name = QFileDialog::getSaveFileName(0, "Save Image", QString(), "BMP File(*.bmp)"); // ����������� ������ ����������� �����
	std::filesystem::path std_path{ saved_file_name.toStdWString() }; //������������ ���������� filesystem �� C++17 ��� ���� ����� ��������� �������� ���� ���������� ����� � ������� imwrite 
	cv::imwrite(std_path.string(), cv_saved_image); // ��������� �����������
}

void CV_test_GUI::on_open_image_action_triggered()
{
	on_open_imge_button_clicked(); // �������� ���� �������� �����������
}

void CV_test_GUI::on_save_image_action_triggered()
{
	on_save_pushButton_clicked(); // �������� ���� ���������� �����������
}

void CV_test_GUI::on_exit_action_triggered()
{
	QApplication::exit(); // ������� �� ���������
}

void CV_test_GUI::on_about_action_triggered()
{
	QMessageBox msg;
	std::wstring W_Title{ L"� ���������" };

	msg.setWindowTitle(QString::fromWCharArray(W_Title.c_str()));
	auto majot =  QString::number(CV_TEST_VER_MAJOR);
	auto minor = QString::number(CV_TEST_VER_MINOR);
	auto patch = QString::number(CV_TEST_VER_PATCH);
	QString text = { "Version " + majot + "." + minor +  "." + patch + " Copright(c) 2019-present, Karim Uzdenov & CV_TEST_GUI & contributs"};
	msg.setText(text);
	msg.exec();
}
