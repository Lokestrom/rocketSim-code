#include "Text.hpp"

#include <ft2build.h>
#include  <freetype/ftglyph.h>
#include  <freetype/freetype.h>
#include <freetype/ftbbox.h>
#include <freetype/ftadvanc.h>

#include <filesystem>
#include <iostream>

#include "App.hpp"
#include "windowFunctions/windowFunctions.hpp"
#include "windowFunctions/helpers/TextInputPopup.hpp"
#include FT_FREETYPE_H

std::unordered_map<char, std::shared_ptr<CharacterGlyph>> CharacterGlyphCache::_cachedCharacters;
std::string CharacterGlyphCache::_fontFolder;
std::string CharacterGlyphCache::_font;

CharacterGlyph::CharacterLayout& CharacterGlyph::CharacterLayout::operator=(CharacterLayout&& other) noexcept
{
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
        Err = 1;
    }
    return Err;
}

CharacterGlyph& CharacterGlyph::operator=(CharacterGlyph&& other) noexcept
{
    _characterLayout = std::move(other._characterLayout);
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
    if (GetOutLine(glyph, &Outg))
        Warning("Font not suported, font" + font + "\n glyph is not in outline format", Warning::Type::Text, setFont, setDefaultFont);
    FT_Outline& FToutline = Outg->outline;

    _characterLayout.horiAdvance = face->glyph->metrics.horiAdvance;
    _characterLayout.lineSpacing = face->height;
    FT_BBox bbBox;
    FT_Outline_Get_BBox(&FToutline, &bbBox);
    _characterLayout.bb.bottom = -bbBox.yMax;
    _characterLayout.bb.top = -bbBox.yMin;
    _characterLayout.bb.right = bbBox.xMax;
    _characterLayout.bb.left = bbBox.xMin;

    if (FToutline.n_points == 0) {
        _characterLayout.hardVertecies = {};
        _characterLayout.becierVertecies = {};
        return;
    }
    
    struct Point
    {
        glm::vec2 cord;
        char tag;
        short contour;

        Point(float x, float y, char tag, short contour) : cord(x, -y), tag(tag), contour(contour) {}
    };

    std::vector<Point> outline;
    outline.reserve(FToutline.n_points);

    short currentContour = 0;
    short currentContourStart = 0;
    for (auto i = 0; i < FToutline.n_points; i++) {
        outline.emplace_back(FToutline.points[i].x, FToutline.points[i].y, FToutline.tags[i], currentContour);
        if (FToutline.tags[i] == FT_CURVE_TAG_CONIC && FToutline.tags[((i) % FToutline.contours[currentContour]) ? (i + 1) : currentContourStart] == FT_CURVE_TAG_CONIC) {
            outline.emplace_back((FToutline.points[i].x + FToutline.points[((i) % FToutline.contours[currentContour]) ? (i + 1) : currentContourStart].x)/2, 
                ((FToutline.points[i].y + FToutline.points[((i) % FToutline.contours[currentContour]) ? (i + 1) : currentContourStart].y) / 2), 
                FT_CURVE_TAG_ON, currentContour);
        }
        if (FToutline.contours[currentContour] == i) {
            FToutline.contours[currentContour] = outline.size() - 1;
            currentContour++;
            currentContourStart = i+1;
        }
    }

    _characterLayout.hardVertecies.emplace_back(1.f / 65.f, 1.f / 65.f);
    currentContour = 0;
    currentContourStart = 0;
    for (auto i = 0; i < outline.size(); i++) {
        if (outline[i].contour != currentContour) {
            _characterLayout.hardVertecies.emplace_back(outline[currentContourStart].cord);
            _characterLayout.hardVertecies.emplace_back(1.f / 65.f, 1.f / 65.f);
            currentContour++;
            currentContourStart = i;
        }
        if (outline[i].tag == FT_CURVE_TAG_ON)
            _characterLayout.hardVertecies.emplace_back(outline[i].cord);
        else {
            _characterLayout.becierVertecies.emplace_back(outline[i - 1].cord);
            _characterLayout.becierVertecies.emplace_back(0,0);
            _characterLayout.becierVertecies.emplace_back(outline[i].cord);
            _characterLayout.becierVertecies.emplace_back(.5, 0);
            _characterLayout.becierVertecies.emplace_back(outline[((i)%FToutline.contours[currentContour]) ? (i+1) : currentContourStart].cord);
            _characterLayout.becierVertecies.emplace_back(1, 1);
        }
    }
    _characterLayout.hardVertecies.emplace_back(outline[currentContourStart].cord);
    _characterLayout.hardVertecies.emplace_back(1.f / 65.f, 1.f / 65.f);
}

