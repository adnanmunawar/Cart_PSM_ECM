#include <iostream>
#include <ros/ros.h>
#include <std_msgs/Float64.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <boost/bind.hpp>
#include <gazebo_msgs/LinkState.h>
#include <gazebo_msgs/LinkStates.h>
#include <opencv2/opencv.hpp>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/Image.h>

class dvrk_gazebo_control{
private:
  std::vector<ros::Publisher> ecmPub;
  ros::Subscriber link_states, image_sub;
  ros::Publisher plot_x, plot_y, plot_z;
  std::vector<ros::Publisher> cartPub;
public:
  dvrk_gazebo_control(ros::NodeHandle n){
    ecmPub.resize(4);
    ecmPub[0]=n.advertise<std_msgs::Float64>("/dvrk/ecm/yaw_joint/SetPosition",1000);
    ecmPub[1]=n.advertise<std_msgs::Float64>("/dvrk/ecm/pitch_back_joint/SetPositionTarget",1000);
    ecmPub[2]=n.advertise<std_msgs::Float64>("/dvrk/ecm/main_insertion_joint/SetPositionTarget",1000);
    ecmPub[3]=n.advertise<std_msgs::Float64>("/dvrk/ecm/tool_joint/SetPosition",1000);
    plot_x=n.advertise<std_msgs::Float64>("/plotx",1000);
    plot_y=n.advertise<std_msgs::Float64>("/ploty",1000);
    plot_z=n.advertise<std_msgs::Float64>("/plotz",1000);
    cartPub.resize(19);
    char topic[100];
    for (int i=1;i<4;i++)
    {
      for (int j=0;j<5;j++)
      {
        sprintf(topic, "/dvrk/SUJ/SUJ_PSM%d_J%d/SetPosition", i,j);

        cartPub[5*(i-1)+j] = n.advertise<std_msgs::Float64>(topic,1000);
      }
    }
    for (int j=0;j<4;j++)
    {
      sprintf(topic, "/dvrk/SUJ/SUJ_ECM_J%d/SetPosition",j);

      cartPub[15+j] = n.advertise<std_msgs::Float64>(topic,1000);
    }
    link_states=n.subscribe("/gazebo/link_states", 100, &dvrk_gazebo_control::getECMEndEffector, this);
    image_sub = n.subscribe("/dvrk/ecm/camera/image_raw", 100, &dvrk_gazebo_control::showImage, this);
  }

  void showImage(const sensor_msgs::ImageConstPtr& img);
  void getECMEndEffector(const gazebo_msgs::LinkStatesPtr &msg);
  void PublishCartStates();

};
