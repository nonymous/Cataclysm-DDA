#ifndef _TRAP_H_
#define _TRAP_H_

#include "color.h"
#include "monster.h"
#include "itype.h"
#include "json.h"
#include <string>

class Creature;

typedef int trap_id;
/** map trap ids to index into @ref traps */
extern std::map<std::string, int> trapmap;
void set_trap_ids();
/** release all trap types, reset @ref traps and @ref trapmap */
void release_traps();
/** load a trap definition from json */
void load_trap(JsonObject &jo);

struct trap;

struct trapfunc {
    // creature is the creature that triggered the trap,
    // p is the point where the trap is (not where the creature is)
    // creature can be NULL.
    void none           (Creature *, int, int) { };
    void bubble         (Creature *creature, int x, int y);
    void cot            (Creature *creature, int x, int y);
    void beartrap       (Creature *creature, int x, int y);
    void snare_light    (Creature *creature, int x, int y);
    void snare_heavy    (Creature *creature, int x, int y);
    void board          (Creature *creature, int x, int y);
    void caltrops       (Creature *creature, int x, int y);
    void tripwire       (Creature *creature, int x, int y);
    void crossbow       (Creature *creature, int x, int y);
    void shotgun        (Creature *creature, int x, int y);
    void blade          (Creature *creature, int x, int y);
    void landmine       (Creature *creature, int x, int y);
    void telepad        (Creature *creature, int x, int y);
    void goo            (Creature *creature, int x, int y);
    void dissector      (Creature *creature, int x, int y);
    void sinkhole       (Creature *creature, int x, int y);
    void pit            (Creature *creature, int x, int y);
    void pit_spikes     (Creature *creature, int x, int y);
    void lava           (Creature *creature, int x, int y);
    void portal         (Creature *creature, int x, int y);
    void ledge          (Creature *creature, int x, int y);
    void boobytrap      (Creature *creature, int x, int y);
    void temple_flood   (Creature *creature, int x, int y);
    void temple_toggle  (Creature *creature, int x, int y);
    void glow           (Creature *creature, int x, int y);
    void hum            (Creature *creature, int x, int y);
    void shadow         (Creature *creature, int x, int y);
    void drain          (Creature *creature, int x, int y);
    void snake          (Creature *creature, int x, int y);
};

typedef void (trapfunc::*trap_function)(Creature *, int, int);

struct trap {
 std::string id;
 int loadid;
 std::string ident_string;
 long sym;
 nc_color color;
 std::string name;
private:
    friend void load_trap(JsonObject &jo);
 int visibility; // 1 to ??, affects detection
 int avoidance;  // 0 to ??, affects avoidance
 int difficulty; // 0 to ??, difficulty of assembly & disassembly
 bool benign;
    trap_function act;
public:
 std::vector<itype_id> components; // For disassembly?

 int get_avoidance() const { return avoidance; }
 int get_difficulty() const { return difficulty; }
// Type of trap
 bool is_benign() const { return benign; }
 // non-generic numbers for special cases
    /** Funnel value can be generated from the surface area or radius of the rain catching device to be modeled 
        following unit conversion between game units: 600 turns = 1 hour, 250 mL = 1 charge of water
        funnel value is easily calculated as surface area in square mm divided by 150
        a standard funnel with a (huge) radius of 380mm (area = 453645.6...)
        has a funnel value of 3024
    */
    int funnel_value;
    /** If an item with this weight or more is thrown onto the trap, it triggers. */
    int trigger_weight;
    /** Can player/npc p see this kind of trap? */
    bool can_see(const player &p, int x, int y) const;
    /** Trigger trap effects by creature that stepped onto it. */
    void trigger(Creature *creature, int x, int y) const;

 double funnel_turns_per_charge( double rain_depth_mm_per_hour ) const;
 /* pending jsonize
 std::set<std::string> flags
 std::string id;
 */

    trap(std::string string_id, int load_id, std::string pname, nc_color pcolor,
            char psym, int pvisibility, int pavoidance, int pdifficulty,
           trap_function pact,
            std::vector<std::string> keys) {
        //string_id is ignored at the moment, will later replace the id
        id = string_id;
        loadid = load_id;
        sym = psym;
        color = pcolor;
        name = pname;
        visibility = pvisibility;
        avoidance = pavoidance;
        difficulty = pdifficulty;
        act = pact;

        components.insert(components.end(), keys.begin(), keys.end());

        // It's a traaaap! So default;
        benign = false;
        // Traps are not typically funnels
        funnel_value = 0;
        trigger_weight = -1;
    };
};

/** list of all trap types */
extern std::vector<trap*> traplist;

trap_function trap_function_from_string(std::string function_name);

extern trap_id
 tr_null,
 tr_bubblewrap,
 tr_cot,
 tr_brazier,
 tr_funnel,
 tr_makeshift_funnel,
 tr_rollmat,
 tr_fur_rollmat,
 tr_beartrap,
 tr_beartrap_buried,
 tr_nailboard,
 tr_caltrops,
 tr_tripwire,
 tr_crossbow,
 tr_shotgun_2,
 tr_shotgun_1,
 tr_engine,
 tr_blade,
 tr_light_snare,
 tr_heavy_snare,
 tr_landmine,
 tr_landmine_buried,
 tr_telepad,
 tr_goo,
 tr_dissector,
 tr_sinkhole,
 tr_pit,
 tr_spike_pit,
 tr_lava,
 tr_portal,
 tr_ledge,
 tr_boobytrap,
 tr_temple_flood,
 tr_temple_toggle,
 tr_glow,
 tr_hum,
 tr_shadow,
 tr_drain,
 tr_snake;

#endif
