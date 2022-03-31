/************************************************************************/
/* Copyright (c) 2007, �й���㼯�ű���ͼ���Ӿ������ֹ�˾�Ӿ�ϵͳ��  
/* All rights reserved.													
/*																		
/* �ļ����ƣ� SpendTime.h
/* ժҪ�� ��ú��뼶�ľ�ȷ��ʱ��
/*
/* ��ǰ�汾�� 1.0
/* ���ߣ� ��׿��
/* ������ڣ� 2007��10��24��
/************************************************************************/

#if !defined(SpendTime_h_)
#define SpendTime_h_
	
class CSpendTime
{
public:
	CSpendTime()
	{
		m_dSpendTime = 0.0;

		QueryPerformanceFrequency(&m_lgiFrequency);
	}

	virtual ~CSpendTime()
	{
	}

	// ��ʼ
	void Start()
	{
		QueryPerformanceCounter(&m_lgiCounterStart);
	}

	// ����
	int End()	// ����ֵ����ʱ����λ�����룩
	{
		QueryPerformanceCounter(&m_lgiCounterEnd);
		
		m_dSpendTime = (m_lgiCounterEnd.QuadPart - m_lgiCounterStart.QuadPart)*1.0 / m_lgiFrequency.QuadPart;

		return (int)m_dSpendTime;
	}
	
	// ��ú�ʱ����λ�����룩
	int GetMillisecondInt()
	{
		return (int)(m_dSpendTime*1000);
	}
	
	// ��ú�ʱ����λ�����룩
	double GetMillisecondDouble()
	{
		return (m_dSpendTime*1000);
	}

protected:
	
	LARGE_INTEGER m_lgiCounterStart;
	LARGE_INTEGER m_lgiCounterEnd;
	LARGE_INTEGER m_lgiFrequency;
	double m_dSpendTime;
};

#endif// !defined(SpendTime)