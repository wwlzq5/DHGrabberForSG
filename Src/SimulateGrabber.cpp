/************************************************************************/
/* Copyright (c) 2007, �й���㼯�ű���ͼ���Ӿ������ֹ�˾�Ӿ�ϵͳ��     
/* All rights reserved.													
/*																		
/* �ļ����ƣ� SimulateGrabber.cpp
/* ժҪ�� ͼ��ɼ���ģ����, ʹ�������ļ����Ѵ��ڵ�ͼ���ļ�ʵ��ģ�ⶨʱ��ͼ
/*
/* ��ǰ�汾�� 3.0
/* ���ߣ� ��׿��
/* ������ڣ� 2009��12��18��	// [2009-12-18 by jzj]: add
/*
/* ��ǰ�汾�� 2.0
/* ���ߣ� ��׿��
/* ������ڣ� 2007��11��13��
/************************************************************************/

#include "stdafx.h"
#include "SimulateGrabber.h"
#include "FileOperate.h"
#include "Image.h"
#include "SpendTime.h"// [2008-1-22 by jzj]

#include "SGParamDlg.h"

// ������öԻ���ʱ��884����
class tempRoutingFrame 
{
   CFrameWnd* m_pFrame;

public:

   tempRoutingFrame(CFrameWnd * pWnd= NULL)
   {
      // Save current value
      m_pFrame = AfxGetThreadState()->m_pRoutingFrame;
      // Set to value passed in. NULL by default.
      AfxGetThreadState()->m_pRoutingFrame = pWnd;
   }

   ~tempRoutingFrame()
   {
      // Restore m_pRoutingFrame to original value.
      AfxGetThreadState()->m_pRoutingFrame = m_pFrame;
   }
};

CSimulateGrabber::CSimulateGrabber()
{
	//////////////////////////////////////////////////////////////////////////
	// ������ʼ��

	m_LastErrorInfo.nErrorCode = 0;
	memset(m_LastErrorInfo.strErrorDescription, 0, GBMaxTextLen);
	memset(m_LastErrorInfo.strErrorRemark, 0, GBMaxTextLen);
	
	m_nCardSN = 0;	//ͼ�����
	m_CallBackFunc  = NULL;	//�ص�����ָ��
	m_Context = NULL;	//��ŵ��ó�ʼ�������Ķ����thisָ��

	m_pImageBuffer = NULL;	//�����洦���õ�ͼ�񻺳���ָ��
	m_nImageWidth = 0;	//ͼ���� (��λ:�ֽ�)
	m_nImageHeight = 0;	//ͼ��߶� (��λ:�ֽ�)
	m_nImageByteCount = 0;	//ͼ�����ش�С (��λ:�ֽ�)
	m_nImageBuffSize = 0;	//ͼ�񻺳�����С (��λ:�ֽ�)
	m_bPlaneRGB = FALSE;		//�Ƿ��ͨ��
	m_pPlaneRGBBuffer = NULL;	//�����洦���õķ�ͨ��ͼ�񻺳���

	m_bInited = FALSE;	//ͼ���Ƿ��Ѿ���ʼ��

	m_bGrab = FALSE;//�Ƿ�ɼ�ͼ��
	m_bGrabbing = FALSE;//�Ƿ����ڲɼ�ͼ��
	m_nGrabSpeed = 0;//�ɼ��ٶ�(��λ: ����/��)

	m_bSnap = FALSE;// �Ƿ�ɼ�һ֡ͼ�� // [2009-12-28 by jzj]: add

	m_pBmpInfo = NULL;

	m_bKillServiceThread = FALSE;

	m_nCapturedFieldCount = 0;//�ɼ�֡����
	
	m_bPreReadImage = FALSE;//�Ƿ�Ԥ�ȶ�ͼ���ڴ���
	m_nImageNum = 0;//ͼ������
	for(int i = 0; i < SGMaxImageNum; i++)
	{
		m_PreReadImageBuffArray[i] = NULL;
	}
	
	m_bImagesOK = FALSE;//ͼ���Ƿ���׼����
	m_bUpdateImagesFolder = FALSE;//����ͼ�������ļ���
	
	m_nMaxImageWidth = 0;
	m_nMaxImageHeight = 0;
	m_nMaxImageByteCount = 0;

	m_bLoopGrab = FALSE;// �Ƿ�ѭ���ɼ�// [2008-10-23 by jzj]

	m_bResetPosition = FALSE;// �Ƿ�����Ԥ��λ��// [2008-12-4 by jzj]: add

	m_iGrabberTypeSN = 0;	// �ɼ������ͱ��// [2009-12-18 by jzj]: add
	//
	//////////////////////////////////////////////////////////////////////////
}

CSimulateGrabber::~CSimulateGrabber()
{
	m_FileFinder.Close();

	if(m_bPreReadImage == FALSE)
	{
		if(m_pImageBuffer != NULL)
		{
			delete []m_pImageBuffer;
			m_pImageBuffer = NULL;
		}
		
		if(m_pPlaneRGBBuffer != NULL)
		{
			delete []m_pPlaneRGBBuffer;
			m_pPlaneRGBBuffer = NULL;
		}
	}
	else
	{
		for(int i = 0; i < m_nImageNum; i++)
		{
			if(m_PreReadImageBuffArray[i] != NULL)
			{
				delete []m_PreReadImageBuffArray[i];
				m_PreReadImageBuffArray[i] = NULL;
			}
		}
	}	

	if(m_pBmpInfo != NULL)
	{
		delete m_pBmpInfo;
		m_pBmpInfo = NULL;
	}
}

