#pragma once

#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/ui/TextInput.hpp>
#include "IconPartCell.hpp"

using namespace geode::prelude;

enum class SelectedSpritePart {
    FirstLayer,
    SecondLayer,
    Outline,
    Detail,
    Dome
};

struct FrameOffsetData {
    std::string frameName;
    CCPoint offset;
};

class AddValuePopup;

class IconOffsetEditorPopup : public Popup {
protected:
    bool init() override;

    SimplePlayer* m_previewPlayer = nullptr;
    CCNode* m_iconContainerNode = nullptr;
    CCSprite* m_cubePreview = nullptr;
    PlayerFireBoostSprite* m_swingTopFire = nullptr;
    PlayerFireBoostSprite* m_swingMidFire = nullptr;
    PlayerFireBoostSprite* m_swingBotFire = nullptr;

    CCSprite* m_trailPreview = nullptr;
    NineSlice* m_waveTrailPreview = nullptr;
    NineSlice* m_waveTrailPreviewLighter = nullptr;

    geode::TextInput* m_inputX = nullptr;
    geode::TextInput* m_inputY = nullptr;
    CCLabelBMFont* m_labelX = nullptr;
    CCLabelBMFont* m_labelY = nullptr;
    CCMenuItemSpriteExtra* m_updateButton = nullptr;

    // -----------------------
    // PART SCROLL LAYER
    // -----------------------
    ScrollLayer* m_partScrollLayer = nullptr;
    std::map<std::string, IconPartCell*> m_frameCells;
    std::map<SelectedSpritePart, IconPartCell*> m_partCells;

    CCMenu* m_colorPickerMenu = nullptr;
    std::string m_currentColorSettingId;
    CCSprite* m_currentColorButtonSprite = nullptr;
    ccColor3B m_previewColor1;
    ccColor3B m_previewColor2;
    ccColor3B m_previewGlowColor;

    // -----------------------
    // PREVIEW TOGGLERS
    // -----------------------
    CCMenuItemToggler* m_glowToggler = nullptr;
    CCMenuItemToggler* m_hitboxToggler = nullptr;
    CCMenuItemToggler* m_trailToggler = nullptr; // unfinished

    CCLabelBMFont* m_rotationSpeedLabel = nullptr;
    Slider* m_cubeOpacitySlider = nullptr;
    CCLabelBMFont* m_cubeOpacityLabel = nullptr;
    CCMenu* m_animButtonsMenu = nullptr;
    Slider* m_rotationSpeedSlider = nullptr;
    CCLabelBMFont* m_iconNameLabel = nullptr;
    bool m_isRotating = false;

    CCDrawNode* m_hitboxDrawNode = nullptr;
    Slider* m_hitboxOpacitySlider = nullptr;
    CCLabelBMFont* m_hitboxOpacityLabel = nullptr;
    float m_hitboxOpacity = 1.0f;
    bool m_showHitbox = false;

    SelectedSpritePart m_selectedPart = SelectedSpritePart::FirstLayer;
    IconType m_currentIconType;

    std::map<std::string, std::vector<CCSprite*>> m_robotSpiderSprites;
    std::map<std::string, CCPoint> m_modifiedOffsets;

    std::vector<std::string> m_frameNames;
    std::string m_currentFrameName;

    fmt::memory_buffer m_logStream;

    void updatePreviewPlayer();
    void onPartSelected(CCObject* sender);
    void onUpdateOffsets(CCObject* sender);
    void onToggleGlow(CCObject* sender);
    void onToggleHitbox(CCObject* sender);
    void onToggleTrail(CCObject* sender);
    void onInfoButton(CCObject* sender);
    void onWhy(CCObject* sender); // :sob:
    void onPlayAnimation(CCObject* sender);
    void onPlayBallRotation(CCObject* sender);
    void onStopBallRotation(CCObject* sender);
    void onRotationSpeedChanged(CCObject* sender);
    void onExtraPreviewOpacityChanged(CCObject* sender);
    void onRenderIcon(CCObject* sender);
    void onModSettings(CCObject* sender);
    void updateInputFields();
    void drawHitbox();
    void onSavePlist(CCObject* sender);
    void onOpenRendersFolder(CCObject* sender);
    void highlightSelectedButton();
    void mapRobotSpiderSprites(CCNode* node);
    void applyOffsetToAllMatchingSprites(CCNode* node, const std::string& frameName, CCPoint offset);
    void addToLog(const std::string& logMsg, int newLines);
    void processPlistSave(bool remapNames);
    CCSprite* getCurrentSelectedSprite();
    CCImage* getIconImage();
    std::string getCurrentRealFrameName();

    void onAddToOffsetX(CCObject* sender);
    void onAddToOffsetY(CCObject* sender);
    void onNudgeOffsetXDown(CCObject* sender);
    void onNudgeOffsetXUp(CCObject* sender);
    void onNudgeOffsetYDown(CCObject* sender);
    void onNudgeOffsetYUp(CCObject* sender);
    void nudgeOffset(geode::TextInput* input, float delta);

    void updateColor(cocos2d::ccColor4B const& color);
    void onHitboxOpacityChanged(CCObject* sender);
    void onColorPicker(CCObject* sender);
    void applyPreviewColors();
    void onClose(CCObject* sender) override;

    void setupPartScrollLayer();

public:
    static IconOffsetEditorPopup* create();
};
