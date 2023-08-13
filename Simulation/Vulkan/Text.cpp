#include "Text.hpp"

#include <ft2build.h>
#include  <freetype/ftglyph.h>
#include  <freetype/freetype.h>
#include <freetype/ftbbox.h>
#include <freetype/ftadvanc.h>
#include FT_FREETYPE_H

std::unordered_map<char, std::shared_ptr<CharacterGlyph>> CharacterGlyphCache::_cachedCharacters;
std::string CharacterGlyphCache::_fontPath;

CharacterGlyph::Contour& CharacterGlyph::Contour::operator=(const Contour&& other)
{
    points = std::move(other.points);
    flags = std::move(other.flags);
    return *this;
}

CharacterGlyph::CharacterLayout& CharacterGlyph::CharacterLayout::operator=(const CharacterLayout&& other)
{
    contours = std::move(other.contours);
    return *this;
}

int GetOutLine(FT_Glyph glyph, FT_OutlineGlyph* Outg)
{
    int Err = 0;

    switch (glyph->format)
    {
    case FT_GLYPH_FORMAT_BITMAP:
        Err = 1;
        break;

    case FT_GLYPH_FORMAT_OUTLINE:
        *Outg = (FT_OutlineGlyph)glyph;
        break;

    default:
        ;
    }
    return Err;
}

CharacterGlyph& CharacterGlyph::operator=(const CharacterGlyph&& other)
{
    characterLayout = std::move(characterLayout);
    return *this;
}

void CharacterGlyph::createCharacter(char character, std::string font)
{
    FT_Face face;
    FT_Library library;
    FT_Error error;

    FT_Init_FreeType(&library);
    error = FT_New_Face(library, font.c_str(), 0, &face);

    FT_Set_Char_Size(face, 1, 1, 72, 72);

    FT_Int32  load_flags = FT_LOAD_NO_SCALE;
    FT_Load_Glyph(face, FT_Get_Char_Index(face, character), load_flags);

    FT_Glyph glyph;
    FT_Get_Glyph(face->glyph, &glyph);

    FT_OutlineGlyph Outg;
    GetOutLine(glyph, &Outg);
    FT_Outline& outline = Outg->outline;

    characterLayout.horiAdvance = face->glyph->metrics.horiAdvance;
    characterLayout.lineSpacing = face->height;
    int currContour = 0;
    FT_BBox bbBox;
    FT_Outline_Get_BBox(&outline, &bbBox);
    characterLayout.bb.top = bbBox.yMax;
    characterLayout.bb.bottom = bbBox.yMin;
    characterLayout.bb.right = bbBox.xMax;
    characterLayout.bb.left = bbBox.xMin;
    characterLayout.contours.reserve(outline.n_contours);
    std::shared_ptr<Contour> contour = std::make_shared<Contour>();
    for (int i = 0; i < outline.n_points; i++) {
        contour->points.push_back({ outline.points[i].x, -outline.points[i].y });
        switch (FT_CURVE_TAG(outline.tags[i]))
        {
        case FT_CURVE_TAG_ON:
            contour->flags.push_back(CurveTag::on);
            break;
        case FT_CURVE_TAG_CONIC:
            contour->flags.push_back(CurveTag::conic);
            break;
        case FT_CURVE_TAG_CUBIC:
            contour->flags.push_back(CurveTag::cubic);
            break;
        default:
            break;
        }
        if (i == outline.contours[currContour]) {
            characterLayout.contours.push_back(contour);
            contour = std::make_shared<Contour>();
            currContour++;
        }
    }
}

void CharacterGlyphCache::cacheCharacter(char character)
{
    std::shared_ptr<CharacterGlyph> newChar = std::make_shared<CharacterGlyph>();
    newChar->createCharacter(character, _fontPath);

    _cachedCharacters[character] = newChar;
}

void CharacterGlyphCache::deCacheCharacter(char character)
{
    _cachedCharacters.erase(character);
}

void CharacterGlyphCache::clear()
{
    _cachedCharacters.clear();
    _fontPath.clear();
}

void CharacterGlyphCache::setFont(std::string fontPath)
{
    clear();
    _fontPath = fontPath;
}

std::shared_ptr<CharacterGlyph> CharacterGlyphCache::getCharacterGlyph(char character)
{
    if (!_cachedCharacters.count(character))
        cacheCharacter(character);
    return _cachedCharacters[character];
}

std::vector<vk::VertexInputBindingDescription> Text::Vertex::getBindingDescriptions() {
    std::vector<vk::VertexInputBindingDescription> bindingDescriptions(1);
    bindingDescriptions[0].binding = 0;
    bindingDescriptions[0].stride = sizeof(Vertex);
    bindingDescriptions[0].inputRate = vk::VertexInputRate::eVertex;
    return bindingDescriptions;
}

std::vector<vk::VertexInputAttributeDescription> Text::Vertex::getAttributeDescriptions() {
    std::vector<vk::VertexInputAttributeDescription> attributeDescriptions{};

    attributeDescriptions.push_back({ 0, 0, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, position) });
    attributeDescriptions.push_back({ 1, 0, vk::Format::eR32G32B32A32Sfloat, offsetof(Vertex, color) });

    return attributeDescriptions;
}

