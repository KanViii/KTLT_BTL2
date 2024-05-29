
#include "study_in_pink2.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

//TASK 3.1: MAP'S ELEMENTS
MapElement::MapElement(ElementType in_type){
    this->type = in_type;
}
ElementType MapElement::getType() const{
    return this->type;
}
FakeWall::FakeWall(int in_req_exp) : MapElement(FAKE_WALL){
    this->req_exp = in_req_exp;
}
int FakeWall::getReqExp() const{
    return this->req_exp;
}

//TASK 3.2: MAP
Map::Map(int num_rows, int num_cols, int num_walls, Position* array_walls, int num_fake_walls, Position* array_fake_walls) : num_rows(num_rows), num_cols(num_cols){
    map = new MapElement**[num_rows];
    for (int i=0; i<num_rows; i++){
        map[i] = new MapElement*[num_cols];
    }

    for (int i=0; i<num_rows; i++){
        for (int j=0; j<num_cols; j++){
            map[i][j] = new Path();
            for (int k=0; k<num_fake_walls; k++){
                if (array_fake_walls[k].isEqual(i,j)) {
                    delete map[i][j];
                    map[i][j] = new FakeWall((i*257 + j*139 + 89)%900 + 1);
                }
            }  
            for (int k=0; k<num_walls; k++){
                if (array_walls[k].isEqual(i,j)) {
                    delete map[i][j];
                    map[i][j] = new Wall();
                }
            }  
        }
    }
}
Map::~Map() {
    for (int i=0; i<num_rows; i++){
        for (int j=0; j<num_cols; j++){
            delete map[i][j];
        }
        delete[] map[i];
    }
    delete[] map;
}
int Map::getNumRows(){
    return this->num_rows;
}
int Map::getNumCols(){
    return this->num_cols;
}
bool Map::isValid (const Position & pos , MovingObject* mv_obj) const{
    int posR = pos.getRow(), posC = pos.getCol();
    if(posR<0 || posR>=num_rows || posC<0 || posC>=num_cols) {return false;}
    if (map[posR][posC]->getType() == WALL) {return false;}
    if (map[posR][posC]->getType() == FAKE_WALL){
        if (mv_obj->getName() == "Watson" && dynamic_cast<Watson*>(mv_obj)->getExp() <= dynamic_cast<FakeWall*>(map[posR][posC])->getReqExp()) {return false;}
    }
    return true;
}

//TASK 3.3: POSITION
Position::Position(int r, int c){
    this->r = r;
    this->c = c;
}
Position::Position(const string &str_pos){
    stringstream ss(str_pos.substr(1, str_pos.length() - 2));
    char comma = ',';
    ss >> this->r >> comma >> this->c;
}
int Position::getRow() const{
    return this->r;
}
int Position::getCol() const {
    return this->c;
}
void Position::setRow(int r){
    this->r = r;
    return;
}
void Position::setCol(int c){
    this->c = c;
    return;
}
string Position::str() const{
    string str_pos = "(" + to_string(this->r) + "," + to_string(this->c) + ")";
    return str_pos;
}
bool Position::isEqual(int in_r, int in_c) const{
    if (this->r == in_r && this->c == in_c) return true;
    return false;
}
bool Position::isEqual(Position pos){
    if (this->r == pos.getRow() && this->c == pos.getCol()) return true;
    return false;
}
const Position npos(-1,-1);

//TASK 3.4: MOVING OBJECT
MovingObject::MovingObject(int index, const Position pos, Map * map, const string & name){
    this->index = index;
    this->pos = pos;
    this->map = map;
    this->name = name;
}
Position MovingObject::getCurrentPosition() const {
    return pos;
}
string MovingObject::getName(){
    return this->name;
}

//TASK 3.45: CHARACTER
Character::Character(int index, int hp, int exp, Map * map, Position pos, string moving_rule):MovingObject(index, pos, map){
    this->hp = setInfor(hp, 500); 
    this->exp = setInfor(exp, 900);
    this->moving_rule = moving_rule;
    num_steps = 0;
}
int Character::getExp(){
    return this->exp;
}
int Character::getHp(){
    return this->hp;
}
void Character::setExp(int in_exp){
    this->exp = setInfor(in_exp, 900);
}
void Character::setHp(int in_hp){
    this->hp = setInfor(in_hp, 500);
}
Position Character::getNextPosition(){
    int nextR = pos.getRow(), nextC = pos.getCol(), x = num_steps % moving_rule.length();
    char step = moving_rule[x];
    if (step == 'L') nextC--;
    if (step == 'R') nextC++;
    if (step == 'U') nextR--;
    if (step == 'D') nextR++;
    num_steps++;
    if (!map->isValid(Position(nextR, nextC), this)) return npos;
    return Position(nextR, nextC);
}
void Character::move(){
    if (this->getExp() == 0) return;
    Position nextPosition = getNextPosition();
    if (!nextPosition.isEqual(npos)){
        pos = nextPosition;
    }
    return;
}

