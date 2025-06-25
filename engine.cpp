#include <iomanip>
#include <string>
#include <cmath>
#include "engine.h"
#include "terrain.h"
#include "unit.h"
#include "actions.h"
#include "algorithms.h"

using namespace std;

// Forward declaration of auxiliary functions
void printHLine(ostream& os, int n);
string getDpSymbol(dp_mode dp);
Grid<int> getFieldCosts(const Field& field, Unit* u);
bool performAction(Field& field, istream& is, ostream& os, Unit* u, Action act);
bool performMove(ostream& os, istream& is, Field& field, Unit* u);
bool attackTarget(Field& field, Unit* attacker, Unit* target, int RT, int CT);
void UpdateCounts(Field& field, int& cnt_player, int& cnt_enemy, int& cnt_moveable_player, int& cnt_moveable_enemy);
pair<int, int> getMinDist(Field& field, int x, int y);
void performBeatBack(Field& field, Unit* attacker, Unit* target);
void performForestHeal(Field& field);

bool check_in_bound(Field& field, int x, int y)
{
    return (x >= 0 && y >= 0 && x < field.getHeight() && y < field.getWidth());
}

int check_win(int cnt_player, int cnt_enemy)
{  
    if(cnt_player && cnt_enemy) return 0; //no side winning
    if(!cnt_enemy) return 1; //player winning
    if(cnt_enemy && !cnt_player) return 2; //enemy winning
}

// load terrains and units into field
void loadMap(istream& is, Field& field) {
  //Fill in your code here
  	int NT, NU; 
  	is >> NT >> NU;
	while(NT -- )
	{
		int R, C;
		string T;
		is >> R >> C >> T;
		TerrainType newType;
		if(T == "M") newType = TerrainType::MOUNTAIN;
		else if(T == "O") newType = TerrainType::OCEAN;
        else newType = TerrainType::FOREST;
        Terrain newTerrain = Terrain();
        newTerrain.set_ttype(newType);
		field.set_Terrain(newTerrain, R, C);
	}
	while(NU -- )
	{
		int R, C;
		string U;
		is >> R >> C >> U;
		UnitType newUnitType;
        switch (U[0])
        {
            case 'S':
            {
                newUnitType = UnitType::SOLDIER;
                break;
            }
            case 'T':
            {
                newUnitType = UnitType::TANK;
                break;
            }
            case 'B':
            {
                newUnitType = UnitType::BEE;
                break;
            }
            case 'F':
            {
                newUnitType = UnitType::FIGHTER;
                break;
            }
            case 'H':
            {
                newUnitType = UnitType::HYDRALISK;
                break;
            }
        }
        int newHP, newDmg;
        bool newSide;
        switch (newUnitType)
        {
            case SOLDIER:
            {
                newHP = 2;
                newDmg = 1;
                newSide = true;
                break;
            }
            case TANK:
            {
                newHP = 3;
                newDmg = 1;
                newSide = true;
                break;
            }
            case BEE:
            {
                newHP = 2;
                newDmg = 3;
                newSide = false;
                break;
            }
            case FIGHTER:
            {
                newHP = 2;
                newDmg = 2;
                newSide = true;
                break;
            }
            case HYDRALISK:
            {
                newHP = 1;
                newDmg = 2;
                newSide = false;
                break;
            }
        } 

        // cout << newSide << endl;
        Unit* newUnit = new Unit(newUnitType, newSide, R, C, newHP, newDmg);
        // cout << newUnit->getSide() << endl;
		field.set_Unit(newUnit, R, C);
	}
}

