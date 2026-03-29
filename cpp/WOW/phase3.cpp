#include <algorithm>
#include <array>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#define LEFT -1
#define RIGHT 1

enum class WeaponType { Sword = 0, Bomb = 1, Arrow = 2 };

enum class WarriorType { Dragon = 0, Ninja, Iceman, Lion, Wolf, COUNT };

class Warrior;
class Weapon;
class Headquarter;
class City;
class GameWorld;

std::array<int, (int)WarriorType::COUNT> Warrior_HP;
std::array<int, (int)WarriorType::COUNT> Warrior_ATK;

static int _time_;

std::array<std::string, 3> Id2Weapon = {"sword", "bomb", "arrow"};
std::array<std::string, 5> Id2Warrior = {"dragon", "ninja", "iceman", "lion",
                                         "wolf"};

// ******************* //
//  Class Declarition  //
// ******************* //

// WEAPON

class Weapon {
  protected:
    WeaponType type;

  public:
    explicit Weapon(WeaponType t) : type(t) {}
    virtual ~Weapon() {}

    WeaponType get_type() const { return type; }

    virtual int cal_cur_dmg(int warrior_dmg) const = 0;
    virtual int cal_self_dmg(int dealt_dmg, bool is_ninja) const = 0;
    virtual void be_used() = 0;
    virtual bool is_usable() const = 0;
    virtual int arrow_used() const = 0;
    virtual bool should_remove_after_use() const { return false; }
    virtual bool should_remove_when_zero_damage() const { return false; }
};

class Sword : public Weapon {
  private:
    bool remove_after_use;
    bool remove_when_zero_damage;

  public:
    Sword()
        : Weapon(WeaponType::Sword), remove_after_use(false),
          remove_when_zero_damage(false) {}

    virtual int cal_cur_dmg(int warrior_dmg) const override {
      return warrior_dmg * 2 / 10;
    }
    virtual int cal_self_dmg(int, bool) const override { return 0; }
    virtual void be_used() override {}
    virtual bool is_usable() const override { return true; }
    virtual int arrow_used() const override { return -1; }
    virtual bool should_remove_after_use() const override {
      return remove_after_use;
    }
    virtual bool should_remove_when_zero_damage() const override {
      return remove_when_zero_damage;
    }

    void set_remove_after_use(bool flag) { remove_after_use = flag; }
    void set_remove_when_zero_damage(bool flag) {
      remove_when_zero_damage = flag;
    }
};

class Bomb : public Weapon {
  private:
    int durability;

  public:
    Bomb() : Weapon(WeaponType::Bomb), durability(1) {}

    virtual int cal_cur_dmg(int warrior_dmg) const override {
      return warrior_dmg * 4 / 10;
    }
    virtual int cal_self_dmg(int dealt_dmg, bool is_ninja) const override {
      if (is_ninja)
        return 0;
      return dealt_dmg / 2;
    }
    virtual void be_used() override { durability--; }
    virtual bool is_usable() const override { return durability > 0; }
    virtual int arrow_used() const override { return -1; }
    virtual bool should_remove_after_use() const override { return true; }
};

class Arrow : public Weapon {
  private:
    int durability;

  public:
    Arrow() : Weapon(WeaponType::Arrow), durability(2) {}

    virtual int cal_cur_dmg(int warrior_dmg) const override {
      return warrior_dmg * 3 / 10;
    }
    virtual int cal_self_dmg(int, bool) const override { return 0; }
    virtual void be_used() override { durability--; }
    virtual bool is_usable() const override { return durability > 0; }
    virtual int arrow_used() const override { return 2 - durability; }
};

bool weapon_cmp(Weapon *a, Weapon *b) {
  if (a->get_type() != b->get_type())
    return (int)a->get_type() < (int)b->get_type();
  if (a->get_type() == WeaponType::Arrow)
    return a->arrow_used() > b->arrow_used();
  return false;
}

bool weapon_rob_cmp(Weapon *a, Weapon *b) {
  if (a->get_type() != b->get_type())
    return (int)a->get_type() < (int)b->get_type();
  if (a->get_type() == WeaponType::Arrow)
    return a->arrow_used() < b->arrow_used();
  return false;
}

