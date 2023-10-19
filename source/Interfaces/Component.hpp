/**
 * @file Component.h
 * @author
 *
 *  Base class
 */

#pragma once

#include <memory>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

namespace i_2D
{
    class Component : public sf::Drawable
            , public sf::Transformable
    {
    public:
        typedef std::shared_ptr<Component> Ptr;
    public:
        Component();
        virtual ~Component();
        virtual bool isSelectable() const = 0;
        bool isSelected() const;
        virtual void select();
        virtual void deselect();
        virtual bool isActive() const;
        virtual void activate();
        virtual void deactivate();
        virtual void handleEvent(const sf::Event& event) = 0;
    private:
        bool mIsSelected;
        bool mIsActive;
    };
}


