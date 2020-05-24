#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>

int main()
{
	//헤더 구조체 선언
	BITMAPFILEHEADER fileHeader;
	BITMAPINFOHEADER fileInfo;

	//파일 선언
	FILE* bitMap1;
	FILE* bitMap2;
	FILE* resultFile;

	//헤더 크기
	DWORD headerSize = sizeof(BITMAPFILEHEADER);
	DWORD infoSize = sizeof(BITMAPINFOHEADER);

	//샘플 파일 개방
	bitMap1 = fopen("sample.bmp", "rb");
	if (bitMap1 == NULL)
	{
		puts("sample 파일을 열 수 없음");
		return 0;
	}

	bitMap2 = fopen("sample2.bmp", "rb");
	if (bitMap2 == NULL)
	{
		puts("sample2 파일을 열 수 없음");
		return 0;
	}

	//샘플 파일 읽기
	fread(&fileHeader, headerSize, 1, bitMap1);
	fread(&fileHeader, headerSize, 1, bitMap2);
	fread(&fileInfo, infoSize, 1, bitMap1);
	fread(&fileInfo, infoSize, 1, bitMap2);

	//이미지 해상도
	DWORD pixelCnt = fileInfo.biWidth * fileInfo.biHeight;

	//이미지 크기
	DWORD imageSize = (fileInfo.biBitCount / 8) * pixelCnt;

	//이미지 버퍼
	DWORD* pImageBuffer1 = (DWORD*)malloc(imageSize);
	DWORD* pImageBuffer2 = (DWORD*)malloc(imageSize);
	
	//이미지 데이터 읽기
	fread(pImageBuffer1, imageSize, 1, bitMap1);
	fread(pImageBuffer2, imageSize, 1, bitMap2);

	//목적 파일 개방
	resultFile = fopen("bitMap.bmp", "wb");
	if (resultFile == NULL)
	{
		puts("bitMap 파일을 열 수 없음");
		return 0;
	}

	//이미지 출력 버퍼 생성
	DWORD* pResultBuffer = (DWORD*)malloc(imageSize);

	//이미지 합성
	for (DWORD i = 0; i < pixelCnt; i++)
	{
		BYTE R1 = (pImageBuffer1[i] & 0xff000000) >> 24;
		BYTE G1 = (pImageBuffer1[i] & 0x00ff0000) >> 16;
		BYTE B1 = (pImageBuffer1[i] & 0x0000ff00) >> 8;
		BYTE A1 = (pImageBuffer1[i] & 0x000000ff);

		BYTE R2 = (pImageBuffer2[i] & 0xff000000) >> 24;
		BYTE G2 = (pImageBuffer2[i] & 0x00ff0000) >> 16;
		BYTE B2 = (pImageBuffer2[i] & 0x0000ff00) >> 8;
		BYTE A2 = (pImageBuffer2[i] & 0x000000ff);

		pResultBuffer[i] = ((R1 + R2) / 2 << 24) | ((G1 + G2) / 2 << 16) |
			((B1 + B2) / 2 << 8) | ((A1 + A2) / 2);
	}

	//목적 파일로 데이터 출력
	fwrite(&fileHeader, headerSize, 1, resultFile);
	fwrite(&fileInfo, infoSize, 1, resultFile);
	fwrite(pResultBuffer, imageSize, 1, resultFile);

	//동적할당 해제
	free(pImageBuffer1);
	free(pImageBuffer2);
	free(pResultBuffer);

	//파일 닫기
	fclose(bitMap1);
	fclose(bitMap2);
	fclose(resultFile);

	return 0;
}
