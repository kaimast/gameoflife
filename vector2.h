#ifndef VECTOR2_H
#define VECTOR2_H

#include <cmath>

class vector2
{
public:
    vector2(int32_t x, int32_t y) : X(x), Y(y)
    {
    }

    vector2(int64_t flatInt)
    {
        X = static_cast<int32_t>(flatInt >> sizeof(uint32_t)*8);
        Y = static_cast<int32_t>(flatInt);
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

    inline int64_t toFlatInt() const
    {
        return (static_cast<uint64_t>(X) << sizeof(uint32_t)*8) + Y;
    }
};

inline vector2 toTilePosition(const vector2& pos, uint32_t tileSize)
{
    float x = pos.X;
    float y = pos.Y;

    vector2 tilepos(floor(x / tileSize), floor(y / tileSize));
    return tilepos;
}

inline vector2 toRelativePosition(const vector2& pos, uint32_t tileSize)
{
    vector2 tilePos = toTilePosition(pos, tileSize);
    return vector2(pos.X - tilePos.X * tileSize, pos.Y - tilePos.Y * tileSize);
}

#endif // VECTOR2_H
