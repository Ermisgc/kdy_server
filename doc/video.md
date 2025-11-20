# video 接收端
本服务器需要部署一个video接收端，接收来自于云端的`rtmp`流，然后提供http接口，供前端网页获取并访问`rtmp`流。这里采用的技术方案是：`nginx-rtmp-module` + `ffmpeg` + `flv.js`的方案

- `nginx-rtmp-module`: 主要做`rtmp`流的接收和上发，它的目的有两个：
    - 将传给服务器的`rtmp`请求转发给服务进程
    - 提供`HTTP-FLV`流的输出，供前端网页通过HTTP协议获取流

- `ffmpeg`: 视频数据如果需要存储，需要在服务器本地执行数据的转化，这里采用ffmpeg进行流转换
- `flv.js`: B站的开源播放器组件，用于在网页端播放视频

