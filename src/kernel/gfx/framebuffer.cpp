#include "framebuffer.hpp"
#include "device/v3d/mbox.hpp"
#include "device/v3d/mbox_message.hpp"
#include "kernel/logging.hpp"

namespace gfx
{

std::unique_ptr<FrameBuffer> FrameBuffer::createFromVC(size_t w, size_t h)
{
    auto set_phy_wh = videocore::MailBox::call<videocore::SetPhysicalWH>(w, h);
    assert(set_phy_wh.buff.code == videocore::MBoxBuffer::MBOX_RESP_SUCCESS);

    auto set_virt_wh = videocore::MailBox::call<videocore::SetVirtualWH>(w, h);
    assert(set_virt_wh.buff.code == videocore::MBoxBuffer::MBOX_RESP_SUCCESS);

    auto set_virt_off =
        videocore::MailBox::call<videocore::SetVirtualOffset>(0, 0);
    assert(set_virt_off.buff.code == videocore::MBoxBuffer::MBOX_RESP_SUCCESS);

    auto set_depth = videocore::MailBox::call<videocore::SetDepth>(32);
    assert(set_depth.buff.code == videocore::MBoxBuffer::MBOX_RESP_SUCCESS);

    auto set_px_order =
        videocore::MailBox::call<videocore::SetPixelOrder>(1); // RGB
    assert(set_px_order.buff.code == videocore::MBoxBuffer::MBOX_RESP_SUCCESS);

    auto alloc_fb = videocore::MailBox::call<videocore::AllocFB>(4096);
    assert(alloc_fb.buff.code == videocore::MBoxBuffer::MBOX_RESP_SUCCESS);

    auto get_pitch = videocore::MailBox::call<videocore::GetPitch>();
    assert(get_pitch.buff.code == videocore::MBoxBuffer::MBOX_RESP_SUCCESS);

    assert(set_depth.msg.resp.depth == 32);
    assert(alloc_fb.msg.resp.addr != 0);
    // assert(setPxOrder.msg.resp.state); // is rgb

    auto fb_ptr = reinterpret_cast<Color*>(alloc_fb.msg.resp.addr & 0x3FFFFFFF);

    size_t resp_w = set_virt_wh.msg.resp.width;
    size_t resp_h = set_virt_wh.msg.resp.height;
    size_t resp_pitch = get_pitch.msg.resp.pitch;

    return std::make_unique<FrameBuffer>(resp_w, resp_h, resp_pitch, fb_ptr);
}

}