#include "actions.h"
#include "unit.h"
#include <cmath>
#include <cassert>
#include <vector>
using namespace std;

// Get actions that can be performed by a unit
vector<Action> getActions(Unit* u)
{
    vector<Action> acts;
    acts.push_back(MOVE);
    acts.push_back(SKIP);
    return acts;
}

// Check if there is only skip
bool isAllSkip(const vector<Action>& acts)
{
    for (size_t i = 0; i < acts.size(); i++)
        if (acts[i] != SKIP)
            return false;
    return true;
}

// Move units
void moveUnit(Field& field, Unit* u, int trow, int tcol)
{
    assert(u != NULL);
    int srow = u->getRow();
    int scol = u->getCol();

    assert(field.getUnit(srow, scol) == u);
    assert(field.moveUnit(srow, scol, trow, tcol));
}

// Select unit
SelectUResult selectUnit(const Field& field, bool side, int row, int col, Unit*& u)
{
    u = field.getUnit(row, col);
    if (u == nullptr)
        return SELECTU_NO_UNIT;

    if (u->getSide() != side)
        return SELECTU_WRONG_SIDE;

    return SELECTU_SUCCESS;
}
