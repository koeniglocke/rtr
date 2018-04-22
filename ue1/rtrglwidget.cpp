
#include "rtrglwidget.h"
#include "scene.h"
#include "scenecontroller.h"
#include "rtr_util.h"

#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QMouseEvent>

#include <iostream>

using namespace std;

rtrGLWidget::rtrGLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
}

rtrGLWidget::~rtrGLWidget()
{
}

SceneController&
rtrGLWidget::sceneController()
{
    return *scene_controller_;
}


QSize rtrGLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize rtrGLWidget::sizeHint() const
{
    return QSize(200, 200);
}

void rtrGLWidget::initializeGL()
{

    // this has to be done before any other GL call is issued
    initializeOpenGLFunctions();
    assert(QOpenGLContext::currentContext());

    // check some OpenGL parameters
    print_OpenGL_info();

    // debut output: supported GLSL version
    const unsigned char* glsl_ver = glGetString(GL_SHADING_LANGUAGE_VERSION);
    cout << "GLSL version: " << glsl_ver << endl;

    // create and initialize logger
    logger = new QOpenGLDebugLogger(this);
    logger->initialize();

    // extension required to do logging :-(
    if(!context()->hasExtension(QByteArrayLiteral("GL_KHR_debug"))) {
        // cout << "OpenGL debug extension not available" << endl;
    } else {
        cout << "OpenGL debug extension available" << endl;
        // start logging
        // connect(logger, &QOpenGLDebugLogger::messageLogged, [](const QOpenGLDebugMessage &msg) {
        //     qDebug() << msg;
        // });
        // logger->startLogging();
    }

    // scene will call OpenGL functions, so it cannot be
    // instantiated before we have initialized OpenGL
    scene_controller_ = make_shared<SceneController>(this, make_shared<Scene>(this));

}

void rtrGLWidget::paintGL()
{
    scene_controller_->draw();
}

void rtrGLWidget::resizeGL(int width, int height)
{
    scene_controller_->updateViewport(width,height);
}

void rtrGLWidget::mousePressEvent(QMouseEvent* event)
{
    scene_controller_->mousePressEvent(event);
}

void rtrGLWidget::mouseMoveEvent(QMouseEvent* event)
{
    scene_controller_->mouseMoveEvent(event);
}

void rtrGLWidget::mouseReleaseEvent(QMouseEvent* event)
{
    scene_controller_->mouseReleaseEvent(event);
}

void rtrGLWidget::wheelEvent(QWheelEvent* event)
{
    scene_controller_->wheelEvent(event);
}
