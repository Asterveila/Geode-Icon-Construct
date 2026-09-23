#include <Geode/modify/GJGarageLayer.hpp>
#include "IconOffsetEditor.hpp"

using namespace geode::prelude;

class $modify(OffsetEditorGarageLayer, GJGarageLayer) {
    bool init() {
        if (!GJGarageLayer::init()) return false;
        
        //auto editorSprite = CCSprite::createWithSpriteFrameName("GJ_editBtn_001.png");
		auto editorSprite = CircleButtonSprite::create(CCSprite::create("offsetIndicatorBtn.png"_spr), CircleBaseColor::Green, CircleBaseSize::SmallAlt);
        auto editorButton = CCMenuItemSpriteExtra::create(
            editorSprite,
            this,
            menu_selector(OffsetEditorGarageLayer::onOpenOffsetEditor)
        );
        
        auto menu = this->getChildByID("shards-menu");
        if (!menu) {
            menu = CCMenu::create();
            menu->setID("offset-editor-menu"_spr);
            menu->setPosition({20.0f, 100.0f});
            this->addChild(menu);
        }
        
        editorButton->setID("icon-workbench"_spr);
        editorButton->setPosition({-180.0f, 120.0f});
        menu->addChild(editorButton);
        
        return true;
    }
    
    void onOpenOffsetEditor(CCObject* sender) {
        IconOffsetEditorPopup::create()->show();
    }
};