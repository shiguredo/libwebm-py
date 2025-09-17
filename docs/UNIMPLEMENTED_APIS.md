# libwebm-py 未実装 API リスト

## 現在の実装状況

### 実装済み機能

#### WebMReader (mkvparser)

- ファイルの読み込み/クローズ
- duration の取得
- トラック数の取得
- トラックタイプの取得
- ビデオ解像度の取得

#### WebMWriter (mkvmuxer)

- ファイルの作成/クローズ
- ビデオトラックの追加
- オーディオトラックの追加
- フレームの追加
- ファイルのファイナライズ

## 未実装の主要 API

### 1. 読み込み機能 (mkvparser)

#### Segment ヘッダと基本情報

- `Segment::ParseHeaders()` - ヘッダだけを先に解釈する段階的パース
- `Segment::LoadCluster(long long& pos, long& size)` / `LoadCluster()` - クラスタ単位の逐次ロード
- `Segment::ParseNext(const Cluster*, const Cluster*&, long long&, long&)` - 解析済みクラスタから次クラスタをパース
- `Segment::DoneParsing()` - 解析完了フラグの確認
- `Segment::GetSeekHead()` / `GetInfo()` / `GetTracks()` / `GetChapters()` / `GetTags()` - セグメント直下メタ要素の参照
- `Segment::GetDuration()` - セグメント全体の duration 取得
- `Segment::GetCount()` / `GetLast()` - ロード済みクラスタ数と末尾アクセス
- `Segment::FindCluster(long long time_nanoseconds)` / `FindOrPreloadCluster(long long pos)` - 時間やオフセットからクラスタを検索
- `Segment::ParseCues(long long cues_off, long long& parse_pos, long& parse_len)` - Cue セクションの明示的パース

#### SegmentInfo の詳細取得

- `SegmentInfo::GetTimeCodeScale()` - timecode scale の取得
- `SegmentInfo::GetDuration()` - スケール済み duration の取得
- `SegmentInfo::GetMuxingAppAsUTF8()` / `GetWritingAppAsUTF8()` / `GetTitleAsUTF8()` - 文字列メタデータの取得

#### SeekHead / Cues 連携

- `SeekHead::GetCount()` / `GetEntry(int)` / `GetVoidElementCount()` / `GetVoidElement(int)` - SeekHead と Void 要素の参照
- `Cues::GetFirst()` / `GetNext(const CuePoint*)` / `GetLast()` - CuePoint の列挙
- `Cues::GetBlock(const CuePoint*, const CuePoint::TrackPosition*)` - Cue から BlockEntry を取得
- `Cues::GetCount()` / `LoadCuePoint()` / `DoneParsing()` - Cue のロード状態管理
- `CuePoint::GetTimeCode()` / `GetTime(const Segment*)` / `Find(const Track*)` - CuePoint の時間情報とトラック別検索
- `CuePoint::TrackPosition` の `m_pos` / `m_block` - クラスタオフセットとブロック番号の取得

#### Block / Cluster 操作

- `Cluster::GetTimeCode()` / `GetTime()` / `GetFirstTime()` / `GetLastTime()` - クラスタ内の時間情報
- `Cluster::GetFirst(const BlockEntry*&)` / `GetLast(const BlockEntry*&)` / `GetNext(const BlockEntry*, const BlockEntry*& )` / `GetEntry(const Track*, long long)` - ブロック列挙と検索
- `Cluster::GetEntryCount()` / `GetPosition()` / `GetElementSize()` / `Load(long long&, long&)` / `Parse(long long&, long&)` - クラスタのサイズ情報とパース
- `BlockEntry::GetKind()` / `GetCluster()` / `GetIndex()` - ブロック種別と位置情報
- `Block::GetTrackNumber()` / `GetTimeCode()` / `GetTime()` - ブロックのトラック番号と時間取得
- `Block::GetFrameCount()` / `GetFrame(int)` / `Block::Frame::Read(IMkvReader*, unsigned char*)` - 複数フレームとデータ取り出し
- `Block::IsKey()` / `SetKey(bool)` / `IsInvisible()` / `GetLacing()` / `GetDiscardPadding()` - ブロック属性取得と設定
- `BlockGroup::GetPrevTimeCode()` / `GetNextTimeCode()` / `GetDurationTimeCode()` - BlockGroup の隣接タイムコード情報

