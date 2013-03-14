////////////////////////////
// TOWER DEFENSE PHASE 2
// Ben Pollock 250575023
// Dec 5, 2011
// track.cpp
////////////////////////////

#include "track.h"
#include "creature.h"
#include "bullet.h"
#include <QGraphicsScene>
#include <QtGui>
#include <cmath>
#include <iostream>
#include <QMessageBox>
#include <ctype.h>
#include <QInputDialog>

// initialize the parameters as appropriate
Track::Track() : wave_countdown(0), delay_countdown(10), shoot_countdown(0), start_wave(false), pause(false), startBtn(0), cash(0), escapees(0), waves(0), wavesend(0),opp_time(6), map_success(false), num("0"),num2("0"),num3("0"),num4("0"),num5("0"),num6("0"),num7("0"),num8("0"),num9("0"),x(0),selected_tower(-1),score(0),tower_pos(-100,-100),first_wave(true)
{
    // Initialize the path.
    path.putPoints(0,6,10,16);
    startTimer(DELAY); // wakeup every DELAY millisec

    //Attempt to open map
    QFile file("tdmap");
    QFile file2("tdmap");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text) )
    { // failed to open file
        pause=!pause;
        QMessageBox::critical(0, tr("File Open"), tr("Could not open tdmap.  Please check if it is readable and located in the\nbpolloc3/tower_defense _phase-build-desktop directory."));
        exit(0);
        return;
    }
    if(!file2.open(QIODevice::ReadOnly | QIODevice::Text)){
        //Stop the game if duplicate file won't open (should never happen)
        exit(0);
    }
    // Map is able to load successfully, implement values
    map_success=true;
    if (testMap(file2)==true){
        mapCheck(file);
        last_success = "tdmap";
    }
    else     // Maps in unable to load successfully, close game
        exit(0);
    file.close();  // Close files
    file2.close();
    pause=!pause;

    //Attempt to open highscore
    QFile highscore("td_highscore");
    if (!highscore.open(QIODevice::ReadOnly | QIODevice::Text) )
    { // failed to open file
       // filler here, just in case td_highscore doesn't exist
       pause=!pause;
       pause=!pause;
    }
    else{
        // Open highscore to user
       highScore(highscore);
       highscore.close();
       pause=!pause;
    }
}

// Tell the area occupied by the track object
QRectF Track::boundingRect() const
{
    return QRectF(-60,-60, 700,500);
}

