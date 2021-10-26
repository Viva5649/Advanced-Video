#include <iostream>
#include <QDebug>
#include "engine.h"
#ifndef _WIN32
#include "engine_beauty_mac.h"
#endif

Engine::Engine(QObject *parent /*= nullptr*/)
:QObject(parent)
{

}

Engine::~Engine()
{
    std::cout << "Engine::~Engine begin" << std::endl;
    uninit();
    std::cout << "Engine::~Engine" << std::endl;
}

bool Engine::Init(const char* log_dir_path)
{
    // 创建RTC引擎
    rtc_engine_ = dynamic_cast<nertc::IRtcEngineEx*>(createNERtcEngine());

    // rtcEngineContext信息初始化
    nertc::NERtcEngineContext engine_context;
    engine_context.app_key = app_key_.c_str();
    engine_context.log_dir_path = log_dir_path;
    engine_context.log_level = nertc::kNERtcLogLevelInfo;
    engine_context.log_file_max_size_KBytes = 1024 * 10;
    engine_context.event_handler = this;
    engine_context.video_use_exnternal_render = false;

#if 1
    // server config
    nertc::NERtcServerAddresses server_addr;
    memset(server_addr.channel_server, 0, kNERtcMaxURILength);
    memset(server_addr.statistics_server, 0, kNERtcMaxURILength);
    memset(server_addr.room_server, 0, kNERtcMaxURILength);
    memset(server_addr.compat_server, 0, kNERtcMaxURILength);
    memset(server_addr.nos_lbs_server, 0, kNERtcMaxURILength);
    memset(server_addr.nos_upload_sever, 0, kNERtcMaxURILength);
    memset(server_addr.nos_token_server, 0, kNERtcMaxURILength);
    server_addr.use_ipv6 = false;
    engine_context.server_config = server_addr;
#endif

    // 初始化RTC引擎，
    //在调用 createNERtcEngine() 方法创建 IRtcEngine 对象后，必须先调用该方法进行初始化，才能使用其他方法。

    int ret = rtc_engine_->initialize(engine_context);
    if (nertc::kNERtcNoError != ret)
    {
        std::cout << "ERROR: Failed to initialize NERtc Engine" << std::endl;
        return false;
    }

#ifdef _WIN32
    //美妆路径
    make_up_path_ = log_dir_path;
    make_up_path_ += "\\..\\..\\data\\beauty\\nebeauty\\StickerZipAndIcons\\makeup_sticker.bundle\\makeup\\template.json";

    //贴纸路径
    sticker_folder_path_ = log_dir_path;
    sticker_folder_path_ += "\\..\\..\\data\\beauty\\nebeauty\\StickerZipAndIcons\\2d_sticker.bundle\\";

    //滤镜路径
    image_filters_folder_path_ = log_dir_path;
    image_filters_folder_path_ += "\\..\\..\\data\\beauty\\nebeauty\\Filters\\filters.bundle\\";
#endif

    return true;
}

int Engine::EnableVideo(const bool &enabled)
{
    return rtc_engine_->enableLocalVideo(enabled);
}

int Engine::EnableAudio(const bool &enabled)
{
    return rtc_engine_->enableLocalAudio(enabled);
}

int Engine::JoinChannel(const std::string &channel_name, const std::string &uid)
{
    return rtc_engine_->joinChannel("", channel_name.c_str(), std::stoull(uid));
}

int Engine::SetupLocalVideo(void* hwnd)
{
    nertc::NERtcVideoCanvas canvas;
    canvas.cb = nullptr;
    canvas.user_data = nullptr;
    canvas.window = hwnd;
    canvas.scaling_mode = nertc::kNERtcVideoScaleFullFill;

    int ret = rtc_engine_->setupLocalVideoCanvas(&canvas);
    if (ret)
    {
        std::cout << "ERROR: Can not setup local video canvas! ERROR CODE:" << ret << std::endl;
    }

    return ret;
}

int Engine::SetupRemoteVideo(quint64 uid, void* hwnd)
{
    nertc::NERtcVideoCanvas canvas;
    canvas.cb = nullptr;
    canvas.user_data = nullptr;
    canvas.window = hwnd;
    canvas.scaling_mode = nertc::kNERtcVideoScaleFullFill;

    int ret = rtc_engine_->setupRemoteVideoCanvas(uid, &canvas);
    if (ret)
    {
        std::cout << "NERtcEngine::setupRemoteVideo: ret: " << ret << std::endl;
    }

    return ret;
}

int Engine::SubscribeRemoteVideoStream(nertc::uid_t uid, nertc::NERtcRemoteVideoStreamType type, bool subscribe)
{
    int ret = rtc_engine_->subscribeRemoteVideoStream(uid, type, subscribe);
    if (ret)
    {
        std::cout << "NERtcEngine::SubscribeRemoteVideoStream: ret: " << ret << std::endl;
    }

    return ret;
}

int Engine::SetParameters(const char* parameters)
{
    int ret = rtc_engine_->setParameters(parameters);
    if (ret)
    {
        std::cout << "NERtcEngine::SetParameters: ret: " << ret << std::endl;
    }

    return ret;
}

#ifdef _WIN32