Weapon *create_weapon(WeaponType type) {
  if (type == WeaponType::Sword) {
    return new Sword();
  } else if (type == WeaponType::Bomb) {
    return new Bomb();
  }
  return new Arrow();
}

WeaponType weapid2type(int id) { return (WeaponType)id; }

// CITY

class City {
  private:
    int id;
    Warrior *red;
    Warrior *blue;

  public:
    City() {}
    explicit City(int i) : id(i), red(nullptr), blue(nullptr) {}

    int get_id() const { return id; }
    Warrior *get_red() const { return red; }
    Warrior *get_blue() const { return blue; }

    void enter(Warrior *w);
    void leave(Warrior *w);
};

// WARRIOR

class Warrior {
  protected:
    WarriorType kind;
    int id;
    int hp;
    int dmg;
    int dir;
    bool alive;
    bool reached_enemy_hq;
    int city_id;

    std::vector<Weapon *> weapon_list;

  public:
    int cur_weap_index;

    Warrior(WarriorType k, int i, int d)
        : kind(k), id(i), hp(Warrior_HP[(int)k]), dmg(Warrior_ATK[(int)k]),
          dir(d), alive(true), reached_enemy_hq(false),
          city_id(d == RIGHT ? 0 : -1), cur_weap_index(0) {}
    virtual ~Warrior() {
      for (auto p : weapon_list)
        delete p;
      weapon_list.clear();
    }

    WarriorType get_type() const { return kind; }
    int get_id() const { return id; }
    int get_hp() const { return hp; }
    int get_dmg() const { return dmg; }
    int get_camp() const { return dir; }
    int get_city() const { return city_id; }

    bool is_alive() const { return alive; }
    bool has_weapon() const { return !weapon_list.empty(); }
    bool is_ninja() const { return kind == WarriorType::Ninja; }
    bool is_wolf() const { return kind == WarriorType::Wolf; }
    bool is_lion() const { return kind == WarriorType::Lion; }
    bool is_dragon() const { return kind == WarriorType::Dragon; }
    bool has_reached_enemy_hq() const { return reached_enemy_hq; }

    void set_city(int c) { city_id = c; }
    void set_reached_enemy_hq(bool flag) { reached_enemy_hq = flag; }

    virtual void print_spawn_extra() const {}
    virtual void after_step(int) {}
    virtual bool should_escape() const { return false; }

    void kill_self() {
      alive = false;
      hp = 0;
    }

    void hurt(int hurt_dmg) {
      hp -= hurt_dmg;
      if (hp <= 0) {
        hp = 0;
        alive = false;
      }
    }

    void add_weapon(Weapon *w) {
      if ((int)weapon_list.size() >= 10 || !w)
        return;
      weapon_list.push_back(w);
    }

    int weapon_count(WeaponType t) const {
      int cnt = 0;
      for (const auto &w : weapon_list)
        if (w->get_type() == t)
          cnt++;
      return cnt;
    }

    void sort_weapon() {
      std::stable_sort(weapon_list.begin(), weapon_list.end(), weapon_cmp);
    }
    void sort_rob_weapon() {
      std::stable_sort(weapon_list.begin(), weapon_list.end(), weapon_rob_cmp);
    }

    Weapon *next_weapon() {
      if (weapon_list.empty())
        return nullptr;
      cur_weap_index %= (int)weapon_list.size();
      return weapon_list[cur_weap_index];
    }

    bool should_remove_weapon(Weapon *w, int dealt_dmg) const {
      if (w == nullptr)
        return false;
      if (!w->is_usable())
        return true;
      if (w->should_remove_after_use())
        return true;
      if (w->should_remove_when_zero_damage() && dealt_dmg == 0)
        return true;
      return false;
    }

    void consume_current_weapon(int dealt_dmg) {
      if (weapon_list.empty())
        return;
      cur_weap_index %= (int)weapon_list.size();

      Weapon *used = weapon_list[cur_weap_index];
      used->be_used();
      if (should_remove_weapon(used, dealt_dmg)) {
        delete used;
        weapon_list.erase(weapon_list.begin() + cur_weap_index);
        if (cur_weap_index >= (int)weapon_list.size())
          cur_weap_index = 0;
      } else {
        cur_weap_index++;
        if (!weapon_list.empty())
          cur_weap_index %= (int)weapon_list.size();
      }
    }

