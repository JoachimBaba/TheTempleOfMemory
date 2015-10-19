#include "sound.h"

TGfxSound * TSoundSystem::s_pSounds[ESound_Count];

TSoundSystem::TSoundSystem()
{
	//sounds available at https://drive.google.com/folderview?id=0B3hkebC7EsxPfktkVHpEZ3hpaDV3S1V4Nm0tdm5LX1FPY3NTY25tRk1qdUp0YXFoTnJWTW8&usp=sharing
	s_pSounds[ESound_Collect] = GfxSoundLoad( "sounds/Collect.wav" );
	s_pSounds[ESound_DoorFailure] = GfxSoundLoad( "sounds/DoorFailure.wav" );
	s_pSounds[ESound_DoorSuccess] = GfxSoundLoad( "sounds/DoorSuccess.wav" );
	s_pSounds[ESound_GameOver] = GfxSoundLoad( "sounds/GameOver.wav" );
	s_pSounds[ESound_Hit] = GfxSoundLoad( "sounds/Hit.wav" );
	s_pSounds[ESound_MenuClick] = GfxSoundLoad( "sounds/MenuClick.wav" );
	s_pSounds[ESound_HudClick] = GfxSoundLoad( "sounds/HudClick.wav" );
	s_pSounds[ESound_Trophy] = GfxSoundLoad( "sounds/Trophy.wav" );
	s_pSounds[ESound_Key] = GfxSoundLoad( "sounds/UseKey.wav" );
	s_pSounds[ESound_Locker] = GfxSoundLoad( "sounds/UseLocker.wav" );
	s_pSounds[ESound_Sword] = GfxSoundLoad( "sounds/UseSword.wav" );
}

TSoundSystem::~TSoundSystem()
{

}