// Do the initialization tasks. This is where all the buttons are created
void Track::init()
{
    QHBoxLayout *layout = new QHBoxLayout;          // Create a horizontal layout for a row of buttons
    layout->setSpacing(2);                          // set spacing beteen buttons to something small

    QPushButton *b = new QPushButton("Load Map");   // Create a button for loading a map
    connect(b, SIGNAL(clicked()), this, SLOT(loadMap()));   // connect this button to map loading function
    layout->addWidget(b);                           // Add it to the row of buttons

    // Create a set of push buttons for selecting towers. Button group is only a logical group. It is not a widget by it self
    // You will need to save the button group as a member variable to enable/disable buttons during game progression and update the tower information when a map is loaded
    // Read documentation of QButtonGroup class to see what facilities are offered
    QButtonGroup *bg = new QButtonGroup(layout);    // create a button group to hold the tower buttons. This is needed to create a mutually exclusive group of buttons
    connect(bg, SIGNAL(buttonClicked(int)), this, SLOT(selectTower(int)));  // Connect the whole button group to tower selection function

    //Determine which towers are on and set their arguments
    bool tower_nums[11];    // An array of 0-10.  If tower_nums[2] is true, that means that tower of type 2 exists, etc.
    int tower_loc[11];      // An array of 0-10.  If tower_loc[2] is 4, that means tower 2 is located at tower_obj.at(4)
    for (int i = 0; i < 11; i ++)
        tower_nums[i]=false;    // Towers default to not being set
    for (int i = 0; i < tower_obj.size();i++){  // Determine which towers exist
        tower_nums[tower_obj.at(i)->getType()]=true;
        tower_loc[tower_obj.at(i)->getType()]=i;
    }

    for (int i=1; i <= Track::MAX_TOWERS; i++) // create buttons for towers
    {
        b = new QPushButton(QString("T %1").arg(i)); // create a button with label "T n"
        if (tower_nums[i]==false){   // Tower doesn't exist so display default tooltip
            b->setEnabled(false);
            b->setToolTip(QString("Tower Not Set By Map")); // Create a tool tip that shows tower parameters
        }
        else   // Tower does exist so create special tooltip
        {
            // Temporary String variables to display tower info
            // More complicated than it should be but I was getting errors with simply using .arg() for more than 4 variables
            QStringList info;
            QString displayinfo("");
            QString temph("");
            //Store tower info
            temph = temph.setNum(tower_obj.at(tower_loc[i])->getCost());
            info << "Cost: " << temph;
           temph = temph.setNum(tower_obj.at(tower_loc[i])->getRange());
            info << "\nRange: " << temph;
            temph = temph.setNum(tower_obj.at(tower_loc[i])->getGroundDamage());
            info << "\nDamage Ground/Air: " << temph;
            temph = temph.setNum(tower_obj.at(tower_loc[i])->getAirDamage());
            info << "/" << temph;
            temph = temph.setNum(tower_obj.at(tower_loc[i])->getSplash());
            info << "\nSplash Damage: " << temph;
            temph = temph.setNum(tower_obj.at(tower_loc[i])->getCooldown());
            info << "\nCooldown Rate: " << temph;


            //Add it to tooltip
            for (int j = 0; j < info.size();j++)
                displayinfo.append(info.at(j));
            b->setToolTip(displayinfo);

            //Set towers to disabled if the user can't afford them, or if they can't be bought yet
            if (cash< tower_obj.at(tower_loc[i])->getCost()||wavesend < tower_obj.at(tower_loc[i])->getEllig())
                b->setEnabled(false);
        }
        b->setCheckable(true);          // make it type that stays pushed in
        b->setMaximumWidth(40);         // Set the width
        bg->addButton(b,i);             // Add it to the button group which takes care of keeping only one button pushed in
        layout->addWidget(b);           // Add the button to the button row
    }

    layout->addStretch(5);              // add a bit if space after the tower selection buttons
    startBtn = new QPushButton("Stop"); // create a button for pausing the game
    connect(startBtn, SIGNAL(clicked()), this, SLOT(gameFlow()));           // connect start button to start/stop function
    layout->addWidget(startBtn);        // add the start/stop button to the button row
    resetBtn = new QPushButton("Reset");
    connect(resetBtn, SIGNAL(clicked()), this, SLOT(reset()));
    layout->addWidget(resetBtn);
    QWidget *window = new QWidget;      // create a window to whold the row of buttons
    window->setLayout(layout);          // add the row of buttons to this window

    this->scene()->addWidget(window);   // add the button window to the scene
}

// Draw the track
void Track::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    // Set drawing parameters
    painter->setBrush(Qt::black);   // Black backround
    painter->setPen(QPen(QColor(139,69,19), 60, Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin)); // Brown, 60 pixels wide solid line with flat ends and round joins
    painter->drawPolyline(path);    // Draw the track
    painter->setPen(QPen(Qt::white,2));
    //Draw the Statistics:
    //Middle Row
    painter->drawText(0,470,"Time left in wave:");
    painter->drawText(125,470,num);
    painter->drawText(150,470,"Time until next wave:");
    painter->drawText(300,470,num2);
    painter->drawText(320,470,"Creatures allowed to escape before loss:");
    painter->drawText(590,470,num3);
    //Bottom Row
    painter->drawText(0,490,"Wave:");
    painter->drawText(125,490,num4);
    painter->drawText(150,490,"Type of next wave:");
    painter->drawText(290,490,num5);
    painter->drawText(320,490,"Creatures allowed to escape:");
    painter->drawText(590,490,num6);
    //Top Row
    painter->drawText(0,450,"Cash:");
    painter->drawText(115, 450, num7);
    painter->drawText(150,450,"Creatures left in wave:");
    //Bug Patching
    if (num8.toInt()<0)
        num8.setNum(0);
    painter->drawText(300,450,num8);
    painter->drawText(320,450,"Score");
    painter->drawText(590,450,num9);

}

