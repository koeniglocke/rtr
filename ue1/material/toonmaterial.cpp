#include "toonmaterial.h"

ToonMaterial::ToonMaterial(std::shared_ptr<QOpenGLShaderProgram> prog, int num_lights)
    : PhongMaterial(prog, num_lights)
{

}

void ToonMaterial::apply(unsigned int light_pass)
{
    // first do what Material needs to do (i.e. bind program, set animation time)
    PhongMaterial::apply(light_pass);

    // globals
    prog_->setUniformValue("myWonderfulTestColor", myTestColor);
}