#### トラック共通情報

- `Tracks::GetTrackByNumber(long)` / `GetTrackByIndex(unsigned long)` - トラック参照
- `Track::GetNameAsUTF8()` / `GetLanguage()` / `GetCodecNameAsUTF8()` - トラックメタ情報
- `Track::GetCodecId()` / `GetCodecPrivate(size_t&)` - コーデック ID と private データの取得
- `Track::GetNumber()` / `GetUid()` / `GetDefaultDuration()` / `GetCodecDelay()` / `GetSeekPreRoll()` - トラック固有値の取得
- `Track::GetFirst(const BlockEntry*&)` / `GetNext(const BlockEntry*, const BlockEntry*& )` / `Seek(long long, const BlockEntry*& )` - トラック単位でのブロック列挙とシーク
- `Track::GetContentEncodingCount()` / `GetContentEncodingByIndex(unsigned long)` - コンテンツエンコーディングの参照
- `ContentEncoding::GetCompressionCount()` / `GetCompressionByIndex(unsigned long)` - 圧縮設定の列挙
- `ContentEncoding::GetEncryptionCount()` / `GetEncryptionByIndex(unsigned long)` / `ContentEncoding::ContentEncAESSettings` - 暗号化設定の取得

#### VideoTrack の詳細情報

- `VideoTrack::GetDisplayWidth()` / `GetDisplayHeight()` / `GetDisplayUnit()` - 表示サイズと単位
- `VideoTrack::GetStereoMode()` - ステレオ 3D モード情報
- `VideoTrack::GetFrameRate()` - フレームレート
- `VideoTrack::GetColourSpace()` - カラースペースの取得
- `VideoTrack::GetColour()` / `Colour::Parse()` / `Colour::mastering_metadata` - HDR/Colour メタデータの参照
- `VideoTrack::GetProjection()` / `Projection::Parse()` - プロジェクション設定（360 度動画など）

#### AudioTrack の詳細情報

- `AudioTrack::GetSamplingRate()` / `GetChannels()` / `GetBitDepth()` - 音声トラック詳細

#### メタデータの取得

- `Chapters::GetEditionCount()` / `GetEdition(int)` - Edition の列挙
- `Chapters::Edition::GetAtomCount()` / `GetAtom(int)` - Chapter Atom の取得
- `Chapters::Atom::GetStringUID()` / `GetStartTimecode()` / `GetStopTimecode()` / `GetDisplayCount()` / `GetDisplay(int)` - チャプター固有情報
- `Chapters::Display::GetString()` / `GetLanguage()` / `GetCountry()` - 言語別タイトルの取得
- `Tags::GetTagCount()` / `GetTag(int)` - タグ列挙
- `Tags::Tag::GetSimpleTagCount()` / `GetSimpleTag(int)` - SimpleTag の取得
- `Tags::SimpleTag::GetTagName()` / `GetTagString()` - タグ名と値の取り出し

### 2. 書き込み機能 (mkvmuxer)

#### Segment 全体制御

- `Segment::AddTrack(int32_t number)` - 汎用トラックの追加
- `Segment::AddCuePoint(uint64_t timestamp, uint64_t track)` - CuePoint の自動生成
- `Segment::AddMetadata(...)` - メタデータフレームの追加
- `Segment::AddFrameWithAdditional(...)` / `AddFrameWithDiscardPadding(...)` / `AddGenericFrame(const Frame*)` - 追加データや DiscardPadding 含みのフレーム書き込み
- `Segment::CopyAndMoveCuesBeforeClusters(...)` - Cues をクラスタ前に移動したコピー生成
- `Segment::CuesTrack(uint64_t track_number)` - Cue 対象トラックの指定
- `Segment::OutputCues(bool)` / `AccurateClusterDuration(bool)` / `UseFixedSizeClusterTimecode(bool)` - 出力挙動のチューニング
- `Segment::SetChunking(bool, const char* filename)` / `chunking()` - チャンク出力の制御
- `Segment::set_max_cluster_duration(uint64_t)` / `set_max_cluster_size(uint64_t)` - クラスタ境界条件の制御
- `Segment::set_estimate_file_duration(bool)` / `set_duration(double)` - duration 推定と明示設定
- `Segment::DocTypeIsWebm()` - コーデック ID の整合性検証

