#pragma once
#ifndef MY_ICUB_H
#define MY_ICUB_H

#include <string.h>
#include <fstream>
#include <vector>
using namespace std;

#include <yarp/dev/PolyDriver.h>
#include <yarp/dev/GazeControl.h>
#include <yarp/dev/CartesianControl.h>
#include <yarp/dev/IPositionControl.h>
#include <yarp/dev/IControlLimits.h>
using namespace yarp::dev;

#include <yarp/sig/Image.h>
#include <yarp/sig/Vector.h>
using namespace yarp::sig;

#include <yarp/os/BufferedPort.h>
#include <yarp/os/RpcClient.h>

using namespace yarp::os;

class My_ICub {
    
    public:
        My_ICub(string robot_name="/icubSim", string own_port_name="/mysim");
        ~My_ICub();

        string getFullPortName(string port, bool own);
        enum Way {HAND_WATCHING, LOOK};
        enum Hand {RIGHT, LEFT};
        //void collectingData(string path, int number, Way way);

        int getRightArmJoints();
        void closeDataFile();
        //void prepareDatasetFile(string path, double const angle);
        void test();

//void randomHandWatchCollecting(string path, int startFrom, int total, int imagesCount, bool armSeen);
void randomLookWayCollecting(string path, int startFrom, int total);

protected:
        string
            robot_name,
            own_port_name;
        bool connectToPort(string port, bool write);

    private:
        Vector x, o;
        string
            world_port,
            head_port,
            left_cam_port,
            right_cam_port,
            right_arm_port,
            left_arm_port,
            last_head_position,
            last_arm_position;

        ofstream datafile;
        int getDataFile(string path);
        double randomAngle(double minAngle, double maxAngle);

        ImageOf<PixelRgb> *getRobotRightEyeImage();
        ImageOf<PixelRgb> *getRobotLeftEyeImage();

        PolyDriver *head_driver;
        PolyDriver *right_arm_driver;
        PolyDriver *left_arm_driver;
        RpcClient  *world_client;
        IControlLimits *headLimit;

        IPositionControl *head_controller;
        IPositionControl *right_arm_controller;
        IPositionControl *left_arm_controller;

        BufferedPort<ImageOf<PixelRgb>> *left_cam;
        BufferedPort<ImageOf<PixelRgb>> *right_cam;

        void getHeadController();
        void getArmController(Hand hand);

        PolyDriver *getRobotHeadDriver();
        PolyDriver *getRobotArmDriver(Hand hand);

        BufferedPort<ImageOf<PixelRgb>> *getRobotRightEyeDriver();
        BufferedPort<ImageOf<PixelRgb>> *getRobotLeftEyeDriver();

        PolyDriver *gaze_driver;
        IGazeControl *iGaze;
        ICartesianControl *iCarCtrl;

        void getRobotGazeInteface();
        void getWorldRpcClient();
        void getRightPalmWorldPosition(Vector &vector);
        void setRightArmVector();

        Vector right_arm_vector;
        Vector head_limit_vector;
        void setHeadAnglesAndMove(Vector pose);
        void getCurrentFixPoint(Vector &vector);
        int randomHeadMotions(int direction, int steps, double minAng, double maxAngle, double maxError);
        void getHeadCurrentVector(Vector &headAngles);
        void invKinArmMovement(Hand hand, Vector pose);
        void armMovement(Vector diff, bool wait);
        void printVector(Vector vec);
        bool checkHeadAngles(Vector headAngles);
        string vectorDataToString(Vector vector);
        int checkError(Vector error, const double maxErr);
        void setRandomVergenceAngle();
        void getArmJoints(Vector &armJoints);
        void setArmJoints(Hand hand, Vector joints);
};

#endif