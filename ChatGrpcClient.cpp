#include "ChatGrpcClient.h"

#include"RedisMgr.h"
#include"ConfigMgr.h"
#include"UserMgr.h"
#include"CSession.h"
#include"MySqlMgr.h"

ChatGrpcClient::ChatGrpcClient()
{
    auto& cfg = ConfigMgr::Inst();
    auto server_list = cfg["PeerServer"]["Servers"];    //提取ini文件中的服务器配置

    std::vector<std::string> words;

    std::stringstream ss(server_list);
    std::string word;

    while (std::getline(ss, word, ',')) {   //根据，提取关键字
        words.push_back(word);
    }

    for (auto& word : words) {
        if (cfg[word]["Name"].empty()) {    //提取存在Name成员的服务器关键字
            continue;
        }
        _pools[cfg[word]["Name"]] = std::make_unique<ChatConPool>(5, cfg[word]["Host"], cfg[word]["Port"]);     //提取地址和端口号，创建服务器连接池
    }

}

//通知添加好友
AddFriendRsp ChatGrpcClient::NotifyAddFriend(std::string server_ip, const AddFriendReq& req) {
    AddFriendRsp rsp;
    return rsp;
}

//通知认证好友
AuthFriendRsp ChatGrpcClient::NotifyAuthFriend(std::string server_ip, const AuthFriendReq& req) {
    AuthFriendRsp rsp;
    return rsp;
}

//获取信息
bool ChatGrpcClient::GetBaseInfo(std::string base_key, int uid, std::shared_ptr<UserInfo>& userinfo) {
    return true;
}

//通知聊天文本
TextChatMsgRsp ChatGrpcClient::NotifyTextChatMsg(std::string server_ip,
    const TextChatMsgReq& req, const Json::Value& rtvalue) {

    TextChatMsgRsp rsp;
    return rsp;
}
