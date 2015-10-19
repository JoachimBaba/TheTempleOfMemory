#pragma once

#define EGfxColor_Azur GfxColor(0, 255, 255, 255)
#define EGfxColor_Orange GfxColor(255, 128, 0, 255)
#define EGfxColor_Yellow GfxColor(255, 255, 0, 255)
#define EGfxColor_Grey GfxColor(128, 128, 128, 255)

const int SCREEN_SIZE_X = 480;
const int SCREEN_SIZE_Y = 320;

//Array Sizes
const int ROOM_MAX = 16;
const int ITEM_MAX = 3;
const int ENNEMY_MAX = 5;
const int OBSTACLE_MAX = 8;
const int STEP_MAX = 10;
const int PATTERN_MAX = 9;
const int BG_COUNT = 3;
const int BUTTON_MAX = 10;
const unsigned char LIFE_MAX = 3;
const int START_ROOM_COUNT = 5;
const int TEXT_ID_SIZE = 64;
const int ROOM_SOUND_COUNT = 10;

const int BUTTON_NULL = BUTTON_MAX+1;

const int KEY_HAZARD = 5;
const int ITEM_CHANCE_MAX = 2;

//Delay
const int TRANSITION_DELAY_MAX = 250;
const int TEXT_INFO_DURATION = 2000;
const int HEALING_DURATION = 2000;
const int DYING_DURATION = 2500;
const int WANDER_TRAVEL_MIN = 500;
const int WANDER_TRAVEL_MAX= 3000;

//Objects Radius
const int CIRCLE_SEG_MAX = 18;//dbg

//feed back
const int FEEDBACK_RANGE = 5;//(every X room that feedback is called)
const float fFEEDBACK_SIZE = 2.f;

const float fCONSUMED_ITEM_RANGE = 50.f;
const float fFOLLOWER_SPEED = 40.f;

const float fPLAYER_RADIUS = 16.0f;
const float fDOOR_RADIUS_X = 30.0f;
const float fDOOR_RADIUS_Y = 25.0f;
const float fWALL_RADIUS_X_HOR = 145.0f;
const float fWALL_RADIUS_X_VERT = 130.0f;
const float fWALL_RADIUS_Y = 10.0f;
const float fITEMRADIUS = 10.0f;
const float fHUNTER_RADIUS = 16.0f;
const float fLIFEPOINTS_RADIUS = 15.f;

//Positions & Margin
const float fBACKGROUND_POS_X = 0.0f;
const float fBACKGROUND_POS_Y = 0.0f;

const float fMENU_BUTTON_RADIUS_X = 100.0f;
const float fMENU_BUTTON_RADIUS_Y = 25.0f;
const float fMENU_BUTTON_MARGIN = 20.0f;

const float fTROPHY_MARGIN = 20.f;

const float fMENU_BUTTON_POS_X = 270.0f;
const float fMENU_BUTTON_POS_Y = 65.0f;

const float fHUD_BUTTON_RADIUS = 30.0f;
const float fHUD_BUTTON_MARGIN = 20.0f;

const float fHUD_BUTTON_POS_X = 0.0f;
const float fHUD_BUTTON_POS_Y = 25.0f;

const float fHUD_ROOM_INFO_MARGIN_X = 40.0f;
const float fHUD_ROOM_INFO_MARGIN_Y = 10.0f;

const float fDOOR_MARGIN_X = -35.0f;
const float fDOOR_MARGIN_Y = -35.0f;

const float fWALL_MARGIN_X = 80.0f;
const float fWALL_MARGIN_Y = 20.0f;

const float fLIFEPOINTS_POS_X = 310.0f;
const float fLIFEPOINTS_POS_Y = 300.0f;

const float fLIFEPOINTS_MARGIN = 40.0f;

const float fITEMS_RANGE = 40.0f;

const float fPLAYER_INPUT_LIMIT = 50.f;

//Speeds
const float fDEFAULT_SPEED = 250.0f;
const float fHUNTER_SPEED = 100.0f;

//Animation
const float fANIM_DELAY_MAX = 0.05f;
const float fFADE_DURATION = 500.f;

