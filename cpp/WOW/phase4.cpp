#include <algorithm>
#include <array>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#define LEFT -1
#define RIGHT 1

enum class WeaponType { Sword = 0, Bomb = 1, Arrow = 2 };
enum class WarriorType { Dragon = 0, Ninja, Iceman, Lion, Wolf, COUNT };
enum class Flag { None = 0, Red = 1, Blue = 2 };

class Warrior;
class Headquarter;
class City;
class GameWorld;

std::array<int, (int)WarriorType::COUNT> Warrior_HP;
std::array<int, (int)WarriorType::COUNT> Warrior_ATK;

static int _time_;
static int ARROW_ATK;

std::array<std::string, 3> Id2Weapon = {"sword", "bomb", "arrow"};
std::array<std::string, 5> Id2Warrior = {"dragon", "ninja", "iceman", "lion",
                                         "wolf"};

struct Sword {
  int atk;
};

struct Arrow {
  int uses;
};

struct Bomb {
};

class Warrior {
  protected:
  WarriorType kind;
  int id;
  int hp;
  int atk;
  int camp;
  bool alive;
  bool reached_enemy_hq;
  bool killed_by_arrow;
  int city_id;

  Sword *sword;
  Arrow *arrow;
  Bomb *bomb;

  public:
  Warrior(WarriorType k, int i, int c)
      : kind(k), id(i), hp(Warrior_HP[(int)k]), atk(Warrior_ATK[(int)k]),
        camp(c), alive(true), reached_enemy_hq(false),
        killed_by_arrow(false), city_id(c == RIGHT ? 0 : -1), sword(nullptr),
        arrow(nullptr), bomb(nullptr) {}

  virtual ~Warrior() {
    delete sword;
    delete arrow;
    delete bomb;
  }

  WarriorType get_type() const { return kind; }
  int get_id() const { return id; }
  int get_hp() const { return hp; }
  int get_atk() const { return atk; }
  int get_camp() const { return camp; }
  int get_city() const { return city_id; }

  bool is_alive() const { return alive; }
  bool is_ninja() const { return kind == WarriorType::Ninja; }
  bool is_wolf() const { return kind == WarriorType::Wolf; }
  bool is_lion() const { return kind == WarriorType::Lion; }
  bool is_dragon() const { return kind == WarriorType::Dragon; }
  bool was_killed_by_arrow() const { return killed_by_arrow; }
  bool has_reached_enemy_hq() const { return reached_enemy_hq; }

  void set_city(int c) { city_id = c; }
  void set_reached_enemy_hq(bool flag) { reached_enemy_hq = flag; }
  void mark_killed_by_arrow() { killed_by_arrow = true; }

  bool has_sword() const { return sword != nullptr; }
  bool has_arrow() const { return arrow != nullptr; }
  bool has_bomb() const { return bomb != nullptr; }

  int sword_atk() const { return sword ? sword->atk : 0; }
  int arrow_uses() const { return arrow ? arrow->uses : 0; }

  void add_sword(int atk_val) {
    if (atk_val <= 0)
      return;
    sword = new Sword{atk_val};
  }

  void add_arrow(int uses = 3) {
    arrow = new Arrow{uses};
  }

  void add_bomb() { bomb = new Bomb(); }

  void lose_sword() {
    delete sword;
    sword = nullptr;
  }

  void lose_arrow() {
    delete arrow;
    arrow = nullptr;
  }

  void lose_bomb() {
    delete bomb;
    bomb = nullptr;
  }

  void kill_self() {
    alive = false;
    hp = 0;
  }

  void hurt(int dmg) {
    hp -= dmg;
    if (hp <= 0) {
      hp = 0;
      alive = false;
    }
  }

  void heal(int amount) {
    hp += amount;
  }

  void dull_sword() {
    if (!sword)
      return;
    sword->atk = sword->atk * 8 / 10;
    if (sword->atk <= 0) {
      lose_sword();
    }
  }

  void use_arrow() {
    if (!arrow)
      return;
    arrow->uses--;
    if (arrow->uses <= 0)
      lose_arrow();
  }

  virtual void print_spawn_extra() const {}
  virtual void on_march() {}
  virtual bool should_escape() const { return false; }
  virtual void after_battle(bool killed_enemy, bool won, int loyalty_dec) {}

  std::string report_weapons() const {
    if (!sword && !arrow && !bomb)
      return "no weapon";
    std::string out;
    bool first = true;
    if (arrow) {
      out += "arrow(" + std::to_string(arrow->uses) + ")";
      first = false;
    }
    if (bomb) {
      if (!first)
        out += ",";
      out += "bomb";
      first = false;
    }
    if (sword) {
      if (!first)
        out += ",";
      out += "sword(" + std::to_string(sword->atk) + ")";
    }
    return out;
  }
};

