{
  "version": "1.0.4",
  "vars": {
    "SKIA_ROOT": "https://raw.githubusercontent.com/cyderjs/libskia/release/m58",
    "V8_ROOT": "https://raw.githubusercontent.com/cyderjs/libv8/release/5.7.492"
  },
  "files": {
    "common": [
      {
        "url": "${SKIA_ROOT}/include.zip",
        "dir": "third_party/skia",
        "unzip": true
      },
      {
        "url": "${V8_ROOT}/include.zip",
        "dir": "third_party/v8",
        "unzip": "true"
      }
    ],
    "mac": [
      {
        "url": "${SKIA_ROOT}/darwin-x64-shared.zip",
        "dir": "third_party/skia",
        "unzip": true
      },
      {
        "url": "${V8_ROOT}/darwin-x64-shared.zip",
        "dir": "third_party/v8",
        "unzip": true
      }
    ],
    "win": [
      {
        "url": "${SKIA_ROOT}/win-ia32-shared.zip",
        "dir": "third_party/skia",
        "unzip": true
      },
      {
        "url": "${V8_ROOT}/win-ia32-shared.zip",
        "dir": "third_party/v8",
        "unzip": true
      }
    ]
  },
  "actions": {
    "common": [
      {
        "command": "node build_ts",
        "dir": "tools"
      }
    ]
  }
}