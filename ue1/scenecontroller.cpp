#include "scenecontroller.h"

using namespace std;

SceneController::SceneController(QWidget *parent, shared_ptr<Scene> scene)
    : parent_(parent), scene_(scene), timer_()

{
    // initialize navigation controllers
    cameraNavigator_ = make_unique<RotateY>(scene_->nodes_["Camera"], nullptr, nullptr);
    cameraNavigator_->setDistance(3.0);
}

void
SceneController::timerEvent(QTimerEvent *)
{
    // the timer is just suppsed to trigger a redraw of the widget/scene
    updateWidget();
}

void
SceneController::setSceneNode(QString node)
{
    auto n = scene_->nodes_[node];
    assert(n); // will crash if invalid string was provided

    scene_->nodes_["Scene"]->children.clear();
    scene_->nodes_["Scene"]->children.push_back(n);

    updateWidget();
}

// change background color
void
SceneController::setBackgroundColor(QVector3D rgb)
{
    scene_->bgcolor_ = rgb;
    updateWidget();
}

void
SceneController::toggleAnimation(bool flag)
{
    if(flag) {
        timer_.start(1000.0 / 60.0, this); // update *roughly* every 1/60th sec
    } else {
        timer_.stop();
    }
}

// methods to change common material parameters
void
SceneController::setLightIntensity(size_t i, float v)
{
    // silently ignore wrong light IDs (...)
    if(i>=scene_->lightNodes_.size())
        return;

    scene_->material_->lights[i].intensity = v;
    updateWidget();
}

void
SceneController::keyPressEvent(QKeyEvent *event)
{
    cameraNavigator_->keyPressEvent(event);
    updateWidget();
}

void
SceneController::mousePressEvent(QMouseEvent *)
{
}

void
SceneController::mouseMoveEvent(QMouseEvent *)
{
}

void
SceneController::mouseReleaseEvent(QMouseEvent *)
{
}

void
SceneController::wheelEvent(QWheelEvent *)
{
}

void
SceneController::updateViewport(size_t width, size_t height)
{
    glViewport(0,0,GLint(width),GLint(height));
}

void
SceneController::updateWidget()
{
    parent_->update();
}

void
SceneController::draw()
{
    scene_->draw_();
}
