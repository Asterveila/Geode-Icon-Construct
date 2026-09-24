#include "IconOffsetEditor.hpp"
#include "Utils.hpp"
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/binding/GameManager.hpp>
#include <Geode/binding/SimplePlayer.hpp>
#include <hiimjustin000.more_icons/include/MoreIcons.hpp>
#include <Geode/ui/GeodeUI.hpp>

void updatePreviewIcon(SimplePlayer* player, IconType iconType) {
    auto manager = GameManager::sharedState();
	int typeAsInt = static_cast<int>(iconType);
    
    switch(iconType) {
        case IconType::Cube:
            player->updatePlayerFrame(manager->getPlayerFrame(), IconType::Cube);
            break;
        case IconType::Ship:
            player->updatePlayerFrame(manager->getPlayerShip(), IconType::Ship);
            break;
        case IconType::Ball:
            player->updatePlayerFrame(manager->getPlayerBall(), IconType::Ball);
            break;
        case IconType::Ufo:
            player->updatePlayerFrame(manager->getPlayerBird(), IconType::Ufo);
            break;
        case IconType::Wave:
            player->updatePlayerFrame(manager->getPlayerDart(), IconType::Wave);
            break;
        case IconType::Robot:
            player->updatePlayerFrame(manager->getPlayerRobot(), IconType::Robot);
            break;
        case IconType::Spider:
            player->updatePlayerFrame(manager->getPlayerSpider(), IconType::Spider);
            break;
        case IconType::Swing:
            player->updatePlayerFrame(manager->getPlayerSwing(), IconType::Swing);
            break;
        case IconType::Jetpack:
            player->updatePlayerFrame(manager->getPlayerJetpack(), IconType::Jetpack);
            break;
        default:
            player->updatePlayerFrame(manager->getPlayerFrame(), IconType::Cube);
            break;
    }
    
    more_icons::updateSimplePlayer(player, iconType);
    
    player->setColor(manager->colorForIdx(manager->getPlayerColor()));
    player->setSecondColor(manager->colorForIdx(manager->getPlayerColor2()));
    player->setGlowOutline(manager->colorForIdx(manager->getPlayerGlowColor()));
    player->enableCustomGlowColor(manager->colorForIdx(manager->getPlayerGlowColor()));
    
    if (!manager->getPlayerGlow()) {
        player->disableGlowOutline();
    }
}

CCSize getHitboxSizeForIconType(IconType iconType) {
    float scale = 3.f;

    switch(iconType) {
        case IconType::Cube:
        case IconType::Ship:
        case IconType::Ball:
        case IconType::Ufo:
        case IconType::Robot:
        case IconType::Swing:
        case IconType::Jetpack:
            return {30.f * scale, 30.f * scale};
        case IconType::Wave:
            return {10.f * scale, 10.f * scale};
        case IconType::Spider:
            return {27.f * scale, 27.f * scale};
        default:
            return {30.f, 30.f};
    }
}

std::string getCurrentTimeString() {
    auto now = std::chrono::system_clock::now();
    return fmt::format("{:%Y%m%d_%H%M%S}", now);
}

std::string getRealFrameName(const std::string& fullFrameName) {
    std::string result = fullFrameName;
    
    constexpr std::string_view moreIconsPrefix = "hiimjustin000.more_icons/";
    if (result.starts_with(moreIconsPrefix)) result = result.substr(moreIconsPrefix.length());
    
    size_t colonPos = result.find(':');
    if (colonPos != std::string::npos) result = result.substr(colonPos + 1);
    
    return result;
}

std::string getPreservedSuffix(const std::string& frameName, IconType type) {
    // @geode-ignore(unknown-resource)
    if (!frameName.ends_with("_001.png")) return "";
    std::string_view end(frameName.data(), frameName.size() - 8);

    if (type == IconType::Robot || type == IconType::Spider) {
        constexpr const char* endings[] = {"_01_2", "_02_2", "_03_2", "_04_2", "_01_extra", "_01_glow", "_02_glow", "_03_glow", "_04_glow", "_01", "_02", "_03", "_04"};
        for (auto ending : endings) {
            if (end.ends_with(ending)) return frameName.substr(frameName.size() - 8 - std::string_view(ending).size());
        }
    } else if (type == IconType::Ufo) {
        constexpr const char* endings[] = {"_2", "_3", "_extra", "_glow"};
        for (auto ending : endings) {
            if (end.ends_with(ending)) return frameName.substr(frameName.size() - 8 - std::string_view(ending).size());
        }
    } else {
        constexpr const char* endings[] = {"_2", "_extra", "_glow"};
        for (auto ending : endings) {
            if (end.ends_with(ending)) return frameName.substr(frameName.size() - 8 - std::string_view(ending).size());
        }
    }
    
    // Fallback to just the base suffix
    return frameName.substr(frameName.size() - 8); 
}

int getPrefixLen(IconType type) {
    switch(type) {
        case IconType::Ufo:
            return 4;
        case IconType::Robot:
            return 6;
        case IconType::Spider:
            return 7;
        case IconType::Swing:
            return 6;
        case IconType::Jetpack:
            return 8;
        default:
            return 5; // cube, ship, ball, wave all are 4 chars long :sob:
        // oh and theres 1 extra char for the _
    }
}

// std::function<void(float)>
class AddValuePopup : public Popup {
protected:
    geode::TextInput* m_valueInput = nullptr;
    std::function<void(float)> m_callback;
    bool m_isXAxis = true;
    
    bool init(std::function<void(float)> callback) {
        if (!Popup::init(240.0f, 140.0f, "GJ_square01.png")) return false;
        m_callback = std::move(callback);
        
        auto winSize = this->m_mainLayer->getContentSize();
        
        this->setTitle(m_isXAxis ? "Add to Offset X" : "Add to Offset Y");
        
        // info
        auto infoLabel = CCLabelBMFont::create(
            "Enter a value to add to the current offset.\nPositive numbers move right/up.\nNegative numbers move left/down.",
            "chatFont.fnt"
        );
        infoLabel->setPosition({winSize.width / 2.0f, winSize.height / 2.0f + 23.0f});
        infoLabel->setScale(0.45f);
        infoLabel->setAlignment(CCTextAlignment::kCCTextAlignmentCenter);
        this->m_mainLayer->addChild(infoLabel);
        
        // input
        m_valueInput = geode::TextInput::create(120.0f, "0.0", "bigFont.fnt");
        m_valueInput->setPosition({winSize.width / 2.0f, winSize.height / 2.0f - 10.0f});
        m_valueInput->setScale(0.8f);
        m_valueInput->setFilter("0123456789.-");
        this->m_mainLayer->addChild(m_valueInput);
        
        // buttons
        auto addBtnSpr = ButtonSprite::create("Add", "goldFont.fnt", "GJ_button_01.png", 0.8f);
        auto addBtn = CCMenuItemSpriteExtra::create(
            addBtnSpr,
            this,
            menu_selector(AddValuePopup::onAdd)
        );
        
        auto cancelBtnSpr = ButtonSprite::create("Cancel", "goldFont.fnt", "GJ_button_06.png", 0.8f);
        auto cancelBtn = CCMenuItemSpriteExtra::create(
            cancelBtnSpr,
            this,
            menu_selector(AddValuePopup::onClose)
        );
        
        // menu
        auto buttonMenu = CCMenu::create();
        buttonMenu->addChild(addBtn);
        buttonMenu->addChild(cancelBtn);
        buttonMenu->setLayout(
            RowLayout::create()
                ->setGap(8.0f)
                ->setAxisAlignment(AxisAlignment::Center)
        );
        buttonMenu->setPosition({winSize.width / 2.0f, winSize.height / 2.0f - 45.0f});
        buttonMenu->setContentSize({150.0f, 30.0f});
        buttonMenu->updateLayout();
        this->m_mainLayer->addChild(buttonMenu);
        
        return true;
    }
    
