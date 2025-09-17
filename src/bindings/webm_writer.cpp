#include "webm_writer.h"

bool WebMWriter::Open(const std::string &filename) {
  if (!this->writer.Open(filename.c_str())) {
    return false;
  }

  this->segment = std::make_unique<mkvmuxer::Segment>();

  if (!this->segment->Init(&this->writer)) {
    return false;
  }

  this->segment->set_mode(mkvmuxer::Segment::kFile);

  mkvmuxer::SegmentInfo *info = this->segment->GetSegmentInfo();
  info->set_writing_app("libwebm-py");
  info->set_muxing_app("libwebm-py");

  return true;
}

uint64_t WebMWriter::AddVideoTrack(uint64_t width, uint64_t height,
                                   const std::string &codec_id) {
  if (!this->segment) {
    return 0;
  }

  this->video_track_id = this->segment->AddVideoTrack(
      static_cast<int>(width), static_cast<int>(height), 0);

  if (this->video_track_id == 0) {
    return 0;
  }

  mkvmuxer::VideoTrack *video_track = static_cast<mkvmuxer::VideoTrack *>(
      this->segment->GetTrackByNumber(this->video_track_id));

  if (video_track) {
    video_track->set_codec_id(codec_id.c_str());
  }

  return this->video_track_id;
}

uint64_t WebMWriter::AddAudioTrack(uint32_t sample_rate, uint32_t channels,
                                   const std::string &codec_id) {
  if (!this->segment) {
    return 0;
  }

  this->audio_track_id = this->segment->AddAudioTrack(sample_rate, channels, 0);

  if (this->audio_track_id == 0) {
    return 0;
  }

  mkvmuxer::AudioTrack *audio_track = static_cast<mkvmuxer::AudioTrack *>(
      this->segment->GetTrackByNumber(this->audio_track_id));

  if (audio_track) {
    audio_track->set_codec_id(codec_id.c_str());
  }

  return this->audio_track_id;
}

bool WebMWriter::AddFrame(const uint8_t *data, size_t size, uint64_t track_id,
                          uint64_t timestamp_ns, bool is_key) {
  if (!this->segment) {
    return false;
  }

  return this->segment->AddFrame(data, size, track_id, timestamp_ns, is_key);
}

bool WebMWriter::Finalize() {
  if (!this->segment) {
    return false;
  }

  if (!this->segment->Finalize()) {
    return false;
  }

  this->writer.Close();
  return true;
}
