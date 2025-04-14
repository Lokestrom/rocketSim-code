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

#include "String.hpp"
#include "../helpers/controles.hpp"
#include "Vector.hpp"
#include "../helpers/math.hpp"

struct WindowInfo;
class Pipeline;

void resetText();

void setFont(WindowInfo& window);
void setDefaultFont(WindowInfo& window);

void setFontFolder(WindowInfo& window);
void setDefaultFontFolder(WindowInfo& window);

class CharacterGlyph {
public:
    struct BoundingBox {
        double top, bottom;
        double right, left;
    };
    struct CharacterLayout {
        std::vector<glm::vec2> hardVertecies;
        std::vector<glm::vec2> becierVertecies;
        BoundingBox bb;
        double horiAdvance;
        double lineSpacing;

        CharacterLayout() {}
        CharacterLayout(const CharacterLayout&) = delete;
        CharacterLayout& operator=(const CharacterLayout&) = delete;
        CharacterLayout& operator=(CharacterLayout&& other) noexcept;
    };

    CharacterGlyph() : _characterLayout() {}
    ~CharacterGlyph(){}

    CharacterGlyph(const CharacterGlyph&) = delete;
    CharacterGlyph& operator=(const CharacterGlyph&) = delete;

    CharacterGlyph& operator=(CharacterGlyph&& other) noexcept;

    void createCharacter(char character, std::string font);
    
    CharacterLayout& getCharacterlayout() { return _characterLayout; }

private:
    CharacterLayout _characterLayout;
};

class CharacterGlyphCache {
public:
    static void cacheCharacter(char character);
    static void deCacheCharacter(char character);
    static void clear();

    static void setFont(std::string font);
    static void setFontFolder(std::string fontFolderPath);


    static std::shared_ptr<CharacterGlyph> getCharacterGlyph(char character);
private:
    static std::unordered_map<char, std::shared_ptr<CharacterGlyph>> _cachedCharacters;
    static std::string _font;
    static std::string _fontFolder;
};

struct Character {
    glm::vec2 pos;
    std::shared_ptr<CharacterGlyph> characterGlyph;
};


class StaticText
{
    friend class VaryingText;
    friend class ScrollField;
public:
    using id_t = unsigned int;
    using Map = std::unordered_map<id_t, std::shared_ptr<StaticText>>;

    enum Alignment{
        left,
		center,
		right,
    };

    struct Vertex {
        glm::vec2 position{};

        static std::vector<vk::VertexInputBindingDescription> getBindingDescriptions();
        static std::vector<vk::VertexInputAttributeDescription> getAttributeDescriptions();

        bool operator==(const Vertex& other) const {
            return position == other.position;
        }
    };

    struct BecierVertex {
        glm::vec2 position{};
        glm::vec2 becierPos{};

        static std::vector<vk::VertexInputBindingDescription> getBindingDescriptions();
        static std::vector<vk::VertexInputAttributeDescription> getAttributeDescriptions();

        bool operator==(const BecierVertex& other) const {
            return position == other.position
                && becierPos == other.becierPos;
        }
    };

    static std::shared_ptr<StaticText> createText(WindowInfo& window, glm::vec2 pos, glm::vec4 color, float scale, std::string text = "", Alignment alignment = Alignment::center);

    StaticText(const StaticText&) = delete;
    StaticText& operator=(const StaticText&) = delete;
    StaticText(StaticText&&) = default;
    StaticText& operator=(StaticText&&) = default;

    id_t getId() { return _id; }
    const float getScale() { return _scale;  }
    const glm::vec2 getPos() { return _pos; }
    const glm::vec4 getColor() { return _color; }
	const std::vector<Character>& getCharacters() { return _characters; }
    bool empty() { return _characters.size() == 0; }

    void assignText(std::string text);
    void removeText();

    void changeColor(const glm::vec4& newColor);

    void bindHard(vk::CommandBuffer commandBuffer);
    void bindBecier(vk::CommandBuffer commandBuffer);
    void drawHard(vk::CommandBuffer commandBuffer);
    void drawBecier(vk::CommandBuffer commandBuffer);
    void drawBox(vk::CommandBuffer commandBuffer);

    bool hasBecier();

    void restart();

private:
    StaticText(Device& device, id_t id, glm::vec2 pos, glm::vec4 color, float scale, std::string text, Alignment alignment) : _device(device), _id(id), _pos(pos), _color(color), _scale(scale), _alignment(alignment) {
        if (text.length() == 0)
            return;
        assignText(text);
    }

    void createVertexBuffer();
private:
    Device& _device;

    id_t _id;
    glm::vec2 _pos;
    glm::vec4 _color;
    float _scale;
	Alignment _alignment;

