#include <iostream>

/*
 * Target interface
 * */
class Media {
public:
  virtual void ready(void) const = 0;
};

class Mp3 : public Media {
public:
  void ready(void) const {
    std::cout << "Playing mp3" << std::endl;
  }
};

/*
 * Client
 * */
class MediaPlayer {
public:
  void play(const Media &media) {
    media.ready();
  }
};

/*
 * Adaptee: unacceptable by MediaPlayer
 * */
class AdvancedMedia {
public:
  virtual void ready(void) const = 0;
};

class Vlc : public AdvancedMedia {
public:
  void ready(void) const override {
    std::cout << "Playing vlc" << std::endl;
  }
};

class Mp4 : public AdvancedMedia {
public:
  void ready(void) const override {
    std::cout << "Playing mp4" << std::endl;
  }
};

/*
 * Class adapter
 * */
class Mp4Adapter : public Media, private Mp4 {
public:
  void ready(void) const override {
    /* Override MediaPlayer::play() */
    Mp4::ready();
  }
};

/*
 * Object adapter: more flexible
 * */
class MediaAdapter : public Media {
  AdvancedMedia &amp;

public:
  MediaAdapter(AdvancedMedia &amp) : amp(amp) {}

  void ready(void) const override {
    amp.ready();
  }
};

int main(void) {
  MediaPlayer player;

  player.play(Mp3());

  /* Incompatible */
  // player.play(Mp4());
  // player.play(Vlc());

  Mp4 mp4;
  Vlc vlc;

  player.play(Mp4Adapter());
  player.play(MediaAdapter(mp4));
  player.play(MediaAdapter(vlc));

  return 0;
}