    void onAdd(CCObject* sender) {
        std::string valueStr = m_valueInput->getString();

        if (valueStr.empty()) {
            FLAlertLayer::create("Error", "Please enter a value!", "OK")->show();
            return;
        }

        auto value = geode::utils::numFromString<float>(valueStr);
        if (!value) {
            FLAlertLayer::create("Error", "Invalid number!", "OK")->show();
            return;
        }

        if (m_callback) m_callback(value.unwrap());
        this->onClose(nullptr);
    }
    
public:
    static AddValuePopup* create(std::function<void(float)> callback, bool isXAxis) {
        auto ret = new AddValuePopup();
        ret->m_isXAxis = isXAxis;
        if (ret->init(std::move(callback))) {
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr;
    }
};

IconOffsetEditorPopup* IconOffsetEditorPopup::create() {
    auto ret = new IconOffsetEditorPopup();
    if (ret->init()) {
        ret->autorelease(); 
        return ret;
    }
    delete ret;
    return nullptr;
}

bool IconOffsetEditorPopup::init() {
    if (!Popup::init(530.0f, 280.0f, "GJ_square01.png")) return false;

    this->setTitle("Icon Workbench");

    auto manager = GameManager::sharedState();
    m_currentIconType = manager->m_playerIconType;
    IconInfo* icInfo = more_icons::activeIcon(m_currentIconType);
    auto size = this->m_mainLayer->getContentSize();
    auto popupSize = this->getContentSize();
    bool isRobotOrSpider = (m_currentIconType == IconType::Robot || m_currentIconType == IconType::Spider);
    bool isRiderMode = (m_currentIconType == IconType::Ship || m_currentIconType == IconType::Ufo || m_currentIconType == IconType::Jetpack);

    const float midX = size.width / 2.f;
    const float midY = size.height / 2.f;
    const float inputX = midX - 40.0f;
    const float inputYTop = midY + 25.0f;
    const float lowerMenuX = midX - 55.f;
    const float lowerMenuBaseY = midY - 52.5f;

    // -----------------------
    // DISABLE FOR VANILLA ICONS (L bozo sorry it's easier to work with MI) <- i am no longer sorry fuck you
    // -----------------------
    if (!icInfo) {
        auto warningLabel = CCLabelBMFont::create("Vanilla icons are not supported!\nPlease load your icons via\nMore Icons instead.", "bigFont.fnt");
        warningLabel->setPosition({midX, midY});
        warningLabel->setScale(0.4f);
        warningLabel->setAlignment(CCTextAlignment::kCCTextAlignmentCenter);
        warningLabel->setColor({255, 0, 0});

        auto whySpr = ButtonSprite::create("Why?", "goldFont.fnt", "GJ_button_01.png", 0.7f);
        auto whyBtn = CCMenuItemSpriteExtra::create(
            whySpr,
            this,
            menu_selector(IconOffsetEditorPopup::onWhy)
        );

        auto whyMenu = CCMenu::create();
        whyMenu->setPosition({0.f, 0.f});
        whyMenu->addChild(whyBtn);

        whyBtn->setPosition({midX, midY - 45.f});
        
        this->m_mainLayer->addChild(warningLabel);
        this->m_mainLayer->addChild(whyMenu);
        return true;
    }

    // -----------------------
    // MISC SETUPS
    // -----------------------
	m_iconNameLabel = CCLabelBMFont::create(fmt::format("Editing: {}", icInfo->getShortName()).c_str(), "chatFont.fnt");
	m_iconNameLabel->setPosition({midX, size.height - 35.f});
	m_iconNameLabel->setScale(0.5f);
    m_iconNameLabel->setOpacity(135);
	this->m_mainLayer->addChild(m_iconNameLabel);

    this->setOpacity(165);

    // -----------------------
    // TOP BUTTONS MENU
    // -----------------------
    auto infoSpr = CircleButtonSprite::create(CCSprite::create("helpIcon.png"_spr), CircleBaseColor::Cyan, CircleBaseSize::Small);
    infoSpr->setScale(0.8f);
    auto infoBtn = CCMenuItemSpriteExtra::create(infoSpr, this, menu_selector(IconOffsetEditorPopup::onInfoButton));

    auto renderSpr = CircleButtonSprite::create(CCSprite::create("renderIcon.png"_spr), CircleBaseColor::Green, CircleBaseSize::Small);
    renderSpr->setScale(0.8f);
    auto renderBtn = CCMenuItemSpriteExtra::create(renderSpr, this, menu_selector(IconOffsetEditorPopup::onRenderIcon));

    auto openFolderSpr = CircleButtonSprite::create(CCSprite::createWithSpriteFrameName("folderIcon_001.png"), CircleBaseColor::Green, CircleBaseSize::Small);
    openFolderSpr->setScale(0.8f);
    auto openFolderBtn = CCMenuItemSpriteExtra::create(openFolderSpr, this, menu_selector(IconOffsetEditorPopup::onOpenRendersFolder));

    infoBtn->setID("info-button");
    renderBtn->setID("render-icon");
    openFolderBtn->setID("open-renders-folder");

    auto topMenu = CCMenu::create();
    topMenu->setLayout(RowLayout::create()->setGap(3.f)->setAxisAlignment(AxisAlignment::Center)->setAutoScale(false));
    topMenu->addChild(infoBtn);
    topMenu->addChild(openFolderBtn);
    topMenu->addChild(renderBtn);
    topMenu->setID("top-right-menu");
    topMenu->setScale(0.5f);
    topMenu->setPosition({midX, size.height - 40.f});
    topMenu->updateLayout();
    this->m_mainLayer->addChild(topMenu);

    // -----------------------
    // SETTINGS BUTTON THINGY HI
    // -----------------------
    auto optionsBtn02Spr = CCSprite::createWithSpriteFrameName("GJ_optionsBtn02_001.png");
    auto optionsBtn02Btn = CCMenuItemSpriteExtra::create(optionsBtn02Spr, this, menu_selector(IconOffsetEditorPopup::onModSettings));
    auto optionsBtnMenu = CCMenu::create();

    optionsBtn02Spr->setScale(0.7f);
    optionsBtnMenu->setPosition({0.f, 0.f});
    optionsBtnMenu->addChild(optionsBtn02Btn);
    optionsBtnMenu->setID("options-menu");

    this->m_mainLayer->addChild(optionsBtnMenu);

    // -----------------------
    // SETUP ICON CONTAINER NODE
    // -----------------------
    m_iconContainerNode = CCNode::create();
    m_iconContainerNode->setContentSize({50.f, 50.f});
    m_iconContainerNode->setAnchorPoint({0.5f, 0.5f});
    m_iconContainerNode->setPosition({midX, midY});
    m_iconContainerNode->setScale(3.f);
    m_iconContainerNode->setZOrder(2);
    m_iconContainerNode->setID("icon-container");

    auto midContainerX = m_iconContainerNode->getContentSize().width / 2.f;
    auto midContainerY = m_iconContainerNode->getContentSize().height / 2.f;
    m_previewPlayer = SimplePlayer::create(0);
    m_previewPlayer->setZOrder(2);
    if (m_currentIconType == IconType::Ship) {
        m_previewPlayer->setPosition({midContainerX, midContainerY - 5.f});
    } else {
        m_previewPlayer->setPosition({midContainerX, midContainerY});
    }
    updatePreviewPlayer();
    
    m_iconContainerNode->addChild(m_previewPlayer);
    this->m_mainLayer->addChild(m_iconContainerNode);

    // underlay
    auto containerUnderlay = NineSlice::create("square02_small.png");
    containerUnderlay->setID("icon-container-underlay");
    containerUnderlay->setOpacity(35);
    containerUnderlay->setPosition({midX, midY});
    containerUnderlay->setContentSize({50.f * 4.f, 50.f * 3.f});
    this->m_mainLayer->addChild(containerUnderlay, 0);

    // -----------------------
    // CUBE PREVIEW FOR RIDER GAMEMODES
    // -----------------------
    if (m_currentIconType == IconType::Ship || m_currentIconType == IconType::Ufo || m_currentIconType == IconType::Jetpack) {        
        m_cubePreview = CCSprite::create("exampleCube.png"_spr);
        m_cubePreview->setID("example-cube-preview");
        
        // welcome back icon preview
        if (m_currentIconType == IconType::Ship) {
            m_cubePreview->setPosition({midContainerX, midContainerY + 5.f});
            m_cubePreview->setScale(0.55f);
        } else if (m_currentIconType == IconType::Ufo) {
            m_cubePreview->setPosition({midContainerX, midContainerY + 6.f});
            m_cubePreview->setScale(0.55f);
        } else if (m_currentIconType == IconType::Jetpack) {
            m_cubePreview->setPosition({midContainerX + 6.f, midContainerY + 4.f});
            m_cubePreview->setScale(0.6f);
        }
        
        m_iconContainerNode->addChild(m_cubePreview);
        
        // cube opacity slider
        auto opacityLabel = CCLabelBMFont::create("Cube Opacity:", "goldFont.fnt");
        opacityLabel->setPosition({lowerMenuX, lowerMenuBaseY + 15.f});
        opacityLabel->setScale(0.35f);
        this->m_mainLayer->addChild(opacityLabel);
        
        m_cubeOpacitySlider = Slider::create(
            this,
            menu_selector(IconOffsetEditorPopup::onExtraPreviewOpacityChanged),
            0.6f
        );
        m_cubeOpacitySlider->setValue(1.0f);
        m_cubeOpacitySlider->m_sliderBar->setContentSize({60.f, m_cubeOpacitySlider->m_sliderBar->getContentSize().height});
        
        auto opacityMenu = CCMenu::create();
        opacityMenu->addChild(m_cubeOpacitySlider);
        opacityMenu->setPosition({lowerMenuX, lowerMenuBaseY});
        opacityMenu->setID("cube-opacity-menu");
        this->m_mainLayer->addChild(opacityMenu);
        
        m_cubeOpacityLabel = CCLabelBMFont::create("100%", "bigFont.fnt");
        m_cubeOpacityLabel->setPosition({lowerMenuX, lowerMenuBaseY - 10.f});
        m_cubeOpacityLabel->setScale(0.25f);
        m_cubeOpacityLabel->setOpacity(150);
        m_cubeOpacityLabel->setID("cube-opacity-label");
        this->m_mainLayer->addChild(m_cubeOpacityLabel);
    }

    // -----------------------
    // SWING FIRE PREVIEW
    // -----------------------
    if (m_currentIconType == IconType::Swing) {
        m_swingTopFire = PlayerFireBoostSprite::create();
        m_swingMidFire = PlayerFireBoostSprite::create();
        m_swingBotFire = PlayerFireBoostSprite::create();

        m_iconContainerNode->addChild(m_swingTopFire, -2);
        m_iconContainerNode->addChild(m_swingMidFire, -2);
        m_iconContainerNode->addChild(m_swingBotFire, -2);

        m_swingTopFire->setPosition({midContainerX - 9.5f, midContainerY + 10.f});
        m_swingMidFire->setPosition({midContainerX - 14.f, midContainerY});
        m_swingBotFire->setPosition({midContainerX - 9.5f, midContainerY - 10.f});

        m_swingTopFire->setRotation(135);
        m_swingMidFire->setRotation(90);
        m_swingBotFire->setRotation(45);

        m_swingTopFire->setID("swing-top-fire-preview");
        m_swingMidFire->setID("swing-middle-fire-preview");
        m_swingBotFire->setID("swing-bottom-fire-preview");

        if (Mod::get()->getSettingValue<bool>("animate-swing-fires")) {
            m_swingTopFire->loopFireAnimation();
            m_swingMidFire->loopFireAnimation();
            m_swingBotFire->loopFireAnimation();
        }

        // control fire opacities
        auto opacityLabel = CCLabelBMFont::create("Swing Fires Opacity:", "goldFont.fnt");
        opacityLabel->setPosition({lowerMenuX, lowerMenuBaseY + 15.f});
        opacityLabel->setScale(0.35f);
        this->m_mainLayer->addChild(opacityLabel);
        
        m_cubeOpacitySlider = Slider::create(
            this,
            menu_selector(IconOffsetEditorPopup::onExtraPreviewOpacityChanged),
            0.6f
        );
        m_cubeOpacitySlider->setValue(1.0f);
        m_cubeOpacitySlider->m_sliderBar->setContentSize({60.f, m_cubeOpacitySlider->m_sliderBar->getContentSize().height});
        
        auto opacityMenu = CCMenu::create();
        opacityMenu->addChild(m_cubeOpacitySlider);
        opacityMenu->setPosition({lowerMenuX, lowerMenuBaseY});
        opacityMenu->setID("swing-fires-opacity-menu");
        this->m_mainLayer->addChild(opacityMenu);
        
        m_cubeOpacityLabel = CCLabelBMFont::create("100%", "bigFont.fnt");
        m_cubeOpacityLabel->setPosition({lowerMenuX, lowerMenuBaseY - 10.f});
        m_cubeOpacityLabel->setScale(0.25f);
        m_cubeOpacityLabel->setOpacity(150);
        this->m_mainLayer->addChild(m_cubeOpacityLabel);
    }

    // -----------------------
    // PREVIEW CONTROLS MENU (Glow + Hitbox)
    // -----------------------
    auto togglersColumn = UIUtils::column(4.f, AxisAlignment::Center, AxisAlignment::Center, false, "preview-togglers-column");
    togglersColumn->setPosition({midX + 90.f, midY - 70.f});

    auto glowRow = UIUtils::togglerRow("Glow", true, this, menu_selector(IconOffsetEditorPopup::onToggleGlow), 90.f, 0.35f, 0.6f, "glow-toggler-row");
    m_glowToggler = glowRow.toggler;
    togglersColumn->addChild(glowRow.container);

    auto hitboxRow = UIUtils::togglerRow("Hitbox", false, this, menu_selector(IconOffsetEditorPopup::onToggleHitbox), 90.f, 0.35f, 0.6f, "hitbox-toggler-row");
    m_hitboxToggler = hitboxRow.toggler;
    togglersColumn->addChild(hitboxRow.container);

    auto trailRow = UIUtils::togglerRow("Trail", false, this, menu_selector(IconOffsetEditorPopup::onToggleTrail), 90.f, 0.35f, 0.6f, "trail-toggler-row");
    m_trailToggler = trailRow.toggler;
    togglersColumn->addChild(trailRow.container);

    togglersColumn->updateLayout();
    this->m_mainLayer->addChild(togglersColumn);

    // -----------------------
    // HITBOX OPACITY
    // -----------------------
    auto hitboxOpacityContainer = UIUtils::column(2.f, AxisAlignment::Center, AxisAlignment::Center, false, "hitbox-opacity-container");
    hitboxOpacityContainer->setPosition({midX, size.height - 55.f});

    auto hitboxOpacityLabel = CCLabelBMFont::create("Hitbox Border Opacity:", "goldFont.fnt");
    hitboxOpacityLabel->setScale(0.35f);
    hitboxOpacityLabel->setID("hitbox-opacity-label-text");
    hitboxOpacityContainer->addChild(hitboxOpacityLabel);

    m_hitboxOpacitySlider = Slider::create(this, menu_selector(IconOffsetEditorPopup::onHitboxOpacityChanged), 0.6f);
    m_hitboxOpacitySlider->setValue(1.0f);
    m_hitboxOpacitySlider->m_sliderBar->setContentSize({60.f, m_hitboxOpacitySlider->m_sliderBar->getContentSize().height});
    auto hitboxOpacityMenu = CCMenu::create();
    hitboxOpacityMenu->addChild(m_hitboxOpacitySlider);
    hitboxOpacityMenu->setID("hitbox-opacity-menu");
    hitboxOpacityContainer->addChild(hitboxOpacityMenu);

    m_hitboxOpacityLabel = CCLabelBMFont::create("100%", "bigFont.fnt");
    m_hitboxOpacityLabel->setScale(0.25f);
    m_hitboxOpacityLabel->setOpacity(150);
    m_hitboxOpacityLabel->setID("hitbox-opacity-value-label");
    hitboxOpacityContainer->addChild(m_hitboxOpacityLabel);

    hitboxOpacityContainer->updateLayout();
    this->m_mainLayer->addChild(hitboxOpacityContainer);

    if (isRobotOrSpider) {
        auto robotSprite = (m_currentIconType == IconType::Robot) ? 
            m_previewPlayer->m_robotSprite : nullptr;
        auto spiderSprite = (m_currentIconType == IconType::Spider) ? 
            m_previewPlayer->m_spiderSprite : nullptr;
        
        if (robotSprite && robotSprite->m_glowSprite) {
            robotSprite->m_glowSprite->setVisible(true);
        }
        if (spiderSprite && spiderSprite->m_glowSprite) {
            spiderSprite->m_glowSprite->setVisible(true);
        }
    } else {
        if (m_previewPlayer->m_outlineSprite) m_previewPlayer->m_outlineSprite->setVisible(true);
    }

    m_hitboxDrawNode = CCDrawNode::create();
    m_hitboxDrawNode->setZOrder(10);
    m_hitboxDrawNode->setVisible(false);
    m_hitboxDrawNode->setID("hitbox-preview");
    //if (m_currentIconType == IconType::Ship) m_hitboxDrawNode->setPosition({0.f, 10.f});
    this->m_mainLayer->addChild(m_hitboxDrawNode);

    drawHitbox();

    // -----------------------
    // ROBOT/SPIDER ANIMATIONS MENU
    // -----------------------
    if (isRobotOrSpider) {
        m_animButtonsMenu = CCMenu::create();
        m_animButtonsMenu->setPosition({lowerMenuX, lowerMenuBaseY - 5.f});
        m_animButtonsMenu->setScale(0.6f);
        m_animButtonsMenu->setContentSize({200.f, 40.f});
        m_animButtonsMenu->setID("animation-players-menu");
        m_animButtonsMenu->setLayout(
            RowLayout::create()
                ->setGap(4.f)
                ->setAxisAlignment(AxisAlignment::Center)
                ->setAxisReverse(false)
        );
        this->m_mainLayer->addChild(m_animButtonsMenu);

        auto animDescLabel = CCLabelBMFont::create("Test Animations", "goldFont.fnt");
        animDescLabel->setPosition({lowerMenuX, lowerMenuBaseY + 15.f});
        animDescLabel->setScale(0.4f);
        animDescLabel->setID("test-animations-label");
        this->m_mainLayer->addChild(animDescLabel);
        
        if (m_currentIconType == IconType::Robot) {
            auto createAnimBtn = [this](const char* label, const char* animName) {
                auto lbl = CCLabelBMFont::create(label, "bigFont.fnt");
                lbl->setScale(0.4f);
                auto btn = CCMenuItemSpriteExtra::create(
                    CircleButtonSprite::create(lbl, CircleBaseColor::Gray, CircleBaseSize::Small),
                    this,
                    menu_selector(IconOffsetEditorPopup::onPlayAnimation)
                );
                btn->setUserObject("anim-name"_spr, CCString::create(animName));
                btn->setID(fmt::format("play-{}-anim", animName));
                return btn;
            };
            
            m_animButtonsMenu->addChild(createAnimBtn("idle", "idle"));
            m_animButtonsMenu->addChild(createAnimBtn("idle01", "idle01"));
            m_animButtonsMenu->addChild(createAnimBtn("run", "run"));
            m_animButtonsMenu->addChild(createAnimBtn("run2", "run2"));
            m_animButtonsMenu->addChild(createAnimBtn("run3", "run3"));
        } else {
            auto createAnimBtn = [this](const char* label, const char* animName) {
                auto lbl = CCLabelBMFont::create(label, "bigFont.fnt");
                lbl->setScale(0.4f);
                auto btn = CCMenuItemSpriteExtra::create(
                    CircleButtonSprite::create(lbl, CircleBaseColor::Gray, CircleBaseSize::Small),
                    this,
                    menu_selector(IconOffsetEditorPopup::onPlayAnimation)
                );
                btn->setUserObject("anim-name"_spr, CCString::create(animName));
                return btn;
            };
            
            m_animButtonsMenu->addChild(createAnimBtn("idle", "idle"));
            m_animButtonsMenu->addChild(createAnimBtn("idle01", "idle01"));
            m_animButtonsMenu->addChild(createAnimBtn("walk", "walk"));
            m_animButtonsMenu->addChild(createAnimBtn("run", "run"));
            m_animButtonsMenu->addChild(createAnimBtn("run2", "run2"));
        }
        
        m_animButtonsMenu->updateLayout();
    }

    // -----------------------
    // BALL ROTATION PREVIEW CONTROLS
    // -----------------------
    if (m_currentIconType == IconType::Ball) {
        m_animButtonsMenu = CCMenu::create();
        m_animButtonsMenu->setPosition({lowerMenuX, lowerMenuBaseY - 18.f});
        m_animButtonsMenu->setContentSize({55.f, 40.f});
        m_animButtonsMenu->setLayout(
            RowLayout::create()
                ->setGap(6.f)
                ->setAxisAlignment(AxisAlignment::Center)
        );
        m_animButtonsMenu->setScale(0.7f);
        this->m_mainLayer->addChild(m_animButtonsMenu);
        
        auto playLbl = CCLabelBMFont::create("Spin", "bigFont.fnt");
        playLbl->setScale(0.35f);
        auto playBtn = CCMenuItemSpriteExtra::create(
            CircleButtonSprite::create(playLbl, CircleBaseColor::Green, CircleBaseSize::Small),
            this,
            menu_selector(IconOffsetEditorPopup::onPlayBallRotation)
        );
        playBtn->setID("play-rolling");
        
        auto stopLbl = CCLabelBMFont::create("Stop", "bigFont.fnt");
        stopLbl->setScale(0.35f);
        auto stopBtn = CCMenuItemSpriteExtra::create(
            CircleButtonSprite::create(stopLbl, CircleBaseColor::Gray, CircleBaseSize::Small),
            this,
            menu_selector(IconOffsetEditorPopup::onStopBallRotation)
        );
        stopBtn->setID("stop-rolling");
        
        m_animButtonsMenu->addChild(playBtn);
        m_animButtonsMenu->addChild(stopBtn);
        m_animButtonsMenu->updateLayout();
        
        // rotation speed slider
        auto speedLabel = CCLabelBMFont::create("Full Spin Duration:", "bigFont.fnt");
        speedLabel->setPosition({lowerMenuX - 15.f, lowerMenuBaseY + 15.f});
        speedLabel->setScale(0.3f);
        speedLabel->setID("spin-label");
        this->m_mainLayer->addChild(speedLabel);
        
        m_rotationSpeedSlider = Slider::create(
            this, 
            menu_selector(IconOffsetEditorPopup::onRotationSpeedChanged), 
            0.6f
        );
        m_rotationSpeedSlider->setValue(0.5f);
        m_rotationSpeedSlider->m_sliderBar->setContentSize({80.f, m_rotationSpeedSlider->m_sliderBar->getContentSize().height});
        
        auto sliderMenu = CCMenu::create();
        sliderMenu->addChild(m_rotationSpeedSlider);
        sliderMenu->setPosition({lowerMenuX, lowerMenuBaseY});
        sliderMenu->setID("slider-menu");
        this->m_mainLayer->addChild(sliderMenu);
        
        m_rotationSpeedLabel = CCLabelBMFont::create("1.0", "bigFont.fnt");
        m_rotationSpeedLabel->setPosition({lowerMenuX + 50.f, lowerMenuBaseY + 15.f});
        m_rotationSpeedLabel->setScale(0.3f);
        m_rotationSpeedLabel->setID("rotation-speed-label");
        this->m_mainLayer->addChild(m_rotationSpeedLabel);
    }

    // -----------------------
    // COLOR PICKER ROWS
    // -----------------------
    m_previewColor1 = manager->colorForIdx(manager->getPlayerColor());
    m_previewColor2 = manager->colorForIdx(manager->getPlayerColor2());
    m_previewGlowColor = manager->colorForIdx(manager->getPlayerGlowColor());

    auto colorColumn = UIUtils::column(4.f, AxisAlignment::Center, AxisAlignment::Start, false, "color-picker-column");
    colorColumn->setPosition({midX - 170.f, midY});

    auto color1Row = UIUtils::colorPickerRow("Col 1", m_previewColor1, this, menu_selector(IconOffsetEditorPopup::onColorPicker), 90.f, 0.4f, "color1-row");
    color1Row.button->setUserObject("color-id"_spr, CCString::create("color1"));
    colorColumn->addChild(color1Row.container);

    auto color2Row = UIUtils::colorPickerRow("Col 2", m_previewColor2, this, menu_selector(IconOffsetEditorPopup::onColorPicker), 90.f, 0.4f, "color2-row");
    color2Row.button->setUserObject("color-id"_spr, CCString::create("color2"));
    colorColumn->addChild(color2Row.container);

    auto glowColorRow = UIUtils::colorPickerRow("Glow", m_previewGlowColor, this, menu_selector(IconOffsetEditorPopup::onColorPicker), 90.f, 0.4f, "glow-color-row");
    glowColorRow.button->setUserObject("color-id"_spr, CCString::create("glow"));
    colorColumn->addChild(glowColorRow.container);

    colorColumn->updateLayout();
    this->m_mainLayer->addChild(colorColumn);

    // -----------------------
    // OFFSET CONTROLS
    // -----------------------
    auto offsetsColumn = UIUtils::column(10.f, AxisAlignment::Center, AxisAlignment::Start, false, "offsets-column");
    offsetsColumn->setAnchorPoint({0.f, 1.f});
    offsetsColumn->setPosition({25.f, size.height - 15.f});

    auto offsetXRow = UIUtils::offsetRow(
        "Offset X:", this,
        menu_selector(IconOffsetEditorPopup::onNudgeOffsetXDown),
        menu_selector(IconOffsetEditorPopup::onNudgeOffsetXUp),
        menu_selector(IconOffsetEditorPopup::onAddToOffsetX),
        70.f, "x-offset"
    );
    m_inputX = offsetXRow.input;
    m_labelX = nullptr;
    if (Mod::get()->getSettingValue<bool>("update-offsets-live")) {
        m_inputX->setCallback([this](std::string const&) { this->onUpdateOffsets(nullptr); });
    }
    offsetsColumn->addChild(offsetXRow.container);

    auto offsetYRow = UIUtils::offsetRow(
        "Offset Y:", this,
        menu_selector(IconOffsetEditorPopup::onNudgeOffsetYDown),
        menu_selector(IconOffsetEditorPopup::onNudgeOffsetYUp),
        menu_selector(IconOffsetEditorPopup::onAddToOffsetY),
        70.f, "y-offset",
        true
    );
    m_inputY = offsetYRow.input;
    m_labelY = nullptr;
    if (Mod::get()->getSettingValue<bool>("update-offsets-live")) {
        m_inputY->setCallback([this](std::string const&) { this->onUpdateOffsets(nullptr); });
    }
    offsetsColumn->addChild(offsetYRow.container);

    offsetsColumn->updateLayout();
    this->m_mainLayer->addChild(offsetsColumn);

    // -----------------------
    // UPDATE / APPLY BUTTONS
    // -----------------------
    auto updateBtnSpr = ButtonSprite::create("Update", "goldFont.fnt", "GJ_button_01.png", 0.7f);
    m_updateButton = CCMenuItemSpriteExtra::create(updateBtnSpr, this, menu_selector(IconOffsetEditorPopup::onUpdateOffsets));
    m_updateButton->setID("update-offsets");

    auto savePlistSpr = ButtonSprite::create("Apply", "goldFont.fnt", "GJ_button_01.png", 0.7f);
    auto savePlistBtn = CCMenuItemSpriteExtra::create(savePlistSpr, this, menu_selector(IconOffsetEditorPopup::onSavePlist));
    savePlistBtn->setID("save-to-plist");

    auto buttonMenu = CCMenu::create();
    buttonMenu->addChild(m_updateButton);
    buttonMenu->addChild(savePlistBtn);
    buttonMenu->setLayout(RowLayout::create()->setGap(4.f)->setAxisAlignment(AxisAlignment::Center));
    buttonMenu->setContentSize({130.f, 30.f});
    buttonMenu->setAnchorPoint({0.5f, 0.5f});
    buttonMenu->setPosition({midX, 0.f});
    buttonMenu->setID("lower-button-menu");
    buttonMenu->updateLayout();
    this->m_mainLayer->addChild(buttonMenu);
    
    // -----------------------
    // PART-SELECT SCROLL LAYER
    // -----------------------
    int partCount = 4;

    if (isRobotOrSpider) {
        if (!icInfo->getFrameNames().empty()) {
            m_frameNames = icInfo->getFrameNames();

            auto targetNode = (m_currentIconType == IconType::Robot)
                ? static_cast<CCNode*>(m_previewPlayer->m_robotSprite)
                : static_cast<CCNode*>(m_previewPlayer->m_spiderSprite);

            if (targetNode) mapRobotSpiderSprites(targetNode);
            if (!m_frameNames.empty()) m_currentFrameName = m_frameNames[0];
        } else {
            log::error("couldn't get More Icons frameNames");
            return true;
        }
    }

    m_partScrollLayer = ScrollLayer::create({IconPartCell::WIDTH, 250.f});
    m_partScrollLayer->setID("part-select-scroll-layer");
    m_partScrollLayer->setPosition({size.width - IconPartCell::WIDTH - 15.f, midY - 110.f});
    this->m_mainLayer->addChild(m_partScrollLayer);

    setupPartScrollLayer();

    updateInputFields();
    highlightSelectedButton();
    
    return true;
}

void IconOffsetEditorPopup::setupPartScrollLayer() {
    bool isRobotOrSpider = (m_currentIconType == IconType::Robot || m_currentIconType == IconType::Spider);

    std::vector<IconPartCell*> cells;

    if (isRobotOrSpider) {
        for (int i = 0; i < static_cast<int>(m_frameNames.size()); i++) {
            const auto& frameName = m_frameNames[i];
            auto frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(frameName.c_str());
            if (!frame || frame->getTag() == FALLBACK_TAG) {
                log::warn("couldn't find frame: {}", frameName);
                continue;
            }

            auto cell = IconPartCell::create(
                frame,
                getRealFrameName(frameName),
                frameName,
                i,
                cells.size() % 2 == 0,
                this,
                menu_selector(IconOffsetEditorPopup::onPartSelected)
            );

            m_frameCells[frameName] = cell;
            cells.push_back(cell);
        }
    } else {
        auto addPartCell = [&](SelectedSpritePart part, CCSprite* sprite, const char* displayName) {
            if (!sprite || !sprite->isVisible()) return;

            auto cell = IconPartCell::create(
                sprite->displayFrame(),
                displayName,
                displayName,
                static_cast<int>(part),
                cells.size() % 2 == 0,
                this,
                menu_selector(IconOffsetEditorPopup::onPartSelected)
            );

            m_partCells[part] = cell;
            cells.push_back(cell);
        };

        addPartCell(SelectedSpritePart::FirstLayer, m_previewPlayer->m_firstLayer, "Primary");
        addPartCell(SelectedSpritePart::SecondLayer, m_previewPlayer->m_secondLayer, "Secondary");
        addPartCell(SelectedSpritePart::Outline, m_previewPlayer->m_outlineSprite, "Glow");
        addPartCell(SelectedSpritePart::Detail, m_previewPlayer->m_detailSprite, "Extra");

        if (m_currentIconType == IconType::Ufo && m_previewPlayer->m_birdDome) {
            addPartCell(SelectedSpritePart::Dome, m_previewPlayer->m_birdDome, "UFO Dome");
        }
    }

    for (size_t i = 0; i < cells.size(); i++) {
        cells[i]->setPosition({0.f, IconPartCell::HEIGHT * static_cast<float>(cells.size() - 1 - i)});
        m_partScrollLayer->m_contentLayer->addChild(cells[i]);
    }

    m_partScrollLayer->m_contentLayer->setContentSize({
        m_partScrollLayer->m_contentLayer->getContentSize().width,
        IconPartCell::HEIGHT * static_cast<float>(cells.size())
    });
    m_partScrollLayer->moveToTop();
}

void IconOffsetEditorPopup::updatePreviewPlayer() {
    if (!m_previewPlayer) return;
    updatePreviewIcon(m_previewPlayer, m_currentIconType);
}

CCSprite* IconOffsetEditorPopup::getCurrentSelectedSprite() {
    if (!m_previewPlayer) return nullptr;
    
    if (m_currentIconType != IconType::Robot && m_currentIconType != IconType::Spider) {
        switch(m_selectedPart) {
            case SelectedSpritePart::FirstLayer:
                return m_previewPlayer->m_firstLayer;
            case SelectedSpritePart::SecondLayer:
                return m_previewPlayer->m_secondLayer;
            case SelectedSpritePart::Outline:
                return m_previewPlayer->m_outlineSprite;
            case SelectedSpritePart::Detail:
                return m_previewPlayer->m_detailSprite;
            case SelectedSpritePart::Dome:
                return m_previewPlayer->m_birdDome;
            default:
                return nullptr;
        }
    }
    
    auto frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(m_currentFrameName.c_str());
    if (frame) {
        return nullptr;
    }
    
    return nullptr;
}

std::string IconOffsetEditorPopup::getCurrentRealFrameName() {
    if (m_currentIconType == IconType::Robot || m_currentIconType == IconType::Spider) {
        return getRealFrameName(m_currentFrameName);
    }
    
    auto sprite = getCurrentSelectedSprite();
    if (!sprite) {
        log::warn("getCurrentSelectedSprite returned nullptr");
        return "";
    }
    
    auto frame = sprite->displayFrame();
    if (!frame) {
        log::warn("sprite->displayFrame() returned nullptr");
        return "";
    }
    
    auto texture = frame->getTexture();
    if (!texture) {
        log::warn("Frame has null texture");
        return "";
    }
    
    auto rect = frame->getRect();
    
    auto icInfo = more_icons::activeIcon(m_currentIconType);
    if (!icInfo || icInfo->getFrameNames().empty()) {
        log::warn("Couldn't get IconInfo or frameNames is empty");
        return "";
    }
    
    for (const auto& fullFrameName : icInfo->getFrameNames()) {
        auto cachedFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(fullFrameName.c_str());
        
        if (!cachedFrame || cachedFrame->getTag() == FALLBACK_TAG) {
            continue;
        }
        
        auto cachedTexture = cachedFrame->getTexture();
        if (!cachedTexture) {
            continue;
        }
        
        auto cachedRect = cachedFrame->getRect();
        
        bool textureMatches = (texture == cachedTexture);
        bool rectMatches = (rect.equals(cachedRect));
        
        if (textureMatches && rectMatches) {
            std::string realName = getRealFrameName(fullFrameName);
            //log::info("Found matching frame: {} -> {}", fullFrameName, realName);
            return realName;
        }
    }
    
    log::warn("No matching frame found for current sprite");
    return "";
}

void IconOffsetEditorPopup::updateInputFields() {
    std::string realFrameName = getCurrentRealFrameName();
    
    if (realFrameName.empty()) {
        log::warn("Couldn't get real frame name");
        return;
    }
    
    if (m_modifiedOffsets.count(realFrameName)) {
        auto offset = m_modifiedOffsets[realFrameName];
        m_inputX->setString(fmt::format("{:.6g}", offset.x));
        m_inputY->setString(fmt::format("{:.6g}", offset.y));
        return;
    }

    if (m_currentIconType == IconType::Robot || m_currentIconType == IconType::Spider) {
        auto frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(m_currentFrameName.c_str());
        if (frame && frame->getTag() != FALLBACK_TAG) {
            auto offset = frame->getOffsetInPixels();
            m_inputX->setString(fmt::format("{:.6g}", offset.x));
            m_inputY->setString(fmt::format("{:.6g}", offset.y));
        }
    } else {
        auto sprite = getCurrentSelectedSprite();
        if (sprite) {
            auto frame = sprite->displayFrame();
            if (frame) {
                auto offset = frame->getOffsetInPixels();
                m_inputX->setString(fmt::format("{:.6g}", offset.x));
                m_inputY->setString(fmt::format("{:.6g}", offset.y));
            }
        }
    }
}

void IconOffsetEditorPopup::onPartSelected(CCObject* sender) {
    auto button = static_cast<CCMenuItemSpriteExtra*>(sender);
    if (!button) return;

    if (m_currentIconType == IconType::Robot || m_currentIconType == IconType::Spider) {
        auto idObj = typeinfo_cast<CCString*>(button->getUserObject("part-id"_spr));
        if (idObj) m_currentFrameName = idObj->getCString();
    } else {
        m_selectedPart = static_cast<SelectedSpritePart>(button->getTag());
    }

    updateInputFields();
    highlightSelectedButton();
}

void IconOffsetEditorPopup::onInfoButton(CCObject* sender) {
    MDPopup::create("Icon Construct Info", infoStr, "OK")->show();
}

void IconOffsetEditorPopup::onWhy(CCObject* sender) {
    MDPopup::create("Icon Construct Info", whyStr, "OK")->show();
}

void IconOffsetEditorPopup::onToggleGlow(CCObject* sender) {

    if (m_currentIconType == IconType::Robot || m_currentIconType == IconType::Spider) {
        auto robotSprite = (m_currentIconType == IconType::Robot) ? 
            m_previewPlayer->m_robotSprite : nullptr;
        auto spiderSprite = (m_currentIconType == IconType::Spider) ? 
            m_previewPlayer->m_spiderSprite : nullptr;
        
        if (robotSprite && robotSprite->m_glowSprite) {
            robotSprite->m_glowSprite->setVisible(!robotSprite->m_glowSprite->isVisible());
        }
        if (spiderSprite && spiderSprite->m_glowSprite) {
            spiderSprite->m_glowSprite->setVisible(!spiderSprite->m_glowSprite->isVisible());
        }
    } else {
        if (m_previewPlayer->m_outlineSprite) m_previewPlayer->m_outlineSprite->setVisible(!m_previewPlayer->m_outlineSprite->isVisible());
    }
}

void IconOffsetEditorPopup::drawHitbox() {
    if (!m_hitboxDrawNode || !m_previewPlayer) return;
    
    m_hitboxDrawNode->clear();
    
    auto hitboxSize = getHitboxSizeForIconType(m_currentIconType);
    auto playerPos = m_iconContainerNode->getPosition();

    bool drawFill = Mod::get()->getSettingValue<bool>("draw-hitbox-fill");
    
    CCRect hitboxRect = {
        playerPos.x - hitboxSize.width / 2.f,
        playerPos.y - hitboxSize.height / 2.f,
        hitboxSize.width,
        hitboxSize.height
    };
    
    ccColor4F borderColor = {1.f, 0.f, 0.f, m_hitboxOpacity};
    ccColor4F fillColor;
    float borderSize = 0.5f;

    if (drawFill) {
        fillColor = {1.f, 0.f, 0.f, 0.15f * m_hitboxOpacity};
    } else {
        fillColor = {0.f, 0.f, 0.f, 0.f};
    }
    
    std::array<CCPoint, 4> vertices = {
        CCPoint{hitboxRect.getMinX(), hitboxRect.getMinY()},
        CCPoint{hitboxRect.getMinX(), hitboxRect.getMaxY()},
        CCPoint{hitboxRect.getMaxX(), hitboxRect.getMaxY()},
        CCPoint{hitboxRect.getMaxX(), hitboxRect.getMinY()}
    };
    
    m_hitboxDrawNode->drawPolygon(vertices.data(), 4, fillColor, borderSize, borderColor);
}

void IconOffsetEditorPopup::onToggleHitbox(CCObject* sender) {
    m_showHitbox = !m_showHitbox;
    
    if (m_hitboxDrawNode) {
        m_hitboxDrawNode->setVisible(m_showHitbox);
    }
}

void IconOffsetEditorPopup::onPlayAnimation(CCObject* sender) {
    auto btn = static_cast<CCMenuItemSpriteExtra*>(sender);
    auto animName = typeinfo_cast<CCString*>(btn->getUserObject("anim-name"_spr));
    if (!animName) return;
    
    if (m_currentIconType == IconType::Robot && m_previewPlayer->m_robotSprite) {
        m_previewPlayer->m_robotSprite->runAnimation(animName->getCString());
    } else if (m_currentIconType == IconType::Spider && m_previewPlayer->m_spiderSprite) {
        m_previewPlayer->m_spiderSprite->runAnimation(animName->getCString());
    }
}

void IconOffsetEditorPopup::onPlayBallRotation(CCObject* sender) {
    if (!m_previewPlayer) return;
    
    m_previewPlayer->stopAllActions();
    m_previewPlayer->setRotation(0);
    
    float speed = 0.1f + (m_rotationSpeedSlider->getValue() * 1.9f);
    
    auto rotate = CCRotateBy::create(speed, 360.f);
    auto repeat = CCRepeatForever::create(rotate);
    m_previewPlayer->runAction(repeat);
    
    m_isRotating = true;
}

void IconOffsetEditorPopup::onStopBallRotation(CCObject* sender) {
    if (!m_previewPlayer) return;
    
    m_previewPlayer->stopAllActions();
    m_previewPlayer->setRotation(0);
    m_isRotating = false;
}

void IconOffsetEditorPopup::onRotationSpeedChanged(CCObject* sender) {
    float speed = 0.1f + (m_rotationSpeedSlider->getValue() * 1.9f);
    
    if (m_rotationSpeedLabel) m_rotationSpeedLabel->setString(fmt::format("{:.1f}s", speed).c_str());
    if (m_isRotating) onPlayBallRotation(nullptr);
}

void IconOffsetEditorPopup::onExtraPreviewOpacityChanged(CCObject* sender) {
    bool isRiderMode = (m_currentIconType == IconType::Ship || m_currentIconType == IconType::Ufo || m_currentIconType == IconType::Jetpack);
    
    float opacity = m_cubeOpacitySlider->getValue();

    if (isRiderMode) {
        if (!m_cubePreview) return;
        m_cubePreview->setOpacity(static_cast<GLubyte>(opacity * 255));
    } else if (m_currentIconType == IconType::Swing) {
        if (!m_swingBotFire || !m_swingMidFire || !m_swingTopFire) return;
        m_swingBotFire->setOpacity(static_cast<GLubyte>(opacity * 255));
        m_swingMidFire->setOpacity(static_cast<GLubyte>(opacity * 255));
        m_swingTopFire->setOpacity(static_cast<GLubyte>(opacity * 255));
    }
    
    if (m_cubeOpacityLabel) m_cubeOpacityLabel->setString(fmt::format("{}%", static_cast<int>(opacity * 100)).c_str());
}

void IconOffsetEditorPopup::onColorPicker(CCObject* sender) {
    auto menuItem = static_cast<CCMenuItemSpriteExtra*>(sender);
    if (!menuItem) return;
    
    auto colorIdStr = typeinfo_cast<CCString*>(menuItem->getUserObject("color-id"_spr));
    if (!colorIdStr) return;
    
    m_currentColorSettingId = std::string(colorIdStr->getCString());
    m_currentColorButtonSprite = typeinfo_cast<CCSprite*>(menuItem->getNormalImage());
    
    ccColor3B currentColor;
    if (m_currentColorSettingId == "color1") {
        currentColor = m_previewColor1;
    } else if (m_currentColorSettingId == "color2") {
        currentColor = m_previewColor2;
    } else if (m_currentColorSettingId == "glow") {
        currentColor = m_previewGlowColor;
    }
    
    auto colorPopup = geode::ColorPickPopup::create(currentColor);
    //colorPopup->setDelegate(this);
    colorPopup->setCallback([this](cocos2d::ccColor4B color) {
        this->updateColor(color);
    });
    colorPopup->show();
}

void IconOffsetEditorPopup::updateColor(cocos2d::ccColor4B const& color) {
    if (m_currentColorSettingId.empty()) return;
    
    ccColor3B color3B = {color.r, color.g, color.b};
    
    if (m_currentColorSettingId == "color1") {
        m_previewColor1 = color3B;
    } else if (m_currentColorSettingId == "color2") {
        m_previewColor2 = color3B;
    } else if (m_currentColorSettingId == "glow") {
        m_previewGlowColor = color3B;
    }
    
    if (m_currentColorButtonSprite) {
        m_currentColorButtonSprite->setColor(color3B);
    }
    
    applyPreviewColors();
    
    log::info("Updated {} to ({}, {}, {})", m_currentColorSettingId, color.r, color.g, color.b);
}

void IconOffsetEditorPopup::applyPreviewColors() {
    if (!m_previewPlayer) return;
    
    if (m_currentIconType == IconType::Robot || m_currentIconType == IconType::Spider) {
        auto robotSprite = (m_currentIconType == IconType::Robot) ? 
            m_previewPlayer->m_robotSprite : nullptr;
        auto spiderSprite = (m_currentIconType == IconType::Spider) ? 
            m_previewPlayer->m_spiderSprite : nullptr;
        
        if (robotSprite) {
            robotSprite->updateColor01(m_previewColor1);
            robotSprite->updateColor02(m_previewColor2);
            robotSprite->updateGlowColor(m_previewGlowColor, true);
        }
        if (spiderSprite) {
            spiderSprite->updateColor01(m_previewColor1);
            spiderSprite->updateColor02(m_previewColor2);
            spiderSprite->updateGlowColor(m_previewGlowColor, true);
        }
    } else {
        m_previewPlayer->setColor(m_previewColor1);
        m_previewPlayer->setSecondColor(m_previewColor2);
        m_previewPlayer->setGlowOutline(m_previewGlowColor);
        m_previewPlayer->enableCustomGlowColor(m_previewGlowColor);
    }
}

void IconOffsetEditorPopup::highlightSelectedButton() {
    bool isRobotOrSpider = (m_currentIconType == IconType::Robot || m_currentIconType == IconType::Spider);

    if (isRobotOrSpider) {
        for (auto& [frameName, cell] : m_frameCells) {
            cell->setSelected(frameName == m_currentFrameName);
        }
    } else {
        for (auto& [part, cell] : m_partCells) {
            cell->setSelected(part == m_selectedPart);
        }
    }
}

void IconOffsetEditorPopup::onUpdateOffsets(CCObject* sender) {
    float offsetX = 0.0f;
    float offsetY = 0.0f;
    
    std::string xStr = m_inputX->getString();
    std::string yStr = m_inputY->getString();

    if (!xStr.empty()) {
        auto xResult = geode::utils::numFromString<float>(xStr);
        if (!xResult) {
            log::error("couldn't parse X offset value: {}", xStr);
            return;
        }
        offsetX = xResult.unwrap();
    }

    if (!yStr.empty()) {
        auto yResult = geode::utils::numFromString<float>(yStr);
        if (!yResult) {
            log::error("couldn't parse Y offset value: {}", yStr);
            return;
        }
        offsetY = yResult.unwrap();
    }
    
    CCPoint newOffset = {offsetX, offsetY};
    std::string realFrameName = getCurrentRealFrameName();
    
    if (realFrameName.empty()) {
        log::error("Couldn't determine frame name");
        return;
    }
    
    m_modifiedOffsets[realFrameName] = newOffset;
    
    if (m_currentIconType == IconType::Robot || m_currentIconType == IconType::Spider) {
        auto robotSprite = (m_currentIconType == IconType::Robot) ? 
            m_previewPlayer->m_robotSprite : nullptr;
        auto spiderSprite = (m_currentIconType == IconType::Spider) ? 
            m_previewPlayer->m_spiderSprite : nullptr;
        
        CCNode* targetNode = robotSprite ? static_cast<CCNode*>(robotSprite) : static_cast<CCNode*>(spiderSprite);
        
        if (targetNode) {
            applyOffsetToAllMatchingSprites(targetNode, m_currentFrameName, newOffset);
        }
    } else {
        auto sprite = getCurrentSelectedSprite();
        if (sprite) {
            auto frame = sprite->displayFrame();
            if (frame) {
                frame->setOffsetInPixels(newOffset);
                sprite->setDisplayFrame(frame);
            }
        }
    }
    
    log::info("Updated offset for frame '{}' to ({}, {})", realFrameName, offsetX, offsetY);
}

void IconOffsetEditorPopup::onSavePlist(CCObject* sender) {
    processPlistSave(false);
}

void IconOffsetEditorPopup::processPlistSave(bool remapNames) {
    bool backupWasMade = false;

    if (m_modifiedOffsets.empty()) {
        FLAlertLayer::create("No Changes", "<cr>You haven't changed any offsets yet!</c> There's nothing to modify in your plist.\nMake some changes before applying!", "OK")->show();
        return;
    }

    auto icInfo = more_icons::activeIcon(m_currentIconType);
    if (!icInfo) {
        FLAlertLayer::create("Error", "<cr>Couldn't get icon info!</c>", "OK")->show();
        return;
    }

    std::string plistPath = icInfo->getSheetString();

    if (plistPath.empty()) {
        FLAlertLayer::create("Error", "<cr>Couldn't get the icon's plist!</c>\nPlease make sure your Texture Pack path is valid, and you're not trying to edit a zipped texture pack.", "OK")->show();
        return;
    }

    m_logStream.clear();
    addToLog("## Plist Editing Process", 0);
    
    if (remapNames) {
        addToLog("<cy>**Note:** Edits made while using frame name remapping workaround.</c>", 2);
    }

    std::string backupPath = plistPath + ".bak";
    auto readResult = utils::file::readString(plistPath);
    if (!readResult) {
        std::string errorMsg = fmt::format("**<cr>Error:</c>** Couldn't read contents of plist file.\n\n**Details:**\n```\n{}\n```", readResult.unwrapErr());

        geode::MDPopup::create(
            "Plist Edit Failed",
            errorMsg,
            "OK", nullptr,
            [](bool) {}
        )->show();
        return;
    }

    std::string plistContent = readResult.unwrap();

    std::vector<std::string> plistKeys;
    size_t keyPos = plistContent.find("<key>");
    while (keyPos != std::string::npos) {
        size_t endPos = plistContent.find("</key>", keyPos);
        if (endPos != std::string::npos) {
            std::string key = plistContent.substr(keyPos + 5, endPos - (keyPos + 5));
            if (key.ends_with(".png")) {
                plistKeys.push_back(key);
            }
        }
        keyPos = plistContent.find("<key>", keyPos + 1);
    }

    addToLog("---", 2);
    addToLog("### Modified Offsets:", 2);

    int updatedCount = 0;
    int notFoundCount = 0;
    std::vector<std::string> successfulUpdates;
    std::vector<std::string> failedUpdates;
    std::vector<std::string> notFoundFrames;

    for (const auto& [frameName, offset] : m_modifiedOffsets) {
        std::string searchFrameName = frameName;

        if (std::find(plistKeys.begin(), plistKeys.end(), frameName) != plistKeys.end()) {
            searchFrameName = frameName;
        } else {
            std::string suffix = getPreservedSuffix(frameName, m_currentIconType);
            std::vector<std::string> candidates;

            for (const auto& key : plistKeys) {
                if (key.ends_with(suffix)) {
                    candidates.push_back(key);
                }
            }

            if (!candidates.empty()) {
                searchFrameName = candidates[0]; 

                if (remapNames) {
                    addToLog(fmt::format("<cy>[SMART REMAP]</c> `{}` <cl>-></c> `{}`", frameName, searchFrameName), 1);
                }
            } else {
                searchFrameName = frameName.substr(getPrefixLen(m_currentIconType));
            }
        }

        std::string frameKey = fmt::format("<key>{}</key>", searchFrameName);
        size_t framePos = plistContent.find(frameKey);

        if (framePos == std::string::npos) {
            if (!remapNames) {
                notFoundFrames.push_back(searchFrameName);
                notFoundCount++;
            } else {
                failedUpdates.push_back(fmt::format("<cr>[ERROR]</c> frame **{}** (remapped to `{}`) not found in plist even after remapping.", searchFrameName, searchFrameName));
            }
            continue;
        }

        size_t frameDictStart = plistContent.find("<dict>", framePos);
        if (frameDictStart == std::string::npos) {
            failedUpdates.push_back(fmt::format("<cr>[ERROR]</c> opening <dict> for frame **{}** not found.", searchFrameName));
            continue;
        }

        size_t frameDictEnd = plistContent.find("</dict>", frameDictStart);
        if (frameDictEnd == std::string::npos) {
            failedUpdates.push_back(fmt::format("<cr>[ERROR]</c> closing </dict> for frame **{}** not found.", searchFrameName));
            continue;
        }

        size_t searchStart = frameDictStart;
        size_t searchEnd = frameDictEnd;

        std::string frameSection = plistContent.substr(searchStart, searchEnd - searchStart);
        size_t offsetKeyPos = frameSection.find("<key>spriteOffset</key>");

        if (offsetKeyPos == std::string::npos) {
            failedUpdates.push_back(fmt::format("<cr>[ERROR]</c> spriteOffset for frame **{}** not found.", searchFrameName));
            continue;
        }

        offsetKeyPos += searchStart;

        size_t stringStart = plistContent.find("<string>", offsetKeyPos);
        size_t stringEnd = plistContent.find("</string>", offsetKeyPos);

        if (stringStart == std::string::npos || stringEnd == std::string::npos ||
            stringStart > frameDictEnd || stringEnd > frameDictEnd) {
            failedUpdates.push_back(fmt::format("<cr>[ERROR]</c> spriteOffset string tags for frame **{}** not found.", searchFrameName));
            continue;
        }

        std::string newOffsetStr = fmt::format("{{{:.6g},{:.6g}}}",
            offset.x,
            offset.y);

        plistContent.replace(
            stringStart + 8,
            stringEnd - (stringStart + 8),
            newOffsetStr
        );

        if (remapNames) {
            successfulUpdates.push_back(fmt::format("**{}** <cl>-></c> `{}`: `{}`", searchFrameName, searchFrameName, newOffsetStr));
        } else {
            successfulUpdates.push_back(fmt::format("**{}**: `{}`", searchFrameName, newOffsetStr));
        }
        updatedCount++;
    }

    if (!remapNames && notFoundCount > 0 && notFoundCount == m_modifiedOffsets.size()) {
        fmt::memory_buffer retryMsg;
        fmt::format_to(std::back_inserter(retryMsg), "# <cr>Frames Not Found.</c>\n\n");
        fmt::format_to(std::back_inserter(retryMsg), "---");
        fmt::format_to(std::back_inserter(retryMsg), "None of the frames you updated and were about to be changed were found in the plist file.\n\n");
        fmt::format_to(std::back_inserter(retryMsg), "## <cy>This could be caused by a</c> <co>Renamed/Duplicate Icon</c>\n\n");
        fmt::format_to(std::back_inserter(retryMsg), "If you've renamed this icon using More Icons' rename feature, the plist file still contains the old frame names, therefore, the mod isn't able to find the frames by searching for the icon's new name inside the plist file.\n\n");
        fmt::format_to(std::back_inserter(retryMsg), "**For Example:**\n");
        fmt::format_to(std::back_inserter(retryMsg), "- You renamed: `player_85` to `renamedtest`\n");
        fmt::format_to(std::back_inserter(retryMsg), "- Construct is looking for: `renamedtest_001.png` to apply your changes.\n");
        fmt::format_to(std::back_inserter(retryMsg), "- But, the plist file still has: `player_85_001.png`\n\n");
        fmt::format_to(std::back_inserter(retryMsg), "### <cp>What Now?</co>\n\n");
        fmt::format_to(std::back_inserter(retryMsg), "Click **Remap and Retry** to try and automatically find and use the internal frame names in your plist, instead of your icon's name.\n\n");
        fmt::format_to(std::back_inserter(retryMsg), "Construct will then:\n");
        fmt::format_to(std::back_inserter(retryMsg), "1. Try and find the first instance of a frame ending with `_001.png` in your plist.\n");
        fmt::format_to(std::back_inserter(retryMsg), "2. Use this to try and find the icon's internal name. (e.g., `player_85_001.png` = `player_85`)\n");
        fmt::format_to(std::back_inserter(retryMsg), "3. Replace your renamed name with the internal name.\n");
        fmt::format_to(std::back_inserter(retryMsg), "4. Search for the correct frames to apply your changes.\n\n");
        fmt::format_to(std::back_inserter(retryMsg), "<cy>**Note:**</c> If anything goes wrong, a backup of your icon's plist has been created at:\n");
        fmt::format_to(std::back_inserter(retryMsg), "`{}`\n\n", backupPath);
        fmt::format_to(std::back_inserter(retryMsg), "So i'd recommend giving remapping a shot anyways.\n\n");
        fmt::format_to(std::back_inserter(retryMsg), "### <cr>Frames that weren't found:</cr>\n\n");
        for (const auto& frame : notFoundFrames) {
            fmt::format_to(std::back_inserter(retryMsg), "- `{}`\n", frame);
        }

        std::string retryMsgStr = fmt::to_string(retryMsg);
        
        geode::MDPopup::create(
            "Couldn't find Frames",
            retryMsgStr,
            "Cancel", "Remap and Retry",
            [this](bool btn2) {
                if (btn2) {
                    processPlistSave(true);
                }
            }
        )->show();
        
        return;
    }

    if (!successfulUpdates.empty()) {
        addToLog(fmt::format("### <cg>Successfully Applied Updates: ({})</c>", updatedCount), 2);
        for (const auto& update : successfulUpdates) {
            addToLog(fmt::format("- <cg>{}</c>", update), 0);
        }
    }

    if (!failedUpdates.empty()) {
        addToLog(fmt::format("### <cr>Unapplied/Failed Updates ({})</c>", failedUpdates.size()), 2);
        for (const auto& failure : failedUpdates) {
            addToLog(fmt::format("- {}", failure), 0);
        }
    }

    if (updatedCount == 0) {
        addToLog("<cr>**Error:**</c> No offsets updated on plist file.", 2);
        addToLog("Please check the failed updates above for details, and send the log in your bug report if you think this is a mod issue.", 0);

        std::string finalLog = fmt::to_string(m_logStream);

        geode::MDPopup::create(
            "Plist Save Failed",
            finalLog,
            "OK", "Copy Log",
            [finalLog](bool btn2) {
                if (btn2) {
                    utils::clipboard::write(finalLog);
                    Notification::create("Log copied to clipboard!", NotificationIcon::Success)->show();
                }
            }
        )->show();
        return;
    }

    auto writeResult = utils::file::writeString(plistPath, plistContent);
    if (!writeResult) {
        addToLog("<cr>**Error:**</c> Failed to write to plist file.", 2);
        addToLog(fmt::format("**Error Details:**\n```\n{}\n```", writeResult.unwrapErr()), 0);

        std::string finalLog = fmt::to_string(m_logStream);

        geode::MDPopup::create(
            "Plist Save Failed",
            finalLog,
            "OK", "Copy Log",
            [finalLog](bool btn2) {
                if (btn2) {
                    utils::clipboard::write(finalLog);
                    Notification::create("Log copied to clipboard!", NotificationIcon::Success)->show();
                }
            }
        )->show();
        return;
    }

    more_icons::updateIcon(icInfo);

    addToLog("---", 2);
    addToLog("## <cg>Success!</c>", 2);
    addToLog(fmt::format("Plist file successfully updated with **{}** changes!", updatedCount), 2);
    addToLog(fmt::format("**Plist Path:**\n`{}`", plistPath), 1);
    addToLog(fmt::format("**Backup Path:**\n`{}`", backupPath), 1);
    
    if (remapNames) {
        addToLog("---", 2);
        addToLog("<cy>**Smart Frame Name Remapping was used**</c>", 2);
        addToLog("Automatically detected and mapped structural suffixes to bypass MoreIcons prefixing.", 1);
        addToLog("If anything goes wrong or your icon looks bad after these changes, you can restore from the created backup file.", 1);
    }
    
    if (backupWasMade) {
        addToLog("---", 2);
        addToLog("<cg>Plist backup exists!</c>", 2);
        addToLog(fmt::format("**Backup is saved to:** `{}`", backupPath), 1);
    }

    std::string finalLog = fmt::to_string(m_logStream);

    geode::MDPopup::create(
        "Plist Saved Successfully",
        finalLog,
        "OK", "Copy Log",
        [finalLog](bool btn2) {
            if (btn2) {
                utils::clipboard::write(finalLog);
                Notification::create("Log copied to clipboard!", NotificationIcon::Success)->show();
            }
        }
    )->show();

    m_modifiedOffsets.clear();
}

void IconOffsetEditorPopup::addToLog(const std::string& logMsg, int newLines) {
    for (int i = 0; i < newLines; ++i) m_logStream.push_back('\n');
    fmt::format_to(std::back_inserter(m_logStream), "{}\n", logMsg);
}

CCImage* IconOffsetEditorPopup::getIconImage() {
    if (!m_previewPlayer || !m_iconContainerNode) {
        log::error("Preview player or container node is null");
        return nullptr;
    }

    auto origIconPos = m_previewPlayer->getPosition();
    auto origContainerScale = m_iconContainerNode->getScale();
    auto origContainerPos = m_iconContainerNode->getPosition();

    auto bgCol = Mod::get()->getSettingValue<ccColor4B>("bg-color");
    auto canvasSize = m_iconContainerNode->getContentSize();

    int extraW = (m_currentIconType == IconType::Ship || m_currentIconType == IconType::Ufo) ? 6 : 0;
    int extraH = (m_currentIconType == IconType::Robot || m_currentIconType == IconType::Spider) ? 8 : 0;

    if (m_currentIconType == IconType::Swing && Mod::get()->getSettingValue<bool>("extra-swing-space")) {
        extraW = extraW + 15.f;
        extraH = extraH + 8.f;
    }

    int texWidth = static_cast<int>(canvasSize.width) + extraW;
    int texHeight = static_cast<int>(canvasSize.height) + extraH;

    float midX = texWidth / 2.f;
    float midY = texHeight / 2.f;

    auto renderTex = CCRenderTexture::create(texWidth, texHeight, kCCTexture2DPixelFormat_RGBA8888);
    if (!renderTex) {
        log::warn("couldn't create render texture.");
        return nullptr;
    }

    m_iconContainerNode->setScale(1.0f);
    m_iconContainerNode->setPosition({texWidth / 2.0f, texHeight / 2.0f});

    float newPosX = origIconPos.x;
    float newPosY = origIconPos.y;

    if (m_currentIconType == IconType::Robot || m_currentIconType == IconType::Spider) {
        newPosY = newPosY - 6.f;
    } else if (m_currentIconType == IconType::Ufo) {
        newPosY = newPosY - 4.f;
    } else if (m_currentIconType == IconType::Swing && Mod::get()->getSettingValue<bool>("extra-swing-space")) {
        newPosX = newPosX + 6.f;
    }

    CCPoint origCubePos;
    if (m_cubePreview) {
        origCubePos = m_cubePreview->getPosition();
        float newCubePosX = origCubePos.x;
        float newCubePosY = origCubePos.y;

        if (m_currentIconType == IconType::Ufo) newCubePosY = newCubePosY - 4.f;
        m_cubePreview->setPosition({newCubePosX, newCubePosY});
    }

    CCPoint origSwingTopPos, origSwingMidPos, origSwingBotPos;
    bool hasSwingFires = (m_currentIconType == IconType::Swing && m_swingTopFire && m_swingMidFire && m_swingBotFire);
    if (hasSwingFires) {
        origSwingTopPos = m_swingTopFire->getPosition();
        origSwingMidPos = m_swingMidFire->getPosition();
        origSwingBotPos = m_swingBotFire->getPosition();

        float swingOffsetX = 6.f;
        m_swingTopFire->setPosition({origSwingTopPos.x + swingOffsetX, origSwingTopPos.y});
        m_swingMidFire->setPosition({origSwingMidPos.x + swingOffsetX, origSwingMidPos.y});
        m_swingBotFire->setPosition({origSwingBotPos.x + swingOffsetX, origSwingBotPos.y});
    }

    m_previewPlayer->setPosition({newPosX, newPosY});

    renderTex->beginWithClear(bgCol.r / 255.f, bgCol.g / 255.f, bgCol.b / 255.f, bgCol.a / 255.f);
    m_iconContainerNode->visit();
    renderTex->end();

    m_iconContainerNode->setScale(origContainerScale);
    m_iconContainerNode->setPosition(origContainerPos);
    m_previewPlayer->setPosition(origIconPos);
    if (m_cubePreview) m_cubePreview->setPosition(origCubePos);

    if (hasSwingFires) {
        m_swingTopFire->setPosition(origSwingTopPos);
        m_swingMidFire->setPosition(origSwingMidPos);
        m_swingBotFire->setPosition(origSwingBotPos);
    }

    CCImage* image = renderTex->newCCImage();
    return image;
}

void IconOffsetEditorPopup::onRenderIcon(CCObject* sender) {
    auto icInfo = more_icons::activeIcon(m_currentIconType);
    if (!icInfo) {
        FLAlertLayer::create("Error", "Couldn't get icon info!", "aw :(")->show();
        return;
    }
    
    auto renderPath = Mod::get()->getSettingValue<std::filesystem::path>("renders-path");
    
    if (!std::filesystem::exists(renderPath)) std::filesystem::create_directories(renderPath);
    
    std::string renderFilename = fmt::format("{}-{}.png", icInfo->getShortName(), getCurrentTimeString());
    
    auto finalPath = renderPath / renderFilename;
    
    log::info("rendering icon to: {}", utils::string::pathToString(finalPath));
    
    CCImage* render = getIconImage();
    
    if (!render) {
        FLAlertLayer::create(
            "Error",
            "Failed to create render image!",
            ":("
        )->show();
        log::error("getIconImage() returned nullptr");
        return;
    }
    
    
    bool saved = render->saveToFile(utils::string::pathToString(finalPath).c_str(), false);
    delete render;
    
    if (saved) {
        FLAlertLayer::create(
            "Rendered!",
            fmt::format("Icon rendered successfully!\nSaved to: <cy>{}</c>", utils::string::pathToString(finalPath)),
            ":D"
        )->show();
        log::info("Successfully rendered icon {} to {}", icInfo->getShortName(), utils::string::pathToString(finalPath));
    } else {
        FLAlertLayer::create(
            "Error",
            fmt::format("Failed to save render to:\n{}", utils::string::pathToString(finalPath)),
            ":("
        )->show();
        log::error("Failed to save image to file");
    }
}

void IconOffsetEditorPopup::onOpenRendersFolder(CCObject* sender) {
    geode::utils::file::openFolder(Mod::get()->getSettingValue<std::filesystem::path>("renders-path"));
}

void IconOffsetEditorPopup::onModSettings(CCObject* sender) {
    geode::openSettingsPopup(Mod::get());
}

void IconOffsetEditorPopup::mapRobotSpiderSprites(CCNode* node) {
    if (!node) return;
    
    static int depth = 0;
    depth++;
    
    std::string indent(depth * 2, ' ');
    
    if (auto sprite = typeinfo_cast<CCSprite*>(node)) {
        auto frame = sprite->displayFrame();
        if (frame) {
            auto texture = frame->getTexture();
            if (!texture) {
                log::debug("{}Sprite has frame but null texture, skipping", indent);
                depth--;
                return;
            }
            
            auto rect = frame->getRect();
            
            bool matched = false;
            for (const auto& frameName : m_frameNames) {
                auto cachedFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(frameName.c_str());
                
                if (cachedFrame && cachedFrame->getTag() != FALLBACK_TAG) {
                    auto cachedTexture = cachedFrame->getTexture();
                    if (!cachedTexture) {
                        log::warn("{}Cached frame has null texture: {}", indent, frameName);
                        continue;
                    }
                    
                    auto cachedRect = cachedFrame->getRect();
                    
                    bool textureMatches = (texture == cachedTexture);
                    bool rectMatches = (rect.equals(cachedRect));
                    
                    if (textureMatches && rectMatches) {
                        m_robotSpiderSprites[frameName].push_back(sprite);
                        matched = true;
                        break;
                    }
                }
            }
        }
    }
    
    if (auto spritePart = typeinfo_cast<CCSpritePart*>(node)) {
        auto frame = spritePart->displayFrame();
        if (frame) {
            auto texture = frame->getTexture();
            if (!texture) {
                log::warn("{}SpritePart has frame but null texture! skipping to avoid crash, but please check ur icon...", indent);
                depth--;
                return;
            }
            
            auto rect = frame->getRect();
            
            bool matched = false;
            for (const auto& frameName : m_frameNames) {
                auto cachedFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(frameName.c_str());
                
                if (cachedFrame && cachedFrame->getTag() != FALLBACK_TAG) {
                    auto cachedTexture = cachedFrame->getTexture();
                    if (!cachedTexture) {
                        log::warn("{}Cached frame has null texture: {} - skippin to avoid crash, but please check ur icon...", indent, frameName);
                        continue;
                    }
                    
                    auto cachedRect = cachedFrame->getRect();
                    
                    bool textureMatches = (texture == cachedTexture);
                    bool rectMatches = (rect.equals(cachedRect));
                    
                    if (textureMatches && rectMatches) {
                        m_robotSpiderSprites[frameName].push_back(static_cast<CCSprite*>(spritePart));
                        matched = true;
                        break;
                    }
                }
            }
        }
    }
    
    auto children = node->getChildren();
    if (children && children->count() > 0) {
        CCArrayExt<CCNode*> extChildren(children);
        for (auto* childNode : extChildren) {
            mapRobotSpiderSprites(childNode);
        }
    }
    
    depth--;
}

void IconOffsetEditorPopup::applyOffsetToAllMatchingSprites(CCNode* node, const std::string& frameName, CCPoint offset) {
    if (m_currentIconType == IconType::Robot || m_currentIconType == IconType::Spider) {
        if (m_robotSpiderSprites.count(frameName)) {
            auto& sprites = m_robotSpiderSprites[frameName];
            //log::info("applying offset to {} sprites with frame: {}", sprites.size(), frameName);
            
            for (auto* sprite : sprites) {
                if (sprite) {
                    auto frame = sprite->displayFrame();
                    if (frame) {
                        frame->setOffsetInPixels(offset);
                        sprite->setDisplayFrame(frame);
                    }
                }
            }
        } else {
            log::warn("no sprites found using: {}", frameName);
        }
    }
}

void IconOffsetEditorPopup::onAddToOffsetX(CCObject* sender) {
    AddValuePopup::create([this](float value) {
        float currentX = 0.0f;
        std::string xStr = m_inputX->getString();
        if (!xStr.empty()) {
            auto xResult = geode::utils::numFromString<float>(xStr);
            if (xResult) {
                currentX = xResult.unwrap();
            }
        }
        
        float newX = currentX + value;
        
        m_inputX->setString(fmt::format("{:.6g}", newX));
        
        onUpdateOffsets(nullptr);
        
        log::info("Added {} to X offset (was {}, now {})", value, currentX, newX);
    }, true)->show();
}

void IconOffsetEditorPopup::onAddToOffsetY(CCObject* sender) {
    AddValuePopup::create([this](float value) {
        float currentY = 0.0f;
        std::string yStr = m_inputY->getString();
        if (!yStr.empty()) {
            auto yResult = geode::utils::numFromString<float>(yStr);
            if (yResult) {
                currentY = yResult.unwrap();
            }
        }
        
        float newY = currentY + value;
        
        m_inputY->setString(fmt::format("{:.6g}", newY));
        
        onUpdateOffsets(nullptr);
        
        log::info("Added {} to Y offset (was {}, now {})", value, currentY, newY);
    }, false)->show();
}

void IconOffsetEditorPopup::nudgeOffset(geode::TextInput* input, float delta) {
    if (!input) return;

    float current = 0.0f;
    std::string str = input->getString();
    if (!str.empty()) {
        auto result = geode::utils::numFromString<float>(str);
        if (result) current = result.unwrap();
    }

    float updated = current + delta;
    input->setString(fmt::format("{:.6g}", updated));
    onUpdateOffsets(nullptr);
}

void IconOffsetEditorPopup::onNudgeOffsetXDown(CCObject* sender) { nudgeOffset(m_inputX, -1.f); }
void IconOffsetEditorPopup::onNudgeOffsetXUp(CCObject* sender)   { nudgeOffset(m_inputX, 1.f); }
void IconOffsetEditorPopup::onNudgeOffsetYDown(CCObject* sender) { nudgeOffset(m_inputY, -1.f); }
void IconOffsetEditorPopup::onNudgeOffsetYUp(CCObject* sender)   { nudgeOffset(m_inputY, 1.f); }

void IconOffsetEditorPopup::onToggleTrail(CCObject* sender) {
    log::info("when te togglean la trail: me togglearon. ....  . .");
}

void IconOffsetEditorPopup::onHitboxOpacityChanged(CCObject* sender) {
    m_hitboxOpacity = m_hitboxOpacitySlider->getValue();
    
    if (m_hitboxOpacityLabel) m_hitboxOpacityLabel->setString(fmt::format("{}%", static_cast<int>(m_hitboxOpacity * 100)).c_str());
    if (m_showHitbox) drawHitbox();
}

void IconOffsetEditorPopup::onClose(CCObject* sender) {
    Popup::onClose(sender);

    if (auto garageLayer = static_cast<GJGarageLayer*>(CCDirector::sharedDirector()->getRunningScene()->getChildByID("GJGarageLayer"))) {
        auto gameManager = GameManager::sharedState();
        auto player1 = garageLayer->m_playerObject;
        auto iconType1 = gameManager->m_playerIconType;
        more_icons::updateSimplePlayer(player1, iconType1, false);

        if (auto separateDualIcons = Loader::get()->getLoadedMod("weebify.separate_dual_icons")) {
            auto player2 = static_cast<SimplePlayer*>(garageLayer->getChildByID("player2-icon"));
            auto iconType2 = static_cast<IconType>(separateDualIcons->getSavedValue("lastmode", 0));
            more_icons::updateSimplePlayer(player2, iconType2, true);
        }

        garageLayer->selectTab(garageLayer->m_iconType);
    }
}
