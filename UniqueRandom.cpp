// UniqueRandom.cpp: implementation of the CUniqueRandom class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UniqueRandom.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUniqueRandom::CUniqueRandom(int bound /*=500*/) : m_boundary(bound), m_times(0)
{
	m_map = new(int[m_boundary]);
	if(!m_map)
	{
		return;
	} 

	for (int i = 0; i < m_boundary; i ++)
	{
		m_map[i] = i + 1;
	}

	srand((unsigned)time(NULL));

}

int CUniqueRandom::random()
{
	if (m_times < m_boundary)
	{
		int rnd = rand() % (m_boundary - m_times);
		int retVal = m_map[rnd];
		m_map[rnd] = m_map[m_boundary - m_times - 1];
		//m_map[m_boundary-m_times-1]=retVal;//unnecessary
		m_times ++;
		return retVal;
	}
	else 
	{
		reset();
		return random();
	}
}

bool CUniqueRandom::allUsed() const
{
	return m_times == m_boundary;
}

void CUniqueRandom::reset()
{
	for (int i = 0; i < m_boundary; i ++)
	{ 
		m_map[i] = i + 1;
	}

	m_times = 0;
}

int CUniqueRandom::getBoundary() const
{
	return m_boundary;
}

CUniqueRandom::~CUniqueRandom()
{
	delete []m_map;
}