//��ʼ��ͼ��
BOOL CSimulateGrabber::Init(const s_GBINITSTRUCT* pInitParam)
{
	s_SGINITCARDSTRUCT InitCardParam;
	InitCardParam.CardSN = pInitParam->nGrabberSN;
	memcpy(InitCardParam.sInitFile, pInitParam->strGrabberFile, GBMaxFileNameLen);
	InitCardParam.CallBackFunc = pInitParam->CallBackFunc;
	InitCardParam.Context = pInitParam->Context;

	m_iGrabberTypeSN = pInitParam->iGrabberTypeSN;	// �ɼ������ͱ��// [2009-12-18 by jzj]: add
	m_strDeviceName = pInitParam->strDeviceName;
	BOOL bRet = InitCard(&InitCardParam);

	return bRet;
}

//�ر�ͼ��
BOOL CSimulateGrabber::Close()
{
	BOOL bRet = CloseCard();

	return bRet;
}

//��ʼ�ɼ�
BOOL CSimulateGrabber::StartGrab()
{
	if(m_bInited == FALSE)
	{
		m_LastErrorInfo.nErrorCode = SGErrorCodeNoInit;
		sprintf(m_LastErrorInfo.strErrorDescription, "����δ��ʼ�������ܽ��иò�����");
		sprintf(m_LastErrorInfo.strErrorRemark, "λ�ã�StartGrab()");
		return FALSE;
	}
	
	if(m_bPreReadImage == TRUE)// [2008-12-4 by jzj]: add
	{
		m_bResetPosition = TRUE;// [2008-12-4 by jzj]: add
	}
	else
	{
		m_bUpdateImagesFolder = TRUE;// [2008-10-23 by jzj]Ϊ��ʵ����������
	}

	Sleep(1000);
	m_bGrab = TRUE;//�Ƿ�ɼ�ͼ��

	return TRUE;
}

//ֹͣ�ɼ�
BOOL CSimulateGrabber::StopGrab()
{
	if(m_bInited == FALSE)
	{
		m_LastErrorInfo.nErrorCode = SGErrorCodeNoInit;
		sprintf(m_LastErrorInfo.strErrorDescription, "����δ��ʼ�������ܽ��иò�����");
		sprintf(m_LastErrorInfo.strErrorRemark, "λ�ã�StopGrab()");
		return FALSE;
	}

	m_bGrab = FALSE;//�Ƿ�ɼ�ͼ��
	
	return TRUE;
}

//��֡�ɼ�
BOOL CSimulateGrabber::Snapshot()
{
	// [2009-12-28 by jzj]: add

	if(m_bInited == FALSE)
	{
		m_LastErrorInfo.nErrorCode = SGErrorCodeNoInit;
		sprintf(m_LastErrorInfo.strErrorDescription, "����δ��ʼ�������ܽ��иò�����");
		sprintf(m_LastErrorInfo.strErrorRemark, "λ�ã�StartGrab()");
		return FALSE;
	}
	
	m_bSnap = TRUE;// �Ƿ�ɼ�һ֡ͼ��
	m_bGrab = TRUE;// �Ƿ�ɼ�ͼ��

	return TRUE;
}

//�õ�����
BOOL CSimulateGrabber::GetParamInt(GBParamID Param, int &nOutputVal)
{
	if(m_bInited == FALSE)
	{
		m_LastErrorInfo.nErrorCode = SGErrorCodeNoInit;
		sprintf(m_LastErrorInfo.strErrorDescription, "����δ��ʼ�������ܽ��иò�����");
		sprintf(m_LastErrorInfo.strErrorRemark, "λ�ã�GetParamInt(GBParamID Param, int &nOutputVal)");
		return FALSE;
	}

	switch(Param)
	{
	case GBImageWidth:
		nOutputVal = m_nImageWidth;
		break;
	case GBImageHeight:
		nOutputVal = m_nImageHeight;
		break;
	case GBImagePixelSize:
		nOutputVal = m_nImageByteCount;
		break;
	case GBImageBufferSize:
		nOutputVal = m_nImageBuffSize;
		break;
	case GBImageBufferAddr:
		{
			if (m_bPlaneRGB)
			{
				nOutputVal = ((__int64)m_pPlaneRGBBuffer) & 0xFFFFFFFF;
			}
			else
			{
				nOutputVal = ((__int64)m_pImageBuffer) & 0xFFFFFFFF;
			}

// 			if(m_bPlaneRGB)
// 			{
// 				nOutputVal = (int)m_pPlaneRGBBuffer;
// 			}
// 			else
// 			{
// 				nOutputVal = (int)m_pImageBuffer;
// 			}
		}
		break;			
	case GBGrabberTypeSN:		// �ɼ������ͱ��	// [2009-12-18 by jzj]: add
		nOutputVal = m_iGrabberTypeSN;
		break;
	case GBImageBufferAddr2:
		{
			if (m_bPlaneRGB)
			{
				nOutputVal = ((__int64)m_pPlaneRGBBuffer) >> 32;
			}
			else
			{
				nOutputVal = ((__int64)m_pImageBuffer) >> 32;
			}

// 			if (m_bPlaneRGB)
// 			{
// 				nOutputVal = (int)m_pPlaneRGBBuffer + 2;
// 			}
// 			else
// 			{
// 				nOutputVal = (int)m_pImageBuffer + 2;;
// 			}
		}
		break;
	default:
		m_LastErrorInfo.nErrorCode = SGErrorCodeParamIll;
		sprintf(m_LastErrorInfo.strErrorDescription, "�������Ϸ���");
		sprintf(m_LastErrorInfo.strErrorRemark, "λ�ã�GetParamInt(GBParamID Param, int &nOutputVal)");
		return FALSE;
	}

	return TRUE;
}

//���ò���
BOOL CSimulateGrabber::SetParamInt(GBParamID Param, int nInputVal)
{
	if(m_bInited == FALSE)
	{
		m_LastErrorInfo.nErrorCode = SGErrorCodeNoInit;
		sprintf(m_LastErrorInfo.strErrorDescription, "����δ��ʼ�������ܽ��иò�����");
		sprintf(m_LastErrorInfo.strErrorRemark, "λ�ã�SetParamInt(GBParamID Param, int nInputVal)");
		return FALSE;
	}

	ASSERT(FALSE);

	m_LastErrorInfo.nErrorCode = SGErrorCodeThisFuncDisable;
	sprintf(m_LastErrorInfo.strErrorDescription, "�ò�����Ч��");
	sprintf(m_LastErrorInfo.strErrorRemark, "λ�ã�SetParamInt(GBParamID Param, int nInputVal)");
	return FALSE;
}

