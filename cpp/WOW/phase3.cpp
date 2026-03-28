#include <iomanip>
#include <iostream>
#include <string>
#include <array>
#include <algorithm>
#include <vector>

#define LEFT -1
#define RIGHT 1

enum class WeaponType {
  Sword = 0, 
  Bomb = 1, 
  Arrow = 2
};

enum class WarriorType {
  Dragon = 0,
  Ninja,
  Iceman,
  Lion,
  Wolf,
  COUNT
};

class Warrior;
class Weapon;
class Headquarter;
class City;

const int N_Warrior_Kinds = 5;
std::array<int, (int)(WarriorType::COUNT)> Warrior_HP;
static int _time_;


// ******************* //
//  Class Declarition  //
// ******************* //

// WEAPON

class Weapon {
  protected:
    int durability;

  public:
    Weapon(int dur) : durability(dur) {}

    virtual WeaponType get_type() = 0;
    virtual int cal_cur_dmg(int warrior_dmg) = 0;
    virtual int cal_self_dmg(int warrior_dmg) = 0;
    int be_used() {
      durability--;
      return durability;
    }
    int is_usable() {
      return durability > 0;
    }
};

class Sword: public Weapon {
  public:
    Sword() : Weapon(INT_MAX) {}
    virtual WeaponType get_type() override {
      return WeaponType::Sword;
    }
    virtual int cal_cur_dmg(int warrior_dmg) override {
      return warrior_dmg * 2 / 10;
    }
    virtual int cal_self_dmg(int warrior_dmg) override {
      return 0;
    }
};

class Bomb: public Weapon {
  public:
    Bomb() : Weapon(1) {}
    virtual WeaponType get_type() override {
      return WeaponType::Bomb;
    }
    virtual int cal_cur_dmg(int warrior_dmg) override {
      return warrior_dmg * 4 / 10;
    }
    virtual int cal_self_dmg(int warrior_dmg) override {
      return cal_cur_dmg(warrior_dmg) / 2;
    }
};

class Arrow: public Weapon {
  public:
    Arrow() : Weapon(2) {}
    virtual WeaponType get_type() override {
      return WeaponType::Arrow;
    }
    virtual int cal_cur_dmg(int warrior_dmg) override {
      return warrior_dmg * 3 / 10;
    }
    virtual int cal_self_dmg(int warrior_dmg) override {
      return 0;
    }
};

bool weapon_cmp(Weapon* a, Weapon* b) {
  // TODO
  return a->get_type() < b->get_type();
}

bool weapon_rob_cmp(Weapon* a, Weapon* b) {
  // TODO
  return a->get_type() < b->get_type();
}

Weapon* create_weapon(WeaponType type) {
  if (type == WeaponType::Sword) {
    return new Sword();
  } else if (type == WeaponType::Bomb) {
    return new Bomb();
  } else if (type == WeaponType::Arrow) {
    return new Arrow();
  }
}

WeaponType weapid2type(int id) {
  return (WeaponType)(id);
}

// WARRIOR

class Warrior {
  protected:
    WarriorType kind;
    int id;
    int hp;
    int dmg;
    int dir; // has to be either 1 or -1
    City* location; // init: City 0 or City n+1
    std::vector<Weapon*> weapon_list;

  public:
    int cur_weap_index = 0;
    Warrior(WarriorType k, int i, int d) : kind(k), id(i), dir(d){
      hp = Warrior_HP[(int)kind];
    }
    virtual ~Warrior() {
      for (auto p: weapon_list) {
        delete p;
      }
    }

    friend class GameWorld;

    WarriorType get_type() const { return kind; }
    int get_camp() const { return dir; }
    virtual void print_spawn();

    void sort_weapon() {
      sort(weapon_list.begin(), weapon_list.end(), weapon_cmp);
    }
    void sort_rob_weapon() {
      sort(weapon_list.begin(), weapon_list.end(), weapon_rob_cmp);
    }

    virtual bool escape() {}
    
