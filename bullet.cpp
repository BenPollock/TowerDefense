////////////////////////////
// TOWER DEFENSE PHASE 2
// Ben Pollock 250575023
// Dec 5, 2011
// bullet.cpp
////////////////////////////

#include "bullet.h"
#include <QGraphicsItem>
#include <QPainter>
#include <QStyleOption>
#include <cmath>

//Constructor
Bullet::Bullet(int x, int y, int damage, int a, Creature *c) : enabled (true), hit_enemy(false){
    x_now = x;
    y_now = y;
    attack = damage;
    splash = a;
    x_target = c->getX();
    y_target = c->getY();
    d = c;

}
QRectF Bullet::boundingRect() const
{
    const int PAD(2);
    // Calculate upper left coordinate and width and height of bounding rectangle of an ellipse WxH centered on (0,0) with a bit of padding around it
    return QRectF(-W/2-PAD, -H/2-PAD, W+2*PAD, H+2*PAD);
}

//Advances the bullet toward target
void Bullet::step(int delay){
    float angle = atan2(y_target-y_now,x_target-x_now); // Determine angle of trajectory
    x_now = x_now + 35*cos(angle); // Advance bullet
    y_now = y_now + 35*sin(angle);
    setPos(x_now, y_now);

    // Determine if creature has reached its target
    if (x_now+20>x_target&&x_now-20<x_target&&y_now-20<y_target&&y_now+20>y_target)
    {
        // Reduce creature health
        d->reduceHealth(attack);
        hit_enemy=true;

        // Delete creature and collect money if bullet kills creature
        if (d->getHealth()<=0){
            d->erase();
            d->setEnabled(false);
            d->shot();
            d->setCollectedMoney(false);
        }

        //Remove from screen
        setPos(1000,1000);
        enabled = false;
    }
}
//Paint the bullet
void Bullet::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setBrush(Qt::lightGray);
    painter->setPen(QPen(Qt::lightGray,2));
    painter->drawEllipse(-W/2,-H/2,W,H);// Ellipse at (-W/2,-H/2) which fits in a WxH box.
}

// Sets enabled to a
void Bullet::setEnabled(bool a){
    enabled = a;
}

// Returns if bullet is enabled
bool Bullet::isEnabled(){
    return enabled;
}

// Returns splash radius
int Bullet::getSplash(){
    return splash;
}


// Returns current x coordinate
int Bullet::getX(){
    return x_now;
}

// Returns current y coordinate
int Bullet::getY(){
    return y_now;
}

// Resets hit enemy to false
void Bullet::setHitEnemy(){
    hit_enemy = false;
}

// Returns if the enemy has been hit
bool Bullet::getHitEnemy(){
    return hit_enemy;
}

// Returns the target creature
Creature* Bullet::getCreature(){
    return d;
}

// Returns the damage value of the bullet
int Bullet::getDamage(){
    return attack;
}
