#pragma once
#include "const.h"
#include "Singleton.h"
#include "ConfigMgr.h"
#include <grpcpp/grpcpp.h>	//grpcԴ�ļ�
#include "message.grpc.pb.h"	//���ɵ�ͷ�ļ�
#include "message.pb.h"
#include <queue>
#include "data.h"
#include <json/json.h>	//json����
#include <json/value.h>
#include <json/reader.h>

using grpc::Channel;
using grpc::Status;
using grpc::ClientContext;

using message::AddFriendReq;
using message::AddFriendRsp;

using message::AuthFriendReq;
using message::AuthFriendRsp;

using message::GetChatServerRsp;
using message::LoginRsp;
using message::LoginReq;
using message::ChatService;

using message::TextChatMsgReq;
using message::TextChatMsgRsp;
using message::TextChatData;

class ChatConPool {
public:
	ChatConPool(size_t poolSize, std::string host, std::string port) :	//��С����ַ���˿�
		poolSize_(poolSize), host_(host), port_(port), b_stop_(false) {
		for (size_t i = 0; i < poolSize_; i++) {
			std::shared_ptr<Channel> channel = grpc::CreateChannel(host + ":" + port, grpc::InsecureChannelCredentials());	//����ͨ��
			connections_.push(ChatService::NewStub(channel));	//ʹ����ʹSTub����ͨ��
		}
	}

	~ChatConPool() {
		std::lock_guard<std::mutex> lock(mutex_);
		Close();
		while (!connections_.empty()) {
			connections_.pop();
		}
	}

	std::unique_ptr<ChatService::Stub> getConnection() {
		std::unique_lock<std::mutex> lock(mutex_);
		cond_.wait(lock, [this] {
			if (b_stop_) {
				return true;
			}
			return !connections_.empty();	//����ֹ�����ռ������¿ɹ���ִ��
			});
		//���ֱֹͣ�ӷ��ؿ�ָ��
			if (b_stop_) {
				return nullptr;
			}
			auto context = std::move(connections_.front());	//ֱ�ӻ�ȡ��һ������ʵ���ڴ�
			connections_.pop();	//�����ź�ȡ����һ������
			return context;	//��������
	}

	void returnConnection(std::unique_ptr<ChatService::Stub> context) {
		std::lock_guard<std::mutex> lock(mutex_);
		if (b_stop_) {
			return;
		}
		connections_.push(std::move(context));	//Ψһ����ָ�������ֵ���÷�ʽ�ƶ�����
		cond_.notify_one();
	}

	void Close() {
		b_stop_ = true;
		cond_.notify_all();
	}


private:
	atomic<bool> b_stop_;
	size_t poolSize_;
	std::string host_;
	std::string port_;
	std::queue<std::unique_ptr<ChatService::Stub>> connections_;	//unique_ptr��֤����Ψһ�ԣ������ӱ�pop��ȥ�ܱ�֤��ptr��ʱ�ͷ�
	std::mutex mutex_;
	std::condition_variable cond_;

};
  
class ChatGrpcClient:public Singleton<ChatGrpcClient>	//����׶ζ�ģ���ⲻ�ϸ�������ʱʵ�ʹ���ChatGrpcClient����ģ��
{
	friend class Singleton < ChatGrpcClient >;
public:
	~ChatGrpcClient() {

	}

	AddFriendRsp NotifyAddFriend(std::string server_ip, const AddFriendReq& req);
	AuthFriendRsp NotifyAuthFriend(std::string server_ip, const AuthFriendReq& req);
	bool GetBaseInfo(std::string base_key, int uid, std::shared_ptr<UserInfo>& userinfo);
	TextChatMsgRsp NotifyTextChatMsg(std::string server_ip, const TextChatMsgReq& req, const Json::Value& rtvalue);
private:
	ChatGrpcClient();
	unordered_map<std::string, std::unique_ptr<ChatConPool>> _pools;

};