void CharacterGlyphCache::cacheCharacter(char character)
{
    std::shared_ptr<CharacterGlyph> newChar = std::make_shared<CharacterGlyph>();
    newChar->createCharacter(character, _fontFolder + _font);

    _cachedCharacters[character] = newChar;
}

void CharacterGlyphCache::deCacheCharacter(char character)
{
    _cachedCharacters.erase(character);
}

void CharacterGlyphCache::clear()
{
    _cachedCharacters.clear();
}

void CharacterGlyphCache::setFont(std::string font)
{
    assert(!_fontFolder.empty() && "Font folder must be set before font");
    if(font.empty())
        for(auto& file : std::filesystem::directory_iterator(_fontFolder))
            if (file.path().extension() == ".ttf" || file.path().extension() == ".TTF") {
                font = file.path().stem().string();
            }
    assert(!font.empty() && "font folder must contain a ttf file. fail in the setfontfolderpath function");
    font += ".ttf";
    if (!std::filesystem::exists(_fontFolder + font))
        Warning("Font not in folder\nfont:" + font + "\n font folder: " + _fontFolder, Warning::Type::BadInput, ::setFont, setDefaultFont);

    clear();
    _font = std::move(font);
    resetText();
}

void CharacterGlyphCache::setFontFolder(std::string fontFolderPath)
{
    if (fontFolderPath.empty()) {
#ifdef _WIN32
        _fontFolder = "C:\\Windows\\Fonts\\"; 
        if (!std::filesystem::exists(_fontFolder))
            Warning("The folder: \"C:\\Windows\\Fonts\\\", does not exist on your windows\n Set a font folder path.", Warning::Type::Text, ::setFontFolder);
#else
        Warning("OS not fully suported.\nSet a font folder path.", Warning::Type::text, ::setFontFolder);
#endif // _WIN32
    }
    else if (!std::filesystem::exists(fontFolderPath))
        Warning("The folder: \"" + fontFolderPath + "\" does not exist", Warning::Type::Text, ::setFontFolder, setDefaultFontFolder);
    else
        _fontFolder = fontFolderPath;

    bool hasFontFile = false;
    for (auto& file : std::filesystem::directory_iterator(_fontFolder)) {
        if (file.path().extension() == ".ttf" || file.path().extension() == ".TTF") {
            hasFontFile = true;
        }
    }
    if (!hasFontFile)
        Warning("The folder: \"" + _fontFolder + "\" does not contain any suported font files.\nSelect a diffrent folder.", Warning::Type::Text, ::setFontFolder, setDefaultFontFolder);

    setFont("");
}

std::shared_ptr<CharacterGlyph> CharacterGlyphCache::getCharacterGlyph(char character)
{
    if (!_cachedCharacters.count(character))
        cacheCharacter(character);
    return _cachedCharacters[character];
}

void restartText() noexcept {
    for (auto& [_, window] : Vulkan::getWindows())
        for (auto& [_, text] : window->staticTexts)
            text->restart();
}

std::vector<vk::VertexInputBindingDescription> StaticText::Vertex::getBindingDescriptions() {
    std::vector<vk::VertexInputBindingDescription> bindingDescriptions(1);
    bindingDescriptions[0].binding = 0;
    bindingDescriptions[0].stride = sizeof(Vertex);
    bindingDescriptions[0].inputRate = vk::VertexInputRate::eVertex;
    return bindingDescriptions;
}

std::vector<vk::VertexInputAttributeDescription> StaticText::Vertex::getAttributeDescriptions() {
    std::vector<vk::VertexInputAttributeDescription> attributeDescriptions{};

    attributeDescriptions.push_back({ 0, 0, vk::Format::eR32G32Sfloat, offsetof(Vertex, position) });

    return attributeDescriptions;
}

