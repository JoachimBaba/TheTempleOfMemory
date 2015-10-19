#pragma once

#include "flib.h"
#include "singleton.h"

struct TGfxSound;

enum ESound
{
	ESound_Collect,
	ESound_DoorFailure,
	ESound_DoorSuccess,
	ESound_GameOver,
	ESound_Hit,
	ESound_MenuClick,
	ESound_HudClick,
	ESound_Trophy,
	ESound_Key,
	ESound_Locker,
	ESound_Sword,

	ESound_Count
};

class TSoundSystem : public TSingleton<TSoundSystem>
{
public:
	TSoundSystem();
	~TSoundSystem();

	static void Play( ESound eSound ) { GfxSoundPlay( s_pSounds[eSound] ); };

private:
	static TGfxSound * s_pSounds[ESound_Count];

};

