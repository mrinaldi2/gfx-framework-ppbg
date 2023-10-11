#include "core/gpu/image.h"

#include <vector>

using namespace gfxc;

void Image::Init (unsigned int width,
    unsigned int height, unsigned int channels)
{
    if (channels > 3) {
        printf ("Eroare!!!");
    }

    this->width = width;
    this->height = height;
    this->channels = channels;

    unsigned int size = width * height * channels;

    imageData = new unsigned char[size];
    std::memset(imageData, 0, size);

    Create(imageData, width, height, channels);

    glBindTexture(GL_TEXTURE_2D, GetTextureID());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void Image::UpdateInternalData()
{
    UploadNewData(imageData);
}

void Image::Set(unsigned int row, unsigned int column, glm::vec3 data)
{
    for (int channelIndex = 0; channelIndex < channels; channelIndex++) {
        imageData[
            (width * row + column) * channels +
            channelIndex
        ] = data [channelIndex] * 255;
    }
}

glm::vec3 Image::Get(unsigned int row, unsigned int column)
{
    glm::vec3 data(0);

    for (int channelIndex = 0; channelIndex < channels; channelIndex++) {
        data[channelIndex] = imageData[
            (width * row + column) * channels + channelIndex
        ] / 255.0f;
    }

    return data;
}
