#include <string>
#include <iostream>
#include "ZmqSubSocket.h"
#include "stdio.h"
#include "Manager/managererrtype.h"


ZmqSubSocket::ZmqSubSocket(void *context)
{
    context_ = context;
    sub_socket_ = NULL;
}

ZmqSubSocket::~ZmqSubSocket()
{

}

void *ZmqSubSocket::ZmqGetSocket(void)
{
    return sub_socket_;
}

void ZmqSubSocket::ZmqSetIpPort(string ip_addr,int port)
{
    snprintf(zmq_connect_str_,256,"tcp://%s:%d%c",(char *)ip_addr.data(),port,'\0');
}

int ZmqSubSocket::ZmqSetSubFilter(const char *filter)
{
    if(NULL == sub_socket_)
    {
       return -1;
    }

    return zmq_setsockopt(sub_socket_,ZMQ_SUBSCRIBE,filter,strlen(filter));
}

int ZmqSubSocket::ZmqSocketStart(void)
{
    sub_socket_ = zmq_socket(context_,ZMQ_SUB);
    if(NULL == sub_socket_)
    {
       return -1;
    }

    return zmq_connect(sub_socket_,zmq_connect_str_);
}

int ZmqSubSocket::ZmqSocketStop(void)
{
    if(NULL == sub_socket_)
    {
       return -1;
    }

    int ret = zmq_close(sub_socket_);
    sub_socket_ = NULL;

    return ret;
}

int ZmqSubSocket::ZmqSubRecv(vector<shared_ptr<string>> &title,
                               vector<shared_ptr<string>> &content,int &err)
{
    if(NULL == sub_socket_)
    {
       return MANAGER_NETWORK_SOCK_NULL;
    }

    zmq_pollitem_t sub_item = {sub_socket_,0,ZMQ_POLLIN,0};
    int ret = zmq_poll(&sub_item,1,0);
    if(ret > 0)
    {
        ret = LTUtilities::RecvAllMessages(sub_socket_,title,content,err);
        if(ret < 0)
        {
            ZmqSocketStop();
            return MANAGER_NETWORK_RECV_FAIL;
        }
    }

    return MANAGER_NO_ERR;
}

int ZmqSubSocket::ZmqSubRecv(vector<shared_ptr<string>> &title,
                               vector<shared_ptr<vector<ByteUnit>>> &content,int &err)
{
    if(NULL == sub_socket_)
    {
       return MANAGER_NETWORK_SOCK_NULL;
    }

    zmq_pollitem_t sub_item = {sub_socket_,0,ZMQ_POLLIN,0};
    int ret = zmq_poll(&sub_item,1,0);
    if(ret > 0)
    {
        ret = LTUtilities::RecvAllMessages(sub_socket_,title,content,err);
        if(ret < 0)
        {
            ZmqSocketStop();
            return MANAGER_NETWORK_RECV_FAIL;
        }
    }

    return MANAGER_NO_ERR;
}