// Called when user clicks the mouse
void Track::mousePressEvent(QGraphicsSceneMouseEvent * event){
    if(event->button() == Qt::LeftButton){
        tower_pos = event->pos();
        drawTower();  // draws a tower at the position of the mouse
    }
}

// Draws a tower of a selected type if user has appropriate funds
void Track::drawTower(){
    int towernum(-1);
    // Determines the location in tower_obj of selection tower
    for (int i = 0; i < tower_obj.size(); i++){
        if (tower_obj.at(i)->getType()==selected_tower)
            towernum = i;
    }
    // Draw the tower
    if (towernum >=0&&tower_obj.at(towernum)->getCost()<=cash && wavesend >= tower_obj.at(towernum)->getEllig()) {  // does the user have enough cash, and is the tower elligible
        Tower *newt = new Tower (0,0,tower_obj.at(towernum)->getType(),tower_obj.at(towernum)->getCost(),tower_obj.at(towernum)->getRange(),tower_obj.at(towernum)->getAirDamage(),tower_obj.at(towernum)->getGroundDamage(),tower_obj.at(towernum)->getSplash(),tower_obj.at(towernum)->getCooldown(),tower_obj.at(towernum)->getEllig());
        drawn_towers.push_front(newt);  // Add temporary tower into the drawn_towers list
        QPoint temp;
        temp = tower_pos.toPoint();
        drawn_towers.at(0)->setX(temp.x());  // Set its x and y coordinates
        drawn_towers.at(0)->setY(temp.y());
        drawn_towers.at(0)->setParentItem(this);
        cash = cash - newt->getCost();     // Decrease cash
        init();                            // Calling init() will redraw widgets to disable tower buttons when user can't afford tower
    }
}

