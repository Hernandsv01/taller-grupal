#!/bin/bash

echo "Instalando dependencias ..."

sudo apt-get update

#Instalo dependencias
sudo apt-get install -y git cmake g++ make
sudo apt-get purge cmake
sudo snap install cmake --classic
sudo apt-get install gnome-terminal
sudo apt-get install -y libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libsdl2-mixer-dev libsdl2-gfx-dev
sudo apt-get install -y qtbase5-dev qt5-qmake
sudo apt-get install -y libyaml-cpp-dev

#Clono el respositorio
git clone https://github.com/Hernandsv01/taller-grupal.git /home/vagrant/taller-grupal

#Compilacion 
cd /home/vagrant/taller-grupal
cmake .
make

#una terminal 
#gnome-terminal -- bash -c "./server; exec bash"

#otra therminal 
#gnome-terminal -- bash -c "./client; exec bash"