//TASK 3.5: SHERLOCK
Sherlock::Sherlock(int index, const string & moving_rule, const Position & init_pos, Map * map, int init_hp, int init_exp):Character(index, init_hp, init_exp, map, init_pos, moving_rule){
    this->hp = init_hp;
    this->exp = init_exp;
    this->name = "Sherlock";
}
string Sherlock::str() const{
    return "Sherlock[index=" + to_string(index) + ";pos=(" + to_string(this->pos.getRow())
    + "," + to_string(this->pos.getCol()) + ");moving_rule=" + moving_rule +"]";
}
void Sherlock::setPos(Position pos){
    this->pos = pos;
}

//TASK 3.6: WASTON
Watson::Watson(int index, const string & moving_rule, const Position & init_pos, Map * map, int init_hp, int init_exp):Character(index,init_hp, init_exp, map, init_pos, moving_rule){
    this->hp = init_hp;
    this->exp = init_exp;
    this->name = "Watson";
}
string Watson::str() const{
    return "Watson[index=" + to_string(index) + ";pos=(" + to_string(this->pos.getRow())
    + "," + to_string(this->pos.getCol()) + ");moving_rule=" + moving_rule +"]";
}

//TASK 3.7: CRIMINAL
Criminal::Criminal(int index, const Position & init_pos, Map * map, Sherlock * sherlock, Watson * watson):Character(index, 1, 1, map, init_pos, "Criminal"){
    this->sherlock = sherlock;
    this->watson = watson;
    this->name = "Criminal";
}
Position Criminal::getNextPosition(){
    bool flag = false;
    Position next[] = {Position(pos.getRow()-1, pos.getCol()), Position(pos.getRow(), pos.getCol()-1), Position(pos.getRow()+1, pos.getCol()), Position(pos.getRow(), pos.getCol()+1)};
    int max = -1, x = 0;
    for (int i=0; i<4; i++){
        if (map->isValid(next[i], this)){
            flag = true;
            int disS = Distance(next[i], sherlock->getCurrentPosition());
            int disW = Distance(next[i], watson->getCurrentPosition());
            if (max < disS + disW){
                max = disS + disW;
                x = i;
            }
        }
    }
    if (flag) return next[x];
    else return npos;
}
string Criminal::str() const{
    return "Criminal[index=" + to_string(index) + ";pos=" + "(" + to_string(this->pos.getRow())
    + "," + to_string(this->pos.getCol()) + ")]";
}
void Criminal::move(){
    Position nextPosition = getNextPosition();
    if (!nextPosition.isEqual(npos)){
        this->pastPos = pos;
        this->pos = nextPosition;
        num_steps++;
    }
    return;
}
int Criminal::getStepReal(){
    return this->num_steps;
}
Position Criminal::getPastPos(){
    return pastPos;
}

//TASK 3.8: ARRAY MOVING OBJECT
ArrayMovingObject::ArrayMovingObject(int capacity) : count(0){
    this->capacity = capacity;
    arr_mv_objs = new MovingObject*[capacity];
}
ArrayMovingObject::~ArrayMovingObject(){
    delete[] arr_mv_objs;
}
bool ArrayMovingObject::isFull() const{
    return count==capacity;
}
bool ArrayMovingObject::add(MovingObject * mv_obj){
    if (!isFull()){
        arr_mv_objs[count] = mv_obj;
        count++;
        return true;
    }
    return false;
}
MovingObject * ArrayMovingObject::get(int index) const{
    return arr_mv_objs[index];
}
int ArrayMovingObject::size() const{
    return count;
}
string ArrayMovingObject::str() const{
    string ans = "ArrayMovingObject[count=" + to_string(count) + ";capacity=" + to_string(capacity);
    for (int i=0; i<count; i++) {
        ans+= (";" + arr_mv_objs[i]->str());
    }
    return ans + "]";
}

//TASK 3.9: CONFIGURATION
Configuration::Configuration(const string& filepath){
    arr_fake_walls = nullptr;
    arr_walls = nullptr;
    ifstream file(filepath);
    if(!file.is_open()) return;

    string line;
    while (getline(file, line)) {
        if (line.find("=") != string::npos){
            string ans = line.substr(line.find("=") + 1);

        if (line.find("MAP_NUM_ROWS=") != string::npos) {
            map_num_rows = stoi(ans);
        } else if (line.find("MAP_NUM_COLS=") != string::npos) {
            map_num_cols = stoi(ans);
        } else if (line.find("MAX_NUM_MOVING_OBJECTS=") != string::npos) {
            max_num_moving_objects = stoi(ans);
        } else if (line.find("ARRAY_WALLS=") != string::npos) {
            parsePositions(ans, arr_walls, true);
        } else if (line.find("ARRAY_FAKE_WALLS=") != string::npos) {
            parsePositions(ans, arr_fake_walls, false);
        } else if (line.find("SHERLOCK_MOVING_RULE=") != string::npos) {
            sherlock_moving_rule = ans;
        } else if (line.find("SHERLOCK_INIT_POS=") != string::npos) {
            parsePosition(ans, sherlock_init_pos);
        } else if (line.find("SHERLOCK_INIT_HP=") != string::npos) {
            sherlock_init_hp = stoi(ans);
        } else if (line.find("SHERLOCK_INIT_EXP=") != string::npos) {
            sherlock_init_exp = stoi(ans);
        } else if (line.find("WATSON_MOVING_RULE=") != string::npos) {
            watson_moving_rule = ans;
        } else if (line.find("WATSON_INIT_POS=") != string::npos) {
            parsePosition(ans, watson_init_pos);
        } else if (line.find("WATSON_INIT_HP=") != string::npos) {
            watson_init_hp = stoi(ans);
        } else if (line.find("WATSON_INIT_EXP=") != string::npos) {
            watson_init_exp = stoi(ans);
        } else if (line.find("CRIMINAL_INIT_POS=") != string::npos) {
            parsePosition(ans, criminal_init_pos);
        } else if (line.find("NUM_STEPS=") != string::npos) {
            num_steps = stoi(ans);
        }
    }}
   file.close();
}
Configuration::~Configuration() {
    if (arr_fake_walls != nullptr) delete[] arr_fake_walls;
    if (arr_walls != nullptr) delete[] arr_walls;
    }