// Main loop for playing the game
void play(Field& field, istream& is, ostream& os)
{
    int h = field.getHeight();
    int w = field.getWidth();
    int cnt_player = 0, cnt_enemy = 0, cnt_moveable_player = 0, cnt_moveable_enemy = 0;
    for(int i = 0 ; i < h ; i ++ )
        for(int j = 0 ; j < w ; j ++ )
            if(field.getUnit(i, j) != nullptr)
                if(field.getUnit(i, j)->getSide()) cnt_player ++ ;
                else cnt_enemy ++ ;
    // cout << cnt_enemy << " " << cnt_player << endl;
    while (is)
    {
        displayField(os, field);

        //initialize the units
        for(int i = 0 ; i < h ; i ++ )
            for(int j = 0 ; j < w ; j ++ )
                if(field.getUnit(i, j) != nullptr)
                {
                    Unit* u = field.getUnit(i, j);
                    u->set_ATTACK(true), u->set_MOVE(true);
                    field.set_Unit(u, i, j);
                }
        UpdateCounts(field, cnt_player, cnt_enemy, cnt_moveable_player, cnt_moveable_enemy);

        //check if winning or defeating
        if(check_win(cnt_player, cnt_enemy) == 1)
        {
            os << "Won" << endl;
            return;
        }
        else if(check_win(cnt_player, cnt_enemy) == 2)
        {
            os << "Failed" << endl;
            return;
        }

        //player's round
        int R, C;
        while(cnt_moveable_player)
        {
            //see which units can move
            Grid<bool> grd_moveable(h, w);
            for(int i = 0 ; i < h ; i ++ )
                for(int j = 0 ; j < w ; j ++ )
                    if(field.getUnit(i, j) != nullptr && (field.getUnit(i, j)->get_MOVE() || field.getUnit(i, j)->get_ATTACK()) && field.getUnit(i, j)->getSide())
                        grd_moveable[i][j] = true;
            
            displayField(os, field, grd_moveable, DP_MOVEABLE);

            //ask the player if end this turn
            string if_end;
            os << "End this turn (y,n)?" << endl;
            is >> if_end;
            if(if_end == "y") break;

            while(true)
            {  
                os << "Please select a unit:" << endl;
                is >> R >> C;
                if(check_in_bound(field, R, C) && field.getUnit(R, C) != nullptr)
                {
                    if(!field.getUnit(R, C)->getSide())
                    {
                        // os << "side is " << field.getUnit(R, C)->getSide() << endl;
                        os << "Unit at (" << R << ", " << C << ") is an enemy!" << endl;
                        continue;
                    }
                    else if(!field.getUnit(R, C)->get_MOVE() && !field.getUnit(R, C)->get_ATTACK())
                    {
                        os << "Unit at (" << R << ", " << C << ") is not actable!" << endl;
                        continue;
                    }
                    else break;
                }
                os << "No unit at (" << R << ", " << C << ")!" << endl;
            }

        // select action
            Unit* nowUnit = field.getUnit(R, C);
            int moveNumber, actionTypes;
            if(nowUnit->get_MOVE() && nowUnit->get_ATTACK())
            {
                os << "1.Move 2.Attack 3.Skip" << endl;
                actionTypes = 3;
            }
            else if(nowUnit->get_ATTACK())
            {
                os << "1.Attack 2.Skip" << endl;
                actionTypes = 2;
            }
            else if(nowUnit->get_MOVE())
            {
                os << "1.Move 2.Skip" << endl;
                actionTypes = 2;
            }

            while(true)
            {
                os << "Select your action:" << endl;
                is >> moveNumber;
                if(moveNumber >= 1 && moveNumber <= actionTypes) break;
                os << "Invalid action!" << endl;
            }
            
            Action moveType = SKIP;
            if(nowUnit->get_MOVE() && nowUnit->get_ATTACK())
            {
                if(moveNumber == 1) moveType = MOVE;
                else if(moveNumber == 2) moveType = ATTACK;
            }
            else if(nowUnit->get_ATTACK())
            {
                if(moveNumber == 1) moveType = ATTACK;
            }
            else if(nowUnit->get_MOVE())
            {
                if(moveNumber == 1) moveType = MOVE;
            }

        // perform action
        // A function called performAction is defined below.
        // You can use it or define your own version.
            if(moveType == MOVE) //MOVE
            {
                nowUnit->set_MOVE(false);
                performAction(field, is, os, nowUnit, MOVE);
                Grid<bool> grd = searchReachable(getFieldCosts(field, nowUnit), nowUnit->getRow(), nowUnit->getCol(), nowUnit->getMovPoints());
                int R1, C1;
                while(true)
                {
                    os << "Please enter your destination:" << endl;
                    is >> R1 >> C1;
                    if(grd.inBounds(R1, C1) && grd[R1][C1]) break;
                    os << "Not a valid destination" << endl;
                }
                //perform the move
                nowUnit->setRow(R1), nowUnit->setCol(C1);
                field.set_Unit(nowUnit, R1, C1);
                if(R != R1 || C != C1) field.set_Unit(nullptr, R, C);
            }
            else if(moveType == ATTACK) //ATTACK
            {
                nowUnit->set_ATTACK(false);
                field.set_Unit(nowUnit, R, C);
                UnitType nowType = nowUnit->get_type();
                Grid<bool> grd(field.getHeight(), field.getWidth());
                switch (nowType)
                {
                    case SOLDIER:
                    {
                        if(grd.inBounds(R, C - 1)) grd[R][C - 1] = true; //west
                        if(grd.inBounds(R, C + 1)) grd[R][C + 1] = true; //east
                        if(grd.inBounds(R + 1, C)) grd[R + 1][C] = true; //south
                        if(grd.inBounds(R - 1, C)) grd[R - 1][C] = true; //north
                        break;
                    }
                    case TANK:
                    {
                        for(int i = C - 1 ; i >= 0 ; i -- ) //west
                        {
                            grd[R][i] = true;
                            Terrain i_Terrain = field.getTerrain(R, i);
                            Unit* i_Unit = field.getUnit(R, i);
                            if(i_Unit != nullptr || i_Terrain.getSymbol() != "  ") break;
                        }
                        for(int i = C + 1 ; i < field.getWidth() ; i ++ ) //east
                        {
                            grd[R][i] = true;
                            Terrain i_Terrain = field.getTerrain(R, i);
                            Unit* i_Unit = field.getUnit(R, i);
                            if(i_Unit != nullptr || i_Terrain.getSymbol() != "  ") break;
                        }
                        for(int i = R + 1 ; i < field.getHeight() ; i ++ ) //south
                        {
                            grd[i][C] = true;
                            Terrain i_Terrain = field.getTerrain(i, C);
                            Unit* i_Unit = field.getUnit(i, C);
                            if(i_Unit != nullptr || i_Terrain.getSymbol() != "  ") break;
                        }
                        for(int i = R - 1 ; i >=0 ; i -- ) //north
                        {
                            grd[i][C] = true;
                            Terrain i_Terrain = field.getTerrain(i, C);
                            Unit* i_Unit = field.getUnit(i, C);
                            if(i_Unit != nullptr || i_Terrain.getSymbol() != "  ") break;
                        }
                        break;
                    }
                    case FIGHTER:
                    {
                        if(grd.inBounds(R, C - 2)) grd[R][C - 2] = true; //west
                        if(grd.inBounds(R, C + 2)) grd[R][C + 2] = true; //east
                        if(grd.inBounds(R + 2, C)) grd[R + 2][C] = true; //south
                        if(grd.inBounds(R - 2, C)) grd[R - 2][C] = true; //north
                        break;
                    }
                }

                displayField(os, field, grd, DP_ATTACK);

                int RT = -1, CT = -1;
                while(!grd.inBounds(RT, CT) || !grd[RT][CT])
                {
                    os << "Please enter your target:" << endl;
                    is >> RT >> CT;
                    if(!grd.inBounds(RT, CT) || !grd[RT][CT]) os << "Not a valid target" << endl;
                }
                
                attackTarget(field, nowUnit, field.getUnit(RT, CT), RT, CT);  
            }

            //update the number of the units on two sides
            UpdateCounts(field, cnt_player, cnt_enemy, cnt_moveable_player, cnt_moveable_enemy);
            // if(check_win(cnt_player, cnt_enemy) == 1)
            // {
            //     displayField(os, field);
            //     os << "Won" << endl;
            //     return;
            // }
            if(!cnt_moveable_player) os << "No more actable units." << endl;
        }
        //end of player's round
        //enemy's round
        while(cnt_moveable_enemy)
        {
            for(int i = 0 ; i < h ; i ++ )
                for(int j = 0 ; j < w ; j ++ )
                {
                    Unit* nowUnit = field.getUnit(i, j);
                    if(nowUnit != nullptr && !nowUnit->getSide() && (nowUnit->get_MOVE() || nowUnit->get_ATTACK()))
                    {
                        if(nowUnit->get_MOVE()) //if this unit can move
                        {
                            nowUnit->set_MOVE(false);
                            Grid<bool> grd = searchReachable(getFieldCosts(field, nowUnit), i, j, nowUnit->getMovPoints());

                            // for(int i = 0 ; i < h ; i ++ )
                            // {
                            //     for(int j = 0 ; j < w ; j ++ )
                            //         cout << grd[i][j] << " ";
                            //     cout << endl;
                            // }

                            int min_dist = 1e9 + 10;
                            pair<int, int> close_unit;
                            //get the termination
                            for(int p = 0 ; p < h ; p ++ )
                                for(int q = 0 ; q < w ; q ++ )
                                {
                                    if(grd[p][q])
                                    {
                                        pair<int, int> u = getMinDist(field, p, q);
                                        int dist = abs(u.first - p) + abs(u.second - q);
                                        if(dist < min_dist)
                                        {
                                            min_dist = dist;
                                            close_unit = make_pair(p, q);
                                        }
                                    }
                                }
                            //the termination is close_unit
                            field.set_Unit(nullptr, nowUnit->getRow(), nowUnit->getCol());
                            nowUnit->setRow(close_unit.first), nowUnit->setCol(close_unit.second);
                            field.set_Unit(nowUnit, close_unit.first, close_unit.second);
                            // cout << "the termination is: " << close_unit.first << ", " << close_unit.second << endl;
                        }

                        if(nowUnit->get_ATTACK()) //if this unit can attack
                        {
                            nowUnit->set_ATTACK(false);
                            int RT = nowUnit->getRow() - 1, CT = nowUnit->getCol();
                            if(check_in_bound(field, RT, CT) && field.getUnit(RT, CT) != nullptr && field.getUnit(RT, CT)->getSide()) 
                                attackTarget(field, nowUnit, field.getUnit(RT, CT), RT, CT);
                            else
                            {
                                RT ++ , CT -- ;
                                if(check_in_bound(field, RT, CT) && field.getUnit(RT, CT) != nullptr && field.getUnit(RT, CT)->getSide())
                                    attackTarget(field, nowUnit, field.getUnit(RT, CT), RT, CT);
                                else
                                {
                                    CT += 2;
                                    if(check_in_bound(field, RT, CT) && field.getUnit(RT, CT) != nullptr && field.getUnit(RT, CT)->getSide())
                                        attackTarget(field, nowUnit, field.getUnit(RT, CT), RT, CT);
                                    else
                                    {
                                        RT ++ , CT -- ;
                                        if(check_in_bound(field, RT, CT) && field.getUnit(RT, CT) != nullptr && field.getUnit(RT, CT)->getSide())
                                            attackTarget(field, nowUnit, field.getUnit(RT, CT), RT, CT);
                                    }
                                }
                            }
                        }

                        UpdateCounts(field, cnt_player, cnt_enemy, cnt_moveable_player, cnt_moveable_enemy);
                        if(check_win(cnt_player, cnt_enemy) == 2)
                        {
                            performForestHeal(field);
                            displayField(os, field);
                            os << "Failed" << endl;
                            return;
                        }
                    }
                }
        }
      //forest heal
        performForestHeal(field);
      // The following line is needed in task 1 and task2.
      // It avoids entering the next loop
      // when reaches the end of the input data.
      // Remove it in task 3 and task 4.
      // if (is.eof()) break;
    }
}

