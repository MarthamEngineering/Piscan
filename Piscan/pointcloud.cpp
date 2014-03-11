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

#include <QDebug>

pointCloud::pointCloud(QObject *parent) :
    QObject(parent)
{

    zMin = 0.0;
}


void pointCloud::addPoint(float x, float y, float z){

   // Invert Z
   // z = 0.0 - z;

    if ((abs(x) < 125) & (abs(y) < 125) & (z > 0) & (z < 125))
    {

    //qDebug() << "Point Added: " << "x:" << x << " y:" << y << " z:" << z;
    //qDebug() << "Points size: " << points.size();
    unsigned int index = points.size();
    points.push_back(point());
    points[index].x = x;
    points[index].y = y;
    points[index].z = z;

    //numberPoints++;
    }else{

    //qDebug() << "Errors size: " << errors.size();
    unsigned int errorIndex = errors.size();
    errors.push_back(point());
    errors[errorIndex].x = x;
    errors[errorIndex].y = y;
    errors[errorIndex].z = z;

    }

    if (z < zMin){
        zMin = z;
       // qDebug() << "zMin:" << zMin;

    }
}


int pointCloud::cloudSize()
{
    return points.size();

}

int pointCloud::errorSize()
{
    return errors.size();

}

float pointCloud::getZmin()
{
    return zMin;
}

float pointCloud::X(int i)
{
return points[i].x;
}

float pointCloud::Y(int i)
{
return points[i].y;
}

float pointCloud::Z(int i)
{
return points[i].z;
}

float pointCloud::errorX(int i)
{
return errors[i].x;
}

float pointCloud::errorY(int i)
{
return errors[i].y;
}

float pointCloud::errorZ(int i)
{
return errors[i].z;
}

void pointCloud::refineCloud()
{
    // Find Average of Good Points

    float xAverage, yAverage, zAverage;

    for (int i = 0; i < cloudSize(); i++)
    {

        xAverage += points[i].x;
        yAverage += points[i].y;
        zAverage += points[i].z;
    }

    xAverage = xAverage / cloudSize();
    yAverage = yAverage / cloudSize();
    zAverage = zAverage / cloudSize();


    qDebug() << "xAverage " << xAverage;
    qDebug() << "yAverage " << yAverage;
    qDebug() << "zAverage " << zAverage;


}

