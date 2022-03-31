/************************************************************************/
/* Copyright (c) 2007, �й���㼯�ű���ͼ���Ӿ������ֹ�˾�Ӿ�ϵͳ��     
/* All rights reserved.													
/*																		
/* �ļ����ƣ� SimulateGrabber.h	
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

#if !defined(SimulateGrabber_h_)
#define SimulateGrabber_h_

#include "DHGrabberForSG.h"

//////////////////////////////////////////////////////////////////////////
// ͷ�ļ�

#include <afxmt.h>          //Ϊ��ʹ��CEvent
//
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// ����

const int SGMaxImageNum = 20;		//Ԥ��ͼ��ģʽʱ���ͼ������//[2006-08-23]

typedef struct _SgSignalInfoStruct//�ص���Ϣ�ṹ
{
	PVOID Context;	//���ӵ�лص������Ķ����thisָ��
	BOOL bBuffProcessing;	//���������ڴ���		
	int nFrameCount;	//�Ѳɼ���֡����
	CString strDescription; //������Ϣ

}s_SGSIGNALINFO;		//�ص���Ϣ�ṹ

//�ص�����ָ������
typedef void (WINAPI *PSGCALLBACK)(const s_SGSIGNALINFO* SignalInfo);

typedef struct _SgInitCardStruct//ͼ�񿨲�����ʼ���ṹ
{
	int CardSN;								//ͼ�����
	char sInitFile[GBMaxFileNameLen];		//���ڳ�ʼ�����ļ�
	PGBCALLBACK CallBackFunc;				//�ص�����ָ��//new
	PVOID Context;							//��ŵ��ó�ʼ�������Ķ����thisָ��

}s_SGINITCARDSTRUCT;		//ͼ�񿨲�����ʼ���ṹ

typedef int SGPARAMID;	//����ID
//typedef signed int* PINT32;// [2009-12-17 by jzj]: delete

// PARAM ID					// [2009-12-17 by jzj]: modify
//#define SG_IMAGE_WIDTH							0	//ͼ���� (��λ:�ֽ�)
//#define SG_IMAGE_HEIGHT							1	//ͼ��߶� (��λ:�ֽ�)
//#define SG_IMAGE_BYTE_COUNT						2	//ͼ�����ش�С (��λ:�ֽ�)
//#define SG_IMAGE_BUFFER_SIZE					3	//ͼ�񻺳�����С (��λ:�ֽ�)
//#define SG_IMAGE_BUFFER_ADDR					4	//ͼ�񻺳�����ַ
//#define SG_IS_PLANERGB							5	//�Ƿ��ͨ����0����1���ǣ�
//#define SG_GRAB_SPEED							6   //�ɼ��ٶȣ���λ������/�ţ�					
#define SG_IS_PLANERGB							0	//�Ƿ��ͨ����0����1���ǣ�
#define SG_GRAB_SPEED							1   //�ɼ��ٶȣ���λ������/�ţ�
#define SG_IMAGE_WIDTH							2	//ͼ���� (��λ:�ֽ�)
#define SG_IMAGE_HEIGHT							3	//ͼ��߶� (��λ:�ֽ�)
#define SG_IMAGE_BYTE_COUNT						4	//ͼ�����ش�С (��λ:�ֽ�)
#define SG_IMAGE_BUFFER_SIZE					5	//ͼ�񻺳�����С (��λ:�ֽ�)
#define SG_IMAGE_BUFFER_ADDR					6	//ͼ�񻺳�����ַ
	
//
//////////////////////////////////////////////////////////////////////////

///ģ��ɼ���
class CSimulateGrabber : public CGrabber
{
//����
protected:
	//////////////////////////////////////////////////////////////////////////
	// �ڲ�����

	int m_nCardSN;							//ͼ�����
	char m_sInitFile[GBMaxFileNameLen];		//���ڳ�ʼ���ĸ�ʽ�ļ�
	PGBCALLBACK m_CallBackFunc;				//�ص�����ָ��
	PVOID m_Context;						//��ŵ��ó�ʼ�������Ķ����thisָ��
	s_SGSIGNALINFO m_SignalInfo;			//�ص���Ϣ
	
	BYTE* m_pImageBuffer;					//�����洦���õ�ͼ�񻺳���
	int m_nImageWidth;						//ͼ���� (��λ:�ֽ�)
	int m_nImageHeight;						//ͼ��߶� (��λ:�ֽ�)
	int m_nImageByteCount;					//ͼ�����ش�С (��λ:�ֽ�)
	int m_nImageBuffSize;					//ͼ�񻺳�����С (��λ:�ֽ�)
	BOOL m_bPlaneRGB;						//�Ƿ��ͨ��
	BYTE* m_pPlaneRGBBuffer;				//�����洦���õķ�ͨ��ͼ�񻺳���

	BOOL m_bInited;							//ͼ���Ƿ��Ѿ���ʼ��

	BOOL m_bGrab;							//�Ƿ�ɼ�ͼ��
	BOOL m_bGrabbing;						//�Ƿ����ڲɼ�ͼ��
	int m_nGrabSpeed;						//�ɼ��ٶ�(��λ: ����/��)
	CString m_strImageFileFolder;			//ͼ���ļ������ļ���
	
	BOOL m_bSnap;							// �Ƿ�ɼ�һ֡ͼ��// [2009-12-28 by jzj]: add

	//λͼ��Ϣָ��
	BITMAPINFO *m_pBmpInfo;
	
	DWORD   m_dwThreadId;						//�ɼ������߳�ID
	static DWORD ServiceThread(LPVOID lpParam);	//�ɼ������߳�
	DWORD ServiceThreadFunc();					//�ɼ������̺߳���
	DWORD PreReadImageServiceThreadFunc();		//Ԥ��ͼ��Ĳɼ������̺߳���
	BOOL m_bKillServiceThread;
	CEvent m_ServiceThreadDeadEvent;

	CFileFind m_FileFinder;

	int m_nCapturedFieldCount;//�ɼ�֡����
	
	//CEvent m_evtReset;// [2008-1-22 by jzj]

	CString m_strDeviceName;//�豸����

	s_GBERRORINFO m_LastErrorInfo;//������Ϣ

	BOOL m_bPreReadImage;//�Ƿ�Ԥ�ȶ�ͼ���ڴ���
	int m_nImageNum;//ͼ������
	BYTE* m_PreReadImageBuffArray[SGMaxImageNum];//Ԥ��ͼ�񻺳���ָ������
	CString m_strImageFileNameArray[SGMaxImageNum];//Ԥ��ͼ���ļ�������

	BOOL m_bImagesOK;//ͼ���Ƿ���׼����
	BOOL m_bUpdateImagesFolder;//����ͼ�������ļ���
	
	int m_nMaxImageWidth;
	int m_nMaxImageHeight;
	int m_nMaxImageByteCount;

	BOOL m_bLoopGrab;// �Ƿ�ѭ���ɼ�// [2008-10-23 by jzj]

	BOOL m_bResetPosition;// �Ƿ�����Ԥ��λ��// [2008-12-4 by jzj]: add

	int m_iGrabberTypeSN;	// �ɼ������ͱ��// [2009-12-18 by jzj]: add
	//
	//////////////////////////////////////////////////////////////////////////
	
//����
public:
	CSimulateGrabber();
	virtual ~CSimulateGrabber();
	
	//��ʼ��ͼ��
	BOOL Init(const s_GBINITSTRUCT* pInitParam);

	//�ر�ͼ��
	BOOL Close();

	//��ʼ�ɼ�
	BOOL StartGrab();

	//ֹͣ�ɼ�
	BOOL StopGrab();

	//��֡�ɼ�
	BOOL Snapshot();

	//�õ�����
	BOOL GetParamInt(GBParamID Param, int &nOutputVal);

	//���ò���
	BOOL SetParamInt(GBParamID Param, int nInputVal);
	
	//���ò����Ի���
	void CallParamDialog();

	//�õ�������Ϣ
	void GetLastErrorInfo(s_GBERRORINFO *pErrorInfo);

	//////////////////////////////////////////////////////////////////////////
	//ģ��ɼ���ר�ýӿ�

	BOOL InitCard(const s_SGINITCARDSTRUCT* pInitCardParam);
	BOOL CloseCard();
	//BOOL GetParamInt(SGPARAMID Param, PINT32 nOutputVal);
	BOOL GetParamInt(SGPARAMID Param, int& nOutputVal);// [2009-12-17 by jzj]: modify
	BOOL SetParamInt(SGPARAMID Param, int nInputVal);
	//
	//////////////////////////////////////////////////////////////////////////
	
	friend class CSGParamDlg;
};

#endif // !define(SimulateGrabber_h_)