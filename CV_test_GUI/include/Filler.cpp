#include "Filler.hpp"




uchar* Filler::fill_image_rec_ver
(
	uchar* image_data,
	int x,
	int y,
	uint32 pixel_brightness,
	uint32 filler_color
)
{
	// ������ � ��������� ������� ������������� � ���� [x + y * filler_rows] ��� ������ ������������ ������ [i][j]
	if (image_data[x + y * filler_rows] >= pixel_brightness) // �������� ���� ��� ������� ������� >= ������� �� �������
	{
		return(image_data);
	}
	if (image_data[x + y * filler_columns] == filler_color) // �������� ���� ��� ������� == ������������ ����� �� ������� �� �������
	{
		return(image_data);
	}

	image_data[x + y * filler_rows] = filler_color; // ����������� ������� �������

	if (x + 1 < filler_rows) // �������� �� ����� �� ������� �����������
	{
		fill_image_rec_ver(image_data, x + 1, y, pixel_brightness, filler_color); // �������� ��������� ������� � ��� ��� ���� �������� �������� � ��������� �� ����� �� ������ �����������
	}
	if (x > 0)
	{
		fill_image_rec_ver(image_data, x - 1, y, pixel_brightness, filler_color);
	}
	if (y + 1 < filler_rows)
	{
		fill_image_rec_ver(image_data, x, y + 1, pixel_brightness, filler_color);
	}
	if (y > 0)
	{
		fill_image_rec_ver(image_data, x, y - 1, pixel_brightness, filler_color);
	}
	return(image_data);
}

uchar* Filler::fill_image
(
	uchar* image_data,
	const Pixel& pos,
	uint32 pixel_brightness,
	uint32 filler_color
) noexcept
{
	std::queue<Pixel> queue; // �������� �������
	queue.push(pos); // ��������� � ������� ��������� �����
	Pixel temp{ 0,0 }; // ������� ��������� ����� ��� �������� ������������� �����
	while (!queue.empty()) // ���� ������� �� �����
	{
		temp = queue.front(); // ���������� � ������� �������� ������� � ��������� �� �� ��������� ����������
		queue.pop();// ������� ������� �� �������

		if (temp.x + 1 < filler_rows)// ��������� ������� �����������
		{
			if (image_data[(temp.x + 1) + temp.y * filler_rows] <= pixel_brightness &&
				image_data[(temp.x + 1) + temp.y * filler_rows] != filler_color)// ���� ��� �������� <= ��������� ��������� � != ����� ������� ����� ������ ��� ������� �
																				//��������� � ������� ��������� ��������� ������� � ��� ��� ���� �������� ��������
			{
				image_data[(temp.x + 1) + temp.y * filler_rows] = filler_color;
				temp.x = temp.x + 1;
				queue.push(temp);
			}
		}

		if (temp.x > 0)
		{
			if (image_data[(temp.x - 1) + temp.y * filler_rows] <= pixel_brightness &&
				image_data[(temp.x - 1) + temp.y * filler_rows] != filler_color)
			{
				image_data[(temp.x - 1) + temp.y * filler_rows] = filler_color;
				temp.x = temp.x - 1;
				queue.push(temp);
			}
		}

		if (temp.y > 0)
		{
			if (image_data[temp.x + (temp.y - 1) * filler_rows] <= pixel_brightness &&
				image_data[temp.x + (temp.y - 1) * filler_rows] != filler_color)
			{
				image_data[temp.x + (temp.y - 1) * filler_rows] = filler_color;
				temp.y = temp.y - 1;
				queue.push(temp);
			}

		}
		if (temp.y + 1 < filler_rows)
		{
			if (image_data[temp.x + (temp.y + 1) * filler_rows] <= pixel_brightness &&
				image_data[temp.x + (temp.y + 1) * filler_rows] != filler_color)
			{
				image_data[temp.x + (temp.y + 1) * filler_rows] = filler_color;
				temp.y = temp.y + 1;
				queue.push(temp);
			}
		}

	}
	return (image_data);
} 


void Filler::set_rows_columns(int32 rows, int32 columns)
{
	filler_rows = rows;
	filler_columns = columns;
}






#ifdef DEPRECATED_FILL_METHODS
cv::Mat Filler::fill_image
(
	cv::Mat image,
	const Pixel& pixel_pos,
	const uint32& pixel_brightness,
	const uint32& filler_color
) noexcept
{
	auto rows = image.rows;				// ���������� ����� � �����������
	auto columns = image.cols;			// ���������� �������� � �����������
	auto image_data = image.data;		// ��������� �� ������� ������ ����������� ������� �������� � ���� ������� ����
	std::queue<Pixel> queue;			// �������� �������
	queue.push(pixel_pos);				// ��������� � ������� ��������� �����
	Pixel temp{ 0,0 };					// ������� ��������� ����� ��� �������� ������������� �����
	while (!queue.empty())				// ���� ������� �� �����
	{
		temp = queue.front();			// ���������� � ������� �������� ������� � ��������� �� �� ��������� ����������
		queue.pop();					// ������� ������� �� �������

		if (temp.x + 1 < rows)			// ��������� ������� �����������
		{
			if (image_data[(temp.x + 1) + temp.y * rows] <= pixel_brightness &&
				image_data[(temp.x + 1) + temp.y * rows] != filler_color)// ���� ��� �������� <= ��������� ��������� � != ����� ������� ����� ������ ��� ������� �
																		//��������� � ������� ��������� ��������� ������� � ��� ��� ���� �������� ��������
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

	}
	return (image);
}


cv::Mat Filler::fill_image_rec_ver
(
	cv::Mat image,
	int x,
	int y,
	uint pixel_brightness,
	uint filler_color,
	uchar* image_data,
	int rows,
	int columns
)
{
	if (image_data[x + y * rows] >= pixel_brightness)
	{
		return image;
	}
	if (image_data[x + y * columns] == filler_color)
	{
		return image;
	}

	image_data[x + y * rows] = filler_color;

	if (x + 1 < rows)
	{
		fill_image_rec_ver(image, x + 1, y, pixel_brightness, filler_color, image_data, rows, columns);
	}
	if (x > 0)
	{
		fill_image_rec_ver(image, x - 1, y, pixel_brightness, filler_color, image_data, rows, columns);
	}
	if (y + 1 < rows)
	{
		fill_image_rec_ver(image, x, y + 1, pixel_brightness, filler_color, image_data, rows, columns);
	}
	if (y > 0)
	{
		fill_image_rec_ver(image, x, y - 1, pixel_brightness, filler_color, image_data, rows, columns);
	}

	return (image);
}

void Filler::fill_image_rec_ver
(
	uchar* image_data,
	int x,
	int y,
	uint pixel_brightness,
	uint filler_color,
	int rows,
	int columns
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

	if (x + 1 < rows)
	{
		fill_image_rec_ver(image_data, x + 1, y, pixel_brightness, filler_color, rows, columns);
	}
	if (x > 0)
	{
		fill_image_rec_ver(image_data, x - 1, y, pixel_brightness, filler_color, rows, columns);
	}
	if (y + 1 < rows)
	{
		fill_image_rec_ver(image_data, x, y + 1, pixel_brightness, filler_color, rows, columns);
	}
	if (y > 0)
	{
		fill_image_rec_ver(image_data, x, y - 1, pixel_brightness, filler_color, rows, columns);
	}
	return;
}

#endif // DEBUG




