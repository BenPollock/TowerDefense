////////////////////////////
// TOWER DEFENSE PHASE 2
// Ben Pollock 250575023
// Dec 5, 2011
// tower.cpp
////////////////////////////

#include <QGraphicsScene>
#include <QPainter>
#include <QStyleOption>
#include <cmath>
#include "tower.h"


// Constuctor
Tower::Tower(int x, int y, int z, int a, int b, int c, int d, int e, int f, int g){
    cost = a;
    range = b;
    air_damage = c;
    ground_damage = d;
    splash = e;
    cooldown = f;
    type = z;
    x_pos = x;
    y_pos = y;
    elligibility = g;
}
// Destructor
Tower::~Tower(){
}
// Returns boundingRect
QRectF Tower::boundingRect() const
{
    const int PAD(2);
    return QRectF(-W/2-PAD, -H/2-PAD, W+2*PAD, H+2*PAD);
}
//Paints the tower
void Tower::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *){
    // Draw range outline
    QPen dotted;
    dotted.setColor(Qt::yellow);
    dotted.setWidth(2);
    dotted.setStyle(Qt::DotLine);
    painter->setBrush(Qt::NoBrush);
    painter->setPen(dotted);
    painter->drawEllipse(x_pos-range,y_pos-range,range*2,range*2);  // Calculate range

    //Draw the physical tower
    painter->setBrush(Qt::darkCyan);
    painter->setPen(QPen(Qt::lightGray,4));
    painter->drawEllipse(x_pos-30, y_pos-30,W,H);
}

// Returns the cost of building the tower
int Tower::getCost(){
    return cost;
}

// Returns the current x coordinate
int Tower::getX(){
    return x_pos;
}

// Returns the current y coordinate
int Tower::getY(){
    return y_pos;
}

// Returns the range radius
int Tower::getRange(){
    return range;
}

// Returns the damage dealt to ground creatures
int Tower::getGroundDamage(){
    return ground_damage;
}

//Returns the damage dealt to air creatures
int Tower::getAirDamage(){
    return air_damage;
}

// Returns the splash radius
int Tower::getSplash(){
    return splash;
}

// Returns time between shoots (ie cooldown)
int Tower::getCooldown(){
    return cooldown;
}

// Returns the tower type
int Tower::getType(){
    return type;
}

// Sets the x position
void Tower::setX(int a){
    x_pos = a;
}

// Sets the y position
void Tower::setY(int a){
    y_pos = a;
}

// Removes tower from screen
void Tower::erase(){
    x_pos = 1000;
    y_pos = 1000;
}

// Returns when the tower can be drawn
int Tower::getEllig(){
    return elligibility;
}

// Detects if a creature is within range
Creature* Tower::detect(int wave, QList<Creature*> baddies){
      // Goes through every enabled creature
      for (int i = 0; i < baddies.size(); i++){
          if(baddies.at(i)->getFlying()==0 && ground_damage>0){  // Can we attack ground creatures?  If so, is it in range?
           if (x_pos+range>baddies.at(i)->pos().x()&&x_pos-range<baddies.at(i)->pos().x()&&y_pos+range>baddies.at(i)->pos().y()&&y_pos-range<baddies.at(i)->pos().y()){
              return baddies.at(i);
            }
          }
          if(baddies.at(i)->getFlying()!=0 && air_damage>0){    // Can we attack flying creatures?  If so, is it in range?
           if (x_pos+range>baddies.at(i)->pos().x()&&x_pos-range<baddies.at(i)->pos().x()&&y_pos+range>baddies.at(i)->pos().y()&&y_pos-range<baddies.at(i)->pos().y()){
              return baddies.at(i);
            }
          }
       }
    return NULL;   // No creatures are within range

}
