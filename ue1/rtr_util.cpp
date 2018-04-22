#include "rtr_util.h"
#include <iostream>

#include <QOpenGLFunctions>

using namespace std;

shared_ptr<QOpenGLShaderProgram>
createShaderProgram(const string& vertex_file,
                    const string& fragment_file,
                    const string& geom_file)
{
    // internal helper struct
    struct ShaderInfo {
        string filename;
        string typeinfo;
        QOpenGLShader::ShaderType type;
    };

    // create empty shader program
    auto p = make_shared<QOpenGLShaderProgram>();

    // which file is which shader type
    auto shaders = {
                     ShaderInfo({vertex_file,   "Vertex",   QOpenGLShader::Vertex}),
                     ShaderInfo({fragment_file, "Fragment", QOpenGLShader::Fragment}),
                     ShaderInfo({geom_file,     "Geometry", QOpenGLShader::Geometry})
                   };

    // compile and add them to the program, shader file by shader file
    for(auto shader : shaders) {
        if(!shader.filename.empty()) {
            if(!p->addShaderFromSourceFile(shader.type, shader.filename.c_str())) {
                cout << string("could not add/compile ") + shader.typeinfo + " shader" << endl;
                qFatal("failed to create shader program");
            }
        }
    }

    // try to link the compiled shaders to a GPU-executable program
    if(!p->link())
        qFatal("could not link shader program");

    // everything OK, so return valid shader program
    return p;
}

void print_OpenGL_info()
{
    QOpenGLFunctions gl(QOpenGLContext::currentContext());

    int minor, major;
    gl.glGetIntegerv(GL_MINOR_VERSION, &minor);
    gl.glGetIntegerv(GL_MAJOR_VERSION, &major);
    cout << "OpenGL context version " << major << "." << minor << endl;

    int texunits_frag, texunits_vert;
    gl.glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &texunits_frag);
    gl.glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &texunits_vert);
    cout << "texture units: " << texunits_frag << " (frag), "
         << texunits_vert << " (vert)" << endl;

    int texsize;
    gl.glGetIntegerv(GL_MAX_TEXTURE_SIZE, &texsize);
    cout << "max texture size: " << texsize << "x" << texsize << endl;

}
