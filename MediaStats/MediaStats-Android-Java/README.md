# MediaStats-Android-Java

这个开源示例项目演示了如何快速集成 网易云信 新一代（G2）音视频 SDK，实现通话过程中数据统计分析功能。

## 环境准备，运行示例项目，一对一通话功能实现
1. 这个开源示例项目基于一对一视频通话，关于**环境准备**，**运行示例项目**，**功能实现**章节请参考[1对1音视频通话](https://github.com/netease-im/Basic-Video-Call/tree/master/One-to-One-Video/NERtcSample-1to1-Android-Java)

2. 将AppID 填写进 "app/src/main/res/values/strings.xml"

```
<!-- 替换为你自己的AppKey -->
<string name="app_key">YOUR APP KEY</string>
```

## 使用方式

如果需要使用质量透明的功能，需要通过`addEngineMediaStatsObserver:`主动设置相应的回调，取消调用`removeEngineMediaStatsObserver:` 即可。

## NERtcEngineMediaStatsObserver 接口预览

void	onLocalAudioStats(NERtcAudioSendStats stats)
本地音频流统计信息回调

void	onLocalVideoStats(NERtcVideoSendStats stats)
本地视频流统计信息回调

void	onNetworkQuality(NERtcNetworkQualityInfo[] statsArray)
通话中所有用户的网络状态回调

void	onRemoteAudioStats(NERtcAudioRecvStats[] statsArray)
通话中远端音频流的统计信息回调数组

void	onRemoteVideoStats(NERtcVideoRecvStats[] statsArray)
通话中远端视频流的统计信息回调数组

void	onRtcStats(NERtcStats stats)
当前通话统计回调

SDK 定期向 App 报告当前通话的统计信息，每 2 秒触发一次


