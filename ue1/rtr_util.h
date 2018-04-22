#pragma once

#include <memory>
#include <string>

#include <QOpenGLShaderProgram>

/**
  * helper function to create shader program from source files
  *
  * reads vertex, fragment, and geometry shader source files,
  * and compiles them into a shader program
  *
  * return value: no need to check for nullptr.
  * function produces a fatal error if one of the files cannot be compiled
  * or if there is a linking error. Errors are printed out on the console.
  */

std::shared_ptr<QOpenGLShaderProgram>
createShaderProgram(const std::string& vertex_file,
                    const std::string& fragment_file,
                    const std::string& geom_file = "");


void print_OpenGL_info();
