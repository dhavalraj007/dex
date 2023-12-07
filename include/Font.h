//
// Created by Dhava on 20-11-2023.
//

#ifndef DEEX_FONT_H
#define DEEX_FONT_H

#include "msdf-atlas-gen/FontGeometry.h"
#include "texture.h"

struct Font {
    std::vector <msdf_atlas::GlyphGeometry> glyphs;
    msdf_atlas::FontGeometry fontGeometry;
    std::unique_ptr<Texture> texture;

    Font(const std::filesystem::path& path);
};
#endif //DEEX_FONT_H
