/*
* Ho Chi Minh City University of Technology
* Faculty of Computer Science and Engineering
* Initial code for Assignment 1
* Programming Fundamentals Spring 2023
* Author: Vu Van Tien
* Date: 02.02.2023
*/

//The library here is concretely set, students are not allowed to include any other libraries.
#ifndef _H_STUDY_IN_PINK_2_H_
#define _H_STUDY_IN_PINK_2_H_

#include "main.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

// Forward declaration
class MapElement;
class Path;
class Wall;
class FakeWall;
class MovingObject;
class Position;
class Configuration;
class Map;

class Character;
class Sherlock;
class Waston;
class Criminal;

class RobotS;
class RobotW;
class RobotSW;
class RobotC;

class ArrayMovingObject;
class StudyPinkProgram;

class BaseItem;
class BaseBag;
class SherlockBag;
class WatsonBag;

class StudyInPink;
class TestStudyInPink;

int mainNumber(int i, int j);
int Distance(Position pos1, Position pos2);
int sumOfNumber(int a);
int setInfor(int x, int max);

enum ItemType { MAGIC_BOOK, ENERGY_DRINK, FIRST_AID, EXCEMPTION_CARD, PASSING_CARD };
enum ElementType { PATH, WALL, FAKE_WALL };
enum RobotType { C=0, S, W, SW };

class MapElement {
    friend class TestStudyInPink;

protected:
    ElementType type;
public:
    MapElement(ElementType in_type);
    ElementType getType() const ;
    virtual ~MapElement(){};
};

class Path : public MapElement {
    friend class TestStudyInPink;

public:
    Path():MapElement(PATH){};
};

class Wall : public MapElement {
    friend class TestStudyInPink;

public:
    Wall():MapElement(WALL){};
};

class FakeWall : public MapElement {
    friend class TestStudyInPink;

private:
    int req_exp;
public:
    FakeWall(int in_req_exp);
    int getReqExp() const;
};

class Map {
    friend class TestStudyInPink;

private:
    int num_rows, num_cols;
    MapElement*** map;

public:
    Map(int num_rows, int num_cols, int num_walls, Position* array_walls, int num_fake_walls, Position* array_fake_walls);
    ~Map();
    bool isValid (const Position & pos , MovingObject* mv_obj) const ;
    int getNumRows();
    int getNumCols();
};

class Position {
    friend class TestStudyInPink;

private:
    int r, c;
public:
    static const Position npos;
    
    Position(int r=0, int c=0);
    Position(const string & str_pos);
    int getRow() const;
    int getCol() const;
    void setRow(int r);
    void setCol(int c);
    string str() const;
    bool isEqual(int in_r, int in_c) const;
    bool isEqual(Position pos);
};

class MovingObject {
    friend class TestStudyInPink;

protected:
    int index;
    Position pos;
    Map *map;
    string name;

public:
    MovingObject(int index, const Position pos, Map * map, const string & name="");
    virtual Position getNextPosition() = 0;
    Position getCurrentPosition() const;
    virtual void move() = 0;
    virtual string str() const = 0;
    virtual string getName();
};

class Character:public MovingObject{
    friend class TestStudyInPink;

protected:
    int hp;
    int exp;
    int num_steps;
    string moving_rule;
public:
    Character(int index, int hp, int exp, Map * map, Position pos, string moving_rule);
    int getExp();
    int getHp();
    void setExp(int in_exp);
    void setHp(int in_hp);
    virtual Position getNextPosition();
    virtual void move();
};

class Sherlock:public Character {
    friend class TestStudyInPink;

public:
    Sherlock(int index, const string & moving_rule, const Position & init_pos, Map * map, int init_hp, int init_exp);
    string str() const;
    void setPos(Position pos);
};

class Watson:public Character {
    friend class TestStudyInPink;

public:
    Watson(int index, const string & moving_rule, const Position & init_pos, Map * map, int init_hp, int init_exp);
    string str() const;

};

class Criminal:public Character{
    friend class TestStudyInPink;

private:
    Position pastPos = Position(-1,-1);
    Sherlock * sherlock;
    Watson * watson;
   
public:
    Criminal(int index, const Position & init_pos, Map * map, Sherlock * sherlock, Watson * watson);
    Position getNextPosition() override;
    string str() const;
    void move() override;
    int getStepReal();
    Position getPastPos();
};

class ArrayMovingObject {
    friend class TestStudyInPink;

private:
    MovingObject **arr_mv_objs;
    int count, capacity;

public:
    ArrayMovingObject(int capacity);

    ~ArrayMovingObject() ;
    bool isFull() const;
    bool add(MovingObject * mv_obj);
    MovingObject * get(int index) const;
    int size() const; // return current number of elements in the array
    string str() const;
}; 

class Configuration {
    friend class TestStudyInPink;
    friend class StudyPinkProgram;

private:
    int map_num_rows, map_num_cols;
    int max_num_moving_objects;
    int num_walls;
    Position *arr_walls;
    int num_fake_walls;
    Position *arr_fake_walls;
    string sherlock_moving_rule;
    Position sherlock_init_pos;
    int sherlock_init_hp, sherlock_init_exp;
    string watson_moving_rule;
    Position watson_init_pos;
    int watson_init_hp, watson_init_exp;
    Position criminal_init_pos;
    int num_steps;

public:
    Configuration(const string & filepath);
    ~Configuration();
    string str() const;
    void parsePositions(const string& input, Position*& positions, bool isNumWall );
    void parsePosition(const string& input, Position& position);
};

