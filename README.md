# Object Placement

### Dependencies
- [ROS-kinetic](http://wiki.ros.org/kinetic)
- [EIGEN](http://eigen.tuxfamily.org/index.php?title=Main_Page)
- [darknet_ros](https://github.com/leggedrobotics/darknet_ros) 

### Required input
- sensor_msgs/LaserScan.msg
- nav_msgs/Odometry.msg
- darknet_ros_msgs/BoundingBoxes.msg 

Change size, scale and topics in config/config.yaml in sonar_mapping_c package

### To run:
```
roslaunch object_placement_c object_placement.launch
```