class Dragon : public Warrior {
  private:
  double morale;

  public:
  Dragon(int i, int c, double m) : Warrior(WarriorType::Dragon, i, c), morale(m) {}

  double get_morale() const { return morale; }
  void change_morale(double delta) { morale += delta; }

  virtual void print_spawn_extra() const override {
    std::cout << "Its morale is " << std::fixed << std::setprecision(2) << morale
              << std::endl;
  }
};

class Ninja : public Warrior {
  public:
  Ninja(int i, int c) : Warrior(WarriorType::Ninja, i, c) {}
};

class Iceman : public Warrior {
  private:
  int step;

  public:
  Iceman(int i, int c) : Warrior(WarriorType::Iceman, i, c), step(0) {}

  virtual void on_march() override {
    step++;
    if (step % 2 == 0) {
      if (hp - 9 <= 0)
        hp = 1;
      else
        hp -= 9;
      atk += 20;
    }
  }
};

class Lion : public Warrior {
  private:
  int loyalty;

  public:
  Lion(int i, int c, int loyalty_init)
      : Warrior(WarriorType::Lion, i, c), loyalty(loyalty_init) {}

  int get_loyalty() const { return loyalty; }

  virtual void print_spawn_extra() const override {
    std::cout << "Its loyalty is " << loyalty << std::endl;
  }

  virtual bool should_escape() const override {
    if (reached_enemy_hq)
      return false;
    return loyalty <= 0;
  }

  virtual void after_battle(bool killed_enemy, bool, int loyalty_dec) override {
    if (!killed_enemy)
      loyalty -= loyalty_dec;
  }
};

class Wolf : public Warrior {
  public:
  Wolf(int i, int c) : Warrior(WarriorType::Wolf, i, c) {}
};

class City {
  private:
  int id;
  int elements;
  Flag flag;
  int last_winner; // 0 none, RIGHT red, LEFT blue
  Warrior *red;
  Warrior *blue;

  public:
  City() {}
  explicit City(int i)
      : id(i), elements(0), flag(Flag::None), last_winner(0), red(nullptr),
        blue(nullptr) {}

  int get_id() const { return id; }
  int get_elements() const { return elements; }
  void add_elements(int v) { elements += v; }
  void clear_elements() { elements = 0; }

  Flag get_flag() const { return flag; }
  void set_flag(Flag f) { flag = f; }
  int get_last_winner() const { return last_winner; }
  void set_last_winner(int w) { last_winner = w; }

  Warrior *get_red() const { return red; }
  Warrior *get_blue() const { return blue; }

  void enter(Warrior *w) {
    if (w->get_camp() == RIGHT)
      red = w;
    else
      blue = w;
  }

  void leave(Warrior *w) {
    if (w->get_camp() == RIGHT)
      red = nullptr;
    else
      blue = nullptr;
  }
};

class Headquarter {
  private:
  std::string name;
  std::vector<Warrior *> warrior_list;
  int spawn_ptr;
  int elements;
  int n_warriors;
  int face;

  public:
  Headquarter() {}
  Headquarter(int origin, const std::string &name_, int f)
      : name(name_), spawn_ptr(0), elements(origin), n_warriors(0), face(f) {}

  std::vector<int> spawn_seq;

  friend class GameWorld;

  int get_elements() const { return elements; }
  void add_elements(int v) { elements += v; }
  bool consume_elements(int v) {
    if (elements < v)
      return false;
    elements -= v;
    return true;
  }

  Warrior *spawn_warrior();

  void remove_dead_warrior() {
    auto it = warrior_list.begin();
    while (it != warrior_list.end()) {
      if (!(*it)->is_alive()) {
        delete *it;
        it = warrior_list.erase(it);
      } else {
        ++it;
      }
    }
  }
};

class GameWorld {
  private:
  int hp_origin;
  int n_city;
  int loyalty_dec;
  int end_time;

  Headquarter *redHQ;
  Headquarter *blueHQ;
  std::vector<City> cities;
  bool war_over;
  std::vector<Warrior *> red_at_blue_hq;
  std::vector<Warrior *> blue_at_red_hq;
  bool red_hq_taken;
  bool blue_hq_taken;

