#include <raylib.h>
#include <raymath.h>

#include <algorithm>
#include <array>
#include <cmath>
#include <iomanip>
#include <random>
#include <sstream>
#include <string>
#include <vector>

// ============================================================
// CONFIG
// ============================================================

constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 720;

constexpr float PI_F = 3.14159265358979323846f;

constexpr float GRAVITY = 22.0f;

constexpr float WALK_SPEED = 8.0f;
constexpr float SPRINT_SPEED = 14.0f;

constexpr float GROUND_ACCEL = 44.0f;
constexpr float AIR_ACCEL = 22.0f;
constexpr float FRICTION = 15.0f;

constexpr float JUMP_SPEED = 10.5f;

constexpr float PLAYER_HEIGHT = 1.8f;
constexpr float PLAYER_RADIUS = 0.35f;

constexpr float CAMERA_DISTANCE = 11.0f;
constexpr float CAMERA_HEIGHT = 3.5f;
constexpr float CAMERA_SENSITIVITY = 0.004f;

constexpr float LEVEL_HEIGHT = 3.4f;
constexpr float PLATFORM_THICKNESS = 0.8f;

constexpr int MAX_ISLANDS = 28;
constexpr int PLANT_COUNT = 8;

// ============================================================
// RANDOM
// ============================================================

std::mt19937 rng(std::random_device{}());

float RandomFloat(float min, float max)
{
    std::uniform_real_distribution<float> dist(min, max);
    return dist(rng);
}

int RandomInt(int min, int max)
{
    std::uniform_int_distribution<int> dist(min, max);
    return dist(rng);
}

// ============================================================
// ENUMS
// ============================================================

enum class IslandTheme
{
    Grassland,
    Forest,
    Desert,
    Ice,
    Swamp,
    Crystal,
    Hell,
    Golden,
    Liminal,
    Void
};

enum class PlantType
{
    Sunleaf,
    Berry,
    Cactus,
    Mushroom,
    Inferno,
    GoldenFlower,
    CrystalBloom,
    VoidOrchid
};

enum class EnchantType
{
    None,
    Shiny,
    Giant,
    Blessed,
    Infernal,
    Jackpot
};

enum class DropType
{
    Seed,
    Cash,
    Growth
};

enum class EventType
{
    None,
    GoldenRain,
    SuperGrowth,
    EnchantStorm,
    TreasureRain,
    MarketBoom,
    LiminalBloom
};

enum class AdminFocus
{
    None,
    Password
};

// ============================================================
// THEME INFO
// ============================================================

struct ThemeInfo
{
    const char* name;
    const char* rarity;

    double valueMultiplier;
    float growthMultiplier;
    float enchantChance;

    Color baseColor;
    Color accentColor;
};

ThemeInfo GetThemeInfo(IslandTheme theme)
{
    switch (theme)
    {
        case IslandTheme::Grassland:
            return {
                "GRASSLAND",
                "COMMON",
                1.0,
                1.0f,
                0.03f,
                Color{62, 77, 69, 255},
                Color{125, 205, 110, 255}
            };

        case IslandTheme::Forest:
            return {
                "FOREST",
                "UNCOMMON",
                5.0,
                1.10f,
                0.06f,
                Color{39, 65, 53, 255},
                Color{85, 175, 100, 255}
            };

        case IslandTheme::Desert:
            return {
                "DESERT",
                "RARE",
                20.0,
                0.95f,
                0.08f,
                Color{102, 78, 55, 255},
                Color{230, 175, 80, 255}
            };

        case IslandTheme::Ice:
            return {
                "ICE",
                "EPIC",
                80.0,
                1.20f,
                0.11f,
                Color{63, 89, 105, 255},
                Color{160, 230, 255, 255}
            };

        case IslandTheme::Swamp:
            return {
                "SWAMP",
                "EPIC",
                350.0,
                1.35f,
                0.16f,
                Color{43, 56, 48, 255},
                Color{105, 185, 110, 255}
            };

        case IslandTheme::Crystal:
            return {
                "CRYSTAL",
                "LEGENDARY",
                10000.0,
                1.45f,
                0.22f,
                Color{47, 44, 70, 255},
                Color{180, 105, 255, 255}
            };

        case IslandTheme::Hell:
            return {
                "HELL LAND",
                "LEGENDARY",
                200000.0,
                2.0f,
                0.38f,
                Color{70, 23, 26, 255},
                Color{255, 75, 40, 255}
            };

        case IslandTheme::Golden:
            return {
                "GOLDEN",
                "MYTHIC",
                1000000.0,
                1.35f,
                0.28f,
                Color{83, 69, 31, 255},
                Color{255, 215, 60, 255}
            };

        case IslandTheme::Liminal:
            return {
                "LIMINAL",
                "MYTHIC",
                250000.0,
                1.15f,
                0.33f,
                Color{76, 75, 67, 255},
                Color{205, 194, 130, 255}
            };

        case IslandTheme::Void:
            return {
                "VOID",
                "TRANSCENDENT",
                5000000.0,
                1.20f,
                0.50f,
                Color{27, 22, 39, 255},
                Color{182, 88, 255, 255}
            };
    }

    return GetThemeInfo(IslandTheme::Grassland);
}

// ============================================================
// PLANT INFO
// ============================================================

struct PlantInfo
{
    const char* name;

    double baseValue;
    double seedPrice;

    float growTime;

    Color color;
};

PlantInfo GetPlantInfo(PlantType plant)
{
    switch (plant)
    {
        case PlantType::Sunleaf:
            return {
                "Sunleaf",
                4.0,
                2.0,
                10.0f,
                Color{90, 220, 90, 255}
            };

        case PlantType::Berry:
            return {
                "Berry Bush",
                14.0,
                7.0,
                14.0f,
                Color{235, 70, 120, 255}
            };

        case PlantType::Cactus:
            return {
                "Cactus",
                40.0,
                20.0,
                18.0f,
                Color{90, 185, 100, 255}
            };

        case PlantType::Mushroom:
            return {
                "Glow Mushroom",
                200.0,
                100.0,
                22.0f,
                Color{190, 90, 255, 255}
            };

        case PlantType::Inferno:
            return {
                "Inferno Flower",
                3000.0,
                1500.0,
                27.0f,
                Color{255, 85, 25, 255}
            };

        case PlantType::GoldenFlower:
            return {
                "Golden Flower",
                120000.0,
                60000.0,
                34.0f,
                Color{255, 215, 55, 255}
            };

        case PlantType::CrystalBloom:
            return {
                "Crystal Bloom",
                600000.0,
                300000.0,
                42.0f,
                Color{100, 235, 255, 255}
            };

        case PlantType::VoidOrchid:
            return {
                "Void Orchid",
                2500000.0,
                1250000.0,
                55.0f,
                Color{185, 85, 255, 255}
            };
    }

    return GetPlantInfo(PlantType::Sunleaf);
}

// ============================================================
// ENCHANT
// ============================================================

double GetEnchantMultiplier(EnchantType enchant)
{
    switch (enchant)
    {
        case EnchantType::None:
            return 1.0;

        case EnchantType::Shiny:
            return 2.0;

        case EnchantType::Giant:
            return 4.0;

        case EnchantType::Blessed:
            return 8.0;

        case EnchantType::Infernal:
            return 20.0;

        case EnchantType::Jackpot:
            return 100.0;
    }

    return 1.0;
}

const char* GetEnchantName(EnchantType enchant)
{
    switch (enchant)
    {
        case EnchantType::None:
            return "NONE";

        case EnchantType::Shiny:
            return "SHINY";

        case EnchantType::Giant:
            return "GIANT";

        case EnchantType::Blessed:
            return "BLESSED";

        case EnchantType::Infernal:
            return "INFERNAL";

        case EnchantType::Jackpot:
            return "JACKPOT";
    }

    return "NONE";
}

// ============================================================
// RANDOM THEME
// ============================================================

IslandTheme RandomTheme()
{
    float r =
        RandomFloat(
            0.0f,
            100.0f
        );

    if (r < 40.0f)
        return IslandTheme::Grassland;

    if (r < 58.0f)
        return IslandTheme::Forest;

    if (r < 71.0f)
        return IslandTheme::Desert;

    if (r < 79.0f)
        return IslandTheme::Ice;

    if (r < 86.0f)
        return IslandTheme::Swamp;

    if (r < 92.0f)
        return IslandTheme::Crystal;

    if (r < 96.5f)
        return IslandTheme::Hell;

    if (r < 98.3f)
        return IslandTheme::Golden;

    if (r < 99.4f)
        return IslandTheme::Liminal;

    return IslandTheme::Void;
}

// ============================================================
// HELPERS
// ============================================================

float Approach(
    float current,
    float target,
    float amount
)
{
    if (current < target)
        return std::min(
            current + amount,
            target
        );

    return std::max(
        current - amount,
        target
    );
}

std::string FormatMoney(double value)
{
    std::ostringstream out;

    if (value < 1000.0)
    {
        out
            << "$"
            << std::fixed
            << std::setprecision(0)
            << value;

        return out.str();
    }

    if (value < 1000000.0)
    {
        out
            << "$"
            << std::fixed
            << std::setprecision(1)
            << value / 1000.0
            << "K";

        return out.str();
    }

    if (value < 1000000000.0)
    {
        out
            << "$"
            << std::fixed
            << std::setprecision(2)
            << value / 1000000.0
            << "M";

        return out.str();
    }

    if (value < 1000000000000.0)
    {
        out
            << "$"
            << std::fixed
            << std::setprecision(2)
            << value / 1000000000.0
            << "B";

        return out.str();
    }

    if (value < 1000000000000000.0)
    {
        out
            << "$"
            << std::fixed
            << std::setprecision(2)
            << value / 1000000000000.0
            << "T";

        return out.str();
    }

    if (value < 1000000000000000000.0)
    {
        out
            << "$"
            << std::fixed
            << std::setprecision(2)
            << value / 1000000000000000.0
            << "Q";

        return out.str();
    }

    out
        << "$"
        << std::scientific
        << std::setprecision(2)
        << value;

    return out.str();
}

// ============================================================
// PLATFORM
// ============================================================

struct PlatformPiece
{
    Vector3 center;
    Vector3 size;

    bool accent = false;
    bool stair = false;
    bool light = false;
};

// ============================================================
// ISLAND
// ============================================================

struct Island
{
    Vector3 position;

    float width = 12.0f;
    float depth = 12.0f;

    int levels = 1;

    IslandTheme theme =
        IslandTheme::Grassland;

    bool owned = false;

    double price = 0.0;

    float luck = 1.0f;

    int capacity = 20;

    std::vector<PlatformPiece> pieces;
};

// ============================================================
// BRIDGE
// ============================================================

struct BridgePiece
{
    Vector3 center;
    Vector3 size;
};

