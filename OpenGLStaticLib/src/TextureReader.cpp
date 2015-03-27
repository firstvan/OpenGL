#include "TextureReader.h"


TextureReader::TextureReader()
{
}


TextureReader::~TextureReader()
{
}

int TextureReader::readShort(std::ifstream & stream)
{
    unsigned char ic;     // input char
    int result;           // result

    ic = stream.get();
    result = ic;
    ic = stream.get();
    result |= ((unsigned int)ic << 8);
    return result;
}

GLubyte * TextureReader::read(const char * fName, /*out*/ int & width, /*out*/ int & height)
{
    // Open file for reading
    std::ifstream inFile(fName, std::ios::binary);

    try
    {
        if (!inFile)
        {
            std::string msg = std::string("Error: can't open ") + fName;
            throw IOException(msg);
        }

        int idLen = inFile.get();      // Length of image ID field
        int mapType = inFile.get();    // Color map type (expect 0 == no color map)
        int typeCode = inFile.get();   // Image type code (expect 2 == uncompressed)
        inFile.ignore(5);              // Color map info (ignored)
        int xOrigin = readShort(inFile);  // X origin
        int yOrigin = readShort(inFile);  // Y origin
        width = readShort(inFile);        // Image width
        height = readShort(inFile);       // Image height
        int bpp = inFile.get();               // Bits per pixel (expect 24 or 32)
        inFile.ignore();                      // Image descriptor (expect 0 for 24bpp and 8 for 32bpp)

        if (typeCode != 2 || mapType != 0)
        {
            throw IOException("File does not appear to be a non-color-mapped, uncompressed TGA image");
        }

        if (bpp != 24 && bpp != 32)
        {
            throw IOException("File must be 24 or 32 bits per pixel.\n");
        }

        // Skip the image ID data
        if (idLen > 0) inFile.ignore(idLen);

        // Color map data would be here, but we assume no color map

        printf("%s: (%d x %d) %d bpp origin (%d, %d)\n", fName, width, height, bpp,
               xOrigin, yOrigin);

        // Read pixel data
        GLubyte *p = new GLubyte[width * height * 4];
        // 24 bpp -- Blue, Green, Red
        // 32 bpp -- Blue, Green, Red, Alpha
        // p -- stored as RGBA
        for (unsigned int i = 0; i < (unsigned int)(width * height); i++)
        {
            p[i * 4 + 2] = inFile.get();  // Blue
            p[i * 4 + 1] = inFile.get();  // Green
            p[i * 4] = inFile.get();  // Red
            if (bpp == 32)
                p[i * 4 + 3] = inFile.get();
            else
                p[i * 4 + 3] = 0xFF;
        }

        inFile.close();
        return p;
    }
    catch (IOException & e)
    {
        inFile.close();
        throw e;
    }
}

GLuint TextureReader::loadTex(const char* fName, GLint & width, GLint &height)
{
    GLubyte * data = read(fName, width, height);

    GLuint texID;
    glGenTextures(1, &texID);

    glBindTexture(GL_TEXTURE_2D, texID);

    // Allocate storage
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, width, height);
    // Copy data into storage
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    delete[] data;

    return texID;
}