// Display the field on the out stream os
void displayField(ostream& os, const Field& field, const Grid<bool>& grd, dp_mode dp)
{
    int height = field.getHeight();
    int width = field.getWidth();
    string dp_symbol = getDpSymbol(dp);

    os << endl;
    // Print the x coordinates
    os << "  ";
    for (int i = 0; i < width; i++)
        os << setw(3) << i << " ";
    os << endl;

    printHLine(os, width);
    for (int i = 0; i < height; i++) {
        os << setw(2) << i;
        for (int j = 0; j < width; j++) {
            os << '|';
            const Unit* u = field.getUnit(i,j);
            Terrain t = field.getTerrain(i, j);
            string sym;
            int width = 3;
            if (grd.inBounds(i,j) && grd[i][j]) {
                os << setw(1) << dp_symbol;
                width -= 1;
            }

            if (u != nullptr) sym += u->getSymbol();
            else sym += t.getSymbol();
            os << setw(width) << sym;
        }
        os << '|' << endl;
        printHLine(os, width);
    }
    os << endl;
}

// Print the horizontal line
void printHLine(ostream& os, int n)
{
    os << "  ";
    for (int i = 0; i < n; i++)
        os << "+---";
    os << "+" << endl;
}

// symbol used in displayField
string getDpSymbol(dp_mode dp) {
    switch (dp)
    {
        case DP_MOVE:
            return ".";
        case DP_ATTACK:
            return "*";
        case DP_MOVEABLE:
            return "+";
        default:
            return " ";
    }
}