//���ò����Ի���
void CSimulateGrabber::CallParamDialog()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
	
#ifdef _DEBUG
	tempRoutingFrame rframe;// Workaround for ASSERT in WINCORE.CPP 884 (CWnd::AssertValid)
#endif

	CSGParamDlg dlg;
	dlg.SetOwner(this);
	dlg.m_nGrabSpeed = m_nGrabSpeed;
	dlg.m_bIsPlaneRGB = m_bPlaneRGB;
	dlg.m_strImagesPath = m_strImageFileFolder;
	
	dlg.m_nImageWidth = m_nImageWidth;
	dlg.m_nImageHeight = m_nImageHeight;
	dlg.m_nChannelNum = m_nImageByteCount;
	
	dlg.DoModal();
}

//�õ�������Ϣ
void CSimulateGrabber::GetLastErrorInfo(s_GBERRORINFO *pErrorInfo)
{
	ASSERT(pErrorInfo != NULL);
	
	pErrorInfo->nErrorCode = m_LastErrorInfo.nErrorCode;
	sprintf(pErrorInfo->strErrorDescription, m_LastErrorInfo.strErrorDescription);
	sprintf(pErrorInfo->strErrorRemark, m_LastErrorInfo.strErrorRemark);
}

//////////////////////////////////////////////////////////////////////////
//ģ��ɼ���ר�ýӿ�

