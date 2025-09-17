#include "webm_reader.h"

#include <utility>

bool WebMReader::Open(const std::string &filename) {
  if (this->reader.Open(filename.c_str()) != 0) {
    return false;
  }

  long long pos = 0;
  if (this->ebml_header.Parse(&this->reader, pos) < 0) {
    return false;
  }

  mkvparser::Segment *seg;
  if (mkvparser::Segment::CreateInstance(&this->reader, pos, seg) != 0) {
    return false;
  }
  this->segment.reset(seg);

  if (this->segment->Load() < 0) {
    return false;
  }

  return true;
}

void WebMReader::Close() {
  this->reader.Close();
  this->segment.reset();
}

std::optional<double> WebMReader::GetDuration() const {
  if (!this->segment) {
    return std::nullopt;
  }

  const mkvparser::SegmentInfo *info = this->segment->GetInfo();
  if (!info) {
    return std::nullopt;
  }

  double duration_ns = static_cast<double>(info->GetDuration());
  return duration_ns / 1000000000.0;
}

std::optional<uint64_t> WebMReader::GetTracksCount() const {
  if (!this->segment) {
    return std::nullopt;
  }

  const mkvparser::Tracks *tracks = this->segment->GetTracks();
  if (!tracks) {
    return std::nullopt;
  }

  return tracks->GetTracksCount();
}

std::vector<std::string> WebMReader::GetTrackTypes() const {
  std::vector<std::string> track_types;

  if (!this->segment) {
    return track_types;
  }

  const mkvparser::Tracks *tracks = this->segment->GetTracks();
  if (!tracks) {
    return track_types;
  }

  for (unsigned long i = 0; i < tracks->GetTracksCount(); ++i) {
    const mkvparser::Track *track = tracks->GetTrackByIndex(i);
    if (track) {
      switch (track->GetType()) {
      case mkvparser::Track::kVideo:
        track_types.push_back("video");
        break;
      case mkvparser::Track::kAudio:
        track_types.push_back("audio");
        break;
      case mkvparser::Track::kSubtitle:
        track_types.push_back("subtitle");
        break;
      default:
        track_types.push_back("unknown");
        break;
      }
    }
  }

  return track_types;
}

std::optional<std::pair<uint64_t, uint64_t>>
WebMReader::GetVideoResolution() const {
  if (!this->segment) {
    return std::nullopt;
  }

  const mkvparser::Tracks *tracks = this->segment->GetTracks();
  if (!tracks) {
    return std::nullopt;
  }

  for (unsigned long i = 0; i < tracks->GetTracksCount(); ++i) {
    const mkvparser::Track *track = tracks->GetTrackByIndex(i);
    if (track && track->GetType() == mkvparser::Track::kVideo) {
      const mkvparser::VideoTrack *video_track =
          static_cast<const mkvparser::VideoTrack *>(track);
      uint64_t width = video_track->GetWidth();
      uint64_t height = video_track->GetHeight();
      return std::make_pair(width, height);
    }
  }

  return std::nullopt;
}
