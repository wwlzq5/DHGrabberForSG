/************************************************************************/
/* Copyright (c) 2005, �й���㼯�ű���ͼ���Ӿ������ֹ�˾�Ӿ�������     
/* All rights reserved.													
/*																		
/* �ļ����ƣ� Image.cpp												
/* �ļ���ʶ�� �����ù���ƻ���
/* ժҪ�� CImage��, �Ա���һ��ͼ��ĸ����ݽṹ���з�װ, ʵ����ͼ�񿽱�,
/*    ͼ����ʾ, ͼ�����, ͼ����еȹ���
/*
/* ��ǰ�汾�� 1.0
/* ���ߣ� ��׿��
/* ������ڣ� 2005��10��10��
/************************************************************************/

#include "stdafx.h"
#include "Image.h"
#include "FileOperate.h"
//#include "SplitMerge.h"
#include "dh_ImgLibEx.h"

CImage::CImage()
{
	m_pImageBuff = NULL;	//ͼ�񻺳���
	m_lBuffSize = 0;		//ͼ�����ݴ�С

	m_lImageWidth = 0;		//ͼ����
	m_lImageHeight = 0;		//ͼ�񳤶�
	m_lImageBitCount = 0;	//����λ��
	m_lImagePixelSize = 0;	//���ش�С����λ���ֽڣ�
	m_lImageSize = 0;		//ͼ�����ݴ�С
	
	//�Լ��ڲ���
	m_pBitmapInfo = (BITMAPINFO *) new BYTE[sizeof(BITMAPINFO) + 255*sizeof(RGBQUAD)];
    m_pBitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    m_pBitmapInfo->bmiHeader.biPlanes = 1;
    m_pBitmapInfo->bmiHeader.biBitCount = 0;
    m_pBitmapInfo->bmiHeader.biCompression = BI_RGB;
    m_pBitmapInfo->bmiHeader.biSizeImage = 0;
    m_pBitmapInfo->bmiHeader.biXPelsPerMeter = 0;
    m_pBitmapInfo->bmiHeader.biYPelsPerMeter = 0;
    m_pBitmapInfo->bmiHeader.biClrUsed = 0;
    m_pBitmapInfo->bmiHeader.biClrImportant = 0;
    m_pBitmapInfo->bmiHeader.biWidth = 0 ;
    m_pBitmapInfo->bmiHeader.biHeight = 0 ;
	for (int i = 0 ; i < 256 ; i++)
	{
		m_pBitmapInfo->bmiColors[i].rgbBlue = (BYTE)i;
		m_pBitmapInfo->bmiColors[i].rgbGreen = (BYTE)i;
		m_pBitmapInfo->bmiColors[i].rgbRed = (BYTE)i;
		m_pBitmapInfo->bmiColors[i].rgbReserved = 0;
	}
	
	//������ʾ�õ�
	m_pShowBitmapInfo = (BITMAPINFO *) new BYTE[sizeof(BITMAPINFO) + 255*sizeof(RGBQUAD)];
    m_pShowBitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    m_pShowBitmapInfo->bmiHeader.biPlanes = 1;
    m_pShowBitmapInfo->bmiHeader.biBitCount = 0;
    m_pShowBitmapInfo->bmiHeader.biCompression = BI_RGB;
    m_pShowBitmapInfo->bmiHeader.biSizeImage = 0;
    m_pShowBitmapInfo->bmiHeader.biXPelsPerMeter = 0;
    m_pShowBitmapInfo->bmiHeader.biYPelsPerMeter = 0;
    m_pShowBitmapInfo->bmiHeader.biClrUsed = 0;
    m_pShowBitmapInfo->bmiHeader.biClrImportant = 0;
    m_pShowBitmapInfo->bmiHeader.biWidth = 0 ;
    m_pShowBitmapInfo->bmiHeader.biHeight = 0 ;
	for (int i = 0 ; i < 256 ; i++)
	{
		m_pShowBitmapInfo->bmiColors[i].rgbBlue = (BYTE)i;
		m_pShowBitmapInfo->bmiColors[i].rgbGreen = (BYTE)i;
		m_pShowBitmapInfo->bmiColors[i].rgbRed = (BYTE)i;
		m_pShowBitmapInfo->bmiColors[i].rgbReserved = 0;
	}
	
	//���������õ�
	m_pSaveBitmapInfo = (BITMAPINFO *) new BYTE[sizeof(BITMAPINFO) + 255*sizeof(RGBQUAD)];
    m_pSaveBitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    m_pSaveBitmapInfo->bmiHeader.biPlanes = 1;
    m_pSaveBitmapInfo->bmiHeader.biBitCount = 0;
    m_pSaveBitmapInfo->bmiHeader.biCompression = BI_RGB;
    m_pSaveBitmapInfo->bmiHeader.biSizeImage = 0;
    m_pSaveBitmapInfo->bmiHeader.biXPelsPerMeter = 0;
    m_pSaveBitmapInfo->bmiHeader.biYPelsPerMeter = 0;
    m_pSaveBitmapInfo->bmiHeader.biClrUsed = 0;
    m_pSaveBitmapInfo->bmiHeader.biClrImportant = 0;
    m_pSaveBitmapInfo->bmiHeader.biWidth = 0 ;
    m_pSaveBitmapInfo->bmiHeader.biHeight = 0 ;
	for (int i = 0 ; i < 256 ; i++)
	{
		m_pSaveBitmapInfo->bmiColors[i].rgbBlue = (BYTE)i;
		m_pSaveBitmapInfo->bmiColors[i].rgbGreen = (BYTE)i;
		m_pSaveBitmapInfo->bmiColors[i].rgbRed = (BYTE)i;
		m_pSaveBitmapInfo->bmiColors[i].rgbReserved = 0;
	}
}

