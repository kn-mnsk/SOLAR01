#pragma once

#ifndef STB_HELPER_H
#define STB_HELPER_H

#ifndef STB_IMAGE_STATIC
#define STB_IMAGE_STATIC
#endif

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif

#ifndef STB_IMAGE_WRITE_STATIC
#define STB_IMAGE_WRITE_STATIC
#endif

#ifndef STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#endif


#ifndef STBI_ONLY_PNG
#define STBI_ONLY_PNG
#endif

#ifndef STBI_ONLY_JPEG
#define STBI_ONLY_JPEG
#endif

#ifndef STBI_ONLY_BMP
#define STBI_ONLY_BMP
#endif

#ifndef STBI_ONLY_GIF
#define STBI_ONLY_GIF
#endif

#ifndef STBI_ONLY_TGA
#define STBI_ONLY_TGA
#endif

#ifndef STBI_ONLY_HDR
#define STBI_ONLY_HDR
#endif

//#include "stb_image.h"
#include <stb_image/stb_image.h>
//#include "stb_image_write.h"
#include <stb_image/stb_image_write.h>

#endif