VAGRANT_FILE_API_VERSION = '1.0.0'

Vagrant.configure(2) do |config|
  config.vm.box = "ubuntu/xenial64"
  config.vm.hostname = "clang"

  config.vm.provider "virtualbox" do |v|
    v.customize ["setextradata", :id, "VBoxInternal2/SharedFoldersEnableSymlinksCreate/v-root", "1"]
    v.name = "clang"
    v.memory = 1024
    v.cpus = 2
  end

  config.vm.provision 'shell', path: 'script/install.sh'
end