    virtual void attack(Warrior* enemy) {
      Weapon* weap = next_weapon();
      if (weap == nullptr) return;

      int weap_dmg = weap->cal_cur_dmg(dmg);
      enemy->hurt(weap_dmg);
      if (weap->get_type() == WeaponType::Bomb && kind != WarriorType::Ninja) {
        hurt(weap->cal_self_dmg(dmg));
      }
      // check weapon
      if (weap->is_usable()) {
        weap->be_used();
        cur_weap_index++;
      } else {
        remove_weapon();
      }
    }
    void hurt(int hurt_dmg) {
      hp -= hurt_dmg;
    }
    bool is_alive() {
      return hp > 0;
    }
    bool has_weapon() {
      return !weapon_list.empty();
    }
    bool unchanged_even() {
      for (auto p: weapon_list) 
        if (p->get_type() == WeaponType::Arrow || p->get_type() == WeaponType::Bomb) 
          return false;
      Weapon* weap = weapon_list.front();
      return weap->cal_cur_dmg(dmg) == 0;
    }

    Weapon* next_weapon() {
      if (weapon_list.empty()) return nullptr;
      cur_weap_index %= weapon_list.size();
      return weapon_list[cur_weap_index];
    }
    void remove_weapon() {
      if (weapon_list.empty()) return;
      delete weapon_list[cur_weap_index];
      weapon_list.erase(weapon_list.begin() + cur_weap_index);
      if (cur_weap_index >= weapon_list.size()) cur_weap_index = 0;
    }
    void rob_weapon(Warrior* enemy) {
      enemy->cur_weap_index = 0;
      enemy->sort_rob_weapon();
      if (!enemy->has_weapon()) return;
      weapon_list.push_back(enemy->weapon_list.front());
      enemy->weapon_list.erase(enemy->weapon_list.begin());
    }


    virtual void forward(City* nxt_city) {
      if (location) location->leave(this);
      location = nxt_city;
      nxt_city->enter(this);
    }
};

class Dragon : public Warrior {
  private:
    double morale;

  public:
    Dragon(int i, int d, Weapon* weap, int hp_origin) : Warrior(WarriorType::Dragon, i, d) {
      morale = (hp_origin * 1.0) / (Warrior_HP[(int)kind] * 1.0);
      weapon_list.push_back(weap);
    }
    
    void print_yell();
    virtual void print_spawn() override;
};

class Ninja : public Warrior {
  public:
    Ninja(int i, int d, Weapon* weap1, Weapon* weap2) : Warrior(WarriorType::Ninja, i, d) {
      weapon_list.push_back(weap1);
      weapon_list.push_back(weap2);
    }
    virtual void print_spawn() override;
};

class Iceman : public Warrior {
  public:
    Iceman(int i, int d, Weapon* weap) : Warrior(WarriorType::Iceman, i, d) {
      weapon_list.push_back(weap);
    }
    virtual void print_spawn() override;
    virtual void forward(City* nxt_city) override {
      Warrior::forward(nxt_city);
      int hp_dec = hp / 10;
      hp -= hp_dec;
    }
};

class Lion : public Warrior {
  private:
    int loyalty;
    int loyalty_dec;

  public:
    Lion(int i, int d, Weapon* weap, int hp_origin, int k) 
      : Warrior(WarriorType::Lion, i, d), loyalty(hp_origin), loyalty_dec(k) {
      weapon_list.push_back(weap);
    }
    virtual void print_spawn() override;
    virtual void forward(City* nxt_city) override {
      Warrior::forward(nxt_city);
      loyalty -= loyalty;
    }
    virtual bool escape() override {
      if (loyalty <= 0) {
        hp = -1;
        // TODO
      }
      return hp <= 0;
    }
};

class Wolf : public Warrior {
  public:
    Wolf(int i, int d) : Warrior(WarriorType::Wolf, i, d){}
    virtual void print_spawn() override;
};

// BATTLE

class Battle {
  private:
    Warrior* first;
    Warrior* second;
    City* location;
  public:
    Battle(Warrior* r, Warrior* b, City* loc) : location(loc) {
      if (location->get_id() % 2 != 0) {
        Warrior* first = r;
        Warrior* second = b;
      } else {
        Warrior* first = b;
        Warrior* second = r;
      }
    }

    void fight() {
      first->sort_weapon();
      second->sort_weapon();
      first->cur_weap_index = second->cur_weap_index = 0;

      while (1) {
        if (!first->has_weapon() && !second->has_weapon()) break;
        if (first->unchanged_even() && second->unchanged_even()) break;
        first->attack(second);
        if (!first->is_alive() || !second->is_alive()) break;
        second->attack(first);
        if (!first->is_alive() || !second->is_alive()) break;
      }
    }

