////////////////////////////
// TOWER DEFENSE PHASE 2
// Ben Pollock 250575023
// Dec 5, 2011
// track.h
// Description:  Most of the code is contained within this object.  It draws a track,
// which contains wave, tower, and bullet objects.  It also contains the widgets to
// select towers, reset the game, input a highscore, and load a new map.  All statistics
// such as creatures escaped, cash, etc are stored and displayed here
////////////////////////////

#ifndef TRACK_H
#define TRACK_H

#include <QGraphicsItem>
#include <QObject>
#include <QQueue>
#include <QMouseEvent>
#include <QList>
#include <QFile>
#include <QPoint>
#include <QPointF>
#include "wave.h"
#include "tower.h"
#include "bullet.h"

class Creature;                             // Phase 2: forward declaration for the creature list
class QPushButton;                          // Phase 2: forward declaration for the button pointer

// Items to be drawn are derived from QGraphicsItem class
// We must override the boundingRect() function to provide a bounding rectangle (allows framework to draw efficiently)
// We must also override the paint() function which performs the actual drawing
// Derive from QObject class as well for the timer functionality

// Track object handles drawing track, creating waves of creatures etc

class Track : public QObject, public QGraphicsItem
{
    Q_OBJECT                                // Phase 2: add support for signals and slots
    Q_INTERFACES(QGraphicsItem)             // Phase 2: needed for signals and slots

public:
    Track();

    void init();                            // Phase 2: Do the initialization tasks
    QRectF boundingRect() const;            // Tell the framework the area occupied by this object
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget); // How this object is drawn
    const QPolygon& getPath() { return path; } // accessor for the track coordinates

    enum {MAX_TOWERS=10, DELAY=50};         // Phase 2: constants for various things
    void mapCheck(QFile &file);             // Used to load variables from a map
    bool testMap(QFile &file);              // Returns false if the map has problems to ensure no variables are overwritten in the event of a bad map
    void highScore(QFile &file);            // Used to load a highscore if td_highscore exists
    void highScoreEntry(QFile &file, QString map, QString user);    // Used to enter a highscore if user wins and td_highscore exists
    void mousePressEvent (QGraphicsSceneMouseEvent * event);        // Called when user clicks the mouse
    void drawTower();                       // Draws a tower when appropriate

public slots:
    void selectTower(int t);                // Phase 2: slot for selecting towers
    void gameFlow();                        // Phase 2: slot for pausing game flow
    void loadMap();                         // Phase 2: slot for loading a map
    void reset();                           // Used to reset map

private:
    void timerEvent(QTimerEvent *event);    // Timer function handles releasing waves of creatures. Called every DELAY msec.
    QPolygon path;                          // coordinates of the track
    int wave_countdown, delay_countdown;    // Parameters for handling waves of creatures
    int shoot_countdown;                    // Parameters for handling cooldown of towers
    bool start_wave;                        // flag indicating that a wave of creatures is being released
    bool pause;                             // Phase 2: flag to pause/continue game
    QList<Creature*> creatures;             // Phase 2: List for holding all the creatures
    QPushButton *startBtn;                  // Phase 2: hold the button so that we can alter the label
    QPushButton *resetBtn;                  // Button to reset the game
    int cash;                               // money the user has
    int escapees;                           // how many escapees are allowed before loss
    int waves;                              // the current wave number
    int wavesend;                           // the current wave, incremented at the END of a wave (for tower elligibility purposes)
    int opp_time;                           // proper time to release a creature
    bool map_success;                       // TRUE if map can be successfully loaded (ie no errors)
    int score;                              // current score

    QQueue<Wave*> wave_obj;                 // Aqueue of waves, loaded from map
    QList<Tower*> tower_obj;                // A list of towers, loaded from map
    QList<Tower*> drawn_towers;             // List of towers on screen

    QString num;                            // String container for time left in wave
    QString num2;                           // String container for time until next wave
    QString num3;                           // String container for escapees
    QString num4;                           // String container for Wave number
    QString num5;                           // String container for the next wave creature type
    QString num6;                           // String container for # of creatures who have escaped
    QString num7;                           // String container for cash
    QString num8;                           // String container for creatures left in the wave
    QString num9;                           // String container for user score

    QString last_success;                    // fileName of the last successful map load (used in reset)
    bool first_wave;                         // Defaults to true, set to false after first wave to avoid resetting num8 (for creatures left in wave)
    int x;                                  // 1 or 0 value used by help update screen when one of the numbers changes
    QList<Bullet*> bullets;                 // List of bullets
    int selected_tower;                      // Int value of selected tower
    QPointF tower_pos;                      // Temporary storage of a tower position
};

#endif // TRACK_H
