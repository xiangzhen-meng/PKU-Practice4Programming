#include <iomanip>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

const int N_Warrior_Kinds = 5;
std::unordered_map<std::string, int> Warrior_HP;
std::unordered_map<int, std::string> Id2Weapon = {
    {0, "sword"}, {1, "bomb"}, {2, "arrow"}};
static int _time_;


// ******************* //
//  Class Declarition  //
// ******************* //

// weapon

class Weapon {
  protected:
    int id;

  public:
    std::string kind;
    Weapon(int id_) : id(id_), kind(Id2Weapon[id_]) {}
};

// warriors, including all derivatives

class Warrior {
  protected:
    std::string kind;
    int id;
    int hp;
    int dmg;

  public:
    Warrior(int id_, std::string kind_) : kind(kind_), id(id_) {
      hp = Warrior_HP[kind];
    }
    virtual ~Warrior() {}
    virtual void print_spawn_derive();
};

class Dragon : public Warrior {
  private:
    Weapon weapon;
    double morale;

  public:
    Dragon(int id_, std::string kind_, int hp_origin)
        : Warrior(id_, kind_), weapon(Weapon(id_ % 3)) {
      morale = (hp_origin * 1.0) / (Warrior_HP[kind] * 1.0);
    }
    virtual void print_spawn_derive() override;
};

class Ninja : public Warrior {
  private:
    Weapon weapon1;
    Weapon weapon2;

  public:
    Ninja(int id_, std::string kind_) 
        : Warrior(id_, kind_), weapon1(Weapon(id_ % 3)), weapon2(Weapon((id_+1) % 3)) {}
    virtual void print_spawn_derive() override;
};

class Iceman : public Warrior {
  private:
    Weapon weapon;

  public:
    Iceman(int id_, std::string kind_) 
        : Warrior(id_, kind_), weapon(Weapon(id_ % 3)) {}
    virtual void print_spawn_derive() override;
};

class Lion : public Warrior {
  private:
    int loyalty;

  public:
    Lion(int id_, std::string kind_, int hp_origin) 
        : Warrior(id_, kind_), loyalty(hp_origin) {}
    virtual void print_spawn_derive() override;
};

class Wolf : public Warrior {
  public:
    Wolf(int id_, std::string kind_) : Warrior(id_, kind_) {}
    virtual void print_spawn_derive() override;
};

// headquarters

class Headquarter {
  private:
    std::string name;
    std::vector<Warrior *> warrior_list;
    std::unordered_map<std::string, int> warrior_num;
    int cur_spawn_ptr;
    int hp_origin;
    int n_warriors;

  public:
    Headquarter() {}
    Headquarter(int origin, std::string name_)
        : name(name_), cur_spawn_ptr(0), hp_origin(origin), n_warriors(0) {

      warrior_num["dragon"] = 0;
      warrior_num["ninja"] = 0;
      warrior_num["iceman"] = 0;
      warrior_num["lion"] = 0;
      warrior_num["wolf"] = 0;
    }
    ~Headquarter() {
      for (auto p: warrior_list) {
        delete p;
      }
      warrior_list.clear();
    }

    bool stop_spawn = false;
    std::vector<std::string> spawn_seq;

    bool spawn_warrior();
    void print_spawn(std::string warrior, int id, int total);
    void print_stop_spawn();
};


// ************************ //
//      Class Methods       //
// ************************ //


