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
        self._Counter = 0
        self._qx = 0
        self._qy = 0
        self._qz = 0
        self._qw = 0
        self._th = 0
        self._px = 0
        self._py = 0
        self._dx = 0
        self.key=''

        #
        port = rospy.get_param("~port", "/dev/ttyUSB0")
        baudRate = int(rospy.get_param("~baudRate", 115200))

        rospy.loginfo("Starting with serial port: "+ port+", baud rate: "+ str(baudRate))
        #
        self._SerialDataGateway = SerialDataGateway(port, baudRate, self._HandleReceivedLine)
        rospy.loginfo("Started serial communication")


#Subscribers and Publishers

        self._px_ = rospy.Publisher('px',Float32,queue_size = 10)
        self._py_ = rospy.Publisher('py',Float32,queue_size = 10)
        self._dx_ = rospy.Publisher('dx',Float32,queue_size = 10)

        self._qx_ = rospy.Publisher('qx',Float32,queue_size = 10)
        self._qy_ = rospy.Publisher('qy',Float32,queue_size = 10)
        self._qz_ = rospy.Publisher('qz',Float32,queue_size = 10)
        self._qw_ = rospy.Publisher('qw',Float32,queue_size = 10)
        self._th_ = rospy.Publisher('th',Float32,queue_size = 10)

        self._SerialPublisher = rospy.Publisher('serial', String,queue_size=10)

        rospy.Subscriber('/teleop/cmd_vel',String,self.key_update)
    def _HandleReceivedLine(self, line):
        self._Counter = self._Counter + 1
        self._SerialPublisher.publish(String(str(self._Counter) + ", in:  " + line))
        if(len(line) > 0):
            lineParts = line.split('\t')
            try:

                if(lineParts[0] == 'p'):
                    self._px = float(lineParts[1])
                    self._py = float(lineParts[2])
                    self._dx = float(lineParts[3])
                    self._px_.publish(self._px)
                    self._py_.publish(self._py)
                    self._dx_.publish(self._dx)
                    #self._px_ = publish(self._px)
                    #self._py_ = publish(self._py)
                    #self._dx_ = publish(self._dx)
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
                    #self._px_.publish(self._px)
                    #self._py_.publish(self._py)





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


    def key_update(self, msg):
        self.key=msg.data
        if(self.key!=''):
            self._WriteSerial(self.key+'\r')



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
