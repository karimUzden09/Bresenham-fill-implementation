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
	// доступ к элементам массива осущетвляется в виде [x + y * filler_rows] эта запись эквивалаента записи [i][j]
	if (image_data[x + y * filler_rows] >= pixel_brightness) // проверка если наш текущий пиксель >= выходим из функции
	{
		return(image_data);
	}
	if (image_data[x + y * filler_columns] == filler_color) // проверка если наш текущий == закрашиваему цвету то выходим из фуекции
	{
		return(image_data);
	}

	image_data[x + y * filler_rows] = filler_color; // закрашиваем текущий пиксель

	if (x + 1 < filler_rows) // Проверка на выход за гранциы изображения
	{
		fill_image_rec_ver(image_data, x + 1, y, pixel_brightness, filler_color); // вызываем рекрсивна функцию и так для всех соседних пикселей с проверкой на выход за гранцы изображения
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
	std::queue<Pixel> queue; // Обявляем очередь
	queue.push(pos); // Добовляем в очередь начальную точку
	Pixel temp{ 0,0 }; // Создаем временный обект для хранения промежуточных точек
	while (!queue.empty()) // Пока очередь не пуста
	{
		temp = queue.front(); // оброщяемся к первому элементу очереди и сохроняем ее во временной переменной
		queue.pop();// удаляем элемент из очереди

		if (temp.x + 1 < filler_rows)// Проверяем границы изоброжения
		{
			if (image_data[(temp.x + 1) + temp.y * filler_rows] <= pixel_brightness &&
				image_data[(temp.x + 1) + temp.y * filler_rows] != filler_color)// если наш соседниц <= пороговой якрокости и != цвету заливки тогда красим наш пиксель и
																				//добавляем в очередь кординаты соседнего пикселя и так для всех соседних пикселей
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
	auto rows = image.rows;				// Количество строк в изоброжении
	auto columns = image.cols;			// Количество столбцов в изоброжении
	auto image_data = image.data;		// Указатель на основые данные изоброжения которая хранится в виде массива байт
	std::queue<Pixel> queue;			// Обявляем очередь
	queue.push(pixel_pos);				// Добовляем в очередь начальную точку
	Pixel temp{ 0,0 };					// Создаем временный обект для хранения промежуточных точек
	while (!queue.empty())				// Пока очередь не пуста
	{
		temp = queue.front();			// оброщяемся к первому элементу очереди и сохроняем ее во временной переменной
		queue.pop();					// удаляем элемент из очереди

		if (temp.x + 1 < rows)			// Проверяем границы изоброжения
		{
			if (image_data[(temp.x + 1) + temp.y * rows] <= pixel_brightness &&
				image_data[(temp.x + 1) + temp.y * rows] != filler_color)// если наш соседниц <= пороговой якрокости и != цвету заливки тогда красим наш пиксель и
																		//добавляем в очередь кординаты соседнего пикселя и так для всех соседних пикселей
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