string Configuration::str() const {
        string result = "Configuration[\n";
        result += "MAP_NUM_ROWS=" + to_string(map_num_rows) + "\n";
        result += "MAP_NUM_COLS=" + to_string(map_num_cols) + "\n";
        result += "MAX_NUM_MOVING_OBJECTS=" + to_string(max_num_moving_objects) + "\n";
        result += "NUM_WALLS=" + to_string(num_walls) + "\n";
        result += "ARRAY_WALLS=[";
        for (int i = 0; i < num_walls; ++i) {
            result += "(" + to_string(arr_walls[i].getRow()) + "," + to_string(arr_walls[i].getCol()) + ")";
            if (i < num_walls - 1) result += ";";
        }
        result += "]\n";
        result += "NUM_FAKE_WALLS=" + to_string(num_fake_walls) + "\n";
        result += "ARRAY_FAKE_WALLS=[";
        for (int i = 0; i < num_fake_walls; ++i) {
            result += "(" + to_string(arr_fake_walls[i].getRow()) + "," + to_string(arr_fake_walls[i].getCol()) + ")";
            if (i < num_fake_walls - 1) result += ";";
        }
        result += "]\n";
        result += "SHERLOCK_MOVING_RULE=" + sherlock_moving_rule + "\n";
        result += "SHERLOCK_INIT_POS=(" + to_string(sherlock_init_pos.getRow()) + "," + to_string(sherlock_init_pos.getCol()) + ")\n";
        result += "SHERLOCK_INIT_HP=" + to_string(sherlock_init_hp) + "\n";
        result += "SHERLOCK_INIT_EXP=" + to_string(sherlock_init_exp) + "\n";
        result += "WATSON_MOVING_RULE=" + watson_moving_rule + "\n";
        result += "WATSON_INIT_POS=(" + to_string(watson_init_pos.getRow()) + "," + to_string(watson_init_pos.getCol()) + ")\n";
        result += "WATSON_INIT_HP=" + to_string(watson_init_hp) + "\n";
        result += "WATSON_INIT_EXP=" + to_string(watson_init_exp) + "\n";
        result += "CRIMINAL_INIT_POS=(" + to_string(criminal_init_pos.getRow()) + "," + to_string(criminal_init_pos.getCol()) + ")\n";
        result += "NUM_STEPS=" + to_string(num_steps) + "\n";
        result += "]";
        return result;
    }
void Configuration::parsePositions(const string& input, Position*& positions, bool isNumWall){
    int cnt = 0;
    for (int i=0; i<input.length(); i++){
        if (input[i] == ',') cnt++; 
    }
    int num_positions = cnt;
    if (isNumWall) num_walls = num_positions;
    else num_fake_walls = num_positions;
    if (cnt == 0) return;

    positions = new Position[num_positions];
    int start = input.find("[") + 1;
    int end = input.find("]");
    string substring = input.substr(start, end - start);
    int pos = 0;
    for (int i=0; i<num_positions; i++){
        pos = substring.find(";");
        string pair = substring.substr(0, pos);
        int comma_pos = pair.find(",");
        int row = stoi(pair.substr(1, comma_pos - 1));
        int col = stoi(pair.substr(comma_pos + 1, pair.length() - comma_pos - 2));
        positions[i] = Position(row,col);
        substring.erase(0,pos+1);
    }
    //the last pair
    int comma_pos = substring.find(",");
    int row = stoi(substring.substr(1, comma_pos - 1));
    int col = stoi(substring.substr(comma_pos + 1, substring.length() - comma_pos - 2));    
    positions[num_positions - 1] = Position(row,col);
    
    return;
}
void Configuration::parsePosition(const string& input, Position& position){
    int start = input.find("(") + 1;
    int comma_pos = input.find(",");
    position.setRow(stoi(input.substr(start, comma_pos - start)));
    int end = input.find(")");
    position.setCol(stoi(input.substr(comma_pos + 1, end - comma_pos - 1)));
}

