#ifndef H264_PACK_HANDLE_HPP
#define H264_PACK_HANDLE_HPP
#include "pack_handle_pub.hpp"
#include "rtp_packet.hpp"
#include "timer.hpp"
#include <queue>

class h264_pack_handle : public timer_interface
{
public:
    h264_pack_handle(pack_callbackI* cb, boost::asio::io_context& io_ctx);
    virtual ~h264_pack_handle();

public:
    virtual void input_rtp_packet(std::shared_ptr<rtp_packet_info> pkt_ptr) override;

public:
    virtual void on_timer() override;
    
private:
    void get_startend_bit(rtp_packet* pkt, bool& start, bool& end);
    void reset_rtp_fua();
    bool demux_fua(MEDIA_PACKET_PTR h264_pkt_ptr, int64_t& timestamp);
    bool demux_stapA(std::shared_ptr<rtp_packet_info>);
    bool parse_stapA_offsets(const uint8_t* data, size_t data_len, std::vector<size_t> &offsets);
    void check_timeout();
    
private:
    bool init_flag_  = false;
    bool start_flag_ = false;
    bool end_flag_   = false;
    int64_t last_extend_seq_ = 0;
    std::deque<std::shared_ptr<rtp_packet_info>> packets_queue_;
    pack_callbackI* cb_ = nullptr;
};

#endif