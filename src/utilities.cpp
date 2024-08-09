#include "../include/utilities.h"
#include <cmath>

#define TOP_BAR_HEIGHT 40

Vector2 Vector2Subtract(Vector2 v1, Vector2 v2) {
    Vector2 result = {v1.x - v2.x, v1.y - v2.y};
    return result;
}

Vector2 Vector2Add(Vector2 v1, Vector2 v2) {
    Vector2 result = {v1.x + v2.x, v1.y + v2.y};
    return result;
}

Vector2 Vector2Scale(Vector2 v, float scale) {
    Vector2 result = {v.x * scale, v.y * scale};
    return result;
}

void LoadFramesFromSpriteSheet(std::unordered_map<MOVE, std::vector<Frame>> &all_moves, const char *fileName, int frameWidth, int frameHeight, int framesPerRow, int totalFrames, std::vector<MOVE> moves) {
    Texture2D spriteSheet = LoadTexture(fileName);
    int j = 0;
    for (auto move : moves) {
        std::vector<Frame> frames;
        for (int i = 0; i < framesPerRow; ++i) {
            Frame frame;
            frame.texture = spriteSheet;
            frame.source = {(float)i * frameWidth, (float)j * frameHeight, (float)frameWidth, (float)frameHeight};
            frames.push_back(frame);
        }
        all_moves[move] = frames;
        j++;
    }
}

void LoadCatapultFramesFromSeparateImages(std::unordered_map<MOVE, std::vector<Frame>> &all_moves, const char *prefix, int directions, int framesPerDir, std::vector<MOVE> moves) {
    int dir = 0;
    for (auto move : moves) {
        std::vector<Frame> frames;
        std::string new_prefix = std::string(prefix) + std::to_string(dir++);
        for (int j = 0; j < framesPerDir; j++) {
            Frame frame;
            std::string num = std::to_string(j);
            int original_size = num.size();
            for (int i = 0; i < (4 - original_size); i++)
                num = "0" + num;

            std::string filename = new_prefix + num + ".png";
            frame.texture = LoadTexture(filename.c_str());
            frame.source = {0.0f, 0.0f, (float)frame.texture.width, (float)frame.texture.height};
            frames.push_back(frame);
        }
        all_moves[move] = frames;
    }
}

void draw_background(Texture2D texture, int numSquaresWidth, int numSquaresHeight, int tileSize) {
    for (int y = 0; y < numSquaresHeight * tileSize; y += texture.height) {
        for (int x = 0; x < numSquaresWidth * tileSize; x += texture.width) {
            DrawTexture(texture, x, y, WHITE);
            char label[16];
            sprintf(label, "(%d, %d)", x, y);
            DrawText(label, x + 5, y + 5, 10, RED);
        }
    }
}

void camera_logic(Camera2D &camera) {
    float zoomSpeed = 0.15f;
    float wheel_move = GetMouseWheelMove();

    if (wheel_move > 0)
        camera.zoom += (zoomSpeed);
    else if (wheel_move < 0)
        camera.zoom -= (zoomSpeed);

    if (GetScreenWidth() / camera.zoom > GRID_WIDTH * TILE_SIZE || GetScreenHeight() / camera.zoom > GRID_HEIGHT * TILE_SIZE) {

    }
    if (camera.zoom > 5.0f) camera.zoom = 5.0f; // Maximum zoom level

    static Vector2 dragStart = {0};
    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
        dragStart = GetMousePosition();
    }
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
        Vector2 mousePos = GetMousePosition();
        Vector2 mouseDelta = Vector2Subtract(dragStart, mousePos);
        dragStart = mousePos;
        camera.target = Vector2Subtract(camera.target, Vector2Scale(mouseDelta, -1.0f / camera.zoom));
    }
    ClampCameraTarget(camera, GRID_WIDTH, GRID_HEIGHT, TILE_SIZE, GetScreenWidth(), GetScreenHeight(), camera.zoom);
}

