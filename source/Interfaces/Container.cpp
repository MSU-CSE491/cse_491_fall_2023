/**
 * @file Container.cpp
 * @author muiph
 */

#include "Container.hpp"

//namespace GUI {
////    Container::Container()
////            :mChildren(0), mSelectedChild(-1)
////    {
////    }
//
//    void Container::pack(Component::Ptr component)
//    {
//        mChildren.push_back(component);
//        if (!hasSelection() && component->isSelectable())
//            select(mChildren.size()-1);
//    }
//
//    bool Container::isSelectable() const
//    {
//        return false;
//    }
//
//    void Container::handleEvent(const sf::Event& event)
//    {
//        if (hasSelection() && mChildren[mSelectedChild]->isActive()) {
//            mChildren[mSelectedChild]->handleEvent(event);
//        }
//        else if (event.type==sf::Event::KeyReleased) {
//            if (event.key.code==sf::Keyboard::W
//                    || event.key.code==sf::Keyboard::Up) {
//                selectPrevious();
//            }
//            else if (event.key.code==sf::Keyboard::S
//                    || event.key.code==sf::Keyboard::Down) {
//                selectNext();
//            }
//            else if (event.key.code==sf::Keyboard::Return
//                    || event.key.code==sf::Keyboard::Space) {
//                if (hasSelection())
//                    mChildren[mSelectedChild]->activate();
//            }
//        }
//    }
//
//    void Container::select(std::size_t index)
//    {
//        if (mChildren[index]->isSelectable()) {
//            if (hasSelection())
//                mChildren[mSelectedChild]->deselect();
//            mChildren[index]->select();
//            mSelectedChild = index;
//        }
//    }
//
//    void Container::selectNext()
//    {
//        if (!hasSelection())
//            return;
//// Search next component that is selectable
//        int next = mSelectedChild;
//        do
//            next = (next+1)%mChildren.size();
//        while (!mChildren[next]->isSelectable());
//// Select that component
//        select(next);
//    }
//
//    void Container::selectPrevious()
//    {
//        if (!hasSelection())
//            return;
//        // Search previous component that is selectable
//        int prev = mSelectedChild;
//        do
//            prev = (prev+mChildren.size()-1)%mChildren.size();
//        while (!mChildren[prev]->isSelectable());
//// Select that component
//        select(prev);
//    }
//
//    bool Container::hasSelection()
//    {
//        return false;
//    }
//}
