/* -*- mode: C++ -*- */
/*
 *  Copyright (C) 2012 Austin Robot Technology, Jack O'Quin
 *  Copyright (C) 2020, Tier IV, Inc., David Robert Wong
 *
 *  License: Modified BSD Software License Agreement
 *
 *  $Id$
 */

/** \file
 *
 *  ROS driver interface for the Velodyne 3D LIDARs
 */

#ifndef _VELODYNE_DRIVER_H_
#define _VELODYNE_DRIVER_H_ 1

#include <string>
#include <ros/ros.h>
#include <diagnostic_updater/diagnostic_updater.h>
#include <diagnostic_updater/publisher.h>
#include <dynamic_reconfigure/server.h>

#include <velodyne_driver/input.h>
#include <velodyne_driver/VelodyneNodeConfig.h>

namespace velodyne_driver
{

class VelodyneDriver
{
public:

  VelodyneDriver(ros::NodeHandle node,
                 ros::NodeHandle private_nh);
  ~VelodyneDriver() {}

  bool poll(void);

private:

  ///Callback for dynamic reconfigure
  void callback(velodyne_driver::VelodyneNodeConfig &config,
              uint32_t level);

  ///Pointer to dynamic reconfigure service srv_
  boost::shared_ptr<dynamic_reconfigure::Server<velodyne_driver::
              VelodyneNodeConfig> > srv_;

  // configuration parameters
  struct
  {
    std::string frame_id;            ///< tf frame ID
    std::string model;               ///< device model name
    double rpm;                      ///< device rotation rate (RPMs)
    double sensor_phase;             ///< sensor phase (degrees)
    double time_offset;              ///< time in seconds added to each velodyne time stamp
  } config_;

  boost::shared_ptr<Input> input_;
  ros::Publisher output_;

  /** diagnostics updater */
  diagnostic_updater::Updater diagnostics_;
  double diag_min_freq_;
  double diag_max_freq_;
  boost::shared_ptr<diagnostic_updater::TopicDiagnostic> diag_topic_;

  uint16_t prev_packet_azm;
  uint16_t curr_packet_azm;
  uint8_t  curr_packet_rmode; //    [strongest return or farthest mode => Singular Retruns per firing]
                              // or [Both  => Dual Retruns per fire]
  uint8_t  curr_packet_sensor_model; // extract the sensor id from packet
  std::string dump_file; // string to hold pcap file name
};

} // namespace velodyne_driver

#endif // _VELODYNE_DRIVER_H_
