#Python client library for ROS
import rospy
import sys
import time
import math

#This module helps to receive values from serial port
from SerialDataGateway import SerialDataGateway
#Importing ROS data types
from std_msgs.msg import Int16,Int32, Int64, Float32, String, Header, UInt64
#Importing ROS data type for IMU
from sensor_msgs.msg import Imu

from geometry_msgs.msg import Quaternion
from geometry_msgs.msg import Twist
from nav_msgs.msg import Odometry
from tf.broadcaster import TransformBroadcaster


class LaunchNode_Class(object):

    def __init__(self):
        print "Initializing LaunchNode Class"




        #
        port = rospy.get_param("~port", "/dev/ttyACM0")
        baudRate = int(rospy.get_param("~baudRate", 115200))

        rospy.loginfo("Starting with serial port: "+ port+", baud rate: "+ str(baudRate))
        #
        self._SerialDataGateway = SerialDataGateway(port, baudRate, self._HandleReceivedLine)
        rospy.loginfo("Started serial communication")


#Subscribers and Publishers

        self._qx_ = rospy.Publisher('qx',Float32,queue_size = 10)
        self._qy_ = rospy.Publisher('qy',Float32,queue_size = 10)
        self._qz_ = rospy.Publisher('qz',Float32,queue_size = 10)
        self._qw_ = rospy.Publisher('qw',Float32,queue_size = 10)

        self._th_ = rospy.Publisher('th',Float32,queue_size = 10)

    def _HandleReceivedLine(self, line):
        if(len(line) > 0):
            lineParts = line.split('\t')
            try:
                #
                if(lineParts[0] == 'q'):
                    self._qw = float(lineParts[1])
                    self._qx = float(lineParts[2])
                    self._qy = float(lineParts[3])
                    self._qz = float(lineParts[4])

                    self._qw_.publish(self._qw)
                    self._qx_.publish(self._qx)
                    self._qy_.publish(self._qy)
                    self._qz_.publish(self._qz)

                if(lineParts[0] == 'e'):
                    self._th = float(lineParts[1])
                    self._th_.publish(self._th)

            #    if(lineParts[0] == 'o'):



            except:
                rospy.logwarn("Error in Sensor values")
                rospy.logwarn(lineParts)
                pass



    def _WriteSerial(self, message):
        self._SerialDataGateway.Write(message)





    def Start(self):
        rospy.logdebug("Starting")
        self._SerialDataGateway.Start()

    def Stop(self):
        rospy.logdebug("Stopping")
        self._SerialDataGateway.Stop()

    def Reset_LaunchNode(self):
        print "Reset"
        reset = 'r\r'
        self._WriteSerial(reset)
        time.sleep(1)
        self._WriteSerial(reset)
        time.sleep(2)


if __name__ =='__main__':
    rospy.init_node('launchnode_ros',anonymous=True)
    launchnode = LaunchNode_Class()
    try:

        launchnode.Start()
        rospy.spin()
    except rospy.ROSInterruptException:
        rospy.logwarn("Error in main function")


    launchnode.Reset_LaunchNode()
    launchnode.Stop()
