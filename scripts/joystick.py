import pygame
import roslib
#roslib.load_manifest('turtlebot_teleop') # No longer needed in catkin!
import rospy

from std_msgs.msg import String



rospy.init_node('teleop')
pub = rospy.Publisher('~cmd_vel',String , queue_size=5)
# Define some colors
BLACK    = (   0,   0,   0)
WHITE    = ( 255, 255, 255)

# This is a simple class that will help us print to the screen
# It has nothing to do with the joysticks, just outputing the
# information.
class TextPrint:
    def __init__(self):
        self.reset()
        self.font = pygame.font.Font(None, 20)

    def mprint(self, screen, textString):
        textBitmap = self.font.render(textString, True, BLACK)
        screen.blit(textBitmap, [self.x, self.y])
        self.y += self.line_height

    def reset(self):
        self.x = 10
        self.y = 10
        self.line_height = 15

    def indent(self):
        self.x += 10

    def unindent(self):
        self.x -= 10



def joystick_start_process():
    pygame.init()

    # Set the width and height of the screen [width,height]
    size = [500, 700]
    screen = pygame.display.set_mode(size)

    pygame.display.set_caption("My Game")

    #Loop until the user clicks the close button.
    done = False

    # Used to manage how fast the screen updates
    clock = pygame.time.Clock()

    # Initialize the joysticks
    pygame.joystick.init()

    # Get ready to print
    textPrint = TextPrint()

    # -------- Main Program Loop -----------
    while done==False:
        # EVENT PROCESSING STEP
        for event in pygame.event.get(): # User did something
            if event.type == pygame.QUIT: # If user clicked close
                done=True # Flag that we are done so we exit this loop

            # Possible joystick actions: JOYAXISMOTION JOYBALLMOTION JOYBUTTONDOWN JOYBUTTONUP JOYHATMOTION
            if event.type == pygame.JOYBUTTONDOWN:
                print("Joystick button pressed.")
            if event.type == pygame.JOYBUTTONUP:
                print("Joystick button released.")


        # DRAWING STEP
        # First, clear the screen to white. Don't put other drawing commands
        # above this, or they will be erased with this command.
        screen.fill(WHITE)
        textPrint.reset()

        # Get count of joysticks
        joystick_count = pygame.joystick.get_count()

        # textPrint.mprint(screen, "Number of joysticks: {}".format(joystick_count) )
        # textPrint.indent()

        # For each joystick:
        for i in range(joystick_count):
            joystick = pygame.joystick.Joystick(i)
            joystick.init()

            # textPrint.mprint(screen, "Joystick {}".format(i) )
            # textPrint.indent()
            #
            # # Get the name from the OS for the controller/joystick
            # name = joystick.get_name()
            # textPrint.mprint(screen, "Joystick name: {}".format(name) )

            # Usually axis run in pairs, up/down for one, and left/right for
            # the other.
            # axes = joystick.get_numaxes()
            # textPrint.mprint(screen, "Number of axes: {}".format(axes) )
            # textPrint.indent()

            # for i in range( axes ):
            #     axis = joystick.get_axis( i )
            #     textPrint.mprint(screen, "Axis {} value: {:>6.3f}".format(i, axis) )
            # textPrint.unindent()

            buttons = joystick.get_numbuttons()
            textPrint.mprint(screen, "Number of buttons: {}".format(buttons) )
            textPrint.indent()
            key = 's'
            for i in range( buttons ):
                button = joystick.get_button( i )
                if button == 1:

                    if i == 0:
                        key = 'x'
                    if i == 1:
                        key = 'd'
                    if i == 2:
                        key = 'a'
                    if i == 3:
                        key = 'w'
                    if i == 4:
                        key = 'l'
                    if i == 5:
                        key = 'r'

                textPrint.mprint(screen, "Button {:>2} value: {}".format(i,button) )
            textPrint.mprint(screen, "CMD: "+key )
            pub.publish(key)
            textPrint.unindent()


        # ALL CODE TO DRAW SHOULD GO ABOVE THIS COMMENT

        # # Go ahead and update the screen with what we've drawn.
        pygame.display.flip()
        #
        # # Limit to 20 frames per second
        clock.tick(20)

    # Close the window and quit.
    # If you forget this line, the program will 'hang'
    # on exit if running from IDLE.
    pygame.quit ()


if __name__=="__main__":

    joystick_start_process()