//��ʼ��ͼ��
BOOL CSimulateGrabber::InitCard(const s_SGINITCARDSTRUCT* pInitCardParam)
{	
	ASSERT(pInitCardParam != NULL);
	ASSERT(pInitCardParam->CallBackFunc != NULL);
	ASSERT(pInitCardParam->Context != NULL);

	if(pInitCardParam == NULL || pInitCardParam->CallBackFunc == NULL || pInitCardParam->Context == NULL)
	{
		m_LastErrorInfo.nErrorCode = SGErrorCodeParamIll;
		sprintf(m_LastErrorInfo.strErrorDescription, "�������Ϸ���");
		sprintf(m_LastErrorInfo.strErrorRemark, "λ�ã�InitCard()");
		return FALSE;
	}
	
	if(m_bInited == TRUE)
	{
		CloseCard();
		m_bInited = FALSE;
	}
	
	//////////////////////////////////////////////////////////////////////////
	//
	
	m_nCardSN = 0;	//ͼ�����
	m_CallBackFunc  = NULL;	//�ص�����ָ��
	m_Context = NULL;	//��ŵ��ó�ʼ�������Ķ����thisָ��

	m_pImageBuffer = NULL;	//�����洦���õ�ͼ�񻺳���ָ��
	m_nImageWidth = 0;	//ͼ���� (��λ:�ֽ�)
	m_nImageHeight = 0;	//ͼ��߶� (��λ:�ֽ�)
	m_nImageByteCount = 0;	//ͼ�����ش�С (��λ:�ֽ�)
	m_nImageBuffSize = 0;	//ͼ�񻺳�����С (��λ:�ֽ�)
	m_bPlaneRGB = FALSE;		//�Ƿ��ͨ��
	m_pPlaneRGBBuffer = NULL;	//�����洦���õķ�ͨ��ͼ�񻺳���

	m_bInited = FALSE;	//ͼ���Ƿ��Ѿ���ʼ��

	m_bGrab = FALSE;//�Ƿ�ɼ�ͼ��
	m_bGrabbing = FALSE;//�Ƿ����ڲɼ�ͼ��
	m_nGrabSpeed = 0;//�ɼ��ٶ�(��λ: ����/��)

	m_bSnap = FALSE;// �Ƿ�ɼ�һ֡ͼ��// [2009-12-28 by jzj]: add
	
	//����λͼ��Ϣ�ռ�
	m_pBmpInfo = (PBITMAPINFO)new BYTE[sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD)];

	m_bKillServiceThread = FALSE;

	m_nCapturedFieldCount = 0;//�ɼ�֡����
	
	m_bPreReadImage = FALSE;//�Ƿ�Ԥ�ȶ�ͼ���ڴ���
	m_nImageNum = 0;//ͼ������
	for(int i = 0; i < SGMaxImageNum; i++)
	{
		m_PreReadImageBuffArray[i] = NULL;
	}
	
	m_bImagesOK = FALSE;//ͼ���Ƿ���׼����
	m_bUpdateImagesFolder = FALSE;//����ͼ�������ļ���
	
	m_nMaxImageWidth = 0;
	m_nMaxImageHeight = 0;
	m_nMaxImageByteCount = 0;

	m_bLoopGrab = FALSE;// �Ƿ�ѭ���ɼ�// [2008-10-23 by jzj]

	m_bResetPosition = FALSE;// �Ƿ�����Ԥ��λ��// [2008-12-4 by jzj]: add
	//
	//////////////////////////////////////////////////////////////////////////
	
	m_nCardSN = pInitCardParam->CardSN;
	memcpy(m_sInitFile, pInitCardParam->sInitFile, GBMaxFileNameLen);
	if(!IsFileExist(m_sInitFile))
	{
		m_LastErrorInfo.nErrorCode = SGErrorCodeInitFileNoExist;
		sprintf(m_LastErrorInfo.strErrorDescription, "��ʼ���ļ������ڣ�");
		sprintf(m_LastErrorInfo.strErrorRemark, "λ�ã�InitCard()");
		return FALSE;
	}
	
	//�������ļ��п�����
	GetPrivateProfileString("��ͼ����", "ͼ���ļ������ļ���", "NULL", m_strImageFileFolder.GetBuffer(100), 100, m_sInitFile);
	m_nImageWidth = GetPrivateProfileInt("��ͼ����", "ͼ����", -1, m_sInitFile);
	m_nImageHeight = GetPrivateProfileInt("��ͼ����", "ͼ��߶�", -1, m_sInitFile);
	m_nImageByteCount = GetPrivateProfileInt("��ͼ����", "ÿ�����ֽ���", -1, m_sInitFile);	
	m_bPlaneRGB = GetPrivateProfileInt("��ͼ����", "�Ƿ��ͨ��", -1, m_sInitFile);
	m_nGrabSpeed = GetPrivateProfileInt("��ͼ����", "�ɼ��ٶ�", -1, m_sInitFile);
	m_bPreReadImage = GetPrivateProfileInt("��ͼ����", "�Ƿ���Ԥ��ģʽ", -1, m_sInitFile);
	m_nImageNum = GetPrivateProfileInt("��ͼ����", "ͼ������", -1, m_sInitFile);
	m_bLoopGrab = GetPrivateProfileInt("��ͼ����", "�Ƿ�ѭ���ɼ�", -1, m_sInitFile);// [2008-10-23 by jzj]

	if(!IsPathExist(m_strImageFileFolder))
	{
		m_LastErrorInfo.nErrorCode = SGErrorCodeInitFileParamIll;
		sprintf(m_LastErrorInfo.strErrorDescription, "�����ļ���������ͼ���ļ������ļ��� �����ڣ�");
		sprintf(m_LastErrorInfo.strErrorRemark, "λ�ã�InitCard()");
		return FALSE;
	}
	if(m_nImageWidth < 1 || m_nImageHeight < 1)
	{
		m_LastErrorInfo.nErrorCode = SGErrorCodeInitFileParamIll;
		sprintf(m_LastErrorInfo.strErrorDescription, "�����ļ���������ͼ���� ���Ϸ���������ֵ��1 -- n��");
		sprintf(m_LastErrorInfo.strErrorRemark, "λ�ã�InitCard()");
		return FALSE;
	}
	if(m_nImageByteCount < 1 || m_nImageByteCount > 4)
	{
		m_LastErrorInfo.nErrorCode = SGErrorCodeInitFileParamIll;
		sprintf(m_LastErrorInfo.strErrorDescription, "�����ļ���������ÿ�����ֽ��� ���Ϸ���������ֵ��1 -- 4��");
		sprintf(m_LastErrorInfo.strErrorRemark, "λ�ã�InitCard()");
		return FALSE;
	}
	if(m_bPlaneRGB < 0 || m_bPlaneRGB > 1)
	{
		m_LastErrorInfo.nErrorCode = SGErrorCodeInitFileParamIll;
		sprintf(m_LastErrorInfo.strErrorDescription, "�����ļ����������Ƿ��ͨ�� ���Ϸ���������ֵ��0 -- 1��");
		sprintf(m_LastErrorInfo.strErrorRemark, "λ�ã�InitCard()");
		return FALSE;
	}
	if(m_nGrabSpeed < 1 || m_nGrabSpeed > 60000)
	{
		m_LastErrorInfo.nErrorCode = SGErrorCodeInitFileParamIll;
		sprintf(m_LastErrorInfo.strErrorDescription, "�����ļ��������󣺲ɼ��ٶ� ���Ϸ���������ֵ��1 -- 60000��");
		sprintf(m_LastErrorInfo.strErrorRemark, "λ�ã�InitCard()");
		return FALSE;
	}
	if(m_bPreReadImage < 0 || m_bPreReadImage > 1)
	{
		m_LastErrorInfo.nErrorCode = SGErrorCodeInitFileParamIll;
		sprintf(m_LastErrorInfo.strErrorDescription, "�����ļ����������Ƿ���Ԥ��ģʽ ���Ϸ���������ֵ��0 -- 1��");
		sprintf(m_LastErrorInfo.strErrorRemark, "λ�ã�InitCard()");
		return FALSE;
	}
	if(m_bPreReadImage == TRUE && (m_nImageNum < 1 || m_nImageNum > SGMaxImageNum))
	{
		m_LastErrorInfo.nErrorCode = SGErrorCodeInitFileParamIll;
		sprintf(m_LastErrorInfo.strErrorDescription, "�����ļ���������ͼ������ ���Ϸ���������ֵ��1 -- %d��", SGMaxImageNum);
		sprintf(m_LastErrorInfo.strErrorRemark, "λ�ã�InitCard()");
		return FALSE;
	}
	if(m_bPlaneRGB == TRUE && m_nImageByteCount != 3)
	{
		m_LastErrorInfo.nErrorCode = SGErrorCodeInitFileParamIll;
		sprintf(m_LastErrorInfo.strErrorDescription, "�����ļ��������󣺷�ͨ��ʱÿ�����ֽ�������Ϊ3��");
		sprintf(m_LastErrorInfo.strErrorRemark, "λ�ã�InitCard()");
		return FALSE;
	}
	if(m_bLoopGrab < 0 || m_bLoopGrab > 1)// [2008-10-23 by jzj]
	{
		m_LastErrorInfo.nErrorCode = SGErrorCodeInitFileParamIll;
		sprintf(m_LastErrorInfo.strErrorDescription, "�����ļ����������Ƿ�ѭ���ɼ� ���Ϸ���������ֵ��0 -- 1��");
		sprintf(m_LastErrorInfo.strErrorRemark, "λ�ã�InitCard()");
		return FALSE;
	}
	
	BOOL bRet = FALSE;
	CString strWildcard; 
	strWildcard.Format("%s\\*.bmp", m_strImageFileFolder); 
	bRet = m_FileFinder.FindFile(strWildcard);
	if(!bRet)
	{
		m_LastErrorInfo.nErrorCode = SGErrorCodeInitFileParamIll;
		sprintf(m_LastErrorInfo.strErrorDescription, "�����ļ���������%s��û��BMP�ļ���", m_strImageFileFolder);
		sprintf(m_LastErrorInfo.strErrorRemark, "λ�ã�InitCard()");
		return FALSE;
	}
	
	m_nMaxImageWidth = m_nImageWidth;
	m_nMaxImageHeight = m_nImageHeight;
	m_nMaxImageByteCount = m_nImageByteCount;
	
	m_CallBackFunc = pInitCardParam->CallBackFunc;
	m_Context = pInitCardParam->Context;
	
	m_nImageBuffSize = m_nImageWidth * m_nImageHeight * m_nImageByteCount;	

	if(m_bPreReadImage == FALSE)//��Ԥ��ģʽ
	{
		m_pImageBuffer = new BYTE[m_nImageBuffSize];
		if(m_bPlaneRGB)
		{
			m_pPlaneRGBBuffer = new BYTE[m_nImageBuffSize];
		}
	}
	else//Ԥ��ͼ��ģʽ
	{
		static BYTE *pTempConvertBuff = NULL;

		for(int i = 0; i < m_nImageNum; i++)
		{
			m_PreReadImageBuffArray[i] = new BYTE[m_nImageBuffSize];
			
			//////////////////////////////////////////////////////////////////////////
			//��ͼ���ڴ�

			CString strFilePath;
			BOOL bLastFile = FALSE;
			
			bLastFile = !m_FileFinder.FindNextFile();
			strFilePath = m_FileFinder.GetFilePath();
			
			BOOL bRet = OpenBMPFile(strFilePath, m_pBmpInfo, m_PreReadImageBuffArray[i]);
			if(bRet == FALSE)
			{
				m_LastErrorInfo.nErrorCode = SGErrorCodeReadBMPFileFail;
				sprintf(m_LastErrorInfo.strErrorDescription, "��ȡͼ��%sʧ��!", strFilePath);
				sprintf(m_LastErrorInfo.strErrorRemark, "λ�ã�InitCard()");
				
				return FALSE;
			}
			else
			{
				if(m_pBmpInfo->bmiHeader.biWidth != m_nImageWidth
					|| m_pBmpInfo->bmiHeader.biHeight != m_nImageHeight
					|| m_pBmpInfo->bmiHeader.biBitCount != m_nImageByteCount * 8)
				{
					m_LastErrorInfo.nErrorCode = SGErrorCodeInitFileParamIll;
					sprintf(m_LastErrorInfo.strErrorDescription, "%s�ļ���������ѡͼ������", m_sInitFile);
					sprintf(m_LastErrorInfo.strErrorRemark, "λ�ã�InitCard()");
					
					return FALSE;
				}
			}
			
			if(bLastFile)
			{
				m_FileFinder.FindFile(strWildcard);
			}
			//
			//////////////////////////////////////////////////////////////////////////
			
			if(m_bPlaneRGB)
			{
				if(pTempConvertBuff == NULL)
				{
					pTempConvertBuff = new BYTE[m_nImageBuffSize];
				}
				
				CImage myImage;
				myImage.ConvertRGBToPlaneRGB(pTempConvertBuff, m_PreReadImageBuffArray[i], m_nImageWidth, m_nImageHeight);

				BYTE *pTempChangeBuff = NULL;
				pTempChangeBuff = m_PreReadImageBuffArray[i];
				m_PreReadImageBuffArray[i] = pTempConvertBuff;
				pTempConvertBuff = pTempChangeBuff;
			}
		}
		
		if(pTempConvertBuff != NULL)
		{
			delete []pTempConvertBuff;
			pTempConvertBuff = NULL;
		}		
	}	
	
	m_nCapturedFieldCount = 0;//�ɼ�֡����
	m_bInited = TRUE;
	
	//�����ɼ������߳�
	HANDLE hThread = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ServiceThread, this, 0, &m_dwThreadId);
	::SetThreadPriority(hThread, THREAD_PRIORITY_HIGHEST);

	return TRUE;
}

