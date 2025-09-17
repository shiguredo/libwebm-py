#!/usr/bin/env python3
"""
バージョン情報表示
"""

import webm


def main():
    """バージョン情報を表示"""
    print("=== バージョン情報 ===")
    print(f"libwebm-py: {webm.__version__}")
    print(f"libwebm:    {webm.native_version}")


if __name__ == "__main__":
    main()