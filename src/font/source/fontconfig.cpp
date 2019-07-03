#if RTERM_CONFIG_SOURCE_FONTCONFIG

#include <rterm/font/source/fontconfig.h>
#include <rterm/util/scoped_guard.h>
#include <fontconfig/fontconfig.h>

namespace rterm::font::source {

namespace {

Weight parseWeight(int weight) {
    switch (weight) {
    case FC_WEIGHT_THIN:
        return Weight::thin;
    case FC_WEIGHT_EXTRALIGHT:
        return Weight::extraLight;
    case FC_WEIGHT_LIGHT:
        return Weight::light;
    case FC_WEIGHT_NORMAL:
        return Weight::normal;
    case FC_WEIGHT_MEDIUM:
        return Weight::medium;
    case FC_WEIGHT_SEMIBOLD:
        return Weight::semiBold;
    case FC_WEIGHT_BOLD:
        return Weight::bold;
    case FC_WEIGHT_EXTRABOLD:
        return Weight::extraBold;
    case FC_WEIGHT_HEAVY:
        return Weight::heavy;
    default:
        return Weight::normal;
    }
}

Width parseWidth(int width) {
    switch (width) {
    case FC_WIDTH_ULTRACONDENSED:
        return Width::ultraCondensed;
    case FC_WIDTH_EXTRAEXPANDED:
        return Width::extraCondensed;
    case FC_WIDTH_CONDENSED:
        return Width::condensed;
    case FC_WIDTH_SEMICONDENSED:
        return Width::semiCondensed;
    case FC_WIDTH_NORMAL:
        return Width::normal;
    case FC_WIDTH_SEMIEXPANDED:
        return Width::semiExpanded;
    case FC_WIDTH_EXPANDED:
        return Width::expanded;
    case FC_WIDTH_EXTRAEXPANDED:
        return Width::extraExpanded;
    case FC_WIDTH_ULTRAEXPANDED:
        return Width::ultraExpanded;
    default:
        return Width::normal;
    }
}

Font makeFont(FcPattern* font) {
    FcChar8* path;
    FcChar8* postscriptName;
    FcChar8* family;
    FcChar8* style;

    FcPatternGetString(font, FC_FILE, 0, &path);
    FcPatternGetString(font, FC_POSTSCRIPT_NAME, 0, &postscriptName);
    FcPatternGetString(font, FC_FAMILY, 0, &family);
    FcPatternGetString(font, FC_STYLE, 0, &style);

    int width;
    int weight;
    int slant;
    int spacing;

    FcPatternGetInteger(font, FC_WIDTH, 0, &width);
    FcPatternGetInteger(font, FC_WEIGHT, 0, &weight);
    FcPatternGetInteger(font, FC_SLANT, 0, &slant);
    FcPatternGetInteger(font, FC_SPACING, 0, &spacing);

    Handle handle{fs::path{reinterpret_cast<const char*>(path)}};
    Properties props{
        reinterpret_cast<const char*>(family),
        reinterpret_cast<const char*>(postscriptName),
        parseWeight(weight),
        parseWidth(width),
        reinterpret_cast<const char*>(style),
        spacing == FC_MONO,
    };
    return Font{handle, props};
}

} // namespace 

Collection FontConfig::load() {
    // Load fontconfig
    FcConfig* config = FcInitLoadConfigAndFonts();
    ScopedGuard guard{[] {FcFini();}};

    // Create pattern
    FcPattern* pattern = FcPatternCreate();
    ScopedGuard pguard{[pattern]{FcPatternDestroy(pattern);}};

    // Fill requested fields
    FcObjectSet* objset = FcObjectSetBuild(FC_FILE, FC_POSTSCRIPT_NAME, FC_FAMILY, FC_STYLE, FC_WEIGHT, FC_WIDTH, FC_SLANT, FC_SPACING, NULL);
    ScopedGuard oguard{[objset]{FcObjectSetDestroy(objset);}};

    // Search fonts
    FcFontSet* set = FcFontList(config, pattern, objset);
    ScopedGuard sguard{[set]{FcFontSetDestroy(set);}};

    Collection result;
    for (int i = 0; i < set->nfont; ++i) {
        result.push(makeFont(set->fonts[i]));
    }
    return result;
}

} // namespace rterm

#endif // RTERM_CONFIG_SOURCE_FONTCONFIG
