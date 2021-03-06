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

#ifndef EXPORTFILE_H
#define EXPORTFILE_H

#include "pointcloud.h"

#include <sstream>
#include <stdexcept>
#include <vector>

#include <QString>
#include <QObject>

class exportfile : public QObject
{
        Q_OBJECT
public:
    exportfile(pointCloud *cloudPtr);
    void save(QString saveFileName, QString fileExtension);

signals:
    void saveComplete(QString text, int timeOut);

private:

void createPlyFile(QString saveFileName);
pointCloud* cloud;
};

#endif // EXPORTFILE_H
