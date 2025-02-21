#include "ros/ros.h"
#include "std_msgs/Float64MultiArray.h"
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>

class Node{

    public:    
        Node() {
            // Publisher for sending Float64MultiArray messages
            command_pub = nh.advertise<std_msgs::Float64MultiArray>("/myrobot/four_joints_position_controllers/command", 10);

            // Initialize data with zero values
            for (int i = 0; i < 4; ++i)
                command_msg.data.push_back(0.0);

            run();
        }

    private:
        // Variables
        ros::NodeHandle nh;
        ros::Publisher command_pub;
        std_msgs::Float64MultiArray command_msg;
        float joint1 = 0;
        float joint2 = 0;
        float joint3 = 0;
        float joint4 = 0;

        // Main function to run
        void run() {
            // Rate at which to publish messages
            ros::Rate loop_rate(10);  // Adjust the rate as needed
            while (ros::ok()) {
                // Read keyboard input
                int key = getch();

                // Determine which joint to control based on keyboard input
                bool check = false;
		if (key == 'q') {
		    joint1 += 0.1;
                    check = true;
                }
                else if (key == 'z') {
		    joint1 -= 0.1;
                    check = true;
                }
		else if (key == 'w') {
		    joint2 += 0.1;
                    check = true;
                }
		else if (key == 'x') {
		    joint2 -= 0.1;
                    check = true;
                }
		else if (key == 'e') {
		    joint3 += 0.1;
                    check = true;
                }
		else if (key == 'c') {
		    joint3 -= 0.1;
                    check = true;
                }
		else if (key == 'r') {
		    joint4 += 0.1;
                    check = true;
                }
		else if (key == 'v') {
		    joint4 -= 0.1;
                    check = true;
                }
                else if (key == 'a') {
		    joint1 = 0;
                    check = true;
                }
		else if (key == 's') {
		    joint2 = 0;
                    check = true;
                }
		else if (key == 'd') {
		    joint3 = 0;
                    check = true;
                }
		else if (key == 'f') {
		    joint4 = 0;
                    check = true;
                }

                // Clear previous data
                command_msg.data.clear();

                // Initialize data with zero values
                for (int i = 0; i < 4; ++i)
                    command_msg.data.push_back(0.0);

                // Fill in message
                command_msg.data[0] = joint1;
                command_msg.data[1] = joint2;
                command_msg.data[2] = joint3;
                command_msg.data[3] = joint4;

                // Publish the message if any joint command is given
                if (check) {
                    command_pub.publish(command_msg);
                    ROS_INFO("Command sent: [%f, %f, %f, %f]", command_msg.data[0], command_msg.data[1], command_msg.data[2], command_msg.data[3]);
                }

                // Spin once to handle callbacks
                ros::spinOnce();

                // Sleep to maintain the specified rate
                loop_rate.sleep();
            }
        }
 
        // Function to read keyboard input without waiting for Enter key
        int getch() {
            static struct termios oldt, newt;
            tcgetattr(STDIN_FILENO, &oldt);
            newt = oldt;
            newt.c_lflag &= ~(ICANON);
            tcsetattr(STDIN_FILENO, TCSANOW, &newt);
            int ch = getchar();
            tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
            return ch;
        }
};

int main(int argc, char **argv) {
    // Initialize ROS node
    ros::init(argc, argv, "keyboard_controller");
    Node node;
    ros::spin();
    return 0;
} 
