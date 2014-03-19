#include <iostream>
#include <OpenGL/gl.h>
#include <algorithm>
#include <cmath>

#include <geomc/function/Path.h>


#include "GLWindow.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "AnimTimer.h"

#include "Manipulator.h"

#include "visible/VisCallback.h"

#define RESOLUTION 1920

using namespace geom;
using namespace std;

struct Vtx {
    Vec3f p;
    Vec3f c;
};


// todo: better warp for jellyfish (ptshader?)
// todo: additive blending
// todo: better animated camera (use a steering algorithm)
// todo: group movement: incompressible fluid? vertical drift?
// todo: frustum culling?
// todo: LOD. choose a hi or lo-res jellyfish.
// todo: antialiasing / point smoothing?
// todo: hidpi

class State : virtual public Animated, virtual public Drawable {
public:
    State(Camera *cam) : cam(cam) {
        r = 500;
        camspeed = 50.0;
    }
    
    Camera *cam;
    Vec3f camv;
    Vec3f cama;
    double t0;
    Vec3f *jfpt;
    float *jfsz;
    int njfs;
    double r;
    double time;
    double camspeed;
    VertexBuffer<Vtx> vbuf;
    
    void init(double t) {
        t0 = t;
        
        cam->setFar(r * 50);
        cam->setNear(r * 0.005);
        
        Sampler<double> smp;
        
        cam->setPosition(smp.solidball<3>(r));
        camv = smp.unit<3>(camspeed);
        cama = smp.oriented_disk(camv, camspeed * 0.1);
        
        int npts = 10000;
        boost::shared_array<Vtx> vtxs(new Vtx[npts]);
        boost::shared_array<GLuint> idxs(new GLuint[npts]);
        for (int i = 0; i < npts; i++) {
            Vec3f p = smp.cap(M_PI / 4.0);
            vtxs[i].p = p;
            vtxs[i].c = p;
            idxs[i] = i;
        }
        
        njfs = 1000;
        jfpt = new Vec3f[njfs];
        jfsz = new float[njfs];
        for (int i = 0; i < njfs; i++) {
            jfpt[i] = smp.solidball<3>() * r * 3.5;
            jfsz[i] = getRandom()->rand<float>(r * 0.1, r);
        }
        
        vbuf.setVertexData(vtxs, npts);
        vbuf.setIndexData(idxs, npts);
        vbuf.defineAttribute("vertex", offsetof(Vtx, p), GL_FLOAT, 3);
        vbuf.defineAttribute("color",  offsetof(Vtx, c), GL_FLOAT, 3);
    }
    
    void update(double t, double dt) {
        Sampler<double> smp;
        double t_rel = t - t0;
        time = t_rel;
        Vec3d pos = cam->getPosition();
        cam->setPosition(pos + camv * dt);
        camv += cama * dt;
        camv = camv.unit() * camspeed;
        cama += smp.unit<3>() * camspeed * dt * 0.5;
        cam->setCenterOfInterest(cam->getPosition() + camv);
        //cam->up = camv ^ cama;
    }
    
    void draw() {
        glBegin(GL_POINTS);
        double r_figure = r * 0.25;
        for (int i = 0; i < 1000; i++) {
            double f = i / 1000.;
            double theta = M_PI * 2 * f;
            Vec3d color = rainbow<double>(f);
            glColor(color);
            glVertex3d(r_figure * sin(theta), r_figure * cos(theta), 0.5 * r_figure * sin(4*theta));
        }
        /*
        glColor3d(1,1,1);
        for (int i = 0; i < 2000; i++) {
            double f = i / 2000.0;
            double t = f * (campath.knots.size() - 1);
            Vec3d p = campath.eval(t);
            glVertex(p);
        }*/
        glEnd();
        
        vbuf.use();
        for (int i = 0; i < njfs; i++) {
            float s = jfsz[i] * (0.05*sin(2 * time + jfsz[i]/100.0) + 1);
            glPushMatrix();
            glTranslate(jfpt[i]);
            glScaled(s,s,s);
            vbuf.drawElements(GL_POINTS, vbuf.getNumPoints());
            glPopMatrix();
        }
        
        vbuf.disable();
        
    }
    
};
int main(int argc, char *argv[]) {
    GLWindow win(&argc, argv, "noodle", RESOLUTION, RESOLUTION);
    AnimTimer timer(&win);
    //Manipulator manip(&win, &win.cam);
    
    win.cam.setPosition(X_AXIS3d * 1000);
    win.cam.setCenterOfInterest(ZERO_VEC3d);
    win.cam.setNear(1);
    win.cam.setFar(1000);
    win.cam.setUp(Z_AXIS3d);
    win.cam.setFov(90);
    
    State s(&win.cam);
    
    win.scene.push_back(&s);
    timer.anims.push_back(&s);
    
    timer.begin();
    win.showAll();
    return 0;
    
}