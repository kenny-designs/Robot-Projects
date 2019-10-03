#ifndef VECTOR2_H
#define VECTOR2_H
#pragma once

/**
 * Struct used to represent a vector in 2D space
 */
struct Vector2 
{
  double x, y;

  Vector2(double xPos, double yPos) : x(xPos), y(yPos) {};

  /** Overload - operator to subtract the Vector2 by another Vector2 */
  Vector2 operator-(Vector2 const& subtrahend)
  { 
    return Vector2(x - subtrahend.x, y - subtrahend.y);
  }

  /** Overload -= operator to subtract the Vector2 by another Vector2 */
  Vector2& operator-=(Vector2 const& subtrahend)
  { 
    this->x -= subtrahend.x;
    this->y -= subtrahend.y;
    return *this;
  }

  /** Overload / operator to divide the Vector2 by a single divisor */
  Vector2 operator/(double divisor)
  {
    return Vector2(x / divisor, y / divisor);
  }

  /** Overload /= operator to divide the Vector2 by a single divisor */
  Vector2& operator/=(double divisor)
  { 
    this->x /= divisor;
    this->y /= divisor;
    return *this;
  }

  double getMagnitude();
};

#endif
