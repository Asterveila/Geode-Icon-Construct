# v1.5.1
- Internal code cleanup when creating the button to access the mod's popup via the Icon Garage.
- Fixed an issue where the normal trail would still be shown (and be out of place) when a wave icon is selected.
- Tweak the Popup's ScrollLayer entries to use more readable names for Robot/Spider icons instead of the full, raw frame names.
- Added a setting to skip the simplified part names for Robot/Spider icons in case the just mentioned system fails.

# v1.5.0
- **COMPLETELY REDESIGNED** the Popup UI!
    - Popup is now bigger.
    - Turned the Icon Parts List into a ScrollLayer.
    - Element Preview Toggles (Hitbox/Glow) are now proper Togglers.
    - Colour picker buttons are no longer outside of the main popup area.
    - Update/Apply buttons are no longer outside of the main popup area.
    - Icon itself is now 3x scale instead of 2x.
    - Added a light underlay for the Icon area.
- Added buttons for offset control to make it easier to adjust them on Mobile devices.
- Added Trail Preview! (Basic Trail and Wave Trail).
    - Has a setting to enable/disable tinting the Trail/Wave Trail preview.
    - Has a setting to enable/disable Trail Preview Blending.
    - Has a setting to enable viewing the outer edges of Normal Trails (to show the area covered by bigger trails, such as the Fire one).

# v1.4.2
- HOPEFULLY *ACTUALLY* fixed plist editing being broken sometimes.

# v1.4.1
- Fixed plist editing being broken with More Icons' latest update.

# v1.4.0
- Updated to Geode v5 (Geometry Dash 2.2081).

# v1.3.0
- Updated More Icons support to the new API version (Added by [hiimjasmine00](user:7466002))
- Added Automatic Reloading after applying plist changes (Added by [hiimjasmine00](user:7466002))
- Internal code optimizations and improvements (Added by [hiimjasmine00](user:7466002))

# v1.2.0
- Added a setting to disable the Hitbox Preview's fill.
- Added a Slider to control the Hitbox Preview Border's Opacity.
- Added a setting to disable the Swing Fires being animated.
- Fixed offsets being rounded and not respecting decimal places in various scenarios.
- Fixed Frame Name Remapping not working properly for Robot/Spider icons.
- Slightly "fixed" Popup positioning so it's not too far to the left.
- Increased background darkening in the popup so elements outside it are a bit more visible.

# v1.1.2
- More internal code fixes and improvements.

# v1.1.1
- Internal code fixes and improvements. (Major noticeable change for end users is Icon Renders now use the current date to generate the filename instead of random numbers, to make it even less possible for filenames to be duplicate.)
- Translated major Info Popups to Spanish (Los textos de informacion mas importantes estan en espanol ahora - al final de los textos en ingles esta la info en espanol).

# v1.1.0
- Added Node IDs to UI elements.
- Increased horizontal size for parts side menu in Robot/Spider modes to accomodate for some potentially broken icons taking up too much space.
- Added **Swing Fires** preview for Swing icons.
- Added a setting to add extra space for Swing renders to make sure the fires fit in the final png.

# v1.0.0
- Initial release.