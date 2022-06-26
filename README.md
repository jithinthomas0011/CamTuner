# CamTuner
A user space application to tune images from video4linux2 devices (like ov426 , ISC )
User can modify the v4l2 devices with th available/supported device paths.
Install Package v4l2-utils //For debian enviroments
Check v4l2-ctl --list-devices to see drivers registed as v4l2 devices/subdevices.
Use v4l2-ctl -d /dev/videoX -l to list all supported Tuning parameters and modify the src file accordingly. 

Thanks!
