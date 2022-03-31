/************************************************************************/
/* Copyright (c) 2005, �й���㼯�ű���ͼ���Ӿ������ֹ�˾�Ӿ�������     
/* All rights reserved.													
/*																		
/* �ļ����ƣ� Image.h												
/* �ļ���ʶ�� �����ù���ƻ���
/* ժҪ�� CImage��, �Ա���һ��ͼ��ĸ����ݽṹ���з�װ, ʵ����ͼ�񿽱�,
/*    ͼ����ʾ, ͼ�����, ͼ����еȹ���
/*
/* ��ǰ�汾�� 1.0
/* ���ߣ� ��׿��
/* ������ڣ� 2005��10��10��
/************************************************************************/

#if !defined(Image_h_)
#define Image_h_

#include <afxmt.h>          //Ϊ��ʹ��CEvent
//#define ByteBitCount 8

enum e_ShowModeFlip	//��ת��ʾ��ʽ
{
	NoFlip = 0,					//����ת��ʾ
	FlipVertical,				//��ֱ��ת��ʾ
	FlipHorizintal,				//ˮƽ��ת��ʾ
	Flip						//ȫ��ת��ʾ
};	//��ת��ʾ��ʽ

enum e_ShowModeStretch	//������ʾ��ʽ
{
	NoStretch = 0,				//��������ʾ
	Stretch						//������ʾ
};	//������ʾ��ʽ

class CImage
{	
//����
protected:	
	BYTE* m_pImageBuff;	//ͼ�񻺳���
	long m_lBuffSize;	//ͼ�񻺳�����С
	
	long m_lImageWidth;		//ͼ����
	long m_lImageHeight;	//ͼ�񳤶�
	long m_lImageBitCount;	//����λ��
	long m_lImagePixelSize;	//���ش�С����λ���ֽڣ�
	long m_lImageSize;		//ͼ�����ݴ�С

	BITMAPINFO* m_pBitmapInfo;
	
	BITMAPINFO* m_pShowBitmapInfo;
	CCriticalSection m_csShowBitmapInfoSection;
	BITMAPINFO* m_pSaveBitmapInfo;
	CCriticalSection m_csSaveBitmapInfoSection;

//����
public:
	CImage();
	virtual ~CImage();
	
	void Init(long lImageBuffSize);
	void SetImageSize(long lImageWidth, long lImageHeight, long lImageBitCount);
	void AcceptImage(const BYTE* pSrcImageBuff, long lSrcImageWidth, long lSrcImageHeight, long lSrcImageBitCount);

	BYTE* GetImageBuffAddr();
	long GetImageBuffSize();
	long GetImageSize();
	long GetImageWidth();
	long GetImageHeight();
	long GetImageBitCount();
	long GetImagePixelSize();
	const BITMAPINFO* GetBitmapInfo();
	
	void ShowDIBImage(CDC* pDC, CRect Rect, e_ShowModeFlip ShowModeFlip, e_ShowModeStretch ShowModeStretch,
		const BYTE* pImageBuff, long lImageWidth, long lImageHeight, long lImageBitCount);
	void SaveDIBImageToBMPFile(const CString& strFileName, const BYTE* pImageBuff, long lImageWidth, long lImageHeight, long lImageBitCount);
	void CutDIBImage(BYTE *pDest, CRect DestRect, const BYTE* pSrcImageBuff, long lSrcImageWidth, long lSrcImageHeight, long lSrcImageBitCount);
	void CutPlaneRGBImage(BYTE *pDest, CRect DestRect, const BYTE* pSrcImageBuff, long lSrcImageWidth, long lSrcImageHeight);
	void ConvertRGBToPlaneR_G_B(BYTE *pDestImageBuffR, BYTE *pDestImageBuffG, BYTE *pDestImageBuffB, 
		const BYTE* pSrcImageBuff, long lSrcImageWidth, long lSrcImageHeight);
	void ConvertPlaneR_G_BToRGB(BYTE* pDestImageBuff, const BYTE *pSrcImageBuffR, const BYTE *pSrcImageBuffG, const BYTE *pSrcImageBuffB, 
		long lSrcImageWidth, long lSrcImageHeight);
	void ConvertRGBToPlaneRGB(BYTE *pDestPlaneRGBBuff,	const BYTE* pSrcImageRGBBuff, long lSrcImageWidth, long lSrcImageHeight);
	void ConvertPlaneRGBToRGB(BYTE* pDestImageRGBBuff, const BYTE *pSrcPlaneRGBBuff, long lSrcImageWidth, long lSrcImageHeight);

};

class CImageException
{
//����
protected:	
	int m_nExceptionType;				//�쳣������
	CString m_strExceptionDescription;	//�쳣������
	CString m_strExceptionContext;		//�쳣�ĸ�����Ϣ
	
//����
public:
	CImageException(int nExceptionType, const CString &strExceptionContext)
		: m_nExceptionType(nExceptionType), 
		m_strExceptionContext(strExceptionContext)
	{
		m_strExceptionDescription.Format("Error Type(%d)");
	};
	
	CImageException(int nExceptionType, const CString &strExceptionDescription, const CString &strExceptionContext)
		: m_nExceptionType(nExceptionType), 
		m_strExceptionDescription(strExceptionDescription),
		m_strExceptionContext(strExceptionContext)
	{
	};
	
	~CImageException()
	{
	};

	//�õ�����
	int GetType() const
	{
		return m_nExceptionType;
	}

	//�õ������͵�����
	CString GetDescription() const
	{
		return m_strExceptionDescription;
	}
	
	//�õ�������Ϣ
	CString GetContext() const
	{
		return m_strExceptionContext;
	}
	
	void ShowMe() const
	{
		CString str;
		str.Format("�������ͣ�%d\n������%s\n������Ϣ��%s", 
			m_nExceptionType,
			m_strExceptionDescription,
			m_strExceptionContext);

		AfxMessageBox(str);
	}
};


#endif // !define(Image_h_)