    bool unchanged_even() const {
      if (weapon_list.empty())
        return true;
      bool has_dynamic = false;
      bool has_pos_dmg = false;
      for (const auto &w : weapon_list) {
        if (w->get_type() == WeaponType::Bomb ||
            w->get_type() == WeaponType::Arrow)
          has_dynamic = true;
        if (w->cal_cur_dmg(dmg) > 0)
          has_pos_dmg = true;
      }
      if (has_dynamic)
        return false;
      return !has_pos_dmg;
    }

    void attack(Warrior *enemy) {
      Weapon *weap = next_weapon();
      if (weap == nullptr)
        return;

      int weap_dmg = weap->cal_cur_dmg(dmg);
      enemy->hurt(weap_dmg);
      int self_dmg = weap->cal_self_dmg(weap_dmg, is_ninja());
      hurt(self_dmg);
      consume_current_weapon(weap_dmg);
    }

    int rob_weapon(Warrior *enemy, WeaponType &robbed_type) {
      if (!enemy || enemy->weapon_list.empty())
        return 0;
      enemy->cur_weap_index = 0;
      enemy->sort_rob_weapon();
      robbed_type = enemy->weapon_list.front()->get_type();

      std::vector<Weapon *> remain;
      remain.reserve(enemy->weapon_list.size());
      int got = 0;

      for (auto w : enemy->weapon_list) {
        if (w->get_type() == robbed_type && (int)weapon_list.size() < 10) {
          weapon_list.push_back(w);
          got++;
        } else {
          remain.push_back(w);
        }
      }
      enemy->weapon_list.clear();
      for (auto w : remain)
        enemy->weapon_list.push_back(w);

      return got;
    }

    void capture_weapon_from(Warrior *enemy) {
      if (!enemy)
        return;
      enemy->sort_rob_weapon();

      std::vector<Weapon *> remain;
      remain.reserve(enemy->weapon_list.size());

      for (auto w : enemy->weapon_list) {
        if ((int)weapon_list.size() < 10)
          weapon_list.push_back(w);
        else
          remain.push_back(w);
      }
      enemy->weapon_list.clear();
      for (auto w : remain)
        enemy->weapon_list.push_back(w);
    }
};

class Dragon : public Warrior {
  public:
    Dragon(int i, int d, Weapon *weap) : Warrior(WarriorType::Dragon, i, d) {
      add_weapon(weap);
    }
};

class Ninja : public Warrior {
  public:
    Ninja(int i, int d, Weapon *weap1, Weapon *weap2)
        : Warrior(WarriorType::Ninja, i, d) {
      add_weapon(weap1);
      add_weapon(weap2);
    }
};

class Iceman : public Warrior {
  public:
    Iceman(int i, int d, Weapon *weap) : Warrior(WarriorType::Iceman, i, d) {
      add_weapon(weap);
    }

    virtual void after_step(int) override {
      int hp_dec = hp / 10;
      hp -= hp_dec;
      if (hp <= 0) {
        hp = 0;
        alive = false;
      }
    }
};

class Lion : public Warrior {
  private:
    int loyalty;

  public:
    Lion(int i, int d, Weapon *weap, int loyalty_init)
        : Warrior(WarriorType::Lion, i, d), loyalty(loyalty_init) {
      add_weapon(weap);
    }

    virtual void print_spawn_extra() const override {
      std::cout << "Its loyalty is " << loyalty << std::endl;
    }

    virtual void after_step(int K) override { loyalty -= K; }
    virtual bool should_escape() const override {
      if (reached_enemy_hq)
        return false;
      return loyalty <= 0;
    }
};

class Wolf : public Warrior {
  public:
    Wolf(int i, int d) : Warrior(WarriorType::Wolf, i, d) {}
};

// HEADQUARTER

class Headquarter {
  private:
    std::string name;
    std::vector<Warrior *> warrior_list;
    int spawn_ptr;
    int hp_origin;
    int n_warriors;
    int face;

  public:
    Headquarter() {}
    Headquarter(int origin, const std::string &name_, int f)
        : name(name_), spawn_ptr(0), hp_origin(origin), n_warriors(0), face(f) {
    }

    std::vector<int> spawn_seq;
    bool stop_spawn = false;

    friend class GameWorld;

