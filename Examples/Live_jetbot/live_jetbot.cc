#include<iostream>
#include<algorithm>
#include<fstream>
#include<chrono>
#include<iomanip>

#include<opencv2/core/core.hpp>
#ifdef OPENCV4
#include <opencv2/imgcodecs/legacy/constants_c.h>
#endif

#include"System.h"

using namespace std;

int main(int argc, char **argv)
{
    if(argc != 3)
    {
        cerr << endl << "Usage: ./livejetbot path_to_vocabulary path_to_settings" << endl;
        return 1;
    }

    // Create SLAM system. It initializes all system threads and gets ready to process frames.
    ORB_SLAM2::System SLAM(argv[1], argv[2], ORB_SLAM2::System::MONOCULAR, true, true);

    string gstreamPipeline =
        "udpsrc port=5001 !"
        "application/x-rtp,payload=96,encoding-name=H264 !"
        "rtpjitterbuffer mode=1 ! rtph264depay !"
        "decodebin !"
        "videoconvert ! video/x-raw, format=(string)GRAY8 !"
        "videoscale ! video/x-raw, width=640, height=360 !"
        "appsink";
    cv::VideoCapture videoCapture(gstreamPipeline, cv::CAP_GSTREAMER);

    // Wecam:
    //cv::VideoCapture videoCapture("v4l2src ! video/x-raw,format=BGR,width=640,height=480,framerate=30/1 ! videoconvert ! appsink", cv::CAP_GSTREAMER);
    
    //cout << cv::getBuildInformation() << endl;

    if(!videoCapture.isOpened()) {
        cout << "Error, cannot open video stream" << endl;
        return 2;
    }

    // Main loop
    cv::Mat cvImg;
    int imageNo = 0;
    while(imageNo < 500)
    {
        videoCapture >> cvImg;

        if(cvImg.empty()) {
            break;
        }
        
        // Pass the image to the SLAM system
        //if(imageNo % 2 == 0)
        {
            double tframe = 0.03333 * imageNo; // TODO proper frame time.
            SLAM.TrackMonocular(cvImg, tframe); //, imageProcessedNo);
            imageNo++;
        }
    }

    videoCapture.release();

    // Stop all threads
    SLAM.Shutdown();
/*
    // Save camera trajectory
    SLAM.SaveKeyFrameTrajectoryTUM("kf_trajectory.tum", p.fps_multiplier);    
    SLAM.SaveTrajectoryTUM("trajectory.tum", p.fps_multiplier);
    SLAM.SaveTrajectoryTUMLost("trajectory_lost.tum", p.fps_multiplier);
    //SLAM.SaveKeyFrameTrajectoryKITTI("kf_trajectory.kitti");    
    SLAM.SaveTrajectoryKITTI("trajectory.kitti");
*/

    return 0;
}
