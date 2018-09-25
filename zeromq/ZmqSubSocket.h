#ifndef ZMQSUBSOCKET_H
#define ZMQSUBSOCKET_H

#include <string>
#include "LTZmq.h"


using namespace std;

class ZmqSubSocket
{
public:
    ZmqSubSocket(void *context);
    ~ZmqSubSocket();

    void *ZmqGetSocket(void);
    void ZmqSetIpPort(string ip_addr,int port);
    int ZmqSetSubFilter(const char *filter);
    int ZmqSocketStart(void);
    int ZmqSocketStop(void);
    int ZmqSubRecv(vector<shared_ptr<string>> &title,
                                   vector<shared_ptr<string> > &content, int &err);
    int ZmqSubRecv(vector<shared_ptr<string>> &title,
                                   vector<shared_ptr<vector<ByteUnit>>> &content,int &err);
private:
    void *context_;
    void *sub_socket_;
    char zmq_connect_str_[256];
};

#endif // ZMQSUBSOCKET_H
