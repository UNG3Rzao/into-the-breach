#include <cassert>
#include "terrain.h"

// Constructor
Terrain::Terrain()
  : ttype(PLAIN) {}

std::string Terrain::getSymbol() {
  	switch (ttype) 
	{
  		case PLAIN:
    		return "  ";
		case MOUNTAIN:
			return "/\\";
		case OCEAN:
			return "~~"; 
		case FOREST:
			return "\\/";
  	}
	
  	// assert(false);
  	return "";
}

TerrainType Terrain::get_ttype() const
{
	return ttype;
}

void Terrain::set_ttype(TerrainType type)
{
	ttype = type;
}