// Timer function that handles waves of creatures
void Track::timerEvent(QTimerEvent *)
{
    if (pause)      // return immediately if paused
        return;
    if (first_wave){  // creatures left in wave set to value in wave_obj on first run
        num8.setNum(wave_obj.head()->getNum());
        first_wave=false;
    }
    shoot_countdown++;  // Increases shot_countdown, used to determine when towers can shoot based on cool_down
    num3=num3.setNum(escapees);
    num7=num7.setNum(cash);


    //Set number of creatures who have escaped
    int fail(0);
    for(int i = 0; i < creatures.size();i++)
        if(creatures.at(i)->isEnabled()==false&&creatures.at(i)->wasShot()==false)
            fail++;
    num6 = num6.setNum(fail);


    //Update Screen
    if (x == 0){
        this->scene()->setSceneRect(0,0,800,599);
        x=1;
    }
    else{
        this->scene()->setSceneRect(0,0,800,600);
        x=0;
    }


    //Erase Creatures off screen, add cash from dead creatures
    for (int i=0; i < creatures.size(); i++){    // go through every creature
        if(creatures.at(i)->isEnabled()==false){ // erase dead creatures
           creatures.at(i)->erase();
        }
        if (creatures.at(i)->getCollectedMoney()==false){ // collect money from recently killed creatures
            creatures.at(i)->setCollectedMoney(true);
            cash+=creatures.at(i)->getTreasure();
            score+=creatures.at(i)->getTreasure();  // increase score and cash
            num9.setNum(score);
            init();                                 // redraw tower buttons to reflect new cash
        }
    }

    //Let towers detect creatures, create new bullets
    for (int i = 0; i < drawn_towers.size(); i++){
        Creature *d = drawn_towers.at(i)->detect(waves,creatures);  // detect if creatures are in tower's range
        if (d != NULL){
            if(shoot_countdown%(drawn_towers.at(i)->getCooldown()*2)==0){  // Calculate cooldown, can the tower shoot?
             if (d->getFlying()==0){  // create a new ground bullet, targeting detected creature, from the tower's center
                Bullet *a = new Bullet(drawn_towers.at(i)->getX(), drawn_towers.at(i)->getY(),drawn_towers.at(i)->getGroundDamage(),drawn_towers.at(i)->getSplash(),d);
                bullets.push_back(a);
                a->setParentItem(this);
             }
             else{                    // create a new air bullet, targeting detected creature, from the tower's center
                Bullet *a = new Bullet(drawn_towers.at(i)->getX(), drawn_towers.at(i)->getY(),drawn_towers.at(i)->getAirDamage(),drawn_towers.at(i)->getSplash(),d);
                bullets.push_back(a);
                a->setParentItem(this);
             }
            }
        }
    }

    //Update bullets and calculate splash damage if neccessary
    for (int i = 0; i < bullets.size(); i ++){ // go through every bullet
        if (bullets.at(i)->isEnabled())
          bullets.at(i)->step(DELAY);          // update enabled bullets
        if (bullets.at(i)->getHitEnemy()){     // if the bullet has hit its enemy
            bullets.at(i)->setHitEnemy();
            // Go through every creature, calculating if they are within range for splash damage
            for (int j = 0; j < creatures.size(); j++){
                if ((int)creatures.at(j)->pos().x() - bullets.at(i)->getX() < bullets.at(i)->getSplash() && bullets.at(i)->getX()-(int)creatures.at(j)->pos().x() < bullets.at(i)->getSplash()){  // determine if x coordinates are within splash damage range
                    if((int)creatures.at(j)->pos().y() - bullets.at(i)->getY() < bullets.at(i)->getSplash() && bullets.at(i)->getY()-(int)creatures.at(j)->pos().y() < bullets.at(i)->getSplash()){ // determine if y coordinates are within splash damage range
                        if(creatures.at(j)!=bullets.at(i)->getCreature()){  // don't calculate splash damage on the same creature as the target creature
                            int x_distance = creatures.at(j)->pos().x()-bullets.at(i)->getX();   // calculate the x distance to the creature
                            int y_distance = creatures.at(j)->pos().y()-bullets.at(i)->getY();   // calculate the y distance to the creature
                            int distance = (int)sqrt(x_distance*x_distance + y_distance*y_distance);  // calculate the angular distance to the creature
                            int damage = (int)(bullets.at(i)->getDamage()*(0.2+0.6*(bullets.at(i)->getSplash()-distance)/bullets.at(i)->getSplash())); // calculate splash damage to be inflicted
                            creatures.at(j)->reduceHealth(damage);  // reduce creature's health by the amount calculated
                            if (creatures.at(j)->getHealth()<=0){  // if the creature dies from the splash damage, perform usual on-death steps
                                creatures.at(j)->erase();
                                creatures.at(j)->setEnabled(false);
                                creatures.at(j)->shot();
                                creatures.at(j)->setCollectedMoney(false);
                            }
                        }
                    }
                }
            }
        }
    }

    // Determine if player has lost
    if(escapees-fail <=0 && escapees!=0){  // have too many creatures escaped?
        QMessageBox::critical(0, tr("Game Over"), tr("You have lost!")); // displaying losing popup
        exit(0);
    }

    // Determine if player has won
    if (wave_obj.isEmpty()){  // no more waves should be left
        bool win = true;
        for (int i = 0; i < creatures.size(); i++)  // no more creatures should be left
            if (creatures.at(i)->isEnabled())
                win = false;
        if (win){  // has the user won?
            QWidget * win;
            QMessageBox::critical(0, tr("Game Over"), tr("You have won!"));  //display winning popup
            QFile file("td_highscore");
            if (!file.open(QIODevice::ReadOnly | QIODevice::Text) ){  // if there is no td_highscore, exit the game
                exit(0);
            }
            // Prompt user to enter highscore
            bool ok;
            QString text = QInputDialog::getText(win, tr("Highscore!!"),tr("Please enter your name for the\nHIGHSCORE Table:"), QLineEdit::Normal,"", &ok);
            QFile hs("td_highscore");
            hs.open(QIODevice::WriteOnly | QIODevice::Append);
            highScoreEntry(hs,last_success, text);   // enter the inputted name to the highscore table
            exit(0);
        }
    }

    if (start_wave&&wave_obj.isEmpty()!=true) // Are we releasing creatures?
    {
        // Still releasing creatures
        wave_countdown--;               // Decrement the count down timer
        num.setNum(wave_countdown);
        num5="-";

        //Reset screen
        if (x == 0){
            this->scene()->setSceneRect(0,0,800,599);
            x=1;
        }
        else{
            this->scene()->setSceneRect(0,0,800,600);
            x=0;
        }

        if (wave_countdown >0)         // Have we reached the end of the wave
        {

            // Not end of wave. Keep releasing creatures
            if ((wave_countdown+1)%((int)((wave_obj.head()->getTime()*10)/wave_obj.head()->getNum()))==0)    // Wait for the opportune moment
            {
                //Reset screen
                num8.setNum(num8.toInt()-1);
                if (x == 0){
                    this->scene()->setSceneRect(0,0,800,599);
                    x=1;
                }
                else{
                    this->scene()->setSceneRect(0,0,800,600);
                    x=0;
                }
                Creature *c = new Creature(getPath(),wave_obj.head()->getHp(),wave_obj.head()->getSpeed(),wave_obj.head()->getTreasure(),wave_obj.head()->getFlying(),wave_obj.head()->getType());  // Instantiate a new creature
                creatures << c;                         // add it to the list
                c->setParentItem(this);                 // Make it a child object of the track so that it gets added to the scene automatically. This also means that the coordinates of the creatures are relative to the track
            }

        }
        else if(wave_obj.isEmpty()!=true)
        {

            // End of wave. Start the count down until next wave
            start_wave = false;     // Indicate that we are waiting for next wave
            delay_countdown = (wave_obj.head()->getDelay()*10);  // Wait for 500*1/10 (50) sec
            wave_obj.dequeue();            // remove the wave from the list
            wavesend++;                    // increase end of wave count
            init();
            if (wave_obj.isEmpty()!=true){ // are there any waves left?
                //Reset screen
                num8.setNum(wave_obj.head()->getNum());
                if (x == 0){
                  this->scene()->setSceneRect(0,0,800,599);
                  x=1;
                }
              else{
                  this->scene()->setSceneRect(0,0,800,600);
                 x=0;
              }
            }

        }
    }
    else if (wave_obj.isEmpty()!=true)
    {
        // Waiting for next wave
        //Draw time until next wave
        num2.setNum(delay_countdown);
        //Draw type of creatures in next wave
        num5.setNum(wave_obj.head()->getType());

        //Reset screen
        if (x == 0){
            this->scene()->setSceneRect(0,0,800,599);
            x=1;
        }
        else{
            this->scene()->setSceneRect(0,0,800,600);
            x=0;
        }

        delay_countdown--;          // count down to next wave

        if (delay_countdown < 0)    // Are we done waiting?
        {
            waves++;  // Increase wave number
            num4.setNum(waves);

            // Reset Screen
            if (x == 0){
                this->scene()->setSceneRect(0,0,800,599);
                x=1;
            }
            else{
                this->scene()->setSceneRect(0,0,800,600);
                x=0;
            }

            // Yes. Start the next wave
            start_wave = true;      // set the flag
            wave_countdown = ((wave_obj.head()->getTime())*10);
        }
    }

    // Update each active creature
    for (int i=0; i < creatures.size(); i++)
        if (creatures.at(i)->isEnabled())
            creatures.at(i)->step(DELAY);
}