std::vector<vk::VertexInputBindingDescription> StaticText::BecierVertex::getBindingDescriptions()
{
    std::vector<vk::VertexInputBindingDescription> bindingDescriptions(1);
    bindingDescriptions[0].binding = 0;
    bindingDescriptions[0].stride = sizeof(BecierVertex);
    bindingDescriptions[0].inputRate = vk::VertexInputRate::eVertex;
    return bindingDescriptions;
}

std::vector<vk::VertexInputAttributeDescription> StaticText::BecierVertex::getAttributeDescriptions()
{
    std::vector<vk::VertexInputAttributeDescription> attributeDescriptions{};

    attributeDescriptions.push_back({ 0, 0, vk::Format::eR32G32Sfloat, offsetof(BecierVertex, position) });
    attributeDescriptions.push_back({ 1, 0, vk::Format::eR32G32Sfloat, offsetof(BecierVertex, becierPos) });

    return attributeDescriptions;
}

std::shared_ptr<StaticText> StaticText::createText(WindowInfo& window, glm::vec2 pos, glm::vec4 color, float scale, std::string text, Alignment alignment) {
    static id_t currentId = 0;
    window.staticTexts.insert({ currentId, std::make_shared<StaticText>(StaticText{ *window.device, currentId, pos, color, scale, text, alignment }) });
    return window.staticTexts[currentId++];
}

void StaticText::assignText(std::string text)
{
    _characters.clear();
    _characters.reserve(text.size());

    if (text.length() == 0)
        return;

    glm::vec2 nextCharacterPos = { 0,0 };
    glm::vec2 textSize = {0,0};
    Character newChar;
    for (auto& c : text) {
        if (c != '\n') {
            newChar.pos = nextCharacterPos;
            newChar.characterGlyph = CharacterGlyphCache::getCharacterGlyph(c);
            _characters.push_back(newChar);
            nextCharacterPos.x += newChar.characterGlyph->getCharacterlayout().horiAdvance;
        }
        else {
            textSize.x = std::max(nextCharacterPos.x, textSize.x);
            nextCharacterPos.x = 0;
            nextCharacterPos.y += newChar.characterGlyph->getCharacterlayout().lineSpacing;
        }
    }

    //sentrerer text positionen
    textSize.x = std::max(nextCharacterPos.x, textSize.x);
    textSize.y = -(nextCharacterPos.y + newChar.characterGlyph->getCharacterlayout().lineSpacing/2);
    textSize.y /= 2;
	if (_alignment == Alignment::left)
		textSize.x = 0;
	else if (_alignment == Alignment::right)
		textSize.x = -textSize.x;
    else
        textSize.x /= 2;

    for (auto& i : _characters) {
        i.pos -= textSize;
    }

    createVertexBuffer();
}

void StaticText::removeText()
{
    _characters.clear();
    _characters.shrink_to_fit();
}

void StaticText::changeColor(const glm::vec4& newColor)
{
    _color = newColor;
}

void StaticText::bindHard(vk::CommandBuffer commandBuffer)
{
    vk::Buffer buffers[] = { _hardVertexBuffer->getBuffer() };
    vk::DeviceSize offsets[] = { 0 };
    commandBuffer.bindVertexBuffers(0, 1, buffers, offsets);
}

void StaticText::bindBecier(vk::CommandBuffer commandBuffer)
{
    assert(_becierVertexCount != 0);
    vk::Buffer buffers[] = { _becierVertexBuffer->getBuffer() };
    vk::DeviceSize offsets[] = { 0 };
    commandBuffer.bindVertexBuffers(0, 1, buffers, offsets);
}

void StaticText::drawHard(vk::CommandBuffer commandBuffer)
{
    commandBuffer.draw(_hardVertexCount, 1, 0, 0);
}

void StaticText::drawBecier(vk::CommandBuffer commandBuffer)
{
    assert(_becierVertexCount != 0);
    commandBuffer.draw(_becierVertexCount, 1, 0, 0);
}

void StaticText::restart()
{
    assignText("");
}

void StaticText::drawBox(vk::CommandBuffer commandBuffer)
{
    commandBuffer.draw(6, 1, _hardVertexCount, 0);
}

bool StaticText::hasBecier()
{
    return _becierVertexCount != 0;
}