CImage::~CImage()
{
	if(m_pImageBuff != NULL)
	{
		delete []m_pImageBuff;
		m_pImageBuff = NULL;
		m_lBuffSize = 0;
	}

	if (m_pBitmapInfo != NULL) 
	{
		delete []m_pBitmapInfo;
		m_pBitmapInfo = NULL;
	}
	
	if (m_pShowBitmapInfo != NULL) 
	{
		delete []m_pShowBitmapInfo;
		m_pShowBitmapInfo = NULL;
	}
	
	if (m_pSaveBitmapInfo != NULL) 
	{
		delete []m_pSaveBitmapInfo;
		m_pSaveBitmapInfo = NULL;
	}
}

void CImage::Init(long lImageBuffSize)
{
	ASSERT(lImageBuffSize >= 0);

	m_lImageWidth = 0;	//ͼ����
	m_lImageHeight = 0;	//ͼ�񳤶�
	m_lImageBitCount = 0;	//����λ��
	m_lImagePixelSize = 0;	//���ش�С����λ���ֽڣ�
	m_lImageSize = 0;	//ͼ�����ݴ�С
	
	m_pBitmapInfo->bmiHeader.biWidth = m_lImageWidth;
	m_pBitmapInfo->bmiHeader.biHeight = m_lImageHeight;
	m_pBitmapInfo->bmiHeader.biBitCount = (WORD)m_lImageBitCount;

	if(m_pImageBuff != NULL)
	{
		delete []m_pImageBuff;
		m_pImageBuff = NULL;
		m_lBuffSize = 0;
	}
	
	m_pImageBuff = new BYTE[lImageBuffSize];
	if(m_pImageBuff == NULL)
	{
		m_lBuffSize = 0;
		throw(CImageException(1, "�ڴ���䲻�ɹ�", "Init()"));
		return;
	}
	else
	{
		m_lBuffSize = lImageBuffSize;
	}
	
}

void CImage::SetImageSize(long lImageWidth, long lImageHeight, long lImageBitCount)
{
	ASSERT(lImageWidth >= 0 && lImageHeight >= 0 && lImageBitCount > 0);
	
	long lImageSize = lImageWidth * lImageHeight * ((lImageBitCount+7)/8);
	
	if(lImageSize > m_lBuffSize)
	{
		throw(CImageException(2, "�������Ϸ�", "SetImageSize()"));
		return;
	}
	
	m_lImageWidth = lImageWidth;				//ͼ����
	m_lImageHeight = lImageHeight;				//ͼ�񳤶�
	m_lImageBitCount = lImageBitCount;			//����λ��
	m_lImagePixelSize = (lImageBitCount+7)/8;	//���ش�С����λ���ֽڣ�
	m_lImageSize = lImageSize;					//ͼ�����ݴ�С
	
	m_pBitmapInfo->bmiHeader.biWidth = m_lImageWidth;
	m_pBitmapInfo->bmiHeader.biHeight = m_lImageHeight;
	m_pBitmapInfo->bmiHeader.biBitCount = (WORD)m_lImageBitCount;
}