    void report(int cond);

    void eval() {
      if (first->is_alive() && second->is_alive()) {
        report(1);
        return;
      }

      if (!first->is_alive() && !second->is_alive()) {
        location->leave(first);
        location->leave(second);
        report(2);
        return;
      }

      // first is alive
      if (second->is_alive()) {
        Warrior* tmp = second;
        second = first;
        first = tmp;
      }
      location->leave(second);
      for (int i = 0; i < 10; i++) {
        first->rob_weapon(second);
      }
      report(3);
    }
};

// GAMEMAP

class City{
  private:
    int id;
    Warrior* red;
    Warrior* blue;
    City* lft;
    City* rgt;
  public:
    City() {}
    City(int i, City* l, City* r): id(i), lft(l), rgt(r), red(nullptr), blue(nullptr) {}
    friend class GameWorld;

    int get_id() { return id; }
    Warrior* get_red() { return red; }
    Warrior* get_blue() { return blue; }

    void enter(Warrior* w) {
      if (w->get_camp() == 1) red = w;
      else blue = w;
    }
    void leave(Warrior* w) {
      if (w->get_camp() == 1) red = nullptr;
      else blue = nullptr;
    }
};

class Headquarter{
  private:
    std::string name;
    std::vector<Warrior *> warrior_list;
    std::array<int, (int)WarriorType::COUNT> warrior_num;
    int spawn_ptr;
    int hp_origin;
    int n_warriors;
    int face;
    int loyalty_dec;

  public:
    Headquarter() {}
    Headquarter(int origin, std::string name_, int f, int loy_dec)
        : name(name_), spawn_ptr(0), hp_origin(origin), n_warriors(0), face(f), loyalty_dec(loy_dec) {
      warrior_num[(int)WarriorType::Dragon] = 0;
      warrior_num[(int)WarriorType::Ninja] = 0;
      warrior_num[(int)WarriorType::Iceman] = 0;
      warrior_num[(int)WarriorType::Lion] = 0;
      warrior_num[(int)WarriorType::Wolf] = 0;
    }
    ~Headquarter() {
      for (auto p: warrior_list) {
        delete p;
      }
      warrior_list.clear();
    }
    std::vector<int> spawn_seq;

    friend class GameWorld;
    
    Warrior* spawn_warrior();
    void remove_dead_warrior() {
      auto it = warrior_list.begin();
      while (it != warrior_list.end()) {
        if (!(*it)->is_alive()) {
            delete *it;
            it = warrior_list.erase(it);
            n_warriors--;
        } else {
            ++it;
        }
      }
    }

    void print_spawn(std::string warrior, int id, int total);
};

class GameWorld {
  public:
    int hp_origin;
    int n_city;
    int loyalty_dec;
    Headquarter* redHQ;
    Headquarter* blueHQ;
    City* head_city;
    City* tail_city;

    GameWorld(int hp_ori, int n, int loy_dec): hp_origin(hp_ori), n_city(n), loyalty_dec(loy_dec) {
      redHQ = new Headquarter(hp_ori, "red", RIGHT, loyalty_dec);
      blueHQ = new Headquarter(hp_ori, "blue", LEFT, loyalty_dec);
      redHQ->spawn_seq = {
        (int)WarriorType::Iceman, (int)WarriorType::Lion, (int)WarriorType::Wolf, 
        (int)WarriorType::Ninja, (int)WarriorType::Dragon };
      blueHQ->spawn_seq = {
        (int)WarriorType::Lion, (int)WarriorType::Dragon, (int)WarriorType::Ninja, 
        (int)WarriorType::Iceman, (int)WarriorType::Wolf };

      head_city = new City(0, nullptr, nullptr);
      tail_city = new City(n_city + 1, nullptr, nullptr);
      City* prev = head_city;
      City* cur = head_city;
      for (int i = 1; i <= n_city; i++) {
        cur = new City(i, prev, nullptr);
        prev = cur->lft;
      }
      cur->rgt = tail_city;
    }

    ~GameWorld() {
      delete redHQ;
      delete blueHQ;
      City* cur = head_city;
      for (int i = 0; i <= n_city + 1; i++) {
        cur = cur->rgt;
        delete cur->lft;
      }
      delete cur;
    }

