{
  "targets": [
    {
      "target_name": "py",
      "cflags!": [ "-fno-exceptions", "<!(python3-config --cflags)" ],
      "cflags_cc!": [ "-fno-exceptions" ],
      "link_settings": {
        "ldflags": [
            "<!(python3-config --ldflags)",
        ]
      },
      "sources": [ "src/python.cc", "src/pythonBinding/pythoncode.cc" ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")",
        "<!(python3-config --includes)",
      ],
      'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ],
    }
  ]
}
