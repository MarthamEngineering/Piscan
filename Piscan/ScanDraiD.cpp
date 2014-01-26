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
//#include <QVariant>

#include <libintl.h>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <math.h>
#include "Jpeg.hpp"
#include "ScanDraiD.hpp"

//#include "mainwindow.h"

namespace scanDraiD {


////////////////////////////////////////////////////////////////////////////////
// static
////////////////////////////////////////////////////////////////////////////////
const float ScanDraiD::DEGREES_TO_RADIANS(3.14159/180.0);

/*
std::string ScanDraiD::describeConfiguration()
{
    return std::string(gettext("The program expects the input frames in jpg format.\n"
"The single pictures must be numbered from 00000000.jpg upwards.\n"
"All angles are in degrees, all distances are in meters and parameters are case sensitive.\n"
"\n"
"CAMERA_DISTANCE: The distance from camera to center of turntable in mm (required/cannot be <=0).\n"
"LASER_OFFSET:    The angle between the centerline of the camera and the laser - should be about 15 degrees (required).\n"
"CAMERA_HFOV:     This is the camera lens angle (default: 50.0 degrees).\n"
"CAMERA_VFOV:     Leave this alone, unless you are working with an unusual kind of camera such as an HDTV camera\n"
"                 which doesn't have the usual aspect ratios (default: CAMERA_HFOV*4.0/5.0 degrees).\n"
"\n"
"    Additionally, you might want to change:\n"
"HORIZ_AVG:       The number of consecutive horizontal points to average together to smooth out the surface (default: 10/cannot be 0).\n"
"VERT_AVG:        The number of consecutive vertical points to average together to smooth out the surface (default: 10/cannot be 0).\n"
"    Increasing these numbers gets rid of odd spikes and other small errors - but reduces the number\n"
"    of polygons making up the final model and thereby reduces the precision of the final model.\n"
"\n"
"FRAME_SKIP:      Use only every n'th frame for speed (default: 1/cannot be 0).\n"
"LINE_SKIP:       Use only every n'th scanline for speed (default: 1/cannot be 0).")
);
}
*/

////////////////////////////////////////////////////////////////////////////////
ScanDraiD::ScanDraiD()
   /* : cameraHFov_(50.0), cameraVFov_(cameraHFov_*4.0/5.0),
      horizAvg_(10), vertAvg_(10), frameSkip_(1), lineSkip_(1),
      cameraDistance_(0), laserOffset_(0),
      numberFrames_(0), numberPoints_(0)
*/
{;}
////////////////////////////////////////////////////////////////////////////////
ScanDraiD::~ScanDraiD()
{;}
////////////////////////////////////////////////////////////////////////////////
void ScanDraiD::convertAngleSideAngle2SideAngleSide(float angA, const float lenB, float angC,
     float* const lenA, float* const angB, float* const lenC) const
     throw()
{
    /* Find the missing angle */
    float bb = 180.0 - (angA + angC);
    if (angB) *angB = bb;

    /* Convert everything to radians */
    angA *= DEGREES_TO_RADIANS;
    angC *= DEGREES_TO_RADIANS;
    bb   *= DEGREES_TO_RADIANS;

    /* Use Sine Rule */
    float sinB = sin(bb);
    if (sinB == 0.0)
    {
        if (lenA)
            *lenA = lenB / 2.0;  /* One valid interpretation */
        if (lenC)
            *lenC = lenB / 2.0;
    }
    else
    {
        if (lenA) *lenA = lenB * sin(angA) / sinB;
        if (lenC) *lenC = lenB * sin(angC) / sinB;
    }
}
////////////////////////////////////////////////////////////////////////////////
void ScanDraiD::getNumberOfFrames(const std::string& path2Frames)
    throw()
{
    std::stringstream framePath(path2Frames+gettext("/00000000.jpg"));
    std::ifstream config(framePath.str().c_str());
    for (int frameNr=1; config.good(); ++frameNr)
    {
        numberFrames_=frameNr;
        config.close();
        framePath.str("");
        framePath << path2Frames << "/" << std::setw(8) << std::setfill('0') << frameNr << gettext(".jpg") << std::flush;
        config.open(framePath.str().c_str());
    }
}
////////////////////////////////////////////////////////////////////////////////
void ScanDraiD::processSingleFrame(const std::string& fileName, const unsigned int frameNr, std::vector<frame>& vecFrames) //const
    throw()
{
    scanDraiD::Jpeg jpg;
    jpg.load(fileName.c_str());
    const int width(jpg.getWidth());
    /** @todo is numberPoints_ necessary or can we use a more flexible aproach ? e.g just using the number of found bright pixels ? */
    numberPoints_ = jpg.getHeight() / lineSkip_;
    vecFrames.push_back(frame());
    frame& ref2Frame = vecFrames.back();
    ref2Frame.points_.reserve(numberPoints_);
    float frameAngle = static_cast<float>(frameNr) * (360.0/static_cast<float>(numberFrames_));
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
            px = jpg.getPixel(i, j*lineSkip_);
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
                ref2Frame.points_.push_back(point(0,0,0));
            }else{
            cameraAngle = cameraHFov_ * (0.5 - static_cast<float>(maxpos) / static_cast<float>(width));
            convertAngleSideAngle2SideAngleSide(cameraAngle, cameraDistance_, laserOffset_, &radius, 0, 0);

            x = radius * sin(frameAngle * DEGREES_TO_RADIANS);
            y = radius * cos(frameAngle * DEGREES_TO_RADIANS);
            z = atan((cameraVFov_ * DEGREES_TO_RADIANS / 2.0)) * 2.0 * cameraDistance_ * static_cast<float>(j) / static_cast<float>(numberPoints_);
            ref2Frame.points_.push_back(point(x,y,z));

            emit addPointScanView(x, y, z);
            //qDebug() << "x: " << x << " y: " << y << " x: " << z ;

        }
    }
}
////////////////////////////////////////////////////////////////////////////////
void ScanDraiD::processFrames(const std::string& path2Frames, std::stringstream& scanResult, std::string& fileType)
    throw (std::runtime_error)
{
    // @todo replace member numberFrames_ with variable !?
    getNumberOfFrames(path2Frames);
    if (0==numberFrames_)
        throw std::runtime_error(gettext("Error: cannot find jpg frames in directory '") + path2Frames + gettext("'!"));

    std::vector<frame> vecFrames;
    vecFrames.reserve(numberFrames_);

    std::stringstream framePath;
    for (unsigned int curFrameNr = 0; curFrameNr<numberFrames_; ++curFrameNr)
    {
        framePath << path2Frames << "/" << std::setw(8) << std::setfill('0') << curFrameNr << gettext(".jpg") << std::flush;
        //std::cerr << gettext("Processing frame ") << curFrameNr+1 << "/" << numberFrames_ << gettext(": '") << framePath.str() << "'\r";

        //create a QVarant to return the percentage complete to the UI
        float percentage =  nearbyint(100 * (((float)curFrameNr+1) / (float)numberFrames_)) ;
        //emit the percentage completeback to the UI
        emit percentageComplete("Processing Frames: " + QString::number(percentage) + "%", 5000);

        processSingleFrame(framePath.str(), curFrameNr, vecFrames);
        framePath.str("");
    }
    //Check the desired output format
    if (fileType == "ac" || fileType == "acc"){
    createAC3DFile(vecFrames, scanResult);
    } 
    else if (fileType == "ply" || fileType == "PLY"){
    createPlyFile(vecFrames, scanResult);
    }
}
////////////////////////////////////////////////////////////////////////////////
void ScanDraiD::createPlyFile(const std::vector<frame>& vecFrames, std::stringstream& file3D) const
    throw()
{
    int num_outframes = numberFrames_ / horizAvg_;
    int num_outpoints = numberPoints_ / vertAvg_;

    file3D << "ply\n";
    file3D << "format ascii 1.0\n";
    file3D << "element vertex " << num_outpoints*num_outframes << "\n";
    file3D << "property float x\n";
    file3D << "property float y\n";
    file3D << "property float z\n";
   // file3D << "element face " << num_outframes*(num_outpoints-1)*2 << "\n";
   // file3D << "property list uchar int vertex_index\n";
    file3D << "end_header\n";

    for(int f = 0; f < num_outframes; ++f)
    {
        for(int i = 0; i < num_outpoints; ++i)
        {
            point myPoint(0.0,0.0,0.0);
            for(unsigned int ff = 0; ff < horizAvg_; ++ff)
            {
                for(unsigned int ii = 0; ii < vertAvg_; ++ii)
                {
                    myPoint.x_ += vecFrames[f*horizAvg_+ff].points_[i*vertAvg_+ii].x_;
                    myPoint.y_ += vecFrames[f*horizAvg_+ff].points_[i*vertAvg_+ii].y_;
                    myPoint.z_ += vecFrames[f*horizAvg_+ff].points_[i*vertAvg_+ii].z_;
                }
            }
            myPoint.x_ /= static_cast<float>(horizAvg_*vertAvg_);
            myPoint.y_ /= static_cast<float>(horizAvg_*vertAvg_);
            myPoint.z_ /= static_cast<float>(horizAvg_*vertAvg_);
            file3D << myPoint.x_ << " " << myPoint.y_ << " " << myPoint.z_ << "\n";
        }
    }

/*
    for(int f = 0; f < num_outframes; ++f)
    {
        int ff = (f==num_outframes-1) ? 0 : f+1;
        for(int i = 0; i < num_outpoints-1; ++i)
        {
            int ii = i+1;
            file3D << "3 " << ff*num_outpoints+ii << " " << static_cast<float>(ff)/static_cast<float>(num_outframes) << " " << static_cast<float>(ii)/static_cast<float>(num_outpoints) << "\n";
            file3D << "3 " << ff*num_outpoints+i << " " << static_cast<float>(ff)/static_cast<float>(num_outframes) << " " << static_cast<float>(i)/static_cast<float>(num_outpoints) << "\n";
            file3D << "3 " << f*num_outpoints+i << " " << static_cast<float>(f)/static_cast<float>(num_outframes) << " " << static_cast<float>(i)/static_cast<float>(num_outpoints) << "\n";
            file3D << "3 " << f*num_outpoints+ii << " " << static_cast<float>(f)/static_cast<float>(num_outframes) << " " << static_cast<float>(i)/static_cast<float>(num_outpoints) << "\n";
            file3D << "3 " << ff*num_outpoints+ii << " " << static_cast<float>(ff)/static_cast<float>(num_outframes) << " " << static_cast<float>(ii)/static_cast<float>(num_outpoints) << "\n";
            file3D << "3 " << f*num_outpoints+i  << " " << static_cast<float>(f)/static_cast<float>(num_outframes) << " " << static_cast<float>(i)/static_cast<float>(num_outpoints) << "\n";
        }
    }
*/
}
////////////////////////////////////////////////////////////////////////////////
void ScanDraiD::createAC3DFile(const std::vector<frame>& vecFrames, std::stringstream& file3D) const
    throw()
{
    int num_outframes = numberFrames_ / horizAvg_;
    int num_outpoints = numberPoints_ / vertAvg_;

    file3D << "AC3Db\n";
    file3D << "MATERIAL \"ac3dmat1\" rgb 1 1 1  amb 0.2 0.2 0.2  emis 0 0 0  spec 0.5 0.5 0.5  shi 10  trans 0\n";
    file3D << "OBJECT world\n";
    file3D << "kids 1\n";
    file3D << "OBJECT poly\n";
    file3D << "name \"scanDraiD\"\n";
    file3D << "loc 0.0 0.0 0.0\n";
    file3D << "numvert " << num_outpoints*num_outframes << "\n";

    for(int f = 0; f < num_outframes; ++f)
    {
        for(int i = 0; i < num_outpoints; ++i)
        {
            point myPoint(0.0,0.0,0.0);
            for(unsigned int ff = 0; ff < horizAvg_; ++ff)
            {
                for(unsigned int ii = 0; ii < vertAvg_; ++ii)
                {
                    myPoint.x_ += vecFrames[f*horizAvg_+ff].points_[i*vertAvg_+ii].x_;
                    myPoint.y_ += vecFrames[f*horizAvg_+ff].points_[i*vertAvg_+ii].y_;
                    myPoint.z_ += vecFrames[f*horizAvg_+ff].points_[i*vertAvg_+ii].z_;
                }
            }
            myPoint.x_ /= static_cast<float>(horizAvg_*vertAvg_);
            myPoint.y_ /= static_cast<float>(horizAvg_*vertAvg_);
            myPoint.z_ /= static_cast<float>(horizAvg_*vertAvg_);
            file3D << myPoint.x_ << " " << myPoint.y_ << " " << myPoint.z_ << "\n";
        }
    }
    file3D << "numsurf " << num_outframes*(num_outpoints-1)*2 << "\n";

    for(int f = 0; f < num_outframes; ++f)
    {
        int ff = (f==num_outframes-1) ? 0 : f+1;
        for(int i = 0; i < num_outpoints-1; ++i)
        {
            int ii = i+1;

            file3D << "SURF 0x10\n";
            file3D << "mat 0\n";
            file3D << "refs 3\n";
            file3D << ff*num_outpoints+ii << " " << static_cast<float>(ff)/static_cast<float>(num_outframes) << " " << static_cast<float>(ii)/static_cast<float>(num_outpoints) << "\n";
            file3D << ff*num_outpoints+i << " " << static_cast<float>(ff)/static_cast<float>(num_outframes) << " " << static_cast<float>(i)/static_cast<float>(num_outpoints) << "\n";
            file3D << f*num_outpoints+i << " " << static_cast<float>(f)/static_cast<float>(num_outframes) << " " << static_cast<float>(i)/static_cast<float>(num_outpoints) << "\n";
            file3D << "SURF 0x10\n";
            file3D << "mat 0\n";
            file3D << "refs 3\n";
            file3D << f*num_outpoints+ii << " " << static_cast<float>(f)/static_cast<float>(num_outframes) << " " << static_cast<float>(i)/static_cast<float>(num_outpoints) << "\n";
            file3D << ff*num_outpoints+ii << " " << static_cast<float>(ff)/static_cast<float>(num_outframes) << " " << static_cast<float>(ii)/static_cast<float>(num_outpoints) << "\n";
            file3D << f*num_outpoints+i  << " " << static_cast<float>(f)/static_cast<float>(num_outframes) << " " << static_cast<float>(i)/static_cast<float>(num_outpoints) << "\n";
        }
    }
    file3D << "kids 0\n";
}

////////////////////////////////////////////////////////////////////////////////
// setters
////////////////////////////////////////////////////////////////////////////////
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

////////////////////////////////////////////////////////////////////////////////
// getters
////////////////////////////////////////////////////////////////////////////////
float ScanDraiD::getCameraHFov() const
    throw()
{
    return cameraHFov_;
}
float ScanDraiD::getCameraVFov() const
    throw()
{
    return cameraVFov_;
}
float ScanDraiD::getCameraDistance() const
    throw()
{
    return cameraDistance_;
}
float ScanDraiD::getLaserOffset() const
    throw()
{
    return laserOffset_;
}
unsigned int ScanDraiD::getHorizontalAverage() const
    throw()
{
    return horizAvg_;
}
unsigned int ScanDraiD::getVerticalAverage() const
    throw()
{
    return vertAvg_;
}
unsigned int ScanDraiD::getFrameSkip() const
    throw()
{
    return frameSkip_;
}
unsigned int ScanDraiD::getLineSkip() const
    throw()
{
    return lineSkip_;
}


} // namespace scanDraiD

