# `rtmp`推流和拉流协定
## benchmark
**测试1**: 定点测试，仅测试的rtmp的推拉流，页面为：`http://122.51.125.235/test/test_rtmp_1.html`

**测试2**：测试从设备列表获取设备rtmp地址再播放，页面为：`http://122.51.125.235/test/test_rtmp_2.html`

## 如何向服务器推流？
对于**测试1**：推流目标地址为：`rtmp://122.51.125.235:1935/live/stream123`

对于**测试2**：仍直接向云服务器发起HTTP，请求为url为`http://122.51.125.235/device/heartbeat`：`POST`，`body`为：
```json
{
    "device_name": "xxxx", //比如可以是中文，标注设备名字
    "stream_id": "stream123",  //自己取
    "param":{
        "exposure_time": 2000   //曝光等参数信息放到param里面
    }
}
```
返回的是具体的指令消息：
```json
{
    "success": true,
    "message": "xxx",
    "target_param":{
        "exposure_time": 3000  //期望的参数，有时target_param里面也会缺省，表示没有参数需要修改
    }
}
```
这里`stream_id`需要与推流地址对应，推流时推到：`rtmp://122.51.125.235:1935/live/${stream_id}`，前端拉流时会从`http://122.51.125.235/live?app=live&stream=${stream_id}`中获取流。