  public:
  GameWorld(int hp_ori, int n, int loy_dec, int end_t)
      : hp_origin(hp_ori), n_city(n), loyalty_dec(loy_dec), end_time(end_t),
        war_over(false), red_hq_taken(false), blue_hq_taken(false) {
    redHQ = new Headquarter(hp_ori, "red", RIGHT);
    blueHQ = new Headquarter(hp_ori, "blue", LEFT);
    redHQ->spawn_seq = {(int)WarriorType::Iceman, (int)WarriorType::Lion,
                        (int)WarriorType::Wolf, (int)WarriorType::Ninja,
                        (int)WarriorType::Dragon};
    blueHQ->spawn_seq = {(int)WarriorType::Lion, (int)WarriorType::Dragon,
                         (int)WarriorType::Ninja, (int)WarriorType::Iceman,
                         (int)WarriorType::Wolf};

    cities.reserve(n_city + 2);
    for (int i = 0; i <= n_city + 1; i++)
      cities.push_back(City(i));
  }

  ~GameWorld() {
    for (auto p : redHQ->warrior_list)
      delete p;
    for (auto p : blueHQ->warrior_list)
      delete p;
    delete redHQ;
    delete blueHQ;
  }

  static void print_time(int time) {
    int h = time / 60;
    int m = time % 60;
    std::cout << std::setw(3) << std::setfill('0') << h << ":" << std::setw(2)
              << std::setfill('0') << m;
  }

  static std::string time_prefix(int time) {
    std::ostringstream oss;
    int h = time / 60;
    int m = time % 60;
    oss << std::setw(3) << std::setfill('0') << h << ":" << std::setw(2)
        << std::setfill('0') << m;
    return oss.str();
  }

  void print_warrior_brief(const Warrior *w) const {
    std::cout << (w->get_camp() == RIGHT ? "red" : "blue") << " "
              << Id2Warrior[(int)w->get_type()] << " " << w->get_id();
  }

  void spawn(Headquarter *HQ) {
    Warrior *w = HQ->spawn_warrior();
    if (w == nullptr)
      return;

    if (w->get_camp() == RIGHT) {
      cities[0].enter(w);
      w->set_city(0);
    } else {
      cities[n_city + 1].enter(w);
      w->set_city(n_city + 1);
    }

    print_time(_time_);
    std::cout << " ";
    print_warrior_brief(w);
    std::cout << " born" << std::endl;
    w->print_spawn_extra();
  }

  void event_spawn() {
    spawn(redHQ);
    spawn(blueHQ);
  }

  void event_lion_escape() {
    for (int i = 0; i <= n_city + 1; i++) {
      Warrior *r = cities[i].get_red();
      if (r && r->is_alive() && r->is_lion() && r->should_escape()) {
        print_time(_time_);
        std::cout << " red lion " << r->get_id() << " ran away" << std::endl;
        r->kill_self();
        cities[i].leave(r);
      }

      Warrior *b = cities[i].get_blue();
      if (b && b->is_alive() && b->is_lion() && b->should_escape()) {
        print_time(_time_);
        std::cout << " blue lion " << b->get_id() << " ran away" << std::endl;
        b->kill_self();
        cities[i].leave(b);
      }
    }
    redHQ->remove_dead_warrior();
    blueHQ->remove_dead_warrior();
  }

