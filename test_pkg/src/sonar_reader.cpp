#include "ros/ros.h"
#include "std_msgs/String.h"
#include <sensor_msgs/Range.h>
#include <geometry_msgs/Twist.h>
#include <tf/transform_listener.h>



/**
 * This tutorial demonstrates simple receipt of messages over the ROS system.
 */
 
float min_range=10000000;
int counter=0;
std::string name;
 
class Sub {
	public: 
		Sub(std::string topic_sonar);
	private:
		std::string name_sonar;
		ros::NodeHandle n;
		void sonarCallback(sensor_msgs::Range msg);
		float sonar_range;
		ros::Subscriber sub;
		ros::Publisher pub;
		
}; 

Sub::Sub(std::string topic_sonar) {
	 name_sonar=topic_sonar;
	 sub = n.subscribe(topic_sonar, 1000, &Sub::sonarCallback, this);
	 pub = n.advertise<geometry_msgs::Twist>("/robot0/cmd_vel", 1000);

}	 	 
	
void Sub::sonarCallback (sensor_msgs::Range msg)
{
  sonar_range= msg.range;
  counter++;
  
  ROS_INFO("%s", name_sonar.c_str());
  ROS_INFO("Range : %f",sonar_range);
  
  if(sonar_range < min_range)
  {	
		min_range= sonar_range;
		//name=name_sonar;
		name=msg.header.frame_id;	
		
  }
  if (counter==4)
  {
	ROS_INFO("Min Range : %f",min_range);
	ROS_INFO("Name : %s",name.c_str());
	counter=0;
	min_range=10000000;
	geometry_msgs::Twist twist;
	
	if(name=="robot0_sonar_1")
	{
			twist.linear.x=-0.1;
			twist.angular.z=0;
	}
	else if(name=="robot0_sonar_2")
	{
			twist.linear.x=0.1;
			twist.angular.z=-0.1;
	}
	else if (name=="robot0_sonar_3")
	{
			twist.linear.x=0.1;
			twist.angular.z=0;
	}
	else if (name=="robot0_sonar_4")
	{
			twist.linear.x=0.1;
			twist.angular.z=0.1;
	}
	twist.linear.z=twist.linear.y=twist.angular.x = twist.angular.y = 0; 
	pub.publish(twist);
	

	/**tf::TransformListener listener;
	 
    tf::StampedTransform transform;
    try{
      listener.lookupTransform("/robot0",name,ros::Time(0), transform);
    }
    catch (tf::TransformException ex){
      ROS_ERROR("%s",ex.what());
      ros::Duration(1.0).sleep(); **/
      
      
	
  }
 }


int main(int argc, char **argv)
{
  /**
   * The ros::init() function needs to see argc and argv so that it can perform
   * any ROS arguments and name remapping that were provided at the command line.
   * For programmatic remappings you can use a different version of init() which takes
   * remappings directly, but for most command-line programs, passing argc and argv is
   * the easiest way to do it.  The third argument to init() is the name of the node.
   *
   * You must call one of the versions of ros::init() before using any other
   * part of the ROS system.
   */
  ros::init(argc, argv, "sonar_reader");
  
   Sub sub1("/robot0/sonar_1");
   Sub sub2("/robot0/sonar_2");
   Sub sub3("/robot0/sonar_3");
   Sub sub4("/robot0/sonar_4");
 
  /**
   * NodeHandle is the main access point to communications with the ROS system.
   * The first NodeHandle constructed will fully initialize this node, and the last
   * NodeHandle destructed will close down the node.
   */
  //ros::NodeHandle n;

  /**
   * The subscribe() call is how you tell ROS that you want to receive messages
   * on a given topic.  This invokes a call to the ROS
   * master node, which keeps a registry of who is publishing and who
   * is subscribing.  Messages are passed to a callback function, here
   * called chatterCallback.  subscribe() returns a Subscriber object that you
   * must hold on to until you want to unsubscribe.  When all copies of the Subscriber
   * object go out of scope, this callback will automatically be unsubscribed from
   * this topic.
   *
   * The second parameter to the subscribe() function is the size of the message
   * queue.  If messages are arriving faster than they are being processed, this
   * is the number of messages that will be buffered up before beginning to throw
   * away the oldest ones.
   */
 // ros::Subscriber sub = n.subscribe("/robot0/sonar_2", 1000, sonarCallback);
  
  /**
   * ros::spin() will enter a loop, pumping callbacks.  With this version, all
   * callbacks will be called from within this thread (the main one).  ros::spin()
   * will exit when Ctrl-C is pressed, or the node is shutdown by the master.
   */
  ros::spin();

  return 0;
}
