#pragma once

#include "myGlew.h"
#include <fstream>

class IOException : public std::runtime_error
{
public:
    IOException(const std::string & msg) :
        std::runtime_error(msg) { }
};


class TextureReader
{
public:
    TextureReader();
    ~TextureReader();
    GLubyte * read(const char * fName, /*out*/ int & width, /*out*/ int & height);
    GLuint TextureReader::loadTex(const char* fName, GLint & width, GLint &height);
    int readShort(std::ifstream&);
};

