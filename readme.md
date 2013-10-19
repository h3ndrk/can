<h1 align="center">
  <img alt="Roboter CAN" src="http://nipe-systems.de/edit/upload/d6a2687a7db7a3885e53e664be03765b" />
</h1>

## What is Roboter CAN?

The "Roboter CAN" is a german project of a robot of [NIPE-SYSTEMS.de](http://www.nipe-systems.de). The objective is to build a robot which finds its way through a labyrinth. The robot should participate at the RoboCup German Open 2014. More Informations at [NIPE-SYSTEMS.de](http://www.nipe-systems.de)

## Why this directory structure?

In the repository you can find 4 directories:

* bin: In this directory you can find the source code of the website and the handler
* chip: Here you can find the source code of the mikrocontroller program
* root: This directory is for little scripts which are executed at boot-up of the robot
* scripts: In this directory are more scripts, useful to decrease often-made work

## Explanations of the code and the behavior

On [NIPE-SYSTEMS.de](http://www.nipe-systems.de) you will find several articles of the topic software of the robot. Take a look! :)

## Dependencies to build the code

In the *chip*-directory you need the following (linux-)packages:

* gcc
* avrlibc
* avrdude

For the handler code in the *bin*-directory you should install:

* gcc
* wiringpi from [source](https://projects.drogon.net/raspberry-pi/wiringpi/download-and-install/)
* cmake for wiringPi
* mmal image library for the raspberry camera module
* rsync

All of the code (except of the *chip*-directory) is optimized and developed for a [Raspberry Pi](http://www.raspberrypi.org). We're using Linux Arch on it. It's hard and not recommended to build this source on your computer.
