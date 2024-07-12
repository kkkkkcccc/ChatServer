#include "StatusGrpcClient.h"
//获取聊天服务器
GetChatServerRsp StatusGrpcClient::GetChatServer(int uid)
{
    ClientContext context;  //上下文
    GetChatServerRsp reply; //回包
    GetChatServerReq request;   //请求
    request.set_uid(uid);
    auto stub = pool_->getConnection(); //获取线程
    Status status = stub->GetChatServer(&context, request, &reply); //获取聊天服务并将回复返回
    Defer defer([&stub, this]() {
        pool_->returnConnection(std::move(stub));   //return前将线程返回
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

//构造函数
StatusGrpcClient::StatusGrpcClient()
{
    auto& gCfgMgr = ConfigMgr::Inst();
    std::string host = gCfgMgr["StatusServer"]["Host"]; //从ini文件中提取配置
    std::string port = gCfgMgr["StatusServer"]["Port"];
    pool_.reset(new StatusConPool(5, host, port));  //初始化5个线程
}