    Warrior *spawn_warrior();
    void remove_dead_warrior() {
      auto it = warrior_list.begin();
      while (it != warrior_list.end()) {
        if (!(*it)->is_alive()) {
          it = warrior_list.erase(it);
        } else {
          ++it;
        }
      }
    }
};

// GAMEWORLD

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

  public:
    GameWorld(int hp_ori, int n, int loy_dec, int end_t)
        : hp_origin(hp_ori), n_city(n), loyalty_dec(loy_dec), end_time(end_t),
          war_over(false) {
      redHQ = new Headquarter(hp_ori, "red", RIGHT);
      blueHQ = new Headquarter(hp_ori, "blue", LEFT);
      redHQ->spawn_seq = {(int)WarriorType::Iceman, (int)WarriorType::Lion,
                          (int)WarriorType::Wolf, (int)WarriorType::Ninja,
                          (int)WarriorType::Dragon};
      blueHQ->spawn_seq = {(int)WarriorType::Lion, (int)WarriorType::Dragon,
                           (int)WarriorType::Ninja, (int)WarriorType::Iceman,
                           (int)WarriorType::Wolf};

      cities.reserve(n_city + 2);
      for (int i = 0; i <= n_city + 1; i++) {
        cities.push_back(City(i));
      }
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

    void move_one(Warrior *w, int to, std::vector<Warrior *> &nred,
                  std::vector<Warrior *> &nblue) {
      if (w == nullptr || !w->is_alive())
        return;
      w->set_city(to);
      w->after_step(loyalty_dec);
      if (!w->is_alive())
        return;
      if (w->get_camp() == RIGHT)
        nred[to] = w;
      else
        nblue[to] = w;
    }

    void event_march() {
      std::vector<Warrior *> next_red(n_city + 2, nullptr),
          next_blue(n_city + 2, nullptr);

      for (int i = n_city; i >= 0; --i) {
        Warrior *r = cities[i].get_red();
        if (!r || !r->is_alive())
          continue;
        cities[i].leave(r);
        move_one(r, i + 1, next_red, next_blue);
        if (r->is_alive() && i + 1 == n_city + 1)
          r->set_reached_enemy_hq(true);
      }
      for (int i = 1; i <= n_city + 1; ++i) {
        Warrior *b = cities[i].get_blue();
        if (!b || !b->is_alive())
          continue;
        cities[i].leave(b);
        move_one(b, i - 1, next_red, next_blue);
        if (b->is_alive() && i - 1 == 0)
          b->set_reached_enemy_hq(true);
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
          Warrior *b = cities[i].get_blue();
          if (b && b->is_alive()) {
            print_time(_time_);
            std::cout << " blue " << Id2Warrior[(int)b->get_type()] << " "
                      << b->get_id() << " reached red headquarter with "
                      << b->get_hp() << " elements and force " << b->get_dmg()
                      << std::endl;
            print_time(_time_);
            std::cout << " red headquarter was taken" << std::endl;
            taken = true;
          }
        } else if (i == n_city + 1) {
          Warrior *r = cities[i].get_red();
          if (r && r->is_alive()) {
            print_time(_time_);
            std::cout << " red " << Id2Warrior[(int)r->get_type()] << " "
                      << r->get_id() << " reached blue headquarter with "
                      << r->get_hp() << " elements and force " << r->get_dmg()
                      << std::endl;
            print_time(_time_);
            std::cout << " blue headquarter was taken" << std::endl;
            taken = true;
          }
        } else {
          Warrior *r = cities[i].get_red();
          if (r && r->is_alive()) {
            print_time(_time_);
            std::cout << " red " << Id2Warrior[(int)r->get_type()] << " "
                      << r->get_id() << " marched to city " << i << " with "
                      << r->get_hp() << " elements and force " << r->get_dmg()
                      << std::endl;
          }
          Warrior *b = cities[i].get_blue();
          if (b && b->is_alive()) {
            print_time(_time_);
            std::cout << " blue " << Id2Warrior[(int)b->get_type()] << " "
                      << b->get_id() << " marched to city " << i << " with "
                      << b->get_hp() << " elements and force " << b->get_dmg()
                      << std::endl;
          }
        }
      }

      if (taken)
        war_over = true;
    }

    void event_wolf_rob() {
      for (int i = 1; i <= n_city; i++) {
        Warrior *r = cities[i].get_red();
        Warrior *b = cities[i].get_blue();
        if (!r || !b || !r->is_alive() || !b->is_alive())
          continue;

        if (r->is_wolf() && !b->is_wolf()) {
          WeaponType robbed_type = WeaponType::Sword;
          int got = r->rob_weapon(b, robbed_type);
          if (got > 0) {
            print_time(_time_);
            std::cout << " red wolf " << r->get_id() << " took " << got << " "
                      << Id2Weapon[(int)robbed_type] << " from blue "
                      << Id2Warrior[(int)b->get_type()] << " " << b->get_id()
                      << " in city " << i << std::endl;
          }
        }
        if (b->is_wolf() && !r->is_wolf()) {
          WeaponType robbed_type = WeaponType::Sword;
          int got = b->rob_weapon(r, robbed_type);
          if (got > 0) {
            print_time(_time_);
            std::cout << " blue wolf " << b->get_id() << " took " << got << " "
                      << Id2Weapon[(int)robbed_type] << " from red "
                      << Id2Warrior[(int)r->get_type()] << " " << r->get_id()
                      << " in city " << i << std::endl;
          }
        }
      }
    }

    void battle_one_city(int i) {
      Warrior *r = cities[i].get_red();
      Warrior *b = cities[i].get_blue();
      if (!r || !b || !r->is_alive() || !b->is_alive())
        return;

      Warrior *first = (i % 2 == 1) ? r : b;
      Warrior *second = (i % 2 == 1) ? b : r;
      first->sort_weapon();
      second->sort_weapon();
      first->cur_weap_index = second->cur_weap_index = 0;

      if (!first->has_weapon() && !second->has_weapon()) {
        print_time(_time_);
        std::cout << " both red " << Id2Warrior[(int)r->get_type()] << " "
                  << r->get_id() << " and blue "
                  << Id2Warrior[(int)b->get_type()] << " " << b->get_id()
                  << " were alive in city " << i << std::endl;
        if (r->is_dragon() && r->is_alive()) {
          print_time(_time_);
          std::cout << " red dragon " << r->get_id() << " yelled in city " << i
                    << std::endl;
        }
        if (b->is_dragon() && b->is_alive()) {
          print_time(_time_);
          std::cout << " blue dragon " << b->get_id() << " yelled in city " << i
                    << std::endl;
        }
        return;
      }

      while (1) {
        if (first->unchanged_even() && second->unchanged_even())
          break;
        if (first->has_weapon())
          first->attack(second);
        if (!first->is_alive() || !second->is_alive())
          break;
        if (second->has_weapon())
          second->attack(first);
        if (!first->is_alive() || !second->is_alive())
          break;

        if ((!first->has_weapon() || first->unchanged_even()) &&
            (!second->has_weapon() || second->unchanged_even()))
          break;
      }

      if (r->is_alive() && b->is_alive()) {
        print_time(_time_);
        std::cout << " both red " << Id2Warrior[(int)r->get_type()] << " "
                  << r->get_id() << " and blue "
                  << Id2Warrior[(int)b->get_type()] << " " << b->get_id()
                  << " were alive in city " << i << std::endl;
        if (r->is_dragon()) {
          print_time(_time_);
          std::cout << " red dragon " << r->get_id() << " yelled in city " << i
                    << std::endl;
        }
        if (b->is_dragon()) {
          print_time(_time_);
          std::cout << " blue dragon " << b->get_id() << " yelled in city " << i
                    << std::endl;
        }
        return;
      }

      if (!r->is_alive() && !b->is_alive()) {
        print_time(_time_);
        std::cout << " both red " << Id2Warrior[(int)r->get_type()] << " "
                  << r->get_id() << " and blue "
                  << Id2Warrior[(int)b->get_type()] << " " << b->get_id()
                  << " died in city " << i << std::endl;
        cities[i].leave(r);
        cities[i].leave(b);
        return;
      }

      Warrior *winner = r->is_alive() ? r : b;
      Warrior *loser = r->is_alive() ? b : r;

      print_time(_time_);
      std::cout << " " << (winner->get_camp() == RIGHT ? "red" : "blue") << " "
                << Id2Warrior[(int)winner->get_type()] << " "
                << winner->get_id() << " killed "
                << (loser->get_camp() == RIGHT ? "red" : "blue") << " "
                << Id2Warrior[(int)loser->get_type()] << " " << loser->get_id()
                << " in city " << i << " remaining " << winner->get_hp()
                << " elements" << std::endl;

      winner->capture_weapon_from(loser);
      cities[i].leave(loser);

      if (winner->is_dragon()) {
        print_time(_time_);
        std::cout << " " << (winner->get_camp() == RIGHT ? "red" : "blue")
                  << " dragon " << winner->get_id() << " yelled in city " << i
                  << std::endl;
      }
    }

    void event_battle() {
      for (int i = 1; i <= n_city; i++) {
        battle_one_city(i);
      }
      redHQ->remove_dead_warrior();
      blueHQ->remove_dead_warrior();
    }

    void event_report_hq() {
      print_time(_time_);
      std::cout << " " << redHQ->hp_origin << " elements in red headquarter"
                << std::endl;
      print_time(_time_);
      std::cout << " " << blueHQ->hp_origin << " elements in blue headquarter"
                << std::endl;
    }

    void report_warrior(Warrior *w) {
      if (!w || !w->is_alive())
        return;
      print_time(_time_);
      std::cout << " " << (w->get_camp() == RIGHT ? "red" : "blue") << " "
                << Id2Warrior[(int)w->get_type()] << " " << w->get_id()
                << " has " << w->weapon_count(WeaponType::Sword) << " sword "
                << w->weapon_count(WeaponType::Bomb) << " bomb "
                << w->weapon_count(WeaponType::Arrow) << " arrow and "
                << w->get_hp() << " elements" << std::endl;
    }

    void event_report_warrior() {
      for (int i = 0; i <= n_city + 1; i++) {
        report_warrior(cities[i].get_red());
        report_warrior(cities[i].get_blue());
      }
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
        else if (minute == 35)
          event_wolf_rob();
        else if (minute == 40)
          event_battle();
        else if (minute == 50)
          event_report_hq();
        else if (minute == 55)
          event_report_warrior();
      }
    }
};

