#pragma once
#include <polygon.h>
#include <segment.h>
#include <QImage>
#include <array>
#include <glm/glm.hpp>
#include <camera.h>

class Rasterizer {
public:
    Rasterizer(const std::vector<Polygon>& polygons);
    ~Rasterizer();
    QImage RenderScene();
    void ClearScene();
    Camera& getCamera();


private:
    std::vector<Polygon> m_polygons;
    std::vector<float> m_zBuffer;
    //add camera
    Camera m_camera;

    std::array<float, 4> boudingBox(glm::vec2& v1, glm::vec2& v2, glm::vec2& v3);
    glm::vec3 BarycentricInterpolation(glm::vec2& v1, glm::vec2& v2, glm::vec2& v3,glm::vec2& point);
    bool IsInsideTriangle(glm::vec2& p1, glm::vec2& p2, glm::vec2& p3,glm::vec2& point);
    float ComputeArea(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c);

};