bool Headquarter::spawn_warrior() {
  // avoid extra end output
  if (stop_spawn)
    return false;
  // check if HQ can spawn
  bool spawnFlag = false;
  for (int i = 0; i < N_Warrior_Kinds; i++) {
    std::string cur_warrior = spawn_seq[i];
    if (hp_origin >= Warrior_HP[cur_warrior]) {
      spawnFlag = true;
      break;
    }
  }
  if (!spawnFlag) {
    stop_spawn = true;
    // print log
    print_stop_spawn();
    return false;
  }

  // HQ can spawn warrior, do it
  for (;; cur_spawn_ptr++) {
    // circulation
    if (cur_spawn_ptr >= N_Warrior_Kinds)
      cur_spawn_ptr %= N_Warrior_Kinds;
    // check if HQ can spawn current warrior
    std::string cur_war_kind = spawn_seq[cur_spawn_ptr];
    if (hp_origin < Warrior_HP[cur_war_kind])
      continue;

    // spawn!
    n_warriors++;
    int cur_id = n_warriors;
    hp_origin -= Warrior_HP[cur_war_kind];
    // warrior_list.push_back(&Warrior(cur_id, cur_war_kind));
    if (cur_war_kind == "dragon") {
      Dragon* new_dragon = new Dragon(cur_id, cur_war_kind, hp_origin);
      warrior_list.push_back(new_dragon);
    } else if (cur_war_kind == "ninja") {
      Ninja* new_ninja = new Ninja(cur_id, cur_war_kind);
      warrior_list.push_back(new_ninja);
    } else if (cur_war_kind == "iceman") {
      Iceman* new_iceman = new Iceman(cur_id, cur_war_kind);
      warrior_list.push_back(new_iceman);
    } else if (cur_war_kind == "lion") {
      Lion* new_lion = new Lion(cur_id, cur_war_kind, hp_origin);
      warrior_list.push_back(new_lion);
    } else if (cur_war_kind == "wolf") {
      Wolf* new_wolf = new Wolf(cur_id, cur_war_kind);
      warrior_list.push_back(new_wolf);
    }
    cur_spawn_ptr++;
    warrior_num[cur_war_kind]++;

    // print log: spawn
    print_spawn(cur_war_kind, cur_id, warrior_num[cur_war_kind]);
    warrior_list.back()->print_spawn_derive();

    return true;
  }
}

void Headquarter::print_spawn(std::string warrior, int id, int total) {
  std::cout << std::setw(3) << std::setfill('0') << _time_ << " " << name
            << " ";
  std::cout << warrior << " " << id << " born with strength "
            << Warrior_HP[warrior] << ",";
  std::cout << total << " " << warrior << " in " << name << " headquarter"
            << std::endl;
}

void Headquarter::print_stop_spawn() {
  std::cout << std::setw(3) << std::setfill('0') << _time_ << " " << name
            << " ";
  std::cout << "headquarter stops making warriors" << std::endl;
}

void Warrior::print_spawn_derive() {}

void Dragon::print_spawn_derive() {
  std::cout << "It has a " << weapon.kind << ",and it's morale is "
            << std::fixed << std::setprecision(2) << morale << std::endl;
}

void Ninja::print_spawn_derive() {
  std::cout << "It has a " << weapon1.kind << " and a " << weapon2.kind << std::endl;
}

void Iceman::print_spawn_derive() {
  std::cout << "It has a " << weapon.kind << std::endl;
}

void Lion::print_spawn_derive() {
  std::cout << "It's loyalty is " << loyalty << std::endl;
}

void Wolf::print_spawn_derive() {}


// ****************** //
//        Main        //
// ****************** //


int main() {
  int T_datasets;
  std::cin >> T_datasets;

  for (int c = 1; c <= T_datasets; c++) {
    int hp_ori;
    std::cin >> hp_ori;
    std::cin >> Warrior_HP["dragon"] >> Warrior_HP["ninja"] >>
        Warrior_HP["iceman"] >> Warrior_HP["lion"] >> Warrior_HP["wolf"];

    Headquarter RedHQ(hp_ori, "red"), BlueHQ(hp_ori, "blue");
    RedHQ.spawn_seq = {"iceman", "lion", "wolf", "ninja", "dragon"};
    BlueHQ.spawn_seq = {"lion", "dragon", "ninja", "iceman", "wolf"};

    std::cout << "Case:" << c << std::endl;
    for (_time_ = 0;; _time_++) {
      if (RedHQ.stop_spawn && BlueHQ.stop_spawn)
        break;
      RedHQ.spawn_warrior();
      BlueHQ.spawn_warrior();
    }
  }

  return 0;
}