void CImage::AcceptImage(const BYTE* pSrcImageBuff, long lSrcImageWidth, long lSrcImageHeight, long lSrcImageBitCount)
{
	ASSERT(pSrcImageBuff != NULL && lSrcImageWidth >= 0 && lSrcImageHeight >= 0 && lSrcImageBitCount > 0);

	long lImageSize = lSrcImageWidth * lSrcImageHeight * (lSrcImageBitCount/8);
	if(lImageSize > m_lBuffSize)
	{
		throw(CImageException(2, "�������Ϸ�", "AcceptImage()"));
	}
		
	memcpy(m_pImageBuff, pSrcImageBuff, lImageSize);
	m_lImageWidth = lSrcImageWidth;
	m_lImageHeight = lSrcImageHeight;
	m_lImageBitCount = lSrcImageBitCount;
	m_lImagePixelSize = ((lSrcImageBitCount+7)/8);
	m_lImageSize = lImageSize;	
		
	m_pBitmapInfo->bmiHeader.biWidth = m_lImageWidth;
	m_pBitmapInfo->bmiHeader.biHeight = m_lImageHeight;
	m_pBitmapInfo->bmiHeader.biBitCount = (WORD)m_lImageBitCount;
}

BYTE* CImage::GetImageBuffAddr()
{
	return m_pImageBuff;
}
long CImage::GetImageBuffSize()
{
	return m_lBuffSize;
}
long CImage::GetImageWidth()
{
	return m_lImageWidth;
}
long CImage::GetImageHeight()
{
	return m_lImageHeight;
}
long CImage::GetImageBitCount()
{
	return m_lImageBitCount;
}
long CImage::GetImagePixelSize()
{
	return m_lImagePixelSize;
}
long CImage::GetImageSize()
{
	return m_lImageSize;
}
const BITMAPINFO* CImage::GetBitmapInfo()
{
	return m_pBitmapInfo;
}

//void CImage::ShowMe(CDC* pDC, CRect Rect, enum e_ShowModeFlip ShowModeFlip, enum e_ShowModeStretch ShowModeStretch)
void CImage::ShowDIBImage(CDC* pDC, CRect Rect, e_ShowModeFlip ShowModeFlip, e_ShowModeStretch ShowModeStretch,
		const BYTE* pImageBuff, long lImageWidth, long lImageHeight, long lImageBitCount)
{
	int XDest = 0;
	int YDest = 0;
	int nDestWidth = 0;
	int nDestHeight = 0;

	ASSERT(pImageBuff != NULL && lImageWidth >= 0 && lImageHeight >= 0 && lImageBitCount > 0);

	m_csShowBitmapInfoSection.Lock();
	
	m_pShowBitmapInfo->bmiHeader.biWidth = lImageWidth;
	m_pShowBitmapInfo->bmiHeader.biHeight = lImageHeight;
	m_pShowBitmapInfo->bmiHeader.biBitCount = (WORD)lImageBitCount;

	if(ShowModeStretch == NoStretch)
	{
		nDestWidth = lImageWidth;
		nDestHeight = lImageHeight;
	}
	else
	{
		nDestWidth = Rect.Width();
		nDestHeight = Rect.Height();
	}
	
	switch(ShowModeFlip) 
	{
	case NoFlip:			//����ת
		XDest = Rect.left;
		YDest = Rect.top;
		break;
	case FlipHorizintal:	//ˮƽ��ת
		XDest = Rect.right;
		YDest = Rect.top;
		nDestWidth = -nDestWidth;
		break;
	case FlipVertical:		//��ֱ��ת
		m_pBitmapInfo->bmiHeader.biHeight = -lImageHeight;
		XDest = Rect.left;
		YDest = Rect.top;
		break;
	case Flip:				//ȫ��ת
		m_pBitmapInfo->bmiHeader.biHeight = -lImageHeight;
		XDest = Rect.right;
		YDest = Rect.top;
		nDestWidth = -nDestWidth;
		break;
	default:				//����ת
		XDest = Rect.left;
		YDest = Rect.top;
	}

	SetStretchBltMode(pDC->m_hDC, COLORONCOLOR);
	
	StretchDIBits(pDC->GetSafeHdc(),
		XDest,
		YDest,	
		nDestWidth,				//��ʾ���ڿ��
		nDestHeight,			//��ʾ���ڸ߶�
		0,
		0,
		lImageWidth,			//ͼ����
		lImageHeight,			//ͼ��߶�
		pImageBuff,				//ͼ�񻺳���
		m_pShowBitmapInfo,			//BMPͼ��������Ϣ
		DIB_RGB_COLORS,
		SRCCOPY
		);

	m_csShowBitmapInfoSection.Unlock();
}

