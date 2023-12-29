# klangstrom-playdate

uses [Playdate-C++](https://github.com/nstbayless/playdate-cpp)

- install [Playdate SDK](https://play.date/dev/)
- set environment variable `PLAYDATE_SDK_PATH` e.g in `~/.zshrc` with `export PLAYDATE_SDK_PATH=$HOME/Developer/PlaydateSDK`
- comment out `int eventHandler(PlaydateAPI* playdate, PDSystemEvent event, uint32_t arg);` in file `$PLAYDATE_SDK_PATH/C_API/pd_api.h` ( approx line 62 )

see [[CPP] Guide: C++ on Playdate](https://devforum.play.date/t/cpp-guide-c-on-playdate/5085) from [Playdate Developer Forum](https://devforum.play.date) for more info.