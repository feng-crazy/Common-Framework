#include "ZmqReqSocket.h"
#include <string>
#include <stdio.h>
#include <iostream>


ZmqReqSocket::ZmqReqSocket(void *context)
{
    context_ = context;
    req_socket_ = NULL;
}

ZmqReqSocket::~ZmqReqSocket()
{

}

void ZmqReqSocket::ZmqSetIpPort(string ip_addr,int port)
{
    sprintf(zmq_connect_str,"tcp://%s:%d%c",(char *)ip_addr.data(),port,'\0');
}

int ZmqReqSocket::ZmqSocketStart(void)
{
    req_socket_ = zmq_socket(context_,ZMQ_REQ);
    if(NULL == req_socket_)
    {
       return MANAGER_NETWORK_SOCK_NULL;
    }
    int ret = zmq_connect(req_socket_,zmq_connect_str);
    if(ret < 0)
    {
        return MANAGER_NETWORK_CONNECT_FAIL;
    }
    return MANAGER_NO_ERR;
}

int ZmqReqSocket::ZmqSocketStop(void)
{
    if(NULL == req_socket_)
    {
       return MANAGER_NETWORK_SOCK_NULL;
    }

    int ret = zmq_close(req_socket_);
    req_socket_ = NULL;

    return ret;
}

int ZmqReqSocket::Recv2String(void* s, std::string & title, std::string & content)
{
        zmq_msg_t msg;
        int more = 0, size = 0;
        size_t moreSize = sizeof(more);
        title.clear();
        content.clear();

        //first frame: title
        zmq_msg_init(&msg);
        size = zmq_msg_recv(&msg, s, 0);

        if (size < 0) {
                int errNum = zmq_errno();
                cout << "err num = " << errNum << endl;
                if (errNum == EAGAIN) {
                        zmq_msg_close(&msg);
                        return 0;
                }
                else {
                        zmq_msg_close(&msg);
                        return -1;
                }
        }
        else if(size == 0)
        {
            zmq_msg_close(&msg);
            return 0;
        }

        title.insert(0, (const char*)zmq_msg_data(&msg), size);

        zmq_getsockopt(s, ZMQ_RCVMORE, &more, &moreSize);
        if (!more) {
                zmq_msg_close(&msg);
                return -1;
        }
        zmq_msg_close(&msg);

        //2rd frame: real data
        zmq_msg_init(&msg);
        size = zmq_msg_recv(&msg, s, ZMQ_DONTWAIT);
        if (size < 0) {
                zmq_msg_close(&msg);
                return size;
        }

        std::stringstream recv_stream;
        recv_stream.write((const char *)zmq_msg_data(&msg),zmq_msg_size(&msg));
        content = recv_stream.str();

        zmq_msg_close(&msg);
        return size;
}

int ZmqReqSocket::ZmqSendReq(const string &req_title,string req_content,
                               string &rep_title,string &rep_content)
{
    if(NULL == req_socket_)
    {
        return MANAGER_NETWORK_SOCK_NULL;
    }

    size_t size;
    size_t liveness = ZMQ_REP_LIVENESS;
    size_t timeout = ZMQ_POLL_TIMEOUT_MAX;
    size_t interval = ZMQ_RECONNECT_INTERVAL_INIT;

    while(timeout > 0)
    {
        size = LTUtilities::Send2Strings(req_socket_,req_title,req_content);
        if(size < 0)
        {
            return MANAGER_NETWORK_SEND_FAIL;
        }

        while(true)
        {
            zmq_pollitem_t rep_item = {req_socket_,0,ZMQ_POLLIN,0};
            size = zmq_poll(&rep_item,1,ZMQ_POLL_INTERVAL);
            if(size > 0)
            {
                /*接收到REP消息*/
                size = LTUtilities::Recv2String(req_socket_,rep_title,rep_content);
                if(size < 0)
                {
                    return MANAGER_NETWORK_RECV_FAIL;
                }

                timeout = 0;
                break;
            }
            else if(0 == (--liveness))  /*接收REP消息超时，活跃度-1*/
            {
                cout << "liveness = 0" << endl;
                /*活跃度递减到0，关闭原有socket，打开新socket重连*/
                Sleep(interval);
                if(interval < ZMQ_RECONNECT_INTERVAL_MAX)
                {
                    interval *= 2;
                }
                ZmqSocketStop();
                ZmqSocketStart();
                liveness = ZMQ_REP_LIVENESS;
                timeout --;
                if(0 == timeout)
                {
                    cout << "network err" << endl;
                    return MANAGER_NETWORK_ERR;    /*网络异常，发出警告*/
                }
                break;
            }
        }
    }

    return MANAGER_NO_ERR;
}
