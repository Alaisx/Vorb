#include "Vorb/stdafx.h"
#include "Vorb/ui/widgets/yaml/Slider.h"

#include "Vorb/graphics/TextureCache.h"
#include "Vorb/ui/widgets/Slider.h"
#include "Vorb/ui/widgets/yaml/Parser.h"
#include "Vorb/ui/widgets/yaml/Widget.h"

bool vui::parseSliderEntry(keg::ReadContext& context, vui::Slider* slider, const nString& name, keg::Node value, Delegate<vui::IWidget*, const nString&, keg::Node>* widgetParser, vg::TextureCache* textureCache) {
    if (name == "bar_color") {
        color4 color;
        if (!parseColor(*value, color)) return false;

        slider->setBarColor(color);
    } else if (name == "slider_color") {
        color4 color;
        if (!parseColor(*value, color)) return false;

        slider->setSlideColor(color);
    } else if (name == "slider_hover_color") {
        color4 color;
        if (!parseColor(*value, color)) return false;

        slider->setSlideHoverColor(color);
    } else if (name == "slider_texture") {
        if (keg::getType(value) != keg::NodeType::VALUE) return false;

        nString texturePath = value->data.as<nString>();

        vg::Texture tex = textureCache->addTexture(texturePath);

        slider->setSlideTexture(tex.id);
    } else if (name == "bar_texture") {
        if (keg::getType(value) != keg::NodeType::VALUE) return false;

        nString texturePath = value->data.as<nString>();

        vg::Texture tex = textureCache->addTexture(texturePath);

        slider->setBarTexture(tex.id);
    } else if (name == "value") {
        i32 slideValue;
        if (!parseValue(*value, slideValue)) return false;

        slider->setValue(slideValue);
    } else if (name == "range") {
        i32v2 range;
        if (!parseVec2(*value, range)) return false;

        slider->setRange(range[0], range[1]);
    } else if (name == "min") {
        i32 minVal;
        if (!parseValue(*value, minVal)) return false;

        slider->setMin(minVal);
    } else if (name == "max") {
        i32 maxVal;
        if (!parseValue(*value, maxVal)) return false;

        slider->setMin(maxVal);
    } else if (name == "slide_size") {
        Length2OrRaw lengthOrRaw{};

        switch (parseLength2OrRaw(*value, lengthOrRaw)) {
        case 1:
            slider->setSlideSize(lengthOrRaw.raw);
            break;
        case 2:
            slider->setSlideSize(lengthOrRaw.length);
            break;
        default:
            return false;
        }
    } else if (name == "vertical") {
        bool isVertical;
        if (!parseValue(*value, isVertical)) return false;

        slider->setIsVertical(isVertical);
    } else if (name == "natural_scroll") {
        bool naturalScroll;
        if (!parseValue(*value, naturalScroll)) return false;

        slider->setNaturalScroll(naturalScroll);
    } else if (name == "elastic_scroll") {
        bool elasticScroll;
        if (!parseValue(*value, elasticScroll)) return false;

        slider->setElasticScroll(elasticScroll);
    } else if (name == "slide_weight") {
        f32 slideWeight;
        if (!parseValue(*value, slideWeight)) return false;

        slider->setSlideWeight(slideWeight);
    } else if (name == "slide_kinetic_friction") {
        f32 slideKineticFriction;
        if (!parseValue(*value, slideKineticFriction)) return false;

        slider->setSlideKineticFriction(slideKineticFriction);
    } else if (name == "slide_static_friction") {
        f32 slideStaticFriction;
        if (!parseValue(*value, slideStaticFriction)) return false;

        slider->setSlideStaticFriction(slideStaticFriction);
    } else if (name == "slide_max_speed") {
        f32 slideMaxSpeed;
        if (!parseValue(*value, slideMaxSpeed)) return false;

        slider->setSlideMaxSpeed(slideMaxSpeed);
    } else if (name == "scroll_sensitivity") {
        f32 scrollSensitivity;
        if (!parseValue(*value, scrollSensitivity)) return false;

        slider->setScrollSensitivity(scrollSensitivity);
    } else if (name == "scroll_on_parent") {
        bool scrollOnParent;
        if (!parseValue(*value, scrollOnParent)) return false;

        slider->setScrollOnParent(scrollOnParent);
    } else {
        return vui::parseWidgetEntry(context, slider, name, value, widgetParser);
    }

    return true;
}