  void event_march() {
    std::vector<Warrior *> next_red(n_city + 2, nullptr),
        next_blue(n_city + 2, nullptr);
    std::vector<Warrior *> new_blue_at_red, new_red_at_blue;

    for (int i = n_city; i >= 0; --i) {
      Warrior *r = cities[i].get_red();
      if (!r || !r->is_alive())
        continue;
      cities[i].leave(r);
      r->set_city(i + 1);
      r->on_march();
      if (!r->is_alive())
        continue;
      if (i + 1 == n_city + 1) {
        r->set_reached_enemy_hq(true);
        red_at_blue_hq.push_back(r);
        new_red_at_blue.push_back(r);
      } else {
        next_red[i + 1] = r;
      }
    }
    for (int i = 1; i <= n_city + 1; ++i) {
      Warrior *b = cities[i].get_blue();
      if (!b || !b->is_alive())
        continue;
      cities[i].leave(b);
      b->set_city(i - 1);
      b->on_march();
      if (!b->is_alive())
        continue;
      if (i - 1 == 0) {
        b->set_reached_enemy_hq(true);
        blue_at_red_hq.push_back(b);
        new_blue_at_red.push_back(b);
      } else {
        next_blue[i - 1] = b;
      }
    }

    for (int i = 0; i <= n_city + 1; i++) {
      if (next_red[i])
        cities[i].enter(next_red[i]);
      if (next_blue[i])
        cities[i].enter(next_blue[i]);
    }

    bool taken = false;
    for (int i = 0; i <= n_city + 1; i++) {
      if (i == 0) {
        for (auto *b : new_blue_at_red) {
          if (!b->is_alive())
            continue;
          print_time(_time_);
          std::cout << " blue " << Id2Warrior[(int)b->get_type()] << " "
                    << b->get_id() << " reached red headquarter with "
                    << b->get_hp() << " elements and force " << b->get_atk()
                    << std::endl;
        }
        if (blue_at_red_hq.size() >= 2 && !red_hq_taken) {
          print_time(_time_);
          std::cout << " red headquarter was taken" << std::endl;
          red_hq_taken = true;
          taken = true;
        }
      } else if (i == n_city + 1) {
        for (auto *r : new_red_at_blue) {
          if (!r->is_alive())
            continue;
          print_time(_time_);
          std::cout << " red " << Id2Warrior[(int)r->get_type()] << " "
                    << r->get_id() << " reached blue headquarter with "
                    << r->get_hp() << " elements and force " << r->get_atk()
                    << std::endl;
        }
        if (red_at_blue_hq.size() >= 2 && !blue_hq_taken) {
          print_time(_time_);
          std::cout << " blue headquarter was taken" << std::endl;
          blue_hq_taken = true;
          taken = true;
        }
      } else {
        Warrior *r = cities[i].get_red();
        if (r && r->is_alive()) {
          print_time(_time_);
          std::cout << " red " << Id2Warrior[(int)r->get_type()] << " "
                    << r->get_id() << " marched to city " << i << " with "
                    << r->get_hp() << " elements and force " << r->get_atk()
                    << std::endl;
        }
        Warrior *b = cities[i].get_blue();
        if (b && b->is_alive()) {
          print_time(_time_);
          std::cout << " blue " << Id2Warrior[(int)b->get_type()] << " "
                    << b->get_id() << " marched to city " << i << " with "
                    << b->get_hp() << " elements and force " << b->get_atk()
                    << std::endl;
        }
      }
    }

    if (taken)
      war_over = true;
  }

  void event_city_produce() {
    for (int i = 1; i <= n_city; i++)
      cities[i].add_elements(10);
  }

  void event_city_take() {
    for (int i = 1; i <= n_city; i++) {
      Warrior *r = cities[i].get_red();
      Warrior *b = cities[i].get_blue();
      int got = cities[i].get_elements();
      if (got == 0)
        continue;
      if (r && r->is_alive() && (!b || !b->is_alive())) {
        redHQ->add_elements(got);
        print_time(_time_);
        std::cout << " red " << Id2Warrior[(int)r->get_type()] << " "
                  << r->get_id() << " earned " << got
                  << " elements for his headquarter" << std::endl;
        cities[i].clear_elements();
      } else if (b && b->is_alive() && (!r || !r->is_alive())) {
        blueHQ->add_elements(got);
        print_time(_time_);
        std::cout << " blue " << Id2Warrior[(int)b->get_type()] << " "
                  << b->get_id() << " earned " << got
                  << " elements for his headquarter" << std::endl;
        cities[i].clear_elements();
      }
    }
  }

  void event_arrow_shot() {
    struct Shot {
      Warrior *shooter;
      Warrior *target;
      int city_id;
    };

    std::vector<Shot> shots;
    shots.reserve(n_city * 2);

    for (int i = 1; i <= n_city; i++) {
      Warrior *r = cities[i].get_red();
      if (r && r->is_alive() && r->has_arrow() && i + 1 <= n_city) {
        Warrior *target = cities[i + 1].get_blue();
        if (target && target->is_alive())
          shots.push_back({r, target, i});
      }
      Warrior *b = cities[i].get_blue();
      if (b && b->is_alive() && b->has_arrow() && i - 1 >= 1) {
        Warrior *target = cities[i - 1].get_red();
        if (target && target->is_alive())
          shots.push_back({b, target, i});
      }
    }

    for (auto &shot : shots) {
      shot.target->hurt(ARROW_ATK);
    }

    std::sort(shots.begin(), shots.end(), [](const Shot &a, const Shot &b) {
      if (a.city_id != b.city_id)
        return a.city_id < b.city_id;
      return a.shooter->get_camp() == RIGHT && b.shooter->get_camp() == LEFT;
    });

    for (auto &shot : shots) {
      bool killed = !shot.target->is_alive();
      if (killed)
        shot.target->mark_killed_by_arrow();
      shot.shooter->use_arrow();

      print_time(_time_);
      std::cout << " "
                << (shot.shooter->get_camp() == RIGHT ? "red" : "blue")
                << " " << Id2Warrior[(int)shot.shooter->get_type()] << " "
                << shot.shooter->get_id() << " shot";
      if (killed) {
        std::cout << " and killed "
                  << (shot.target->get_camp() == RIGHT ? "red" : "blue")
                  << " " << Id2Warrior[(int)shot.target->get_type()] << " "
                  << shot.target->get_id();
      }
      std::cout << std::endl;
    }
  }

