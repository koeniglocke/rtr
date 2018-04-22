#pragma once

#include <QWidget>
#include <QSettings>

#include "scenecontroller.h"


namespace Ui {
class AppWindow;
}

/*
 *   RTR demo App, AppWindow
 *   Author: Hartmut Schirmacher
 *
 *   The AppWindow wraps the GUI of the application that has been created
 *   and can be edited in Qt UI designer.
 *
 *   It consists of an OpenGL Widget for rendering the OpenGL Scene,
 *   plus additional UI elements like buttons.
 *
 *   UI events as well as keyboard events are forwarded to the
 *   SceneController object.
 *
 */

class AppWindow : public QWidget
{
    Q_OBJECT

public:
    explicit AppWindow(QWidget *parent = 0);
    ~AppWindow();

    // convenience shortcut to the OpenGL scene
    SceneController& sceneController();

public slots:

    /* show buttons etc, and a border around the OpenGL widget */
    void showUI();

    /* hide all buttons etc, and remove border around the OpenGL widget */
    void hideUI();

    /* process app-wide key events and trigger respective actions in UI or in the scene */
    void keyPressEvent(QKeyEvent *event) override;

    /* set default values for the UI widgets.
     * must only be called AFTER constructor, after scene has been instantiated. */
    void setDefaultUIValues();

protected:

    /* when closing, the window stores its geometry in the settings */
    void closeEvent(QCloseEvent *event) override;

    /* when showing, initialize values of UI widgets */
    void showEvent(QShowEvent *event) override;

private slots:

private:
    // this is the connection to the class that will come out of the UI designer
    Ui::AppWindow *ui;

    // remember if the window has been shown / initialized already
    bool wasInitialized = false;

    // this is where the app remembers its settings, i.e. window size
    QSettings settings_;
};

