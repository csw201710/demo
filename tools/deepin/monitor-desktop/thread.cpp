#include "thread.h"
#include "utils.h"

Thread::Thread()
{

}

void Thread::run()
{
    while (1) {
        unsigned long long int prevRecv = 0, prevSend = 0;
        unsigned long long int recv = 0, send = 0;

        Utils::getNetworkBandWidth(prevRecv, prevSend);
        sleep(1);
        Utils::getNetworkBandWidth(recv, send);

        emit updateNetworkSpeed("↑" + Utils::networkConversion(send - prevSend), "↓" + Utils::networkConversion(recv - prevRecv));
    }
}