//void CImage::SaveMe(const CString& strFileName)
void CImage::SaveDIBImageToBMPFile(const CString& strFileName, const BYTE* pImageBuff, long lImageWidth, long lImageHeight, long lImageBitCount)
{
	BOOL bRet = FALSE;

	ASSERT(pImageBuff != NULL && lImageWidth >= 0 && lImageHeight >= 0 && lImageBitCount > 0);
	ASSERT((strFileName.GetLength() >= 5) && (strFileName.Right(4) == ".bmp"));

	m_csSaveBitmapInfoSection.Lock();
	
	m_pSaveBitmapInfo->bmiHeader.biWidth = lImageWidth;
	m_pSaveBitmapInfo->bmiHeader.biHeight = lImageHeight;
	m_pSaveBitmapInfo->bmiHeader.biBitCount = (WORD)lImageBitCount;

	bRet = MyWriteBMPFile(strFileName, m_pSaveBitmapInfo, pImageBuff);

	m_csSaveBitmapInfoSection.Unlock();

	if(!bRet)
	{
		throw(CImageException(3, "�ļ����治�ɹ�", "SaveDIBImageToBMPFile()"));
		return;
	}	
}

//�ü�����(������DIBͼ������)(��ԭͼ�����Ͻ�Ϊԭ��, x����, y����)
//Ŀ�껺����//���о���
//����ֵ: TRUE: �ɹ�
//BOOL CImage::CutMe(BYTE *pDest, CRect DestRect)
void CImage::CutDIBImage(BYTE *pDest, CRect DestRect, const BYTE* pSrcImageBuff, long lSrcImageWidth, long lSrcImageHeight, long lSrcImageBitCount)
{
	//ע: DIBͼ�����ݵĴ洢��ʽ��: ԭͼ��ĵ�һ�д洢��DIBͼ�����ݻ����������һ��, �����������������д洢.
	
	ASSERT(pDest != NULL && pSrcImageBuff != NULL && lSrcImageWidth >= 0 && lSrcImageHeight >= 0 && lSrcImageBitCount > 0);

	int nOffset_Source = 0;//����ڻ�����ͷ��ƫ�Ƶ��ֽ���(Դ)
	int nOffset_Dest = 0;  //����ڻ�����ͷ��ƫ�Ƶ��ֽ���(Ŀ��)

	int CutPositionX = DestRect.left;
	int CutPositionY = DestRect.top;
	int CutWidth = DestRect.Width();
	int CutHeight = DestRect.Height();

	int nPixelByteCount = (lSrcImageBitCount+7)/8;//һ�������ü����ֽڱ�ʾ(RGB:3)
	
	//�������
	if((CutPositionX + CutWidth) > lSrcImageWidth || (CutPositionY + CutHeight) > lSrcImageHeight)
	{
		throw(CImageException(2, "�������Ϸ�", "CutDIBImage()"));
		return;
	}
	
	//���м�����ʼ�е�ת��
	int nBeginLine = lSrcImageHeight - CutPositionY - CutHeight;
	
	//ȥ��DIBͼ�����ݻ������ļ����е���ʼ��
	nOffset_Source = (lSrcImageWidth * nBeginLine + CutPositionX) * nPixelByteCount;
	
	for (int i = 0; i < CutHeight; i++)//�ж�����
	{
		memcpy(pDest + nOffset_Dest, pSrcImageBuff + nOffset_Source, CutWidth*nPixelByteCount);
		
		nOffset_Dest += (CutWidth * nPixelByteCount);  //��һ��(Ŀ��)
		
		nOffset_Source += (lSrcImageWidth*nPixelByteCount);  //��һ��(Դ)
	}
}

