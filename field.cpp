#include <cassert>
#include "field.h"

// Constructor
Field::Field(int h, int w)
    : units(h, w), terrains(h, w) {}

// Destructor
// Reclaim all the units
Field::~Field()
{
    for (size_t i = 0; i < units.numRows(); i++)
    for (size_t j = 0; j < units.numCols(); j++)
        if (units[i][j] != nullptr)
            delete units[i][j];
}

// Get the height of the field
size_t Field::getHeight() const
{
    return units.numRows();
}

// Get the width of the field
size_t Field::getWidth() const
{
    return units.numCols();
}

// Get the unit at row and col
Unit* Field::getUnit(int row, int col) const
{
    assert (units.inBounds(row, col));

    return units[row][col];
}

// Get the terrain at row and col
Terrain Field::getTerrain(int row, int col) const
{
    return terrains[row][col];
}


bool Field::moveUnit(int srow, int scol, int trow, int tcol)
{
    assert (units.inBounds(srow, scol));
    assert (units.inBounds(trow, tcol));

    return true;
}

void Field::set_Terrain(Terrain t, int row, int col)
{
    terrains[row][col] = t;
}

void Field::set_Unit(Unit* u, int row, int col)
{
    units[row][col] = u;
}