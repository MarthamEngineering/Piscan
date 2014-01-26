/*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU LESSER GENERAL PUBLIC LICENSE Version 3 as published by
* the Free Software Foundation.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU LESSER GENERAL PUBLIC LICENSE Version 3 for more details.
*
* Changelog:
*
* 2007 The original source was created by Steve Baker:
*    http://www.sjbaker.org/wiki/index.php?title=A_Simple_3D_Scanner
* 2009-02-20 <a.w@quirxi.net>:
*   Rewrite of code so that it can be used as a command line tool.
*/

#ifndef ___SCANDRAID___
#define ___SCANDRAID___

#include <string>
#include <sstream>
#include <stdexcept>
#include <vector>

#include <QObject>

namespace scanDraiD {

/**
* TODO
*/
class ScanDraiD : public QObject
{
    Q_OBJECT

public:
    ScanDraiD();
    ~ScanDraiD();
    // misc
    static const float DEGREES_TO_RADIANS;
    static std::string describeConfiguration();
    void processFrames(const std::string& path2Frames, std::stringstream& scanResult, std::string& fileType)
        throw (std::runtime_error);

    // getters
    float getCameraHFov() const
        throw();
    float getCameraVFov() const
        throw();
    float getCameraDistance() const
        throw();
    float getLaserOffset() const
        throw();
    unsigned int getHorizontalAverage() const
        throw();
    unsigned int getVerticalAverage() const
        throw();
    unsigned int getFrameSkip() const
        throw();
    unsigned int getLineSkip() const
        throw();
    // setters
    void setCameraHFov(const float cameraHFov, bool adjustCameraVFov=true)
        throw (std::runtime_error);
    void setCameraVFov(const float cameraVFov)
        throw (std::runtime_error);
    void setCameraDistance(const float cameraDistance)
        throw (std::runtime_error);
    void setLaserOffset(const float laserOffset)
        throw (std::runtime_error);
    void setHorizontalAverage(const unsigned int horizAvg)
        throw (std::runtime_error);
    void setVerticalAverage(const unsigned int vertAvg)
        throw (std::runtime_error);
    void setFrameSkip(const unsigned int frameSkip)
        throw (std::runtime_error);
    void setLineSkip(const unsigned int lineSkip)
        throw (std::runtime_error);

signals:
    void percentageComplete(QString text, int timeOut);
    void addPointScanView(float, float, float);

private:

    struct point
    {
        point(const float x, const float y, const float z)
            : x_(x), y_(y), z_(z)
        {;}

        float x_;
        float y_;
        float z_;
    };
    struct frame
    {
        std::vector<point> points_;
    };
    /**
    * Converts two Angles with a Side (Angle-Side-Angle) into Side-Angle-Side -
    * the remaining facts from the triangle.
    * @todo reorder parameter list ?
    */
    void convertAngleSideAngle2SideAngleSide(float angA, const float lenB, float angC, float* const lenA,
                                      float* const angB, float* const lenC) const
        throw();
    void getNumberOfFrames(const std::string& path2Frames)
        throw();
    /*
    * @todo change algo - so that id does not look vor points/vertices where there are non - probably use masking features for that ?
    */
    void processSingleFrame(const std::string& fileName, const unsigned int frameNr, std::vector<frame>& vecFrames) ///const
        throw();
    void createAC3DFile(const std::vector<frame>& vecFrames, std::stringstream& file3D) const
        throw();
    void createPlyFile(const std::vector<frame>& vecFrames, std::stringstream& file3D) const
        throw();

    // configuration parameters
    /** Focal distance of camera lense. Default: 50.0 degrees. */
    float cameraHFov_;        
    /** Default: (CAMERA_HFOV*4.0/5.0) degrees. */ 
    float cameraVFov_;         
    /** Num horizontal points to average. Default: 10. */
    unsigned int horizAvg_;    
    /** Num vertical points to average. Default: 10. */
    unsigned int vertAvg_;     
    /** Use only every n'th frame for speed. Default: 1. */
    unsigned int frameSkip_;   
    /** Use only every n'th scanline for speed. Default: 1. */
    unsigned int lineSkip_;    
    /** Distance from camera to center of turntable in mm. */
    float cameraDistance_;     
    /** Angle between the centerline of the camera and the laser. */
    float laserOffset_;        
    /**
    * Stores number of frames to be analysed. 
    * @todo: removme !? 
    */
    unsigned int numberFrames_;
    /** @todo: removme !? */
    mutable unsigned int numberPoints_;  
};


} // namespace scanDraiD
#endif // ___SCANDRAID___

