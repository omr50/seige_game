/*

    utilities here include things such as
    - structs and enums for frames, movements, etc.
    - functions for loading frames, textures from files.

*/

#ifndef UTILITIES_H
#define UTILITIES_H

#include <raylib.h>
#include <vector>
#include <string>
#include <unordered_map>

#define TOP_BAR_HEIGHT 40
#define TASKBAR_HEIGHT 40
#define TILE_SIZE 128
#define GRID_WIDTH 50
#define GRID_HEIGHT 18

struct Frame {
    Texture2D texture;
    Rectangle source;
};

enum MOVE {EAST, SOUTH_EAST, SOUTH, SOUTH_WEST, WEST, NORTH_WEST, NORTH, NORTH_EAST };

void LoadFramesFromSpriteSheet(std::unordered_map<MOVE, std::vector<Frame>>& all_moves, const char* fileName, int frameWidth, int frameHeight, int framesPerRow, int totalFrames, std::vector<MOVE> moves); 

void LoadCatapultFramesFromSeparateImages(std::unordered_map<MOVE, std::vector<Frame>>& all_moves, const char* prefix, int directions, int framesPerDir, std::vector<MOVE> moves);

void draw_background(Texture2D background_texture, int numSquaresWidth, int numSquaresHeight, int tileSize);
void draw_castle(Texture2D draw_castle);
void camera_logic(Camera2D& camera);
void ClampCameraTarget(Camera2D& camera, float gridWidth, float gridHeight, float tileSize, float screenWidth, float screenHeight, float zoom);

void InitializeWindowWithAspectRatio();

void draw_grid(int numSquaresWidth, int numSquaresHeight, int tileSize);

void keyboard_control(Camera2D& camera);
void resize_logic(int screen_width, int screen_height);
#endif