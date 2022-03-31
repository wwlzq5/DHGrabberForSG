/************************************************************************/
/* Copyright (c) 2007, �й���㼯�ű���ͼ���Ӿ������ֹ�˾�����Ӿ��㷨��     
/* All rights reserved.													
/*																		
/* �ļ����ƣ� CPQIAssistant.h												
/* �ļ���ʶ�� 
/* ժҪ�� ׼ȷ�ؼ���ʱ��
/*
/* ��ǰ�汾�� 1.0
/* ���ߣ� ���ķ�
/* ������ڣ� 2007��08��28��
/************************************************************************/

// CPQIAssistant.h : main header file for the CPQIASSISTANT DLL

#if !defined(AFX_CPQIASSISTANT_H_)
#define AFX_CPQIASSISTANT_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
#error include 'stdafx.h' before including this file for PCH
#endif
#include "vector"
using namespace std;
#include "resource.h"		// main symbols

#define DLL_EXT _declspec(dllexport)
#define RGB24  1
#define RGB32  2
#define RRGGBB 3
/////////////////////////////////////////////////////////////////////////////
// CCPQIAssistantApp
// See CPQIAssistant.cpp for the implementation of this class
//

typedef struct tagRectPara
{
   RECT rect;		// ƥ��ɹ���MARK����
   double fRatio;	// ƥ���
}MultiRect;

typedef struct tagImgPara
{
   int nWidth;		// ͼ���
   int nHeight;		// ͼ���
   int nChannel;	// ͼ��ͨ������Ŀǰֻ֧��1��3
   PBYTE pImgbuf;	// ͼ������ָ��
}s_ImgData;

class CCPQIAssistantApp : public CWinApp
{
public:
	CCPQIAssistantApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCPQIAssistantApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CCPQIAssistantApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	vector<MultiRect> m_vMultiRect;

};
/**********************************************************************
created:	28:8:2007 
function:   �ж��Ƿ��н�ֽ�źţ�ԭ��Ϊ�������RGB��ͨ��ͼ���ƽ���Ҷ�ֵ�����趨�ĻҶ�ֵΪ
            �н�ֽ�źţ������ޣ��Լ���д��
Input_para: const char * dataR, const char * dataG, const char * dataB ����RGBͼ���ָ��
			int nWidth, int nHeight ͼ����
			RECT rect,              ָ����ROI����
			int nThresh             �����ж�ֵ
Output_para:
return:     �н�ֽ����true�����򷵻�false
***********************************************************************/
DLL_EXT bool  EnterPaper(const char * dataR, const char * dataG, const char * dataB, 
						int nWidth, int nHeight, RECT rect, int nThresh);
/**********************************************************************
created:	28:8:2007 
function:   �ж��Ƿ��н�ֽ�źţ�ԭ��Ϊ�������RGB��ͨ��ͼ���ƽ���Ҷ�ֵ�����趨�ĻҶ�ֵΪ
            �н�ֽ�źţ������ޣ���OpenCV��
Input_para: const char * dataR, const char * dataG, const char * dataB ����RGBͼ���ָ��
			int nWidth, int nHeight ͼ����
			RECT rect,              ָ����ROI����
			int nThresh             �����ж�ֵ
Output_para:
return:     �н�ֽ����true�����򷵻�false
***********************************************************************/
DLL_EXT bool EnterPaper_OpenCv(char *dataR, char *dataG, char *dataB, 
							   int nWidth, int nHeight, RECT rect, int nThresh);

//_declspec(dllexport) PixToPlane(const char *srcRGB, char *dstR, char *dstG, 
//									 char *dstB, int nWidth, int nHeight);
//
//_declspec(dllexport) PlaneToPix(const char *srcR, const char *srcG, 
//									 const char *srcB, char *dstRGB,  int nWidth,
//									 int nHeight);
/************************************************************************/
/* 
���ܣ���RGB ͼ��ֽ�ΪR��G��B��ͨ��ͼ�񣬽�32λͼ��ֽ�ΪΪR��G��B��ͨ��ͼ��
      ���߽�RRGGBBͼ��ֽ�ΪR��G��B��ͨͼ��
������const char *srcRGB ����Դͼ�������
	  char *dstR, char *dstG, char *dstB ���ͼ��R��G��B��ͨ��ͼ��
	  int nWidth, int nHeight����ͼ��Ŀ��
      const int nType�ֽ�ͼ������ͣ� RGB24  1��24λͼ�����ݣ�RGB32  2��32λͼ�����ݣ�
      RRGGBB 3����λ�ĻҶ�ͼ������
    

                                                                     */
/************************************************************************/
bool _declspec(dllexport)RGBToGray(const char *srcRGB, char *dstR, char *dstG, 
					char *dstB, int nWidth, int nHeight, const int nType);


/*******************************************************************************
���������    
	sImgData��  ͼ�����ݽṹ
	rcMark��    mark����
	dMatchRedio:�ⲿ�������Сƥ����
	nMarkExtend:mark����������չ��Χ
	nWNum��	    ��ȷ������
	nHNum��     �߶ȷ������
    nptLast��   ���һ��MARK��������ϽǶ���������
���������
	sRectVector���洢MultiRect�ṹ��vectorָ�����
����ֵ��bool ���ͣ�true�������ɹ���false������ʧ��
***********************************************************************************/
DLL_EXT bool FindMultiRegion(const s_ImgData& sImgData,
				const RECT& rcMark,
				const double dMatchRedio,
				int nMarkExtend,
				int nWNum,
				int nHNum,
				const POINT& nptLast,
				vector<MultiRect> **sRectVector);

