#ifndef TERRAIN_H_INCLUDED
#define TERRAIN_H_INCLUDED

#include <string>

// TerrainType
enum TerrainType {PLAIN, MOUNTAIN, OCEAN, FOREST, };

class Terrain {
public:
    Terrain();

    std::string getSymbol();
    
    //getter and setter
	TerrainType get_ttype() const;
    void set_ttype(TerrainType type);
private:
    TerrainType ttype;
};

#endif // TERRAIN_H_INCLUDED