bool performAction(Field& field, istream& is, ostream& os, Unit* u, Action act) {
    switch(act) {
        case MOVE:
            return performMove(os, is, field, u);

        case SKIP:
            return true;
        
        default:
        {
            os << "(Action not implemented yet)" << endl;
            return false;
        }
    }
}

// Perform the move action
// The implementation is incomplete
bool performMove(ostream& os, istream& is, Field& field, Unit* u)
{
    // Display the reachable points
    Grid<bool> grd =
        searchReachable(getFieldCosts(field, u), u->getRow(), u->getCol(), u->getMovPoints());
    // print test
    // for(int i = 0 ; i < grd.numRows() ; i ++ )
    // {
    //     for(int j = 0 ; j < grd.numCols() ; j ++ )
    //         os << grd[i][j] << " ";
    //     os << endl;
    // }
    displayField(os, field, grd, DP_MOVE);

    return true;
}

// Convert field to costs
// The cost should depend on the terrain type and unit type
Grid<int> getFieldCosts(const Field& field, Unit* u)
{
    int h = field.getHeight();
    int w = field.getWidth();
    Grid<int> costs(h, w);

    for (int i = 0; i < h; i++)
        for (int j = 0; j < w; j++) 
        {
            Unit* nowUnit = field.getUnit(i, j);
            Terrain nowTerrain = field.getTerrain(i, j);
            string unitType = nowUnit == nullptr ? "" : nowUnit->getSymbol();
            TerrainType terrainType = nowTerrain.get_ttype();
            UnitType uType = u->get_type();
            
            costs[i][j] = 1;
            if(uType == SOLDIER || uType == TANK || uType == HYDRALISK) //land units
            {
                if(unitType != "") costs[i][j] = 100;
                else if(terrainType == MOUNTAIN || terrainType == OCEAN) costs[i][j] = 100;
            }
            else //air units
            {
                if(unitType != "") costs[i][j] = 100;
                else if(terrainType == MOUNTAIN || terrainType == FOREST) costs[i][j] = 100;
            }
        }
    
    // print test
    // for(int i = 0 ; i < h ; i ++ )
    // {
    //     for(int j = 0 ; j < w ; j ++ )
    //         cout << costs[i][j] << " ";
    //     cout << endl;
    // }

    return costs;
}

