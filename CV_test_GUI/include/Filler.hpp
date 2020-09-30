#pragma once
#include<stdint.h>
#include<queue>
/*
��������� ������������� DEPRECATED_FILL_METHODS ������� �� ��������� ���������� 
��� ������������� ��� �������� ��� ���������� ���������� ������ ������ ������� � ������� ����������� opencv
� ������������ ����� ������� ��������� �������������� � �������� �� �.�. � ������� ���� ���������, ��� � ���-
����� ������ ���������� ������������ ������ STL ��� ���� ���������� � ����� ��� ���� ������������ ������ ������ ����
�������.
*/

// #define DEPRECATED_FILL_METHODS 


#ifdef DEPRECATED_FILL_METHODS
#include<opencv2/core.hpp>
#endif // DEPRECATED_FILL_METHODS


using int32 = int32_t; // ������ ���������� ��� ������������� �����
using uint32 = uint32_t;
using uchar = unsigned char;
struct Pixel // ��������� ��� �������� ��������� ������� 
{
	uint32 x, y;
};

class Filler
{
public:

	/*
	����������� ������ ��������� ������� � ���������
	���������� ��������� �� ���������� ������ �.�. �� ������ ���� �����������
	*/
	uchar* fill_image 
	(
		uchar* image_data,// ��������� �� ������ �.�. �� ������ ���� �����������
		const Pixel& pos, // ���������� ������������ �������
		uint32 pixel_brightness, // ������� �������
		uint32 filler_color // �� ����� ���� ����� �������������
	) noexcept; // ������� �� ������� ���������� �������� � ��� ����� ��� �� ���������� ���� ����� �������������� �������

	/*
	����������� ������ ��������� ������� � ���������
	���������� ��������� �� ���������� ������ �.�. �� ������ ���� �����������
	*/
	uchar* fill_image_rec_ver
	(
		uchar* image_data, // ��������� �� ������ �.�. �� ������ ���� �����������
		int x, // ���������� x ������������ �������
		int y, //  ���������� y ������������ �������
		uint32 pixel_brightness, // ������� �������
		uint32 filler_color		 // �� ����� ���� ����� �������������
	);

	/*
	����� ������ ����� ��� ����� int32 filler_rows,	
	int32 filler_columns � ������� �������� ������ � ������ �����������
	��� ����� ����� �� ����� �� ������� �����������
	*/

	void set_rows_columns
	(
		int32 rows, // ���������� �����
		int32 columns // ���������� ��������
	);

	

private:
	int32 filler_rows;// ������  
	int32 filler_columns; // �������


#ifdef DEPRECATED_FILL_METHODS
	static cv::Mat fill_image // ��������� ������� ������� � ��������� � �������������� ������� 
	(
		cv::Mat image,					// �������� �������� ���������� �  ���� ������ ������ Mat �� ���������� opencv 4.1.1
		const Pixel& pixel_pos,			// ��������� ��� �������� ��������� x , y
		const uint32& pixel_brightness, // �������� ������� �������  
		const uint32& filler_color      // ���� �������
	) noexcept; // ������� �� ������� ���������� �������� � ��� ����� ��� �� ���������� ���� ����� �������������� �������


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