int Engine::StartBeauty()
{
    if (rtc_engine_)
    {
        return rtc_engine_->startBeauty();
    }
    return -1;
}

void Engine::StopBeauty()
{
    if (rtc_engine_)
    {
        rtc_engine_->stopBeauty();
    }
}

void Engine::EnableNertcBeauty(const bool &enable)
{
    if (rtc_engine_)
    {
        rtc_engine_->enableBeauty(enable);
    }
}

void Engine::EnableNertcMirror(const bool &enable)
{
    if (rtc_engine_)
    {
        rtc_engine_->enableBeautyMirrorMode(enable);
    }
}

int Engine::EnableNertcMakeup(const bool &enable)
{
    if (rtc_engine_)
    {
        if (false == enable)
        {
            return rtc_engine_->removeBeautyMakeup();
        }
        else
        {
            return rtc_engine_->addBeautyMakeup(make_up_path_.c_str());
        }
    }

    return -1;
}

int Engine::SelectBeautySticker(const std::string &bundle_name)
{
    if (rtc_engine_)
    {
        if (0 != strcmp("none", bundle_name.c_str()))
        {
            std::string file_path = sticker_folder_path_ + bundle_name;
            int ret = rtc_engine_->addBeautySticker(file_path.c_str());
            return ret;
        }
        else
        {
            int ret = rtc_engine_->removeBeautySticker();
            return ret;
        }
    }

    return -1;
}

int Engine::SelectBeautyFilter(const std::string &filter_path, const int &val)
{
    if (rtc_engine_)
    {
        if (0 != strcmp("none", filter_path.c_str()))
        {
            std::string file_path = image_filters_folder_path_ + filter_path;
            int ret = rtc_engine_->addBeautyFilter(file_path.c_str());
            ret = rtc_engine_->setBeautyFilterLevel(val / 100.f);
            return ret;
        }
        else
        {
            int ret = rtc_engine_->removeBeautyFilter();
            return ret;
        }
    }
    return -1;
}

int Engine::SetBeautyEffect(const int &type, float level)
{
    if (rtc_engine_)
    {
        return rtc_engine_->setBeautyEffect((nertc::NERtcBeautyEffectType)type, level);
    }
    return -1;
}

#else
int Engine::StartBeauty()
{
    if (rtc_engine_)
    {
        return engine_StartBeauty();
    }
    return -1;
}

void Engine::StopBeauty()
{
    if (rtc_engine_)
    {
        engine_StopBeauty();
    }
}

void Engine::EnableNertcBeauty(const bool &enable)
{
    if (rtc_engine_)
    {
        engine_EnableNertcBeauty(enable);
    }
}

void Engine::EnableNertcMirror(const bool &enable)
{
    if (rtc_engine_)
    {
        engine_EnableNertcMirror(enable);
    }
}

int Engine::EnableNertcMakeup(const bool &enable)
{
    if (rtc_engine_)
    {
        return engine_EnableNertcMakeup(enable);
    }

    return -1;
}

int Engine::SelectBeautySticker(const std::string &bundle_name)
{
    if (rtc_engine_)
    {
        return engine_SelectBeautySticker(bundle_name);
    }

    return -1;
}

int Engine::SelectBeautyFilter(const std::string &filter_path, const int &val)
{
    if (rtc_engine_)
    {
        return engine_SelectBeautyFilter(filter_path, val);
    }
    return -1;
}

int Engine::SetBeautyEffect(const int &type, float level)
{
    if (rtc_engine_)
    {
        return engine_SetBeautyEffect(type, level);
    }
    return -1;
}

#endif

void Engine::uninit()
{
    if (rtc_engine_)
    {

#ifdef _WIN32
        rtc_engine_->stopBeauty();
#endif
        int ret = rtc_engine_->enableLocalAudio(false);
        ret = rtc_engine_->enableLocalVideo(false);
        ret = rtc_engine_->leaveChannel();

        rtc_engine_->release(true);
        destroyNERtcEngine((void*&)rtc_engine_);
        rtc_engine_ = nullptr;
    }
}


void Engine::onJoinChannel(nertc::channel_id_t   cid,
                           nertc::uid_t          uid, 
                           nertc::NERtcErrorCode result, 
                           uint64_t              elapsed)
{
    qDebug() << "Engine::onJoinChannel";
    Q_EMIT sigJoinChannel(result);

}

void Engine::onUserJoined(nertc::uid_t uid,
                          const char * user_name)
{
    qDebug() << "Engine::onUserJoined";
    Q_EMIT sigUserJoined(uid, user_name);
}

void Engine::onUserVideoStart(nertc::uid_t                 uid, 
                              nertc::NERtcVideoProfileType max_profile)
{
    qDebug() << "Engine::onUserVideoStart";
    Q_EMIT sigUserVideoStart(uid, max_profile);
}

void Engine::onLeaveChannel(nertc::NERtcErrorCode result)
{
    qDebug() << "Engine::onLeaveChannel";
}

void Engine::onUserLeft(nertc::uid_t                   uid, 
                        nertc::NERtcSessionLeaveReason reason)
{
    qDebug() << "Engine::onUserLeft";
    Q_EMIT sigUserLeft(uid, reason);
}
