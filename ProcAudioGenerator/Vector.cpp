#include "Vector.h"
#include <cmath>

Vec2 operator+(const Vec2& lhs, const Vec2& rhs)
{
    return Vec2(lhs.x + rhs.x, lhs.y + rhs.y);
}

Vec2 operator+(const Vec2& lhs, const float rhs)
{
    return Vec2(lhs.x + rhs, lhs.x + rhs);
}

Vec2 Vec2::operator+=(const Vec2& other)
{
    x += other.x;
    y += other.y;
    return *this;
}

Vec2 operator-(const Vec2& lhs, const Vec2& rhs)
{
    return Vec2(lhs.x - rhs.x, lhs.y - rhs.y);
}

Vec2 operator-(const Vec2& lhs, const float rhs)
{
    return Vec2(lhs.x - rhs, lhs.y - rhs);
}

Vec2 Vec2::operator-=(const Vec2& other)
{
    x -= other.x;
    y -= other.y;
    return *this;
}

Vec2 Vec2::operator-() const
{
    return Vec2(-x, -y);
}

Vec2 Vec2::operator*=(const float other)
{
    x *= other;
    y *= other;
    return *this;
}

Vec2 operator*(const float lhs, const Vec2& rhs)
{
    return Vec2(rhs.x * lhs, rhs.y * lhs);
}

Vec2 operator*(const Vec2& lhs, const float rhs)
{
    return rhs * lhs;
}

Vec2 operator*(const Vec2& lhs, const Vec2& rhs)
{
    return Vec2(lhs.x * rhs.x, lhs.y * rhs.y);
}

Vec2 Vec2::operator*=(const Vec2& other)
{
    x *= other.x;
    y *= other.y;
    return *this;
}

Vec2 operator/(const Vec2& lhs, const float rhs)
{
    return Vec2(lhs.x / rhs, lhs.y / rhs);
}

Vec2 Vec2::operator/=(const float& other)
{
    x /= other;
    y /= other;
    return *this;
}

bool Vec2::operator==(const Vec2& other) const
{
    if (x != other.x)
    {
        return false;
    }

    if (y != other.y)
    {
        return false;
    }

    return true;
}

float Vec2::operator[](int i) const
{
    switch (i)
    {
    case 0:
        return x;
        break;
    case 1:
        return y;
        break;
    default:
        break;
    }
    return -1;
}

Vec2& Vec2::operator=(const Vec2 point)
{
    x = point.x;
    y = point.y;
    return *this;
}

float Vec2::X() const
{
    return x;
}

float Vec2::Y() const
{
    return y;
}

void Vec2::Normalise()
{
    float length = Length();
    if (length != 0) {
        x /= length;
        y /= length;
    }
}

float Vec2::Length() const
{
    return sqrtf(x * x + y * y);
}

float Vec2::LengthSqr() const
{
    return x * x + y * y;
}

Vec2 Vec2::Inverse() const
{
    return Vec2(x ? -x : 0, y ? -y : 0);
}

Vec2 VecBetween(const Vec2 start, const Vec2 end)
{
    return end - start;
}

float AngleBetween(const Vec2 vecA, const Vec2 vecB)
{
    return acosf(DotProduct(vecA, vecB) / (vecA.Length() * vecB.Length()));
}

float DistBetween(const Vec2 vecA, const Vec2 vecB)
{
    return (vecA - vecB).Length();
}

float DistBetweenSqr(const Vec2 vecA, const Vec2 vecB)
{
    return (vecA - vecB).LengthSqr();
}

Vec2 MidPoint(const Vec2 pointA, const Vec2 pointB)
{
    return (pointA + pointB) / 2;
}

float DotProduct(const Vec2 vecA, const Vec2 vecB)
{
    return vecA.x * vecB.x + vecA.y * vecB.y;
}

float CrossProduct(const Vec2 vecA, const Vec2 vecB)
{
    return vecA.x * vecB.y - vecA.y * vecB.x;
}

Vec2 CrossProduct(const float floatA, const Vec2 vecB)
{
    return Vec2(-floatA * vecB.y, floatA * vecB.x);
}

Vec2 CrossProduct(const Vec2 vecA, const float floatB)
{
    return Vec2(floatB * vecA.y, -floatB * vecA.x);
}

Vec2 TripleProduct(const Vec2 vecA, const Vec2 vecB, const Vec2 vecC)
{
    return (vecB * DotProduct(vecA, vecC)) - (vecC * DotProduct(vecA, vecB));
}

bool SameDirection(const Vec2& dir, const Vec2& vec)
{
    return DotProduct(dir, vec) > 0;
}