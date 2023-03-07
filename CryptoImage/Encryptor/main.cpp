#include "../Src/common.h"
#include "../Src/ImageEncryptor.h"

int main(int argc, char** argv)
{
	if (argc < 3) {
		printf("It requires at least two arguments\n");
		printf("Usage: Extuction_file InputFileName OutputFilePrefix\n");
		return -1;
	}

	ImageEncryptor en;
	en.encrytImg2Bin(argv[1], argv[2]);
}