// ============================================================
// PLANT
// ============================================================

struct Plant
{
    Vector3 position;

    PlantType type;

    EnchantType enchant;

    int islandIndex;

    float age = 0.0f;

    float growTime = 10.0f;
};

// ============================================================
// DROP
// ============================================================

struct Drop
{
    Vector3 position;

    DropType type;

    PlantType seedType =
        PlantType::Sunleaf;

    double cashValue = 0.0;

    bool active = true;
};

// ============================================================
// EVENT
// ============================================================

struct EventState
{
    EventType type =
        EventType::None;

    float remaining = 0.0f;

    double saleMultiplier =
        1.0;

    float growthMultiplier =
        1.0f;

    float enchantMultiplier =
        1.0f;

    std::string name;

    std::string description;
};

// ============================================================
// PLAYER
// ============================================================

struct Player
{
    Vector3 position =
    {
        0,
        1,
        0
    };

    Vector3 velocity =
    {
        0,
        0,
        0
    };

    float rotationYaw =
        0.0f;

    bool grounded =
        false;

    float coyoteTimer =
        0.0f;

    float jumpBufferTimer =
        0.0f;

    int airJumps =
        1;

    float animationTime =
        0.0f;

    void Update(
        float dt,
        float cameraYaw,
        const std::vector<Island>& islands,
        const std::vector<BridgePiece>& bridges
    )
    {
        // ====================================================
        // CAMERA RELATIVE DIRECTIONS
        // ====================================================

        Vector3 forward =
        {
            std::sin(cameraYaw),
            0.0f,
            std::cos(cameraYaw)
        };

        Vector3 right =
        {
            std::cos(cameraYaw),
            0.0f,
            -std::sin(cameraYaw)
        };

        Vector3 input =
        {
            0.0f,
            0.0f,
            0.0f
        };

        if (IsKeyDown(KEY_W))
        {
            input =
                Vector3Add(
                    input,
                    forward
                );
        }

        if (IsKeyDown(KEY_S))
        {
            input =
                Vector3Subtract(
                    input,
                    forward
                );
        }

        if (IsKeyDown(KEY_A))
        {
            input =
                Vector3Add(
                    input,
                    right
                );
        }

        if (IsKeyDown(KEY_D))
        {
            input =
                Vector3Subtract(
                    input,
                    right
                );
        }

        bool moving =
            Vector3Length(input) > 0.001f;

        if (moving)
        {
            input =
                Vector3Normalize(input);

            rotationYaw =
                std::atan2(
                    input.x,
                    input.z
                );

            animationTime +=
                dt *
                (
                    IsKeyDown(KEY_LEFT_SHIFT) ||
                    IsKeyDown(KEY_RIGHT_SHIFT)
                    ? 13.0f
                    : 9.0f
                );
        }

        bool sprint =
            IsKeyDown(KEY_LEFT_SHIFT) ||
            IsKeyDown(KEY_RIGHT_SHIFT);

        float maxSpeed =
            sprint
            ? SPRINT_SPEED
            : WALK_SPEED;

        Vector3 wantedVelocity =
            Vector3Scale(
                input,
                maxSpeed
            );

        // ====================================================
        // HORIZONTAL ACCELERATION
        // ====================================================

        float acceleration =
            grounded
            ? GROUND_ACCEL
            : AIR_ACCEL;

        velocity.x =
            Approach(
                velocity.x,
                wantedVelocity.x,
                acceleration * dt
            );

        velocity.z =
            Approach(
                velocity.z,
                wantedVelocity.z,
                acceleration * dt
            );

        if (!moving && grounded)
        {
            velocity.x =
                Approach(
                    velocity.x,
                    0.0f,
                    FRICTION * dt
                );

            velocity.z =
                Approach(
                    velocity.z,
                    0.0f,
                    FRICTION * dt
                );
        }

        // ====================================================
        // JUMP BUFFER
        // ====================================================

        if (IsKeyPressed(KEY_SPACE))
        {
            jumpBufferTimer =
                0.15f;
        }
        else
        {
            jumpBufferTimer -=
                dt;
        }

        if (grounded)
        {
            coyoteTimer =
                0.12f;

            airJumps =
                1;
        }
        else
        {
            coyoteTimer -=
                dt;
        }

        // ====================================================
        // JUMP
        // ====================================================

        if (jumpBufferTimer > 0.0f)
        {
            if (
                grounded ||
                coyoteTimer > 0.0f
            )
            {
                velocity.y =
                    JUMP_SPEED;

                grounded =
                    false;

                coyoteTimer =
                    0.0f;

                jumpBufferTimer =
                    0.0f;
            }
            else if (airJumps > 0)
            {
                velocity.y =
                    JUMP_SPEED * 0.90f;

                airJumps--;

                jumpBufferTimer =
                    0.0f;
            }
        }

        // ====================================================
        // GRAVITY
        // ====================================================

        velocity.y -=
            GRAVITY * dt;

        // ====================================================
        // MOVE
        // ====================================================

        position.x +=
            velocity.x * dt;

        position.y +=
            velocity.y * dt;

        position.z +=
            velocity.z * dt;

        // ====================================================
        // COLLISION
        // ====================================================

        grounded =
            false;

        float bestTop =
            -100000.0f;

        // ----------------------------------------------------
        // ISLAND PLATFORMS
        // ----------------------------------------------------

        for (
            const Island& island :
            islands
        )
        {
            for (
                const PlatformPiece& piece :
                island.pieces
            )
            {
                float left =
                    piece.center.x -
                    piece.size.x * 0.5f -
                    PLAYER_RADIUS;

                float rightEdge =
                    piece.center.x +
                    piece.size.x * 0.5f +
                    PLAYER_RADIUS;

                float front =
                    piece.center.z -
                    piece.size.z * 0.5f -
                    PLAYER_RADIUS;

                float back =
                    piece.center.z +
                    piece.size.z * 0.5f +
                    PLAYER_RADIUS;

                float top =
                    piece.center.y +
                    piece.size.y * 0.5f;

                if (
                    position.x >= left &&
                    position.x <= rightEdge &&
                    position.z >= front &&
                    position.z <= back &&
                    velocity.y <= 0.0f &&
                    position.y <=
                        top + 0.30f &&
                    position.y >=
                        top - 1.25f
                )
                {
                    if (top > bestTop)
                    {
                        bestTop =
                            top;
                    }
                }
            }
        }

        // ----------------------------------------------------
        // BRIDGES
        // ----------------------------------------------------

        for (
            const BridgePiece& bridge :
            bridges
        )
        {
            float left =
                bridge.center.x -
                bridge.size.x * 0.5f -
                PLAYER_RADIUS;

            float rightEdge =
                bridge.center.x +
                bridge.size.x * 0.5f +
                PLAYER_RADIUS;

            float front =
                bridge.center.z -
                bridge.size.z * 0.5f -
                PLAYER_RADIUS;

            float back =
                bridge.center.z +
                bridge.size.z * 0.5f +
                PLAYER_RADIUS;

            float top =
                bridge.center.y +
                bridge.size.y * 0.5f;

            if (
                position.x >= left &&
                position.x <= rightEdge &&
                position.z >= front &&
                position.z <= back &&
                velocity.y <= 0.0f &&
                position.y <=
                    top + 0.30f &&
                position.y >=
                    top - 1.25f
            )
            {
                if (top > bestTop)
                {
                    bestTop =
                        top;
                }
            }
        }

        if (
            bestTop >
            -50000.0f
        )
        {
            position.y =
                bestTop;

            velocity.y =
                0.0f;

            grounded =
                true;
        }

        // ====================================================
        // FALL RESET
        // ====================================================

        if (position.y < -30.0f)
        {
            position =
            {
                0,
                1,
                0
            };

            velocity =
            {
                0,
                0,
                0
            };

            grounded =
                false;
        }
    }

    void Draw() const
    {
        float bob =
            grounded
            ? std::sin(
                animationTime
            ) * 0.04f
            : 0.0f;

        Vector3 body =
        {
            position.x,
            position.y +
                0.90f +
                bob,
            position.z
        };

        // ----------------------------------------------------
        // BODY
        // ----------------------------------------------------

        DrawCubeV(
            body,
            {
                0.72f,
                1.75f,
                0.58f
            },
            BLUE
        );

        // ----------------------------------------------------
        // BACKPACK
        // ----------------------------------------------------

        Vector3 back =
        {
            -std::sin(rotationYaw),
            0.0f,
            -std::cos(rotationYaw)
        };

        DrawCubeV(
            Vector3Add(
                {
                    position.x,
                    position.y +
                        1.0f +
                        bob,
                    position.z
                },
                Vector3Scale(
                    back,
                    0.45f
                )
            ),
            {
                0.46f,
                0.78f,
                0.24f
            },
            DARKBLUE
        );

        // ----------------------------------------------------
        // HEAD
        // ----------------------------------------------------

        Vector3 head =
        {
            position.x,
            position.y +
                1.92f +
                bob,
            position.z
        };

        DrawSphere(
            head,
            0.46f,
            SKYBLUE
        );

        // ----------------------------------------------------
        // VISOR
        // ----------------------------------------------------

        Vector3 front =
        {
            std::sin(rotationYaw),
            0.0f,
            std::cos(rotationYaw)
        };

        DrawCubeV(
            Vector3Add(
                head,
                Vector3Scale(
                    front,
                    0.38f
                )
            ),
            {
                0.48f,
                0.18f,
                0.06f
            },
            BLACK
        );

        // ----------------------------------------------------
        // FRONT INDICATOR
        // ----------------------------------------------------

        DrawLine3D(
            head,
            Vector3Add(
                head,
                Vector3Scale(
                    front,
                    0.72f
                )
            ),
            SKYBLUE
        );

        // ----------------------------------------------------
        // SHADOW
        // ----------------------------------------------------

        DrawCircle3D(
            {
                position.x,
                position.y +
                    0.025f,
                position.z
            },
            0.48f,
            {
                1,
                0,
                0
            },
            90.0f,
            Fade(
                BLACK,
                0.32f
            )
        );
    }
};

// ============================================================
// CAMERA
// ============================================================

struct GameCamera
{
    float yaw =
        0.0f;

    float pitch =
        0.35f;

    Camera3D camera =
    {};

