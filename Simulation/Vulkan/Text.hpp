#pragma once
#include "Buffer.hpp"
#include "Device.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>


// std
#include <memory>
#include <vector>
#include <unordered_map>

#include "Exception.hpp"
#include "String.hpp"

enum class CurveTag {
    on,
    conic,
    cubic
};

class CharacterGlyph {
public:
    struct BoundingBox {
        double top, bottom;
        double right, left;
    };
    struct Contour {
        std::vector<glm::vec2> points;
        std::vector<CurveTag> flags;
        
        Contour() {}
        Contour(const Contour&) = delete;
        Contour& operator=(const Contour&) = delete;
        Contour& operator=(const Contour&& other);
    };
    struct CharacterLayout {
        std::vector<std::shared_ptr<Contour>> contours;
        BoundingBox bb;
        double horiAdvance;
        double lineSpacing;

        CharacterLayout() {}
        CharacterLayout(const CharacterLayout&) = delete;
        CharacterLayout& operator=(const CharacterLayout&) = delete;
        CharacterLayout& operator=(const CharacterLayout&& other);
    };

    CharacterGlyph() : characterLayout() {}
    ~CharacterGlyph(){}

    CharacterGlyph(const CharacterGlyph&) = delete;
    CharacterGlyph& operator=(const CharacterGlyph&) = delete;

    CharacterGlyph& operator=(const CharacterGlyph&& other);

    void createCharacter(char character, std::string font);
    
    CharacterLayout& getCharacterlayout() { return characterLayout; }

private:
    CharacterLayout characterLayout;
};

class CharacterGlyphCache {
public:
    static void cacheCharacter(char character);
    static void deCacheCharacter(char character);
    static void clear();
    static void setFont(std::string fontPath);
    static std::shared_ptr<CharacterGlyph> getCharacterGlyph(char character);
private:
    static std::unordered_map<char, std::shared_ptr<CharacterGlyph>> _cachedCharacters;
    static std::string _fontPath;
};

struct Character {
    glm::vec2 pos;
    std::shared_ptr<CharacterGlyph> characterGlyph;
};

class StaticText
{
public:
    using id_t = unsigned int;
    using Map = std::unordered_map<id_t, StaticText>;
    using MapRef = std::unordered_map<id_t, StaticText*>;

    struct Vertex {
        glm::vec3 position{};
        glm::vec4 color{};

        static std::vector<vk::VertexInputBindingDescription> getBindingDescriptions();
        static std::vector<vk::VertexInputAttributeDescription> getAttributeDescriptions();

        bool operator==(const Vertex& other) const {
            return position == other.position && color == other.color;
        }
    };

    static StaticText createText(Device& device, glm::vec2 pos, glm::vec4 color, float scale, std::string text = "") {
        static id_t currentId = 0;
        return StaticText{ device, currentId, pos, color, scale, text };
    }

    StaticText(const StaticText&) = delete;
    StaticText& operator=(const StaticText&) = delete;
    StaticText(StaticText&&) = default;
    StaticText& operator=(StaticText&&) = default;

    id_t getId() { return _id; }
    float getScale() { return _scale;  }
    glm::vec4 getPos() { return { _pos, 0, 0 }; }

    void assignText(std::string text);
    void removeText();

    void bind(vk::CommandBuffer commandBuffer);
    void draw(vk::CommandBuffer commandBuffer);

private:
    StaticText(Device& device, id_t id, glm::vec2 pos, glm::vec4 color, float scale, std::string text) : _device(device), _id(id), _pos(pos), _color(color), _scale(scale) {
        assignText(text);
        createVertexBuffer();
    }

    void createVertexBuffer();
    void bindBox(vk::CommandBuffer commandBuffer);
private:
    Device& _device;

    id_t _id;
    glm::vec2 _pos;
    glm::vec4 _color;
    float _scale;

    glm::vec2 _nextCharacterPos;

    std::vector<Character> _characters;
    std::unique_ptr<Buffer> _vertexBuffer;
    size_t _vertexCount;
    std::unique_ptr<Buffer> _boxBuffer;
};

template <typename T>
class VaryingText {
public:
    using id_t = unsigned int;
    using Map = std::unordered_map<id_t, VaryingText>;
    static VaryingText createText(Device& device, glm::vec2 pos, glm::vec4 color, float scale, T& text) {
        static id_t currentId = 0;
        StaticText::createText(device, pos, color, scale);
        return VaryingText{ std::move(textObj), text };
    }
    void update();

    StaticText::id_t getId();
    StaticText& staticText();
private:
    VaryingText(StaticText&& textObj, T& text) : _textObj(textObj), _text(text) {}
private:
    StaticText _textObj;
    T& _text;
};

template<typename T>
inline void VaryingText<T>::update()
{
    throw InvalidArgument("Can't convert input to text.");
}
template<typename T>
inline StaticText::id_t VaryingText<T>::getId()
{
    return _textObj.getId();
}
template<typename T>
inline StaticText& VaryingText<T>::staticText()
{
    return _textObj;
}
template<>
inline void VaryingText<int>::update()
{
    _textObj.assignText(std::to_string(_text));
}
template<>
inline void VaryingText<short>::update()
{
    _textObj.assignText(std::to_string(_text));
}
template<>
inline void VaryingText<long>::update()
{
    _textObj.assignText(std::to_string(_text));
}
template<>
inline void VaryingText<long long>::update()
{
    _textObj.assignText(std::to_string(_text));
}
template<>
inline void VaryingText<char>::update()
{
    _textObj.assignText(std::to_string(_text));
}
template<>
inline void VaryingText<unsigned int>::update()
{
    _textObj.assignText(std::to_string(_text));
}
template<>
inline void VaryingText<unsigned short>::update()
{
    _textObj.assignText(std::to_string(_text));
}
template<>
inline void VaryingText<unsigned long>::update()
{
    _textObj.assignText(std::to_string(_text));
}
template<>
inline void VaryingText<unsigned long long>::update()
{
    _textObj.assignText(std::to_string(_text));
}
template<>
inline void VaryingText<unsigned char>::update()
{
    _textObj.assignText(std::to_string(_text));
}
template<>
inline void VaryingText<float>::update()
{
    _textObj.assignText(std::to_string(_text));
}
template<>
inline void VaryingText<double>::update()
{
    _textObj.assignText(std::to_string(_text));
}
template<>
inline void VaryingText<long double>::update()
{
    _textObj.assignText(std::to_string(_text));
}
template<>
inline void VaryingText<std::string>::update()
{
    _textObj.assignText(_text);
}
template<>
inline void VaryingText<Database::String>::update()
{
    _textObj.assignText(Database::toSTD(_text));
}