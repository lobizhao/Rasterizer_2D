#ifndef SEGMENT_H
#define SEGMENT_H
#include <glm/glm.hpp>

class Segment
{
private:
    glm::vec2 endpoint1;
    glm::vec2 endpoint2;
    float dX;  // Change in X (delta X)
    float dY;  // Change in Y (delta Y)
public:
    // Constructor with initialization list
    Segment(const glm::vec2& p1, const glm::vec2& p2);

    // Function to get intersection with a horizontal line
    bool getIntersection(int y, float* x) const;

};

#endif // SEGMENT_H
