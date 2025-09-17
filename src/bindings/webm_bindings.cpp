#include <nanobind/nanobind.h>
#include <nanobind/stl/optional.h>
#include <nanobind/stl/pair.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>

#include "mkvmuxer/mkvmuxer.h"

#include "webm_reader.h"
#include "webm_writer.h"

namespace nb = nanobind;
using namespace nb::literals;

NB_MODULE(_webm, m) {
  m.doc() = "Python bindings for libwebm";

  // libwebm のバージョンを取得
  m.def(
      "get_libwebm_version",
      []() -> std::string {
        int32_t major, minor, build, revision;
        mkvmuxer::GetVersion(&major, &minor, &build, &revision);
        return std::to_string(major) + "." + std::to_string(minor) + "." +
               std::to_string(build) + "." + std::to_string(revision);
      },
      "Get libwebm library version");

  nb::class_<WebMReader>(m, "WebMReader")
      .def(nb::init<>())
      .def("open", &WebMReader::Open, "filename"_a,
           "Open a WebM file for reading")
      .def("close", &WebMReader::Close, "Close the WebM file")
      .def("get_duration", &WebMReader::GetDuration,
           "Get the duration of the WebM file in seconds")
      .def("get_tracks_count", &WebMReader::GetTracksCount,
           "Get the number of tracks in the WebM file")
      .def("get_track_types", &WebMReader::GetTrackTypes,
           "Get the types of all tracks in the WebM file")
      .def("get_video_resolution", &WebMReader::GetVideoResolution,
           "Get the resolution of the first video track as (width, height)")
      .def(
          "__enter__", [](WebMReader &self) -> WebMReader * { return &self; },
          nb::rv_policy::reference_internal)
      .def("__exit__", [](WebMReader &self, nb::handle exc_type,
                          nb::handle exc_value, nb::handle traceback) {
        self.Close();
        return false; // 例外を抑制しない
      });

  nb::class_<WebMWriter>(m, "WebMWriter")
      .def(nb::init<>())
      .def("open", &WebMWriter::Open, "filename"_a,
           "Open a WebM file for writing")
      .def("add_video_track", &WebMWriter::AddVideoTrack, "width"_a, "height"_a,
           "codec_id"_a = "V_VP8", "Add a video track to the WebM file")
      .def("add_audio_track", &WebMWriter::AddAudioTrack, "sample_rate"_a,
           "channels"_a, "codec_id"_a = "A_OPUS",
           "Add an audio track to the WebM file")
      .def(
          "add_frame",
          [](WebMWriter &self, const nb::bytes &data, uint64_t track_id,
             uint64_t timestamp_ns, bool is_key) {
            return self.AddFrame(
                reinterpret_cast<const uint8_t *>(data.c_str()), data.size(),
                track_id, timestamp_ns, is_key);
          },
          "data"_a, "track_id"_a, "timestamp_ns"_a, "is_key"_a = false,
          "Add a frame to the WebM file")
      .def("finalize", &WebMWriter::Finalize, "Finalize the WebM file")
      .def(
          "__enter__", [](WebMWriter &self) -> WebMWriter * { return &self; },
          nb::rv_policy::reference_internal)
      .def("__exit__", [](WebMWriter &self, nb::handle exc_type,
                          nb::handle exc_value, nb::handle traceback) {
        self.Finalize();
        return false; // 例外を抑制しない
      });
}
