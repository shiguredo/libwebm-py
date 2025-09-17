#!/usr/bin/env python3
"""
WebM ファイル読み込み例
"""

import sys
import webm


def main(filename="output.webm"):
    """WebM ファイルを読み込んで情報を表示"""
    print(f"Reading {filename}...")

    reader = webm.WebMReader()
    if not reader.open(filename):
        print(f"Failed to open {filename}")
        sys.exit(1)

    # ファイル情報を取得
    duration = reader.get_duration()
    if duration:
        print(f"Duration: {duration:.2f} seconds")

    tracks_count = reader.get_tracks_count()
    print(f"Number of tracks: {tracks_count}")

    track_types = reader.get_track_types()
    print(f"Track types: {track_types}")

    resolution = reader.get_video_resolution()
    if resolution:
        print(f"Video resolution: {resolution[0]}x{resolution[1]}")

    reader.close()


if __name__ == "__main__":
    filename = sys.argv[1] if len(sys.argv) > 1 else "output.webm"
    main(filename)