    std::vector<Character> _characters;
    std::unique_ptr<Buffer> _hardVertexBuffer, _becierVertexBuffer;
    size_t _hardVertexCount, _becierVertexCount;
};

class VaryingText {
public:
    class VaryingTextString {
    public:
        struct Variable {
            enum Type
            {
                StdString,
                Int,
                LongDouble,
                Double,
                String,
                Float,
                Vec2,
                Vec3,
                Quaternion,
            };
            void* variable;
            Type type;
            sizeT index;

            friend bool operator> (const Variable& lhs, const Variable& rhs) {
                return lhs.index > rhs.index;
            }

            std::string get();
        };
        VaryingTextString(const std::string& text) : _planeText(text), _variable() {}

        VaryingTextString(const VaryingTextString&) = delete;
        VaryingTextString& operator=(const VaryingTextString&) = delete;
        VaryingTextString(VaryingTextString&& other)
            : _planeText(std::move(other._planeText)),
            _variable(std::move(other._variable))
        {
        };
        VaryingTextString& operator=(VaryingTextString&&) = default;

        template<typename T>
        void addVariable(T& var, sizeT index);
        void assignText(std::string text);

        std::string getString();
    private:
        Vector<Variable> _variable;
        std::string _planeText;
    };
public:
    using Map = std::unordered_map<StaticText::id_t, std::shared_ptr<VaryingText>>;
    static std::shared_ptr<VaryingText> createText(WindowInfo& window, glm::vec2 pos, glm::vec4 color, float scale, const std::string& text = "", StaticText::Alignment alignment = StaticText::Alignment::center);

    VaryingText(const VaryingText&) = delete;
    VaryingText& operator=(const VaryingText&) = delete;
    VaryingText(VaryingText&& other) :
        _textObj(other._textObj),
        _text(std::move(other._text))
    {
    }
    VaryingText& operator=(VaryingText&&) = default;

    void update();
    template<typename T>
    void addVariable(T& var, sizeT index);
    void assignText(std::string text);

    StaticText::id_t getId();
    std::shared_ptr<StaticText> staticText();
private:
    VaryingText(std::shared_ptr<StaticText> textObj, const std::string& text) : _textObj(textObj), _text(text) {}
private:
    std::shared_ptr<StaticText> _textObj;
    VaryingTextString _text;
};

template<typename T>
inline void VaryingText::VaryingTextString::addVariable(T& var, sizeT index)
{
	Error("VaryingText::VaryingTextString::addVariable: No specialization for this type", Error::Type::codeFault, Error::recoveryType::unrecoverable);
}
template<>
inline void VaryingText::VaryingTextString::addVariable(int& var, sizeT index)
{
	_variable.pushBack({ &var, Variable::Type::Int, index });
	_variable.bubbleSort();
}

template<>
inline void VaryingText::VaryingTextString::addVariable(long double& var, sizeT index)
{
	_variable.pushBack({ &var, Variable::Type::LongDouble, index });
    _variable.bubbleSort();
}
template<>
inline void VaryingText::VaryingTextString::addVariable(double& var, sizeT index)
{
	_variable.pushBack({ &var, Variable::Type::Double, index });
	_variable.bubbleSort();
}
template<>
inline void VaryingText::VaryingTextString::addVariable(float& var, sizeT index)
{
    _variable.pushBack({ &var, Variable::Type::Float, index });
    _variable.bubbleSort();
}
template<>
inline void VaryingText::VaryingTextString::addVariable(std::string& var, sizeT index)
{
	_variable.pushBack({ &var, Variable::Type::StdString, index });
    _variable.bubbleSort();
}
template<>
inline void VaryingText::VaryingTextString::addVariable(Database::String& var, sizeT index)
{
	_variable.pushBack({ &var, Variable::Type::String, index });
	_variable.bubbleSort();
}
template<>
inline void VaryingText::VaryingTextString::addVariable(Vector2& var, sizeT index)
{
	_variable.pushBack({ &var, Variable::Type::Vec2, index });
	_variable.bubbleSort();
}
template<>
inline void VaryingText::VaryingTextString::addVariable(Vector3& var, sizeT index)
{
	_variable.pushBack({ &var, Variable::Type::Vec3, index });
	_variable.bubbleSort();
}
template<>
inline void VaryingText::VaryingTextString::addVariable(Quaternion& var, sizeT index)
{
	_variable.pushBack({ &var, Variable::Type::Quaternion, index });
	_variable.bubbleSort();
}

template<typename T>
inline void VaryingText::addVariable(T& var, sizeT index)
{
    _text.addVariable(var, index);
}