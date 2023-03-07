#include "ImageEncryptor.h"

ImageEncryptor::ImageEncryptor()
{
	inputImg = NULL;
	imgSize[0] = imgSize[1] = imgNumChannel = -1;
}

bool ImageEncryptor::loadImage(char* _fileName)
{
	if (_fileName == NULL)
		return false;

	inputImg = imread(_fileName, IMREAD_COLOR);
	if (inputImg.empty()) {
		printf("Fail to open the image: %s\n", _fileName);
		return false;
	}

	imgSize[WIDTH] = inputImg.cols;
	imgSize[HEIGHT] = inputImg.rows;
	imgNumChannel = inputImg.channels();

#ifdef _DEBUG
	printf("LoadImg: %d x %d, %d channels\n"
		, imgSize[WIDTH], imgSize[HEIGHT], imgNumChannel);
#endif

	return true;
}

void ImageEncryptor::getSize(int* _size)
{
	_size[WIDTH] = imgSize[WIDTH];
	_size[HEIGHT] = imgSize[HEIGHT];
}

void ImageEncryptor::getSize(int& _width, int& _height)
{
	_width = imgSize[WIDTH];
	_height = imgSize[HEIGHT];
}

bool ImageEncryptor::encrytImg(char* _fileName, uchar** _outputA, uchar** _outputB)
{
	if (!loadImage(_fileName))
		return false;

	// memory allocation
	*_outputA = new uchar[dataSize()];	
	*_outputB = new uchar[dataSize()];
	memset(*_outputA, 0, sizeof(uchar) * dataSize());
	memset(*_outputB, 0, sizeof(uchar) * dataSize());

	// Encryption
	uchar* inputData = inputImg.data;
	//#pragma omp parallel for
	for (int i = 0; i < dataSize(); i++) {
		(*_outputA)[i] = (uchar)(inputData[i] / _ENCRPYT_CODE);
		(*_outputB)[i] = (uchar)(inputData[i] % _ENCRPYT_CODE);
	}

	return true;
}

bool ImageEncryptor::encrytImg(char* _inputFileName, char* _outputFileName)
{
	uchar* outputA = NULL, * outputB = NULL;
	if (!encrytImg(_inputFileName, &outputA, &outputB))
		return false;

	string fileNameA = _outputFileName;
	fileNameA += "A.png";
	string fileNameB = _outputFileName;
	fileNameB += "B.png";

	Mat imgA = Mat(imgSize[HEIGHT], imgSize[WIDTH], CV_8UC3, outputA);
	Mat imgB = Mat(imgSize[HEIGHT], imgSize[WIDTH], CV_8UC3, outputB);

	imwrite(fileNameA, imgA);
	imwrite(fileNameB, imgB);

	return true;
}

bool ImageEncryptor::encrytImg2Bin(char* _inputFileName, char* _outputFileName)
{
	uchar* outputA = NULL, * outputB = NULL;
	if (!encrytImg(_inputFileName, &outputA, &outputB))
		return false;

	string fileNameA = _outputFileName;
	fileNameA += "A.out";
	string fileNameB = _outputFileName;
	fileNameB += "B.out";

	FILE* fp = NULL;

	fopen_s(&fp, fileNameA.c_str(), "wb");
	if (!fp) {
		printf("Fail to write %s\n", fileNameA);
		return false;
	}
	fwrite(outputA, sizeof(uchar), dataSize(), fp);
	fclose(fp);
	printf("The first encrption file(1/2) was written to %s\n", fileNameA.c_str());

	fopen_s(&fp, fileNameB.c_str(), "wb");
	if (!fp) {
		printf("Fail to open %s\n", fileNameB);
		return false;
	}
	fwrite(outputB, sizeof(uchar), dataSize(), fp);
	fclose(fp);
	printf("The second encrption file(2/2) was written to %s\n", fileNameB.c_str());

	printf("Image size = %d x %d\n", imgSize[WIDTH], imgSize[HEIGHT]);

	return true;
}
