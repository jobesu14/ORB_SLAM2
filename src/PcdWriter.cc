#include <fstream>
#include <sstream>
#include <iostream>
#include <opencv2/core/core.hpp>

#include "PcdWriter.h"

namespace ORB_SLAM2
{

bool PcdWriter::save(const std::string pcdFile, Map* map)
{
    std::ofstream out(pcdFile);
    if(!out)
    {
        std::cerr << "Cannot Write to pcd file: " << pcdFile << std::endl;
        return false;
    }

    std::cout << "Saving PCD file: " << pcdFile << std::flush;
    out.precision(6);
    out << pcdHeader(map->MapPointsInMap());
    out.flush();

    for(MapPoint *pt : map->GetAllMapPoints())
    {
        cv::Mat pos = pt->GetWorldPos();
        out << pos.at<float>(0) << " " << pos.at<float>(1) << " "
            << pos.at<float>(2) << " " << 4.2108e+06 << std::endl;

        //cv::Mat norm = pt->GetNormal();
        //out << norm.at<float>(0) << " " << norm.at<float>(1) << " " << norm.at<float>(2) << std::endl; 
    }

    out.close();
    std::cout << " ...done" << std::endl;
    return true;
}

std::string PcdWriter::pcdHeader(const uint16_t width, const uint16_t height)
{
    std::ostringstream header;
    header << "# .PCD v.7 - Point Cloud Data file format" << std::endl;
    header << "VERSION .7" << std::endl;
    header << "FIELDS x y z rgb" << std::endl;
    header << "SIZE 4 4 4 4" << std::endl;
    header << "TYPE F F F F" << std::endl;
    header << "COUNT 1 1 1 1" << std::endl;
    header << "WIDTH " << width << std::endl;
    header << "HEIGHT " << height << std::endl;
    header << "VIEWPOINT 0 0 0 1 0 0 0" << std::endl;
    header << "POINTS " << (width * height) << "" << std::endl;
    header << "DATA ascii" << std::endl;
    return header.str();
}

}