bool attackTarget(Field& field, Unit* attacker, Unit* target, int RT, int CT)
{
    UnitType attType = attacker->get_type();
    switch (attType)
    {
        case SOLDIER:
        {
            if(target == nullptr) return false;
            int targetHP = target->get_HP(), attackerDmg = attacker->get_Dmg();
            if(targetHP > attackerDmg) 
            {
                field.getUnit(RT, CT)->set_HP(targetHP - attackerDmg);
                return false;
            }
            else
            {
                field.set_Unit(nullptr, RT, CT);
                return true;
            }
            break;
        }
        case BEE:
        {
            if(target == nullptr) return false;
            int targetHP = target->get_HP(), attackerDmg = attacker->get_Dmg();
            if(targetHP > attackerDmg) 
            {
                field.getUnit(RT, CT)->set_HP(targetHP - attackerDmg);
                return false;
            }
            else
            {
                field.set_Unit(nullptr, RT, CT);
                return true;
            }
            break;
        }
        case TANK:
        {
            if(target == nullptr)
            {
                Terrain tgtTerrain = field.getTerrain(RT, CT);
                TerrainType tgtType = tgtTerrain.get_ttype();
                if(tgtType == MOUNTAIN) 
                {
                    Terrain newTerrain = Terrain();
                    field.set_Terrain(newTerrain, RT, CT);
                }
                return false;
            }

            //perform beat back
            performBeatBack(field, attacker, target);
            break;
        }
        case FIGHTER:
        {
            if(target != nullptr)
            {
                if(target->get_HP() > 2) target->set_HP(target->get_HP() - 2);
                else target = nullptr;
                field.set_Unit(target, RT, CT);
            }
            int dx[4] = {0, 0, 1, -1}, dy[4] = {-1, 1, 0, 0};
            for(int i = 0 ; i < 3 ; i ++ )
            {
                int r = RT + dx[i], c = CT + dy[i];
                if(check_in_bound(field, r, c) && field.getUnit(r, c) != nullptr)
                {
                    Unit* nullAttacker = new Unit(SOLDIER, true, RT, CT, 1, 0);
                    performBeatBack(field, nullAttacker, field.getUnit(r, c));
                }
            }
            break;
        }
        case HYDRALISK:
        {
            performBeatBack(field, attacker, target);
            break;
        }
    }
}

