#include "mpegts_mux.hpp"
#include "mpegts_pub.hpp"
#include "byte_stream.hpp"
#include "logger.hpp"


static const uint32_t crc32table[256] = {
    0x00000000, 0xB71DC104, 0x6E3B8209, 0xD926430D, 0xDC760413, 0x6B6BC517,
    0xB24D861A, 0x0550471E, 0xB8ED0826, 0x0FF0C922, 0xD6D68A2F, 0x61CB4B2B,
    0x649B0C35, 0xD386CD31, 0x0AA08E3C, 0xBDBD4F38, 0x70DB114C, 0xC7C6D048,
    0x1EE09345, 0xA9FD5241, 0xACAD155F, 0x1BB0D45B, 0xC2969756, 0x758B5652,
    0xC836196A, 0x7F2BD86E, 0xA60D9B63, 0x11105A67, 0x14401D79, 0xA35DDC7D,
    0x7A7B9F70, 0xCD665E74, 0xE0B62398, 0x57ABE29C, 0x8E8DA191, 0x39906095,
    0x3CC0278B, 0x8BDDE68F, 0x52FBA582, 0xE5E66486, 0x585B2BBE, 0xEF46EABA,
    0x3660A9B7, 0x817D68B3, 0x842D2FAD, 0x3330EEA9, 0xEA16ADA4, 0x5D0B6CA0,
    0x906D32D4, 0x2770F3D0, 0xFE56B0DD, 0x494B71D9, 0x4C1B36C7, 0xFB06F7C3,
    0x2220B4CE, 0x953D75CA, 0x28803AF2, 0x9F9DFBF6, 0x46BBB8FB, 0xF1A679FF,
    0xF4F63EE1, 0x43EBFFE5, 0x9ACDBCE8, 0x2DD07DEC, 0x77708634, 0xC06D4730,
    0x194B043D, 0xAE56C539, 0xAB068227, 0x1C1B4323, 0xC53D002E, 0x7220C12A,
    0xCF9D8E12, 0x78804F16, 0xA1A60C1B, 0x16BBCD1F, 0x13EB8A01, 0xA4F64B05,
    0x7DD00808, 0xCACDC90C, 0x07AB9778, 0xB0B6567C, 0x69901571, 0xDE8DD475,
    0xDBDD936B, 0x6CC0526F, 0xB5E61162, 0x02FBD066, 0xBF469F5E, 0x085B5E5A,
    0xD17D1D57, 0x6660DC53, 0x63309B4D, 0xD42D5A49, 0x0D0B1944, 0xBA16D840,
    0x97C6A5AC, 0x20DB64A8, 0xF9FD27A5, 0x4EE0E6A1, 0x4BB0A1BF, 0xFCAD60BB,
    0x258B23B6, 0x9296E2B2, 0x2F2BAD8A, 0x98366C8E, 0x41102F83, 0xF60DEE87,
    0xF35DA999, 0x4440689D, 0x9D662B90, 0x2A7BEA94, 0xE71DB4E0, 0x500075E4,
    0x892636E9, 0x3E3BF7ED, 0x3B6BB0F3, 0x8C7671F7, 0x555032FA, 0xE24DF3FE,
    0x5FF0BCC6, 0xE8ED7DC2, 0x31CB3ECF, 0x86D6FFCB, 0x8386B8D5, 0x349B79D1,
    0xEDBD3ADC, 0x5AA0FBD8, 0xEEE00C69, 0x59FDCD6D, 0x80DB8E60, 0x37C64F64,
    0x3296087A, 0x858BC97E, 0x5CAD8A73, 0xEBB04B77, 0x560D044F, 0xE110C54B,
    0x38368646, 0x8F2B4742, 0x8A7B005C, 0x3D66C158, 0xE4408255, 0x535D4351,
    0x9E3B1D25, 0x2926DC21, 0xF0009F2C, 0x471D5E28, 0x424D1936, 0xF550D832,
    0x2C769B3F, 0x9B6B5A3B, 0x26D61503, 0x91CBD407, 0x48ED970A, 0xFFF0560E,
    0xFAA01110, 0x4DBDD014, 0x949B9319, 0x2386521D, 0x0E562FF1, 0xB94BEEF5,
    0x606DADF8, 0xD7706CFC, 0xD2202BE2, 0x653DEAE6, 0xBC1BA9EB, 0x0B0668EF,
    0xB6BB27D7, 0x01A6E6D3, 0xD880A5DE, 0x6F9D64DA, 0x6ACD23C4, 0xDDD0E2C0,
    0x04F6A1CD, 0xB3EB60C9, 0x7E8D3EBD, 0xC990FFB9, 0x10B6BCB4, 0xA7AB7DB0,
    0xA2FB3AAE, 0x15E6FBAA, 0xCCC0B8A7, 0x7BDD79A3, 0xC660369B, 0x717DF79F,
    0xA85BB492, 0x1F467596, 0x1A163288, 0xAD0BF38C, 0x742DB081, 0xC3307185,
    0x99908A5D, 0x2E8D4B59, 0xF7AB0854, 0x40B6C950, 0x45E68E4E, 0xF2FB4F4A,
    0x2BDD0C47, 0x9CC0CD43, 0x217D827B, 0x9660437F, 0x4F460072, 0xF85BC176,
    0xFD0B8668, 0x4A16476C, 0x93300461, 0x242DC565, 0xE94B9B11, 0x5E565A15,
    0x87701918, 0x306DD81C, 0x353D9F02, 0x82205E06, 0x5B061D0B, 0xEC1BDC0F,
    0x51A69337, 0xE6BB5233, 0x3F9D113E, 0x8880D03A, 0x8DD09724, 0x3ACD5620,
    0xE3EB152D, 0x54F6D429, 0x7926A9C5, 0xCE3B68C1, 0x171D2BCC, 0xA000EAC8,
    0xA550ADD6, 0x124D6CD2, 0xCB6B2FDF, 0x7C76EEDB, 0xC1CBA1E3, 0x76D660E7,
    0xAFF023EA, 0x18EDE2EE, 0x1DBDA5F0, 0xAAA064F4, 0x738627F9, 0xC49BE6FD,
    0x09FDB889, 0xBEE0798D, 0x67C63A80, 0xD0DBFB84, 0xD58BBC9A, 0x62967D9E,
    0xBBB03E93, 0x0CADFF97, 0xB110B0AF, 0x060D71AB, 0xDF2B32A6, 0x6836F3A2,
    0x6D66B4BC, 0xDA7B75B8, 0x035D36B5, 0xB440F7B1
};

