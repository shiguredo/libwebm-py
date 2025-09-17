#!/usr/bin/env python3
"""
WebM ファイル作成例
"""

import webm


def main():
    """WebM ファイルを作成"""
    print("Creating WebM file...")

    writer = webm.WebMWriter()
    writer.open("output.webm")

    # ビデオトラックを追加 (640x480, VP8 コーデック)
    video_track = writer.add_video_track(640, 480, "V_VP8")
    print(f"Added video track: {video_track}")

    # オーディオトラックを追加 (48kHz, 2チャンネル, Opus コーデック)
    audio_track = writer.add_audio_track(48000, 2, "A_OPUS")
    print(f"Added audio track: {audio_track}")

    # ダミーフレームを追加
    for i in range(10):
        timestamp_ns = i * 33333333  # 約30fps
        is_keyframe = (i % 5) == 0  # 5フレームごとにキーフレーム

        # ビデオフレームを追加 (ダミーデータ)
        video_data = b'\x00' * 1000
        writer.add_frame(video_data, video_track, timestamp_ns, is_keyframe)

        # オーディオフレームを追加 (ダミーデータ)
        audio_data = b'\x00' * 500
        writer.add_frame(audio_data, audio_track, timestamp_ns, False)

    writer.finalize()
    print("WebM file created: output.webm")


if __name__ == "__main__":
    main()