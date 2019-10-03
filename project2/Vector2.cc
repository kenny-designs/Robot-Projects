#include "Vector2.h"
#include <cmath> // hypot()

/**
 * Gets the magnitude, or distance, between points x and y
 *
 * @return Magnitude of the vector
 */ 
double Vector2::getMagnitude()
{
  return hypot(x, y);
}
