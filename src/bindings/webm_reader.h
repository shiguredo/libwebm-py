#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "mkvparser/mkvparser.h"
#include "mkvparser/mkvreader.h"

class WebMReader {
private:
  mkvparser::MkvReader reader;
  mkvparser::EBMLHeader ebml_header;
  std::unique_ptr<mkvparser::Segment> segment;

public:
  bool Open(const std::string &filename);
  void Close();

  std::optional<double> GetDuration() const;
  std::optional<uint64_t> GetTracksCount() const;
  std::vector<std::string> GetTrackTypes() const;
  std::optional<std::pair<uint64_t, uint64_t>> GetVideoResolution() const;
};