void ClampCameraTarget(Camera2D& camera, float gridWidth, float gridHeight, float tileSize, float screenWidth, float screenHeight, float zoom) {
    float halfScreenWidth = screenWidth / (2 * zoom);
    float halfScreenHeight = screenHeight / (2 * zoom);
    float mapWidth = gridWidth * tileSize;
    float mapHeight = gridHeight * tileSize;

    float minX = halfScreenWidth;
    float maxX = mapWidth - halfScreenWidth;
    float minY = halfScreenHeight;
    float maxY = mapHeight - halfScreenHeight;

    if (halfScreenWidth > mapWidth / 2) {
        minX = maxX = mapWidth / 2;
    }

    if (halfScreenHeight > mapHeight / 2) {
        minY = maxY = mapHeight / 2;
    }

    if (camera.target.x < minX) camera.target.x = minX;
    if (camera.target.y < minY) camera.target.y = minY;
    if (camera.target.x > maxX) camera.target.x = maxX;
    if (camera.target.y > maxY) camera.target.y = maxY;

    camera.offset.x  = screenWidth / 2;
    camera.offset.y  = screenHeight / 2;

}

void InitializeWindowWithAspectRatio() {
    int screen_width = 1280; // Adjusted to maintain aspect ratio 16:9
    int screen_height = 720;
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screen_width, screen_height, "Aspect Ratio Window");
    SetTargetFPS(60);
}

void draw_grid(int numSquaresWidth, int numSquaresHeight, int tileSize) {
    for (int i = 0; i <= numSquaresHeight; i++) {
        DrawLine(0, i * tileSize, numSquaresWidth * tileSize, i * tileSize, LIGHTGRAY); // Horizontal lines
    }
    for (int i = 0; i <= numSquaresWidth; i++) {
        DrawLine(i * tileSize, 0, i * tileSize, numSquaresHeight * tileSize, LIGHTGRAY); // Vertical lines
    }
}

void keyboard_control(Camera2D &camera) {
    if (IsKeyDown(KEY_W)) camera.target.y -= 10;
    if (IsKeyDown(KEY_S)) camera.target.y += 10;
    if (IsKeyDown(KEY_A)) camera.target.x -= 10;
    if (IsKeyDown(KEY_D)) camera.target.x += 10;
}

void resize_logic(int screen_width, int screen_height) {
    int map_width = TILE_SIZE * GRID_WIDTH;
    int map_height = TILE_SIZE * GRID_HEIGHT;

    if (screen_width > map_width) {
        screen_width = map_width;
        SetWindowSize(screen_width, screen_height);
    }
    if (screen_height > map_height) {
        screen_height = map_height;
        SetWindowSize(screen_width, screen_height);
    }
}

void draw_castle(Texture2D castle) {
    // Define the source rectangle (the whole texture in this case)
    Rectangle source = { 0.0f, 0.0f, (float)castle.width, (float)castle.height };

    // Calculate the scaling factors
    float scale_width = (float)(TILE_SIZE * 2) / castle.width;
    float scale_height = (float)(TILE_SIZE * 2) / castle.height;
    float scale = std::min(scale_width, scale_height);
    float scaledWidth = scale * castle.width;
    float scaledHeight = scale * castle.height;
    printf("scale %f and width height (%f, %f)\n", scale, scaledWidth, scaledHeight);

    // Define the destination rectangles
    Rectangle dest1 = { (float)(TILE_SIZE * 2), (float)(TILE_SIZE * 8), -scaledWidth, -scaledHeight };
    Rectangle dest2 = { (float)(TILE_SIZE * (GRID_WIDTH - 2)), (float)(TILE_SIZE * 8), scaledWidth, scaledHeight };

    // Calculate the origin (center) for rotation
    Vector2 origin = { scaledWidth / 2.0f , scaledHeight / 2.0f };

    // Draw the left castle with a 180-degree rotation
    DrawTexturePro(castle, source, dest1, origin, 0, WHITE);
    // Draw the right castle with no rotation
    DrawTexturePro(castle, source, dest2, origin, 0, WHITE);
}




// #include "../include/utilities.h"
// #include <cmath>

// #define TOP_BAR_HEIGHT 40


// // Function to subtract two Vector2 values
// Vector2 Vector2Subtract(Vector2 v1, Vector2 v2) {
//     Vector2 result = { v1.x - v2.x, v1.y - v2.y };
//     return result;
// }

