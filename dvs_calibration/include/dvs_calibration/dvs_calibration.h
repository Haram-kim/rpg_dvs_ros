#ifndef DVS_CALIBRATION_H
#define DVS_CALIBRATION_H

#include "ros/ros.h"

#include <Eigen/Geometry>
#include <dvs_msgs/Event.h>
#include <dvs_msgs/EventArray.h>
#include "dvs_calibration/pattern.h"
#include "dvs_calibration/circlesgrid.hpp"
#include "dvs_calibration/board_detection.h"
#include "dvs_calibration/transition_map.h"

#include <opencv2/calib3d/calib3d.hpp>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Float64.h>
#include <std_srvs/Empty.h>
#include <sensor_msgs/CameraInfo.h>
#include <sensor_msgs/SetCameraInfo.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <geometry_msgs/PoseStamped.h>

#include <list>

class DvsCalibration
{
public:
  DvsCalibration();
  virtual ~DvsCalibration() {};

protected:
  // parameters
  static const int sensor_width = 128;
  static const int sensor_height = 128;
  static const int blinking_time_us = 1000;
  static const int blinking_time_tolerance = 500;
  static const int enough_transitions_threshold = 200;
  static const int minimum_transitions_threshold = 10;
  static const int minimum_led_mass = 50;
  static const int dots = 5;
  static const double dot_distance = 0.05;

  static const double pattern_search_timeout = 2.0;
  ros::Time last_pattern_found;

  // event maps
  std::map<int, TransitionMap> transition_maps;

  // status
  bool calibration_running;

  // callbacks
  void eventsCallback(const dvs_msgs::EventArray::ConstPtr& msg, int camera_id);

  // services
  bool resetCalibrationCallback(std_srvs::Empty::Request& request, std_srvs::Empty::Response& response);
  bool startCalibrationCallback(std_srvs::Empty::Request& request, std_srvs::Empty::Response& response);
  bool saveCalibrationCallback(std_srvs::Empty::Request& request, std_srvs::Empty::Response& response);

  // virtual functions for these services
  virtual void resetCalibration() = 0;
  virtual void startCalibration() = 0;
  virtual void saveCalibration() = 0;

  // ROS interface
  ros::NodeHandle nh;
  ros::ServiceServer startCalibrationService;
  ros::ServiceServer saveCalibrationService;
  ros::ServiceServer resetCalibrationService;
};

#endif // DVS_CALIBRATION_H