static uint32_t mpeg_crc32(uint32_t crc, const uint8_t *buffer, uint32_t size)
{  
    unsigned int i;

    for (i = 0; i < size; i++) {
        crc = crc32table[(crc ^ buffer[i]) & 0xff] ^ (crc >> 8);  
    }  
    return crc ;  
}

mpegts_mux::mpegts_mux(av_format_callback* cb):cb_(cb) {
    memset(pat_data_, 0xff, TS_PACKET_SIZE);
    memset(pmt_data_, 0xff, TS_PACKET_SIZE);

    pat_data_[0] = 0x47;
    pat_data_[1] = 0x40;
    pat_data_[2] = 0x00;
    pat_data_[3] = 0x10;
    pat_data_[4] = 0x00;

    pmt_data_[0] = 0x47;
    pmt_data_[1] = 0x50;
    pmt_data_[2] = 0x01;
    pmt_data_[3] = 0x10;
    pmt_data_[4] = 0x00;
}

mpegts_mux::~mpegts_mux() {

}

int mpegts_mux::input_packet(MEDIA_PACKET_PTR pkt_ptr) {
    int ret = -1;

    if ((last_pat_dts_ < 0) || ((last_pat_dts_ + pat_interval_) < pkt_ptr->dts_)) {
        last_pat_dts_ = pkt_ptr->dts_;
        ret = write_pat(pat_data_ + 5);
        if (ret < 0) {
            return ret;
        }
        ret = write_pmt(pmt_data_);
        if (ret < 0) {
            return ret;
        }
    }

    ret = write_pes((uint8_t*)pkt_ptr->buffer_ptr_->data(), pkt_ptr->buffer_ptr_->data_len(),
                (pkt_ptr->av_type_ == MEDIA_VIDEO_TYPE), pkt_ptr->is_key_frame_,
                pkt_ptr->dts_, pkt_ptr->pts_);
    if (ret < 0) {
        return ret;
    }
    return 0;
}

