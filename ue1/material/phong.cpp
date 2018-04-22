#include "material/phong.h"
#include <assert.h>

PhongMaterial::PhongMaterial(std::shared_ptr<QOpenGLShaderProgram> prog, int num_lights)
    : Material(prog), lights(num_lights)
{

}

void PhongMaterial::apply(unsigned int light_pass)
{
    // first do what Material needs to do (i.e. bind program, set animation time)
    Material::apply(light_pass);

    // globals
    prog_->setUniformValue("ambientLightIntensity", ambientLightIntensity);

    // point light
    assert(light_pass>=0 && light_pass<lights.size());
    prog_->setUniformValue("light.position_WC", lights[light_pass].position_WC);
    prog_->setUniformValue("light.intensity", lights[light_pass].color * lights[light_pass].intensity);
    prog_->setUniformValue("light.pass", light_pass);

    // Phong
    prog_->setUniformValue("phong.k_ambient",  phong.k_ambient);
    prog_->setUniformValue("phong.k_diffuse",  phong.k_diffuse);
    prog_->setUniformValue("phong.k_specular", phong.k_specular);
    prog_->setUniformValue("phong.shininess",  phong.shininess);

}



