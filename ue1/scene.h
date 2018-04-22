#pragma once

#include <QWidget>

#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QTimer>
#include <QOpenGLFramebufferObject>

#include "node.h"
#include "camera.h"
#include "material/phong.h"
#include "material/toonmaterial.h"
#include "navigator/rotate_y.h"

#include <memory> // std::unique_ptr
#include <map>    // std::map
#include <chrono> // clock, time calculations

/*
 * OpenGL-based scene. Required objects are created in the constructor,
 * and the scene is rendered using draw().
 *
 * Do not call draw() directly, call SceneController::updateWidget() instead.
 *
 */

class Scene : protected QOpenGLFunctions
{

    // Allow SceneController to access protected members.
    friend class SceneController;

public:

    /**
     * @brief Scene constructor builds up the initial scene content including camera and light
     * @param parent the parent widget required to query rendering viewport size
     */
    explicit Scene(QWidget* parent);

protected:

    // ==== note: all protected attributes may be altered directly by SceneController ==== //

    /*
     *  perform OpenGL rendering of the entire scene.
     *  Don't call this yourself (!) - SceneController::update() should be used instead.
     *
     */
    void draw_();

    // bg color
    QVector3D bgcolor_ = QVector3D(0.4f,0.4f,0.4f);

    // Phong material to be used on all objects
    std::shared_ptr<PhongMaterial> material_;
    std::shared_ptr<ToonMaterial> materialToon_;

    // mesh(es) to be used / shared
    std::map<QString, std::shared_ptr<Mesh>> meshes_;

    // nodes to be used
    std::map<QString, std::shared_ptr<Node>> nodes_;

    // light nodes for any number of lights
    std::vector<std::shared_ptr<Node>> lightNodes_;


private:

    // for querying the viewport size
    QWidget* parent_;

    // construct main camera based on scene graph
    Camera createCamera();

    // draw the actual scene, called from within draw()
    void draw_scene_();

    // track time for animations
    std::chrono::high_resolution_clock clock_;
    std::chrono::time_point<std::chrono::high_resolution_clock> firstDrawTime_;
    std::chrono::time_point<std::chrono::high_resolution_clock> lastDrawTime_;

};

