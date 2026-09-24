#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

class IconPartCell : public CCLayer {
protected:
    CCLayerColor* m_background = nullptr;
    CCMenuItemSpriteExtra* m_button = nullptr;
    CCSprite* m_previewSprite = nullptr;

    bool init(CCSpriteFrame* frame, const std::string& displayName, const std::string& partId,int tag, bool even, CCObject* target, SEL_MenuHandler selector);

public:
    static constexpr float HEIGHT = 30.f;
    static constexpr float WIDTH = 130.f;

    static IconPartCell* create(CCSpriteFrame* frame, const std::string& displayName, const std::string& partId, int tag, bool even, CCObject* target, SEL_MenuHandler selector);

    void setSelected(bool selected);

    CCMenuItemSpriteExtra* getButton() const { return m_button; }
};