// slot for the start/stop button
void Track::gameFlow()
{
    pause = !pause; // flip the game state
    if (pause)      // Change button label as appropriate
        startBtn->setText("Start");
    else
        startBtn->setText("Stop");
}

// Resets the game to state when map was originally loaded
void Track::reset()
{
    pause = !pause;
    QFile file(last_success);  // sets QFile to the last map file that was loaded (successfully)
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text) )
    { // failed to open file
        QMessageBox::critical(0, tr("File Open"), tr("File cannot be opened."));
        return;
    }
    QFile file2(last_success);
    if(!file2.open(QIODevice::ReadOnly | QIODevice::Text)){
        pause=!pause;
        pause=!pause;
    }

    // Go through file again, redoing all values
    if (testMap(file2)==true){
       //Reset Values
       wave_countdown=0; delay_countdown=10; shoot_countdown=0; start_wave=false; startBtn=0; cash=0; escapees=0; waves=0; wavesend=0; opp_time=6; map_success=false; num="0";num2="0"; num3="0"; num4="0"; num5="0"; num6="0"; num7="0";num8="0"; x=0; selected_tower=-1; tower_pos.setX(-100); tower_pos.setY(-100); first_wave=true; score=0;
       mapCheck(file);
    }
    file.close();  // Close the files
    file2.close();
    init();  // Recreate the widgets and buttons
    update();
    pause=!pause;
}

