#pragma once

class Vec2
{
public:
    Vec2() : x(0.0), y(0.0) {}
    Vec2(float x, float y) : x(x), y(y) {}

    Vec2 operator+=(const Vec2& other);

    Vec2 operator-=(const Vec2& other);

    Vec2 operator-() const;

    Vec2 operator*=(const float other);

    Vec2 operator*=(const Vec2& other);

    Vec2 operator/=(const float& other);

    bool operator==(const Vec2& other) const;

    float operator[](int i) const;

    Vec2& operator=(const Vec2 point);

    float X() const;

    float Y() const;

    void Normalise();

    float Length() const;

    float LengthSqr() const;

    Vec2 Inverse() const;

    float x, y;
};

Vec2 operator+(const Vec2& lhs, const float rhs);
Vec2 operator+(const Vec2& lhs, const Vec2& rhs);

Vec2 operator-(const Vec2& lhs, const Vec2& rhs);
Vec2 operator-(const Vec2& lhs, const float rhs);

Vec2 operator*(const float lhs, const Vec2& rhs);
Vec2 operator*(const Vec2& lhs, const float rhs);

Vec2 operator*(const Vec2& lhs, const Vec2& rhs);

Vec2 operator/(const Vec2& lhs, const float rhs);

Vec2 VecBetween(const Vec2 start, const Vec2 end);

float AngleBetween(const Vec2 vecA, const Vec2 vecB);

Vec2 MidPoint(const Vec2 pointA, const Vec2 pointB);

float DistBetween(const Vec2 vecA, const Vec2 vecB);

float DistBetweenSqr(const Vec2 vecA, const Vec2 vecB);

float DotProduct(const Vec2 vecA, const Vec2 vecB);

float CrossProduct(const Vec2 vecA, const Vec2 vecB);

Vec2 CrossProduct(const float floatA, const Vec2 vecB);
Vec2 CrossProduct(const Vec2 vecA, const float floatB);

Vec2 TripleProduct(const Vec2 vecA, const Vec2 vecB, const Vec2 vecC);

bool SameDirection(const Vec2& dir, const Vec2& vec);