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

#ifndef POINTCLOUD_H
#define POINTCLOUD_H

#include <QObject>

#include <vector>


class pointCloud : public QObject
{
    Q_OBJECT
public:
    explicit pointCloud(QObject *parent = 0);
    
signals:
    
public slots:

    void addPoint(float x, float y, float z);
    int cloudSize();
    int errorSize();
    float getZmin();
    void refineCloud();

    float X(int i);
    float Y(int i);
    float Z(int i);

    float errorX(int i);
    float errorY(int i);
    float errorZ(int i);

private:

    struct point {

        float x;
        float y;
        float z;

    };


    float zMin;
    std::vector<point> points;
    std::vector<point> errors;
    
};

#endif // POINTCLOUD_H
