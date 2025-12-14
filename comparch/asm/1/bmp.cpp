#include <windows.h>
#include <iostream>
#include <string>

int main()
{
	//std::string sFileName;
	constexpr size_t paletteWidth = 2, paletteMultiplier = sizeof(RGBQUAD);
	BITMAPFILEHEADER bmpFileHeader;
	BITMAPINFOHEADER bmpInfoHeader;
	int Width = 0, Height = 0;
	RGBQUAD Palette[256]{}, OldPalette[256]{};
	BYTE *inBuf = nullptr;
	BYTE *outBuf = nullptr;
	HANDLE hInputFile, hOutFile;
	DWORD RW = 0, WRITE = 0;
	const char * im = "C:\\Users\\georv\\cat.bmp";
	int threshold = 100;
	// std::cout << "Enter the full name, please: ";
	// std::cin >> sFileName;

	hInputFile = CreateFile(im, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hInputFile == INVALID_HANDLE_VALUE)
	{
		std::cerr << "INVALID HANDLE\n";
		return 1;
	}

	// Считываем инфу
	ReadFile(hInputFile, &bmpFileHeader, sizeof(bmpFileHeader), &RW, NULL);
	ReadFile(hInputFile, &bmpInfoHeader, sizeof(bmpInfoHeader), &RW, NULL);
	ReadFile(hInputFile, &OldPalette, sizeof(OldPalette), &RW, NULL);

	hOutFile = CreateFile("Result.bmp", GENERIC_WRITE, 0, NULL, CREATE_NEW, 0, NULL);
	if (hOutFile == INVALID_HANDLE_VALUE)
	{
		CloseHandle (hInputFile);
		std::cerr << "INVALID HANDLE\n";
		return 1;
	}

	// Установим указатель на начало растра
	SetFilePointer (hInputFile, bmpFileHeader.bfOffBits, NULL, FILE_BEGIN);
	Width = bmpInfoHeader.biWidth;
	Height = bmpInfoHeader.biHeight;

	// Выделим память
	inBuf = new BYTE[Width]{};
	outBuf = new BYTE[Width / 8]{};

    // Заполним заголовки
    bmpFileHeader.bfOffBits = sizeof (bmpFileHeader) + sizeof (bmpInfoHeader) + 1024;
    bmpInfoHeader.biBitCount = 1;
    bmpFileHeader.bfSize = bmpFileHeader.bfOffBits + (Width * Height / 8) + ((Height * Width / 8) % 4);

	//RW = 0;
    // Запишем заголовки
    WriteFile (hOutFile, &bmpFileHeader, sizeof(bmpFileHeader), &WRITE, NULL);
    WriteFile (hOutFile, &bmpInfoHeader, sizeof(bmpInfoHeader), &WRITE, NULL);

	// Палитра черно-белая
	Palette[0].rgbBlue = 0;
	Palette[0].rgbGreen = 0;
	Palette[0].rgbRed = 0;
	Palette[1].rgbBlue = 255;
	Palette[1].rgbGreen = 255;
	Palette[1].rgbRed = 255;

	WriteFile (hOutFile, Palette, paletteMultiplier * 256, &WRITE, NULL);
  // Начнем преобразовывать
  for (int i = 0; i < Height; ++i)
  {
    ReadFile (hInputFile, inBuf, sizeof(BYTE) * Width, &RW, NULL);
		int widthCount = 0;
  	for (int j = 0; j < Width / 8; ++j)//widthCount < Width; ++j)
  	{
		  outBuf[j] = 0;
  		// unsigned char k = 128;
  		// for (int n = 0; n < 8; n++, ++widthCount)
  		// {
  		// 	auto temp = OldPalette[inBuf[widthCount]];
  		// 	if ((temp.rgbRed + temp.rgbBlue + temp.rgbGreen) / 3 > threshold)
  		// 		outBuf[j] = outBuf[j] | k;
  		// 	k /= 2;
  		// }
  	}
    WriteFile (hOutFile, outBuf, sizeof(BYTE) * Width / 8, &WRITE, NULL);

    // Пишем мусор для выравнивания
    WriteFile (hOutFile, Palette, (Width) % 4, &WRITE, NULL);
    SetFilePointer (hInputFile, Width % 4, NULL, FILE_CURRENT);
	}

    delete[] inBuf;
    delete[] outBuf;
    CloseHandle (hInputFile);
    CloseHandle (hOutFile);

	std::cout << "Updating has come to the end successfully!";
	//system("pause");
}
