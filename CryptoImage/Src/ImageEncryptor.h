/**
@author Duksu Kim
@date 2020/03/22
*/
#ifndef __IMAGE_ENCRYPTOR_H
#define __IMAGE_ENCRYPTOR_H

#include "common.h"
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;

#define _ENCRPYT_CODE 50

class ImageEncryptor
{
public:
	ImageEncryptor();
	~ImageEncryptor() {}

private:
	Mat inputImg;
	int imgSize[2];
	int imgNumChannel;

	inline int dataSize(void) { return imgSize[0] * imgSize[1] * imgNumChannel; }
	bool loadImage(char* _fileName);

public:
	void getSize(int* _size);
	void getSize(int& _width, int& _height);

	bool encrytImg(char* _fileName, uchar **_outputA, uchar **_outputB);
	bool encrytImg(char* _inputFileName, char* _outputFileName);
	bool encrytImg2Bin(char* _inputFileName, char* _outputFileName);
};

#endif