  int attack_damage(Warrior *w) const {
    return w->get_atk() + w->sword_atk();
  }

  int counter_damage(Warrior *w) const {
    return w->get_atk() / 2 + w->sword_atk();
  }

  bool will_attacker_die(Warrior *attacker, Warrior *defender) const {
    int dmg = attack_damage(attacker);
    if (dmg >= defender->get_hp())
      return false;
    if (defender->is_ninja())
      return false;
    int dmg2 = counter_damage(defender);
    return dmg2 >= attacker->get_hp();
  }

  bool will_defender_die(Warrior *attacker, Warrior *defender) const {
    int dmg = attack_damage(attacker);
    return dmg >= defender->get_hp();
  }

  void event_bomb() {
    for (int i = 1; i <= n_city; i++) {
      Warrior *r = cities[i].get_red();
      Warrior *b = cities[i].get_blue();
      if (!r || !b || !r->is_alive() || !b->is_alive())
        continue;

      bool red_attack = false;
      if (cities[i].get_flag() == Flag::Red)
        red_attack = true;
      else if (cities[i].get_flag() == Flag::Blue)
        red_attack = false;
      else
        red_attack = (i % 2 == 1);

      Warrior *attacker = red_attack ? r : b;
      Warrior *defender = red_attack ? b : r;

      bool attacker_use = attacker->has_bomb() &&
                          will_attacker_die(attacker, defender);
      bool defender_use = defender->has_bomb() &&
                          will_defender_die(attacker, defender);

      Warrior *user = nullptr;
      Warrior *other = nullptr;
      if (attacker_use) {
        user = attacker;
        other = defender;
      } else if (defender_use) {
        user = defender;
        other = attacker;
      }

      if (user) {
        print_time(_time_);
        std::cout << " " << (user->get_camp() == RIGHT ? "red" : "blue")
                  << " " << Id2Warrior[(int)user->get_type()] << " "
                  << user->get_id() << " used a bomb and killed "
                  << (other->get_camp() == RIGHT ? "red" : "blue")
                  << " " << Id2Warrior[(int)other->get_type()] << " "
                  << other->get_id() << std::endl;
        user->lose_bomb();
        user->kill_self();
        other->kill_self();
        cities[i].leave(r);
        cities[i].leave(b);
      }
    }
  }

  struct BattleOutcome {
    bool happened = false;
    bool draw = false;
    bool red_attack = false;
    bool win_by_arrow = false;
    int city_id = 0;
    Warrior *red = nullptr;
    Warrior *blue = nullptr;
    Warrior *winner = nullptr;
    Warrior *loser = nullptr;
    std::vector<std::string> outputs;
    int transfer_elements = 0;
    bool raise_flag = false;
    Flag new_flag = Flag::None;
  };

