////////////////////////////
// TOWER DEFENSE PHASE 2
// Ben Pollock 250575023
// Dec 5, 2011
// bullet.h
// Description: A bullet, launches from tower and hurts creature when it hits
////////////////////////////

#ifndef BULLET_H
#define BULLET_H

#include <QGraphicsItem>
#include "creature.h"

class Bullet: public QGraphicsItem
{
public:
    Bullet(int x, int y, int damage, int a, Creature *c);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void step (int delay);                  // advances the bullet
    void setEnabled(bool a);                // sets enabled to a
    bool isEnabled();                       // returns enabled
    int getSplash();                        // returns splash value
    int getX();                             // returns X coordinate
    int getY();                             // returns y coordinate
    void setHitEnemy();                     // sets hitEnemy to false
    bool getHitEnemy();                     // returns hitEnemy
    int getDamage();                        // returns attack
    Creature* getCreature();

protected:
    enum {W = 2, H = 2};                    // width and height
    int x_target;                           // x coordinate of target creature
    int y_target;                           // y coordinate of target creature
    int x_now;                              // current x coordinate
    int y_now;                              // current y coordinate
    int attack;                             // damage dealt on collision with creature
    int splash;                             // splash radius on collision
    bool enabled;                           // True when bullet is moving
    bool hit_enemy;                          // True when bullet hits target
    Creature *d;                            // The target creature
};

#endif // BULLET_H