/*************[7/2/2007]*********************************************************
  ���ܣ���һ���������Ҷ��mark
  const s_ImgData& sImgData   ͼ�����ݽṹ--�ο�����ṹ��
  const RECT& rcMark, markͼ��������Դͼ����
  const RECT& rcSearchMark  markͼ����������
  dMatchRedio:�ⲿ�������Сƥ����
���������
	sRectVector���洢MultiRect�ṹ��vectorָ����б�ʾ�ҵ�mark�ĸ���������������ṹ��
  ����ֵ��bool ���ͣ�true�������ɹ���false������ʧ��  
/*************[7/2/2007]*********************************************************/
DLL_EXT bool FindMultiMark(const s_ImgData& sImgData,
				const RECT& rcMark,
				const RECT& rcSearchMark,
				const double dMatchRedio,
				vector<MultiRect> **sRectVector);

/***************************************************************************************
 ��̬��ʹ��˵����
 1�����ȳ�ʼ����̬��
 2��������غ��������С�����ǳ�ʼ���õĴ�С
 3���ͷŶ�̬����Դ
********************************************************************************************/

/*************[5/3/2007]******************************
��ֵ����ʼ��
int nWidth, int nHeight  ԭʼͼ���С
/*************[5/3/2007]*******************************/
DLL_EXT bool Threhold_init(int nWidth, int nHeight); 
/*************[5/1/2007]******************************
���ܣ�����halcon��dyn_threshold��������
      fDilation ����halcon���������ͺ���
	  bFillOrNot ����halcon��fill_up����
 
bFillOrNot 0:�����, 1:���
fDilation  ����ϵ��Ĭ��
char *ImageSrc  Դͼ������ͨ��
char *ImageDst  ��ֵ����ͼ��һ��ͨ��
RECT &ROI ����Ȥ����
int nChannel ѡ���ͨ��  1==R, 2==G, 3==B
int nFlag   const int DARK = 0��const int LIGHT = 1��

/*************[5/1/2007]*******************************/

DLL_EXT bool Dyn_Threshold_Image3(char *imageRGB,  char *imageDst, RECT &ROI, int nChannel, 
							int nDynOffset,  bool bFillOrNot, 
							int nSizeMean, float fDilation, int nFlag);
/*************[5/1/2007]******************************
��Dyn_Threshold_Image3��������

char *ImageSrc  Դͼ�񣬵�ɫͼ��
char *ImageDst  ��ֵ����ͼ�񣬵�ɫͼ��
RECT &ROI ����Ȥ����
int nFlag   const int DARK = 0��const int LIGHT = 1��

/*************[5/1/2007]*******************************/


DLL_EXT bool Dyn_Threshold_Image(char *imageSrc,  char *imageDst,  RECT &ROI, 
							int nDynOffset,  bool bFillOrNot, 
							int nSizeMean, float fDilation, int nFlag);


/*************[5/1/2007]******************************
 
char *ImageSrc  Դͼ�񣬵�ɫͼ��
char *ImageDst  ��ֵ����ͼ�񣬵�ɫͼ��
RECT &ROI ����Ȥ����
int nFlag   const int DARK = 0��const int LIGHT = 1��
int nLowThres, int nHighThres��ֵ��Χ��nLowThres,nHighThres��,
       ���nLowThres>nHighThres ����ֵ��ΧΪ��0,nHighThres��and��nLowThres,255��
/*************[5/1/2007]*******************************/
DLL_EXT bool Bin_Threshold_Image(char *imageSrc,  char *imageDst,  RECT &ROI,
							    int nLowThres, int nHighThres, int nFlag);
/*************[5/1/2007]******************************
 
char *imageRGB  Դͼ������ͨ��
char *imageDst  ��ֵ����ͼ�񣬵�ɫͼ��
RECT &ROI ����Ȥ����
int nFlag   const int DARK = 0��const int LIGHT = 1��
int nLowThres, int nHighThres��ֵ��Χ��nLowThres,nHighThres��,
       ���nLowThres>nHighThres ����ֵ��ΧΪ��0,nHighThres��and��nLowThres,255��
/*************[5/1/2007]*******************************/
DLL_EXT bool Bin_Threshold_Image3(char *imageRGB,  char *imageDst,  RECT &ROI, int nChannel,  
							      int nLowThres, int nHighThres, int nFlag);
/*************[6/7/2007]*******************************************************
 �ҶȻ�ͼ������
 ����ͼ����ԭʼ��С
/*************[6/7/2007]******************************************************/
DLL_EXT bool RGB_to_Gray(char* imageRGB, char* imageDst, RECT &ROI,
					     double dRatioR, double dRatioG, double dRatioB);
/*************[5/3/2007]********************************************************
�ͷŷ������Դ
/*************[5/3/2007]********************************************************/
DLL_EXT bool Threshold_Free();
/********************************************************************
	created:	2007/11/30
	file base:	CPQIAssistant
	file ext:	h
	author:		by lwf
    inputPara:  sImageMean ��ֵģ�壬sImageVary:ƫ��ģ�壬sImageIn:Сͼ��
				cPoint��Сͼ����ģ���е��󶥵�
				
    outPara:    sImageMean ��ֵģ�壬sImageVary:ƫ��ģ��
	
	purpose: Ϊ���������ɾ�ֵģ�塢ƫ��ģ��,ͬʱ���ݲ�ɫ���ڰ�	
*********************************************************************/
DLL_EXT bool UpDateModel(s_ImgData &sImageMean, s_ImgData &sImageVary,
		s_ImgData sImageIn, CPoint cPoint, bool bFlagR, bool bFlagG, bool bFlagB);

#endif // !defined(AFX_CPQIASSISTANT_H_)
