/************************************************************************/
/* Copyright (c) 2007, �й���㼯�ű���ͼ���Ӿ������ֹ�˾�Ӿ�ϵͳ��  
/* All rights reserved.													
/*																		
/* �ļ����ƣ� GrabberExportForSG.cpp
/* ժҪ�� �����ɼ�����
/*
/* ��ǰ�汾�� 3.0
/* �޸��ߣ� ��׿��
/* �޸�����ժҪ����ʼ���ṹ�弰�ɼ��ص��ṹ����롰�ɼ������ͱ�š���// [2009-12-18 by jzj]: add
/* ������ڣ� 2009��12��18��
/*
/* ��ǰ�汾�� 2.0
/* �޸��ߣ� ��׿��
/* �޸�����ժҪ�������Ͳɼ�����ʹ�ò�ͬ����������CDHGrabberSG��������Ӧһ�������ö��ֲɼ�����÷���
				 ʹ��ʾ����CGrabber* pGrabber = new CDHGrabberSG; pGrabber->StartGrab(); ��
/* ������ڣ� 2009��12��14��
/* 
/* ��ǰ�汾�� 1.1
/* �޸��ߣ� ��׿��
/* �޸�����ժҪ�����뵥֡�ɼ��ӿ�Snapshot()��
/* ������ڣ� 2007��11��05��
/*
/* ��ǰ�汾�� 1.0
/* ���ߣ� ��׿��
/* ������ڣ� 2006��07��27��
/************************************************************************/

#include "stdafx.h"
#include "DHGrabberForSG.h"

#include "SimulateGrabber.h"

CDHGrabberSG::CDHGrabberSG()
{
	m_pGrabber = new CSimulateGrabber;
}

CDHGrabberSG::~CDHGrabberSG()
{
	if(m_pGrabber != NULL)
	{
		delete m_pGrabber;
		m_pGrabber = NULL;
	}
}
	
//��ʼ��
BOOL CDHGrabberSG::Init(const s_GBINITSTRUCT* pInitParam)
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->Init(pInitParam));
}

//�ر�
BOOL CDHGrabberSG::Close()
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->Close());
}

//��ʼ�ɼ�
BOOL CDHGrabberSG::StartGrab()
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->StartGrab());
}

//ֹͣ�ɼ�
BOOL CDHGrabberSG::StopGrab()
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->StopGrab());
}

//��֡�ɼ�
BOOL CDHGrabberSG::Snapshot()
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->Snapshot());
}
	
//�õ�����
BOOL CDHGrabberSG::GetParamInt(GBParamID Param, int &nOutputVal)
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->GetParamInt(Param, nOutputVal));
}
	
//���ò���
BOOL CDHGrabberSG::SetParamInt(GBParamID Param, int nInputVal)
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->SetParamInt(Param, nInputVal));
}

//���ò����Ի���
void CDHGrabberSG::CallParamDialog()
{
	ASSERT(m_pGrabber != NULL);
	m_pGrabber->CallParamDialog();
}

//�õ�������Ϣ
void CDHGrabberSG::GetLastErrorInfo(s_GBERRORINFO *pErrorInfo)
{
	ASSERT(m_pGrabber != NULL);
	m_pGrabber->GetLastErrorInfo(pErrorInfo);
}

//////////////////////////////////////////////////////////////////////////
//ģ��ɼ���ר�нӿ�
	
// [2009-12-17 by jzj]: add
//���ò���
BOOL CDHGrabberSG::SGSetParamInt(SGParamID Param, int nInputVal)
{
	ASSERT(m_pGrabber != NULL);
	return(((CSimulateGrabber *)m_pGrabber)->SetParamInt(Param, nInputVal));
}	

// [2009-12-17 by jzj]: add
//�õ�����
BOOL CDHGrabberSG::SGGetParamInt(SGParamID Param, int &nOutputVal)
{
	ASSERT(m_pGrabber != NULL);
	return(((CSimulateGrabber *)m_pGrabber)->GetParamInt(Param, nOutputVal));
}
//
//////////////////////////////////////////////////////////////////////////
