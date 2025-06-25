#ifndef ACTIONS_H_INCLUDED
#define ACTIONS_H_INCLUDED

/******* Actions of Units ********/

#include <vector>
#include "field.h"


/** Actions **/
enum Action {MOVE, ATTACK, SKIP};

/** Attack Types **/
enum AttackType {CLOSE, FAR};

// Get actions that can be performed by a unit
std::vector<Action> getActions(Unit* u);

// Check if there is only SKIP
bool isAllSkip(const std::vector<Action>& acts);

/** Movement **/

// Move the unit at (srow, scol) to (trow, tcol)
void moveUnit(Field& field, Unit* u, int trow, int tcol);

/** Selection **/

// Result of selecting units
enum SelectUResult {SELECTU_SUCCESS, SELECTU_NO_UNIT,
  SELECTU_WRONG_SIDE, SELECTU_NO_ACTABLE};

// Select a unit and return a value
// Return false if I/O failed
// Otherwise, return a message for selection together with the unit
// SELECTU_SUCCESS:     Succeed in selecting a unit
// SELECTU_NO_UNIT:     no unit at (row, col)
// SELECTU_WRONG_SIDE:  Unit at (row, col) is not on our side
// SELECTU_NO_ACTABLE:  no more actable units in this turn
SelectUResult selectUnit(const Field& field, bool side,
                int row, int col, Unit*& u);


#endif // ACTIONS_H_INCLUDED
