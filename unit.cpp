#include <string>
#include <cassert>
#include "unit.h"

using namespace std;

// Constructor
Unit::Unit(UnitType t, bool sd, int row, int col, int HP, int Dmg)
    : type(t), side(sd), urow(row), ucol(col), uHP(HP), uDmg(Dmg), ifMove(true), ifAttack(true)
    {}

// Get the symbol of the unit
string Unit::getSymbol() const
{
    switch (type)
    {
        case SOLDIER:
        {
            return "S" + to_string(uHP);
        }
        case TANK:
        {
            return "T" + to_string(uHP);
        }
        case BEE:
        {
            return "b" + to_string(uHP);
        }
        case FIGHTER:
        {
            return "F" + to_string(uHP);
        }
        case HYDRALISK:
        {
            return "h" + to_string(uHP);
        }
        default:
            return "";
    }
}

// Get the coordinate of the current unit
int Unit::getRow() const
{
    return urow;
}

int Unit::getCol() const
{
    return ucol;
}

void Unit::setRow(int row)
{
    urow = row;
}

void Unit::setCol(int col)
{
    ucol = col;
}

// Set the coordinates
void Unit::setCoord(int row, int col)
{
    urow = row;
    ucol = col;
}

// unit performs move action
void Unit::move(int row, int col) {
    // setCoord(row, col);
}

bool Unit::getSide() const
{
    switch (type)
    {
        case SOLDIER:
            return true;
        case TANK:
            return true;
        case FIGHTER:
            return true;
        case BEE:
            return false;
        case HYDRALISK:
            return false;
    }
}

// Get movement point
int Unit::getMovPoints() const
{
    switch (type)
    {
        case SOLDIER:
            return 3;
        case TANK:
            return 2;
        case BEE:
            return 3;
        case FIGHTER:
            return 5;
        case HYDRALISK:
            return 3;
        default:
            return 0;
    }
}

UnitType Unit::get_type() const
{
    return type;
}

int Unit::get_HP() const
{
    return uHP;
}

void Unit::set_HP(int HP)
{
    uHP = HP;
}

int Unit::get_Dmg() const
{
    return uDmg;
}

bool Unit::get_MOVE() const
{
    return ifMove;
}

bool Unit::get_ATTACK() const
{
    return ifAttack;
}
    
void Unit::set_MOVE(bool t)
{
    ifMove = t;
}
    
void Unit::set_ATTACK(bool t)
{
    ifAttack = t;
}