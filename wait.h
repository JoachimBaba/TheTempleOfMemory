#pragma once

struct TWait
{
public:
	static TWait * CreateDelay();
	TWait();
	TWait( const int iStart, const int iDelay, const bool bActive );
	~TWait();
	
	bool Update();
	void SetDelayTime( const int iDelay ) { m_iDelay = iDelay; };
	void SetStartTime( const int iStart ) { m_iStart = iStart; };
	void SetActive( const bool bActive ) { m_bActive = bActive; };
	bool IsActive()const { return m_bActive; };
	void Start( const int iDelay);
	int GetPassedTime();
	float GetProgress();

private:
	int m_iStart;
	int m_iDelay;
	bool m_bActive;
};