//TASK 3.10: ROBOT
Robot::Robot(int index, const Position pos, Map *map, Criminal* criminal):MovingObject(index, pos, map){
    this->criminal = criminal;
    this->name = "Robot";

    int s = mainNumber(pos.getRow(), pos.getCol());
    int t = (pos.getRow()*11 + pos.getCol())%4;
    if (s >= 0 && s <= 1) {
        item = new MagicBook();
    } else if (s >= 2 && s <= 3) {
        item = new EnergyDrink();
    } else if (s >= 4 && s <= 5) {
        item = new FirstAid();
    } else if (s >= 6 && s <= 7) {
        item = new ExcemptionCard();
    } else if (s >= 8 && s <= 9) {
        item = new PassingCard(t);
    } else {item = nullptr;}
}
Robot::~Robot(){
    if (item != nullptr) delete item;
}
void Robot::move(){
    Position nextPosition = getNextPosition();
    if (!nextPosition.isEqual(npos)){
        pos = nextPosition;
    }
    return;
}
BaseItem* Robot::getItem(){
    return item;
}
RobotType Robot::getType(){
    return this->robot_type;
}

//robotC
RobotC::RobotC(int index, const Position & init_pos, Map *map, Criminal *criminal):Robot(index, init_pos, map, criminal){
    this->robot_type = C;
}
Position RobotC::getNextPosition(){
    return criminal->getPastPos();
}
int RobotC::getDistance(Character *character) const{
    return Distance(this->pos, character->getCurrentPosition());
}
string RobotC::str() const{
    return "Robot[pos=" + pos.str() + ";type=C;dist=]"; 
}

//robotS
RobotS::RobotS(int index, const Position & init_pos, Map *map, Criminal *criminal, Sherlock *sherlock):Robot(index, init_pos, map, criminal){
    this->sherlock = sherlock;
    this->robot_type = S;
}
Position RobotS::getNextPosition(){
    bool flag = false;
    Position next[] = {Position(pos.getRow()-1, pos.getCol()), Position(pos.getRow(), pos.getCol()+1), Position(pos.getRow()+1, pos.getCol()), Position(pos.getRow(), pos.getCol()-1)};
    int min = SHRT_MAX, x = 0;
    for (int i=0; i<4; i++){
        if (map->isValid(next[i], this)){
            flag = true;
            int dis = Distance(next[i], sherlock->getCurrentPosition());
            if (min > dis){
                min = dis;
                x = i;
            }
        }
    }
    if (flag) return next[x];
    else return npos;
}
int RobotS::getDistance() const{
    return Distance(this->pos, sherlock->getCurrentPosition());
}
string RobotS::str() const {
    return "Robot[pos=" + pos.str() + ";type=S;dist=" + to_string(getDistance()) + "]";
}

//robotW
RobotW::RobotW(int index, const Position & init_pos, Map *map, Criminal *criminal, Watson *watson):Robot(index, init_pos, map, criminal){
    this->watson = watson;
    this->robot_type = W;
}
Position RobotW::getNextPosition(){
    bool flag = false;
    Position next[] = {Position(pos.getRow()-1, pos.getCol()), Position(pos.getRow(), pos.getCol()+1), Position(pos.getRow()+1, pos.getCol()), Position(pos.getRow(), pos.getCol()-1)};
    int min = SHRT_MAX, x = 0;
    for (int i=0; i<4; i++){
        if (map->isValid(next[i], this)){
            flag = true;
            int dis = Distance(next[i], watson->getCurrentPosition());
            if (min > dis){
                min = dis;
                x = i;
            }
        }
    }
    if (flag) return next[x];
    else return npos;
}
int RobotW::getDistance() const{
    return Distance(this->pos, watson->getCurrentPosition());
}
string RobotW::str() const{
    return "Robot[pos=" + pos.str() + ";type=W;dist=" + to_string(getDistance()) + "]";
}

//robotSW
RobotSW::RobotSW(int index, const Position & init_pos, Map *map, Criminal *criminal, Sherlock *sherlock, Watson *watson):Robot(index, init_pos, map, criminal){
    this->sherlock = sherlock;
    this->watson = watson;
    this->robot_type = SW;
}
Position RobotSW::getNextPosition(){
    bool flag = false;
    Position next[] = {Position(pos.getRow()-2,pos.getCol()),Position(pos.getRow()-1,pos.getCol()+1),Position(pos.getRow(),pos.getCol()+2),Position(pos.getRow()+1,pos.getCol()+1),
                       Position(pos.getRow()+2,pos.getCol()),Position(pos.getRow()+1,pos.getCol()-1),Position(pos.getRow(),pos.getCol()-2),Position(pos.getRow()-1,pos.getCol()-1)};
    int min = SHRT_MAX, x = 0;
    for (int i=0; i<8; i++){
        if (map->isValid(next[i], this)){
            flag = true;
            int dis = Distance(next[i], sherlock->getCurrentPosition()) + Distance(next[i], watson->getCurrentPosition());
            if (min > dis){
                min = dis;
                x = i;
            }
        }
    }
    if (flag) return next[x];
    else return npos;
}
int RobotSW::getDistance() const{
    return Distance(this->pos, sherlock->getCurrentPosition()) + Distance(this->pos, watson->getCurrentPosition());
}
string RobotSW::str() const{
     return "Robot[pos=" + pos.str() + ";type=SW;dist=" + to_string(getDistance()) + "]";
}