// // Function to add two Vector2 values
// Vector2 Vector2Add(Vector2 v1, Vector2 v2) {
//     Vector2 result = { v1.x + v2.x, v1.y + v2.y };
//     return result;
// }

// // Function to scale a Vector2 by a scalar value
// Vector2 Vector2Scale(Vector2 v, float scale) {
//     Vector2 result = { v.x * scale, v.y * scale };
//     return result;
// }


// // load frames based on the input vector of moves. So each row should be frames for one move, and that is value mapped to enum key. 
// void LoadFramesFromSpriteSheet(std::unordered_map<MOVE, std::vector<Frame>>& all_moves, const char* fileName, int frameWidth, int frameHeight, int framesPerRow, int totalFrames, std::vector<MOVE> moves) {
//     Texture2D spriteSheet = LoadTexture(fileName);
//     int j = 0;
//     for (auto move : moves) {
//         std::vector<Frame> frames;
//         for (int i = 0; i < framesPerRow; ++i) {
//             Frame frame;
//             frame.texture = spriteSheet;
//             frame.source = { (float)i * frameWidth, (float)j * frameHeight, (float)frameWidth, (float)frameHeight };
//             // printf("Loading frame for move %d: X: %f Y:%f\n", move, frame.source.x, frame.source.y);
//             frames.push_back(frame);
//         }
//         // add the new move frames to the map.
//         all_moves[move] = frames;
//         j++;
//     }
// }

// // If separate images we load each sprite sheet individually and use that along with the standard rect size.
// // ex: 31 frames for each direction, 8 directions so we can specify inner and outer.
// void LoadCatapultFramesFromSeparateImages(std::unordered_map<MOVE, std::vector<Frame>>& all_moves, const char* prefix, int directions, int framesPerDir, std::vector<MOVE> moves) {
//     int dir = 0;
//     for (auto move : moves) {
//         std::vector<Frame> frames;
//         std::string new_prefix = std::string(prefix) +  std::to_string(dir++); 
//         for (int j = 0; j < framesPerDir; j++) {
//             Frame frame;
//             std::string num = std::to_string(j);
//             int original_size = num.size();
//             // padding so it has 4 digits always
//             for (int i = 0; i < (4 - original_size); i++)
//                 num = "0" + num;

//             std::string filename = new_prefix + num + ".png";
//             frame.texture = LoadTexture(filename.c_str());
//             frame.source = { 0.0f, 0.0f, (float)frame.texture.width, (float)frame.texture.height };
//             frames.push_back(frame);
//         }
//         // add the new move frames to the map.
//         all_moves[move] = frames;
//     }

// }

// void draw_background(Texture2D texture, int numSquaresWidth, int numSquaresHeight, int tileSize) {
//     for (int y = 0; y < numSquaresHeight * tileSize; y += texture.height) {
//         for (int x = 0; x < numSquaresWidth * tileSize; x += texture.width) {
//             // printf("TEXTURE (%d, %d)\n", x, y);
//             DrawTexture(texture, x, y, WHITE);
//             char label[16];
//             sprintf(label, "(%d, %d)", x, y);
//             DrawText(label, x + 5, y + 5, 10, RED);
//         }
//     }
// }

// void camera_logic(Camera2D& camera) {
//     float zoomSpeed = 0.15f;
//     float wheel_move = GetMouseWheelMove();

//     if (wheel_move > 0)
//         camera.zoom += (zoomSpeed);
//     else if (wheel_move < 0)
//         camera.zoom -= (zoomSpeed);

//     if (GetScreenWidth() / camera.zoom > GRID_WIDTH * TILE_SIZE || GetScreenHeight() / camera.zoom > GRID_HEIGHT * TILE_SIZE) {

//     }
//     if (camera.zoom > 5.0f) camera.zoom = 5.0f; // Maximum zoom level
//     // printf("Current Zoom: %f\n", camera.zoom);

