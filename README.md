# dvs_cpp_dataLoader 

## Description

Event camera or DVS/DAVIS has a different type of data.

UZH has developed DVS renderer such as rpg_dvs_ros (https://github.com/uzh-rpg/rpg_dvs_ros), but in a ROS environment (and use ROS .bag files).

This dvs_cpp_dataLoader written by C++, without need for a ROS environment. 

So far, this code is only used for load data from a DVS/DAVIS dataset. Such as [DAVIS 240C Datasets](http://rpg.ifi.uzh.ch/davis_data.html). Download **.zip** instead of .bag.

## Features

1. Without need for a ROS environment;
2. Use multiple threads, to load the events data as well as images in two separate streams, and you can choose only event stream.
3. Load speed is controllable, since the process may be slower than real time, a *timeScale* can be changed to slow down the data stream.

## Usage

1. Download this codes, then compile.

2. Download dataset from [http://rpg.ifi.uzh.ch/davis_data.html](http://rpg.ifi.uzh.ch/davis_data.html), and extract.

3. Run **demo** with dataset path:

   `$ ./demo  /path/to/the/dataset/folder

## Futher work

1. Will use one thread to generate 2 streams together with a synchronized system time.
2. Use buffer to load a group of data in advance.

## Contact

E-mail: jfjxxdy2007@126.com