<h2 align="center">
  <br>
  <img width="256" src="https://cdn.rawgit.com/frabert/libdmusic/f7aeed1b/media/logo.svg">
  <br>
</h2>

libdmusic [![Linux build Status](https://travis-ci.org/frabert/libdmusic.svg?branch=master)](https://travis-ci.org/frabert/libdmusic) [![Windows build status](https://ci.appveyor.com/api/projects/status/7t7ral8wos4p7idc?svg=true)](https://ci.appveyor.com/project/frabert/libdmusic) [![Gitter chat](https://badges.gitter.im/frabert/libdmusic.png)](https://gitter.im/frabert/libdmusic)
=========

Project goals
-------------

`libdmusic` aims to provide a framework and suite of tools to enable applications and games playback of Microsoft DirectMusic audio files. The main aim of the project for now is to provide a way for the [REGoth project](https://github.com/REGoth-project/REGoth) to reproduce music files from the original Gothic and Gothic II games.

Roadmap
-------
- DLS to SoundFont conversion
- Realtime SoundFont playback
- Realtime DLS level 2 playback
- `.sgt` and `.sty` files loading

For now, the `dls2sf` utility does not support DLS articulators, so the envelopes will sound weird. Converting articulators is difficult because DLS level 1 (and especially level 2) specify many modulating sources and destinations which are not supported by the SoundFont 2 specification. In the long run, I hope to be able to implement a custom renderer that doesn't require DLS to SF conversion.

The `dmrender` utility is able to render an arbitrary amount of audio data from a segment. It requires that all the DLS instruments are converted into SoundFont2 files via the aforementioned `dls2sf` utility. It is still extremely experimental and is being heavily worked on.

Compiling
---------

````
$ git clone --recursive https://github.com/frabert/libdmusic
$ cd libdmusic
$ cmake .
$ make
````

Acknowledgements
----------------

`dmrender` uses [TinySoundFont](https://github.com/schellingb/TinySoundFont) for SoundFont rendering, [libsndfile](http://www.mega-nerd.com/libsndfile/) and [args](https://github.com/Taywee/args).

`dls2sf`, `sampledump` and `dmrender` use [libsndfile](http://www.mega-nerd.com/libsndfile/) for wave output and conversion.