#ifndef SCREEN_OVERLAY_DATA
#define SCREEN_OVERLAY_DATA

const int OPAQUE = 255;
const int TRANSPARENT = 0;

class TScreenOverlay;

enum EScreenOverlayState
{
	EScreenOverlayState_AllBlack,
	EScreenOverlayState_FadingIn,
	EScreenOverlayState_AllTranslucent,
	EScreenOverlayState_FadingOut,
};

struct TScreenOverlayData
{
	friend class TScreenOverlay;
public:
	TScreenOverlayData();
	TScreenOverlayData(EScreenOverlayState m_eState, float fDelay);
	~TScreenOverlayData();

private:

	float GetMaxDelay()const { return m_fMaxDelay; };

	float m_fDelay;
	float m_fMaxDelay;
	EScreenOverlayState m_eState;
	int m_iStateTime;
	int m_iAlphaLevel;
	int m_iAlphaMax;
	bool m_bSwitchedInCourse;

};

#endif // !SCREEN_OVERLAY_DATA
