// HighResElapsedTimer.h: interface for the HighResElapsedTimer class.
//
//////////////////////////////////////////////////////////////////////

// Original:
// http://www.codeproject.com/KB/datetime/highreselapsedtimer.aspx


#if !defined(HIGHRESELAPSEDTIMER_H_INCLUDED_)
#define HIGHRESELAPSEDTIMER_H_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>

class HighResElapsedTimer  
{
public:
	HighResElapsedTimer()
	{
		if (m_llFrequency == 0)			// frequency value not yet set
		{
			LARGE_INTEGER	liFrequency;

			QueryPerformanceFrequency(&liFrequency);
			m_llFrequency = liFrequency.QuadPart;
		}

	}

	virtual ~HighResElapsedTimer()
	{
			
	}

	void StartTimer()
	{
		QueryPerformanceCounter(&m_llCounter);
	}

	double GetDurationInMicroSeconds()
	{
		LARGE_INTEGER		liNow;
		QueryPerformanceCounter(&liNow);
		double				duration = (double)(liNow.QuadPart - m_llCounter.QuadPart)/m_llFrequency;

		return duration;
//		TRACE(_T("Elapsed time = %.3lf microseconds\n"), duration);
	}

protected:
	LARGE_INTEGER	m_llCounter;
	static LONGLONG	m_llFrequency;		// Frequency setting is based hardware clock which doesn't
										// does change - so we want to set this only once

private:
};

#endif	// !defined(HIGHRESELAPSEDTIMER_H_INCLUDED_)
