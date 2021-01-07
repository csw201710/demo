
void testCaptureScreen() {
	int ret;
	HDC hdc = GetDC(0);
	HDC comHdc = CreateCompatibleDC(hdc);
	int width = GetSystemMetrics(SM_CXSCREEN);
	int height = GetSystemMetrics(SM_CYSCREEN);

	HBITMAP hBitmap = CreateCompatibleBitmap(hdc, width, height);
	SelectObject(comHdc, hBitmap);
	if (!BitBlt(comHdc, 0, 0, width, height, hdc, 0, 0, SRCCOPY)) {
		printf("BitBlt failed\n");
		return;
	}

	BITMAPINFOHEADER bmiHeader = {0};

	//本结构大小
	bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	//宽度
	bmiHeader.biWidth = width;
	//高度
	bmiHeader.biHeight = height;
	// TODO
	bmiHeader.biPlanes = 1;
	// 一个像素用多少位表示
	bmiHeader.biBitCount = 16;
	//压缩方式，0表示不压缩，1表示RLE8压缩，2表示RLE4压缩，3表示每个像素值由指定的掩码决定
	bmiHeader.biCompression = 0;
	//BMP图像数据大小，必须是4的倍数，图像数据大小不是4的倍数时用0填充补足
	bmiHeader.biSizeImage = 0;
	//水平分辨率
	bmiHeader.biXPelsPerMeter = 0;
	//垂直分辨率
	bmiHeader.biYPelsPerMeter = 0;
	//BMP图像使用的颜色，0表示使用全部颜色，对于256色位图来说，此值为100h=256
	bmiHeader.biClrUsed = 0;
	//重要的颜色数，此值为0时所有颜色都重要，对于使用调色板的BMP图像来说，当显卡不能够显示所有颜色时，此值将辅助驱动程序显示颜色
	bmiHeader.biClrImportant = 0;

	DWORD dwBmpSize = ((width * bmiHeader.biBitCount + 31) / 32) * 4 * height;

	DWORD dwSizeofDIB = dwBmpSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);


	BITMAPFILEHEADER bmfHeader = {0};
	
	// 标志
	bmfHeader.bfType = ((WORD)('M' << 8) | 'B');;
	// BMP图像大小
	bmfHeader.bfSize;
	// 保留
	bmfHeader.bfReserved1 = 0;
	bmfHeader.bfReserved2 = 0;
	//bitmap数据偏移
	bmfHeader.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER) + (DWORD) sizeof(BITMAPINFOHEADER);

	char * lpBitmap =(char*) calloc(1, dwBmpSize );
	ret = GetDIBits(comHdc, hBitmap, 0, height, lpBitmap, (BITMAPINFO*)&bmiHeader, DIB_RGB_COLORS);
	printf("GetDIBits ret:%d\n", ret);
	std::ofstream of("tt.bmp", std::ios_base::binary);
	of.write((char*)&bmfHeader, sizeof(BITMAPFILEHEADER));
	of.write((char*)&bmiHeader, sizeof(BITMAPINFOHEADER));
	of.write(lpBitmap, dwBmpSize);
	of.close();

	//TODO 释放数据
}




