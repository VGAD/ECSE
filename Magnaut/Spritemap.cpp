#include "Spritemap.h"

Spritemap::Spritemap()
    : m_texture(nullptr), callback(nullptr), m_currentFrame(0), m_currentTime(sf::Time::Zero)
{

}

Spritemap::Spritemap(const sf::Texture& texture)
    : Spritemap()
{
    setTexture(texture);
}

Spritemap::Spritemap(const sf::Texture& texture, const sf::Vector2f& frameSize)
    : Spritemap(texture)
{
    setFrameSize(frameSize);
    setIndex(0);
}

void Spritemap::addAnimation(std::string name, FrameVector& frames, sf::Time frameTime, bool looping)
{
    Animation& anim = m_anims[name];
    anim.frames = frames;
    anim.frameTime = frameTime;
    anim.looping = looping;
}

void Spritemap::playAnimation(std::string name, bool reset)
{
    Animation &newAnim = m_anims[name];

    // Already playing
    if (&newAnim == m_currentAnim && reset)
    {
        setIndex(0, true);
        return;
    }

    // Start playing
    m_currentAnim = &newAnim;
    m_currentTime = sf::Time::Zero;
    m_playing = true;
}

void Spritemap::stop()
{
    m_playing = false;
    m_currentAnim = nullptr;
}

void Spritemap::update(sf::Time deltaTime)
{
    if (!m_playing || !m_currentAnim) return;

    size_t frameCount = m_currentAnim->frames.size();
    sf::Time frameTime = m_currentAnim->frameTime;

    // Calculate new frame
    m_currentTime += deltaTime;
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

        setIndex(m_currentAnim->frames[m_currentFrame]);
    }
}

void Spritemap::setTexture(const sf::Texture& texture)
{
    m_texture = &texture;

    updateGrid();
}

void Spritemap::setFrameSize(const sf::Vector2f& frameSize)
{
    m_frameSize = frameSize;

    updateGrid();
}

void Spritemap::setIndex(size_t newFrame, bool stop)
{
    m_currentIndex = newFrame;

    sf::FloatRect rect = getIndexRect(m_currentIndex);

    m_vertices[0].position = sf::Vector2f(0.f, 0.f);
    m_vertices[1].position = sf::Vector2f(0.f, rect.width);
    m_vertices[2].position = sf::Vector2f(rect.height, 0.f);
    m_vertices[3].position = sf::Vector2f(rect.height, rect.width);

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

const sf::Color& Spritemap::getColor() const
{
    return m_vertices[0].color;
}

sf::FloatRect Spritemap::getLocalBounds() const
{
    return getIndexRect(m_currentIndex);
}

sf::FloatRect Spritemap::getGlobalBounds() const
{
    return getTransform().transformRect(getLocalBounds());
}

sf::FloatRect Spritemap::getIndexRect(size_t frame) const
{
    if (m_frameGrid.x == 0 || m_frameGrid.y == 0) return sf::FloatRect();

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

    m_frameGrid = sf::Vector2u(
        textureSize.x / static_cast<size_t>(m_frameSize.x),
        textureSize.y / static_cast<size_t>(m_frameSize.y)
        );
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