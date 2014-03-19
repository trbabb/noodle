#include <iostream>
#include <cmath>
#include <geomc/linalg/Vec.h>

#include "GLWindow.h"
#include "GUIListener.h"
#include "Drawable.h"
#include "AnimTimer.h"

#define RESOLUTION 1920

using namespace std;
using namespace geom;

class CircleVis : public Drawable, public GUIListener {
public:
    
    CircleVis():
            m_fieldWidth(1000),
            m_circleRad(m_fieldWidth * 0.18),
            m_mouseAngle(0),
            m_mouseIn(false) {
        // pass
    }
    
    void draw() {
        glColor3d(1,1,1);
        drawCircle(100, m_circleRad);
        
        if (m_mouseIn) {
            glColor(Vec3d(0.5));
            glBegin(GL_LINES);
            glVertex(ZERO_VEC2d);
            glVertex(Vec2d(m_mouseAngle, m_circleRad).fromPolar());
            glEnd();
        }
    }
    
    void drawCircle(int segs, double rad, int mode=GL_LINE_LOOP) {
        glBegin(mode);
        for (int i = 0; i < segs; i++) {
            double f = i / (double)segs;
            double a = f * TAU;
            Vec2d vtx = Vec2d(sin(a), cos(a));
            glVertex(vtx * rad);
        }
        glEnd();
    }
    
    bool mouseMotion(GLWindow *win, int x, int y, int buttons) {
        Vec2d pos = Vec2d(x - win->w / 2, -(y - win->h / 2)) / win->w;
        pos *= m_fieldWidth;
        m_mouseAngle = pos.getAngle();
        m_mouseIn = pos.mag() <= m_circleRad;
        m_pt = pos;
        return true;
    }
    
    double m_fieldWidth;
    double m_circleRad;
    double m_mouseAngle;
    bool   m_mouseIn;
    Vec2d m_pt;
};

int main(int argc, char** argv) {
    GLWindow win(&argc, argv, "circle", RESOLUTION, RESOLUTION / 2.39);
    AnimTimer timer(&win);
    
    CircleVis c;
    
    win.cam.setProjectionMode(CAM_ORTHOGRAPHIC);
    win.cam.setPosition(Vec3d(0, 0, 1000));
    win.cam.setNear(0.0001);
    win.cam.setFar(2000);
    win.cam.setOrthoWidth(c.m_fieldWidth);
    win.cam.setUp(Vec3d(0, 1, 0));
    win.cam.setDirection(Vec3d(0, 0, -1));
    
    win.scene.push_back(&c);
    win.guiListeners.push_back(&c);
    
    win.setClearColor(Vec4d(0.125, 0.125, 0.125, 1));
    
    timer.begin();
    win.showAll();
    
    return 0;
}