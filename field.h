#ifndef FIELD_H_INCLUDED
#define FIELD_H_INCLUDED

#include "Grid.h"
#include "terrain.h"
#include "unit.h"

/* Battle field */
class Field {
public:
    // Constructor
    Field(int h, int w);
    // Destructor
    ~Field();

    // Get the height and width of the field
    size_t getHeight() const;
    size_t getWidth() const;

    // Get the unit at row and col
    Unit* getUnit(int row, int col) const;
    void set_Unit(Unit* u, int row, int col);

    // Get the terrain at row and col
    Terrain getTerrain(int row, int col) const;
    void set_Terrain(Terrain t, int row, int col);

    // Move a unit to a different coordinate
    // Return false if the target coordinate is already occupied
    bool moveUnit(int srow, int scol, int trow, int tcol);

private:
    // Store the units
    Grid<Unit*> units;
    // Store the terrains
    Grid<Terrain> terrains;
};

#endif // FIELD_H_INCLUDED
