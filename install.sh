#!/bin/bash

echo "Instalando dependencias ..."

sudo apt-get update


sudo apt-get install -y git cmake g++ make
sudo snap install cmake --classic
sudo apt-get install -y libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libsdl2-mixer-dev libsdl2-gfx-dev
sudo apt-get install -y qtbase5-dev qt5-qmake
sudo apt-get install -y libyaml-cpp-dev

# Clone the repository
git clone https://github.com/Hernandsv01/taller-grupal.git /home/vagrant/taller-grupal

# Compile the project
cd /home/vagrant/taller-grupal
cmake .
make

# Any additional installation steps for the game can go here

