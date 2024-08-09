#include <raylib.h>
#include "./include/utilities.h"
#include "./include/unit.h"

int main() {
    int num_catapult_frames = 31;
    std::string prefix_file_name = "../assets/catapult/isometric/move/catapult_move_";
    Vector2 catapult_pos = {300.0f, 300.0f};
    Vector2 knight_pos = {500.0f, 400.0f};
    int hp = 5;
    std::unordered_map<MOVE, std::vector<Frame>> catapult_frames;
    std::unordered_map<MOVE, std::vector<Frame>> knight_frames;
    Camera2D camera = {0};
    Unit catapult(hp, catapult_pos, NORTH, camera, 0.5);
    Unit knight(hp, knight_pos, SOUTH, camera, 1);
    SetTargetFPS(60);

    InitializeWindowWithAspectRatio();
    Texture2D grass_texture = LoadTexture("../assets/tiles/Grass/Grass_16-128x128.png");
    Texture2D castle_texture = LoadTexture("../assets/castle/castle.png");
    printf("LOADED CASTLE OR NO!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
    LoadCatapultFramesFromSeparateImages(catapult_frames, prefix_file_name.c_str(), 8, num_catapult_frames, {SOUTH, SOUTH_EAST, EAST, NORTH_EAST, NORTH, NORTH_WEST, WEST, SOUTH_WEST});
    LoadFramesFromSpriteSheet(knight_frames, "../assets/knight/Walking/Orc_Walking.png", 180, 180, 16, 128, {EAST, NORTH, NORTH_EAST, NORTH_WEST, SOUTH, SOUTH_EAST, SOUTH_WEST, WEST});

    camera.target = (Vector2){(GRID_WIDTH * TILE_SIZE) / 2.0f, (GRID_HEIGHT * TILE_SIZE) / 2.0f};
    camera.offset = (Vector2){GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    catapult.all_frames = catapult_frames;
    knight.all_frames = knight_frames;
    int map_width = TILE_SIZE * GRID_WIDTH;
    int map_height = TILE_SIZE * GRID_HEIGHT;

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();
        catapult.UpdateAnimation(deltaTime);
        knight.UpdateAnimation(deltaTime);
        resize_logic(GetScreenWidth(), GetScreenHeight());
        keyboard_control(camera);
        camera_logic(camera);

        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode2D(camera);
        draw_background(grass_texture, GRID_WIDTH, GRID_HEIGHT, TILE_SIZE);
        draw_grid(GRID_WIDTH, GRID_HEIGHT, TILE_SIZE);
        draw_castle(castle_texture);
        catapult.Draw();
        knight.Draw();
        EndMode2D();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}


// #include <raylib.h>
// #include "./include/utilities.h"
// #include "./include/unit.h"
// #include "raylib.h"

// int main() {

//     // load some textures, get stuff on the screen.
    
//     int num_catapult_frames = 31;
//     int w = 309, h = 282;
//     std::string prefix_file_name = "../assets/catapult/isometric/move/catapult_move_";
//     // std::string prefix_file_name = " assets\catapult\isometric\move\catapult_move_";
//     Vector2 catapult_pos = {300.0f, 300.0f};
//     Vector2 knight_pos = {500.0f, 400.0f};
//     int hp = 5;
//     std::unordered_map<MOVE, std::vector<Frame>> catapult_frames;
//     std::unordered_map<MOVE, std::vector<Frame>> knight_frames;
//     Camera2D camera = { 0 };
//     Unit catapult(hp, catapult_pos, NORTH, camera, 0.5);
//     Unit knight(hp, knight_pos, SOUTH, camera, 1);
//     catapult.is_moving = true;
//     knight.is_moving = true;
//     SetTargetFPS(2);

//     // so there is 8 directions, pre-fix for direction 0 to 7
//     // and within each direction there is 31 frames, so 2 parts after
//     // prefix. catapult_move_10002 means 1st direction (0 indexed) and 2nd frame in that
//     // so we will call the utility function 8 different times, for each direction with a different
//     // prefix.

//     /*
//     Goals:
//     1. load map flooring
//     2. load units
//     3. load castle
//     4. scale everything the right size to match each other
//     5. make a block system
//     6. make things move
//     7. drag and drops
//     8. different stats for units
//     9. make towers, walls
//     10. make things move correctly (avoid walls, dir = next block dir)
//         (using vector move controls, but definitely have to avoid walls)  
//         (use chat gpt for more info on this)
//     11. make things attack each other
//     12. 
//     */
//     InitializeWindowWithAspectRatio();
//     Texture2D grass_texture = LoadTexture("../assets/tiles/Grass/Grass_16-128x128.png");
//     LoadCatapultFramesFromSeparateImages(catapult_frames, prefix_file_name.c_str(), 8, num_catapult_frames, { SOUTH, SOUTH_EAST, EAST, NORTH_EAST, NORTH, NORTH_WEST, WEST, SOUTH_WEST });
//     LoadFramesFromSpriteSheet(knight_frames, "../assets/knight/Walking/Orc_Walking.png", 180, 180, 16, 128, { EAST, NORTH, NORTH_EAST, NORTH_WEST, SOUTH, SOUTH_EAST, SOUTH_WEST, WEST }); 
//     // Set up the camera
//     int screenWidth = 128 * 10;
//     int screenHeight = 128 * 6;

//     // Calculate the maximum possible window size that maintains the aspect ratio
//     camera.target = (Vector2){ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
//     camera.offset = (Vector2){ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
//     camera.rotation = 0.0f;
//     camera.zoom = 1.0f;

//     // printf("FINISHED?-----------------------------------------------------------------------------------------------------\n");
//     catapult.all_frames = catapult_frames;
//     knight.all_frames = knight_frames;
//     int map_width = TILE_SIZE * GRID_WIDTH;
//     int map_height = TILE_SIZE * GRID_HEIGHT;

//     while (!WindowShouldClose()) {
//         float deltaTime = GetFrameTime();
//         // unit manager to update all frames 
//         // instead of putting them all in the main loop
//         catapult.UpdateAnimation(deltaTime);
//         knight.UpdateAnimation(deltaTime);
//         printf("MOUSE (%f, %f)\n", GetMousePosition().x, GetMousePosition().y);
//         resize_logic(GetScreenWidth(), GetScreenHeight());
//         keyboard_control(camera);
//         camera_logic(camera);

//         BeginDrawing();
//         BeginMode2D(camera);
//         ClearBackground(BLACK);
//         draw_background(grass_texture, GRID_WIDTH, GRID_HEIGHT, TILE_SIZE);
//         draw_grid(GRID_WIDTH, GRID_HEIGHT, TILE_SIZE);
//         // draw_grid(50, 10);
//         catapult.Draw();
//         knight.Draw();
//         EndMode2D();
//         EndDrawing();
//     }

//     CloseWindow();
//     return 0;
// }
