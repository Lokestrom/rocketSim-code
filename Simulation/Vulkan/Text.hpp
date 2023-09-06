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
#include "../helpers/controles.hpp"
#include "Vector.hpp"

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
        return StaticText{ device, currentId++, pos, color, scale, text };
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
    class VaryingTextString {
    public:
        struct Variable {
            T* variable;
            sizeT index;

            friend bool operator> (const Variable& lhs, const Variable& rhs) {
                return lhs.index > rhs.index;
            }
        };
        VaryingTextString(const std::string& text) : _planeText(text) {}

        VaryingTextString(const VaryingTextString&) = delete;
        VaryingTextString& operator=(const VaryingTextString&) = delete;
        VaryingTextString(VaryingTextString&& other) 
            : _planeText(std::move(other._planeText)),
            _variable(std::move(other._variable))
            {};
        VaryingTextString& operator=(VaryingTextString&&) = default;

        void addVariable(T& var, sizeT index);
        void assignText(std::string text);

        std::string getString() noexcept;

    private:
        std::string toS(T& var);
    private:
        Vector<Variable> _variable;
        std::string _planeText;
    };
public:
    using id_t = unsigned int;
    using Map = std::unordered_map<id_t, VaryingText<T>>;
    static VaryingText<T> createText(Device& device, glm::vec2 pos, glm::vec4 color, float scale, const std::string& text) {
        static id_t currentId = 0;
        auto textObj = StaticText::createText(device, pos, color, scale);
        return VaryingText<T>(textObj, text);
    }

    void update();
    void addVariable(T& var, sizeT index);
    void assignText(std::string text);

    StaticText::id_t getId();
    StaticText& staticText();
private:
    VaryingText(StaticText& textObj, const std::string& text) : _textObj(std::move(textObj)), _text(text) {}
private:
    StaticText _textObj;
    VaryingTextString _text;
};

template<typename T>
inline void VaryingText<T>::VaryingTextString::addVariable(T& var, sizeT index)
{
    _variable.pushBack({ &var, index });
    _variable.bubbleSort();
}

template<typename T>
inline void VaryingText<T>::VaryingTextString::assignText(std::string text)
{
    _planeText = text;
}

template<typename T>
inline std::string VaryingText<T>::VaryingTextString::getString() noexcept
{
    std::string ans;
    ans.reserve(_planeText.size() + (_variable.size() * 5));

    auto planeStringIndex = 0;
    auto variableIndex = 0;
    while (planeStringIndex <= _planeText.size()) {
        if (_variable[variableIndex].index == planeStringIndex) {
            ans += (toS(*(_variable[variableIndex].variable)));
            variableIndex++;
        }
        else {
            if (planeStringIndex == _planeText.size())
                throw OutOfRange("There are variables out of range");
            ans.push_back(_planeText[planeStringIndex]);
            planeStringIndex++;
        }
    }
}


template<typename T>
inline std::string VaryingText<T>::VaryingTextString::toS(T& var)
{
    throw InvalidArgument("Can't convert input to text.");
}

template<>
inline std::string VaryingText<int>::VaryingTextString::toS(int& var)
{
    return std::to_string(var);
}
template<>
inline std::string VaryingText<long double>::VaryingTextString::toS(long double& var)
{
    return std::to_string(var);
}
template<>
inline std::string VaryingText<std::string>::VaryingTextString::toS(std::string& var)
{
    return var;
}
template<>
inline std::string VaryingText<Database::String>::VaryingTextString::toS(Database::String& var)
{
    return toSTD(var);
}

template<typename T>
inline void VaryingText<T>::update()
{
    _textObj.assignText(_text.getString());
}

template<typename T>
inline void VaryingText<T>::addVariable(T& var, sizeT index)
{
    _text.addVariable(var, index);
}

template<typename T>
inline void VaryingText<T>::assignText(std::string text)
{
    _text.assignText(text);
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


