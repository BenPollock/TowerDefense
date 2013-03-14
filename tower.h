////////////////////////////
// TOWER DEFENSE PHASE 2
// Ben Pollock 250575023
// Dec 5, 2011
// tower.h
// Description: A tower object.  Cannot shoot bullets on its own (done through track.cpp)
////////////////////////////

#ifndef TOWER_H
#define TOWER_H

#include <QGraphicsItem>
#include <QList>
#include "creature.h"
#include "bullet.h"

class Tower : public QGraphicsItem
{
public:
    Tower(int x, int y, int z, int a, int b, int c, int d,int e, int f, int g);
    ~Tower();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    //void shoot(int x, int y);
    int getCost();                  // returns cost
    int getRange();                 // returns radius of range
    int getGroundDamage();          // returns ground_damage
    int getAirDamage();             // returns air_damage
    int getSplash();                // returns splash radius
    int getCooldown();              // returns cooldown value
    int getType();                  // returns tower type
    int getX();                     // returns x coordinate
    int getY();                     // returns y coordinate
    void setX(int a);               // sets the x coordinate
    void setY(int a);               // sets the y coordinate
    void erase();                   // removes tower from screen
    int getEllig();                 // returns elligibility
    Creature* detect(int wave, QList<Creature*>baddies);    // detects if any creatures are within its range

protected:
    enum {W = 60, H = 60};
    int cost;                       // cost to build the tower
    int range;                      // range radius of the tower
    int air_damage;                 // the damage to air creatures
    int ground_damage;              // the damage to ground creatures
    int splash;                     // radius of splash damage
    int cooldown;                   // time between successive shots
    int type;                       // type of tower
    int x_pos;                      // x coordinate of tower
    int y_pos;                      // y coordinate of tower
    int elligibility;               // when the tower can be drawn

};

#endif // TOWER_H
