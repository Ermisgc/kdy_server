# 如何配置项目
## 环境
后端所需的环境包括：
- `nginx`，伟大无需多盐，用于转发请求，因为一般而言服务器不能暴露过多端口，比如暴露`3306`端口，会有用户直接访问`Mysql`注入的风险，因此服务器一般只开若干必要的端口，比如`80`，然后由`nginx`做转发。`nginx`的配置文件在本目录下的`nginx.conf`上，目前只写了简单的请求转发功能。由于本软件存在`nginx-http-flv-module`，因此还需要下载该`rtmp`模块，安装方法为：
```bash
# 如果没有安装nginx的话，先编译nginx:
wget http://nginx.org/download/nginx-1.24.0.tar.gz  # 版本随便，我用的nginx 1.24.0
tar -zxf nginx-1.24.0.tar.gz
git clone https://github.com/winshining/nginx-http-flv-module.git  # 下载nginx-rtmp-module源码

cd nginx-1.24.0
./configure --prefix=/usr/local/nginx \
            --with-http_ssl_module \
            --with-http_stub_status_module \
            --with-http_realip_module \
            --add-module=../nginx-http-flv-module
make && make install

# 安装好了nginx之后将配置文件拷贝过去，然后利用配置文件启动nginx
cp nginx.conf /usr/local/nginx/conf/
nginx -c /usr/local/nginx/conf/nginx.conf
```

- `boost`库，这个库是muduo网络库等一系列网络库的基础库，安装方式为：
```bash
sudo yum install boost-devel  # Centos 或者 OpenCloudOS
sudo apt install boost-devel  # Ubuntu 
``` 

- `muduo`网络库，后端网络框架基础库，安装方式为：
```bash
git clone https://github.com/chenshuo/muduo.git
cd muduo
./build.sh install # 这里记得要给执行权限:chmod 777 build.sh

cd ../build/release-install-cpp11/
cp -r include /usr/local/  # 把muduo头文件放到local目录下，如果local目录下没有的话
cp -r lib /usr/local/  # 把muduo生成的静态库文件放到local目录下，如果local目录下没有的话
```
- `nghttp2`库(<b>尚未集成</b>)，用于提供http2协议解析，这里要注意http2是比较重要的，因为它基于https开发，有加密功能，后续开发将集成http2，前期开发暂时采用http1.1

- `MySQL`，用于存储视频元数据和用户登录信息。视频文件将直接存储于文件系统，但视频的元数据（时间、时长、存储位置）存于MySQL上。此外，用户信息、重要的日志信息也存于`MySQL`上。`MySQL`的安装教程见：https://cloud.tencent.com/developer/article/2533258

- `libmysqlcppconn-dev`，MySQL官方提供的C++连接类库，这里采用MySQL官网下载的方式：
```bash
wget https://repo.mysql.com/RPM-GPG-KEY-mysql-2023  # 导入GPG密钥
sudo rpm --import RPM-GPG-KEY-mysql-2023

sudo yum install mysql-connector-c++-devel  # Ubuntu改为apt-get install
```

- `paho-mqtt`，Eclipse的MQTT库，这里选择编译安装。


## 编译
配置好了之后可以直接尝试`./autobuild.sh`编译，然后`./bin/test_server`运行

# 文件编排
- `bin`，生成的可以执行文件目录
- `lib`，生成的库文件目录
- `include`，包含文件目录
- `src`，源文件目录
- `www`，前端网页的地址
- `design`，部分尚未实现功能的设计图

提交时注意提交到自己的分支，然后由管理员一起合并。

# 主要工作安排和进展
## 前端
- [x] Login的前端页面，HTML、CSS和JS
- [ ] 各个前端页面
- [ ] 前端的状态信息显示、测点信息等
- [ ] 前端的视频流，用js的MSE
- [ ] 前端的相机控制和服务器控制功能实现

## 后端
- [x] 端口转发和基本服务器框架
- [ ] 后端WebSocket协议实现，用于主动推送视频
- [ ] MQTT客户端集成，获取视频、调整摄像头参数
- [ ] 视频文件暂定于H.264编码，利用WebSocket推送，调用cv的函数即可
- [ ] Login API，注册功能实现，需要和数据库打交道
- [ ] MySQL功能集成，用HTTP调用Restful API直接访问MySQL
- [ ] 前端API的实现