#pragma once

#include <QWidget>
#include <QBasicTimer>

#include "scene.h"

/**
 * @brief The SceneController class provides the interface to the UI
 * for setting various parameters and triggering update() calls to
 * trigger eventual redrawing of the scene.
 *
 * It also provides:
 * - a timer that object to call update() periodically.
 * - a navigator object to control the Camera via the keyboard.
 *
 */

class SceneController : public QObject
{

//    Q_OBJECT

public:

    /**
     * @brief SceneController constructor
     * @param parent the parent widget used to request update() of the widget's content
     * @param scene the Scene object to be manipulated by this controller
     */
    SceneController(QWidget* parent, std::shared_ptr<Scene> scene);

signals:


public slots:

    // change the node to be rendered in the scene
    void setSceneNode(QString node);

    // change background color
    void setBackgroundColor(QVector3D rgb);

    // methods to change common material parameters
    void toggleAnimation(bool flag);
    void setLightIntensity(size_t i, float v);

    // key/mouse events from UI system, pass on to navigators or such
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

    // adjust camera / viewport / ... if drawing surface changes
    void updateViewport(size_t width, size_t height);

    // anyone can trigger a redraw of the widget through this method
    void updateWidget();

    // perform the actual draw call. do not call this yourself, call update() instead!
    void draw();

protected:

    // overload timerEvent to get periodic timer updates
    void timerEvent(QTimerEvent *event) override;

private:

    // parent widget
    QWidget* parent_;

    // the scene to be controlled
    std::shared_ptr<Scene> scene_;

    // periodically update the scene for animations
    // QTimer timer_;
    QBasicTimer timer_;

    // navigation
    std::unique_ptr<RotateY> cameraNavigator_;

};

