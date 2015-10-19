#include "flib.h"
#include "wait.h"


TWait * TWait::CreateDelay()
{
	return new TWait();
}

TWait::TWait()
	:m_iStart(0), m_iDelay(0), m_bActive(false)
{

}

TWait::TWait( const int iStart, const int iDelay, const bool bActive )
	: m_iStart(iStart), m_iDelay(iDelay), m_bActive(bActive)
{

}

TWait::~TWait()
{

}

bool TWait::Update()
{
	if( m_bActive )
	{
		int iTime = GetPassedTime();

		if( iTime >= m_iDelay )
		{
			m_bActive = false;
			return false;
			//GfxDbgPrintf( "delay ends\n" );//dbg
		}
		return true;
	}
	return false;
}

void TWait::Start( const int iDelay )
{
	m_iStart = GfxTimeGetMilliseconds();
	m_iDelay = iDelay;
	m_bActive = true;
	//GfxDbgPrintf( "delay starts\n" );//dbg
}

int TWait::GetPassedTime()
{
	int iDelta = GfxTimeGetMilliseconds();
	return iDelta - m_iStart;
}

float TWait::GetProgress()
{
	if( m_iDelay == 0 )
		return 0.f;

	int iTime = GetPassedTime();
	
	return float( iTime ) / m_iDelay;//normalized value
}
