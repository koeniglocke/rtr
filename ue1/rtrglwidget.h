/*
 *  OpenGL Widget: handles GL init, paint, and resize events.
 *
 *  Initializes and holds the scene, catches mouse events and forwards them to the scene.
 *
 *  see also: http://doc.qt.io/qt-5/qopenglwidget.html
 */

#pragma once

#include <memory> // std::unique_ptr, std::shared_ptr

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLDebugLogger>

// forward declaration
class SceneController;
class QOpenGLShaderProgram;
class QOpenGLTexture;

/**
 * @brief The rtrGLWidget class represents the actual OpenGL widget in the
 * Qt application that will be used to draw the 3D scene.
 *
 * The constructor will instantiate a Scene and its SceneController.
 * During UI operation, the widget forwards paint, resize, and mouse events
 * to the SceneController.
 *
 */

class rtrGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:

    /**
     * @brief rtrGLWidget contructor will initialize OpenGL and instantiate a Scene object
     *        along with its SceneController object.
     * @param parent the parent widget
     */
    explicit rtrGLWidget(QWidget *parent = 0);
    ~rtrGLWidget();

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

    // reference to the actual scene (via its controller) to be drawn
    SceneController& sceneController();

signals:
    void clicked();

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:

    // scene (via its controller) to be drawn and/or manipulated
    std::shared_ptr<SceneController> scene_controller_;

    QOpenGLDebugLogger *logger;
};

