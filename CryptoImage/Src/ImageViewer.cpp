#include "ImageViewer.h"

bool ImageViewer::convertToMat(uchar* _img, int _width, int _height)
{
	if (_img == NULL) return false;
	if (_width < 1 || _height < 1) return false;

	img = Mat(_height, _width, CV_8UC3, _img);
	if (img.empty()) {
		printf("Fail to convert the image\n");
		return false;
	}

	return true;
}

bool ImageViewer::ShowImage(ImageLoader* _img, char* _title)
{
	return ShowImage(_img->getImg() , _img->getWidth(), _img->getHeight(), _title);
}

bool ImageViewer::ShowImage(uchar* _img, int _width, int _height, char* _title)
{
	if (!convertToMat(_img, _width, _height))
		return false;

	string windowName;
	if (_title == NULL)
		windowName = "Display";
	else
		windowName = _title;

//	namedWindow(windowName, WINDOW_NORMAL);
//	resizeWindow(windowName, WND_PROP_FULLSCREEN, WINDOW_FULLSCREEN);
	imshow(windowName, img);
	waitKey();	
	return true;
}

bool ImageViewer::writeImage(uchar* _img, int _width, int _height, char* _fileName)
{
	if (!convertToMat(_img, _width, _height))
		return false;
	imwrite(_fileName, img);

	return false;
}
