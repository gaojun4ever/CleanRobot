
import roslib
#roslib.load_manifest('turtlebot_teleop') # No longer needed in catkin!
import rospy

from std_msgs.msg import String

import sys, select, termios, tty


def getKey():
    tty.setraw(sys.stdin.fileno())
    rlist, _, _ = select.select([sys.stdin], [], [], 0.1)
    if rlist:
        key = sys.stdin.read(1)
    else:
        key = ''

    termios.tcsetattr(sys.stdin, termios.TCSADRAIN, settings)
    return key

if __name__=="__main__":
    settings = termios.tcgetattr(sys.stdin)
    rospy.init_node('teleop')
    pub = rospy.Publisher('~cmd_vel',String , queue_size=5)
    
    while(1):
        key = getKey()
        if(key!=''):
            print key
            pub.publish(key)
            key=''