void Text::assignText(std::string text)
{
    Character newChar;
    _characters.clear();
    _characters.reserve(text.size());
    for (auto& c : text) {
        if (c != '\n') {
            newChar.pos = _nextCharacterPos;
            newChar.characterGlyph = CharacterGlyphCache::getCharacterGlyph(c);
            _characters.push_back(newChar);
            _nextCharacterPos.x += newChar.characterGlyph->getCharacterlayout().horiAdvance;
        }
        else {
            _nextCharacterPos.x = 0;
            _nextCharacterPos.y += newChar.characterGlyph->getCharacterlayout().lineSpacing;
        }
    }
}

void Text::removeText()
{
    _characters.clear();
    _characters.shrink_to_fit();
}

void Text::bind(vk::CommandBuffer commandBuffer)
{
    vk::Buffer buffers[] = { _vertexBuffer->getBuffer() };
    vk::DeviceSize offsets[] = { 0 };
    commandBuffer.bindVertexBuffers(0, 1, buffers, offsets);
}

void Text::draw(vk::CommandBuffer commandBuffer)
{
    commandBuffer.draw(_vertexCount, 1, 0, 0);
}

void Text::createVertexBuffer()
{
    Vertex vertex;
    std::vector<Vertex> vertices;
    glm::vec2 middle;
    CharacterGlyph::BoundingBox& textbb = _characters[0].characterGlyph->getCharacterlayout().bb;
    vertex.color = _color;
    for (const auto& c : _characters) {
        CharacterGlyph::BoundingBox& bb = c.characterGlyph->getCharacterlayout().bb;
        middle = { (bb.right - bb.left)/2, (-bb.top + bb.bottom)/2};
        for (const auto& contour : c.characterGlyph->getCharacterlayout().contours) {
            vertices.reserve(vertices.size() + contour->points.size());
            auto i = 0;
            for (;contour->flags[i] != CurveTag::on; i++);
            vertex.position = { contour->points[i] + c.pos, 0 };
            for (; i < contour->points.size(); i++) {
                if (contour->flags[i] != CurveTag::on)
                    continue;
                vertices.push_back(vertex);
                vertex.position = { c.pos + _pos + middle, 1};
                vertices.push_back(vertex);
                vertex.position = { contour->points[i] + c.pos, 0 };
                vertices.push_back(vertex);
            }
            i = 0;
            for (;contour->flags[i] != CurveTag::on; i++);
            vertices.push_back(vertex);
            vertex.position = { c.pos + _pos + middle, 1 };
            vertices.push_back(vertex);
            vertex.position = { contour->points[i] + c.pos, 0 };
            vertices.push_back(vertex);
        }
    }

    vk::DeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();
    uint32_t vertexSize = sizeof(vertices[0]);

    Buffer stagingBuffer{
        _device,
        vertexSize,
        (uint32_t)vertices.size(),
        vk::BufferUsageFlagBits::eTransferSrc,
        vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
    };

    stagingBuffer.map();
    stagingBuffer.writeToBuffer((void*)vertices.data());

    _vertexBuffer = std::make_unique<Buffer>(
        _device,
        vertexSize,
        (uint32_t)vertices.size(),
        vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst,
        vk::MemoryPropertyFlagBits::eDeviceLocal);
    _vertexCount = vertices.size();

    _device.copyBuffer(stagingBuffer.getBuffer(), _vertexBuffer->getBuffer(), bufferSize);

    vertices.clear();
    vertex.color = { 1,0,0,1 };
    vertex.position = { textbb.left, -textbb.bottom, 0.5 };
    vertices.push_back(vertex);
    vertex.position = { textbb.left, -textbb.top, 0.5 };
    vertices.push_back(vertex);
    vertex.position = { textbb.right, -textbb.top, 0.5 };
    vertices.push_back(vertex);
    vertex.position = { textbb.right, -textbb.top, 0.5 };
    vertices.push_back(vertex);
    vertex.position = { textbb.left, -textbb.bottom, 0.5 };
    vertices.push_back(vertex);
    vertex.position = { textbb.right, -textbb.bottom, 0.5 };
    vertices.push_back(vertex);
    bufferSize = sizeof(vertices[0]) * vertices.size();
    vertexSize = sizeof(vertices[0]);

    Buffer stagingBufferbox = {
        _device,
        vertexSize,
        (uint32_t)vertices.size(),
        vk::BufferUsageFlagBits::eTransferSrc,
        vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
    };

    stagingBufferbox.map();
    stagingBufferbox.writeToBuffer((void*)vertices.data());

    _boxBuffer = std::make_unique<Buffer>(
        _device,
        vertexSize,
        (uint32_t)vertices.size(),
        vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst,
        vk::MemoryPropertyFlagBits::eDeviceLocal);
    _device.copyBuffer(stagingBufferbox.getBuffer(), _boxBuffer->getBuffer(), bufferSize);
}

void Text::bindBox(vk::CommandBuffer commandBuffer)
{

}
