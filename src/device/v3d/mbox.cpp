#include "mbox.hpp"
#include "device/mmio.hpp"
#include "mbox_message.hpp"

namespace videocore
{
void MailBox::doCall(Channel channel,
                     const void* box = const_cast<u32*>(s_mbox))
{
    u32 ptr =
        static_cast<u32>(reinterpret_cast<intptr_t>(box) | (channel & 0xF));

    // wait until we can write
    while (isFull())
        ;

    // write address
    mmio::write(mmio::MBOX_WRITE, ptr);

    while (true)
    {
        // wait for reply
        while (isEmpty())
            ;

        // is it a reply to our message?
        if (mmio::read(mmio::MBOX_READ) == ptr)
            break;
    }
}
}