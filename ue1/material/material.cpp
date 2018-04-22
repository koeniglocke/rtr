#include "material.h"
#include <assert.h>


void Material::apply(unsigned int )
{
    prog_->bind();

    // animation time is the only property shared by all materials
    prog_->setUniformValue("time", time);
}

