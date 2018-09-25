#ifndef ZMQREQSOCKET_H
#define ZMQREQSOCKET_H

#include <string>
#include "LTZmq.h"
#include "Manager/managererrtype.h"

using namespace std;

#define ZMQ_REP_LIVENESS                5
#define ZMQ_POLL_TIMEOUT_MAX            3
#define ZMQ_POLL_INTERVAL               1000    //毫秒
#define ZMQ_RECONNECT_INTERVAL_INIT     1000    //毫秒
#define ZMQ_RECONNECT_INTERVAL_MAX      32000   //毫秒

class ZmqReqSocket
{
public:
    ZmqReqSocket(void *context);
    ~ZmqReqSocket();

    void ZmqSetIpPort(string ip_addr,int port);
    int ZmqSocketStart(void);
    int ZmqSocketStop(void);
    int Recv2String(void* s, std::string & title, std::string & content);
    int ZmqSendReq(const string &req_title, string req_content, string &rep_title, string &rep_content);

private:
    void *context_;
    void *req_socket_;
    char zmq_connect_str[256];
};

#endif // ZMQREQSOCKET_H
