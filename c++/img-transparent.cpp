int ImgTransparent(Bitmap& bitmap){
  // 转换为 32 位像素  a r g b
  Bitmap bitmap2 = bitmap.ConvertFormat(foxit::common::Bitmap::DIBFormat::e_DIBArgb,0);
  int bitmap_height = bitmap2.GetHeight ();
  int bitmap_width = bitmap2.GetWidth  ();
  unsigned char * p = (unsigned char*)bitmap2.GetBuffer();
  printf("bitmap_width : %d , bitmap_height : %d", bitmap_width,bitmap_height);
  int Stride = bitmap_width * 4;
  for(int i=0;i<bitmap_width;i++){
    for(int j=0;j<bitmap_width;j++){
      	int b = p[i*Stride+(j*4)];
							int g = p[i*Stride+(j*4)+1];
							int r = p[i*Stride+(j*4)+2];
							int a = p[i*Stride+(j*4)+3];
							if(r == 0xff && g == 0xff && b == 0xff){
							  *(p + i*Stride+(j*4)+3) = 0x00;
							}
    }
  }
  return 0;
  
}


