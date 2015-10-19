#pragma once

#include "flib.h"

struct TTileInfo
{
	int x;
	int y;

	TTileInfo(): x( 0 ), y( 0 ) {};
	TTileInfo( int ix, int iy ): x( ix ), y( iy ){};
	int GetTileCount()const { return x * y; };
};
/*
TTileInfo operator / ( const TTileInfo & t0, const int i )
{
	GfxDbgAssert( i != 0 );
	return TTileInfo( t0.x / i, t0.y / i );
}
*/