    void Update(
        Vector3 target
    )
    {
        if (
            IsMouseButtonDown(
                MOUSE_BUTTON_RIGHT
            )
        )
        {
            Vector2 delta =
                GetMouseDelta();

            yaw -=
                delta.x *
                CAMERA_SENSITIVITY;

            pitch -=
                delta.y *
                CAMERA_SENSITIVITY;

            pitch =
                std::clamp(
                    pitch,
                    -1.0f,
                    1.0f
                );
        }

        float horizontal =
            std::cos(
                pitch
            ) *
            CAMERA_DISTANCE;

        // ----------------------------------------------------
        // Orbit camera.
        //
        // pitch > 0 means we are looking upward,
        // therefore the camera moves underneath the target.
        // ----------------------------------------------------

        camera.position =
        {
            target.x -
                std::sin(yaw) *
                horizontal,

            target.y +
                CAMERA_HEIGHT -
                std::sin(pitch) *
                CAMERA_DISTANCE,

            target.z -
                std::cos(yaw) *
                horizontal
        };

        camera.target =
            target;

        camera.up =
        {
            0,
            1,
            0
        };

        camera.fovy =
            65.0f;

        camera.projection =
            CAMERA_PERSPECTIVE;
    }
};

// ============================================================
// GENERATE ISLAND
// ============================================================

Island GenerateIsland(
    Vector3 position,
    bool startingIsland
)
{
    Island island;

    island.position =
        position;

    if (startingIsland)
    {
        island.theme =
            IslandTheme::Grassland;

        island.levels =
            3;

        island.width =
            19.0f;

        island.depth =
            19.0f;

        island.owned =
            true;

        island.price =
            0.0;

        island.luck =
            1.6f;
    }
    else
    {
        island.theme =
            RandomTheme();

        bool gigantic =
            RandomFloat(
                0.0f,
                1.0f
            ) <
            0.055f;

        if (gigantic)
        {
            island.levels =
                RandomInt(
                    40,
                    100
                );

            island.width =
                RandomFloat(
                    18.0f,
                    32.0f
                );

            island.depth =
                RandomFloat(
                    18.0f,
                    32.0f
                );
        }
        else
        {
            island.levels =
                RandomInt(
                    4,
                    16
                );

            island.width =
                RandomFloat(
                    11.0f,
                    23.0f
                );

            island.depth =
                RandomFloat(
                    11.0f,
                    23.0f
                );
        }
    }

    // ========================================================
    // GENERATE LEVELS
    // ========================================================

    Vector2 drift =
    {
        0.0f,
        0.0f
    };

    for (
        int level = 0;
        level < island.levels;
        level++
    )
    {
        drift.x +=
            RandomFloat(
                -2.0f,
                2.0f
            );

        drift.y +=
            RandomFloat(
                -2.0f,
                2.0f
            );

        drift.x =
            std::clamp(
                drift.x,
                -island.width * 0.28f,
                island.width * 0.28f
            );

        drift.y =
            std::clamp(
                drift.y,
                -island.depth * 0.28f,
                island.depth * 0.28f
            );

        float y =
            island.position.y +
            level *
            LEVEL_HEIGHT;

        PlatformPiece main;

        main.center =
        {
            island.position.x +
                drift.x,

            y +
                PLATFORM_THICKNESS *
                0.5f,

            island.position.z +
                drift.y
        };

        float sizeVariation =
            RandomFloat(
                0.78f,
                1.0f
            );

        main.size =
        {
            island.width *
                sizeVariation,

            PLATFORM_THICKNESS,

            island.depth *
                RandomFloat(
                    0.76f,
                    1.0f
                )
        };

        main.accent =
            level % 2 == 0;

        main.light =
            level % 3 == 0;

        island.pieces.push_back(
            main
        );

        // ====================================================
        // SECONDARY PLATFORM PIECES
        // ====================================================

        int sidePieces =
            island.levels > 35
            ? RandomInt(0, 1)
            : RandomInt(1, 3);

        for (
            int i = 0;
            i < sidePieces;
            i++
        )
        {
            PlatformPiece side;

            float sideWidth =
                RandomFloat(
                    3.5f,
                    island.width * 0.45f
                );

            float sideDepth =
                RandomFloat(
                    3.5f,
                    island.depth * 0.45f
                );

            float offsetX =
                RandomFloat(
                    -main.size.x * 0.28f,
                    main.size.x * 0.28f
                );

            float offsetZ =
                RandomFloat(
                    -main.size.z * 0.28f,
                    main.size.z * 0.28f
                );

            side.center =
            {
                main.center.x +
                    offsetX,

                main.center.y,

                main.center.z +
                    offsetZ
            };

            side.size =
            {
                sideWidth,
                PLATFORM_THICKNESS,
                sideDepth
            };

            side.accent =
                RandomFloat(
                    0.0f,
                    1.0f
                ) < 0.65f;

            side.light =
                RandomFloat(
                    0.0f,
                    1.0f
                ) < 0.25f;

            island.pieces.push_back(
                side
            );
        }

        // ====================================================
        // STAIRS
        // ====================================================

        if (
            level <
            island.levels - 1
        )
        {
            PlatformPiece current =
                main;

            float nextX =
                island.position.x +
                drift.x +
                RandomFloat(
                    -2.0f,
                    2.0f
                );

            float nextZ =
                island.position.z +
                drift.y +
                RandomFloat(
                    -2.0f,
                    2.0f
                );

            Vector3 start =
            {
                current.center.x,
                current.center.y +
                    current.size.y *
                    0.5f,
                current.center.z
            };

            Vector3 end =
            {
                nextX,
                current.center.y +
                    LEVEL_HEIGHT,
                nextZ
            };

            constexpr int stairCount =
                9;

            float stepHeight =
                LEVEL_HEIGHT /
                static_cast<float>(
                    stairCount
                );

            for (
                int s = 1;
                s <= stairCount;
                s++
            )
            {
                float t =
                    static_cast<float>(s) /
                    static_cast<float>(stairCount);

                float x =
                    Lerp(
                        start.x,
                        end.x,
                        t
                    );

                float z =
                    Lerp(
                        start.z,
                        end.z,
                        t
                    );

                float topY =
                    start.y +
                    stepHeight *
                    static_cast<float>(s);

                PlatformPiece stair;

                stair.center =
                {
                    x,
                    topY -
                        stepHeight *
                        0.5f,
                    z
                };

                stair.size =
                {
                    2.8f,
                    stepHeight,
                    2.8f
                };

                stair.stair =
                    true;

                stair.accent =
                    true;

                stair.light =
                    s % 2 == 0;

                island.pieces.push_back(
                    stair
                );
            }
        }
    }

    // ========================================================
    // LUCK
    // ========================================================

    float areaFactor =
        (
            island.width *
            island.depth
        ) /
        180.0f;

    float levelFactor =
        std::log(
            static_cast<float>(
                std::max(
                    island.levels,
                    1
                )
            )
        ) *
        0.06f;

    island.luck =
        0.75f +
        areaFactor +
        levelFactor;

    if (
        island.levels >=
        40
    )
    {
        island.luck +=
            0.6f;
    }

    // ========================================================
    // PLANT CAPACITY
    // ========================================================

    island.capacity =
        std::max(
            10,
            static_cast<int>(
                island.pieces.size() *
                1.8f
            )
        );

    // ========================================================
    // PRICE
    // ========================================================

    ThemeInfo theme =
        GetThemeInfo(
            island.theme
        );

    double rarityFactor =
        std::pow(
            theme.valueMultiplier,
            0.42
        );

    double sizeFactor =
        0.8 +
        (
            island.width *
            island.depth
        ) /
        130.0 +
        island.levels /
        55.0;

    island.price =
        300.0 *
        rarityFactor *
        sizeFactor *
        RandomFloat(
            0.88f,
            1.18f
        );

    if (
        island.price <
        300.0
    )
    {
        island.price =
            300.0;
    }

    return island;
}

// ============================================================
// GENERATE WORLD
// ============================================================

std::vector<Island> GenerateWorld()
{
    std::vector<Island> islands;

    islands.push_back(
        GenerateIsland(
            {
                0,
                0,
                0
            },
            true
        )
    );

    int attempts =
        0;

    while (
        islands.size() <
        MAX_ISLANDS &&
        attempts <
        20000
    )
    {
        attempts++;

        float angle =
            RandomFloat(
                0,
                PI_F * 2.0f
            );

        float distance =
            RandomFloat(
                25.0f,
                120.0f
            );

        Vector3 position =
        {
            std::cos(angle) *
                distance,

            0,

            std::sin(angle) *
                distance
        };

        Island candidate =
            GenerateIsland(
                position,
                false
            );

        float candidateRadius =
            std::sqrt(
                candidate.width *
                candidate.width +
                candidate.depth *
                candidate.depth
            ) *
            0.58f;

        bool valid =
            true;

        for (
            const Island& other :
            islands
        )
        {
            float otherRadius =
                std::sqrt(
                    other.width *
                    other.width +
                    other.depth *
                    other.depth
                ) *
                0.58f;

            float distanceBetween =
                Vector2Distance(
                    {
                        position.x,
                        position.z
                    },
                    {
                        other.position.x,
                        other.position.z
                    }
                );

            if (
                distanceBetween <
                candidateRadius +
                otherRadius +
                8.0f
            )
            {
                valid =
                    false;

                break;
            }
        }

        if (valid)
        {
            islands.push_back(
                candidate
            );
        }
    }

    return islands;
}

// ============================================================
// GENERATE BRIDGES
// ============================================================

std::vector<BridgePiece> GenerateBridges(
    const std::vector<Island>& islands
)
{
    std::vector<BridgePiece> bridges;

    for (
        int i = 1;
        i < static_cast<int>(islands.size());
        i++
    )
    {
        int closest =
            -1;

        float bestDistance =
            999999.0f;

        for (
            int j = 0;
            j < i;
            j++
        )
        {
            float d =
                Vector2Distance(
                    {
                        islands[i].position.x,
                        islands[i].position.z
                    },
                    {
                        islands[j].position.x,
                        islands[j].position.z
                    }
                );

            if (d < bestDistance)
            {
                bestDistance =
                    d;

                closest =
                    j;
            }
        }

        if (closest == -1)
            continue;

        Vector3 a =
        {
            islands[closest].position.x,
            0.4f,
            islands[closest].position.z
        };

        Vector3 b =
        {
            islands[i].position.x,
            0.4f,
            islands[i].position.z
        };

        float distance =
            Vector2Distance(
                {
                    a.x,
                    a.z
                },
                {
                    b.x,
                    b.z
                }
            );

        int segments =
            std::max(
                2,
                static_cast<int>(
                    std::ceil(
                        distance /
                        2.3f
                    )
                )
            );

        for (
            int s = 0;
            s <= segments;
            s++
        )
        {
            float t =
                static_cast<float>(s) /
                static_cast<float>(segments);

            BridgePiece bridge;

            bridge.center =
            {
                Lerp(
                    a.x,
                    b.x,
                    t
                ),
                0.4f,
                Lerp(
                    a.z,
                    b.z,
                    t
                )
            };

            bridge.size =
            {
                2.8f,
                0.8f,
                2.8f
            };

            bridges.push_back(
                bridge
            );
        }
    }

    return bridges;
}

// ============================================================
// FIND ISLAND
// ============================================================