int mpegts_mux::write_pat(uint8_t* data) {
    uint32_t len = 0;

    len = pmt_count_ * 4 + 5 + 4; // 5 bytes remain header and 4 bytes crc32
    uint8_t* p = data;

    p[0] = PAT_TID_PAS;
    p++;

    // section_syntax_indicator = '1'
    // '0'
    // reserved '11'
    write_2bytes(p, (uint16_t)(0xb000 | len));
    p += 2;

    // transport_stream_id
    write_2bytes(p, transport_stream_id_);
    p += 2;

    // reserved '11'
    // version_number 'xxxxx'
    // current_next_indicator '1'
    *p = 0xc1 | (pat_ver_ << 1);
    p++;

    // section_number/last_section_number
    *p++ = 0;
    *p++ = 0;

    for(int i = 0; i < pmt_count_; i++)
    {
        write_2bytes(p + i * 4 + 0, program_number_);
        write_2bytes(p + i * 4 + 2, (uint16_t)(0xE000 | pmt_pid_));
    }
    p += 4 * pmt_count_;

    // crc32
    uint32_t crc = mpeg_crc32(0xffffffff, data, len-1);
    
    write_4bytes(p, crc);
    p += 4;

    return 0;
}

int mpegts_mux::write_pmt(uint8_t* data) {
    uint8_t* p = data;

    p[0] = PAT_TID_PMS;
    p++;

    // skip section_length
    p += 2;

    // program_number
    write_2bytes(p, pmt_pn);//0x00 01
    p += 2;
    
    // reserved '11'
    // version_number 'xxxxx'
    // current_next_indicator '1'
    *p++ = (uint8_t)(0xC1 | (pmt_ver_ << 1));

    // section_number/last_section_number
    *p++ = 0x00;
    *p++ = 0x00;

    // reserved '111'
    // PCR_PID 13-bits 0x1FFF
    write_2bytes(p, (uint16_t)(0xE000 | pcr_id_));
    p += 2;

    //assert(pmt->pminfo_len < 0x400);
    write_2bytes(p, (uint16_t)(0xF000 | pminfo_len_));
    p += 2;
    
    //TODO: set pminfo, there is no pm infor usually.

    //video
    if (1) {
        // stream_type
        *p++ = video_stream_type_;

        // reserved '111'
        // elementary_PID 13-bits
        write_2bytes(p, 0xE000 | video_pid_);
        p += 2;

        // reserved '1111'
        // ES_info_lengt 12-bits
        
        write_2bytes(p, 0xF000 | (uint16_t)0);// | len
        p += 2;
    }

    //audio
    if (1) {
        // stream_type
        *p++ = audio_stream_type_;

        // reserved '111'
        // elementary_PID 13-bits
        write_2bytes(p, 0xE000 | audio_pid_);
        p += 2;

        // reserved '1111'
        // ES_info_lengt 12-bits
        write_2bytes(p, 0xF000 | (uint16_t)0);// | len
        p += 2;
    }

    uint32_t len = p + 4 - (data + 3); // 4 bytes crc32

    write_2bytes(data + 1, (uint16_t)(0xb000 | len));
    // crc32
    uint32_t crc = mpeg_crc32(0xffffffff, data, (uint32_t)(p-data));
    //put32(p, crc);
    p[3] = (crc >> 24) & 0xFF;
    p[2] = (crc >> 16) & 0xFF;
    p[1] = (crc >> 8) & 0xFF;
    p[0] = crc & 0xFF;

    return (p - data) + 4; // total length
}

int mpegts_mux::write_pes_header(int64_t data_size,
        bool is_video, int64_t dts, int64_t pts) {
    uint8_t* p = pes_header_;
    uint16_t size = 0;

    *p++ = 0x00;
    *p++ = 0x00;
    *p++ = 0x01;

    if (is_video) {
        *p++ = video_sid_;
    } else {
        *p++ = audio_sid_;
    }

    uint8_t flag = 0x80;
    uint8_t ptslen = 5;
    uint8_t dtslen = 5;

    uint8_t header_size = ptslen;
    if (is_video && (pts != dts)) {
        flag |= 0x40;
        header_size += 5; //add dts
    }
    size = data_size + header_size + 3;
    if (size > 0xffff) {
        size = 0;
    }

    write_2bytes(p, size);
    p += 2;

    *p++ = 0x80;

    *p++ = flag;

    *p++ = header_size;

    write_ts(p, flag>>6, pts);
    p += 5;
    if (is_video && (pts != dts)) {
        write_ts(p, 1, dts);
        p += 5;
    }

    pes_header_size_ = p - pes_header_;
    return 0;
}

int mpegts_mux::write_pcr(uint8_t* data, int64_t pcr) {
    uint8_t* p = data;

    *p++ = (uint8_t)(pcr >> 25);
    *p++ = (uint8_t)((pcr >> 17) & 0xff);
    *p++ = (uint8_t)((pcr >> 9) & 0xff);
    *p++ = (uint8_t)((pcr >> 1) & 0xff);
    *p++ = (uint8_t)(((pcr & 0x1) << 7) | 0x7e);
    *p++ = 0x00;
    return 0;
}