  void battle_city(int i, std::vector<BattleOutcome> &outcomes,
                   std::vector<Warrior *> &red_winners,
                   std::vector<Warrior *> &blue_winners) {
    BattleOutcome out;
    out.city_id = i;
    Warrior *r = cities[i].get_red();
    Warrior *b = cities[i].get_blue();
    if (!r && !b)
      return;
    out.red = r;
    out.blue = b;

    if (!r || !b) {
      return;
    }

    bool red_attack = false;
    if (cities[i].get_flag() == Flag::Red)
      red_attack = true;
    else if (cities[i].get_flag() == Flag::Blue)
      red_attack = false;
    else
      red_attack = (i % 2 == 1);
    out.red_attack = red_attack;

    std::string attack_line;
    std::string fight_line;
    std::vector<std::string> death_lines;
    std::string yell_line;
    std::string earn_line;
    std::string flag_line;

    if (!r->is_alive() && r->was_killed_by_arrow() && b->is_alive()) {
      out.happened = true;
      out.win_by_arrow = true;
      out.winner = b;
      out.loser = r;
    } else if (!b->is_alive() && b->was_killed_by_arrow() && r->is_alive()) {
      out.happened = true;
      out.win_by_arrow = true;
      out.winner = r;
      out.loser = b;
    } else if (!r->is_alive() || !b->is_alive()) {
      return;
    } else {
      out.happened = true;
      Warrior *attacker = red_attack ? r : b;
      Warrior *defender = red_attack ? b : r;

      int attacker_hp_before = attacker->get_hp();
      int defender_hp_before = defender->get_hp();

      int dmg1 = attack_damage(attacker);
      {
        std::ostringstream oss;
        oss << time_prefix(_time_) << " "
            << (attacker->get_camp() == RIGHT ? "red" : "blue") << " "
            << Id2Warrior[(int)attacker->get_type()] << " " << attacker->get_id()
            << " attacked "
            << (defender->get_camp() == RIGHT ? "red" : "blue") << " "
            << Id2Warrior[(int)defender->get_type()] << " " << defender->get_id()
            << " in city " << i << " with " << attacker_hp_before
            << " elements and force " << attacker->get_atk();
        attack_line = oss.str();
      }

      defender->hurt(dmg1);
      attacker->dull_sword();

      bool defender_dead = !defender->is_alive();
      bool attacker_dead = false;

      if (!defender_dead && !defender->is_ninja()) {
        int dmg2 = counter_damage(defender);
        {
          std::ostringstream oss;
          oss << time_prefix(_time_) << " "
              << (defender->get_camp() == RIGHT ? "red" : "blue")
              << " " << Id2Warrior[(int)defender->get_type()] << " "
              << defender->get_id() << " fought back against "
              << (attacker->get_camp() == RIGHT ? "red" : "blue") << " "
              << Id2Warrior[(int)attacker->get_type()] << " "
              << attacker->get_id() << " in city " << i;
          fight_line = oss.str();
        }
        attacker->hurt(dmg2);
        defender->dull_sword();
        attacker_dead = !attacker->is_alive();
      }

      if (defender_dead) {
        if (defender->is_lion())
          attacker->heal(defender_hp_before);
        std::ostringstream oss;
        oss << time_prefix(_time_) << " "
            << (defender->get_camp() == RIGHT ? "red" : "blue") << " "
            << Id2Warrior[(int)defender->get_type()] << " " << defender->get_id()
            << " was killed in city " << i;
        death_lines.push_back(oss.str());
      }
      if (attacker_dead) {
        if (attacker->is_lion())
          defender->heal(attacker_hp_before);
        std::ostringstream oss;
        oss << time_prefix(_time_) << " "
            << (attacker->get_camp() == RIGHT ? "red" : "blue") << " "
            << Id2Warrior[(int)attacker->get_type()] << " " << attacker->get_id()
            << " was killed in city " << i;
        death_lines.push_back(oss.str());
      }

      if (attacker->is_alive() && defender->is_alive()) {
        out.draw = true;
      } else {
        out.winner = attacker->is_alive() ? attacker : defender;
        out.loser = attacker->is_alive() ? defender : attacker;
      }
    }

    if (out.happened) {
      if (out.winner) {
        if (out.winner->get_camp() == RIGHT)
          red_winners.push_back(out.winner);
        else
          blue_winners.push_back(out.winner);
      }

      if (r && r->is_alive())
        r->after_battle(r == out.winner, out.winner == r, loyalty_dec);
      if (b && b->is_alive())
        b->after_battle(b == out.winner, out.winner == b, loyalty_dec);

      if (r && r->is_dragon()) {
        Dragon *dr = static_cast<Dragon *>(r);
        if (out.winner == r)
          dr->change_morale(0.2);
        else
          dr->change_morale(-0.2);
      }
      if (b && b->is_dragon()) {
        Dragon *db = static_cast<Dragon *>(b);
        if (out.winner == b)
          db->change_morale(0.2);
        else
          db->change_morale(-0.2);
      }

      if (out.winner && out.winner->is_dragon()) {
        bool winner_attack = out.red_attack
                                 ? (out.winner->get_camp() == RIGHT)
                                 : (out.winner->get_camp() == LEFT);
        if (winner_attack) {
          Dragon *d = static_cast<Dragon *>(out.winner);
          if (d->get_morale() > 0.8) {
            std::ostringstream oss;
            oss << time_prefix(_time_) << " "
                << (out.winner->get_camp() == RIGHT ? "red" : "blue")
                << " dragon " << out.winner->get_id() << " yelled in city " << i;
            yell_line = oss.str();
          }
        }
      } else if (out.draw) {
        Warrior *attacker = red_attack ? r : b;
        if (attacker && attacker->is_alive() && attacker->is_dragon()) {
          Dragon *d = static_cast<Dragon *>(attacker);
          if (d->get_morale() > 0.8) {
            std::ostringstream oss;
            oss << time_prefix(_time_) << " "
                << (attacker->get_camp() == RIGHT ? "red" : "blue")
                << " dragon " << attacker->get_id() << " yelled in city " << i;
            yell_line = oss.str();
          }
        }
      }

      if (out.winner && out.winner->is_wolf()) {
        Warrior *loser = out.loser;
        if (loser) {
          if (!out.winner->has_arrow() && loser->has_arrow()) {
            out.winner->add_arrow(loser->arrow_uses());
          }
          if (!out.winner->has_bomb() && loser->has_bomb()) {
            out.winner->add_bomb();
          }
          if (!out.winner->has_sword() && loser->has_sword()) {
            out.winner->add_sword(loser->sword_atk());
          }
        }
      }

      if (out.winner) {
        out.transfer_elements = cities[i].get_elements();
        if (out.transfer_elements > 0) {
          std::ostringstream oss;
          oss << time_prefix(_time_) << " "
              << (out.winner->get_camp() == RIGHT ? "red" : "blue")
              << " " << Id2Warrior[(int)out.winner->get_type()] << " "
              << out.winner->get_id() << " earned " << out.transfer_elements
              << " elements for his headquarter";
          earn_line = oss.str();
        }
      }

      if (out.happened) {
        if (out.draw) {
          cities[i].set_last_winner(0);
        } else if (out.winner) {
          int cur_winner = out.winner->get_camp();
          if (cities[i].get_last_winner() == cur_winner) {
            Flag want_flag = cur_winner == RIGHT ? Flag::Red : Flag::Blue;
            if (cities[i].get_flag() != want_flag) {
              cities[i].set_flag(want_flag);
              out.raise_flag = true;
              out.new_flag = want_flag;
              std::ostringstream oss;
              oss << time_prefix(_time_) << " "
                  << (want_flag == Flag::Red ? "red" : "blue")
                  << " flag raised in city " << i;
              flag_line = oss.str();
            }
          }
          cities[i].set_last_winner(cur_winner);
        }
      }

      if (!r->is_alive()) {
        cities[i].leave(r);
      }
      if (!b->is_alive()) {
        cities[i].leave(b);
      }
    }

    if (out.happened) {
      if (!attack_line.empty())
        out.outputs.push_back(attack_line);
      if (!fight_line.empty())
        out.outputs.push_back(fight_line);
      for (const auto &line : death_lines)
        out.outputs.push_back(line);
      if (!yell_line.empty())
        out.outputs.push_back(yell_line);
      if (!earn_line.empty())
        out.outputs.push_back(earn_line);
      if (!flag_line.empty())
        out.outputs.push_back(flag_line);
      outcomes.push_back(out);
    }
  }