int FindIslandAtPosition(
    Vector3 position,
    const std::vector<Island>& islands
)
{
    for (
        int i = 0;
        i < static_cast<int>(islands.size());
        i++
    )
    {
        const Island& island =
            islands[i];

        for (
            const PlatformPiece& piece :
            island.pieces
        )
        {
            float left =
                piece.center.x -
                piece.size.x * 0.5f;

            float right =
                piece.center.x +
                piece.size.x * 0.5f;

            float front =
                piece.center.z -
                piece.size.z * 0.5f;

            float back =
                piece.center.z +
                piece.size.z * 0.5f;

            float top =
                piece.center.y +
                piece.size.y *
                0.5f;

            if (
                position.x >= left &&
                position.x <= right &&
                position.z >= front &&
                position.z <= back &&
                std::abs(
                    position.y -
                    top
                ) < 0.55f
            )
            {
                return i;
            }
        }
    }

    return -1;
}

// ============================================================
// RAYCAST PLATFORM
// ============================================================

bool RaycastWorldPlatforms(
    Ray ray,
    const std::vector<Island>& islands,
    Vector3& hitPosition,
    int& islandIndex
)
{
    if (
        std::abs(
            ray.direction.y
        ) < 0.00001f
    )
    {
        return false;
    }

    float closestT =
        999999.0f;

    bool hitSomething =
        false;

    islandIndex =
        -1;

    for (
        int i = 0;
        i < static_cast<int>(islands.size());
        i++
    )
    {
        const Island& island =
            islands[i];

        for (
            const PlatformPiece& piece :
            island.pieces
        )
        {
            float top =
                piece.center.y +
                piece.size.y *
                0.5f;

            float t =
                (
                    top -
                    ray.position.y
                ) /
                ray.direction.y;

            if (t <= 0.0f)
                continue;

            Vector3 hit =
                Vector3Add(
                    ray.position,
                    Vector3Scale(
                        ray.direction,
                        t
                    )
                );

            float left =
                piece.center.x -
                piece.size.x *
                0.5f;

            float right =
                piece.center.x +
                piece.size.x *
                0.5f;

            float front =
                piece.center.z -
                piece.size.z *
                0.5f;

            float back =
                piece.center.z +
                piece.size.z *
                0.5f;

            if (
                hit.x >= left &&
                hit.x <= right &&
                hit.z >= front &&
                hit.z <= back
            )
            {
                if (t < closestT)
                {
                    closestT =
                        t;

                    hitPosition =
                        hit;

                    islandIndex =
                        i;

                    hitSomething =
                        true;
                }
            }
        }
    }

    return hitSomething;
}

// ============================================================
// FIND NEAREST MATURE PLANT
// ============================================================

int FindClosestMaturePlant(
    Vector3 playerPosition,
    const std::vector<Plant>& plants,
    float maxDistance
)
{
    int bestIndex =
        -1;

    float bestDistance =
        maxDistance;

    for (
        int i = 0;
        i < static_cast<int>(plants.size());
        i++
    )
    {
        const Plant& plant =
            plants[i];

        if (
            plant.age <
            plant.growTime
        )
        {
            continue;
        }

        float distance =
            Vector3Distance(
                playerPosition,
                plant.position
            );

        if (
            distance <
            bestDistance
        )
        {
            bestDistance =
                distance;

            bestIndex =
                i;
        }
    }

    return bestIndex;
}

// ============================================================
// FIND DROP
// ============================================================

int FindClosestDrop(
    Vector3 playerPosition,
    const std::vector<Drop>& drops,
    float maxDistance
)
{
    int bestIndex =
        -1;

    float bestDistance =
        maxDistance;

    for (
        int i = 0;
        i < static_cast<int>(drops.size());
        i++
    )
    {
        if (!drops[i].active)
            continue;

        float d =
            Vector3Distance(
                playerPosition,
                drops[i].position
            );

        if (d < bestDistance)
        {
            bestDistance =
                d;

            bestIndex =
                i;
        }
    }

    return bestIndex;
}

// ============================================================
// FIND NEAREST ISLAND
// ============================================================

int FindNearestUnownedIsland(
    Vector3 position,
    const std::vector<Island>& islands,
    float& outDistance
)
{
    int bestIndex =
        -1;

    float best =
        999999.0f;

    for (
        int i = 0;
        i < static_cast<int>(islands.size());
        i++
    )
    {
        const Island& island =
            islands[i];

        if (island.owned)
            continue;

        float centerDistance =
            Vector2Distance(
                {
                    position.x,
                    position.z
                },
                {
                    island.position.x,
                    island.position.z
                }
            );

        float radius =
            std::sqrt(
                island.width *
                island.width +
                island.depth *
                island.depth
            ) *
            0.42f;

        float edgeDistance =
            std::max(
                0.0f,
                centerDistance -
                radius
            );

        if (
            edgeDistance <
            best
        )
        {
            best =
                edgeDistance;

            bestIndex =
                i;
        }
    }

    outDistance =
        best;

    return bestIndex;
}

// ============================================================
// ENCHANT
// ============================================================

EnchantType RollEnchant(
    const Island& island,
    const EventState& event
)
{
    ThemeInfo theme =
        GetThemeInfo(
            island.theme
        );

    float chance =
        theme.enchantChance *
        island.luck *
        event.enchantMultiplier;

    chance =
        std::min(
            chance,
            0.96f
        );

    if (
        RandomFloat(
            0.0f,
            1.0f
        ) > chance
    )
    {
        return EnchantType::None;
    }

    float r =
        RandomFloat(
            0.0f,
            1.0f
        );

    if (r < 0.50f)
        return EnchantType::Shiny;

    if (r < 0.78f)
        return EnchantType::Giant;

    if (r < 0.92f)
        return EnchantType::Blessed;

    if (r < 0.99f)
        return EnchantType::Infernal;

    return EnchantType::Jackpot;
}

// ============================================================
// RANDOM SEED
// ============================================================

PlantType RandomSeedType()
{
    float r =
        RandomFloat(
            0.0f,
            100.0f
        );

    if (r < 30.0f)
        return PlantType::Sunleaf;

    if (r < 46.0f)
        return PlantType::Berry;

    if (r < 59.0f)
        return PlantType::Cactus;

    if (r < 70.0f)
        return PlantType::Mushroom;

    if (r < 82.0f)
        return PlantType::Inferno;

    if (r < 90.0f)
        return PlantType::GoldenFlower;

    if (r < 97.0f)
        return PlantType::CrystalBloom;

    return PlantType::VoidOrchid;
}

// ============================================================
// RANDOM DROP
// ============================================================

void SpawnRandomDrop(
    std::vector<Drop>& drops,
    const std::vector<Island>& islands
)
{
    if (islands.empty())
        return;

    int islandIndex =
        RandomInt(
            0,
            static_cast<int>(
                islands.size()
            ) - 1
        );

    const Island& island =
        islands[
            islandIndex
        ];

    if (island.pieces.empty())
        return;

    int pieceIndex =
        RandomInt(
            0,
            static_cast<int>(
                island.pieces.size()
            ) - 1
        );

    const PlatformPiece& piece =
        island.pieces[
            pieceIndex
        ];

    Drop drop;

    drop.position =
    {
        piece.center.x +
            RandomFloat(
                -piece.size.x * 0.30f,
                piece.size.x * 0.30f
            ),

        piece.center.y +
            piece.size.y * 0.5f,

        piece.center.z +
            RandomFloat(
                -piece.size.z * 0.30f,
                piece.size.z * 0.30f
            )
    };

    float roll =
        RandomFloat(
            0.0f,
            1.0f
        );

    if (roll < 0.78f)
    {
        drop.type =
            DropType::Seed;

        drop.seedType =
            RandomSeedType();
    }
    else if (roll < 0.96f)
    {
        drop.type =
            DropType::Cash;

        ThemeInfo theme =
            GetThemeInfo(
                island.theme
            );

        drop.cashValue =
            std::max(
                50.0,
                70.0 *
                theme.valueMultiplier
            );
    }
    else
    {
        drop.type =
            DropType::Growth;
    }

    drops.push_back(
        drop
    );
}

// ============================================================
// EVENT
// ============================================================

void TriggerRandomEvent(
    EventState& event,
    std::vector<Drop>& drops,
    const std::vector<Island>& islands
)
{
    event = {};

    int r =
        RandomInt(
            0,
            5
        );

    switch (r)
    {
        case 0:
            event.type =
                EventType::GoldenRain;

            event.name =
                "GOLDEN RAIN";

            event.description =
                "Every harvest is worth x3.";

            event.saleMultiplier =
                3.0;

            event.remaining =
                35.0f;

            break;

        case 1:
            event.type =
                EventType::SuperGrowth;

            event.name =
                "SUPER GROWTH";

            event.description =
                "Plants grow x3 faster.";

            event.growthMultiplier =
                3.0f;

            event.remaining =
                35.0f;

            break;

        case 2:
            event.type =
                EventType::EnchantStorm;

            event.name =
                "ENCHANT STORM";

            event.description =
                "Enchant luck massively increased.";

            event.enchantMultiplier =
                3.0f;

            event.remaining =
                40.0f;

            break;

        case 3:
            event.type =
                EventType::TreasureRain;

            event.name =
                "TREASURE RAIN";

            event.description =
                "A huge number of drops appeared.";

            event.remaining =
                25.0f;

            for (
                int i = 0;
                i < 14;
                i++
            )
            {
                SpawnRandomDrop(
                    drops,
                    islands
                );
            }

            break;

        case 4:
            event.type =
                EventType::MarketBoom;

            event.name =
                "MARKET BOOM";

            event.description =
                "Every crop sells for x5.";

            event.saleMultiplier =
                5.0;

            event.remaining =
                30.0f;

            break;

        case 5:
            event.type =
                EventType::LiminalBloom;

            event.name =
                "LIMINAL BLOOM";

            event.description =
                "Massive value and growth boost.";

            event.saleMultiplier =
                7.0;

            event.growthMultiplier =
                2.0f;

            event.remaining =
                30.0f;

            break;
    }
}

// ============================================================
// DRAW ISLAND
// ============================================================

