#pragma once

#include "material/material.h"

#include <QOpenGLTexture>


class PhongMaterial : public Material {
public:

    // constructor requires existing shader program
    PhongMaterial(std::shared_ptr<QOpenGLShaderProgram> prog, int num_lights=1);

    // bind underlying shader program and set required uniforms
    void apply(unsigned int light_pass = 0) override;

    // ambient light
    QVector3D ambientLightIntensity = QVector3D(0.3f,0.3f,0.3f);

    // properties of the Phong aspects of the material
    struct Phong {
        QVector3D k_ambient  = QVector3D(0.50f,0.10f,0.10f); // red-ish
        QVector3D k_diffuse  = QVector3D(0.50f,0.10f,0.10f); // red-ish
        QVector3D k_specular = QVector3D(0.80f,0.80f,0.80f); // white-ish
        float     shininess  = 80; // middle-ish
    } phong;

    // vector of point lights used to light the material
    struct PointLight {
        QVector4D position_WC = QVector4D(0,1,5,1);
        QVector3D color = QVector3D(1,1,1);
        float intensity = 0.5;
    };
    std::vector<PointLight> lights;

};


