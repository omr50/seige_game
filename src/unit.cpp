#include "../include/unit.h"
#include "../include/unit.h"
#include <cmath>

Unit::Unit(int hp, Vector2 position, MOVE direction, Camera2D& camera, float scale = 1)
    : hp(hp), position(position), direction(direction), scale(scale), camera(camera) {
}


void Unit::UpdateAnimation(float delta_time) {
    change_dir();
    if (is_moving) {
        frame_counter += delta_time;
        if (frame_counter >= frame_time) {
            frame_counter = 0.0f;
            current_frame++;
            if (current_frame >= all_frames[direction].size()) {
                current_frame = 0;
            }
        }
    }
}

// void Unit::Draw() {
//     if (!all_frames[direction].empty()) {
//         Frame& frame = all_frames[direction][current_frame];
//         printf("DRAW DIRECTION %d\n", direction);
//         // Calculate the scaled dimensions
//         float scaledWidth = frame.source.width * scale;
//         float scaledHeight = frame.source.height * scale;

//         // Calculate the source and destination rectangles
//         Rectangle source = frame.source;
//         Rectangle dest = { position.x, position.y, scaledWidth, scaledHeight };

//         // Calculate the origin (center) for rotation
//         Vector2 origin = { scaledWidth / 2.0f, scaledHeight / 2.0f };

//         // Draw the texture using DrawTexturePro
//         DrawTexturePro(frame.texture, source, dest, origin, 0, WHITE);

//     }
// }

// void Unit::Draw() {
//     if (!all_frames[direction].empty()) {
//         Frame& frame = all_frames[direction][current_frame];
//         printf("DRAW DIRECTION %d\n", direction);
        
//         // Calculate the scaled dimensions
//         float scaledWidth = frame.source.width * scale;
//         float scaledHeight = frame.source.height * scale;

//         // Calculate the source and destination rectangles
//         Rectangle source = frame.source;
//         Rectangle dest = { position.x, position.y, scaledWidth, scaledHeight };

//         // Calculate the origin (center) for rotation
//         Vector2 origin = { scaledWidth / 2.0f, scaledHeight / 2.0f };

//         // Draw the texture using DrawTexturePro
//         DrawTexturePro(frame.texture, source, dest, origin, 0, WHITE);

//         // Calculate the center of the destination rectangle
//         Vector2 center = { position.x + scaledWidth / 2.0f, position.y + scaledHeight / 2.0f };

//         // Draw a small circle at the center
//         DrawCircleV(center, 5, RED); // Draw a red circle with a radius of 5
//     }
// }

void Unit::Draw() {
    if (!all_frames[direction].empty()) {
        Frame& frame = all_frames[direction][current_frame];
        // printf("DRAW DIRECTION %d\n", direction);
        
        // Calculate the scaled dimensions
        float scaledWidth = frame.source.width * scale;
        float scaledHeight = frame.source.height * scale;

        // Calculate the source and destination rectangles
        Rectangle source = frame.source;
        Rectangle dest = { position.x, position.y, scaledWidth, scaledHeight };

        // Calculate the origin (center) for rotation
        Vector2 origin = { scaledWidth / 2.0f , scaledHeight / 2.0f };

        // Draw the texture using DrawTexturePro
        DrawTexturePro(frame.texture, source, dest, origin, 0, WHITE);


        // Draw a small circle at the center
        DrawCircleV(position, 5, RED); // Draw a red circle with a radius of 5
    }
}


float Unit::unit_mouse_angle() {
    Vector2 mouse_pos = GetScreenToWorld2D(GetMousePosition(), camera);
    float dx = mouse_pos.x - position.x;
    float dy = mouse_pos.y - position.y;

    float angle = RAD2DEG * atan2(dy, dx) * -1;
    if (angle < 0)
        angle += 360.0f;

    return angle;
}

MOVE Unit::dir_from_angle(float angle) {
    // 8 directions and so we want to split into angle ranges.
    //the unit_mouse_angle function returns the angle from -180 to 180 so we can use that to determine dir.
    // 360 / 8 = 45 => -22.5 starting
    // East -> 20 to - 20,  NE -> 20 to 60, and so on.
    MOVE direction_order[] = { NORTH_EAST, NORTH, NORTH_WEST, WEST,  SOUTH_WEST, SOUTH, SOUTH_EAST };
                        //           67.5        112.5  157.5       202.5  247.5       292.5  337.5
    // if statement for all of them except east is relatively straight forward, so just keep the last case to be
    // east if it fits none of these. Because east is from 0 to 22.5 and also from 337.5 to 0, so a bit more annoying. 
    float starting_angle = 22.5;
    float offset = 45;
    for (int i = 0; i < 7; i++) {
        float new_dir = starting_angle + offset;
        if (starting_angle <= angle && angle < new_dir) 
            return direction_order[i];

        starting_angle = new_dir;
    }
    return EAST;
}

void Unit::change_dir() {
    MOVE dir = dir_from_angle(unit_mouse_angle());
    // printf("Dir from angle: %d\n", dir);
    direction = dir;
}

void Unit::move() {
    // vector distance from a to b
    // A* algorithm to move from a to b if there is 
}