void DrawIsland(
    const Island& island,
    Vector3 cameraPosition
)
{
    float distance =
        Vector3Distance(
            cameraPosition,
            island.position
        );

    if (
        distance >
        230.0f
    )
    {
        return;
    }

    ThemeInfo theme =
        GetThemeInfo(
            island.theme
        );

    // --------------------------------------------------------
    // LOD
    // --------------------------------------------------------

    bool far =
        distance > 115.0f;

    int levelSkip =
        1;

    if (far)
        levelSkip =
            3;

    // --------------------------------------------------------
    // DRAW PIECES
    // --------------------------------------------------------

    int currentLevel =
        -1;

    for (
        const PlatformPiece& piece :
        island.pieces
    )
    {
        int level =
            static_cast<int>(
                std::round(
                    (
                        piece.center.y -
                        island.position.y
                    ) /
                    LEVEL_HEIGHT
                )
            );

        if (
            far &&
            level %
                levelSkip !=
            0 &&
            !piece.stair
        )
        {
            continue;
        }

        float pieceDistance =
            Vector3Distance(
                cameraPosition,
                piece.center
            );

        if (
            pieceDistance >
            220.0f
        )
        {
            continue;
        }

        Color base =
            theme.baseColor;

        if (piece.stair)
        {
            base =
                Color{
                    static_cast<unsigned char>(
                        std::min(
                            255,
                            base.r + 12
                        )
                    ),
                    static_cast<unsigned char>(
                        std::min(
                            255,
                            base.g + 12
                        )
                    ),
                    static_cast<unsigned char>(
                        std::min(
                            255,
                            base.b + 12
                        )
                    ),
                    255
                };
        }

        DrawCubeV(
            piece.center,
            piece.size,
            base
        );

        // ----------------------------------------------------
        // ACCENT STRIPS
        // ----------------------------------------------------

        if (
            piece.accent &&
            !far
        )
        {
            DrawCubeV(
                {
                    piece.center.x,
                    piece.center.y +
                        piece.size.y *
                        0.5f +
                        0.035f,
                    piece.center.z
                },
                {
                    piece.size.x *
                        0.82f,
                    0.07f,
                    0.11f
                },
                theme.accentColor
            );
        }

        // ----------------------------------------------------
        // LIGHT BARS
        // ----------------------------------------------------

        if (
            piece.light &&
            !far
        )
        {
            DrawCubeV(
                {
                    piece.center.x,
                    piece.center.y +
                        piece.size.y *
                        0.5f +
                        0.10f,
                    piece.center.z
                },
                {
                    0.18f,
                    0.08f,
                    piece.size.z *
                        0.58f
                },
                theme.accentColor
            );
        }
    }

    // --------------------------------------------------------
    // LIMINAL STRUCTURE
    // --------------------------------------------------------

    if (
        island.theme ==
        IslandTheme::Liminal
    )
    {
        if (
            !far &&
            island.levels >= 15
        )
        {
            for (
                int level = 0;
                level < island.levels;
                level += 4
            )
            {
                float y =
                    island.position.y +
                    level *
                    LEVEL_HEIGHT +
                    2.0f;

                DrawCubeV(
                    {
                        island.position.x -
                            island.width *
                            0.28f,
                        y,
                        island.position.z
                    },
                    {
                        0.18f,
                        3.3f,
                        0.18f
                    },
                    Fade(
                        theme.accentColor,
                        0.65f
                    )
                );

                DrawCubeV(
                    {
                        island.position.x +
                            island.width *
                            0.28f,
                        y,
                        island.position.z
                    },
                    {
                        0.18f,
                        3.3f,
                        0.18f
                    },
                    Fade(
                        theme.accentColor,
                        0.65f
                    )
                );
            }
        }
    }
}

// ============================================================
// DRAW BRIDGE
// ============================================================

void DrawBridge(
    const BridgePiece& bridge,
    Vector3 cameraPosition
)
{
    float d =
        Vector3Distance(
            cameraPosition,
            bridge.center
        );

    if (d > 220.0f)
        return;

    DrawCubeV(
        bridge.center,
        bridge.size,
        Color{
            48,
            56,
            66,
            255
        }
    );

    DrawCubeV(
        {
            bridge.center.x,
            bridge.center.y +
                bridge.size.y *
                0.5f +
                0.04f,
            bridge.center.z
        },
        {
            bridge.size.x *
                0.70f,
            0.08f,
            0.12f
        },
        Color{
            85,
            210,
            255,
            255
        }
    );
}

// ============================================================
// DRAW PLANT
// ============================================================

void DrawPlant(
    const Plant& plant,
    Vector3 cameraPosition
)
{
    float d =
        Vector3Distance(
            cameraPosition,
            plant.position
        );

    if (d > 130.0f)
        return;

    PlantInfo info =
        GetPlantInfo(
            plant.type
        );

    float growth =
        std::clamp(
            plant.age /
                plant.growTime,
            0.0f,
            1.0f
        );

    float height =
        0.30f +
        2.1f *
        growth;

    DrawCylinder(
        {
            plant.position.x,
            plant.position.y +
                height * 0.5f,
            plant.position.z
        },
        0.08f,
        0.13f,
        height,
        8,
        DARKGREEN
    );

    DrawSphere(
        {
            plant.position.x,
            plant.position.y +
                height,
            plant.position.z
        },
        0.25f +
            growth *
            0.28f,
        info.color
    );

    if (
        plant.age >=
        plant.growTime
    )
    {
        DrawCircle3D(
            {
                plant.position.x,
                plant.position.y +
                    0.035f,
                plant.position.z
            },
            0.46f,
            {
                1,
                0,
                0
            },
            90.0f,
            Fade(
                YELLOW,
                0.25f
            )
        );

        if (
            plant.enchant !=
            EnchantType::None
        )
        {
            DrawSphereWires(
                {
                    plant.position.x,
                    plant.position.y +
                        height,
                    plant.position.z
                },
                0.44f,
                8,
                8,
                YELLOW
            );
        }
    }
}

// ============================================================
// DRAW DROP
// ============================================================

void DrawDrop(
    const Drop& drop,
    Vector3 cameraPosition
)
{
    if (!drop.active)
        return;

    if (
        Vector3Distance(
            cameraPosition,
            drop.position
        ) > 120.0f
    )
    {
        return;
    }

    float bob =
        std::sin(
            static_cast<float>(
                GetTime()
            ) *
            3.2f
        ) *
        0.14f;

    Vector3 position =
    {
        drop.position.x,
        drop.position.y +
            0.50f +
            bob,
        drop.position.z
    };

    if (
        drop.type ==
        DropType::Seed
    )
    {
        PlantInfo info =
            GetPlantInfo(
                drop.seedType
            );

        DrawCubeV(
            position,
            {
                0.42f,
                0.42f,
                0.42f
            },
            info.color
        );
    }
    else if (
        drop.type ==
        DropType::Cash
    )
    {
        DrawSphere(
            position,
            0.30f,
            GOLD
        );
    }
    else
    {
        DrawSphere(
            position,
            0.27f,
            SKYBLUE
        );
    }
}

// ============================================================
// ADMIN STATE
// ============================================================

struct AdminState
{
    bool panelOpen =
        false;

    bool authenticated =
        false;

    AdminFocus focus =
        AdminFocus::None;

    std::string password;

    const std::string username =
        "GRAVITY_INT";

    const std::string correctPassword =
        "PlMan";
};

// ============================================================
// ADMIN LOGIN
// ============================================================

void TryAdminLogin(
    AdminState& admin,
    std::string& message,
    float& messageTimer
)
{
    if (
        admin.password ==
        admin.correctPassword
    )
    {
        admin.authenticated =
            true;

        admin.focus =
            AdminFocus::None;

        message =
            "ADMIN ACCESS GRANTED";

        messageTimer =
            3.0f;
    }
    else
    {
        admin.password.clear();

        message =
            "WRONG PASSWORD";

        messageTimer =
            2.0f;
    }
}

// ============================================================
// ADMIN INPUT
// ============================================================

void UpdateAdminInput(
    AdminState& admin,
    std::string& message,
    float& messageTimer
)
{
    if (
        admin.authenticated
    )
    {
        return;
    }

    Rectangle passwordBox =
    {
        390,
        340,
        500,
        48
    };

    if (
        IsMouseButtonPressed(
            MOUSE_BUTTON_LEFT
        )
    )
    {
        Vector2 mouse =
            GetMousePosition();

        if (
            CheckCollisionPointRec(
                mouse,
                passwordBox
            )
        )
        {
            admin.focus =
                AdminFocus::Password;
        }
        else
        {
            admin.focus =
                AdminFocus::None;
        }
    }

    if (
        admin.focus ==
        AdminFocus::Password
    )
    {
        int key;

        while (
            (key = GetCharPressed()) != 0
        )
        {
            if (
                key >= 32 &&
                key <= 126
            )
            {
                admin.password +=
                    static_cast<char>(
                        key
                    );
            }
        }

        if (
            IsKeyPressed(
                KEY_BACKSPACE
            )
        )
        {
            if (
                !admin.password.empty()
            )
            {
                admin.password.pop_back();
            }
        }

        if (
            IsKeyPressed(
                KEY_ENTER
            )
        )
        {
            TryAdminLogin(
                admin,
                message,
                messageTimer
            );
        }
    }

    Rectangle loginButton =
    {
        390,
        420,
        500,
        52
    };

    if (
        IsMouseButtonPressed(
            MOUSE_BUTTON_LEFT
        )
    )
    {
        Vector2 mouse =
            GetMousePosition();

        if (
            CheckCollisionPointRec(
                mouse,
                loginButton
            )
        )
        {
            TryAdminLogin(
                admin,
                message,
                messageTimer
            );
        }
    }
}

// ============================================================
// ADMIN COMMANDS
// ============================================================

void UpdateAdminCommands(
    const AdminState& admin,
    double& money,
    std::array<int, PLANT_COUNT>& seeds,
    EventState& event,
    std::vector<Drop>& drops,
    std::vector<Island>& islands,
    Vector3& playerPosition,
    std::string& message,
    float& messageTimer
)
{
    if (
        !admin.authenticated
    )
    {
        return;
    }

    if (
        IsKeyPressed(KEY_F1)
    )
    {
        money +=
            1000000.0;

        message =
            "ADMIN: +$1,000,000";

        messageTimer =
            2.0f;
    }

    if (
        IsKeyPressed(KEY_F2)
    )
    {
        for (
            int& amount :
            seeds
        )
        {
            amount +=
                100;
        }

        message =
            "ADMIN: +100 EVERY SEED";

        messageTimer =
            2.0f;
    }

    if (
        IsKeyPressed(KEY_F3)
    )
    {
        TriggerRandomEvent(
            event,
            drops,
            islands
        );

        message =
            "ADMIN: EVENT TRIGGERED";

        messageTimer =
            2.0f;
    }

    if (
        IsKeyPressed(KEY_F4)
    )
    {
        for (
            int i = 0;
            i < 20;
            i++
        )
        {
            SpawnRandomDrop(
                drops,
                islands
            );
        }

        message =
            "ADMIN: 20 DROPS SPAWNED";

        messageTimer =
            2.0f;
    }

    if (
        IsKeyPressed(KEY_F5)
    )
    {
        for (
            Island& island :
            islands
        )
        {
            island.owned =
                true;
        }

        message =
            "ADMIN: ALL ISLANDS UNLOCKED";

        messageTimer =
            2.0f;
    }

    if (
        IsKeyPressed(KEY_F6)
    )
    {
        if (!islands.empty())
        {
            const PlatformPiece& start =
                islands[0].pieces[0];

            playerPosition =
            {
                start.center.x,
                start.center.y +
                    start.size.y *
                    0.5f,
                start.center.z
            };
        }

        message =
            "ADMIN: TELEPORTED HOME";

        messageTimer =
            2.0f;
    }
}

