#ifndef GLSCANWIDGET_H
#define GLSCANWIDGET_H

#include <QGLWidget>

class GLScanWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit GLScanWidget(QWidget *parent = 0);
    ~GLScanWidget();
    
signals:
    void xRotationChanged(int angle);
    void yRotationChanged(int angle);
    void zRotationChanged(int angle);
    
public slots:
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);

    void drawPoint(float x, float y, float z);
    void drawVertex(QPoint point1, QPoint point2, QPoint point3);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    //void paintEvent(QPaintEvent *event);

private:
    void draw();
    int xRot;
    int yRot;
    int zRot;
    float zMin;
    GLfloat scaling;
    QPoint lastPos;

    //void createGradient();
    //QRadialGradient gradient;
    //void drawBackground(QPainter *painter);


    struct point {

        float x;
        float y;
        float z;

    };

    //point points[153600];

    std::vector<point> points;

    int numberPoints;
    
};

#endif // GLSCANWIDGET_H
