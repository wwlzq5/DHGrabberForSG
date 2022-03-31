/************************************************************************/
/* Copyright (c) 2007, 中国大恒集团北京图像视觉技术分公司视觉系统部  
/* All rights reserved.													
/*																		
/* 文件名称： GrabberExportForSG.cpp
/* 摘要： 导出采集卡类
/*
/* 当前版本： 3.0
/* 修改者： 江卓佳
/* 修改内容摘要：初始化结构体及采集回调结构体加入“采集卡类型编号”。// [2009-12-18 by jzj]: add
/* 完成日期： 2009年12月18日
/*
/* 以前版本： 2.0
/* 修改者： 江卓佳
/* 修改内容摘要：各类型采集对象使用不同的类名（如CDHGrabberSG），以适应一个程序用多种采集类的用法。
				 使用示例：CGrabber* pGrabber = new CDHGrabberSG; pGrabber->StartGrab(); 。
/* 完成日期： 2009年12月14日
/* 
/* 以前版本： 1.1
/* 修改者： 江卓佳
/* 修改内容摘要：加入单帧采集接口Snapshot()。
/* 完成日期： 2007年11月05日
/*
/* 当前版本： 1.0
/* 作者： 江卓佳
/* 完成日期： 2006年07月27日
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
	
//初始化
BOOL CDHGrabberSG::Init(const s_GBINITSTRUCT* pInitParam)
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->Init(pInitParam));
}

//关闭
BOOL CDHGrabberSG::Close()
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->Close());
}

//开始采集
BOOL CDHGrabberSG::StartGrab()
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->StartGrab());
}

//停止采集
BOOL CDHGrabberSG::StopGrab()
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->StopGrab());
}

//单帧采集
BOOL CDHGrabberSG::Snapshot()
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->Snapshot());
}
	
//得到参数
BOOL CDHGrabberSG::GetParamInt(GBParamID Param, int &nOutputVal)
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->GetParamInt(Param, nOutputVal));
}
	
//设置参数
BOOL CDHGrabberSG::SetParamInt(GBParamID Param, int nInputVal)
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->SetParamInt(Param, nInputVal));
}

//调用参数对话框
void CDHGrabberSG::CallParamDialog()
{
	ASSERT(m_pGrabber != NULL);
	m_pGrabber->CallParamDialog();
}

//得到出错信息
void CDHGrabberSG::GetLastErrorInfo(s_GBERRORINFO *pErrorInfo)
{
	ASSERT(m_pGrabber != NULL);
	m_pGrabber->GetLastErrorInfo(pErrorInfo);
}

//////////////////////////////////////////////////////////////////////////
//模拟采集卡专有接口
	
// [2009-12-17 by jzj]: add
//设置参数
BOOL CDHGrabberSG::SGSetParamInt(SGParamID Param, int nInputVal)
{
	ASSERT(m_pGrabber != NULL);
	return(((CSimulateGrabber *)m_pGrabber)->SetParamInt(Param, nInputVal));
}	

// [2009-12-17 by jzj]: add
//得到参数
BOOL CDHGrabberSG::SGGetParamInt(SGParamID Param, int &nOutputVal)
{
	ASSERT(m_pGrabber != NULL);
	return(((CSimulateGrabber *)m_pGrabber)->GetParamInt(Param, nOutputVal));
}
//
//////////////////////////////////////////////////////////////////////////
