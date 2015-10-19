#pragma once

#include "circle.h"

enum EDynamicState
{
	EDynamicState_Active,
	EDynamicState_Hurt,
	EDynamicState_Dying
};

enum EDynamicAnimState
{
	EDynamicAnimState_Walking,
	EDynamicAnimState_Idle,
	EDynamicAnimState_Death,

	EDynamicAnimState_Count
};

enum EEnnemyType
{
	EEnnemyType_None,
	EEnnemyType_Sentinel,//go from a spot to another
	EEnnemyType_Chaser,//hunt the player
	EEnnemyType_Wanderer,//Walk around in the room

	EEnnemyType_Count
};

class TDynamic : public TCircle
{
public:
	TDynamic( const TGfxVec2 & tPos, float fOrient, float fRadius, float fSpeed );
	virtual ~TDynamic();

	virtual void Update();

	virtual void Move();

	float GetSpeed()const { return m_fSpeed; };
	const TGfxVec2 & GetDir()const { return m_tDir; };
	void SetSpeed(const float fSpeed) { m_fSpeed = fSpeed; };
	void SetDir(const TGfxVec2 & tDir) { m_tDir = tDir; };

	virtual void Repulse(const TGfxVec2 & tPos);
	bool ReachPoint(const TGfxVec2 & tPos);
	void ChangeDir();//set to a random cardinal direction

	void SetState( const EDynamicState eState ) { m_eState = eState; };
	EDynamicState GetState()const { return m_eState; };
	
protected:
	TGfxVec2 m_tDir;
	float m_fSpeed;
	EDynamicState m_eState;

	virtual void Die();
};