//�ر�ͼ��
BOOL CSimulateGrabber::CloseCard()
{	
	if(m_bInited == FALSE)
	{
		return TRUE;
	}	

	m_bGrab = FALSE;//�Ƿ�ɼ�ͼ��
	m_bKillServiceThread = TRUE;	
	//m_evtReset.SetEvent();// [2008-1-22 by jzj]

	TRACE("CSimulateCard_Kill\n");
	WaitForSingleObject(m_ServiceThreadDeadEvent, INFINITE);
	TRACE("CSimulateCard_Dead\n");

	if(m_bPreReadImage == FALSE)
	{
		if(m_pImageBuffer != NULL)
		{
			delete []m_pImageBuffer;
			m_pImageBuffer = NULL;
		}
		
		if(m_pPlaneRGBBuffer != NULL)
		{
			delete []m_pPlaneRGBBuffer;
			m_pPlaneRGBBuffer = NULL;
		}
	}
	else
	{
		for(int i = 0; i < m_nImageNum; i++)
		{
			if(m_PreReadImageBuffArray[i] != NULL)
			{
				delete []m_PreReadImageBuffArray[i];
				m_PreReadImageBuffArray[i] = NULL;
			}
		}
	}

	if(m_pBmpInfo != NULL)
	{
		delete m_pBmpInfo;
		m_pBmpInfo = NULL;
	}

	m_bInited = FALSE;
	return TRUE;
}

//�ɼ������߳�
DWORD CSimulateGrabber::ServiceThread(LPVOID lpParam)
{
	CSimulateGrabber* This = (CSimulateGrabber*)lpParam;
	
	DWORD dwRet = 0;
	if(This->m_bPreReadImage == TRUE)
	{
		This->PreReadImageServiceThreadFunc();
	}
	else
	{
		This->ServiceThreadFunc();
	}

	return dwRet;
}

