#include "scene.h"

#include <iostream> // std::cout etc.
#include <assert.h> // assert()
#include <random>   // random number generation

#include <QtMath>
#include <QMessageBox>

#include "rtr_util.h"
#include "geometry/cube.h" // geom::Cube
#include "material/phong.h"
#include "material/toonmaterial.h"


using namespace std;

Scene::Scene(QWidget* parent) :
    QOpenGLFunctions(QOpenGLContext::currentContext()),
    parent_(parent),
    firstDrawTime_(clock_.now()),
    lastDrawTime_(clock_.now())
{

    // load shader source files and compile them into OpenGL program objects
    auto phong_prog = createShaderProgram(":/shaders/phong.vert", ":/shaders/phong.frag");
    auto toon_prog = createShaderProgram(":/shaders/toon.vert", ":/shaders/toon.frag");

    // Phong materials
    material_ = make_shared<PhongMaterial>(phong_prog, 1);
    material_->phong.k_diffuse = QVector3D(0.8f,0.1f,0.1f);
    material_->phong.k_ambient = material_->phong.k_diffuse * 0.3f;
    material_->phong.shininess = 80;

    materialToon_ = make_shared<ToonMaterial>(toon_prog, 1);
    //materialToon_->phong.k_diffuse = QVector3D(0.1f,0.8f,0.1f);
    //materialToon_->phong.k_ambient = material_->phong.k_diffuse * 0.3f;
    //materialToon_->phong.shininess = 80;
    materialToon_->myTestColor = QVector3D(0,1,0);

    // which material to use as default for all objects? (just a shortcut)
    auto mat = material_;

    // load meshes from .obj files and assign shader programs to them
    meshes_["Duck"]    = make_shared<Mesh>(":/models/duck/duck.obj", mat);
    meshes_["Teapot"]  = make_shared<Mesh>(":/models/teapot/teapot.obj", materialToon_);
    meshes_["Goblin"]  = make_shared<Mesh>(":/models/goblin/goblin.obj", mat);

    // add meshes of some procedural geometry objects (not loaded from OBJ files)
    meshes_["Cube"]   = make_shared<Mesh>(make_shared<geom::Cube>(), mat);

    // pack each mesh into a scene node, scaled to standard size [1,1,1]
    nodes_["Cube"]    = make_shared<Node>(meshes_["Cube"],   Node::Transform::ScaleToOne);
    nodes_["Duck"]    = make_shared<Node>(meshes_["Duck"],   Node::Transform::ScaleToOne);
    nodes_["Teapot"]  = make_shared<Node>(meshes_["Teapot"], Node::Transform::ScaleToOne);
    nodes_["Goblin"]  = make_shared<Node>(meshes_["Goblin"], Node::Transform::ScaleToOne);

    // world contains the scene plus the camera
    nodes_["World"] = make_shared<Node>();

    // scene means everything but the main camera
    nodes_["Scene"] = make_shared<Node>();
    nodes_["World"]->children.push_back(nodes_["Scene"]);

    // add main camera node
    nodes_["Camera"] = make_shared<Node>();
    nodes_["World"]->children.push_back(nodes_["Camera"]);

    // add a light relative to the scene or world or camera
    nodes_["Light0"] = make_shared<Node>();
    lightNodes_.push_back(nodes_["Light0"]);

    // light attached to camera, placed right above camera
    nodes_["Camera"]->children.push_back(nodes_["Light0"]);
    nodes_["Light0"]->transformation.translate(QVector3D(0, 1, 0));

}

// this method is called implicitly when a redraw is required.
// the call goes rtrGLWidget.paintGL() -> SceneController.draw() -> Scene.draw()
void Scene::draw_()
{
    // calculate animation time
    chrono::milliseconds millisec_since_first_draw;
    chrono::milliseconds millisec_since_last_draw;

    // calculate total elapsed time and time since last draw call
    auto current = clock_.now();
    millisec_since_first_draw = chrono::duration_cast<chrono::milliseconds>(current - firstDrawTime_);
    millisec_since_last_draw = chrono::duration_cast<chrono::milliseconds>(current - lastDrawTime_);
    lastDrawTime_ = current;

    // set time uniform in animated shader(s)
    float t = millisec_since_first_draw.count() / 1000.0f;
    material_->time = t;

    draw_scene_();
}

// set camera based on node position in scene graph
Camera
Scene::createCamera() {

    // derive aspect ratio from viewport size
    float aspect = float(parent_->width())/float(parent_->height());

    // create projection matrix based on 30 degrees field of view
    QMatrix4x4 projectionMatrix;
    projectionMatrix.perspective(30.0f,aspect,0.01f,1000.0f);

    // derive camera position from scene node world coordinates
    auto camToWorld = nodes_["World"]->toParentTransform(nodes_["Camera"]);
    auto viewMatrix = camToWorld.inverted();

    return Camera(viewMatrix, projectionMatrix);
}

void Scene::draw_scene_()
{
    Camera camera = createCamera();

    // clear buffer
    glClearColor(bgcolor_[0], bgcolor_[1], bgcolor_[2], 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // first light pass: standard depth test, no blending
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glDisable(GL_CULL_FACE);

    // draw one pass for each light
    for(unsigned int i=0; i<lightNodes_.size(); i++) {

        // determine current light position in world coordinates
        QVector3D lightPosWC = nodes_["World"]->toParentTransform(lightNodes_[i]) * QVector3D(0,0,0);

        // set light position in material
        material_->lights[i].position_WC = lightPosWC;

        // draw light pass i
        nodes_["World"]->draw(camera, i);

        // settings for i>0 (add light contributions using alpha blending)
        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE,GL_ONE);
        glDepthFunc(GL_EQUAL);
    }
}







