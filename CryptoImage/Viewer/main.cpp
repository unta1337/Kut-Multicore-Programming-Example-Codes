#include "../Src/common.h"
#include "../Src/ImageViewer.h"
#include "../Src/ImageLoader.h"

int main(int argc, char** argv)
{
	if (argc < 3) {
		printf("It requires at least three arguments\n");
		printf("Usage: Extuction_file dataFileName Width Height OutputFileName\n");
		return -1;
	}

	int width = atoi(argv[2]);
	int height = atoi(argv[3]);
	int dataSize = width * height * 3;

	uchar* data = new uchar[dataSize];
	memset(data, 0, sizeof(uchar) * dataSize);

	FILE* fp;
	fopen_s(&fp, argv[1], "rb");
	if (!fp) {
		printf("Fail to open %s\n", argv[1]);
		return -1;
	}
	fread(data, sizeof(uchar), dataSize, fp);
	fclose(fp);

	ImageViewer viewer;
	viewer.ShowImage(data, width, height, "Converted Image");

	if (argc > 3) {
		viewer.writeImage(data, width, height, argv[4]);
		printf("The conveted result was written to %s\n", argv[4]);
	}
}