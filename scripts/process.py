

import rospy
#import roslib
#roslib.load_manifest('differential_drive')
from math import sin, cos, pi

from geometry_msgs.msg import Quaternion
from geometry_msgs.msg import Twist
from geometry_msgs.msg import Transform
from nav_msgs.msg import Odometry
from tf.broadcaster import TransformBroadcaster
from std_msgs.msg import Int16, Int64, Float32
from sensor_msgs.msg import LaserScan


#############################################################################
class DiffTf:
#############################################################################

    #############################################################################
    def __init__(self):
    #############################################################################
        rospy.init_node("process")
        self.nodename = rospy.get_name()
        rospy.loginfo("-I- %s started" % self.nodename)

        #### parameters #######
        self.rate = rospy.get_param('~rate',10.0)  # the rate at which to publish the transform


        self.base_frame_id = rospy.get_param('~base_frame_id','base_link') # the name of the base frame of the robot
        self.odom_frame_id = rospy.get_param('~odom_frame_id', 'odom') # the name of the odometry reference frame
        self.laser_frame_id = rospy.get_param('~laser_frame_id', 'laser_link')
        self.map_frame_id = rospy.get_param('~map_frame_id', 'map')
        self.t_delta = rospy.Duration(1.0/self.rate)
        self.t_next = rospy.Time.now() + self.t_delta

        # internal data

        self.x = 0                  # position in xy plane
        self.y = 0
        self.th = 0
        self.th_pre = 0
        self.dx = 0                 # speeds in x/rotation
        self.dr = 0
        self.then = rospy.Time.now()

        self.qw = 0
        self.qx = 0
        self.qy = 0
        self.qz = 0
        self.laser = LaserScan()
        # subscriptions

        rospy.Subscriber("qx", Float32,self.qx_update)
        rospy.Subscriber("qy", Float32,self.qy_update)
        rospy.Subscriber("qz", Float32,self.qz_update)
        rospy.Subscriber("qw", Float32,self.qw_update)
        rospy.Subscriber("th", Float32,self.th_update)
        rospy.Subscriber("scan", LaserScan,self.laser_update)
        self.odomPub = rospy.Publisher("odom", Odometry,queue_size=10)
        self.laserPub = rospy.Publisher("laser_scan", LaserScan,queue_size=20)
        self.odomBroadcaster = TransformBroadcaster()
        self.laserBroadcaster = TransformBroadcaster()

    #############################################################################
    def spin(self):
    #############################################################################
        r = rospy.Rate(self.rate)
        while not rospy.is_shutdown():
            self.update()
            r.sleep()


    #############################################################################
    def update(self):
    #############################################################################
        now = rospy.Time.now()
        if now > self.t_next:
            elapsed = now - self.then
            self.then = now
            elapsed = elapsed.to_sec()



            # this approximation works (in radians) for small angles
            th = self.th - self.th_pre

            self.dr = th / elapsed

            # publish the odom information
            quaternion = Quaternion()
            quaternion.x = self.qx
            quaternion.y = self.qy
            quaternion.z = self.qz
            quaternion.w = self.qw
            self.odomBroadcaster.sendTransform(
                (self.x, self.y, 1),
                (quaternion.x, quaternion.y, quaternion.z, quaternion.w),
                rospy.Time.now(),
                self.base_frame_id,
                self.odom_frame_id
                )


            self.laserBroadcaster.sendTransform(
                (self.x, self.y, 0),
                (0, 0, quaternion.z, quaternion.w),
                rospy.Time.now(),
                self.laser_frame_id,
                self.base_frame_id
            )



            odom = Odometry()
            odom.header.stamp = now
            odom.header.frame_id = self.odom_frame_id
            odom.pose.pose.position.x = 0
            odom.pose.pose.position.y = 0
            odom.pose.pose.position.z = 0
            odom.pose.pose.orientation = quaternion
            odom.child_frame_id = self.base_frame_id
            odom.twist.twist.linear.x = 0
            odom.twist.twist.linear.y = 0
            odom.twist.twist.angular.z = self.dr
            self.odomPub.publish(odom)
            laser = LaserScan()
            laser.header.stamp = now
            laser.header.frame_id = self.laser_frame_id
            laser.angle_min = self.laser.angle_min
            laser.angle_max = self.laser.angle_max
            laser.angle_increment =self.laser.angle_increment
            laser.time_increment= self.laser.time_increment
            laser.scan_time = self.laser.scan_time
            laser.range_min = self.laser.range_min
            laser.range_max = self.laser.range_max
            laser.ranges = self.laser.ranges
            laser.intensities = self.laser.intensities
            self.laserPub.publish(laser)

    def qx_update(self, msg):
        self.qx = msg.data
    def qy_update(self, msg):
        self.qy = msg.data
    def qz_update(self, msg):
        self.qz = msg.data
    def qw_update(self, msg):
        self.qw = msg.data
    def th_update(self, msg):
        self.th_pre = self.th
        self.th = msg.data
    def laser_update(self, msg):
        self.laser=msg
        # self.laser.angle_min = msg.angle_min
        # self.laser.angle_max = msg.angle_max
        # self.laser.angle_increment =msg.angle_increment
        # self.laser.time_increment= msg.time_increment
        # self.laser.scan_time = msg.scan_time
        # self.laser.range_min = msg.range_min
        # self.laser.range_max = msg.range_max
        # self.laser.ranges = msg.ranges
        # self.laser.intensities = msg.intensities





#############################################################################
#############################################################################
if __name__ == '__main__':
    """ main """
    diffTf = DiffTf()
    diffTf.spin()
