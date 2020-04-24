class CBuffer {
  private:
    unsigned long pointer;
    unsigned long consumed;
    long buf[3];

  public:
    CBuffer();
    bool add(long val);
    long avg();
};
