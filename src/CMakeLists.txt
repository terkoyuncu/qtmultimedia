# Generated from src.pro.

add_subdirectory(resonance-audio)
add_subdirectory(multimedia)
if(ANDROID)
    add_subdirectory(android)
endif()

# special case begin
if(TARGET Qt::Quick)
    add_subdirectory(multimediaquick)
endif()
if(TARGET Qt::Quick3D AND QT_FEATURE_spatialaudio)
    add_subdirectory(multimediaquick3d)
endif()

if(TARGET Qt::Widgets)
    add_subdirectory(multimediawidgets)
endif()

add_subdirectory(plugins)
#special case end
