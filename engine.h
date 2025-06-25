#ifndef ENGINE_H_INCLUDED
#define ENGINE_H_INCLUDED

#include <iostream>
#include "field.h"

// display mode used in function displayField
enum dp_mode {
  DP_DEFAULT, DP_MOVE, DP_ATTACK, DP_MOVEABLE, 
};

// load terrains and units into field
void loadMap(std::istream& is, Field& field);

// Main loop for playing the game
void play(Field& field, std::istream& is, std::ostream& os);

// Display the battle field
void displayField(std::ostream& os, const Field& field,
                  const Grid<bool>& grd = Grid<bool>(), dp_mode dp = DP_DEFAULT);

bool attackTarget(Field& field, Unit* attacker, Unit* target, int RT, int CT);

#endif // ENGINE_H_INCLUDED