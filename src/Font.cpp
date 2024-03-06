//
// Created by Dhava on 20-11-2023.
//
#include "Font.h"
#include "log.h"
#include "msdf-atlas-gen/Charset.h"
#include "msdf-atlas-gen/msdf-atlas-gen.h"
#include "stb_image_write.h"

Font::Font(const std::filesystem::path &path)
{
    // Initialize instance of FreeType library
    msdfgen::FreetypeHandle *ft = msdfgen::initializeFreetype();
    if (!ft)
    {
        LOG("Error: cant init freetype");
        return;
    }

    std::string filePath = path.string();
    // Load font file
    msdfgen::FontHandle *font = msdfgen::loadFont(ft, filePath.c_str());
    if (!font)
    {
        LOG("Error: cant load font");
        return;
    }

    // Storage for glyph geometry and their coordinates in the atlas
    // std::vector<msdf_atlas::GlyphGeometry> glyphs;
    // FontGeometry is a helper class that loads a set of glyphs from a single font.
    // It can also be used to get additional font metrics, kerning information, etc.
    fontGeometry = msdf_atlas::FontGeometry(&glyphs);
    // Load a set of character glyphs:
    // The second argument can be ignored unless you mix different font sizes in one atlas.
    // In the last argument, you can specify a charset other than ASCII.
    // To load specific glyph indices, use loadGlyphs instead.
    //    msdf_atlas::Charset charset;
    //    charset.add('A');
    //    fontGeometry.loadCharset(font, 1.0, charset);
    msdf_atlas::Charset charset(msdf_atlas::Charset::ASCII);
    int loaded = fontGeometry.loadCharset(font, 1.0, msdf_atlas::Charset::ASCII);
    LOG("loaded chars: {}", loaded);
    LOG("glyph for \\n loaded? : {}", (std::size_t)fontGeometry.getGlyph('\n'));

    // Apply MSDF edge coloring. See edge-coloring.h for other coloring strategies.
    const double maxCornerAngle = 3.0;
    for (msdf_atlas::GlyphGeometry &glyph : glyphs)
        glyph.edgeColoring(&msdfgen::edgeColoringInkTrap, maxCornerAngle, 0);

    // TightAtlasPacker class computes the layout of the atlas.
    msdf_atlas::TightAtlasPacker packer;
    // Set atlas parameters:
    // setDimensions or setDimensionsConstraint to find the best value
    packer.setDimensionsConstraint(msdf_atlas::TightAtlasPacker::DimensionsConstraint::POWER_OF_TWO_RECTANGLE);
    // setScale for a fixed size or setMinimumScale to use the largest that fits
    packer.setScale(40.0);
    packer.setPadding(0);
    // setPixelRange or setUnitRange
    packer.setPixelRange(2.0);
    packer.setMiterLimit(1.0);
    // Compute atlas layout - pack glyphs
    packer.pack(glyphs.data(), glyphs.size());
    // Get final atlas dimensions
    int width = 0, height = 0;
    packer.getDimensions(width, height);
    // The ImmediateAtlasGenerator class facilitates the generation of the atlas bitmap.
    msdf_atlas::ImmediateAtlasGenerator<
        float,                      // pixel type of buffer for individual glyphs depends on generator function
        3,                          // number of atlas color channels
        &msdf_atlas::msdfGenerator, // function to generate bitmaps for individual glyphs
        msdf_atlas::BitmapAtlasStorage<msdf_atlas::byte, 3> // class that stores the atlas bitmap
        // For example, a custom atlas storage class that stores it in VRAM can be used.
        >
        generator(width, height);
    // GeneratorAttributes can be modified to change the generator's default settings.
    msdf_atlas::GeneratorAttributes attributes;
    generator.setAttributes(attributes);
    generator.setThreadCount(8);
    // Generate atlas bitmap
    generator.generate(glyphs.data(), glyphs.size());
    // The atlas bitmap can now be retrieved via atlasStorage as a BitmapConstRef.
    // The glyphs array (or fontGeometry) contains positioning data for typesetting text.
    // success = myProject::submitAtlasBitmapAndLayout(generator.atlasStorage(), glyphs);
    msdfgen::destroyFont(font);
    msdfgen::deinitializeFreetype(ft);
    auto bitmapRef = (msdfgen::BitmapConstRef<uint8_t, 3>)generator.atlasStorage();
    Texture::TextureSpecs specs(bitmapRef.width, bitmapRef.height, GL_RGB, Texture::TextureFiltering::Linear,
                                Texture::TextureFiltering::Linear);
    // stbi_write_bmp("test.bmp", bitmapRef.width, bitmapRef.height, 3, bitmapRef.pixels);
    this->texture = std::make_unique<Texture>(specs, 1);
    texture->LoadTexture(bitmapRef.pixels);
}
bool Font::hasGlyph(msdfgen::unicode_t codepoint) const
{
    return this->fontGeometry.getGlyph(codepoint) != nullptr;
}