void StaticText::createVertexBuffer()
{
    Vertex vertex;
    std::vector<Vertex> hardVertexBuffer;
    std::vector<BecierVertex> becierVertexBuffer;
    CharacterGlyph::BoundingBox textbb = { 0,0,0,0 };

    for (auto& i : _characters){
        textbb.top = std::max(textbb.top, i.pos.y + i.characterGlyph->getCharacterlayout().bb.top);
        textbb.bottom = std::min(textbb.bottom, i.pos.y + i.characterGlyph->getCharacterlayout().bb.bottom);
        textbb.right = std::max(textbb.right, i.pos.x + i.characterGlyph->getCharacterlayout().bb.right);
        textbb.left = std::min(textbb.left, i.pos.x - i.characterGlyph->getCharacterlayout().bb.left);
        hardVertexBuffer.reserve(hardVertexBuffer.size() + i.characterGlyph->getCharacterlayout().hardVertecies.size());
        for (auto& j : i.characterGlyph->getCharacterlayout().hardVertecies) {
            if (j.x == 1.f / 65.f && j.y == 1.f / 65.f) {
                hardVertexBuffer.push_back({ j });
                continue;
            }
            hardVertexBuffer.push_back({ i.pos + j });
        }
        becierVertexBuffer.reserve(becierVertexBuffer.size() + i.characterGlyph->getCharacterlayout().becierVertecies.size());
        for (auto j = 0; j < i.characterGlyph->getCharacterlayout().becierVertecies.size(); j+=2) {
            becierVertexBuffer.push_back({ i.characterGlyph->getCharacterlayout().becierVertecies[j] + i.pos,
                i.characterGlyph->getCharacterlayout().becierVertecies[j + 1] });
        }
    }
    assert((becierVertexBuffer.size() % 3 == 0) && "must be a multiple of 3");
    _hardVertexCount = hardVertexBuffer.size();
    _becierVertexCount = becierVertexBuffer.size();
    hardVertexBuffer.reserve(hardVertexBuffer.size() + 6);
    hardVertexBuffer.push_back({ { textbb.left, textbb.top } });
    hardVertexBuffer.push_back({ { textbb.right, textbb.top } });
    hardVertexBuffer.push_back({ { textbb.right, textbb.bottom } });
    hardVertexBuffer.push_back({ { textbb.left, textbb.top } });
    hardVertexBuffer.push_back({ { textbb.left, textbb.bottom } });
    hardVertexBuffer.push_back({ { textbb.right, textbb.bottom } });

    vk::DeviceSize bufferSize = sizeof(hardVertexBuffer[0]) * hardVertexBuffer.size();
    
    _device.device().waitIdle();

    Buffer stagingBuffer{
        _device,
        sizeof(hardVertexBuffer[0]),
        (uint32_t)hardVertexBuffer.size(),
        vk::BufferUsageFlagBits::eTransferSrc,
        vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
    };

    stagingBuffer.map();
    stagingBuffer.writeToBuffer((void*)hardVertexBuffer.data());

    _hardVertexBuffer = std::make_unique<Buffer>(
        _device,
        sizeof(hardVertexBuffer[0]),
        hardVertexBuffer.size(),
        vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst,
        vk::MemoryPropertyFlagBits::eDeviceLocal);

    _device.copyBuffer(stagingBuffer.getBuffer(), _hardVertexBuffer->getBuffer(), bufferSize);

    if (_becierVertexCount == 0)
        return;

    bufferSize = sizeof(becierVertexBuffer[0]) * becierVertexBuffer.size();

    Buffer stagingBuffera{
    _device,
    sizeof(becierVertexBuffer[0]),
    (uint32_t)becierVertexBuffer.size(),
    vk::BufferUsageFlagBits::eTransferSrc,
    vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
    };

    stagingBuffera.map();
    stagingBuffera.writeToBuffer((void*)becierVertexBuffer.data());

    _becierVertexBuffer = std::make_unique<Buffer>(
        _device,
        sizeof(becierVertexBuffer[0]),
        becierVertexBuffer.size(),
        vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst,
        vk::MemoryPropertyFlagBits::eDeviceLocal);

    _device.copyBuffer(stagingBuffera.getBuffer(), _becierVertexBuffer->getBuffer(), bufferSize);
}

