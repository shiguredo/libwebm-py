"""
libwebm-py のテスト
"""

import os
import pytest
import webm


@pytest.fixture
def test_data_dir():
    """テストデータディレクトリのパス"""
    return "_build/_deps/libwebm-src/testing/testdata"


def test_import():
    """モジュールのインポートテスト"""
    assert hasattr(webm, "WebMReader")
    assert hasattr(webm, "WebMWriter")
    assert hasattr(webm, "__version__")
    assert hasattr(webm, "native_version")
    assert webm.__version__ != "unknown"  # ビルド時に生成されるはず
    assert webm.native_version == "0.3.4.0"  # libwebm のバージョン


def test_reader_simple_block(test_data_dir):
    """simple_block.webm の読み込みテスト"""
    test_file = os.path.join(test_data_dir, "simple_block.webm")

    reader = webm.WebMReader()
    assert reader.open(test_file)

    # ファイル情報の取得
    duration = reader.get_duration()
    assert duration is not None
    assert duration > 0

    tracks_count = reader.get_tracks_count()
    assert tracks_count is not None
    assert tracks_count > 0

    track_types = reader.get_track_types()
    assert track_types is not None
    assert len(track_types) > 0

    reader.close()


def test_reader_metadata_block(test_data_dir):
    """metadata_block.webm の読み込みテスト"""
    test_file = os.path.join(test_data_dir, "metadata_block.webm")

    reader = webm.WebMReader()
    assert reader.open(test_file)

    tracks_count = reader.get_tracks_count()
    assert tracks_count is not None

    reader.close()


def test_reader_colour(test_data_dir):
    """colour.webm の読み込みテスト（ビデオ解像度確認）"""
    test_file = os.path.join(test_data_dir, "colour.webm")

    reader = webm.WebMReader()
    assert reader.open(test_file)

    resolution = reader.get_video_resolution()
    assert resolution is not None
    assert resolution[0] > 0  # width
    assert resolution[1] > 0  # height

    reader.close()


def test_writer_basic():
    """基本的な WebM ファイル作成テスト"""
    output_file = "test_output.webm"

    try:
        writer = webm.WebMWriter()
        assert writer.open(output_file)

        # ビデオトラック追加
        video_track = writer.add_video_track(320, 240, "V_VP8")
        assert video_track > 0

        # ダミーフレーム追加
        test_data = b"\x00" * 100
        assert writer.add_frame(test_data, video_track, 0, True)
        assert writer.add_frame(test_data, video_track, 33333333, False)

        assert writer.finalize()

        # 作成したファイルの確認
        assert os.path.exists(output_file)
        assert os.path.getsize(output_file) > 0

    finally:
        if os.path.exists(output_file):
            os.remove(output_file)


def test_writer_with_audio():
    """オーディオトラック付き WebM ファイル作成テスト"""
    output_file = "test_audio_output.webm"

    try:
        writer = webm.WebMWriter()
        assert writer.open(output_file)

        # ビデオトラック追加
        video_track = writer.add_video_track(640, 480, "V_VP9")
        assert video_track > 0

        # オーディオトラック追加
        audio_track = writer.add_audio_track(48000, 2, "A_OPUS")
        assert audio_track > 0

        # フレーム追加
        video_data = b"\x00" * 1000
        audio_data = b"\x00" * 500

        for i in range(5):
            timestamp_ns = i * 40000000  # 25fps
            is_key = i == 0
            assert writer.add_frame(video_data, video_track, timestamp_ns, is_key)
            assert writer.add_frame(audio_data, audio_track, timestamp_ns, False)

        assert writer.finalize()

        # 作成したファイルを読み込んで確認
        reader = webm.WebMReader()
        assert reader.open(output_file)

        tracks = reader.get_track_types()
        assert "video" in tracks
        assert "audio" in tracks

        reader.close()

    finally:
        if os.path.exists(output_file):
            os.remove(output_file)


def test_invalid_file():
    """存在しないファイルの読み込みテスト"""
    reader = webm.WebMReader()
    assert not reader.open("non_existent_file.webm")
