#pragma once

struct Vector2Int {
    int64_t x;
    int64_t y;
};

struct Vector3 {
    union {
        struct {
            float x, y, z;
        };
        struct {
            float r, g, b;
        };
    };
};

struct Vector2 {
    union {
        struct {
            double x, y;
        };
        struct {
            double r, g;
        };
    };
};