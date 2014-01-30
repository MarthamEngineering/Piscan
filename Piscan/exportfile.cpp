#include "exportfile.h"


exportfile::exportfile()
{
}
/*

void exportfile::createPlyFile(const std::vector<frame>& vecFrames, std::stringstream& file3D) const
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

}
////////////////////////////////////////////////////////////////////////////////
void exportfile::createAC3DFile(const std::vector<frame>& vecFrames, std::stringstream& file3D) const
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
*/
