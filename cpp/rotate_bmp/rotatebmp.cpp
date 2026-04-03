#include <fstream>
#include <string>
#include <cstdint>
#include <cstdlib>
using namespace std;

#pragma pack(1)

typedef uint16_t WORD;
typedef uint32_t DWORD;
typedef int32_t LONG;
typedef uint8_t BYTE;

struct tagBITMAPFILEHEADER {
  WORD Type;      // 位图文件的类型，必须为“BM”
  DWORD Size;     // 位图文件的大小，以字节为单位
  WORD Reserved1; // 位图文件保留字，必须为 0
  WORD Reserved2; // 位图文件保留字，必须为 0
  DWORD OffBits; // 位图数据的起始位置，以相对于位图文件头的偏移量表示，以字节为单位
}; // 该结构占据 14 个字节

struct tagBITMAPINFOHEADER {
  DWORD Size; // 本结构所占用字节数
  LONG Width; // 位图的宽度，以像素为单位
  LONG Height; // 位图的高度，以像素为单位
  WORD Planes; // 目标设备的平面数不清，必须为 1
  WORD BitCount; // 每个像素所需的位数，必须是 1(双色), 4(16 色)，8(256 色)或 24(真彩色)之一
  DWORD Compression; // 位图压缩类型，必须是 0(不压缩),1(BI_RLE8 压缩类型)或 2(BI_RLE4压缩类型)之一
  DWORD SizeImage; // 位图的大小，以字节为单位
  LONG XPelsPerMeter; // 位图水平分辨率，每米像素数
  LONG YPelsPerMeter; // 位图垂直分辨率，每米像素数
  DWORD ClrUsed; // 位图实际使用的颜色表中的颜色数
  DWORD ClrImportant; // 位图显示过程中重要的颜色数
}; // 该结构占据 40 个字节

tagBITMAPFILEHEADER fileForheader; 
tagBITMAPINFOHEADER infoHeader;

// 计算扫描线字节数（4 字节对齐）
int calculateBytesPerLine(int width) {
  return ((width * 3 + 3) / 4) * 4;
}

// 读取 BMP 文件
bool readBMPFile(const char* filename, tagBITMAPFILEHEADER& fileHeader,
                 tagBITMAPINFOHEADER& infoHeader, BYTE*& pixelData,
                 int& width, int& height) {
  ifstream file(filename, ios::binary);
  if (!file.is_open()) {
    return false;
  }
  
  file.read((char*)&fileHeader, sizeof(tagBITMAPFILEHEADER));
  file.read((char*)&infoHeader, sizeof(tagBITMAPINFOHEADER));
  if (!file) {
    return false;
  }

  width = abs((int)infoHeader.Width);
  height = abs((int)infoHeader.Height);
  if (width <= 0 || height <= 0) {
    return false;
  }
  
  int bytesPerLine = calculateBytesPerLine(width);
  int totalPixelBytes = bytesPerLine * height;
  
  pixelData = new BYTE[totalPixelBytes];
  
  file.seekg(fileHeader.OffBits, ios::beg);
  file.read((char*)pixelData, totalPixelBytes);
  if (!file) {
    delete[] pixelData;
    pixelData = nullptr;
    return false;
  }
  
  file.close();
  return true;
}

// 旋转 BMP 数据（顺时针 90 度）
void rotateBMPData(const BYTE* srcPixels, int srcW, int srcH,
                   BYTE*& dstPixels) {
  int srcBytesPerLine = calculateBytesPerLine(srcW);
  int dstW = srcH;
  int dstH = srcW;
  int dstBytesPerLine = calculateBytesPerLine(dstW);
  
  int totalDstBytes = dstBytesPerLine * dstH;
  dstPixels = new BYTE[totalDstBytes];
  
  for (int i = 0; i < totalDstBytes; i++) {
    dstPixels[i] = 0;
  }
  
  // (x, y) -> (y, dstH - 1 - x) 顺时针 90 度
  for (int y = 0; y < srcH; y++) {
    for (int x = 0; x < srcW; x++) {
      int srcOffset = y * srcBytesPerLine + x * 3;
      int newX = y;
      int newY = dstH - 1 - x;
      int dstOffset = newY * dstBytesPerLine + newX * 3;
      
      dstPixels[dstOffset] = srcPixels[srcOffset];
      dstPixels[dstOffset + 1] = srcPixels[srcOffset + 1];
      dstPixels[dstOffset + 2] = srcPixels[srcOffset + 2];
    }
  }
}

// 写入 BMP 文件
void writeBMPFile(const char* filename, tagBITMAPFILEHEADER fileHeader,
                  tagBITMAPINFOHEADER infoHeader, const BYTE* pixelData) {
  int bytesPerLine = calculateBytesPerLine(infoHeader.Width);
  infoHeader.SizeImage = bytesPerLine * infoHeader.Height;
  fileHeader.Size = fileHeader.OffBits + infoHeader.SizeImage;
  
  ofstream file(filename, ios::binary);
  
  file.write((char*)&fileHeader, sizeof(tagBITMAPFILEHEADER));
  file.write((char*)&infoHeader, sizeof(tagBITMAPINFOHEADER));
  file.write((char*)pixelData, infoHeader.SizeImage);
  
  file.close();
}

// 从文件名生成输出文件名（在扩展名前插入 _rotated）
string generateOutputFilename(const char* inputFilename) {
  string filename = inputFilename;
  size_t dotPos = filename.rfind('.');
  if (dotPos != string::npos) {
    filename.insert(dotPos, "_rotated");
  } else {
    filename += "_rotated";
  }
  return filename;
}

int main(int argc, char* argv[]) {
  if (argc < 2) {
    return 1;
  }
  
  const char* inputFile = argv[1];
  string outputFile = generateOutputFilename(inputFile);
  
  BYTE* srcPixels = nullptr;
  int srcW = 0;
  int srcH = 0;
  if (!readBMPFile(inputFile, fileForheader, infoHeader, srcPixels, srcW, srcH)) {
    return 1;
  }
  
  BYTE* dstPixels = nullptr;
  rotateBMPData(srcPixels, srcW, srcH, dstPixels);

  infoHeader.Width = srcH;
  infoHeader.Height = srcW;
  
  writeBMPFile(outputFile.c_str(), fileForheader, infoHeader, dstPixels);
  
  delete[] srcPixels;
  delete[] dstPixels;

  return 0;
}

