#pragma once

#include <map>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

typedef std::vector<size_t> FrameVector;

//! A class used to draw animated sprites.
class Spritemap : public sf::Drawable, public sf::Transformable
{
public:
    //! Create a Spritemap with no texture.
    explicit Spritemap();

    //! Create a Spritemap with a given texture.
    /*!
      \param texture The texture to use.
    */
    explicit Spritemap(const sf::Texture& texture);

    //! Create a Spritemap with a given frame width and height.
    /*!
      \param texture    The texture to use.
      \param frameSize  The size of a single frame.
    */
    explicit Spritemap(const sf::Texture& texture, const sf::Vector2f & frameSize);

    //! Add an animation type.
    /*!
      \param name       The name of the new animation.
      \param frames     A vector containing the frame indices to display at each animation frame.
      \param frameTime  The amount of time between frames.
      \param looping    Whether the animation should loop.
    */
    void addAnimation(std::string name, FrameVector& frames, sf::Time frameTime, bool looping = true);

    //! Play an animation.
    /*!
      \param name   The name of the animation to play.
      \param reset  Whether the animation should restart if it's already playing.
    */
    void playAnimation(std::string name, bool reset = false);

    //! Stop animating.
    void stop();

    //! Update the animation frame.
    /*!
      \param deltaTime  The amount of time to animate.
    */
    void update(sf::Time deltaTime);

    //! Set the texture from which frames are taken.
    /*!
      \param texture    The new texture to use.
    */
    void setTexture(const sf::Texture& texture);

    //! Set the size of a single frame.
    /*!
      \param frameSize  The size of a single frame.
    */
    void setFrameSize(const sf::Vector2f& frameSize);

    //! Set the current frame index.
    /*!
      \param newFrame   The new frame.
      \param stop       Whether the animation should stop at this frame.
    */
    void setIndex(size_t newFrame, bool stop = false);

    //! Set the color of the sprite.
    /*!
      \param color  The new color to use.
    */
    void setColor(const sf::Color& color);

    //! Get a pointer to the texture.
    /*!
      \return A pointer to the texture.
    */
    const sf::Texture* getTexture() const;

    //! Get the color of the sprite.
    /*!
      \return The color of the sprite.
    */
    const sf::Color& getColor() const;

    //! Get the local bounds of the current frame.
    /*!
      \return The local bounds of the sprite frame.
    */
    sf::FloatRect getLocalBounds() const;

    //! Get the global bounds of the current frame.
    /*!
      \return The global bounds of the sprite frame.
    */
    sf::FloatRect getGlobalBounds() const;


    // Data

    //! Callback function for when the animation completes.
    void (*callback)();

private:
    //! Determine the number of rows and columns.
    void updateGrid();

    //! Gets the rectangle containing the given frame.
    sf::FloatRect getIndexRect(size_t frame) const;

    //! Draw the sprite to a render target.
    /*!
      \param target Render target to draw to
      \param states Current render states
    */
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    // Data

    //! Used to store animation data
    struct Animation
    {
        FrameVector frames;
        sf::Time frameTime;
        bool looping;
    };

    sf::Vertex          m_vertices[4];  //!< Vertices defining the sprite's geometry
    const sf::Texture*  m_texture;      //!< Texture of the sprite
    const Animation*    m_currentAnim;  //!< A pointer to the current animation
    sf::Time            m_currentTime;  //!< The current animation time
    sf::Vector2f        m_frameSize;    //!< The size of a frame
    sf::Vector2u        m_frameGrid;    //!< The number of rows and columns
    bool                m_playing;      //!< Whether the animation is currently playing
    size_t              m_currentIndex; //!< The current frame index
    size_t              m_currentFrame; //!< The current frame of animation
    std::map<std::string, Animation> m_anims; //!< Map from names to animations
};