// slot for the tower buttons. Because of button group, this gets called with button id for any of the buttons
void Track::selectTower(int t)
{
    selected_tower = t;
}

// Enters a highscore in td_highscore
void Track::highScoreEntry(QFile &file, QString map, QString user){
    QTextStream x( &file );
    x << map <<":"<<user<<":"<<num7<<"\n";  //Enters the highscore
}

// Displays the highscores it td_highscore
void Track::highScore(QFile &file){
    QTextStream x( &file );        // use a text stream
    if (pause == false)
        pause = true;
    int nline(0);
    QString line;
    QStringList names;
    names << "Map Name\tPlayer\tScore\n\n";  // used to display title of highscore table
    while ( !x.atEnd() )
    {
        line = x.readLine();        // read a line of text excluding '\n'
        if (line.at(0) == '#')      // if it begins with '#', skip
            continue;
        nline++;                    // increment line counter
        QStringList fields = line.split(':');  // split the line along ':'
        names << fields.at(0) << "\t" << fields.at(1)<<"\t" << fields.at(2)  << "\n";
    }
    if (names.isEmpty()!=true){  // is the name string empty?
        QString storage;
        for (int i=0; i < names.count();i++)  // add names and scores to highscore value
            storage.append(names.value(i));
        QMessageBox::information(0, tr("Highscore Table"), tr("%1").arg(storage)); // display highscore table
    }

}