  void event_battle() {
    std::vector<BattleOutcome> outcomes;
    std::vector<Warrior *> red_winners;
    std::vector<Warrior *> blue_winners;

    for (int i = 1; i <= n_city; i++) {
      battle_city(i, outcomes, red_winners, blue_winners);
    }

    std::sort(red_winners.begin(), red_winners.end(),
              [](Warrior *a, Warrior *b) { return a->get_city() > b->get_city(); });
    std::sort(blue_winners.begin(), blue_winners.end(),
              [](Warrior *a, Warrior *b) { return a->get_city() < b->get_city(); });

    for (auto w : red_winners) {
      if (redHQ->consume_elements(8))
        w->heal(8);
    }
    for (auto w : blue_winners) {
      if (blueHQ->consume_elements(8))
        w->heal(8);
    }

    for (const auto &out : outcomes) {
      for (const auto &line : out.outputs)
        std::cout << line << std::endl;
      if (out.winner && out.transfer_elements > 0) {
        if (out.winner->get_camp() == RIGHT)
          redHQ->add_elements(out.transfer_elements);
        else
          blueHQ->add_elements(out.transfer_elements);
        cities[out.city_id].clear_elements();
      }
    }

    for (int i = 1; i <= n_city; i++) {
      Warrior *r = cities[i].get_red();
      if (r && !r->is_alive())
        cities[i].leave(r);
      Warrior *b = cities[i].get_blue();
      if (b && !b->is_alive())
        cities[i].leave(b);
    }

    redHQ->remove_dead_warrior();
    blueHQ->remove_dead_warrior();
  }

  void event_report_hq() {
    print_time(_time_);
    std::cout << " " << redHQ->get_elements() << " elements in red headquarter"
              << std::endl;
    print_time(_time_);
    std::cout << " " << blueHQ->get_elements() << " elements in blue headquarter"
              << std::endl;
  }

  void report_warrior(Warrior *w) {
    if (!w || !w->is_alive())
      return;
    print_time(_time_);
    std::cout << " " << (w->get_camp() == RIGHT ? "red" : "blue") << " "
              << Id2Warrior[(int)w->get_type()] << " " << w->get_id() << " has "
              << w->report_weapons() << std::endl;
  }