//BaseItem
BaseItem::BaseItem(ItemType in_itemType){
    this->itemType = in_itemType;
}
ItemType BaseItem::getItemType(){
    return itemType;
}

//MagicBook
bool MagicBook::canUse(Character* obj, Robot* robot){
    if (obj->getName() != "Sherlock" && obj->getName() != "Watson") return false;
    if (obj->getExp() > 350) return false;
    if (robot != nullptr) return false;
    return true;
}
void MagicBook::use(Character* obj, Robot* robot){
    obj->setExp(ceil(obj->getExp()*1.25));
}
string MagicBook::str() const{
    return "MagicBook";
}

//EnergyDrink
bool EnergyDrink::canUse(Character* obj, Robot* robot){
    if (obj->getName() != "Sherlock" && obj->getName() != "Watson") return false;
    if (obj->getHp() > 100) return false;
    if (robot != nullptr) return false;
    return true;
}
void EnergyDrink::use(Character* obj, Robot* robot){
    obj->setHp(ceil(obj->getHp()*1.2));
}
string EnergyDrink::str() const{
    return "EnergyDrink";
}

//FirstAid
bool FirstAid::canUse(Character* obj, Robot* robot){
    if (obj->getName() != "Sherlock" && obj->getName() != "Watson") return false;
    if (obj->getHp() > 100 && obj->getExp() > 350) return false;
    if (robot != nullptr) return false;
    return true;
}
void FirstAid::use(Character* obj, Robot* robot){
    obj->setHp(ceil(obj->getHp()*1.5));
}
string FirstAid::str() const{
    return "FirstAid";
}

//ExcemptionCard
bool ExcemptionCard::canUse(Character* obj, Robot* robot){
    if (obj->getName() != "Sherlock") return false;
    if (obj->getHp() %2 == 0) return false;
    return true;
}
void ExcemptionCard::use(Character* obj, Robot* robot){}
string ExcemptionCard::str() const{
    return "ExcemptionCard";
}

//PassingCard
PassingCard::PassingCard(int t): BaseItem(PASSING_CARD){
    switch (t) {
        case 0:
            challenge = "RobotS";
            break;
        case 1:
            challenge = "RobotC";
            break;
        case 2:
            challenge = "RobotSW";
            break;
        case 3:
            challenge = "all";
            break;
    }
}
bool PassingCard::canUse(Character* obj, Robot* robot){
    if (obj->getName() != "Watson") return false;
    if (obj->getHp() %2 == 1) return false;
    return true;
}
void PassingCard::use(Character* obj, Robot* robot){
    if (challenge == "RobotS" && robot->getType() == S) return;
    else if (challenge == "RobotC" && robot->getType() == C) return;
    else if (challenge == "RobotSW" && robot->getType() == SW) return;
    else if (challenge == "all") return;
    else {
        obj->setExp(ceil(obj->getExp() - 50));
        return;
    }
}
string PassingCard::str() const{
    return "PassingCard";
}

//BASEBAG
BaseBag::BaseBag(int capacity):capacity(capacity), head(nullptr){
    size = 0;
}
BaseBag::~BaseBag(){
    while (head != nullptr){
        Node* temp = head;
        head = head->next;
        delete temp;
    }
}
void BaseBag::swap(Node* a, Node* b){
    BaseItem* temp = a->item;
    a->item = b->item;
    b->item = temp;
}
bool BaseBag::insert(BaseItem* item){
    if (size == capacity) return false;
    Node* temp = new Node(item);
    if (size == 0) head = temp; 
    else {  temp->next = head;
            head = temp;
    }
    size++;
    return true;
}
bool BaseBag:: isFull(){
    return size==capacity;
}
bool BaseBag::checkItem(ItemType item){
    Node* temp = head;
    while (temp != nullptr){
        if (temp->item->getItemType() == item) return true;  
        temp = temp->next;
    }
    return false;
}
string BaseBag::str() const{
    string result = "Bag[count=" + to_string(size) + ";";
    if (size == 0) return result += "]";
    Node* temp = head;
    for (int i = 0; i < size; ++i){
        result += temp->item->str();
        temp = temp->next;
        if (temp == nullptr) break;
        result += ",";
    }
    result += "]";
    return result;
}

//SHERLOCKBAG
SherlockBag::SherlockBag(Sherlock* sherlock)  : BaseBag(13) {
    this->sherlock = sherlock;
}
BaseItem* SherlockBag::get(){
    if (size == 0) return nullptr;
    Node* temp = head;
    for (int i = 0; i < size; ++i) {
        if (temp->item->canUse(sherlock, nullptr) && temp->item->getItemType() != EXCEMPTION_CARD && temp->item->getItemType() != PASSING_CARD){
            swap(head, temp);
            BaseItem* result = head->item;
            Node* oldNode = head;
            head = head->next;
            delete oldNode;
            size--;
            return result;
        }
        temp = temp->next;
    }
    return nullptr;
}
BaseItem* SherlockBag::get(ItemType itemType){
    if (size == 0) return nullptr;
    Node* temp = head;
    for (int i =0; i <size; ++i) {
        if (temp->item->getItemType() == itemType){
            swap(head, temp);
            BaseItem* result = head->item;
            Node* oldNode = head;
            head = head->next;
            delete oldNode;
            size--;
            return result;
        }
        temp = temp->next;
    }
    return nullptr;
}

