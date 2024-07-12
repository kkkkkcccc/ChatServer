#include "StatusGrpcClient.h"
//��ȡ���������
GetChatServerRsp StatusGrpcClient::GetChatServer(int uid)
{
    ClientContext context;  //������
    GetChatServerRsp reply; //�ذ�
    GetChatServerReq request;   //����
    request.set_uid(uid);
    auto stub = pool_->getConnection(); //��ȡ�߳�
    Status status = stub->GetChatServer(&context, request, &reply); //��ȡ������񲢽��ظ�����
    Defer defer([&stub, this]() {
        pool_->returnConnection(std::move(stub));   //returnǰ���̷߳���
        });
    if (status.ok()) {
        return reply;
    }
    else {
        reply.set_error(ErrorCodes::RPCFailed);
        return reply;
    }
}

LoginRsp StatusGrpcClient::Login(int uid, std::string token)
{
    ClientContext context;
    LoginRsp reply;
    LoginReq request;
    request.set_uid(uid);
    request.set_token(token);

    auto stub = pool_->getConnection();
    Status status = stub->Login(&context, request, &reply);
    Defer defer([&stub, this]() {
        pool_->returnConnection(std::move(stub));
        });
    if (status.ok()) {
        return reply;
    }
    else {
        reply.set_error(ErrorCodes::RPCFailed);
        return reply;
    }
}

//���캯��
StatusGrpcClient::StatusGrpcClient()
{
    auto& gCfgMgr = ConfigMgr::Inst();
    std::string host = gCfgMgr["StatusServer"]["Host"]; //��ini�ļ�����ȡ����
    std::string port = gCfgMgr["StatusServer"]["Port"];
    pool_.reset(new StatusConPool(5, host, port));  //��ʼ��5���߳�
}