#### Cues の管理

- `Cues::AddCue(CuePoint*)` - 手動 CuePoint 追加
- `Cues::GetCueByIndex(int32_t)` / `Size()` / `Write(IMkvWriter*)` - CuePoint の列挙と書き出し
- `Cues::set_output_block_number(bool)` / `output_block_number()` - CuePoint の BlockNumber 出力制御
- `CuePoint::set_block_number(uint64_t)` / `set_output_block_number(bool)` - CuePoint 単位の詳細設定
- `Segment::GetCues()` - 生成済み Cues オブジェクトへのアクセス

#### フレーム追加周りのユーティリティ

- `Frame::Init(const uint8_t*, uint64_t)` / `CopyFrom(const Frame&)` - フレームデータのセットアップ
- `Frame::AddAdditionalData(const uint8_t*, uint64_t, uint64_t)` - Additional データの追加
- `Frame::set_duration(uint64_t)` / `duration_set()` - BlockDuration の明示
- `Frame::set_discard_padding(int64_t)` - DiscardPadding の設定
- `Frame::set_reference_block_timestamp(int64_t)` / `reference_block_timestamp_set()` - 参照ブロックタイムコード
- `Frame::set_track_number(uint64_t)` / `set_timestamp(uint64_t)` / `set_is_key(bool)` - 各種基本プロパティ設定

#### 詳細なトラック設定

- `Track::SetCodecPrivate(const uint8_t*, uint64_t)` - コーデック private データの設定
- `Track::set_codec_id(const char*)` / `set_name(const char*)` / `set_language(const char*)` - メタ情報の設定
- `Track::set_number(uint64_t)` / `set_type(uint64_t)` / `set_uid(uint64_t)` - トラック識別子の制御
- `Track::set_codec_delay(uint64_t)` / `set_seek_pre_roll(uint64_t)` / `set_default_duration(uint64_t)` - タイミング関連設定
- `Track::AddContentEncoding()` / `GetContentEncodingByIndex(uint32_t)` - コンテンツエンコーディング要素の生成と参照

#### VideoTrack の詳細設定

- `VideoTrack::SetStereoMode(uint64_t)` / `SetAlphaMode(uint64_t)` - ステレオ/アルファ設定
- `VideoTrack::set_pixel_width(uint64_t)` / `set_pixel_height(uint64_t)` - ピクセルサイズ
- `VideoTrack::set_display_width(uint64_t)` / `set_display_height(uint64_t)` - 表示設定
- `VideoTrack::set_crop_left(uint64_t)` / `set_crop_right(uint64_t)` / `set_crop_top(uint64_t)` / `set_crop_bottom(uint64_t)` - クロップ領域
- `VideoTrack::set_frame_rate(double)` / `set_width(uint64_t)` / `set_height(uint64_t)` - 基本プロパティ
- `VideoTrack::set_colour_space(const char*)` / `SetColour(const Colour&)` / `colour()` - カラー関連設定
- `VideoTrack::SetProjection(const Projection&)` / `projection()` - プロジェクションデータ

#### AudioTrack の詳細設定

- `AudioTrack::set_sample_rate(double)` / `set_channels(uint64_t)` / `set_bit_depth(uint64_t)` - 音声パラメータ調整

#### SegmentInfo の詳細設定

- `SegmentInfo::Init()` / `Write(IMkvWriter*)` / `Finalize(IMkvWriter*)` - SegmentInfo の書き出しライフサイクル
- `SegmentInfo::set_muxing_app(const char*)` / `set_writing_app(const char*)` - アプリケーション情報
- `SegmentInfo::set_timecode_scale(uint64_t)` / `set_duration(double)` / `set_date_utc(int64_t)` - タイムスケールと日付

