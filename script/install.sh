echo "Updating packages"
apt-get update
apt-get upgrade -y

echo "Downloading packages"
apt-get install -y gcc
apt-get install -y clang
apt-get install -y make
apt-get install -y graphviz
apt-get install -y python3

echo "Setting up project folders"
cd /vagrant
mkdir build
mkdir libs
mkdir output

echo "Installing clang"
cd /vagrant/libs
wget http://releases.llvm.org/3.9.1/clang+llvm-3.9.1-x86_64-linux-gnu-ubuntu-16.04.tar.xz
tar xf clang+llvm-3.9.1-x86_64-linux-gnu-ubuntu-16.04.tar.xz
mv clang+llvm-3.9.1-x86_64-linux-gnu-ubuntu-16.04 clang_llvm && rm clang+llvm-3.9.1-x86_64-linux-gnu-ubuntu-16.04.tar.xz

echo "Add tools to path"
echo 'export PATH=$PATH:/vagrant/libs/clang_llvm/bin' >> /home/ubuntu/.bashrc
source /home/ubuntu/.bashrc

echo "Compilation de l'outil clang" # Clang tools
cd /vagrant
make

echo "Installation finished"
echo "Run 'vagrant reload' to complete installation"