// ============================================================
// MAIN
// ============================================================

int main()
{
    InitWindow(
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        "Island Garden"
    );

    SetTargetFPS(
        120
    );

    // IMPORTANT:
    // We intentionally do NOT call DisableCursor().
    // The cursor must remain visible for planting and UI.

    // ========================================================
    // WORLD
    // ========================================================

    std::vector<Island> islands =
        GenerateWorld();

    std::vector<BridgePiece> bridges =
        GenerateBridges(
            islands
        );

    std::vector<Plant> plants;

    std::vector<Drop> drops;

    // ========================================================
    // PLAYER
    // ========================================================

    Player player;

    if (
        !islands.empty() &&
        !islands[0].pieces.empty()
    )
    {
        const PlatformPiece& start =
            islands[0].pieces[0];

        player.position =
        {
            start.center.x,
            start.center.y +
                start.size.y *
                0.5f,
            start.center.z
        };
    }

    // ========================================================
    // CAMERA
    // ========================================================

    GameCamera gameCamera;

    // ========================================================
    // ECONOMY
    // ========================================================

    double money =
        1000.0;

    std::array<int, PLANT_COUNT> seeds =
    {
        12,
        5,
        1,
        0,
        0,
        0,
        0,
        0
    };

    PlantType selectedPlant =
        PlantType::Sunleaf;

    // ========================================================
    // SHOP
    // ========================================================

    bool shopOpen =
        false;

    // ========================================================
    // ADMIN
    // ========================================================

    AdminState admin;

    // ========================================================
    // EVENT
    // ========================================================

    EventState event;

    float eventTimer =
        RandomFloat(
            25.0f,
            45.0f
        );

    float dropTimer =
        RandomFloat(
            10.0f,
            18.0f
        );

    // ========================================================
    // MESSAGE
    // ========================================================

    std::string message =
        "Welcome to Island Garden";

    float messageTimer =
        4.0f;

    // ========================================================
    // MAIN LOOP
    // ========================================================

    while (
        !WindowShouldClose()
    )
    {
        float dt =
            GetFrameTime();

        // ====================================================
        // F10 ADMIN
        // ====================================================

        if (
            IsKeyPressed(
                KEY_F10
            )
        )
        {
            admin.panelOpen =
                !admin.panelOpen;

            shopOpen =
                false;

            admin.focus =
                AdminFocus::None;

            // Cursor stays visible.
        }

        // ====================================================
        // ADMIN MODE
        // ====================================================

        if (
            admin.panelOpen
        )
        {
            UpdateAdminInput(
                admin,
                message,
                messageTimer
            );

            UpdateAdminCommands(
                admin,
                money,
                seeds,
                event,
                drops,
                islands,
                player.position,
                message,
                messageTimer
            );
        }
        else
        {
            // =================================================
            // SHOP
            // =================================================

            if (
                IsKeyPressed(
                    KEY_B
                )
            )
            {
                shopOpen =
                    !shopOpen;
            }

            // =================================================
            // SHOP MODE
            // =================================================

            if (shopOpen)
            {
                PlantType shopPlants[] =
                {
                    PlantType::Sunleaf,
                    PlantType::Berry,
                    PlantType::Cactus,
                    PlantType::Mushroom,
                    PlantType::Inferno,
                    PlantType::GoldenFlower,
                    PlantType::CrystalBloom,
                    PlantType::VoidOrchid
                };

                KeyboardKey keys[] =
                {
                    KEY_ONE,
                    KEY_TWO,
                    KEY_THREE,
                    KEY_FOUR,
                    KEY_FIVE,
                    KEY_SIX,
                    KEY_SEVEN,
                    KEY_EIGHT
                };

                for (
                    int i = 0;
                    i < PLANT_COUNT;
                    i++
                )
                {
                    if (
                        IsKeyPressed(
                            keys[i]
                        )
                    )
                    {
                        PlantInfo info =
                            GetPlantInfo(
                                shopPlants[i]
                            );

                        if (
                            money >=
                            info.seedPrice
                        )
                        {
                            money -=
                                info.seedPrice;

                            seeds[i]++;

                            message =
                                "Bought " +
                                std::string(
                                    info.name
                                );

                            messageTimer =
                                2.0f;
                        }
                        else
                        {
                            message =
                                "Not enough money.";

                            messageTimer =
                                2.0f;
                        }
                    }
                }
            }
            else
            {
                // =================================================
                // PLAYER
                // =================================================

                player.Update(
                    dt,
                    gameCamera.yaw,
                    islands,
                    bridges
                );

                // =================================================
                // SELECT PLANT
                // =================================================

                if (
                    IsKeyPressed(
                        KEY_ONE
                    )
                )
                {
                    selectedPlant =
                        PlantType::Sunleaf;
                }

                if (
                    IsKeyPressed(
                        KEY_TWO
                    )
                )
                {
                    selectedPlant =
                        PlantType::Berry;
                }

                if (
                    IsKeyPressed(
                        KEY_THREE
                    )
                )
                {
                    selectedPlant =
                        PlantType::Cactus;
                }

                if (
                    IsKeyPressed(
                        KEY_FOUR
                    )
                )
                {
                    selectedPlant =
                        PlantType::Mushroom;
                }

                if (
                    IsKeyPressed(
                        KEY_FIVE
                    )
                )
                {
                    selectedPlant =
                        PlantType::Inferno;
                }

                if (
                    IsKeyPressed(
                        KEY_SIX
                    )
                )
                {
                    selectedPlant =
                        PlantType::GoldenFlower;
                }

                if (
                    IsKeyPressed(
                        KEY_SEVEN
                    )
                )
                {
                    selectedPlant =
                        PlantType::CrystalBloom;
                }

                if (
                    IsKeyPressed(
                        KEY_EIGHT
                    )
                )
                {
                    selectedPlant =
                        PlantType::VoidOrchid;
                }

                // =================================================
                // PLANT
                // =================================================

                if (
                    IsMouseButtonPressed(
                        MOUSE_BUTTON_LEFT
                    )
                )
                {
                    Ray ray =
                        GetScreenToWorldRay(
                            GetMousePosition(),
                            gameCamera.camera
                        );

                    Vector3 hit;

                    int islandIndex =
                        -1;

                    if (
                        RaycastWorldPlatforms(
                            ray,
                            islands,
                            hit,
                            islandIndex
                        )
                    )
                    {
                        Island& island =
                            islands[
                                islandIndex
                            ];

                        if (
                            !island.owned
                        )
                        {
                            message =
                                "Buy this island first.";

                            messageTimer =
                                2.0f;
                        }
                        else
                        {
                            int seedIndex =
                                static_cast<int>(
                                    selectedPlant
                                );

                            if (
                                seeds[
                                    seedIndex
                                ] <= 0
                            )
                            {
                                message =
                                    "You don't have that seed.";

                                messageTimer =
                                    2.0f;
                            }
                            else
                            {
                                int used =
                                    0;

                                for (
                                    const Plant& plant :
                                    plants
                                )
                                {
                                    if (
                                        plant.islandIndex ==
                                        islandIndex
                                    )
                                    {
                                        used++;
                                    }
                                }

                                if (
                                    used >=
                                    island.capacity
                                )
                                {
                                    message =
                                        "Island is full.";

                                    messageTimer =
                                        2.0f;
                                }
                                else
                                {
                                    bool tooClose =
                                        false;

                                    for (
                                        const Plant& plant :
                                        plants
                                    )
                                    {
                                        if (
                                            plant.islandIndex ==
                                            islandIndex
                                        )
                                        {
                                            if (
                                                Vector3Distance(
                                                    plant.position,
                                                    hit
                                                ) <
                                                1.05f
                                            )
                                            {
                                                tooClose =
                                                    true;

                                                break;
                                            }
                                        }
                                    }

                                    if (
                                        tooClose
                                    )
                                    {
                                        message =
                                            "Too close to another plant.";

                                        messageTimer =
                                            2.0f;
                                    }
                                    else
                                    {
                                        PlantInfo info =
                                            GetPlantInfo(
                                                selectedPlant
                                            );

                                        float growthMultiplier =
                                            GetThemeInfo(
                                                island.theme
                                            ).growthMultiplier *
                                            event.growthMultiplier;

                                        float growTime =
                                            info.growTime /
                                            growthMultiplier;

                                        growTime *=
                                            RandomFloat(
                                                0.90f,
                                                1.10f
                                            );

                                        Plant plant;

                                        plant.position =
                                            hit;

                                        plant.type =
                                            selectedPlant;

                                        plant.enchant =
                                            RollEnchant(
                                                island,
                                                event
                                            );

                                        plant.islandIndex =
                                            islandIndex;

                                        plant.growTime =
                                            growTime;

                                        plant.age =
                                            0.0f;

                                        plants.push_back(
                                            plant
                                        );

                                        seeds[
                                            seedIndex
                                        ]--;

                                        message =
                                            "Planted " +
                                            std::string(
                                                info.name
                                            );

                                        if (
                                            plant.enchant !=
                                            EnchantType::None
                                        )
                                        {
                                            message +=
                                                " [" +
                                                std::string(
                                                    GetEnchantName(
                                                        plant.enchant
                                                    )
                                                ) +
                                                "]";
                                        }

                                        messageTimer =
                                            2.5f;
                                    }
                                }
                            }
                        }
                    }
                }

                // =================================================
                // INTERACTION
                // =================================================

                if (
                    IsKeyPressed(
                        KEY_E
                    )
                )
                {
                    // ---------------------------------------------
                    // DROP
                    // ---------------------------------------------

                    int dropIndex =
                        FindClosestDrop(
                            player.position,
                            drops,
                            2.4f
                        );

                    if (
                        dropIndex !=
                        -1
                    )
                    {
                        Drop& drop =
                            drops[
                                dropIndex
                            ];

                        if (
                            drop.type ==
                            DropType::Seed
                        )
                        {
                            seeds[
                                static_cast<int>(
                                    drop.seedType
                                )
                            ]++;

                            message =
                                "Seed found: " +
                                std::string(
                                    GetPlantInfo(
                                        drop.seedType
                                    ).name
                                );
                        }
                        else if (
                            drop.type ==
                            DropType::Cash
                        )
                        {
                            money +=
                                drop.cashValue;

                            message =
                                "Found " +
                                FormatMoney(
                                    drop.cashValue
                                );
                        }
                        else
                        {
                            int closestPlant =
                                -1;

                            float best =
                                4.0f;

                            for (
                                int i = 0;
                                i <
                                static_cast<int>(
                                    plants.size()
                                );
                                i++
                            )
                            {
                                float d =
                                    Vector3Distance(
                                        player.position,
                                        plants[i].position
                                    );

                                if (
                                    d <
                                    best
                                )
                                {
                                    best =
                                        d;

                                    closestPlant =
                                        i;
                                }
                            }

                            if (
                                closestPlant !=
                                -1
                            )
                            {
                                plants[
                                    closestPlant
                                ].age +=
                                    plants[
                                        closestPlant
                                    ].growTime *
                                    0.45f;

                                plants[
                                    closestPlant
                                ].age =
                                    std::min(
                                        plants[
                                            closestPlant
                                        ].age,
                                        plants[
                                            closestPlant
                                        ].growTime
                                    );
                            }

                            message =
                                "Growth boosted.";
                        }

                        drop.active =
                            false;

                        messageTimer =
                            2.5f;
                    }
                    else
                    {
                        // ---------------------------------------------
                        // HARVEST
                        // ---------------------------------------------

                        int plantIndex =
                            FindClosestMaturePlant(
                                player.position,
                                plants,
                                2.8f
                            );

                        if (
                            plantIndex !=
                            -1
                        )
                        {
                            Plant harvested =
                                plants[
                                    plantIndex
                                ];

                            Island& island =
                                islands[
                                    harvested.islandIndex
                                ];

                            ThemeInfo theme =
                                GetThemeInfo(
                                    island.theme
                                );

                            PlantInfo info =
                                GetPlantInfo(
                                    harvested.type
                                );

                            // =====================================
                            // GUARANTEED PROFIT
                            // =====================================

                            double luckBonus =
                                1.0 +
                                island.luck *
                                0.45;

                            double value =
                                info.baseValue *
                                theme.valueMultiplier *
                                luckBonus *
                                GetEnchantMultiplier(
                                    harvested.enchant
                                ) *
                                event.saleMultiplier;

                            // Explicit floor:
                            // every seed ALWAYS returns profit.

                            double minimumValue =
                                info.seedPrice *
                                2.0;

                            value =
                                std::max(
                                    value,
                                    minimumValue
                                );

                            money +=
                                value;

                            message =
                                "Harvested " +
                                std::string(
                                    info.name
                                ) +
                                "  +" +
                                FormatMoney(
                                    value
                                );

                            if (
                                harvested.enchant !=
                                EnchantType::None
                            )
                            {
                                message +=
                                    " [" +
                                    std::string(
                                        GetEnchantName(
                                            harvested.enchant
                                        )
                                    ) +
                                    "]";
                            }

                            messageTimer =
                                4.0f;

                            plants.erase(
                                plants.begin() +
                                plantIndex
                            );
                        }
                        else
                        {
                            // ---------------------------------------------
                            // BUY ISLAND
                            // ---------------------------------------------

                            float distanceToIsland =
                                999999.0f;

                            int islandIndex =
                                FindNearestUnownedIsland(
                                    player.position,
                                    islands,
                                    distanceToIsland
                                );

                            if (
                                islandIndex !=
                                -1 &&
                                distanceToIsland <
                                5.0f
                            )
                            {
                                Island& island =
                                    islands[
                                        islandIndex
                                    ];

                                if (
                                    money >=
                                    island.price
                                )
                                {
                                    money -=
                                        island.price;

                                    island.owned =
                                        true;

                                    for (
                                        int i = 0;
                                        i < 3;
                                        i++
                                    )
                                    {
                                        PlantType seed =
                                            RandomSeedType();

                                        seeds[
                                            static_cast<int>(
                                                seed
                                            )
                                        ]++;
                                    }

                                    message =
                                        "ISLAND PURCHASED: " +
                                        std::string(
                                            GetThemeInfo(
                                                island.theme
                                            ).name
                                        );

                                    messageTimer =
                                        4.0f;
                                }
                                else
                                {
                                    message =
                                        "Need " +
                                        FormatMoney(
                                            island.price -
                                            money
                                        ) +
                                        " more.";

                                    messageTimer =
                                        2.5f;
                                }
                            }
                        }
                    }
                }
            }
        }

        // ========================================================
        // EVENT TIMER
        // ========================================================

        if (
            event.type !=
            EventType::None
        )
        {
            event.remaining -=
                dt;

            if (
                event.remaining <=
                0.0f
            )
            {
                event = {};
            }
        }

        eventTimer -=
            dt;

        if (
            eventTimer <=
            0.0f &&
            event.type ==
            EventType::None
        )
        {
            TriggerRandomEvent(
                event,
                drops,
                islands
            );

            message =
                "EVENT: " +
                event.name;

            messageTimer =
                4.0f;

            eventTimer =
                RandomFloat(
                    30.0f,
                    55.0f
                );
        }

        // ========================================================
        // RANDOM DROPS
        // ========================================================

        dropTimer -=
            dt;

        if (
            dropTimer <=
            0.0f
        )
        {
            int count =
                RandomInt(
                    1,
                    3
                );

            for (
                int i = 0;
                i < count;
                i++
            )
            {
                SpawnRandomDrop(
                    drops,
                    islands
                );
            }

            dropTimer =
                RandomFloat(
                    12.0f,
                    24.0f
                );
        }

        // ========================================================
        // UPDATE PLANTS
        // ========================================================

        for (
            Plant& plant :
            plants
        )
        {
            if (
                plant.islandIndex <
                0 ||
                plant.islandIndex >=
                static_cast<int>(
                    islands.size()
                )
            )
            {
                continue;
            }

            const Island& island =
                islands[
                    plant.islandIndex
                ];

            ThemeInfo theme =
                GetThemeInfo(
                    island.theme
                );

            plant.age +=
                dt *
                theme.growthMultiplier *
                event.growthMultiplier;

            if (
                plant.age >
                plant.growTime
            )
            {
                plant.age =
                    plant.growTime;
            }
        }

        // ========================================================
        // CLEAN DROPS
        // ========================================================

        drops.erase(
            std::remove_if(
                drops.begin(),
                drops.end(),
                [](const Drop& drop)
                {
                    return !drop.active;
                }
            ),
            drops.end()
        );

        // ========================================================
        // CAMERA
        // ========================================================

        Vector3 cameraTarget =
        {
            player.position.x,
            player.position.y +
                1.0f,
            player.position.z
        };

        gameCamera.Update(
            cameraTarget
        );

        // ========================================================
        // MESSAGE TIMER
        // ========================================================

        messageTimer -=
            dt;

        // ========================================================
        // DRAW
        // ========================================================

        BeginDrawing();

        ClearBackground(
            Color{
                17,
                22,
                31,
                255
            }
        );

        BeginMode3D(
            gameCamera.camera
        );

        // --------------------------------------------------------
        // ABYSS / WATER
        // --------------------------------------------------------

        DrawCubeV(
            {
                0,
                -5.0f,
                0
            },
            {
                300,
                1,
                300
            },
            Color{
                20,
                54,
                73,
                255
            }
        );

        // --------------------------------------------------------
        // BRIDGES
        // --------------------------------------------------------

        for (
            const BridgePiece& bridge :
            bridges
        )
        {
            DrawBridge(
                bridge,
                gameCamera.camera.position
            );
        }

        // --------------------------------------------------------
        // ISLANDS
        // --------------------------------------------------------

        for (
            const Island& island :
            islands
        )
        {
            DrawIsland(
                island,
                gameCamera.camera.position
            );
        }

        // --------------------------------------------------------
        // PLANTS
        // --------------------------------------------------------

        for (
            const Plant& plant :
            plants
        )
        {
            DrawPlant(
                plant,
                gameCamera.camera.position
            );
        }

        // --------------------------------------------------------
        // DROPS
        // --------------------------------------------------------

        for (
            const Drop& drop :
            drops
        )
        {
            DrawDrop(
                drop,
                gameCamera.camera.position
            );
        }

        // --------------------------------------------------------
        // PLAYER
        // --------------------------------------------------------

        player.Draw();

        // --------------------------------------------------------
        // GRID
        // --------------------------------------------------------

        DrawGrid(
            100,
            2.0f
        );

        // --------------------------------------------------------
        // PLANT TARGET PREVIEW
        // --------------------------------------------------------

        if (
            !shopOpen &&
            !admin.panelOpen
        )
        {
            Ray ray =
                GetScreenToWorldRay(
                    GetMousePosition(),
                    gameCamera.camera
                );

            Vector3 hit;

            int hitIsland =
                -1;

            if (
                RaycastWorldPlatforms(
                    ray,
                    islands,
                    hit,
                    hitIsland
                )
            )
            {
                if (
                    hitIsland >=
                    0
                )
                {
                    const Island& island =
                        islands[
                            hitIsland
                        ];

                    Color previewColor =
                        island.owned
                        ? GREEN
                        : RED;

                    DrawCircle3D(
                        {
                            hit.x,
                            hit.y +
                                0.04f,
                            hit.z
                        },
                        0.38f,
                        {
                            1,
                            0,
                            0
                        },
                        90.0f,
                        Fade(
                            previewColor,
                            0.45f
                        )
                    );
                }
            }
        }

        EndMode3D();

        // ========================================================
        // HUD
        // ========================================================

        DrawRectangle(
            16,
            16,
            405,
            190,
            Fade(
                BLACK,
                0.70f
            )
        );

        DrawText(
            "ISLAND GARDEN",
            32,
            30,
            26,
            WHITE
        );

        DrawText(
            (
                "Money: " +
                FormatMoney(
                    money
                )
            ).c_str(),
            32,
            66,
            20,
            GOLD
        );

        PlantInfo selectedInfo =
            GetPlantInfo(
                selectedPlant
            );

        DrawText(
            TextFormat(
                "Selected: %s",
                selectedInfo.name
            ),
            32,
            94,
            18,
            selectedInfo.color
        );

        DrawText(
            TextFormat(
                "Seeds: %d",
                seeds[
                    static_cast<int>(
                        selectedPlant
                    )
                ]
            ),
            32,
            120,
            18,
            WHITE
        );

        int currentIsland =
            FindIslandAtPosition(
                player.position,
                islands
            );

        if (
            currentIsland !=
            -1
        )
        {
            const Island& island =
                islands[
                    currentIsland
                ];

            ThemeInfo theme =
                GetThemeInfo(
                    island.theme
                );

            int plantCount =
                0;

            for (
                const Plant& plant :
                plants
            )
            {
                if (
                    plant.islandIndex ==
                    currentIsland
                )
                {
                    plantCount++;
                }
            }

            DrawText(
                TextFormat(
                    "Island: %s",
                    theme.name
                ),
                32,
                147,
                18,
                theme.accentColor
            );

            DrawText(
                TextFormat(
                    "Luck: %.2fx",
                    island.luck
                ),
                220,
                147,
                18,
                YELLOW
            );

            DrawText(
                TextFormat(
                    "Plants: %d / %d",
                    plantCount,
                    island.capacity
                ),
                32,
                173,
                18,
                WHITE
            );

            DrawText(
                TextFormat(
                    "Levels: %d",
                    island.levels
                ),
                210,
                173,
                18,
                LIGHTGRAY
            );
        }
        else
        {
            DrawText(
                "Island: NONE",
                32,
                147,
                18,
                SKYBLUE
            );
        }

        // ========================================================
        // EVENT HUD
        // ========================================================

        if (
            event.type !=
            EventType::None
        )
        {
            DrawRectangle(
                SCREEN_WIDTH / 2 -
                    270,
                15,
                540,
                82,
                Fade(
                    BLACK,
                    0.75f
                )
            );

            DrawText(
                event.name.c_str(),
                SCREEN_WIDTH / 2 -
                    245,
                27,
                24,
                YELLOW
            );

            DrawText(
                event.description.c_str(),
                SCREEN_WIDTH / 2 -
                    245,
                58,
                17,
                WHITE
            );

            DrawText(
                TextFormat(
                    "%.0fs",
                    event.remaining
                ),
                SCREEN_WIDTH / 2 +
                    195,
                58,
                17,
                ORANGE
            );
        }

        // ========================================================
        // CONTROLS
        // ========================================================

        DrawRectangle(
            16,
            SCREEN_HEIGHT - 145,
            470,
            125,
            Fade(
                BLACK,
                0.68f
            )
        );

        DrawText(
            "WASD  MOVE",
            32,
            SCREEN_HEIGHT - 127,
            16,
            WHITE
        );

        DrawText(
            "SHIFT  SPRINT",
            32,
            SCREEN_HEIGHT - 102,
            16,
            WHITE
        );

        DrawText(
            "SPACE  JUMP / DOUBLE JUMP",
            32,
            SCREEN_HEIGHT - 77,
            16,
            WHITE
        );

        DrawText(
            "RMB  CAMERA",
            250,
            SCREEN_HEIGHT - 127,
            16,
            WHITE
        );

        DrawText(
            "LMB  PLANT",
            250,
            SCREEN_HEIGHT - 102,
            16,
            WHITE
        );

        DrawText(
            "E  INTERACT",
            250,
            SCREEN_HEIGHT - 77,
            16,
            WHITE
        );

        DrawText(
            "B SHOP     F10 ADMIN",
            250,
            SCREEN_HEIGHT - 52,
            16,
            LIGHTGRAY
        );

        // ========================================================
        // NEAREST ISLAND
        // ========================================================

        float islandDistance =
            999999.0f;

        int nearestIsland =
            FindNearestUnownedIsland(
                player.position,
                islands,
                islandDistance
            );

        if (
            nearestIsland !=
            -1 &&
            islandDistance <
            7.5f
        )
        {
            const Island& island =
                islands[
                    nearestIsland
                ];

            ThemeInfo theme =
                GetThemeInfo(
                    island.theme
                );

            DrawRectangle(
                470,
                SCREEN_HEIGHT - 150,
                400,
                110,
                Fade(
                    BLACK,
                    0.76f
                )
            );

            DrawText(
                "ISLAND FOR SALE",
                490,
                SCREEN_HEIGHT - 130,
                21,
                YELLOW
            );

            DrawText(
                theme.name,
                490,
                SCREEN_HEIGHT - 101,
                20,
                theme.accentColor
            );

            DrawText(
                (
                    "Price: " +
                    FormatMoney(
                        island.price
                    )
                ).c_str(),
                490,
                SCREEN_HEIGHT - 72,
                18,
                WHITE
            );

            DrawText(
                TextFormat(
                    "%d levels",
                    island.levels
                ),
                700,
                SCREEN_HEIGHT - 72,
                18,
                LIGHTGRAY
            );

            DrawText(
                "Press E to buy",
                700,
                SCREEN_HEIGHT - 101,
                18,
                WHITE
            );
        }

        // ========================================================
        // MESSAGE
        // ========================================================

        if (
            messageTimer >
            0.0f
        )
        {
            int textWidth =
                MeasureText(
                    message.c_str(),
                    20
                );

            DrawRectangle(
                SCREEN_WIDTH / 2 -
                    textWidth / 2 -
                    24,
                SCREEN_HEIGHT - 215,
                textWidth + 48,
                44,
                Fade(
                    BLACK,
                    0.78f
                )
            );

            DrawText(
                message.c_str(),
                SCREEN_WIDTH / 2 -
                    textWidth / 2,
                SCREEN_HEIGHT - 203,
                20,
                WHITE
            );
        }

        // ========================================================
        // CROSSHAIR
        // ========================================================

        if (
            !shopOpen &&
            !admin.panelOpen
        )
        {
            Vector2 mouse =
                GetMousePosition();

            DrawLine(
                static_cast<int>(
                    mouse.x - 7
                ),
                static_cast<int>(
                    mouse.y
                ),
                static_cast<int>(
                    mouse.x + 7
                ),
                static_cast<int>(
                    mouse.y
                ),
                WHITE
            );

            DrawLine(
                static_cast<int>(
                    mouse.x
                ),
                static_cast<int>(
                    mouse.y - 7
                ),
                static_cast<int>(
                    mouse.x
                ),
                static_cast<int>(
                    mouse.y + 7
                ),
                WHITE
            );
        }

        // ========================================================
        // SHOP
        // ========================================================

        if (shopOpen)
        {
            DrawRectangle(
                390,
                80,
                500,
                555,
                Fade(
                    BLACK,
                    0.93f
                )
            );

            DrawRectangleLines(
                390,
                80,
                500,
                555,
                WHITE
            );

            DrawText(
                "SEED SHOP",
                420,
                108,
                28,
                WHITE
            );

            DrawText(
                "Press 1-8 to buy seeds",
                420,
                145,
                18,
                LIGHTGRAY
            );

            PlantType shopPlants[] =
            {
                PlantType::Sunleaf,
                PlantType::Berry,
                PlantType::Cactus,
                PlantType::Mushroom,
                PlantType::Inferno,
                PlantType::GoldenFlower,
                PlantType::CrystalBloom,
                PlantType::VoidOrchid
            };

            for (
                int i = 0;
                i < PLANT_COUNT;
                i++
            )
            {
                PlantInfo info =
                    GetPlantInfo(
                        shopPlants[i]
                    );

                int y =
                    185 +
                    i * 46;

                DrawText(
                    TextFormat(
                        "%d.",
                        i + 1
                    ),
                    420,
                    y,
                    18,
                    WHITE
                );

                DrawText(
                    info.name,
                    455,
                    y,
                    18,
                    info.color
                );

                DrawText(
                    FormatMoney(
                        info.seedPrice
                    ).c_str(),
                    705,
                    y,
                    18,
                    GOLD
                );

                DrawText(
                    TextFormat(
                        "x%d",
                        seeds[i]
                    ),
                    810,
                    y,
                    18,
                    WHITE
                );
            }

            DrawText(
                "B = close shop",
                420,
                595,
                18,
                LIGHTGRAY
            );
        }

        // ========================================================
        // ADMIN PANEL
        // ========================================================

        if (admin.panelOpen)
        {
            DrawRectangle(
                330,
                95,
                620,
                530,
                Fade(
                    BLACK,
                    0.95f
                )
            );

            DrawRectangleLines(
                330,
                95,
                620,
                530,
                WHITE
            );

            DrawText(
                "GRAVITY_INT ADMIN PANEL",
                370,
                125,
                28,
                YELLOW
            );

            if (
                !admin.authenticated
            )
            {
                DrawText(
                    "ADMIN LOGIN",
                    390,
                    190,
                    22,
                    WHITE
                );

                DrawText(
                    "USERNAME",
                    390,
                    230,
                    15,
                    LIGHTGRAY
                );

                DrawRectangle(
                    390,
                    252,
                    500,
                    48,
                    Fade(
                        DARKGRAY,
                        0.65f
                    )
                );

                DrawText(
                    admin.username.c_str(),
                    405,
                    265,
                    20,
                    WHITE
                );

                DrawText(
                    "PASSWORD",
                    390,
                    322,
                    15,
                    LIGHTGRAY
                );

                Color passwordColor =
                    admin.focus ==
                    AdminFocus::Password
                    ? Fade(
                        SKYBLUE,
                        0.30f
                    )
                    : Fade(
                        DARKGRAY,
                        0.65f
                    );

                DrawRectangle(
                    390,
                    340,
                    500,
                    48,
                    passwordColor
                );

                std::string passwordStars(
                    admin.password.size(),
                    '*'
                );

                DrawText(
                    passwordStars.c_str(),
                    405,
                    353,
                    20,
                    WHITE
                );

                DrawText(
                    "Click the password field and type the password.",
                    390,
                    405,
                    16,
                    LIGHTGRAY
                );

                DrawRectangle(
                    390,
                    420,
                    500,
                    52,
                    Color{
                        40,
                        120,
                        75,
                        255
                    }
                );

                DrawText(
                    "LOGIN",
                    610,
                    434,
                    19,
                    WHITE
                );

                DrawText(
                    "ENTER = login",
                    390,
                    505,
                    17,
                    LIGHTGRAY
                );

                DrawText(
                    "F10 = close panel",
                    390,
                    535,
                    17,
                    LIGHTGRAY
                );
            }
            else
            {
                DrawText(
                    "ACCESS GRANTED",
                    390,
                    190,
                    23,
                    GREEN
                );

                DrawText(
                    "F1   +$1,000,000",
                    390,
                    240,
                    20,
                    WHITE
                );

                DrawText(
                    "F2   +100 EVERY SEED",
                    390,
                    273,
                    20,
                    WHITE
                );

                DrawText(
                    "F3   TRIGGER EVENT",
                    390,
                    306,
                    20,
                    WHITE
                );

                DrawText(
                    "F4   SPAWN 20 DROPS",
                    390,
                    339,
                    20,
                    WHITE
                );

                DrawText(
                    "F5   UNLOCK ALL ISLANDS",
                    390,
                    372,
                    20,
                    WHITE
                );

                DrawText(
                    "F6   TELEPORT HOME",
                    390,
                    405,
                    20,
                    WHITE
                );

                DrawText(
                    "F10 = close panel",
                    390,
                    470,
                    18,
                    LIGHTGRAY
                );

                DrawText(
                    "ACCOUNT: GRAVITY_INT",
                    390,
                    525,
                    18,
                    YELLOW
                );
            }
        }

        // ========================================================
        // FPS
        // ========================================================

        DrawFPS(
            SCREEN_WIDTH - 100,
            SCREEN_HEIGHT - 28
        );

        EndDrawing();
    }

    CloseWindow();

    return 0;
}