int mpegts_mux::write_ts(uint8_t* data, uint8_t flag, int64_t ts) {
    uint32_t val = 0;
    uint8_t* p = data;

    if (ts > 0x1ffffffff) {
        ts -= 0x1ffffffff;
    }

    val = (uint32_t)(flag<<4) | (((uint32_t)(ts>>30) & 0x07) << 1) | 1;

    *p++ = (uint8_t)(val);

    val = ((uint32_t(ts>>15) & 0x7fff) << 1) | 1;
    *p++ = (uint8_t)(val >> 8);
    
    *p++ = (uint8_t)(val);

    val = ((uint32_t)(ts&0x7fff) << 1) | 1;
    *p++ = (uint8_t)(val >> 8);

    *p++ = (uint8_t)(val);
    return 0;
}

int mpegts_mux::adaptation_bufinit(uint8_t* data, uint8_t data_size, uint8_t remain_bytes) {
    data[0] = remain_bytes - 1;

    if (remain_bytes != 1) {
        data[1] = 0x00;
        for (int i = 2; i < data_size; i++) {
            data[i] = 0xff;
        }
    }
    return 0;
}

int mpegts_mux::write_pes(uint8_t* data, int64_t data_size,
                bool is_video, bool is_keyframe, int64_t dts, int64_t pts) {
    const int TS_DEF_DATALEN = 184;
    bool first = true;
    uint8_t data_len = 0;
    int64_t packet_bytes_len = 0;
    uint16_t pid = audio_pid_;
    uint8_t tmpLen = TS_PACKET_SIZE;
    int wBytes = 0;

    if (is_video) {
        pid = video_pid_;
    }

    write_pes_header(data_size, is_video, dts, pts);

    packet_bytes_len = data_size + pes_header_size_;

    while (packet_bytes_len > 0) {
        uint8_t ts_packet[TS_PACKET_SIZE];
        int i = 0;

        if (is_video) {
            video_cc_++;
            if (video_cc_ > 0xf) {
                video_cc_ = 0;
            }
        } else {
            audio_cc_++;
            if (audio_cc_ > 0xf) {
                audio_cc_ = 0;
            }
        }
        //sync byte
        ts_packet[i++] = 0x47;

        //error indicator, unit start indicator,ts priority,pid
        ts_packet[i++] = (uint8_t)(pid >> 8); //pid high 5 bits
        if (first) {
            ts_packet[i] = ts_packet[i] | 0x40; //unit start indicator
        }

        //pid low 8 bits
        ts_packet[i++] = pid;

        //scram control, adaptation control, counter
        if (is_video) {
            ts_packet[i++] = 0x10 | (uint8_t)(video_cc_&0x0f);
        } else {
            ts_packet[i++] = 0x10 | (uint8_t)(audio_cc_&0x0f);
        }

        //video key frame need pcr
        if (first && is_video && is_keyframe) {
            ts_packet[3] |= 0x20;
            ts_packet[i++] = 7;
            
            ts_packet[i++] = 0x50;

            write_pcr(ts_packet + i, dts);
            i += 6;
        }

        //frame data
        if (packet_bytes_len >= TS_DEF_DATALEN) {
            data_len = TS_DEF_DATALEN;
            if (first) {
                data_len -= (i - 4);
            }
        } else {
            ts_packet[3] |= 0x20; //have adaptation
            uint8_t remain_bytes = 0;
            data_len = (uint8_t)(packet_bytes_len);
            if (first) {
                remain_bytes = TS_DEF_DATALEN - data_len - (i - 4);
            } else {
                remain_bytes = TS_DEF_DATALEN - data_len;
            }
            //muxer.adaptationBufInit(muxer.tsPacket[i:], byte(remainBytes))
            adaptation_bufinit(ts_packet + i, TS_PACKET_SIZE - i, remain_bytes);
            i += remain_bytes;
        }

        if (first && (i < TS_PACKET_SIZE)) {
            tmpLen = TS_PACKET_SIZE - i;
            if (pes_header_size_ <= tmpLen) {
                tmpLen = pes_header_size_;
            }
            memcpy(ts_packet + i, pes_header_, tmpLen);
            i += tmpLen;
            data_len -= tmpLen;
        }

        if (i < TS_PACKET_SIZE) {
            tmpLen = TS_PACKET_SIZE - i;
            if (tmpLen <= data_len) {
                data_len = tmpLen;
            }
            memcpy(ts_packet + i, data + wBytes, data_len);

            wBytes += data_len;
            packet_bytes_len -= data_len;
        }
        first = false;
    }
    return 0;
}