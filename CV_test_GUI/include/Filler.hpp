#pragma once
#include<stdint.h>
#include<queue>
/*
Директива препроцессора DEPRECATED_FILL_METHODS которая по умолчанию отключенна 
она пердназначана для вклчения или отключения компиляции других версии функции в которых используеця opencv
и предосовляют более простой интерфейс взаимодействия с методами но т.к. в условии было написанно, что в фун-
кциях залвки разрешенно использовать только STL они были отключенны в место них были разработанны другие версии этих
функции.
*/

// #define DEPRECATED_FILL_METHODS 


#ifdef DEPRECATED_FILL_METHODS
#include<opencv2/core.hpp>
#endif // DEPRECATED_FILL_METHODS


using int32 = int32_t; // обявим псевдонымы для целочисленных типов
using uint32 = uint32_t;
using uchar = unsigned char;
struct Pixel // Структура для хронения координат пикселя 
{
	uint32 x, y;
};

class Filler
{
public:

	/*
	Итеративная версия алгоритма заливки с затравкой
	возврощает указатель на измененные данные т.е. на массив байт изоброжения
	*/
	uchar* fill_image 
	(
		uchar* image_data,// указатель на данные т.е. на массив байт изоброжения
		const Pixel& pos, // координаты затравочного пикселя
		uint32 pixel_brightness, // яроксть пикселя
		uint32 filler_color // на какой цвет будет закрашиваться
	) noexcept; // функция не бросает исключения сделанно с той целью что бы компилятор смог лучше оптимизировать функцию

	/*
	рекурсивная версия алгоритма заливки с затравкой
	возврощает указатель на измененные данные т.е. на массив байт изоброжения
	*/
	uchar* fill_image_rec_ver
	(
		uchar* image_data, // указатель на данные т.е. на массив байт изоброжения
		int x, // координата x затравочного пикселя
		int y, //  координата y затравочного пикселя
		uint32 pixel_brightness, // яроксть пикселя
		uint32 filler_color		 // на какой цвет будет закрашиваться
	);

	/*
	метод сеттер метод для полей int32 filler_rows,	
	int32 filler_columns в которой хронятся высота и ширина изоброжения
	они нужны чтобы не выйти за пределы изоброжения
	*/

	void set_rows_columns
	(
		int32 rows, // Количество строк
		int32 columns // Количество столбцов
	);

	

private:
	int32 filler_rows;// Строки  
	int32 filler_columns; // Столбцы


#ifdef DEPRECATED_FILL_METHODS
	static cv::Mat fill_image // Сигнатура функции заливки с затравкой с использыванием очереди 
	(
		cv::Mat image,					// Исходная картинка передается в  виде обекта класса Mat из библиотеки opencv 4.1.1
		const Pixel& pixel_pos,			// Структура для хронения кординать x , y
		const uint32& pixel_brightness, // Порговая яркость пикселя  
		const uint32& filler_color      // Цвет заливки
	) noexcept; // функция не бросает исключения сделанно с той целью что бы компилятор смог лучше оптимизировать функцию


	static cv::Mat fill_image_rec_ver
	(
		cv::Mat image,
		int x,
		int y,
		uint32 pixel_brightness,
		uint32 filler_color,
		uchar* image_data,
		int rows,
		int columns
	);

	static void fill_image_rec_ver
	(
		uchar* image_data,
		int x,
		int y,
		uint32 pixel_brightness,
		uint32 filler_color,
		int rows,
		int columns
	);

	
#endif // DEBUG

	

	
};

