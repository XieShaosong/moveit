/*********************************************************************
* Software License Agreement (BSD License)
*
*  Copyright (c) 2012, Willow Garage, Inc.
*  All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*   * Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*   * Redistributions in binary form must reproduce the above
*     copyright notice, this list of conditions and the following
*     disclaimer in the documentation and/or other materials provided
*     with the distribution.
*   * Neither the name of the Willow Garage nor the names of its
*     contributors may be used to endorse or promote products derived
*     from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
*  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
*  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
*  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
*  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
*  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
*  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
*  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
*  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
*  POSSIBILITY OF SUCH DAMAGE.
*********************************************************************/

#include <planning_interface/planning_interface.h>
#include <planning_request_adapter/planning_request_adapter.h>
#include <pluginlib/class_loader.h>
#include <boost/scoped_ptr.hpp>
#include <ros/ros.h>

namespace planning_pipeline
{

class PlanningPipeline
{

public:
  
  PlanningPipeline(const planning_models::KinematicModelConstPtr& model, 
                   const std::string &planning_plugin_param_name = "planning_plugin",
                   const std::string &adapter_plugins_param_name = "request_adapters");
  
  PlanningPipeline(const planning_models::KinematicModelConstPtr& model, 
                   const std::string &planning_plugin_name,
                   const std::vector<std::string> &adapter_plugin_names);
  
  void displayComputedMotionPlans(bool flag);
  
  void publishReceivedRequests(bool flag);
  
  void checkSolutionPaths(bool flag);
  
  bool getDisplayComputedMotionPlansFlag(void) const
  {
    return display_computed_motion_plans_;
  }
  
  bool getPublishReceivedRequests(void) const
  {
    return publish_received_requests_;
  }

  bool getCheckSolutionPaths(void) const
  {
    return check_solution_paths_;
  }
  
  bool generatePlan(const planning_scene::PlanningSceneConstPtr& planning_scene,
                    const moveit_msgs::GetMotionPlan::Request& req,
                    moveit_msgs::GetMotionPlan::Response& res) const;
  
  bool generatePlan(const planning_scene::PlanningSceneConstPtr& planning_scene,
                    const moveit_msgs::GetMotionPlan::Request& req,
                    moveit_msgs::GetMotionPlan::Response& res,
                    std::vector<std::size_t> &adapter_added_state_index) const;
  
  const std::string& getPlannerPluginName(void) const
  {
    return planner_plugin_name_;
  }
  
  const std::vector<std::string>& getAdapterPluginNames(void) const
  {
    return adapter_plugin_names_;
  }
  
private:
  
  void configure(const planning_models::KinematicModelConstPtr& model);
  
  ros::NodeHandle nh_;

  /// Flag indicating whether motion plans should be published as a moveit_msgs::DisplayTrajectory
  bool display_computed_motion_plans_;
  ros::Publisher display_path_publisher_;

  /// Flag indicating whether received requests should be published just before beginning processing (useful for debugging)
  bool publish_received_requests_;
  ros::Publisher received_request_publisher_;

  boost::scoped_ptr<pluginlib::ClassLoader<planning_interface::Planner> > planner_plugin_loader_;
  planning_interface::PlannerPtr planner_instance_;
  std::string planner_plugin_name_;
  
  boost::scoped_ptr<pluginlib::ClassLoader<planning_request_adapter::PlanningRequestAdapter> > adapter_plugin_loader_;
  boost::scoped_ptr<planning_request_adapter::PlanningRequestAdapterChain> adapter_chain_;
  std::vector<std::string> adapter_plugin_names_;
  
  /// Flag indicating whether the reported plans should be checked once again, by the planning pipeline itself
  bool check_solution_paths_;
  ros::Publisher contacts_publisher_;
  
};

}
