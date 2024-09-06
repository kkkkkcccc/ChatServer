#include "ChatGrpcClient.h"

#include"RedisMgr.h"
#include"ConfigMgr.h"
#include"UserMgr.h"
#include"CSession.h"
#include"MySqlMgr.h"

ChatGrpcClient::ChatGrpcClient()
{
    auto& cfg = ConfigMgr::Inst();
    auto server_list = cfg["PeerServer"]["Servers"];    //��ȡini�ļ��еķ���������

    std::vector<std::string> words;

    std::stringstream ss(server_list);
    std::string word;

    while (std::getline(ss, word, ',')) {   //���ݣ���ȡ�ؼ���
        words.push_back(word);
    }

    for (auto& word : words) {
        if (cfg[word]["Name"].empty()) {    //��ȡ����Name��Ա�ķ������ؼ���
            continue;
        }
        _pools[cfg[word]["Name"]] = std::make_unique<ChatConPool>(5, cfg[word]["Host"], cfg[word]["Port"]);     //��ȡ��ַ�Ͷ˿ںţ��������������ӳ�
    }

}

//֪ͨ��Ӻ���
AddFriendRsp ChatGrpcClient::NotifyAddFriend(std::string server_ip, const AddFriendReq& req) {
    AddFriendRsp rsp;
    return rsp;
}

//֪ͨ��֤����
AuthFriendRsp ChatGrpcClient::NotifyAuthFriend(std::string server_ip, const AuthFriendReq& req) {
    AuthFriendRsp rsp;
    return rsp;
}

//��ȡ��Ϣ
bool ChatGrpcClient::GetBaseInfo(std::string base_key, int uid, std::shared_ptr<UserInfo>& userinfo) {
    return true;
}

//֪ͨ�����ı�
TextChatMsgRsp ChatGrpcClient::NotifyTextChatMsg(std::string server_ip,
    const TextChatMsgReq& req, const Json::Value& rtvalue) {

    TextChatMsgRsp rsp;
    return rsp;
}