// ************************ //
//      Class Methods       //
// ************************ //

void City::enter(Warrior *w) {
  if (w->get_camp() == RIGHT)
    red = w;
  else
    blue = w;
}

void City::leave(Warrior *w) {
  if (w->get_camp() == RIGHT)
    red = nullptr;
  else
    blue = nullptr;
}

Warrior *Headquarter::spawn_warrior() {
  if (stop_spawn)
    return nullptr;

  int war_kind = spawn_seq[spawn_ptr];
  spawn_ptr = (spawn_ptr + 1) % (int)WarriorType::COUNT;
  if (hp_origin < Warrior_HP[war_kind]) {
    stop_spawn = true;
    return nullptr;
  }

  n_warriors++;
  int cur_id = n_warriors;
  hp_origin -= Warrior_HP[war_kind];

  Warrior *w = nullptr;
  if (war_kind == (int)WarriorType::Dragon) {
    int weap_id = cur_id % 3;
    w = new Dragon(cur_id, face, create_weapon(weapid2type(weap_id)));
  } else if (war_kind == (int)WarriorType::Ninja) {
    int weap1_id = cur_id % 3;
    int weap2_id = (cur_id + 1) % 3;
    w = new Ninja(cur_id, face, create_weapon(weapid2type(weap1_id)),
                  create_weapon(weapid2type(weap2_id)));
  } else if (war_kind == (int)WarriorType::Iceman) {
    int weap_id = cur_id % 3;
    w = new Iceman(cur_id, face, create_weapon(weapid2type(weap_id)));
  } else if (war_kind == (int)WarriorType::Lion) {
    int weap_id = cur_id % 3;
    w = new Lion(cur_id, face, create_weapon(weapid2type(weap_id)), hp_origin);
  } else if (war_kind == (int)WarriorType::Wolf) {
    w = new Wolf(cur_id, face);
  }

  if (w)
    warrior_list.push_back(w);
  return w;
}

// ****************** //
//        Main        //
// ****************** //

int main() {
  int T_datasets;
  std::cin >> T_datasets;

  for (int c = 1; c <= T_datasets; c++) {
    int hp_ori, n_city, loyalty_decrease, end_time;
    std::cin >> hp_ori >> n_city >> loyalty_decrease >> end_time;
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