void UpdateCounts(Field& field, int& cnt_player, int& cnt_enemy, int& cnt_moveable_player, int& cnt_moveable_enemy)
{
    int cntP = 0, cntE = 0, cntMP = 0, cntME = 0;
    int h = field.getHeight(), w = field.getWidth();
    for(int i = 0 ; i < h ; i ++ )
        for(int j = 0 ; j < w ; j ++ )
            if(field.getUnit(i, j) != nullptr)
            {
                Unit* nowUnit = field.getUnit(i, j);
                if(nowUnit->get_ATTACK() || nowUnit->get_MOVE()) nowUnit->getSide() ? cntMP ++ : cntME ++ ;
                nowUnit->getSide() ? cntP ++ : cntE ++ ;
            }
    cnt_player = cntP, cnt_enemy = cntE, cnt_moveable_player = cntMP, cnt_moveable_enemy = cntME;
    return;
}

pair<int, int> getMinDist(Field& field, int x, int y)
{
    int h = field.getHeight(), w = field.getWidth();
    int min_dist = 1e9 + 10;
    pair<int, int> res;
    for(int i = 0 ; i < h ; i ++ )
        for(int j = 0 ; j < w ; j ++ )
        {
            if(i == x && j == y) continue;
            if(field.getUnit(i, j) != nullptr && field.getUnit(i, j)->getSide())
            {
                int dist = abs(i - x) + abs(j - y);
                if(min_dist > dist)
                {
                    min_dist = dist;
                    res = make_pair(i, j);
                }
            }
        }
    return res;
}