//�ɼ������̺߳���
DWORD CSimulateGrabber::ServiceThreadFunc()
{
	BOOL bRet = FALSE;
	CString strWildcard; 
	strWildcard.Format("%s\\*.bmp", m_strImageFileFolder); 
	bRet = m_FileFinder.FindFile(strWildcard);
	if(!bRet)
	{
		m_LastErrorInfo.nErrorCode = SGErrorCodeInitFileParamIll;
		sprintf(m_LastErrorInfo.strErrorDescription, "%s��û��BMP�ļ���", m_strImageFileFolder);
		sprintf(m_LastErrorInfo.strErrorRemark, "λ�ã�ServiceThreadFunc()");
		
		s_GBSIGNALINFO SignalInfo;
		SignalInfo.iGrabberTypeSN = m_iGrabberTypeSN;// [2009-12-18 by jzj]: add
		SignalInfo.Context = m_Context;
		SignalInfo.nGrabberSN = m_nCardSN;
		SignalInfo.nErrorCode = m_LastErrorInfo.nErrorCode;
		SignalInfo.nFrameCount = 0;
		sprintf(SignalInfo.strDescription, m_LastErrorInfo.strErrorDescription);
		m_CallBackFunc(&SignalInfo);//���ûص������Դ�����д���

		m_bImagesOK = FALSE;
	}
	else
	{
		m_bImagesOK = TRUE;
	}
	
	CSpendTime mySpendTime;// [2008-1-22 by jzj]
	mySpendTime.Start();// [2008-1-22 by jzj]

	while(!m_bKillServiceThread)
	{	
		if(m_bUpdateImagesFolder)
		{
			m_bUpdateImagesFolder = FALSE;
			
			strWildcard.Format("%s\\*.bmp", m_strImageFileFolder); 
			bRet = m_FileFinder.FindFile(strWildcard);
			if(!bRet)
			{
				m_LastErrorInfo.nErrorCode = SGErrorCodeReadBMPFileFail;
				sprintf(m_LastErrorInfo.strErrorDescription, "%s��û��BMP�ļ���", m_strImageFileFolder);
				sprintf(m_LastErrorInfo.strErrorRemark, "λ�ã�ServiceThreadFunc()");
				
				s_GBSIGNALINFO SignalInfo;
				SignalInfo.iGrabberTypeSN = m_iGrabberTypeSN;// [2009-12-18 by jzj]: add
				SignalInfo.Context = m_Context;
				SignalInfo.nGrabberSN = m_nCardSN;
				SignalInfo.nErrorCode = m_LastErrorInfo.nErrorCode;
				SignalInfo.nFrameCount = 0;
				sprintf(SignalInfo.strDescription, m_LastErrorInfo.strErrorDescription);
				m_CallBackFunc(&SignalInfo);//���ûص������Դ�����д���
				
				m_bImagesOK = FALSE;
			}
			else
			{
				m_bImagesOK = TRUE;
			}
		}

		if(m_bImagesOK == FALSE)
		{
			Sleep(10);
			continue;
		}
		
		if(m_bGrab)
		{
			static int iLostTime = 0;// here!!!

			mySpendTime.End();// [2008-1-22 by jzj]
			if(mySpendTime.GetMillisecondInt() >= m_nGrabSpeed - iLostTime)// �������һ֡ͼ // [2008-1-22 by jzj]
			{
				m_bGrabbing = TRUE;
				
				CString strFilePath;
				BOOL bLastFile = FALSE;
				
				bLastFile = !m_FileFinder.FindNextFile();
				strFilePath = m_FileFinder.GetFilePath();
				
				BOOL bRet = OpenBMPFile(strFilePath, m_pBmpInfo, m_pImageBuffer);
				if(bRet == FALSE)
				{
					m_LastErrorInfo.nErrorCode = SGErrorCodeReadBMPFileFail;
					sprintf(m_LastErrorInfo.strErrorDescription, "��ȡͼ��%sʧ��!", strFilePath);
					sprintf(m_LastErrorInfo.strErrorRemark, "λ�ã�ServiceThreadFunc()");
					
					s_GBSIGNALINFO SignalInfo;
					SignalInfo.iGrabberTypeSN = m_iGrabberTypeSN;// [2009-12-18 by jzj]: add
					SignalInfo.Context = m_Context;
					SignalInfo.nGrabberSN = m_nCardSN;
					SignalInfo.nErrorCode = m_LastErrorInfo.nErrorCode;
					SignalInfo.nFrameCount = 0;
					sprintf(SignalInfo.strDescription, m_LastErrorInfo.strErrorDescription);
					m_CallBackFunc(&SignalInfo);//���ûص������Դ�����д���
				}
				else
				{
					if(m_pBmpInfo->bmiHeader.biWidth != m_nImageWidth
						|| m_pBmpInfo->bmiHeader.biHeight != m_nImageHeight
						|| m_pBmpInfo->bmiHeader.biBitCount != m_nImageByteCount * 8)
					{
						m_LastErrorInfo.nErrorCode = SGErrorCodeInitFileParamIll;
						sprintf(m_LastErrorInfo.strErrorDescription, "%s�ļ���������ѡͼ������", m_sInitFile);
						sprintf(m_LastErrorInfo.strErrorRemark, "λ�ã�ServiceThreadFunc()");
						
						s_GBSIGNALINFO SignalInfo;
						SignalInfo.iGrabberTypeSN = m_iGrabberTypeSN;// [2009-12-18 by jzj]: add
						SignalInfo.Context = m_Context;
						SignalInfo.nGrabberSN = m_nCardSN;
						SignalInfo.nErrorCode = m_LastErrorInfo.nErrorCode;
						SignalInfo.nFrameCount = 0;
						sprintf(SignalInfo.strDescription, m_LastErrorInfo.strErrorDescription);
						m_CallBackFunc(&SignalInfo);//���ûص������Դ�����д���
					}
					else
					{
						m_nCapturedFieldCount++;
						
						if(m_bPlaneRGB)
						{
							CImage myImage;
							myImage.ConvertRGBToPlaneRGB(m_pPlaneRGBBuffer, m_pImageBuffer, m_nImageWidth, m_nImageHeight);
						}
						
						s_GBSIGNALINFO SignalInfo;
						SignalInfo.iGrabberTypeSN = m_iGrabberTypeSN;// [2009-12-18 by jzj]: add
						SignalInfo.Context = m_Context;
						SignalInfo.nGrabberSN = m_nCardSN;
						SignalInfo.nErrorCode = 0;
						SignalInfo.nFrameCount = m_nCapturedFieldCount;
						sprintf(SignalInfo.strDescription, "%s", m_FileFinder.GetFileName().Left(GBMaxFileNameLen));
						m_CallBackFunc(&SignalInfo);//���ûص�������ͼ����д���
					}
				}
				
				m_bGrabbing = FALSE;

				// [2009-12-28 by jzj]: add
				if(m_bSnap)
				{
					m_bSnap = FALSE;
					m_bGrab = FALSE;
				}
				
				if(bLastFile)
				{
					m_FileFinder.FindFile(strWildcard);

					if(m_bLoopGrab == FALSE)// [2008-10-24 by jzj]
					{
						m_bGrab = FALSE;
					}
				}

				mySpendTime.Start();// [2008-1-22 by jzj]
			}// if(mySpendTime.GetMillisecondInt() >= m_nGrabSpeed)// �������һ֡ͼ // [2008-1-22 by jzj]
		}

		//WaitForSingleObject(m_evtReset, iWaitTime);// [2008-1-22 by jzj]
		Sleep(1);// [2008-1-22 by jzj]
	}

	m_ServiceThreadDeadEvent.SetEvent();
	return 0;
}

