#include "ImageLoader.h"

ImageLoader::ImageLoader() {
	numChannels = -1;
	memset(size, 0, sizeof(int) * 2);
}

uchar* ImageLoader::getImg(void)
{
	if (img.empty())
		return NULL;
	return img.data;
}

void ImageLoader::getSize(int _dst[2])
{
	_dst[WIDTH] = size[WIDTH];
	_dst[HEIGHT] = size[HEIGHT];
}

uchar* ImageLoader::loadImg(char* _fileName)
{
	if (_fileName == NULL)
		return NULL;

	img = imread(_fileName, IMREAD_COLOR);
	if (img.empty()) {
		printf("Fail to open the image: %s\n", _fileName);
		return NULL;
	}

	size[WIDTH] = img.cols;
	size[HEIGHT] = img.rows;
	numChannels = img.channels();

	return img.data;
}
