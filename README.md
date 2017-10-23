Maintainers: [Abhinav Arora](https://github.com/Abhi10arora)

#IMU

Inertial Measurement Unit is a device which tells us about the motion of the body it is attached to. Basically, an IMU consists of an accelerometer, magnetometer and gyroscope. Each one of them gives some data which is used to study the motion of the body and to localize its position.

To know more about the working of the IMU a good place to read up on the basics and the math involved can be understood [here]( http://www.starlino.com/imu_guide.html
). On the mentioned link you would also be able to find a simple algorithm which can be used to reduce the bias errors from the measurements we get from the IMU. [Here](https://www.cl.cam.ac.uk/techreports/UCAM-CL-TR-696.pdf) is a report which talks about different kinds of errors and noises which one encounter while taking and processing the data from an IMU.

#Filters for data processing

Filters are data algorithms used to process the data and remove the errors that accompany with any data. Here we have to calculate linear and angular displacements of the body attached to the IMU, but we have linear acceleration, angular velocity and the magnetometer readings from the IMU. So, we have to integrate the data to get the results. But, we have to twice integrate the values of linear accelerations to get the displacements, which will produce a lot of error in the results. So, some standard filters are used to get displacements.

One of them is [EKF](https://en.wikipedia.org/wiki/Extended_Kalman_filter). There is a standard ROS package [Robot_pose_Ekf](http://wiki.ros.org/robot_pose_ekf) which can be used to get the output directly from the input of the sensors. Other popular algorithm is [AHRS](http://x-io.co.uk/open-source-imu-and-ahrs-algorithms/). It applies **DCM** filter on [quateranion](http://math.ucr.edu/~huerta/introquaternions.pdf) form of data.

#PIXHAWK

These days IMU's come with the above mentioned filters in their firmwares. The one we are using here is [PIXHAWK](https://pixhawk.org). It is a flight controller which has many sensors inside it. With the help from some other devices, it is used in executing and studying flight motions. There are many algorithms that runs in it which processes data outputs from various sensors and use the processed data for various motions of the flight. Similarly, it has an IMU as well. It applies **EKF** and **AHRS** algorithms on the IMU's data and gives the output in form of **quateranions** to us.