class Robot:public MovingObject {
    friend class TestStudyInPink;

protected:
    RobotType robot_type;
    BaseItem *item;
    Criminal* criminal;
public:
    Robot(int index, const Position pos, Map *map, Criminal* criminal);
    ~Robot();
    RobotType getType();
    void move();
    BaseItem* getItem();
    virtual Position getNextPosition() = 0;
    virtual int getDistance() const = 0;
    virtual string str() const = 0;
};

class RobotC : public Robot {
    friend class TestStudyInPink;

public:
    RobotC (int index, const Position & init_pos, Map *map, Criminal *criminal);
    Position getNextPosition();
    int getDistance(Character *character) const;
    int getDistance() const{return -1;};
    string str() const;
};

class RobotS : public Robot {
    friend class TestStudyInPink;

private:
    Sherlock *sherlock;
public:
    RobotS (int index, const Position & init_pos, Map *map, Criminal *criminal, Sherlock *sherlock);
    Position getNextPosition();
    int getDistance() const;
    string str() const;
};

class RobotW : public Robot {
    friend class TestStudyInPink;

private:
    Watson *watson;
public:
    RobotW (int index, const Position & init_pos, Map *map, Criminal *criminal, Watson *watson);
    Position getNextPosition();
    int getDistance() const;
    string str() const;
};

class RobotSW : public Robot {
    friend class TestStudyInPink;

private:
    Sherlock *sherlock;
    Watson *watson;
public:
    RobotSW (int index, const Position & init_pos, Map *map, Criminal *criminal, Sherlock *sherlock, Watson *watson);
    Position getNextPosition();
    int getDistance() const;
    string str() const;
};

class BaseItem {
    friend class TestStudyInPink;

private:
    ItemType itemType;
public:
    BaseItem(ItemType in_itemType);
    virtual bool canUse(Character* obj, Robot* robot) = 0;
    virtual void use(Character* obj, Robot* robot) = 0;
    ItemType getItemType();
    virtual string str() const = 0;
}; 
 
class MagicBook : public BaseItem {
    friend class TestStudyInPink;

public:
    MagicBook() : BaseItem(MAGIC_BOOK){};
    bool canUse(Character* obj, Robot* robot);
    void use(Character* obj, Robot* robot);
    string str() const;
};

class EnergyDrink : public BaseItem {
    friend class TestStudyInPink;

public:
    EnergyDrink() : BaseItem(ENERGY_DRINK){};
    bool canUse(Character* obj, Robot* robot);
    void use(Character* obj, Robot* robot);
    string str() const;
};

class FirstAid : public BaseItem {
    friend class TestStudyInPink;

public:
    FirstAid() : BaseItem(FIRST_AID){};
    bool canUse(Character* obj, Robot* robot);
    void use(Character* obj, Robot* robot);
    string str() const;
};

class ExcemptionCard : public BaseItem {
    friend class TestStudyInPink;

public:
    ExcemptionCard() : BaseItem(EXCEMPTION_CARD){};
    bool canUse(Character* obj, Robot* robot);
    void use(Character* obj, Robot* robot);
    string str() const;
};

class PassingCard : public BaseItem {
    friend class TestStudyInPink;

private:
    string challenge;
public:
    PassingCard(int t);
    PassingCard(string v):BaseItem(PASSING_CARD){
        this->challenge = v;
    }
    bool canUse(Character* obj, Robot* robot);
    void use(Character* obj, Robot* robot);
    string str() const;
};

class Node{
    friend class TestStudyInPink;

    public:
        BaseItem* item;
        Node* next;
    public:
        Node(BaseItem* item, Node* next = nullptr):item(item), next(next){}
};

class BaseBag {
    friend class TestStudyInPink;

protected:
    int size, capacity;
    Node* head;
public:
    BaseBag(int capacity);
    ~BaseBag();
    void swap (Node* a, Node* b);
    bool insert (BaseItem* item); 
    bool isFull();
    virtual BaseItem* get () = 0;
    virtual BaseItem* get(ItemType itemType) = 0;
    bool checkItem(ItemType item);
    string str () const;
};

class SherlockBag : public BaseBag {
    friend class TestStudyInPink;

private:
    Sherlock* sherlock;
public:
    SherlockBag(Sherlock* sherlock);
    BaseItem* get () override;
    BaseItem* get(ItemType itemType) override;
};

class WatsonBag : public BaseBag {
    friend class TestStudyInPink;

private:
    Watson* watson;
public:
    WatsonBag(Watson* watson);
    BaseItem* get () override;
    BaseItem* get(ItemType itemType) override;
};

class StudyPinkProgram{
    friend class TestStudyInPink;

private:
    Configuration *config;
    Sherlock *sherlock;
    Watson *watson;
    Criminal *criminal;
    Map *map;
    ArrayMovingObject *arr_mv_objs;
    SherlockBag* sherlockBag;
    WatsonBag* watsonBag;

public:
    StudyPinkProgram(const string &config_file_path);
    bool isStop() const;
    void printResult() const
    {
        if (sherlock->getCurrentPosition().isEqual(criminal->getCurrentPosition()))
        {
            cout << "Sherlock caught the criminal" << endl;
        }
        else if (watson->getCurrentPosition().isEqual(criminal->getCurrentPosition()))
        {
            cout << "Watson caught the criminal" << endl;
        }
        else
        {
            cout << "The criminal escaped" << endl;
        }
    }

    void printStep(int si) const
    {
        cout << "Step: " << setw(4) << setfill('0') << si
             << "--"
             << sherlock->str() << "--|--" << watson->str() << "--|--" << criminal->str() << endl;
    }
    void run(bool verbose);
    ~StudyPinkProgram();
};

////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////
#endif /* _H_STUDY_IN_PINK_2_H_ */
