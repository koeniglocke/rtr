#pragma once

#include "material/phong.h"
#include <QVector3D>


class ToonMaterial : public PhongMaterial
{
public:
    // constructor requires existing shader program
    ToonMaterial(std::shared_ptr<QOpenGLShaderProgram> prog, int num_lights=1);

    // bind underlying shader program and set required uniforms
    void apply(unsigned int light_pass = 0) override;

    //just a test
    QVector3D myTestColor;
};