//     // Dragging (panning) the camera
//     static Vector2 dragStart = { 0 };
//     if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
//         dragStart = GetMousePosition();
//     }
//     if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
//         Vector2 mousePos = GetMousePosition();
//         Vector2 mouseDelta = Vector2Subtract(dragStart, mousePos);
//         dragStart = mousePos;
//         camera.target = Vector2Subtract(camera.target, Vector2Scale(mouseDelta, -1.0f / camera.zoom));
//     }
//     printf("CAM ZOOM %f\n", camera.zoom);
//     ClampCameraTarget(camera.target, GRID_WIDTH, GRID_HEIGHT, TILE_SIZE, GetScreenWidth(), GetScreenHeight(), camera.zoom);
// }

// void ClampCameraTarget(Vector2 &target, float gridWidth, float gridHeight, float tileSize, float screenWidth, float screenHeight, float zoom) {
//     float halfScreenWidth = screenWidth / (2 * zoom);
//     float halfScreenHeight = screenHeight / (2 * zoom);
//     printf("SCREEN (%f, %f)\n", halfScreenWidth, halfScreenHeight);
//     float mapWidth = gridWidth * tileSize;
//     float mapHeight = gridHeight * tileSize;

//     printf("map (%f, %f)\n", mapWidth, mapHeight);
//     printf("Camera (%f, %f)\n", target.x, target.y);
//     printf("\n");
//     float minX = halfScreenWidth;
//     float maxX = mapWidth - halfScreenWidth;
//     float minY = halfScreenHeight;
//     float maxY = mapHeight - halfScreenHeight;

//     if (halfScreenWidth > mapWidth / 2) {
//         minX = maxX = mapWidth / 2;
//     }

//     if (halfScreenHeight > mapHeight / 2) {
//         minY = maxY = mapHeight / 2;
//     }

//     if (target.x < minX) target.x = minX;
//     if (target.y < minY) target.y = minY;
//     if (target.x > maxX) target.x = maxX;
//     if (target.y > maxY) target.y = maxY;
// }



// void InitializeWindowWithAspectRatio() {
//     // Get screen dimensions
//     int screen_width = 128 * 8;
//     int screen_height = 128 * 4;
//     SetConfigFlags(FLAG_WINDOW_RESIZABLE);
//     // Initialize the window with calculated dimensions
//     InitWindow(screen_width, screen_height, "Aspect Ratio Window");

//     // Set the target FPS
//     SetTargetFPS(60);
// }

// void draw_grid(int numSquaresWidth, int numSquaresHeight, int tileSize) {
//     for (int i = 0; i <= numSquaresHeight; i++) {
//         DrawLine(0, i * tileSize, numSquaresWidth * tileSize, i * tileSize, LIGHTGRAY); // Horizontal lines
//     }
//     for (int i = 0; i <= numSquaresWidth; i++) {
//         DrawLine(i * tileSize, 0, i * tileSize, numSquaresHeight * tileSize, LIGHTGRAY); // Vertical lines
//     }
// }

// void keyboard_control(Camera2D& camera) {
//     if (IsKeyDown(KEY_W)) camera.target.y -= 10;
//     if (IsKeyDown(KEY_S)) camera.target.y += 10;
//     if (IsKeyDown(KEY_A)) camera.target.x -= 10;
//     if (IsKeyDown(KEY_D)) camera.target.x += 10;
// }

// void resize_logic(int screen_width, int screen_height) {
//     int map_width = TILE_SIZE * GRID_WIDTH;
//     int map_height = TILE_SIZE * GRID_HEIGHT;

//     if (screen_width > map_width) {
//         screen_width = map_width;
//         SetWindowSize(screen_width, screen_height);
//     }
//     if (screen_height > map_height) {
//         screen_height = map_height;
//         SetWindowSize(screen_width, screen_height);
//     }
//     // maintain aspect ratio
//     if (screen_width / screen_height != (GRID_WIDTH / GRID_HEIGHT)) {
//         printf("WORKING?\n");
//         screen_height = (screen_width / (int)GRID_WIDTH) * GRID_HEIGHT;
//     }
        
//     SetWindowSize(screen_width, screen_height);
// }


// // instead of capping the zoom at some arbitrary value, cap it at the
// // point where the zoom * screen width or screen height will go past
// // the map length.