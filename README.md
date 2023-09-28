# klangstrom-playdate

- install [Playdate SDK](https://play.date/dev/)
- comment out `int eventHandler(PlaydateAPI* playdate, PDSystemEvent event, uint32_t arg);` in file `pd_api.h` ( approx line 62 )
- set environment variable `PLAYDATE_SDK_PATH` e.g in `~/.zshrc` with `export PLAYDATE_SDK_PATH=$HOME/Developer/PlaydateSDK`
