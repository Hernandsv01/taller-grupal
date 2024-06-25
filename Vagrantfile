# -*- mode: ruby -*-
# vi: set ft=ruby :


Vagrant.configure("2") do |config|
  
  config.vm.box = "ubuntu/focal64"

  config.vm.provider "virtualbox" do |vb|
    vb.gui = true
    vb.memory = "2048"
  end
  

  config.vm.provision "shell", inline: <<-SHELL
    sudo apt-get update
    sudo apt-get install -y ubuntu-desktop
    sudo usermod -a -G sudo vagrant
    sudo shutdown -r now
  SHELL

  config.vm.provision "shell", path: "install.sh"

end
