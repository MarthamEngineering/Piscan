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

 Changelog:

 2007 The original source was created by Steve Baker:
    http://www.sjbaker.org/wiki/index.php?title=A_Simple_3D_Scanner
 2009-02-20 <a.w@quirxi.net>:
    Rewrite of code so that it can be used as a command line tool.
 2014-01-05 <dwood@marthamengineering.co.uk>
    -Relicensed from LGPL to GPL
    -Added PLY file output
    -Converted ScanDraid to a Qt class to use signal and slots
*/

#include <QDebug>
#include <QImage>
#include <QDir>
#include <QFileInfo>

#include <libintl.h>
//#include <fstream>
//#include <iostream>
//#include <iomanip>
#include <math.h>
#include "scandraid.h"

//#include "mainwindow.h"

namespace scanDraiD {

const float ScanDraiD::DEGREES_TO_RADIANS(3.14159/180.0);

ScanDraiD::ScanDraiD()
{

}

ScanDraiD::~ScanDraiD()
{

}

void ScanDraiD::start(QString inDir)//, std::stringstream& scanResult, std::string& fileType)
    throw (std::runtime_error)
{
    // @todo replace member numberFrames_ with variable !?
    //getNumberOfFrames(inDir);
    QDir dir(inDir);
    QStringList filters;
    filters << "*.png" << "*.jpg" << "*.jpeg" << "*.bmp" << "*.gif";
    QFileInfoList imageFileList = dir.entryInfoList(filters, QDir::Files|QDir::NoDotAndDotDot);
    numberFrames_ = imageFileList.size();


    if (0==numberFrames_)
        throw std::runtime_error(gettext("Error: cannot find jpg frames in directory '") );

    std::vector<frame> vecFrames;
    vecFrames.reserve(numberFrames_);

    for (unsigned int curFrameNr = 0; curFrameNr<numberFrames_; ++curFrameNr)
    {
        QFileInfo imageFile = imageFileList.at(curFrameNr);
        QString framePath = inDir + "/" + imageFile.fileName();

        //create a QVarant to return the percentage complete to the UI
        float percentage =  nearbyint(100 * (((float)curFrameNr+1) / (float)numberFrames_)) ;
        //emit the percentage completeback to the UI
        emit percentageComplete("Processing Frames: " + QString::number(percentage) + "%", 5000);

        processSingleFrame(framePath.toStdString(), curFrameNr, vecFrames);
        //framePath.str("");
    }
}

void ScanDraiD::processSingleFrame(const std::string& fileName, const unsigned int frameNr, std::vector<frame>& vecFrames) //const
    throw()
{
    QImage image;
    image.load(fileName.c_str());
    const int width(image.width());

    /** @todo is numberPoints_ necessary or can we use a more flexible aproach ? e.g just using the number of found bright pixels ? */
    numberPoints_ = image.height() / lineSkip_;
    float x(0.0), y(0.0), z(0.0), brightness(0.0), max(0.0), radius(0.0), cameraAngle(0.0);
    unsigned int px(0);
    int maxpos(-1);
    //float threshold = 1.0;

    for (unsigned int j = 0; j < numberPoints_; ++j)
    {
        // Find the brightest pixel
        /** @todo do we have to specify minimal pixel brightness in order not to use dark points for object values ??!!! */
        max  = 0.0;
        maxpos = -1;
        for (int i = 0; i < width; ++i)
        {
            px = image.pixel(i, j*lineSkip_);
            //Bitshift the RGB value and extract just the RED colour brightness
            //brightness = (static_cast<float>((px >> 16) & 0xFF));
                         
            
            brightness = (static_cast<float>((px >> 24) & 0xFF)) / 255.0 +
                         (static_cast<float>((px >> 16) & 0xFF)) / 255.0 +
                         (static_cast<float>((px >>  8) & 0xFF)) / 255.0;             
                       
            /**
            * @todo use swap here ?
            */


            if (brightness > max)
            {
                max = brightness;
                maxpos = i;
            }
        }
        
        //Quick and Dirty method to ignore dark pixels
        if (maxpos == -1){
            qDebug() << "No laser detected";
            }else{

            float frameAngle = static_cast<float>(frameNr) * (360.0/static_cast<float>(numberFrames_));
            vecFrames.push_back(frame());
            frame& ref2Frame = vecFrames.back();
            ref2Frame.points_.reserve(numberPoints_);
            cameraAngle = cameraHFov_ * (0.5 - static_cast<float>(maxpos) / static_cast<float>(width));
            float pointAngle = 180.0 - cameraAngle + laserOffset_;
            radius = cameraDistance_ * sin(cameraAngle * DEGREES_TO_RADIANS) / sin(pointAngle * DEGREES_TO_RADIANS);

            x = radius * sin(frameAngle * DEGREES_TO_RADIANS);
            y = radius * cos(frameAngle * DEGREES_TO_RADIANS);
            z = atan((cameraVFov_ * DEGREES_TO_RADIANS / 2.0)) * 2.0 * cameraDistance_ * static_cast<float>(j) / static_cast<float>(numberPoints_);

            ref2Frame.points_.push_back(point(x,y,z));
            emit addPointScanView(x, y, z);
            //qDebug() << "x: " << x << " y: " << y << " x: " << z ;

        }
    }
}


// setters
void ScanDraiD::setCameraHFov(const float cameraHFov, bool adjustCameraVFov)
    throw (std::runtime_error)
{
    if (cameraHFov > 360 || cameraHFov < 0)
        throw std::runtime_error("Error: invalid angle for CAMERA_HFOV !");

    cameraHFov_=cameraHFov;

    if (adjustCameraVFov)
        setCameraVFov(cameraHFov_*4.0/5.0);
}
void ScanDraiD::setCameraVFov(const float cameraVFov)
    throw (std::runtime_error)
{
    if (cameraVFov > 360 || cameraVFov < 0)
        throw std::runtime_error("Error: invalid angle for CAMERA_VFOV !");
    cameraVFov_=cameraVFov;
}
void ScanDraiD::setCameraDistance(const float cameraDistance)
    throw (std::runtime_error)
{
    if (cameraDistance <= 0)
        throw std::runtime_error("Error: Distance from camera to center of turntable (=CAMERA_DISTANCE) cannot be <= 0 !");
    cameraDistance_=cameraDistance;
}
void ScanDraiD::setLaserOffset(const float laserOffset)
    throw (std::runtime_error)
{
    if (laserOffset > 360 || laserOffset < 0)
        throw std::runtime_error(gettext("Error: invalid angle for LASER_OFFSET !"));
    laserOffset_=laserOffset;
}
void ScanDraiD::setHorizontalAverage(const unsigned int horizAvg)
    throw (std::runtime_error)
{
    if (horizAvg==0)
        throw std::runtime_error(gettext("Error: value vor horizontal average (=HORIZ_AVG) cannot be 0 !"));
    horizAvg_=horizAvg;
}
void ScanDraiD::setVerticalAverage(const unsigned int vertAvg)
    throw (std::runtime_error)
{
    if (vertAvg==0)
        throw std::runtime_error(gettext("Error: value vor verical average (=VERT_AVG) cannot be 0 !"));
    vertAvg_=vertAvg;
}
void ScanDraiD::setFrameSkip(const unsigned int frameSkip)
    throw (std::runtime_error)
{
    if (frameSkip==0)
        throw std::runtime_error(gettext("Error: value for frames to skip (=FRAME_SKIP) cannot be 0 !"));
    frameSkip_=frameSkip;
}
void ScanDraiD::setLineSkip(const unsigned int lineSkip)
    throw (std::runtime_error)
{
    if (lineSkip==0)
        throw std::runtime_error(gettext("Error: value for lines to skip (=LINE_SKIP) cannot be 0 !"));
    lineSkip_=lineSkip;
}

} // namespace scanDraiD

