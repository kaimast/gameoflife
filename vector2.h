#ifndef VECTOR2_H
#define VECTOR2_H

#include <cmath>

class vector2
{
public:
    vector2(int32_t x, int32_t y) : X(x), Y(y)
    {
    }

    int32_t X;
    int32_t Y;

    vector2 operator*(int32_t scalar) const
    {
        return vector2(X * scalar, Y * scalar);
    }

    vector2 operator+(const vector2& other) const
    {
        return vector2(X + other.X, Y + other.Y);
    }
};

inline vector2 toTilePosition(const vector2& pos)
{
    float x = pos.X;
    float y = pos.Y;

    vector2 tilepos(floor(x / TILE_SIZE), floor(y / TILE_SIZE));
    return tilepos;
}

inline vector2 toRelativePosition(const vector2& pos)
{
    vector2 tilePos = toTilePosition(pos);
    return vector2(pos.X - tilePos.X * TILE_SIZE, pos.Y - tilePos.Y * TILE_SIZE);
}

#endif // VECTOR2_H
