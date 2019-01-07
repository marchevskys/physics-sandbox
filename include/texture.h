#ifndef TEXTURE_H
#define TEXTURE_H

class Texture {
    unsigned int m_textureID;
    int m_width, m_height, m_channels;

  public:
    Texture(const char *path);
    ~Texture();
    void use();
};

#endif // TEXTURE_H