// Tests the map before implementing to avoid needing to reset everything
bool Track::testMap(QFile &file){
    QTextStream x( &file );        // use a text stream
    x.seek(0);
    x.flush();

    int nline(0);
    QString line;
    if (pause == false)
        pause = true;
    while ( !x.atEnd() )
    {
        // read and parse the file
        line = x.readLine();        // read a line of text excluding '\n'
        if (line.at(0) == '#')      // if it begins with '#', skip
            continue;
        nline++;                    // increment line counter
        // do something with the line
        QStringList fields = line.split(':');  // split the line along ':'


        //Called when a new track is uploaded
        if (fields.at(0)!="T"&&fields.at(0)!="W"&&isdigit(fields.at(0).toStdString()[0])&&fields.at(0)!="0"){
            QStringList errors;
            if (fields.count()!=fields.at(0).toInt()*2+3){
                QMessageBox::critical(0, tr("File Open"), tr("Could not open line %1, invalid number of values.").arg(nline));
                return false;
            }
            if (fields.at(0).toInt()<=0)
                errors << "Point value is not > 0 " << fields.at(0) <<"/n";
            // Test the new path
            for (int i=1; i < fields.count()-2; i=i+2) // fields is an array of QString
            {
                if (fields.at(i).toInt()<0 || fields.at(i).toInt()<0)
                    errors << "Invalid Coordinate\n";
            }
            if (fields.at(fields.count()-2).toInt()<0)
                errors << "Negative Cash Value";
            if (fields.at(fields.count()-1).toInt()<=0)
                errors << "Escapees is not > 0";
            QString storage;
            if (errors.isEmpty()!=true){
                errors.removeDuplicates();
                for (int i=0; i < errors.count();i++)
                    storage.append(errors.value(i));
                QMessageBox::critical(0, tr("File Open"), tr("Error: \n%1On line %2").arg(storage).arg(nline));
                return false;
            }

        }

        // Tests the wave
        else if(fields.at(0)=="W"){

            QStringList errors;
            if (fields.count()!=9){
                QMessageBox::critical(0, tr("File Open"), tr("Could not open line %1, invalid number of values.").arg(nline));
                return false;
            }
            if (fields.at(1).toInt()<0||fields.at(1).toInt()>50)
                errors << "Creature Type is not between 0 and 50.\n";
            if (fields.at(2).toInt()<=0)
                errors << "Creature Hitpoints are not positive.\n";
            if (fields.at(3).toInt()<0)
                errors << "Invalid Creature Flying parameter.\n";
            if (fields.at(4).toFloat()<0 || fields.at(4).toFloat()>5)
                errors << "Creature Speed is not between 0 and 5.\n";
            if (fields.at(5).toInt()<=0)
                errors << "Treasure value is not positive.\n";
            if (fields.at(6).toInt()<=0)
                errors << "Number of creatures is not positive.\n";
            if (fields.at(7).toInt()<=0)
                errors << "Time is not positive.\n";
            if (fields.at(8).toInt()<=0)
                errors << "Delay is not positive.\n";


            QString storage;
            if (errors.isEmpty()!=true){
                for (int i=0; i < errors.count();i++)
                    storage.append(errors.value(i));
                QMessageBox::critical(0, tr("File Open"), tr("Error: \n%1On line %2").arg(storage).arg(nline));
                return false;
            }
        }
        // Tests the tower
        else if(fields.at(0)=="T"){
            QStringList errors;
            if (fields.count()!=8){
                QMessageBox::critical(0, tr("File Open"), tr("Could not open line %1, invalid number of values.").arg(nline));
                return false;
            }
            if (fields.at(1).toInt()<1 || fields.at(1).toInt()>10)
                errors << "Tower type is not between 1 and 10.\n";
            if (fields.at(2).toInt() <=0)
                errors << "Tower cost is not positive.\n";
            if (fields.at(3).toInt() <=0)
                errors << "Tower range is not positive.\n";
            if (fields.at(4).toInt()<0)
                errors << "Air damage is less than 0\n";
            if (fields.at(5).toInt()<0)
                errors << "Ground Damage is less than 0\n";
            if (fields.at(4).toInt()==0&&fields.at(5).toInt()==0)
                errors << "Air and ground damage are both 0.\n";
            if (fields.at(6).toInt()<0 || fields.at(6).toInt()>100)
                errors << "Splash damage is not between 0 and 200.\n";
            if (fields.at(7).toInt()<=0)
                errors << "Cooldown is less than 0.\n";
            QString storage;
            if (errors.isEmpty()!=true){
                for (int i=0; i < errors.count();i++)
                    storage.append(errors.value(i));
                QMessageBox::critical(0, tr("File Open"), tr("Error: \n%1On line %2").arg(storage).arg(nline));
                return false;
            }
        }
        else{
            QMessageBox::critical(0,("File Open"), tr("Error: \nLine %1does not start with T or W.").arg(nline));
            return false;
        }
    }
    return true;
}

