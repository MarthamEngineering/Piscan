/*
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <QtWidgets>
#include <QtOpenGL>

#include <math.h>

#include "glscanwidget.h"

GLScanWidget::GLScanWidget(QWidget *parent) :
    QGLWidget(parent)
{

    xRot = -75 * 16;
    yRot = 0;
    zRot = 0;
    numberPoints = 0;
    scaling = 1.0;
    zMin = 0.0;

   // createGradient();

}

GLScanWidget::~GLScanWidget()
{
}

QSize GLScanWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize GLScanWidget::sizeHint() const
{
    return QSize(400, 400);
}

static void qNormalizeAngle(int &angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360)
        angle -= 360 * 16;
}

void GLScanWidget::setXRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != xRot) {
        xRot = angle;
        emit xRotationChanged(angle);
        updateGL();
    }
}

void GLScanWidget::setYRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != yRot) {
        yRot = angle;
        emit yRotationChanged(angle);
        updateGL();
    }
}

void GLScanWidget::setZRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != zRot) {
        zRot = angle;
        emit zRotationChanged(angle);
        updateGL();
    }
}

void GLScanWidget::wheelEvent(QWheelEvent *event){

    double numDegrees = -event->delta()/8.0;
    double numSteps = numDegrees / 15.0;
    scaling *= pow(1.25, numSteps);
    if (scaling > 3){
        scaling = 3;
    }else if (scaling < 0.25){
        scaling = 0.25;
    }
    //qDebug() << "scaling: " << QString::number(scaling);
    updateGL();
}


void GLScanWidget::initializeGL()
{
    qglClearColor(Qt::white);

    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    glShadeModel(GL_FLAT);
    //glEnable(GL_LIGHTING);
    //glEnable(GL_LIGHT0);

    //static GLfloat lightPosition[4] = { 0, 0, 10, 1.0 };
    //glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
}

void GLScanWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -250.0);
    glRotatef(xRot / 16.0, 1.0, 0.0, 0.0);
    glRotatef(yRot / 16.0, 0.0, 1.0, 0.0);
    glRotatef(zRot / 16.0, 0.0, 0.0, 1.0);
    glScalef(scaling, scaling, scaling);

    //Draw Z Axis
    glLineWidth(1.5);
    qglColor(Qt::red);
    glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 200);
    glEnd();
    //renderText(0, 0, 260, "z");
    //Draw X Axis
    qglColor(Qt::green);
    glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(125, 0, 0);
    glEnd();
    //Draw X Axis
    qglColor(Qt::blue);
    glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 125, 0);
    glEnd();


    qglColor(Qt::lightGray);
    int sections = 36;
    GLfloat radius = 125;
    GLfloat twoPi = 2.0 * 3.14159;

    glBegin(GL_TRIANGLE_FAN);

    glVertex3f(0.0, 0.0, -1.5); //center of triangles

    for(int i = 0; i <= sections; i++) {

        glVertex3f(radius*cos(i*twoPi / sections),
                   radius*sin(i*twoPi / sections), -1.5);
    }

    glEnd();


    draw();
}

void GLScanWidget::resizeGL(int width, int height)
{
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
#ifdef QT_OPENGL_ES_1
    glOrthof(-2, +2, -2, +2, 1.0, 15.0);
#else
    glOrtho(-width/2, +width/2, -height/4, +height/4*3, -width, width);
#endif
    glMatrixMode(GL_MODELVIEW);
}

void GLScanWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void GLScanWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        //setXRotation(xRot + 8 * dy);
        setZRotation(zRot + 8 * dx);
    } else if (event->buttons() & Qt::RightButton) {
        //setXRotation(xRot + 8 * dy);
        //setYRotation(yRot + 8 * dx);
    }

    lastPos = event->pos();
}

void GLScanWidget::drawPoint(float x, float y, float z){

    z = 0.0 - z;

    //qDebug() << "x:" << x << " y:" << y << " z:" << z;


    points.push_back(point());

    points[numberPoints].x = x;
    points[numberPoints].y = y;
    points[numberPoints].z = z;

    if (z < zMin){
        zMin = z;
        //qDebug() << "zMin:" << zMin;
    }

    numberPoints++;

    //draw();


}

void GLScanWidget::drawVertex(QPoint point1, QPoint point2, QPoint point3){

    glBegin(GL_TRIANGLES);
        glNormal3f(1,0, 0.707);
        glVertex3f(1,-1,0);
        glVertex3f(1,1,0);
        glVertex3f(0,0,1.2);
    glEnd();


}

void GLScanWidget::draw()
{
    qglColor(Qt::black);


/*
    glBegin(GL_TRIANGLES);
    for(int i=0; i < numberPoints; i++){
        glVertex3f(points[i].x, points[i].y, points[i].z);
        glVertex3f(points[i+1].x, points[i+1].y, points[i+1].z);
        glVertex3f(points[i+2].x, points[i+2].y, points[i+2].z);
    }
    glEnd();

*/

    glBegin(GL_POINTS);
    for(int i=0; i < numberPoints; i++){
        glVertex3f(points[i].x, points[i].y, abs(zMin) + points[i].z);
    }
    glEnd();

}