//Ԥ��ͼ��Ĳɼ������̺߳���
DWORD CSimulateGrabber::PreReadImageServiceThreadFunc()
{	
	CSpendTime mySpendTime;// [2008-1-22 by jzj]
	mySpendTime.Start();// [2008-1-22 by jzj]
	
	int nPosition = 0;// [2008-12-4 by jzj]: add

	while(!m_bKillServiceThread)
	{	
		if(m_bResetPosition == TRUE)// [2008-12-4 by jzj]: add
		{
			m_bResetPosition = FALSE;	
			nPosition = 0;
		}

		if(m_bGrab)
		{
			mySpendTime.End();// [2008-1-22 by jzj]
			if(mySpendTime.GetMillisecondInt() >= m_nGrabSpeed)// �������һ֡ͼ // [2008-1-22 by jzj]
			{
				//int nPosition = 0;// [2008-12-4 by jzj]: delete
				
				m_bGrabbing = TRUE;
				
				//nPosition = m_nCapturedFieldCount % m_nImageNum;// [2008-12-4 by jzj]: delete
				m_pImageBuffer = m_PreReadImageBuffArray[nPosition];
				
				if(m_pImageBuffer == NULL)
				{
					m_LastErrorInfo.nErrorCode = SGErrorCodeReadImageFromMemFail;
					sprintf(m_LastErrorInfo.strErrorDescription, "���ڴ��ж�ȡͼ��ʧ��!");
					sprintf(m_LastErrorInfo.strErrorRemark, "λ�ã�PreReadImageServiceThreadFunc()");
					
					s_GBSIGNALINFO SignalInfo;
					SignalInfo.iGrabberTypeSN = m_iGrabberTypeSN;// [2009-12-18 by jzj]: add
					SignalInfo.Context = m_Context;
					SignalInfo.nGrabberSN = m_nCardSN;
					SignalInfo.nErrorCode = m_LastErrorInfo.nErrorCode;
					SignalInfo.nFrameCount = 0;
					sprintf(SignalInfo.strDescription, m_LastErrorInfo.strErrorDescription);
					m_CallBackFunc(&SignalInfo);//���ûص������Դ�����д���
				}
				else
				{
					m_nCapturedFieldCount++;				
					
					if(m_bPlaneRGB)
					{
						m_pPlaneRGBBuffer = m_pImageBuffer;
					}
					
					s_GBSIGNALINFO SignalInfo;
					SignalInfo.iGrabberTypeSN = m_iGrabberTypeSN;// [2009-12-18 by jzj]: add
					SignalInfo.Context = m_Context;
					SignalInfo.nGrabberSN = m_nCardSN;
					SignalInfo.nErrorCode = 0;
					SignalInfo.nFrameCount = m_nCapturedFieldCount;
					sprintf(SignalInfo.strDescription, "%s", m_strImageFileNameArray[nPosition].Left(GBMaxFileNameLen));
					m_CallBackFunc(&SignalInfo);//���ûص�������ͼ����д���
				}	
				
				m_bGrabbing = FALSE;

				// [2009-12-28 by jzj]: add
				if(m_bSnap)
				{
					m_bSnap = FALSE;
					m_bGrab = FALSE;
				}

				// [2008-12-4 by jzj]: add				
				nPosition++;
				if(nPosition >= m_nImageNum)
				{
					nPosition = 0;
					
					if(m_bLoopGrab == FALSE)
					{
						m_bGrab = FALSE;
					}
				}
				
				mySpendTime.Start();// [2008-1-22 by jzj]
			}// if(mySpendTime.GetMillisecondInt() >= m_nGrabSpeed)// �������һ֡ͼ // [2008-1-22 by jzj]
		}

		//WaitForSingleObject(m_evtReset, m_nGrabSpeed);// [2008-1-22 by jzj]
		Sleep(1);// [2008-1-22 by jzj]
	}

	m_ServiceThreadDeadEvent.SetEvent();
	return 0;
}