#### SeekHead の制御

- `SeekHead::AddSeekEntry(uint32_t, uint64_t)` / `SetSeekEntry(int, uint32_t, uint64_t)` - SeekHead エントリ管理
- `SeekHead::Write(IMkvWriter*)` / `Finalize(IMkvWriter*)` - SeekHead の書き出し
- `SeekHead::GetId(int)` / `GetPosition(int)` - 生成済みエントリの参照

#### メタデータの書き込み

- `Segment::AddChapter()` / `Chapters::Count()` / `Chapters::Write(IMkvWriter*)` - チャプター出力
- `Chapter::set_id(const char*)` / `set_time(const Segment&, uint64_t, uint64_t)` / `set_uid(uint64_t)` / `add_string(const char*, const char*, const char*)` - チャプター詳細設定
- `Segment::AddTag()` / `Tags::Count()` / `Tags::Write(IMkvWriter*)` - タグ出力
- `Tag::add_simple_tag(const char*, const char*)` - SimpleTag 追加

#### クラスタ管理

- `Cluster::AddFrame(const Frame*)` / `AddFrame(const uint8_t*, uint64_t, uint64_t, uint64_t, bool)` - クラスタ直書き
- `Cluster::AddMetadata(...)` - メタデータブロック追加
- `Cluster::Finalize()` / `Finalize(bool, uint64_t)` - クラスタクローズ処理
- `Cluster::set_write_last_frame_with_duration(bool)` / `write_last_frame_with_duration()` - 最終フレーム duration の扱い
- `Cluster::payload_size()` / `position_for_cues()` / `timecode()` などのゲッター - 状態把握

#### ContentEncoding と暗号化

- `Track::AddContentEncoding()` - エンコーディング要素の生成
- `ContentEncoding::SetEncryptionID(const uint8_t*, uint64_t)` - 暗号化キー ID の設定
- `ContentEncoding::enc_aes_settings()` / `ContentEncAESSettings::Write(IMkvWriter*)` - AES 設定の書き出し

### 3. 高度な機能

#### ストリーミング対応

- `Segment::set_mode(Segment::kLive)` / `mode()` - ライブモード切り替え
- `Segment::SetChunking()` - 大規模配信向けチャンク分割
- `Segment::CopyAndMoveCuesBeforeClusters()` - Cue を前方に移動した再配置

#### 暗号化/DRM

- `ContentEncoding::ContentEncryption` / `ContentEncAESSettings` - 暗号化アルゴリズムとキー情報
- `Frame::set_reference_block_timestamp()` - 参照ブロック指定による再生制御

#### HDR/広色域対応

- `Colour` / `MasteringMetadata` / `PrimaryChromaticity` / `Projection` - HDR10 や 360 度動画向け付加情報の取得・設定
- `VideoTrack::SetColour()` / `GetColour()` - カラーコンポーネントのラウンドトリップ

### 4. ユーティリティ機能

#### I/O と基盤ユーティリティ

- `mkvparser::MkvReader::Open()` / `Close()` / `Read()` / `Length()` - ファイルリーダの直接利用とカスタム IMkvReader 実装
- `mkvmuxer::MkvWriter::Open()` / `Close()` / `Write()` / `ElementStartNotify()` - ファイルライタの直接利用
- `mkvmuxer::IMkvWriter` / `mkvparser::IMkvReader` - カスタム I/O 実装用インターフェイス
- `mkvmuxer::WriteEbmlHeader()` / `WriteEbmlMasterElement()` / `WriteEbmlElement()` / `WriteVoidElement()` - EBML ヘルパ
- `mkvmuxer::ChunkedCopy()` - 既存ファイルから部分コピー

#### WebVTT サポート

- `libwebvtt::Parser` / `Cue` / `Time` - WebVTT 字幕の解析
- `dumpvtt.cc` / `vttdemux.cc` / `webm_parser` - WebVTT と WebM の相互変換サンプル
