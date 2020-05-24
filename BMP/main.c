#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>

int main()
{
	//��� ����ü ����
	BITMAPFILEHEADER fileHeader;
	BITMAPINFOHEADER fileInfo;

	//���� ����
	FILE* bitMap1;
	FILE* bitMap2;
	FILE* resultFile;

	//��� ũ��
	DWORD headerSize = sizeof(BITMAPFILEHEADER);
	DWORD infoSize = sizeof(BITMAPINFOHEADER);

	//���� ���� ����
	bitMap1 = fopen("sample.bmp", "rb");
	if (bitMap1 == NULL)
	{
		puts("sample ������ �� �� ����");
		return 0;
	}

	bitMap2 = fopen("sample2.bmp", "rb");
	if (bitMap2 == NULL)
	{
		puts("sample2 ������ �� �� ����");
		return 0;
	}

	//���� ���� �б�
	fread(&fileHeader, headerSize, 1, bitMap1);
	fread(&fileHeader, headerSize, 1, bitMap2);
	fread(&fileInfo, infoSize, 1, bitMap1);
	fread(&fileInfo, infoSize, 1, bitMap2);

	//�̹��� �ػ�
	DWORD pixelCnt = fileInfo.biWidth * fileInfo.biHeight;

	//�̹��� ũ��
	DWORD imageSize = (fileInfo.biBitCount / 8) * pixelCnt;

	//�̹��� ����
	DWORD* pImageBuffer1 = (DWORD*)malloc(imageSize);
	DWORD* pImageBuffer2 = (DWORD*)malloc(imageSize);
	
	//�̹��� ������ �б�
	fread(pImageBuffer1, imageSize, 1, bitMap1);
	fread(pImageBuffer2, imageSize, 1, bitMap2);

	//���� ���� ����
	resultFile = fopen("bitMap.bmp", "wb");
	if (resultFile == NULL)
	{
		puts("bitMap ������ �� �� ����");
		return 0;
	}

	//�̹��� ��� ���� ����
	DWORD* pResultBuffer = (DWORD*)malloc(imageSize);

	//�̹��� �ռ�
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

	//���� ���Ϸ� ������ ���
	fwrite(&fileHeader, headerSize, 1, resultFile);
	fwrite(&fileInfo, infoSize, 1, resultFile);
	fwrite(pResultBuffer, imageSize, 1, resultFile);

	//�����Ҵ� ����
	free(pImageBuffer1);
	free(pImageBuffer2);
	free(pResultBuffer);

	//���� �ݱ�
	fclose(bitMap1);
	fclose(bitMap2);
	fclose(resultFile);

	return 0;
}
