/*

This class is for units (may be a base class which will be extended). The
units are like cavalry, knight, archer, scout, catapult, etc.

Each unit should have a position, set of movements and frames (map), a logical movement
system and detection system for allowing them to move logically across barriers, and also
detect and fight enemy troops. You can override who they fight, but once in range of enemy
troop they have to engage (or flee if you command that as well but make sure no loop where
they are stuck fleeing then attacking).

The intersection checking and clicking should be handled by some outside class, and we register
units to that class, and so when we click, we can see which unit, or structure, or what ever else
we clicked on.

-------------------------------------------MAP----------------------------------------------------
maybe all units of the same type can share pointer to the same map, since they are read only
then maybe we won't have an issue since we won't touch the map after creation. It should just
have the same elements.
*/

#ifndef UNIT_H
#define UNIT_H

#include <raylib.h>
#include <unordered_map>
#include "utilities.h"

class Unit {
public:
    int hp;
    float scale;
    Vector2 position;
    std::unordered_map<MOVE, std::vector<Frame>> all_frames;
    MOVE direction;
    bool is_moving = false;
    int current_frame = 0;
    float frame_time = 0.1f;
    float frame_counter = 0.0f;
    Camera2D& camera;
    Vector2 destination = { 0.0, 0.0 };

    Unit(int hp, Vector2 position, MOVE direction, Camera2D& camera, float scale);
    void UpdateAnimation(float delta_time);
    void Draw();
    float unit_mouse_angle();
    MOVE dir_from_angle(float angle); 
    void change_dir();
    void move();
};

#endif 