//WATSONBAG
WatsonBag::WatsonBag(Watson* watson) : BaseBag(15) {
    this->watson = watson;
}
BaseItem* WatsonBag::get(){
    if (size == 0) return nullptr;
    Node* temp = head;
    for (int i = 0; i < size; ++i) {
        if (temp->item != nullptr) {
            if (temp->item->canUse(watson, nullptr) && temp->item->getItemType() != EXCEMPTION_CARD && temp->item->getItemType() != PASSING_CARD){
                swap(head, temp);
                BaseItem* result = head->item;
                Node* oldNode = head;
                head = head->next;
                delete oldNode;
                size--;
                return result;
            }
            temp = temp->next;
        }
        
    }
    return nullptr;
}
BaseItem* WatsonBag::get(ItemType itemType){
    if (size == 0) return nullptr;
    Node* temp = head;
    for (int i =0; i <size; ++i) {
        if (temp->item->getItemType() == itemType){
            swap(head, temp);
            BaseItem* result = head->item;
            Node* oldNode = head;
            head = head->next;
            delete oldNode;
            size--;
            return result;
        }
        temp = temp->next;
    }
    return nullptr;
}

///TASK3.13: STUDYINPINKPROGRAM
StudyPinkProgram::StudyPinkProgram(const string& config_file_path){
    this->config = new Configuration(config_file_path);
    this->map = new Map(config->map_num_rows, config->map_num_cols, config->num_walls, config->arr_walls, config->num_fake_walls, config->arr_fake_walls);
    this->arr_mv_objs = new ArrayMovingObject(config->max_num_moving_objects);
    this->sherlock = new Sherlock(1, config->sherlock_moving_rule, config->sherlock_init_pos, map, config->sherlock_init_hp, config->sherlock_init_exp);
    this->watson = new Watson(2, config->watson_moving_rule, config->watson_init_pos,map, config->watson_init_hp, config->watson_init_exp);
    this->criminal = new Criminal(0, config->criminal_init_pos, map, sherlock, watson);
    arr_mv_objs->add(criminal);
    arr_mv_objs->add(sherlock);
    arr_mv_objs->add(watson);

    sherlockBag = new SherlockBag(sherlock);
    watsonBag = new WatsonBag(watson);
}
StudyPinkProgram::~StudyPinkProgram(){
    delete sherlockBag;
    delete watsonBag;
    delete config;
    delete map;
    delete sherlock;
    delete watson;
    delete criminal;
    delete arr_mv_objs;
}
bool StudyPinkProgram::isStop() const{
    if (sherlock->getHp() == 1 || watson->getHp() == 1) return true;
    if (sherlock->getCurrentPosition().isEqual(criminal->getCurrentPosition()) || watson->getCurrentPosition().isEqual(criminal->getCurrentPosition())) return true;
    return false;
}
void StudyPinkProgram::run(bool verbose){
    if (isStop()) {
            printStep(0);
            printResult();
            return;
        }
    for (int istep = 0; istep < config->num_steps; ++istep) {
        bool end = false;
        for (int i = 0; i < arr_mv_objs->size(); ++i) {
            arr_mv_objs->get(i)->move();

//1. CRIMINAL - có gặp Sherlock/Watson hay không
//            - check xem có tạo robot không. có ->hàm create
            if (arr_mv_objs->get(i)->getName() == "Criminal"){
                // GẶP SHERLOCK/WATSON
                if (criminal->getCurrentPosition().isEqual(sherlock->getCurrentPosition()) || criminal->getCurrentPosition().isEqual(watson->getCurrentPosition())){
                    printStep(istep);
                    printResult();
                    return;
                }
                //TẠO ROBOT
                if (criminal->getStepReal() == 3){
                    arr_mv_objs->add(new RobotC(3, criminal->getPastPos(), map, criminal));
                }
                if (criminal->getStepReal()%3 == 0 && criminal->getStepReal() > 3){
                    int disS = Distance(criminal->getPastPos(), sherlock->getCurrentPosition());
                    int disW = Distance(criminal->getPastPos(), watson->getCurrentPosition());
                    if (disS < disW){
                        arr_mv_objs->add(new RobotS(arr_mv_objs->size(), criminal->getPastPos(), map, criminal, sherlock));
                    } else if (disS > disW){
                        arr_mv_objs->add(new RobotW(arr_mv_objs->size(), criminal->getPastPos(), map, criminal, watson));
                    } else {
                        arr_mv_objs->add(new RobotSW(arr_mv_objs->size(), criminal->getPastPos(), map, criminal, sherlock, watson));
                    }
                }
            }
//2. SHERLOCK - có gặp robot hay không -> xài thẻ + oánh robot + xài buff
//            - có gặp robotC hay không -> thắng -> bắt được tội phạm. thua -> k băt được nhưng loot được đồ
//            - có gặp Watson hay không -> trao đổi thẻ
//            - có gặp Criminal hay không -> endGame
            if (arr_mv_objs->get(i)->getName() == "Sherlock"){
                for (int j = 0; j < arr_mv_objs->size(); ++j){
                    if (sherlock->getCurrentPosition().isEqual(arr_mv_objs->get(j)->getCurrentPosition()) && arr_mv_objs->get(j)->getName() != "Sherlock"){
                        //GẶP CRIMINAL
                        if (arr_mv_objs->get(j)->getName() == "Criminal"){
                            end = true;
                            break;
                        }

                        //GẶP ROBOT
                        if (arr_mv_objs->get(j)->getName() == "Robot"){
                            Robot* robot = dynamic_cast<Robot*>(arr_mv_objs->get(j));
                            BaseItem* item = nullptr;
                            if (sherlockBag->checkItem(EXCEMPTION_CARD) && sherlock->getHp()%2 == 1) item = sherlockBag->get(EXCEMPTION_CARD);
                            if (robot->getType() == S){
                                if (sherlock->getExp() > 400) sherlockBag->insert(robot->getItem());
                                else if (item == nullptr) sherlock->setExp(ceil(sherlock->getExp()*0.9));
                            }
                            else if (robot->getType() == W) sherlockBag->insert(robot->getItem());
                            else if (robot->getType() == SW){
                                if (sherlock->getExp() > 300 && sherlock->getHp() > 335) sherlockBag->insert(robot->getItem());
                                else if (item == nullptr) {
                                        sherlock->setHp(ceil(sherlock->getHp()*0.85));
                                        sherlock->setExp(ceil(sherlock->getExp()*0.85));
                                    }
                            }
                            else if (robot->getType() == C) {
                                if (sherlock->getExp() > 500){
                                    sherlock->setPos(criminal->getCurrentPosition());
                                    end = true;
                                    break;
                                }
                                else sherlockBag->insert(robot->getItem());
                            }
                            //BUFF
                            BaseItem* itemBuff = sherlockBag->get();
                            if (itemBuff != nullptr) itemBuff->use(sherlock, nullptr);
                        }

                        //GẶP WATSON
                        if (arr_mv_objs->get(j)->getName() == "Watson"){
                            if (sherlockBag->checkItem(PASSING_CARD) && watsonBag->checkItem(EXCEMPTION_CARD)){
                                while (sherlockBag->checkItem(PASSING_CARD) && !watsonBag->isFull()) watsonBag->insert(sherlockBag->get(PASSING_CARD));
                                while (watsonBag->checkItem(EXCEMPTION_CARD) && !sherlockBag->isFull()) sherlockBag->insert(watsonBag->get(EXCEMPTION_CARD));
                            }
                        }
                    }
                }
            }

//3.WASTON - có gặp robot hay không -> xài thẻ + oánh robot + xài buff
//            - có gặp robotC hay không -> criminal bị giữ chân + loot đồ từ robot
//            - có gặp Sherlock hay không -> đổi thẻ
//            - có gặp Criminal hay không -> endGame
            if (arr_mv_objs->get(i)->getName() == "Watson"){
                for (int j = 0; j < arr_mv_objs->size(); ++j){
                    if (watson->getCurrentPosition().isEqual(arr_mv_objs->get(j)->getCurrentPosition()) && arr_mv_objs->get(j)->getName() != "Watson"){
                        //GẶP CRIMINAL
                        if (arr_mv_objs->get(j)->getName() == "Criminal"){
                            end = true;
                            break;
                        }

                        //GẶP ROBOT
                        if (arr_mv_objs->get(j)->getName() == "Robot"){
                            Robot* robot = dynamic_cast<Robot*>(arr_mv_objs->get(j));
                            BaseItem* item = nullptr;
                            if (watsonBag->checkItem(PASSING_CARD) && watson->getHp()%2 == 0) item = watsonBag->get(PASSING_CARD);
                            if (robot->getType() == S){
                                if (item != nullptr) {
                                    item->use(watson, robot); 
                                }
                            }
                            else if (robot->getType() == W){
                                if (item != nullptr) {
                                    item->use(watson, robot); 
                                    watsonBag->insert(robot->getItem());
                                }
                                else if (watson->getHp() > 350) watsonBag->insert(robot->getItem());
                                else {watson->setHp(ceil(watson->getHp()*0.95));}
                            }
                            else if (robot->getType() == SW){
                                if (item != nullptr) {
                                    item->use(watson, robot); 
                                    watsonBag->insert(robot->getItem());
                                }
                                else if (watson->getExp() > 600 && watson->getHp() > 165) watsonBag->insert(robot->getItem());
                                else {
                                    watson->setHp(ceil(watson->getHp()*0.85));
                                    watson->setExp(ceil(watson->getExp()*0.85));
                                }
                            }
                            else if (robot->getType() == C){
                                if (item != nullptr) {
                                    item->use(watson, robot); 
                                }
                                watsonBag->insert(robot->getItem());
                            }
                            //BUFF
                            BaseItem* itemBuff = watsonBag->get();
                            if (itemBuff != nullptr) itemBuff->use(watson, nullptr);
                        }

                        //GẶP SHERLOCK
                        if (arr_mv_objs->get(j)->getName() == "Sherlock"){
                            if (sherlockBag->checkItem(PASSING_CARD) && watsonBag->checkItem(EXCEMPTION_CARD)){
                                while (sherlockBag->checkItem(PASSING_CARD) && !watsonBag->isFull()) watsonBag->insert(sherlockBag->get(PASSING_CARD));
                                while (watsonBag->checkItem(EXCEMPTION_CARD) && !sherlockBag->isFull()) sherlockBag->insert(watsonBag->get(EXCEMPTION_CARD));
                            }
                        }
                    }
                }
            }         
//4. Các robot - có gặp sherlock/watson hay không, có -> như trên
            if (arr_mv_objs->get(i)->getName() == "Robot"){
                Robot* robot = dynamic_cast<Robot*>(arr_mv_objs->get(i));
                for (int j = 0; j < arr_mv_objs->size(); ++j){
                    //ROBOT GẶP SHERLOCK
                    if (robot->getCurrentPosition().isEqual(arr_mv_objs->get(j)->getCurrentPosition()) && arr_mv_objs->get(j)->getName() == "Sherlock"){
                        BaseItem* item = nullptr;
                        if (sherlockBag->checkItem(EXCEMPTION_CARD) && sherlock->getHp()%2 == 1) item = sherlockBag->get(EXCEMPTION_CARD);
                        if (robot->getType() == S){
                            if (sherlock->getExp() > 400) sherlockBag->insert(robot->getItem());
                            else if (item == nullptr) sherlock->setExp(ceil(sherlock->getExp()*0.9));
                        }
                        else if (robot->getType() == W) sherlockBag->insert(robot->getItem());
                        else if (robot->getType() == SW){
                            if (sherlock->getExp() > 300 && sherlock->getHp() > 335) sherlockBag->insert(robot->getItem());
                            else if (item == nullptr) {
                                    sherlock->setHp(ceil(sherlock->getHp()*0.85));
                                    sherlock->setExp(ceil(sherlock->getExp()*0.85));
                                }
                        }
                        else if (robot->getType() == C) {
                            if (sherlock->getExp() > 500){
                                sherlock->setPos(criminal->getCurrentPosition());
                                end = true;
                                break;
                            }
                            else sherlockBag->insert(robot->getItem());
                        }
                        //BUFF
                        BaseItem* itemBuff = sherlockBag->get();
                        if (itemBuff != nullptr) itemBuff->use(sherlock, nullptr);
                    }
                    //ROBOT GẶP WATSON
                    if (robot->getCurrentPosition().isEqual(arr_mv_objs->get(j)->getCurrentPosition()) && arr_mv_objs->get(j)->getName() == "Watson"){
                        BaseItem* item = nullptr;
                        if (watsonBag->checkItem(PASSING_CARD) && watson->getHp()%2 == 0) item = watsonBag->get(PASSING_CARD);
                            if (robot->getType() == S){
                                if (item != nullptr) {
                                    item->use(watson, robot); 
                                }
                            }
                            else if (robot->getType() == W){
                                if (item != nullptr) {
                                    item->use(watson, robot); 
                                    watsonBag->insert(robot->getItem());
                                }
                                else if (watson->getHp() > 350) watsonBag->insert(robot->getItem());
                                else {watson->setHp(ceil(watson->getHp()*0.95));}
                            }
                            else if (robot->getType() == SW){
                                if (item != nullptr) {
                                    item->use(watson, robot); 
                                    watsonBag->insert(robot->getItem());
                                }
                                else if (watson->getExp() > 600 && watson->getHp() > 165) watsonBag->insert(robot->getItem());
                                else {
                                    watson->setHp(ceil(watson->getHp()*0.85));
                                    watson->setExp(ceil(watson->getExp()*0.85));
                                }
                            }
                            else if (robot->getType() == C){
                                if (item != nullptr) {
                                    item->use(watson, robot); 
                                }
                                watsonBag->insert(robot->getItem());
                            } 
                            //BUFF
                            BaseItem* itemBuff = watsonBag->get();
                            if (itemBuff != nullptr) itemBuff->use(watson, nullptr);
                    }
                }                
            }
            if (isStop()) {
                printStep(istep);
                end = true;
                break;
            }
            if (verbose) printStep(istep);
        }
    if (end) break;
    }
    printResult();    
}
////////////////////////////////////////////////
int Distance(Position pos1, Position pos2){
    return abs(pos1.getCol() - pos2.getCol()) + abs(pos1.getRow() - pos2.getRow());
}
int sumOfNumber(int a){
    if (a<9) return a;
    int s = 0;
    while (a>0){
        s += a%10;  a /= 10;
    }
    return s;
}
int mainNumber(int i, int j){
    int p = i*j;
    while (p > 9) p = sumOfNumber(p);
    return p; 
}
int setInfor(int x, int max){
    if (x < 0) return 0;
    else if (x > max) return max;
    else return x;
}
////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////