//�õ�����
//BOOL CSimulateGrabber::GetParamInt(SGPARAMID Param, PINT32 nOutputVal)
BOOL CSimulateGrabber::GetParamInt(SGPARAMID Param, int& nOutputVal)// [2009-12-17 by jzj]: modify
{
	if(m_bInited == FALSE)
	{
		m_LastErrorInfo.nErrorCode = SGErrorCodeNoInit;
		sprintf(m_LastErrorInfo.strErrorDescription, "����δ��ʼ�������ܽ��иò�����");
		sprintf(m_LastErrorInfo.strErrorRemark, "λ�ã�GetParamInt(SGPARAMID Param, PINT32 nOutputVal)");
		return FALSE;
	}

	switch(Param)
	{
	case SG_IMAGE_WIDTH:
		//*nOutputVal = m_nImageWidth;// [2009-12-17 by jzj]: modify
		nOutputVal = m_nImageWidth;
		break;
	case SG_IMAGE_HEIGHT:
		//*nOutputVal = m_nImageHeight;// [2009-12-17 by jzj]: modify
		nOutputVal = m_nImageHeight;
		break;
	case SG_IMAGE_BYTE_COUNT:
		//*nOutputVal = m_nImageByteCount;// [2009-12-17 by jzj]: modify
		nOutputVal = m_nImageByteCount;
		break;
	case SG_IMAGE_BUFFER_SIZE:
		//*nOutputVal = m_nImageBuffSize;// [2009-12-17 by jzj]: modify
		nOutputVal = m_nImageBuffSize;
		break;
	case SG_IMAGE_BUFFER_ADDR:
		{
			if(m_bPlaneRGB)
			{
				//*nOutputVal = (int)m_pPlaneRGBBuffer;// [2009-12-17 by jzj]: modify
				nOutputVal = (int)m_pPlaneRGBBuffer;
			}
			else
			{
				//*nOutputVal = (int)m_pImageBuffer;// [2009-12-17 by jzj]: modify
				nOutputVal = (int)m_pImageBuffer;
			}
		}
		break;
	case SG_IS_PLANERGB:
		//*nOutputVal = m_bPlaneRGB;// [2009-12-17 by jzj]: modify
		nOutputVal = m_bPlaneRGB;
		break;
	case SG_GRAB_SPEED:
		//*nOutputVal = m_nGrabSpeed;// [2009-12-17 by jzj]: modify
		nOutputVal = m_nGrabSpeed;
		break;
	default:
		m_LastErrorInfo.nErrorCode = SGErrorCodeParamIll;
		sprintf(m_LastErrorInfo.strErrorDescription, "�������Ϸ���");
		sprintf(m_LastErrorInfo.strErrorRemark, "λ�ã�GetParamInt(SGPARAMID Param, PINT32 nOutputVal)");
		return FALSE;
	}

	return TRUE;
}

//���ò���
BOOL CSimulateGrabber::SetParamInt(SGPARAMID Param, int nInputVal)
{
	if(m_bInited == FALSE)
	{
		m_LastErrorInfo.nErrorCode = SGErrorCodeNoInit;
		sprintf(m_LastErrorInfo.strErrorDescription, "����δ��ʼ�������ܽ��иò�����");
		sprintf(m_LastErrorInfo.strErrorRemark, "λ�ã�SetParamInt(SGPARAMID Param, int nInputVal)");
		return FALSE;
	}
	
	switch(Param)
	{
	case SG_IS_PLANERGB://�ģ�����У��
		{
			BOOL bPlaneRGB = nInputVal;
			if(bPlaneRGB)
			{
				if(m_nImageByteCount != 3)
				{
					return FALSE;
				}

				if(m_pPlaneRGBBuffer == NULL)
				{
					m_pPlaneRGBBuffer = new BYTE[m_nImageBuffSize];
				}
			}				
			m_bPlaneRGB = bPlaneRGB;
		}
		break;
	case SG_GRAB_SPEED://�ģ�����У��
		m_nGrabSpeed = nInputVal;
		//m_evtReset.SetEvent();// [2008-1-22 by jzj]
		break;
	case SG_IMAGE_WIDTH:
		if(nInputVal > m_nMaxImageWidth)
		{
			m_LastErrorInfo.nErrorCode = SGErrorCodeParamIll;
			sprintf(m_LastErrorInfo.strErrorDescription, "�������Ϸ������ӦС�ڵ���%d", m_nImageWidth);
			sprintf(m_LastErrorInfo.strErrorRemark, "λ�ã�SetParamInt(SGPARAMID Param, int nInputVal)");
			return FALSE;
		}

		m_nImageWidth = nInputVal;
		break;
	case SG_IMAGE_HEIGHT:
		if(nInputVal > m_nMaxImageHeight)
		{
			m_LastErrorInfo.nErrorCode = SGErrorCodeParamIll;
			sprintf(m_LastErrorInfo.strErrorDescription, "�������Ϸ����߶�ӦС�ڵ���%d", m_nImageHeight);
			sprintf(m_LastErrorInfo.strErrorRemark, "λ�ã�SetParamInt(SGPARAMID Param, int nInputVal)");
			return FALSE;
		}

		m_nImageHeight = nInputVal;
		break;
	case SG_IMAGE_BYTE_COUNT:
		if(nInputVal > m_nMaxImageByteCount)
		{
			m_LastErrorInfo.nErrorCode = SGErrorCodeParamIll;
			sprintf(m_LastErrorInfo.strErrorDescription, "�������Ϸ���ÿ�����ֽ���ӦС�ڵ���%d", m_nImageByteCount);
			sprintf(m_LastErrorInfo.strErrorRemark, "λ�ã�SetParamInt(SGPARAMID Param, int nInputVal)");
			return FALSE;
		}

		m_nImageByteCount = nInputVal;
		break;
	default:
		m_LastErrorInfo.nErrorCode = SGErrorCodeParamIll;
		sprintf(m_LastErrorInfo.strErrorDescription, "�������Ϸ���");
		sprintf(m_LastErrorInfo.strErrorRemark, "λ�ã�SetParamInt(SGPARAMID Param, int nInputVal)");
		return FALSE;
	}
	
	return TRUE;
}
//
//////////////////////////////////////////////////////////////////////////