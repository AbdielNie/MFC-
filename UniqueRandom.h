// UniqueRandom.h: interface for the CUniqueRandom class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UNIQUERANDOM_H__DB11D0B2_3B81_4FD4_BD4D_9F104A7561D3__INCLUDED_)
#define AFX_UNIQUERANDOM_H__DB11D0B2_3B81_4FD4_BD4D_9F104A7561D3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CUniqueRandom  
{
public:
	CUniqueRandom(int bound = 1000);
	virtual ~CUniqueRandom(); 

	//random's return-value is between 1 and boundary if succuss;else -1; 
	int		random();
	void	reset();
	bool	allUsed() const;
	int		getBoundary() const;

protected:
	const int m_boundary;	//the max number that will be generated.
	int *m_map;			//used for generating unique number
	int m_times;			//record the amount of numbers that have been used

};

#endif // !defined(AFX_UNIQUERANDOM_H__DB11D0B2_3B81_4FD4_BD4D_9F104A7561D3__INCLUDED_)
