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

#include "exportfile.h"

#include <vector>

#include <QFile>
#include <QTextStream>
#include <QDebug>

#include <math.h>
//#include <QString>

exportfile::exportfile(pointCloud *cloudPtr)
{
    cloud = cloudPtr;

}

void exportfile::save(QString saveFileName, QString fileExtension)
{

    if (fileExtension == ".ply"){

        //qDebug() << saveFileName + fileExtension;
        createPlyFile(saveFileName + fileExtension);

    }

}

void exportfile::createPlyFile(QString saveFileName)
{

    QFile file(saveFileName);
    file.open(QIODevice::WriteOnly);
    QTextStream file3D(&file);


    //int num_outframes = numberFrames_ / horizAvg_;
    //int num_outpoints = numberPoints_ / vertAvg_;

    file3D << "ply" << endl;
    file3D << "format ascii 1.0" << endl;
    file3D << "element vertex " << cloud->cloudSize() << endl;
    file3D << "property float x" << endl;
    file3D << "property float y" << endl;
    file3D << "property float z" << endl;
    file3D << "property float nx" << endl;
    file3D << "property float ny" << endl;
    file3D << "property float nz" << endl;
   // file3D << "element face " << num_outframes*(num_outpoints-1)*2 << "\n";
   // file3D << "property list uchar int vertex_index\n";
    file3D << "end_header" << endl;

    for(int i=0; i < cloud->cloudSize(); i++){
        file3D << cloud->X(i) << " ";
        file3D << cloud->Y(i) << " ";
        file3D << cloud->Z(i) << endl;

    }
    for(int i=0; i < cloud->cloudSize(); i++){
        //Assume all normal point outwards
        float normalLength = sqrt((cloud->X(i) * cloud->X(i)) + (cloud->Y(i) * cloud->Y(i)) + (0.0 * 0.0));

        file3D << cloud->X(i) / (normalLength / 2) << " ";
        file3D << cloud->Y(i) / (normalLength / 2) << " ";
        file3D << 0.0 << endl;

    }


    file.close();
    emit saveComplete("Cloud saved to " + saveFileName, 5000);
}