    void spawn(Headquarter* HQ) {
      Warrior* w = HQ->spawn_warrior();
      if (HQ->name == "red") {
        if (head_city->red == nullptr) head_city->red = w;
        else throw "Head city's warrior not null";
      } else if (HQ->name == "blue") {
        if (tail_city->blue == nullptr) tail_city->blue = w;
        else throw "Tail city's warrior not null";
      }
    }

    void battle(Warrior* r, Warrior* b, City* c) {
      Battle bat(r, b, c);
      bat.fight();
      bat.eval();
      redHQ->remove_dead_warrior();
      blueHQ->remove_dead_warrior();
    }

    void lion_escape() {
      for (auto p: redHQ->warrior_list) {
        if (p->get_type() == WarriorType::Lion) {
          p->escape();
        }
      }
      redHQ->remove_dead_warrior();
      for (auto p: blueHQ->warrior_list) {
        if (p->get_type() == WarriorType::Lion) {
          p->escape();
        }
      }
      blueHQ->remove_dead_warrior();
    }

    void all_forward() {
      
    }

};


// ************************ //
//      Class Methods       //
// ************************ //


Warrior* Headquarter::spawn_warrior() {
  // check if HQ can spawn
  bool spawnFlag = false;
  for (int i = 0; i < N_Warrior_Kinds; i++) {
    int cur_warrior = spawn_seq[i];
    if (hp_origin >= Warrior_HP[cur_warrior]) {
      spawnFlag = true;
      break;
    }
  }
  if (!spawnFlag) return nullptr;

  // HQ can spawn warrior, do it
  for (;; spawn_ptr++) {
    spawn_ptr %= (int)WarriorType::COUNT;
    int war_kind = spawn_seq[spawn_ptr];
    if (hp_origin < Warrior_HP[war_kind])
      continue;

    // spawn!
    n_warriors++;
    int cur_id = n_warriors;
    hp_origin -= Warrior_HP[war_kind];

    if (war_kind == (int)WarriorType::Dragon) {
      int weap_id = cur_id % 3;
      WeaponType weap_type = weapid2type(weap_id);
      Weapon* weap = create_weapon(weap_type);
      Dragon* w = new Dragon(cur_id, face, weap, hp_origin);
      warrior_list.push_back(w);
    } else if(war_kind == (int)WarriorType::Ninja) {
      int weap1_id = cur_id % 3;
      int weap2_id = (cur_id + 1) % 3;
      WeaponType weap1_type = weapid2type(weap1_id);
      WeaponType weap2_type = weapid2type(weap2_id);
      Weapon* weap1 = create_weapon(weap1_type);
      Weapon* weap2 = create_weapon(weap2_type);
      Ninja* w = new Ninja(cur_id, face, weap1, weap2);
      warrior_list.push_back(w);
    } else if (war_kind == (int)WarriorType::Iceman) {
      int weap_id = cur_id % 3;
      WeaponType weap_type = weapid2type(weap_id);
      Weapon* weap = create_weapon(weap_type);
      Iceman* w = new Iceman(cur_id, face, weap);
      warrior_list.push_back(w);
    } else if (war_kind == (int)WarriorType::Lion) {
      int weap_id = cur_id % 3;
      WeaponType weap_type = weapid2type(weap_id);
      Weapon* weap = create_weapon(weap_type);
      Lion* w = new Lion(cur_id, face, weap, hp_origin, loyalty_dec);
      warrior_list.push_back(w);
    }

    spawn_ptr++;
    warrior_num[war_kind]++;

    return warrior_list.back();
  }
}


// ****************** //
//       Output       //
// ****************** //



// ****************** //
//        Main        //
// ****************** //


int main() {
  int T_datasets;
  std::cin >> T_datasets;

  for (int c = 1; c <= T_datasets; c++) {
    int hp_ori, n_city, loyalty_decrease, end_time;
    std::cin >> hp_ori >> n_city >> loyalty_decrease >> end_time;
    std::cin >> Warrior_HP["dragon"] >> Warrior_HP["ninja"] >>
        Warrior_HP["iceman"] >> Warrior_HP["lion"] >> Warrior_HP["wolf"];

    GameWorld map(hp_ori, n_city);

    std::cout << "Case:" << c << std::endl;
    for (_time_ = 0; _time_ <= end_time; _time_++) {
      map.redHQ->spawn_warrior();
      map.blueHQ->spawn_warrior();

    }
  }

  return 0;
}