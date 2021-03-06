#include "Spritemap.h"
#include "Logging.h"

namespace ECSE
{

Spritemap::Spritemap()
    : callback(nullptr), m_texture(nullptr), m_animSet(nullptr), m_currentAnim(nullptr),
    m_currentTime(sf::Time::Zero), m_currentFrame(0), m_currentVariantOffset(0)
{

}

Spritemap::Spritemap(const sf::Texture& texture)
    : Spritemap()
{
    setTexture(texture);
}

Spritemap::Spritemap(const sf::Texture& texture, const AnimationSet& animSet)
    : Spritemap(texture)
{
    setAnimationSet(animSet);
    updateFrameIndex();
}

bool Spritemap::hasAnimation(std::string name) const
{
    auto *newAnim = m_animSet->getAnimation(name);
    return newAnim != nullptr;
}

void Spritemap::playAnimation(std::string name, bool reset)
{
    if (!m_animSet)
    {
        LOG(WARNING) << "Tried to play animation \"" + name + "\" on a Spritemap with no animations!";
    }

    const Animation *newAnim = m_animSet->getAnimation(name);

    if (newAnim == nullptr)
    {
        LOG(WARNING) << "Animation " << name << " does not exist!";
    }

    // Already playing
    if (newAnim == m_currentAnim && !reset)
    {
        return;
    }

    // Start playing
    m_currentAnim = newAnim;
    m_currentTime = sf::Time::Zero;
    m_currentFrame = 0;
    updateFrameIndex();
    m_playing = true;
}

void Spritemap::stop()
{
    m_playing = false;
    m_currentAnim = nullptr;
}

void Spritemap::setVariant(std::string name)
{
    if (!m_animSet)
    {
        LOG(WARNING) << "Tried to play animation \"" + name + "\" on a Spritemap with no animations!";
    }

    auto newOffset = m_animSet->getVariantOffset(name);

    if (!newOffset)
    {
        LOG(WARNING) << "Variant " << name << " does not exist!";
    }

    m_currentVariantOffset = *newOffset;
    updateFrameIndex();
}

void Spritemap::update(sf::Time deltaTime)
{
    if (!m_playing || !m_currentAnim) return;

    size_t frameCount = m_currentAnim->frames.size();
    sf::Time frameTime = m_currentAnim->frameTime;

    // Calculate new frame
    m_currentTime += deltaTime;

    // If frame time is 0, we just want the first frame
    if (frameTime == sf::seconds(0))
    {
        m_currentFrame = 0;
        updateFrameIndex();
        return;
    }

    if (m_currentTime > frameTime)
    {
        size_t frameIncrease = static_cast<size_t>(m_currentTime / frameTime);
        m_currentFrame += frameIncrease;

        // Reset time with remainder
        m_currentTime = m_currentTime % frameTime;
        
        if (m_currentFrame >= frameCount)
        {
            // Call back if there's a callback function
            if (callback) callback();

            // Loop
            if (m_currentAnim->looping) {
                m_currentFrame = m_currentFrame % frameCount;
            }
            else 
            {
                m_currentFrame = frameCount - 1;
                m_playing = false;
            }
        }

        updateFrameIndex();
    }
}

void Spritemap::setTexture(const sf::Texture& texture)
{
    m_texture = &texture;

    if (!m_animSet)
    {
        m_currentFrame = 0;
        updateFrameIndex();
    }

    updateGrid();
}

void Spritemap::setAnimationSet(const AnimationSet& animSet)
{
    m_animSet = &animSet;
    m_frameSize = sf::Vector2f(static_cast<float>(animSet.frameSize.x),
                               static_cast<float>(animSet.frameSize.y));

    updateGrid();
}

void Spritemap::setIndex(size_t newFrame, bool stop)
{
    m_currentIndex = newFrame;

    sf::FloatRect rect = getIndexRect(m_currentIndex);

    m_vertices[0].position = sf::Vector2f(0.f, 0.f);
    m_vertices[1].position = sf::Vector2f(0.f, rect.height);
    m_vertices[2].position = sf::Vector2f(rect.width, 0.f);
    m_vertices[3].position = sf::Vector2f(rect.width, rect.height);

    m_vertices[0].texCoords = sf::Vector2f(rect.left, rect.top);
    m_vertices[1].texCoords = sf::Vector2f(rect.left, rect.top + rect.height);
    m_vertices[2].texCoords = sf::Vector2f(rect.left + rect.width, rect.top);
    m_vertices[3].texCoords = sf::Vector2f(rect.left + rect.width, rect.top + rect.height);

    if (stop)
    {
        m_currentTime = sf::Time::Zero;
        m_playing = false;
    }
}

void Spritemap::setColor(const sf::Color& color)
{
    m_vertices[0].color = color;
    m_vertices[1].color = color;
    m_vertices[2].color = color;
    m_vertices[3].color = color;
}

const sf::Texture* Spritemap::getTexture() const
{
    return m_texture;
}

const AnimationSet* Spritemap::getAnimationSet() const
{
    return m_animSet;
}

const sf::Color& Spritemap::getColor() const
{
    return m_vertices[0].color;
}

sf::Vector2f Spritemap::getSize() const
{
    if (m_frameGrid.x == 0 || m_frameGrid.y == 0)
    {
        return sf::Vector2f(m_texture->getSize());
    }

    return m_frameSize;
}

sf::FloatRect Spritemap::getGlobalBounds() const
{
    auto rect = sf::FloatRect(sf::Vector2f(), getSize());
    return getTransform().transformRect(rect);
}

sf::FloatRect Spritemap::getIndexRect(size_t frame) const
{
    if (m_frameGrid.x == 0 || m_frameGrid.y == 0)
    {
        return sf::FloatRect(sf::Vector2f(),
                             sf::Vector2f(m_texture->getSize()));
    }

    sf::Vector2f frameOffset = sf::Vector2f(
        static_cast<float>(frame % m_frameGrid.x),
        static_cast<float>(frame / m_frameGrid.x)
        );

    frameOffset.x *= m_frameSize.x;
    frameOffset.y *= m_frameSize.y;

    return sf::FloatRect(frameOffset, m_frameSize);
}

void Spritemap::updateGrid()
{
    // No data to work with
    if (!m_texture || m_frameSize.x == 0 || m_frameSize.y == 0) return;

    sf::Vector2u textureSize = m_texture->getSize();

    if (static_cast<size_t>(m_frameSize.x) > textureSize.x ||
        static_cast<size_t>(m_frameSize.y) > textureSize.y)
    {
        LOG(WARNING) << "Texture not big enough for provided frame width";
    }

    m_frameGrid = sf::Vector2u(
        textureSize.x / static_cast<size_t>(m_frameSize.x),
        textureSize.y / static_cast<size_t>(m_frameSize.y)
        );
}

size_t Spritemap::calcFrameIndex() const
{
    if (m_currentAnim)
    {
        return m_currentAnim->frames[m_currentFrame] + m_currentVariantOffset;
    }
    else
    {
        return 0;
    }
}

void Spritemap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (m_texture)
    {
        states.transform *= getTransform();
        states.texture = m_texture;
        target.draw(m_vertices, 4, sf::TrianglesStrip, states);
    }
}

}
