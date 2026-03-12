#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <iomanip>

const int N_Warrior_Kinds = 5;
std::unordered_map<std::string, int> Warrior_HP;
static int _time_;

// ******************* //
//  Class Declarition  //
// ******************* //

class Warrior {
  private:
    std::string kind;
    int id;
    int hp;
    int dmg;
  public:
    Warrior(int id_, std::string kind_): kind(kind_), id(id_) {
      hp = Warrior_HP[kind];
    }
};

class Headquarter {
  private:
    std::string name;
    std::vector<Warrior> warrior_list;
    std::unordered_map<std::string, int> warrior_num;
    int cur_spawn_ptr;
    int hp_origin;
    int n_warriors;

  public:
    Headquarter() {}
    Headquarter(int origin, std::string name_): name(name_), cur_spawn_ptr(0), hp_origin(origin), n_warriors(0) {
      warrior_num["dragon"] = 0;
      warrior_num["ninja"] = 0;
      warrior_num["iceman"] = 0;
      warrior_num["lion"] = 0;
      warrior_num["wolf"] = 0;
    }
    ~Headquarter() {}

    bool stop_spawn = false;
    std::vector<std::string> spawn_seq;

    bool spawn_warrior();
    void print_spawn(std::string warrior, int id, int total);
    void print_stop_spawn();
};

class Dragon: public Warrior {};
class Ninja: public Warrior {};
class Iceman: public Warrior {};
class Lion: public Warrior {};
class Wolf: public Warrior {};


// ************************ //
//      Class Methods       //
// ************************ //


bool Headquarter::spawn_warrior() {
  // avoid extra end output
  if (stop_spawn) return false;
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
  for (; ; cur_spawn_ptr++) {
    // circulation
    if (cur_spawn_ptr >= N_Warrior_Kinds) cur_spawn_ptr %= N_Warrior_Kinds;
    // check if HQ can spawn current warrior
    std::string cur_war_kind = spawn_seq[cur_spawn_ptr];
    if (hp_origin < Warrior_HP[cur_war_kind]) continue;

    // spawn!
    n_warriors++;
    int cur_id = n_warriors;
    hp_origin -= Warrior_HP[cur_war_kind];
    warrior_list.push_back(Warrior(cur_id, cur_war_kind));
    cur_spawn_ptr++;
    warrior_num[cur_war_kind]++;
    
    // print log: spawn
    print_spawn(cur_war_kind, cur_id, warrior_num[cur_war_kind]);

    return true;
  }
}

void Headquarter::print_spawn(std::string warrior, int id, int total) {
  std::cout << std::setw(3) << std::setfill('0') << _time_ << " " << name << " ";
  std::cout << warrior << " " << id << " born with strength " << Warrior_HP[warrior] << ",";
  std::cout << total << " " << warrior << " in " << name << " headquarter" << std::endl;
}

void Headquarter::print_stop_spawn() {
  std::cout << std::setw(3) << std::setfill('0') << _time_ << " " << name << " ";
  std::cout << "headquarter stops making warriors" << std::endl;
}


int main() { 
  int T_datasets;
  std::cin >> T_datasets;

  for (int c = 1; c <= T_datasets; c++) {
    int hp_ori;
    std::cin >> hp_ori;
    std::cin >> Warrior_HP["dragon"] >> Warrior_HP["ninja"] >> Warrior_HP["iceman"]
             >> Warrior_HP["lion"] >> Warrior_HP["wolf"];
    
    Headquarter RedHQ(hp_ori, "red"), BlueHQ(hp_ori, "blue");
    RedHQ.spawn_seq = {"iceman", "lion", "wolf", "ninja", "dragon"};
    BlueHQ.spawn_seq = {"lion", "dragon", "ninja","iceman", "wolf"};

    std::cout << "Case:" << c << std::endl;
    for (_time_ = 0; ; _time_++) {
      if (RedHQ.stop_spawn && BlueHQ.stop_spawn) break;
      RedHQ.spawn_warrior();
      BlueHQ.spawn_warrior();
    }
  }
  
  return 0; 
}