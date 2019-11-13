#include "Vector2.h"
#include <limits> // std::numeric_limits
#include <cmath>  // hypot()

// used for comparing doubles to zero
#define EPSILON std::numeric_limits<double>::epsilon()

/**
 * Subtracts one Vector2 from another
 *
 * @return A new Vector2 object as a result of the subtraction.
 */
Vector2 Vector2::operator-(Vector2 const& subtrahend)
{ 
  return Vector2(x - subtrahend.x, y - subtrahend.y);
}

/**
 * Subtracts from the Vector2 on the left side of the -= operator by the one on the right.
 *
 * @return A reference to the Vector2 object that was subtracted from.
 */
Vector2& Vector2::operator-=(Vector2 const& subtrahend)
{ 
  this->x -= subtrahend.x;
  this->y -= subtrahend.y;
  return *this;
}

/**
 * Divides the Vector2 by a single scalar value.
 *
 * @return A new Vector2 object as a resulting of the division
 */
Vector2 Vector2::operator/(double divisor)
{
  return Vector2(x / divisor, y / divisor);
}

/**
 * Divides the Vector2 on the left side of the /= operator by the scalar divisor on the right.
 *
 * @return A reference to the Vector2 object that was divided
 */
Vector2& Vector2::operator/=(double divisor)
{ 
  this->x /= divisor;
  this->y /= divisor;
  return *this;
}

/**
 * Compares the x and y values of both vectors. Returns true if they are equal.
 *
 * @ return True if the vectors are equal. False otherwise.
 */ 
bool Vector2::operator==(Vector2 const& other)
{
  return fabs(this->x - other.x) < EPSILON &&
         fabs(this->y - other.y) < EPSILON;
}

/**
 * Compares the x and y values of both vectors. Returns true if they are not equal.
 *
 * @ return True if the vectors are not equal. False otherwise.
 */ 
bool Vector2::operator!=(Vector2 const& other)
{
  return !(*this == other);
}

/**
 * Gets the magnitude, or distance, between points x and y of the given vector.
 *
 * @param vec2 - The Vector2 to get the magnitude of
 * @return Magnitude of the vector
 */ 
double Vector2::getMagnitude(Vector2 const& vec2)
{
  return hypot(vec2.x, vec2.y);
}

/**
 * Normalizes the given Vector2.
 *
 * @param vec2 - The Vector2 we wish to normalize
 */ 
void Vector2::normalize(Vector2& vec2)
{
  // find the magnitude of vec2
  double magnitude = Vector2::getMagnitude(vec2);

  // if magnitude is 0, return
  if (magnitude < EPSILON) return;

  // normalize vec2
  vec2 /= magnitude;
}
