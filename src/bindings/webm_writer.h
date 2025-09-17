#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>

#include "mkvmuxer/mkvmuxer.h"
#include "mkvmuxer/mkvmuxerutil.h"
#include "mkvmuxer/mkvwriter.h"

class WebMWriter {
private:
  mkvmuxer::MkvWriter writer;
  std::unique_ptr<mkvmuxer::Segment> segment;
  uint64_t video_track_id = 0;
  uint64_t audio_track_id = 0;

public:
  bool Open(const std::string &filename);

  uint64_t AddVideoTrack(uint64_t width, uint64_t height,
                         const std::string &codec_id);
  uint64_t AddAudioTrack(uint32_t sample_rate, uint32_t channels,
                         const std::string &codec_id);
  bool AddFrame(const uint8_t *data, size_t size, uint64_t track_id,
                uint64_t timestamp_ns, bool is_key);
  bool Finalize();
};
