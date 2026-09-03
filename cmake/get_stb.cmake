CPMAddPackage(URI "gh:nothings/stb#2c980bb59875b0d32144a71867fbdebb2f77cd20" DOWNLOAD_ONLY YES)

if (stb_ADDED) 
    add_library(stb STATIC)
    add_library(stb::stb ALIAS stb)

    target_sources(stb
            PRIVATE
            "${CMAKE_CURRENT_LIST_DIR}/../src/stb/stb_image_write.c"
            "${CMAKE_CURRENT_LIST_DIR}/../src/stb/stb_image.c"
            "${CMAKE_CURRENT_LIST_DIR}/../src/stb/stb_rect_pack.c"

            PUBLIC
            FILE_SET headers
            TYPE HEADERS
            BASE_DIRS
            "${stb_SOURCE_DIR}"
            FILES
            "${stb_SOURCE_DIR}/stb_image_write.h"
            "${stb_SOURCE_DIR}/stb_image.h"
            "${stb_SOURCE_DIR}/stb_rect_pack.h"
    )

    target_compile_features(stb PRIVATE cxx_std_23)

    if (MSVC)
        target_compile_options(stb PRIVATE /utf-8 /W0)
    else ()
        target_compile_options(stb PRIVATE -w)
    endif ()
endif ()
