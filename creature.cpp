////////////////////////////
// TOWER DEFENSE PHASE 2
// Ben Pollock 250575023
// Dec 5, 2011
// creature.cpp
////////////////////////////

#include "creature.h"
#include <QGraphicsScene>
#include <QPainter>
#include <QStyleOption>
#include <cmath>

// Initialize the path, next vertex and flag that indicates movement
Creature::Creature(const QPolygon& p, int a, float b, int c, int d, int e) : path(p), next(0),hp(a),speed(b),treasure(c),flying(d),type(e),health("0"),death_by_bullet(false),collected_money(true)
{
    hp = a;
    speed = b;
    treasure = c;
    flying = d;
    type = e;
    health = health.setNum(a);
    if (type==1)
        type_colour=Qt::blue;
    else
        type_colour=Qt::lightGray;
    //timerID = startTimer(30); // trigger every 30 ms
}
Creature::~Creature(){
}

// Tell the area of the object to the framework
QRectF Creature::boundingRect() const
{
    const int PAD(2);
    // Calculate upper left coordinate and width and height of bounding rectangle of an ellipse WxH centered on (0,0) with a bit of padding around it
    return QRectF(-W/2-PAD, -H/2-PAD, W+2*PAD, H+2*PAD);
}

// Draw the object. Framework calls this function whenever the object needs to be drawn.
// We have no control over when this actually gets called
// Add code here to make the creature look the way you want
void Creature::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    QBrush bg;
    bg.setColor(type_colour);
    health = health.setNum(hp);
    if (flying==0)
       bg.setStyle(Qt::SolidPattern);  // Ground creatures have a solid background
    else
       bg.setStyle(Qt::Dense3Pattern);  // Air creatures have a patterned background
    painter->setBrush(bg);
    painter->setPen(QPen(Qt::black,2));
    painter->drawEllipse(-W/2,-H/2,W,H);// Ellipse at (-W/2,-H/2) which fits in a WxH box.
    painter->drawText(-W/2,-H/2,health); // Draw the HP value of the creature
    //Different draw method if creature is flying
}

// This gets called periodically and avoids a separate timer for each creature. This allows easily pausing the game from outside
// Add code here that handles the state of the creature (position and attributes such as colors, hit points etc)
void Creature::step(int delay)
{
    const float STEP(5.0f*speed);     // distance travelled every 30 ms
    float x0 = pos().x(); float y0 = pos().y();     // Initial x,y coordinates

    if (next >= path.count())   // Have we reached the end of the track?
    {
        // Yes. We are at the end of the track
        setPos(x0+20, y0);
        setEnabled(false);      // Tell the framework that this object doesn't need to be monitored
        //killTimer(timerID);     // Disable the timer as the creature has escaped
    }
    else
    {
        // We are still moving. Calculate the next position and move
        float dx = path.point(next).x() - pos().x();    // (x,y) vector to next vertex
        float dy = path.point(next).y() - pos().y();
        float angle = atan2(dy,dx);                     // Angle of the path to next vertex

        if (dx*dx+dy*dy < 2*STEP+0.5*speed) // Did we reach the next vertex?
        {
            // Yes. Advance to next vertex
            next++;
        }
        else
        {
            // No. Move STEP pixels toward the next vertex
            setPos(x0+STEP*cos(angle), y0+STEP*sin(angle));
        }
    } // if (next >= path.count())
}

//Removes the creature from the screen
void Creature::erase(){
    float x0 = pos().x(); float y0 = pos().y();
    setPos(x0+700,y0+700);
}

//Returns X coordinate
int Creature::getX(){
    return pos().x();
}

//Returns Y coordinate
int Creature::getY(){
    return pos().y();
}

//Reduces health by a
void Creature::reduceHealth(int a){
    hp = hp - a;
    type_colour = type_colour.darker(150);  //Darkens color to demonstrate getting hit
}

//Returns health
int Creature::getHealth(){
    return hp;
}

//Returns treasure value
int Creature::getTreasure(){
    return treasure;
}

//Creature set to death by shot
void Creature::shot(){
    death_by_bullet = true;
}

//Returns if creature died from bullet
bool Creature::wasShot(){
    return death_by_bullet;
}

//Returns creature type
int Creature::getType(){
    return type;
}

//Returns if the creature has money that needs to be collected
bool Creature::getCollectedMoney(){
    return collected_money;
}

//Sets collected money to a
void Creature::setCollectedMoney(bool a){
    collected_money = a;
}

//Returns creature flying value
int Creature::getFlying(){
    return flying;
}
