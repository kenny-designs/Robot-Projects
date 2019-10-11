#ifndef VECTOR2_H
#define VECTOR2_H
#pragma once

/**
 * Struct used to represent a vector in 2D space
 */
struct Vector2 
{
  double x, y;

  Vector2() : x(0), y(0) {};
  Vector2(double xPos, double yPos) : x(xPos), y(yPos) {};

  // Overload subtraction operators
  Vector2  operator-(Vector2 const& subtrahend);
  Vector2& operator-=(Vector2 const& subtrahend);

  // Overload division operators
  Vector2  operator/(double divisor);
  Vector2& operator/=(double divisor);

  // Overload comparison
  bool operator==(Vector2 const& other);
  bool operator!=(Vector2 const& other);

  // Static helper methods
  static double getMagnitude(Vector2 const& vec2);
  static void normalize(Vector2& vec2);
};

#endif
