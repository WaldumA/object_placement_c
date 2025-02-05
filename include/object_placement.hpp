#ifndef OBJECT_PLACEMENT_NODE  
#define OBJECT_PLACEMENT_NODE 

// Dependencies
#include "math_functions.hpp"
#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>
#include <nav_msgs/Odometry.h>
#include <vortex_msgs/ObjectPlacement.h>
#include <darknet_ros_msgs/BoundingBoxes.h>
#include <vector>
#include <tuple>
#include <list>
#include <math.h>
 
using namespace std;



class objectClass {

public:
    // Necesarry variables
    string name;
    bool detected;
    int x_pos, y_pos, z_pos, queue;

    objectClass() {
        // Initialising variables
        //name = name_of_object;
        detected = false;
        x_pos = -1;
        y_pos = -1;
        z_pos = -1;
        queue = 0;

    }

    ~objectClass() {

    }

    // Update position
    void update_object(Vector2i map_coordinates);  


};

class objectPlacing {

private:
// ROS handle
ros::NodeHandle nh;

// Variables to store messages from callback functions
sensor_msgs::LaserScan sonar_data;
nav_msgs::Odometry ekf_data;
darknet_ros_msgs::BoundingBoxes darknet_data;

// Publisher
vortex_msgs::ObjectPlacement objectPlacement_data;
ros::Publisher objectPlacement_pub;

// Subscribers
ros::Subscriber ekf_sub;
ros::Subscriber sonar_sub;
ros::Subscriber darknet_sub;

// Callback timer
ros::Timer timer;

// Calibrated parameters for pixel to sonar mapping
float a, b, c, d;

// Initiates objectClasses
objectClass bootlegger;
objectClass gman;
objectClass tommygun;
objectClass badge;

// ROSparams
float rate;
int size,scale;
string laser_topic, odometry_topic, darknet_topic, object_placement_topic;

public:
// Constructor
objectPlacing(int argc, char** argv) {

    // Get ROSparams
    nh.getParam("MAP_SIZE",size);
    nh.getParam("SCALE",scale);
    nh.getParam("PUBLISHING_RATE",rate);
    nh.getParam("LASER_SCAN_TOPIC", laser_topic);
    nh.getParam("ODOMETRY_TOPIC", odometry_topic);
    nh.getParam("DARKNET_BBOX_TOPIC",darknet_topic);
    nh.getParam("OBJECT_PLACEMENT_TOPIC",object_placement_topic);

    // Initiate publisher
    objectPlacement_pub = nh.advertise<vortex_msgs::ObjectPlacement>(object_placement_topic,100);

    // Initiate subscribers
    ekf_sub = nh.subscribe(odometry_topic,100,&objectPlacing::ekfCallback, this);
    sonar_sub = nh.subscribe(laser_topic,100,&objectPlacing::sonarCallback, this);
    darknet_sub = nh.subscribe(darknet_topic,100,&objectPlacing::darknetCallback, this);
    timer = nh.createTimer(ros::Duration(rate),&objectPlacing::publishCallback, this);



}
// Destructor
~objectPlacing() {

}

// Callback functions
void sonarCallback(const sensor_msgs::LaserScan::ConstPtr& msg);
void ekfCallback(const nav_msgs::Odometry::ConstPtr& msg);
void darknetCallback(const darknet_ros_msgs::BoundingBoxes::ConstPtr& msg);
void publishCallback(const ros::TimerEvent& event);

// Object Placement Functions
tuple<float,float> calculate_bearings(tuple<int,int,int,int,string> bbox);
float pixel_to_sonar(int pixel_width);
float calculate_depth(tuple<float,float> bearings);
Vector2i calculate_map_coordinates(tuple<float,float> bearings, float depth);
// ObjectPlacement message functions



};

#endif