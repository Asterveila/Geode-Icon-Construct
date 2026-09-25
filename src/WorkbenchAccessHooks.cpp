#include <Geode/modify/GJGarageLayer.hpp>
#include "IconOffsetEditor.hpp"

using namespace geode::prelude;

class $modify(OffsetEditorGarageLayer, GJGarageLayer) {
    bool init() {
        if (!GJGarageLayer::init()) return false;
        
		auto editorSprite = CircleButtonSprite::create(CCSprite::create("offsetIndicatorBtn.png"_spr), CircleBaseColor::Green, CircleBaseSize::SmallAlt);
        auto editorButton = CCMenuItemSpriteExtra::create(
            editorSprite,
            this,
            menu_selector(OffsetEditorGarageLayer::onOpenOffsetEditor)
        );
        
        auto menu = this->getChildByID("shards-menu");
        if (menu) {
            editorButton->setID("icon-workbench"_spr);
            editorButton->setPosition({-180.0f, 120.0f});
            menu->addChild(editorButton);
            menu->updateLayout();
        }
        
        return true;
    }
    
    void onOpenOffsetEditor(CCObject* sender) {
        IconOffsetEditorPopup::create()->show();
    }
};