// Used to actually change the map variables
void Track::mapCheck(QFile &file){
    if (pause==false)
        pause = true;

    //Reset the object files
    while (!wave_obj.isEmpty())
        delete wave_obj.dequeue();
    while (!drawn_towers.isEmpty()){
        Tower *temp = drawn_towers.at(0);
        temp->erase();
        drawn_towers.pop_front();
    }
    while (!tower_obj.isEmpty()){
        tower_obj.pop_front();
    }
    while (!creatures.isEmpty()){
        Creature *temp = creatures.at(0);
        creatures.pop_front();
        temp->setEnabled(false);
        temp->erase();
    }

    QTextStream t( &file );        // use a text stream
    int nline(0);
    QString line;
    while ( !t.atEnd() )
    {
        // read and parse the file
        line = t.readLine();        // read a line of text excluding '\n'
        if (line.at(0) == '#')      // if it begins with '#', skip
            continue;
        nline++;                    // increment line counter
        // do something with the line
        QStringList fields = line.split(':');  // split the line along ':'


        //Called when a new track is uploaded
        if (fields.at(0)!="T"&&fields.at(0)!="W"){
            //Create the new path
            path.resize(fields.at(0).toInt());
            for (int i=1; i < fields.count()-2; i=i+2) // fields is an array of QString
                  path.setPoint((int)((i-1)/2),fields.at(i).toInt(),fields.at(i+1).toInt());

            //Set the cash and escapees value
            cash=(fields.at(fields.count()-2)).toInt();
            escapees=(fields.at(fields.count()-1)).toInt();


            //Erase creatures already on the map
            for (int i=0; i < creatures.size(); i++){
                    creatures.at(i)->setEnabled(false);
                    creatures.at(i)->erase();
            }
            update();
        }

        // Creates a new wave
        else if(fields.at(0)=="W"){
            //Create new wave
            Wave *a = new Wave(fields.at(6).toInt(),fields.at(7).toInt(),fields.at(8).toInt(),fields.at(1).toInt(),fields.at(3).toInt(),fields.at(4).toFloat(),fields.at(2).toInt(),fields.at(5).toInt());
            wave_obj.enqueue(a);
        }
        // Creates a new tower
        else if(fields.at(0)=="T"){
            Tower *a = new Tower(0,0,fields.at(1).toInt(),fields.at(2).toInt(), fields.at(3).toInt(),fields.at(4).toInt(),fields.at(5).toInt(),fields.at(6).toInt(),fields.at(7).toInt(),wave_obj.size());
            tower_obj.push_back(a);
        }
    }
}

// slot for loading a map file
void Track::loadMap()
{
    // Get the file user wants to load
    pause = !pause;
    QString fileName = QFileDialog::getOpenFileName(0, tr("Load Map"));
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text) )
    { // failed to open file
        QMessageBox::critical(0, tr("File Open"), tr("File cannot be opened."));
        return;
    }

    // Attempt to open file
    QFile file2(fileName);
    if(!file2.open(QIODevice::ReadOnly | QIODevice::Text)){
        //Filler, just in case the second file won't load
        pause=!pause;
        pause=!pause;
    }
    if (testMap(file2)==true){ //if the file is vaild
       //Reset Values
       wave_countdown=0; delay_countdown=10; shoot_countdown=0; start_wave=false; startBtn=0; cash=0; escapees=0; waves=0; wavesend=0; opp_time=6; map_success=false; num="0";num2="0"; num3="0"; num4="0"; num5="0"; num6="0"; num7="0";num8="0"; x=0; selected_tower=-1; tower_pos.setX(-100); tower_pos.setY(-100); first_wave=true; score=0;
        mapCheck(file);  // load the new map
        last_success = fileName; // set the map name
    }
    file.close();
    file2.close();  // close the files
    init();         // redraw the widgets
    update();       // redraw the screen
}

