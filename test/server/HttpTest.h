#ifndef __HttpTest_H__
#define __HttpTest_H__

#include "kmapi.h"
#include "TestLoop.h"

#include <map>
#include <string>

using namespace kuma;

class HttpTest : public TestObject
{
public:
    HttpTest(ObjectManager* obj_mgr, long conn_id, const char* ver);

    KMError attachFd(SOCKET_FD fd, uint32_t ssl_flags, void *init, size_t len);
    KMError attachSocket(TcpSocket&& tcp, HttpParser&& parser, void *init, size_t len);
    KMError attachStream(H2Connection* conn, uint32_t streamId);
    int close();
    
    void onSend(KMError err);
    void onClose(KMError err);
    
    void onHttpData(void*, size_t);
    void onHeaderComplete();
    void onRequestComplete();
    void onResponseComplete();
    
private:
    enum class State {
        NONE,
        SENDING_FILE,
        SENDING_TEST_DATA,
        COMPLETED
    };
    void setupCallbacks();
    void cleanup();
    void sendTestFile();
    void sendTestData();
    void sendNormal();
    
private:
    ObjectManager*  obj_mgr_;
    HttpResponse    http_;
    long            conn_id_;
    State           state_ = State::NONE;
    bool            is_options_ = false;
    size_t          total_bytes_read_ = 0;
    std::string     file_name_;
};

#endif