void CImage::CutPlaneRGBImage(BYTE *pDest, CRect DestRect, const BYTE* pSrcImageBuff, long lSrcImageWidth, long lSrcImageHeight)
{	
	ASSERT(pDest != NULL && pSrcImageBuff != NULL && lSrcImageWidth >= 0 && lSrcImageHeight >= 0);

	int nOffset_Source = 0;//����ڻ�����ͷ��ƫ�Ƶ��ֽ���(Դ)
	int nOffset_Dest = 0;  //����ڻ�����ͷ��ƫ�Ƶ��ֽ���(Ŀ��)

	int CutPositionX = DestRect.left;
	int CutPositionY = DestRect.top;
	int CutWidth = DestRect.Width();
	int CutHeight = DestRect.Height();
	
	//�������
	if((CutPositionX + CutWidth) > lSrcImageWidth || (CutPositionY + CutHeight) > lSrcImageHeight)
	{
		throw(CImageException(2, "�������Ϸ�", "CutDIBImage()"));
		return;
	}
	
	//���м�����ʼ�е�ת��
	int nBeginLine = lSrcImageHeight - CutPositionY - CutHeight;
	
	//ȥ��DIBͼ�����ݻ������ļ����е���ʼ��
	nOffset_Source = lSrcImageWidth * nBeginLine + CutPositionX;
	
	for (int i = 0; i < CutHeight; i++)//�ж�����
	{
		memcpy(pDest + nOffset_Dest, pSrcImageBuff + nOffset_Source, CutWidth);					//copyһ��B������
		memcpy(pDest + nOffset_Dest + CutHeight * CutWidth,
			pSrcImageBuff + nOffset_Source + lSrcImageWidth * lSrcImageHeight, CutWidth);		//copyһ��G������
		memcpy(pDest + nOffset_Dest + CutHeight * CutWidth * 2,
			pSrcImageBuff + nOffset_Source + lSrcImageWidth * lSrcImageHeight * 2, CutWidth);	//copyһ��R������
		
		nOffset_Dest += CutWidth;  //��һ��(Ŀ��)
		
		nOffset_Source += lSrcImageWidth;  //��һ��(Դ)
	}
}

void CImage::ConvertRGBToPlaneR_G_B(BYTE *pDestImageBuffR, BYTE *pDestImageBuffG, BYTE *pDestImageBuffB, 
									const BYTE* pSrcImageBuff, long lSrcImageWidth, long lSrcImageHeight)
{
	ASSERT(pDestImageBuffR != NULL && pDestImageBuffG != NULL && pDestImageBuffB != NULL
		&& pSrcImageBuff != NULL && lSrcImageWidth >= 0 && lSrcImageHeight >= 0);

	//RGB -> R��G��B
	//PixToPlane((const char *)pSrcImageBuff,
	//	(char *)pDestImageBuffR, (char *)pDestImageBuffG, (char *)pDestImageBuffB, 
	//	lSrcImageWidth, lSrcImageHeight);
}

void CImage::ConvertPlaneR_G_BToRGB(BYTE* pDestImageBuff, const BYTE *pSrcImageBuffR, const BYTE *pSrcImageBuffG, const BYTE *pSrcImageBuffB, 
									long lSrcImageWidth, long lSrcImageHeight)
{
	ASSERT(pDestImageBuff != NULL
		&& pSrcImageBuffR != NULL && pSrcImageBuffG != NULL && pSrcImageBuffB != NULL
		&& lSrcImageWidth >= 0 && lSrcImageHeight >= 0);

	//R��G��B -> RGB
	//PlaneToPix((const char *)pSrcImageBuffR, (const char *)pSrcImageBuffG, (const char *)pSrcImageBuffB,
	//	(char *)pDestImageBuff, 
	//	lSrcImageWidth, lSrcImageHeight);
}

void CImage::ConvertRGBToPlaneRGB(BYTE *pDestPlaneRGBBuff,	const BYTE* pSrcImageRGBBuff, long lSrcImageWidth, long lSrcImageHeight)
{
	ASSERT(pDestPlaneRGBBuff != NULL && pSrcImageRGBBuff != NULL
		&& lSrcImageWidth >= 0 && lSrcImageHeight >= 0);
	
	//RGB -> R��G��B
	//PixToPlane((const char *)pSrcImageRGBBuff,
	//	(char *)pDestPlaneRGBBuff, 
	//	(char *)pDestPlaneRGBBuff+lSrcImageWidth*lSrcImageHeight, 
	//	(char *)pDestPlaneRGBBuff+lSrcImageWidth*lSrcImageHeight*2, 
	//	lSrcImageWidth, lSrcImageHeight);
}

void CImage::ConvertPlaneRGBToRGB(BYTE* pDestImageRGBBuff, const BYTE *pSrcPlaneRGBBuff, long lSrcImageWidth, long lSrcImageHeight)
{
	ASSERT(pDestImageRGBBuff != NULL && pSrcPlaneRGBBuff != NULL
		&& lSrcImageWidth >= 0 && lSrcImageHeight >= 0);
	
	//R��G��B -> RGB
	//PlaneToPix((const char *)pSrcPlaneRGBBuff, 
	//	(const char *)pSrcPlaneRGBBuff+lSrcImageWidth*lSrcImageHeight, 
	//	(const char *)pSrcPlaneRGBBuff+lSrcImageWidth*lSrcImageHeight*2,
	//	(char *)pDestImageRGBBuff, 
	//	lSrcImageWidth, lSrcImageHeight);
}

