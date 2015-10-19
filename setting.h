#pragma once

#include "singleton.h"
#include "constants.h"

class TXML_Root;
class TXML_Element;
struct TInfo;

class TSetting: public TSingleton < TSetting >
{
	friend class TSingleton < TSetting >;
public:
	TGfxVec2 * GetPattern(int iPattern, int iEnnemy) { return &m_pEnnemiesPatterns[iPattern][iEnnemy][0]; };
	int GetDestinationsCount(int iPattern, int iEnnemy);
	int GetEnnemyCount(int iPattern)const;
	int GetPatternsCount()const { return m_iPatternCount; }

private:
	TSetting();
	~TSetting();

	void CreateRoomsModel(TXML_Element * pRoot);
	void LoadPattern(TXML_Element * pElement);
	void InitPatterns();
	void LoadIds( TXML_Element * pElement );

	TGfxVec2 m_pEnnemiesPatterns[PATTERN_MAX][ENNEMY_MAX][STEP_MAX];
	int m_iPatternsId[PATTERN_MAX];
	TXML_Root * m_pXmlRoot;
	int m_iPatternCount;
};

