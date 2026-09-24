#include "IconPartCell.hpp"

bool IconPartCell::init(CCSpriteFrame* frame, const std::string& displayName, const std::string& partId, int tag, bool even, CCObject* target, SEL_MenuHandler selector) {
    if (!CCLayer::init()) return false;

    CCSize size = {WIDTH, HEIGHT};
    this->setContentSize(size);
    this->setID(fmt::format("{}-cell", partId));

    m_background = CCLayerColor::create();
    m_background->setContentSize(size);
    m_background->setOpacity(even ? 60 : 30);
    m_background->setColor({ 0, 0, 0 });
    this->addChild(m_background, -1);

    auto menu = CCMenu::create();
    menu->setContentSize(size);
    menu->setPosition({0.f, 0.f});
    menu->setAnchorPoint({0.f, 0.f});
    this->addChild(menu);

    m_previewSprite = (frame != nullptr)
        ? CCSprite::createWithSpriteFrame(frame)
        : CCSprite::create();
    m_previewSprite->setColor({ 255, 255, 255 });
    m_previewSprite->setScale(0.75f);

    m_button = CCMenuItemSpriteExtra::create(m_previewSprite, target, selector);
    m_button->setUserObject("part-id"_spr, CCString::create(partId));
    m_button->setTag(tag);
    m_button->setPosition({22.f, size.height / 2.f});
    m_button->setID(fmt::format("{}-btn", partId));
    menu->addChild(m_button);

    auto nameLabel = CCLabelBMFont::create(displayName.c_str(), "chatFont.fnt");
    nameLabel->setAnchorPoint({0.f, 0.5f});
    nameLabel->setPosition({44.f, size.height / 2.f});
    nameLabel->setScale(0.6f);
    nameLabel->setOpacity(190);
    nameLabel->limitLabelWidth(WIDTH - 48.f, 0.6f, 0.1f);
    this->addChild(nameLabel);

    return true;
}

IconPartCell* IconPartCell::create(CCSpriteFrame* frame, const std::string& displayName, const std::string& partId, int tag, bool even, CCObject* target, SEL_MenuHandler selector) {
    auto ret = new IconPartCell();
    if (ret->init(frame, displayName, partId, tag, even, target, selector)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

void IconPartCell::setSelected(bool selected) {
    if (!m_previewSprite) return;

    m_previewSprite->stopAllActions();
    m_background->stopAllActions();

    if (selected) {
        auto tintToGray = CCTintTo::create(0.35f, 90, 90, 90);
        auto tintToWhite = CCTintTo::create(0.35f, 255, 255, 255);
        auto sequence = CCSequence::create(tintToGray, tintToWhite, nullptr);
        m_previewSprite->runAction(CCRepeatForever::create(sequence));
    } else {
        m_previewSprite->setColor({255, 255, 255});
    }

}
