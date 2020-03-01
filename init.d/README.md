# 启动脚本使用方法
1. 拷贝到init.d/下
sudo cp init.d/provision_device /etc/init.d/

2. 设置脚本可执行权限：
sudo chmod 777 /etc/init.d/provision_device

3. 最后将该脚本作为服务设置开机自动加载：
sudo update-rc.d provision_device defaults 