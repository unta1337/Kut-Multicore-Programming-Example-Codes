/**
@author Duksu Kim
@date 2020/03/22
*/
#ifndef __IMAGE_LOADER_H
#define __IMAGE_LOADER_H

#include "common.h"
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;

class ImageLoader
{
public:
	ImageLoader();
	~ImageLoader() {}

private:
	Mat img;
	int size[2];
	int numChannels;

public:
	inline int dataSize(void) { return size[0] * size[1] * numChannels; }

	uchar* getImg(void);
	int getWidth(void) { return size[WIDTH]; }
	int getHeight(void) { return size[HEIGHT]; }

	void getSize(int _dst[2]);
	inline int getNumChannels(void) {return numChannels;}
	uchar* loadImg(char* _fileName);
};

#endif