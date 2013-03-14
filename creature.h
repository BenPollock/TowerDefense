////////////////////////////
// TOWER DEFENSE PHASE 2
// Ben Pollock 250575023
// Dec 5, 2011
// creature.h
// Description: An enemy, contains coordinates, health, and treasure values.
////////////////////////////

#ifndef CREATURE_H
#define CREATURE_H

#include <QGraphicsItem>
#include <QString>

// Items to be drawn are derived from QGraphicsItem class
// We must override the boundingRect() function to provide a bounding rectangle (allows framework to draw efficiently)
// We must also override the paint() function which performs the actual drawing

// Phase 2: timer in each Creature object was removed in favour of a single timer in the Track class
// The step(int delay) function is now invoked from Track class periodcally.

// Creature object handles all creture behaviour
class Creature : public QGraphicsItem
{
public:
    Creature(const QPolygon& path, int a, float b, int c, int d, int e); // Constructor takes the path along the creature must travel
    ~Creature();

    QRectF boundingRect() const;    // Tells the framework the area that this object occupies
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget); // Draw the object

    void step(int delay);           // advance the creature
    void erase();                   // removes the creature from the screen
    int getX();                     // returns X coordinate of creature
    int getY();                     // returns Y coordinate of creature
    void reduceHealth(int a);       // reduces the creature's health by a
    int getHealth();                // returns the creature's health
    void shot();                    // sets deathByBullet to true
    bool wasShot();                 // returns a true when creature dies from bullet
    int getType();                  // returns creature type
    int getTreasure();              // returns creature treasure value
    void setCollectedMoney(bool a); // sets collectMoney to a
    bool getCollectedMoney();       // determines if money needs to be collected
    int getFlying();                // returns creature flying value

protected:
    enum {W = 20, H = 15};                  // width and height
    const QPolygon &path;                   // The path along which to crawl
    int next;                               // Next vertex of the path to move toward
    int hp;                                 // Health of creature
    float speed;                            // Speed of creature
    int treasure;                           // Money left on death
    int flying;                             // Flying value for the creature (0 for ground)
    int type;                               // Type value for creature
    bool collected_money;                    // If money has been collected
    QString health;                         // String container for health, used in paint
    bool death_by_bullet;                     // Returns if creature died from bullet
    QColor type_colour;                      // Colour of creature, used in paint
};

#endif // CREATURE_H
