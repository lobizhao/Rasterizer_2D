#include "segment.h"

Segment::Segment(const glm::vec2& p1, const glm::vec2& p2)
    : endpoint1(p1), endpoint2(p2), dX(p2.x - p1.x), dY(p2.y - p1.y) {}

// Function to compute intersection with a horizontal line
bool Segment::getIntersection(int y, float* x) const {
    if (dY == 0)  // Horizontal line check; no vertical intersection
        return false;

    // The line's y range must include 'y' to have an intersection
    if ((y < glm::min(endpoint1.y, endpoint2.y)) || (y > glm::max(endpoint1.y, endpoint2.y)))
        return false;

    // Compute the intersection x using the line equation
    float t = (y - endpoint1.y) / dY;
    *x = endpoint1.x + t * dX;
    return true;
}