  void event_report_warrior() {
    report_warrior(cities[0].get_red());
    for (int i = 1; i <= n_city; i++) {
      report_warrior(cities[i].get_red());
    }
    for (auto *w : red_at_blue_hq)
      report_warrior(w);

    for (auto *w : blue_at_red_hq)
      report_warrior(w);
    for (int i = 1; i <= n_city; i++) {
      report_warrior(cities[i].get_blue());
    }
    report_warrior(cities[n_city + 1].get_blue());
  }

  void run() {
    for (_time_ = 0; _time_ <= end_time; _time_++) {
      if (war_over)
        break;
      int minute = _time_ % 60;
      if (minute == 0)
        event_spawn();
      else if (minute == 5)
        event_lion_escape();
      else if (minute == 10)
        event_march();
      else if (minute == 20)
        event_city_produce();
      else if (minute == 30)
        event_city_take();
      else if (minute == 35)
        event_arrow_shot();
      else if (minute == 38)
        event_bomb();
      else if (minute == 40)
        event_battle();
      else if (minute == 50)
        event_report_hq();
      else if (minute == 55)
        event_report_warrior();
    }
  }
};

Warrior *Headquarter::spawn_warrior() {
  int war_kind = spawn_seq[spawn_ptr];
  if (elements < Warrior_HP[war_kind])
    return nullptr;

  elements -= Warrior_HP[war_kind];
  n_warriors++;
  int cur_id = n_warriors;

  spawn_ptr = (spawn_ptr + 1) % (int)WarriorType::COUNT;

  Warrior *w = nullptr;
  if (war_kind == (int)WarriorType::Dragon) {
    double morale = 0.0;
    int cost = Warrior_HP[war_kind];
    if (cost > 0)
      morale = (double)elements / cost;
    w = new Dragon(cur_id, face, morale);
    int weap_id = cur_id % 3;
    if (weap_id == (int)WeaponType::Sword)
      w->add_sword(w->get_atk() * 2 / 10);
    else if (weap_id == (int)WeaponType::Bomb)
      w->add_bomb();
    else
      w->add_arrow();
  } else if (war_kind == (int)WarriorType::Ninja) {
    w = new Ninja(cur_id, face);
    int weap1_id = cur_id % 3;
    int weap2_id = (cur_id + 1) % 3;
    if (weap1_id == (int)WeaponType::Sword)
      w->add_sword(w->get_atk() * 2 / 10);
    else if (weap1_id == (int)WeaponType::Bomb)
      w->add_bomb();
    else
      w->add_arrow();

    if (weap2_id == (int)WeaponType::Sword)
      w->add_sword(w->get_atk() * 2 / 10);
    else if (weap2_id == (int)WeaponType::Bomb)
      w->add_bomb();
    else
      w->add_arrow();
  } else if (war_kind == (int)WarriorType::Iceman) {
    w = new Iceman(cur_id, face);
    int weap_id = cur_id % 3;
    if (weap_id == (int)WeaponType::Sword)
      w->add_sword(w->get_atk() * 2 / 10);
    else if (weap_id == (int)WeaponType::Bomb)
      w->add_bomb();
    else
      w->add_arrow();
  } else if (war_kind == (int)WarriorType::Lion) {
    w = new Lion(cur_id, face, elements);
  } else if (war_kind == (int)WarriorType::Wolf) {
    w = new Wolf(cur_id, face);
  }

  if (w)
    warrior_list.push_back(w);
  return w;
}

int main() {
  int T_datasets;
  std::cin >> T_datasets;

  for (int c = 1; c <= T_datasets; c++) {
    int hp_ori, n_city, loyalty_decrease, end_time;
    std::cin >> hp_ori >> n_city >> ARROW_ATK >> loyalty_decrease >> end_time;
    std::cin >> Warrior_HP[(int)WarriorType::Dragon] >>
        Warrior_HP[(int)WarriorType::Ninja] >>
        Warrior_HP[(int)WarriorType::Iceman] >>
        Warrior_HP[(int)WarriorType::Lion] >>
        Warrior_HP[(int)WarriorType::Wolf];
    std::cin >> Warrior_ATK[(int)WarriorType::Dragon] >>
        Warrior_ATK[(int)WarriorType::Ninja] >>
        Warrior_ATK[(int)WarriorType::Iceman] >>
        Warrior_ATK[(int)WarriorType::Lion] >>
        Warrior_ATK[(int)WarriorType::Wolf];

    std::cout << "Case " << c << ":" << std::endl;
    GameWorld world(hp_ori, n_city, loyalty_decrease, end_time);
    world.run();
  }

  return 0;
}
