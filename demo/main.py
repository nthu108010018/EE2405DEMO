import sensor, image, time, math, os, tf
import pyb
enable_lens_corr = False # turn on for straighter lines...
uart = pyb.UART(3,9600,timeout_char=1000)
uart.init(9600,bits=8,parity = None, stop=1, timeout_char=1000)
tmp = ""

sensor.reset()
sensor.set_pixformat(sensor.RGB565) # grayscale is faster
sensor.set_framesize(sensor.QQVGA)
sensor.skip_frames(time = 2000)
clock = time.clock()

# All lines also have `x1()`, `y1()`, `x2()`, and `y2()` methods to get their end-points
# and a `line()` method to get all the above as one 4 value tuple for `draw_line()`.

while(True):
   clock.tick()
   img = sensor.snapshot()
   if enable_lens_corr: img.lens_corr(1.8) # for 2.8mm lens...

   # `merge_distance` controls the merging of nearby lines. At 0 (the default), no
   # merging is done. At 1, any line 1 pixel away from another is merged... and so
   # on as you increase this value. You may wish to merge lines as line segment
   # detection produces a lot of line segment results.

   # `max_theta_diff` controls the maximum amount of rotation difference between
   # any two lines about to be merged. The default setting allows for 15 degrees.

   for l in img.find_line_segments(threshold = 1000, theta_margin = 0, rho_margin = 15, segment_threshold = 10):
     if(l.y1()<5):
        img.draw_line(l.line(), color = (255, 0, 0))
        turn_angle = 0
        turn_angle = l.theta()
        if(l.theta()>90) : turn_angle = l.theta()-180
        if(abs(turn_angle)<30):
          print("%f" % turn_angle)
          uart.write(('/line/run %f\r\n' % turn_angle).encode())
