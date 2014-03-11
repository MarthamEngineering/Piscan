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


#include "pointcloud.h"

#include <QGLWidget>
#include <QTimer>


class GLScanWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLScanWidget(QWidget *parent = 0);
    ~GLScanWidget();

    QSize sizeHint() const;
    int xRotation() const { return xRot; }
    int yRotation() const { return yRot; }
    int zRotation() const { return zRot; }

public slots:
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);

    void setCloud(pointCloud *cloudptr);

protected:
    void initializeGL();
    void paintEvent(QPaintEvent *event);
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void showEvent(QShowEvent *event);

private slots:


private:

    void drawGL();
    void drawInstructions(QPainter *painter);
    void setupViewport(int width, int height);
    //void paintGL();
    void createGradient();
    void drawBackground(QPainter *painter);
    void drawLegend(QPainter *painter);

    pointCloud* cloud;

    QColor qtGreen;
    QColor qtPurple;
    QRadialGradient gradient;

    //GLuint object;
    int xRot;
    int yRot;
    int zRot;
    GLfloat scaling;
    QPoint lastPos;

};