void resetText() {
    for (const auto& [_, window] : Vulkan::getWindows()) {
        for (const auto& [_, text] : window->staticTexts) {
            text->restart();
        }
    }
}

void setFont(WindowInfo& window)
{
    auto* info = (windows::TextInputPopup::Info*)windows::createInfo(windows::Type::TextInputPopup);
    info->what = "Set font.\nIf left empty a default font wil be set.";
    info->returnFunc = CharacterGlyphCache::setFont;
    windows::createWindow(windows::Type::TextInputPopup, info);
}

void setDefaultFont(WindowInfo& window)
{
    CharacterGlyphCache::setFont("");
}

void setFontFolder(WindowInfo& window)
{
    auto* info = (windows::TextInputPopup::Info*)windows::createInfo(windows::Type::TextInputPopup);
    info->what = "Set font folder";
    info->returnFunc = CharacterGlyphCache::setFontFolder;
    windows::createWindow(windows::Type::TextInputPopup, info);
}

void setDefaultFontFolder(WindowInfo& window)
{
    CharacterGlyphCache::setFontFolder("");
}

std::shared_ptr<VaryingText> VaryingText::createText(WindowInfo& window, glm::vec2 pos, glm::vec4 color, float scale, const std::string& text, StaticText::Alignment alignment) {
    std::shared_ptr<VaryingText> vtext = std::make_shared<VaryingText>(VaryingText{ StaticText::createText(window, pos, color, scale, "", alignment), text});
    window.varyingTexts.insert({ vtext->getId(), vtext });
    return window.varyingTexts[vtext->getId()];
}

void VaryingText::VaryingTextString::assignText(std::string text)
{
    _planeText = text;
}

std::string VaryingText::VaryingTextString::getString()
{
    std::string ans;
    ans.reserve(_planeText.size() + (_variable.size() * 5));

    auto planeStringIndex = 0;
    auto variableIndex = 0;
    while (planeStringIndex < _planeText.size()) {
        if (_variable[variableIndex].index == planeStringIndex) {
            ans += _variable[variableIndex].get();
            variableIndex++;
        }
        else {
            if (planeStringIndex == _planeText.size())
                OutOfRange("There are variables out of range");
            ans.push_back(_planeText[planeStringIndex]);
            planeStringIndex++;
        }
    }
    if (_variable[variableIndex].index == planeStringIndex) {
        ans += _variable[variableIndex].get();
        variableIndex++;
    }
    return ans;
}

void VaryingText::update()
{
    _textObj->assignText(_text.getString());
}

void VaryingText::assignText(std::string text)
{
    _text.assignText(text);
}

StaticText::id_t VaryingText::getId()
{
    return _textObj->getId();
}
std::shared_ptr<StaticText> VaryingText::staticText()
{
    return _textObj;
}

std::string VaryingText::VaryingTextString::Variable::get()
{
    switch (type) {
    case Type::Int:
        return std::to_string(*static_cast<int*>(variable));
    case Type::Float:
        return std::to_string(*static_cast<float*>(variable));
	case Type::Double:
		return std::to_string(*static_cast<double*>(variable));
    case Type::LongDouble:
        return std::to_string(*static_cast<long double*>(variable));
    case Type::StdString:
        return *static_cast<std::string*>(variable);
    case Type::String:
        return toSTD(*static_cast<Database::String*>(variable));
    case Type::Vec2:
        return std::format("{{{:.3f}, {:.3f}}}", static_cast<Vector2*>(variable)->x, static_cast<Vector2*>(variable)->y);
    case Type::Vec3:
        return std::format("{{{:.3f}, {:.3f}, {:.3f}}}", static_cast<Vector3*>(variable)->x, static_cast<Vector3*>(variable)->y, static_cast<Vector3*>(variable)->z);
    case Type::Quaternion:
        return std::format("{{{:.3f}, {:.3f}, {:.3f}, {:.3f}}}", static_cast<::Quaternion*>(variable)->x, static_cast<::Quaternion*>(variable)->y, static_cast<::Quaternion*>(variable)->z, static_cast<::Quaternion*>(variable)->w);
    }

	assert(false && "VaryingText::VaryingTextString::Variable::get: No specialization for this type");
}