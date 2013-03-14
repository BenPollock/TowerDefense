////////////////////////////
// TOWER DEFENSE PHASE 2
// Ben Pollock 250575023
// Dec 5, 2011
//  main.cpp
////////////////////////////


// Tower Defense template created by Jagath Samarabandu, Sept. 23, 2010. Modified Oct 24, 2011
// This applications uses the GraphicsView framework in Qt.

#include <QtGui>
#include "track.h"
#include "creature.h"

#include <math.h>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    // Scene object takes care of drawing all the items in the scene
    QGraphicsScene scene;
    scene.setSceneRect(0, 0, 800, 600); // Top left coordinate is (0,0). Size is 800x600

    // Track is a subclass of QGraphicsItem which defines how it is drawn
    Track *t = new Track;   // create the object
    t->setPos(100,100);     // Set its position relative to scene
    scene.addItem(t);       // Add the track object to scene
    t->init();              // Initialize only after adding to scene

    // Create a viewing area for the scene
    QGraphicsView view(&scene);
    view.setRenderHint(QPainter::Antialiasing); // anti-aliasing makes it draw nice smooth lines
    view.setBackgroundBrush(Qt::black);         // Set the backtround color to black

    // Set the window title
    view.setWindowTitle(QT_TRANSLATE_NOOP(QGraphicsView, "Tower Defence - Phase 1"));
    view.show();        // show the viewing area on screen

    return app.exec();  // Start the application
}
