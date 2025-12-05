# 本软件提供若干网络api
`/` 根目录访问，发送`www/login/index.html`文件

## 1. 登录服务 POST `/api/login`
前端->后端`json`为：
```json
{
    "username": "xxx",
    "password": "xxx"
}
```
后端->前端`json`为：
```json
{
    "success": true, //bool值
    "message": "xxx",  //返回信息
    "redirectUrl": "/dashboard"  //optional，只有success为true时才会设置这个值
}
```
登陆后，会查询位于云服务器的本地数据库，如果查询结果不存在，将会返回`200`，提示用户名不存在。

取得查询结果后，将对密码进行比对，如果密码错误，仍返回`200`，提示用户密码错误。

如果密码比对成功，返回`200`, 并返回`www/main/main.html`页面

## 2. 视频流列表获取 GET `/api/streams/status`
**没有body**

**响应body**：
```json
{
    "success": true,
    "message": "xxx",
    "rtmp_list": [
        { "name": "camera1", "stream_id": "stream1" },
        { "name": "camera2", "stream_id": "stream2" }
    ]
}
```

## 3. 视频流注册 POST `/device/heartbeat`
**请求body**：
```json
{
    "device_name": "xxxx", //比如可以是中文，标注设备名字
    "stream_id": "stream123",  //自己取
    "param":{
        "exposure_time": 2000   //曝光等参数信息放到param里面，当前参数
    }
}
```

**响应body**
```json
{
    "success": true,
    "message": "xxx",
    "target_param":{
        "exposure_time": 3000  //期望的参数，有时target_param里面也会缺省，表示没有参数需要修改
    }
}
```