void performBeatBack(Field& field, Unit* attacker, Unit* target)
{
    int R = attacker->getRow(), C = attacker->getCol(); //attacker's position
    int RT = target->getRow(), CT = target->getCol();
    int attDmg = attacker->get_Dmg(), targetHP = target->get_HP();
    int RN, CN; //new position after beat back
    if(RT == R)
    {
        RN = RT;
        if(CT < C) CN = CT - 1;
        else CN = CT + 1;
    }
    else
    {
        CN = CT;
        if(RT < R) RN = RT - 1;
        else RN = RT + 1;
    }
    if(RN < 0 || CN < 0 || RN > field.getHeight() || CN > field.getWidth()) //the target is already on the border
    {
        if(targetHP > attDmg)
        {
            target->set_HP(targetHP - attDmg);
            field.set_Unit(target, RT, CT);
            return;
        }
        else 
        {
            field.set_Unit(nullptr, RT, CT);
            return;
        }
    }

    if(field.getUnit(RN, CN) != nullptr) //there is a unit back to back
    {
        Unit* BeatBackUnit = field.getUnit(RN, CN);
        int BeatBackUnitHP = BeatBackUnit->get_HP();
        if(targetHP > 1 + attDmg)
        {
            target->set_HP(targetHP - 1 - attDmg);
            targetHP -= 1 + attDmg;
            if(BeatBackUnitHP > 1) BeatBackUnit->set_HP(BeatBackUnitHP - 1), BeatBackUnitHP -- ;
            else BeatBackUnit = nullptr;
            field.set_Unit(target, RT, CT);
            field.set_Unit(BeatBackUnit, RN, CN);
            return;
        }
        else if(targetHP <= 1 + attDmg)
        {
            target = nullptr;
            if(BeatBackUnitHP > 1) BeatBackUnit->set_HP(BeatBackUnitHP - 1), BeatBackUnitHP -- ;
            else BeatBackUnit = nullptr;
            field.set_Unit(target, RT, CT);
            field.set_Unit(BeatBackUnit, RN, CN);
            return;
        }
    }

    if(field.getTerrain(RN, CN).get_ttype() != PLAIN) //there is a special terrain at the new position
    {
        TerrainType BeatBackttype = field.getTerrain(RN, CN).get_ttype();
        switch (BeatBackttype)
        {
            case MOUNTAIN:
            {
                // cout << "beat back is a MOUNTAIN" << endl;
                if(targetHP > 1 + attDmg) 
                {
                    target->set_HP(targetHP - 1 - attDmg);
                    targetHP -= 1 + attDmg;
                    field.set_Unit(target, RT, CT);
                    field.set_Terrain(Terrain(), RN, CN);
                    return;
                }
                else if(targetHP <= 1 + attDmg)
                {
                    target = nullptr;
                    field.set_Unit(target, RT, CT);
                    field.set_Terrain(Terrain(), RN, CN);
                    return;
                }
                break;
            }
            case OCEAN:
            {
                // cout << "beat back is an OCEAN" << endl;
                field.set_Unit(nullptr, RT, CT);
                if(target->get_type() == BEE || target->get_type() == FIGHTER) //the target is air unit
                {
                    if(targetHP > attDmg)
                    {
                        Unit* tgtUnit = new Unit(target->get_type(), target->getSide(), RN, CN, targetHP - attDmg, target->get_Dmg());
                        field.set_Unit(tgtUnit, RN, CN);
                        return;
                    }
                }
                return;
                break;
            }
            case FOREST:
            {
                if(target->get_type() == BEE || target->get_type() == FIGHTER)
                {
                    if(targetHP > attDmg)
                    {
                        target->set_HP(targetHP - attDmg);
                        field.set_Unit(target, RT, CT);
                    }
                    else field.set_Unit(nullptr, RT, CT);
                }
                else
                {
                    if(targetHP > attDmg)
                    {
                        target->set_HP(targetHP - attDmg);
                        target->setRow(RN), target->setCol(CN);
                        field.set_Unit(nullptr, RT, CT);
                        field.set_Unit(target, RN, CN);
                    }
                    else field.set_Unit(nullptr, RN, CN), field.set_Unit(nullptr, RT, CT);
                }
                break;
            }
        }
    }
            
    //only beat back
    target->setRow(RN), target->setCol(CN);
    if(targetHP > attDmg) target->set_HP(targetHP - attDmg);
    else target = nullptr;
    field.set_Unit(target, RN, CN);
    field.set_Unit(nullptr, RT, CT);
    return;
}

void performForestHeal(Field& field)
{
    int h = field.getHeight(), w = field.getWidth();
    for(int i = 0 ; i < h ; i ++ )
        for(int j = 0 ; j < w ; j ++ )
        {
            if(field.getTerrain(i, j).get_ttype() == FOREST)
            {
                for(int p = max(0, i - 2) ; p <= min(h - 1, i + 2) ; p ++ )
                    for(int q = max(0, j - 2) ; q <= min(w - 1, j + 2) ; q ++ )
                        if(field.getUnit(p, q) != nullptr)
                        {
                            Unit* u = field.getUnit(p, q);
                            u->set_HP(u->get_HP() + 1);
                            field.set_Unit(u, p, q);
                        }
            }
        }
    return;
}