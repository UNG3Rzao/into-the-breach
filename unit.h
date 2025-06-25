#ifndef UNITS_H_INCLUDED
#define UNITS_H_INCLUDED

#include <string>

/* Type of Units */
enum UnitType {SOLDIER, TANK, BEE, FIGHTER, HYDRALISK};

/* Class for units */
class Unit {
public:
    Unit(UnitType u, bool sd, int row, int col, int HP, int Dmg);

    // UnitType information
    std::string getSymbol() const;

    // Get the coordinate of the current unit
    int getRow() const;
    int getCol() const;
    void setRow(int row);
    void setCol(int col);

    // Set the coordinates
    void setCoord(int row, int col);

    // unit performs move action
    void move(int row, int col);

    // Check which side the unit belongs to
    bool getSide() const;

    // Get movement point, which depends on unit type
    int getMovPoints() const;
	
    UnitType get_type() const;

    //HP getter and setter
    int get_HP() const;
    void set_HP(int HP);

    //Damage getter
    int get_Dmg() const;

    //moveable getter and setter
    bool get_MOVE() const;
    bool get_ATTACK() const;
    void set_MOVE(bool t);
    void set_ATTACK(bool t);
    
private:
    UnitType type;
    bool side;
    int urow, ucol;
    int uHP, uDmg;
    bool ifMove, ifAttack;
};

#endif // UNITS_H_INCLUDED
