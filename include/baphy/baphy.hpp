#ifndef BAPHY_BAPHY_HPP
#define BAPHY_BAPHY_HPP

#define BAPHY_VERSION_MAJOR 0
#define BAPHY_VERSION_MINOR 1
#define BAPHY_VERSION_PATCH 0

/* #[[[cog
from pathlib import Path
for item in sorted(Path("include").rglob("*"), key=lambda e: e.as_posix()):
    if item.is_file():
        p = item.as_posix()
        if "baphy.hpp" in p:
            continue
        p = p[p.find("include/") + len("include/"):]
        cog.outl(f"#include \"{p}\"")
]]] */
#include "baphy/application.hpp"
#include "baphy/color.hpp"
#include "baphy/detail/polyline.hpp"
#include "baphy/detail/shaders.hpp"
#include "baphy/detail/vbo_list.hpp"
#include "baphy/event/all.hpp"
#include "baphy/event/common.hpp"
#include "baphy/event/keyboard.hpp"
#include "baphy/event/mouse.hpp"
#include "baphy/event/quit.hpp"
#include "baphy/font/cp_437.hpp"
#include "baphy/gl.hpp"
#include "baphy/glh/buffer.hpp"
#include "baphy/glh/gl_texture.hpp"
#include "baphy/glh/glh.hpp"
#include "baphy/glh/shader.hpp"
#include "baphy/glh/vertex_array.hpp"
#include "baphy/input_mgr.hpp"
#include "baphy/log.hpp"
#include "baphy/painter.hpp"
#include "baphy/runner.hpp"
#include "baphy/texture.hpp"
#include "baphy/timer_mgr.hpp"
#include "baphy/util/averagers.hpp"
#include "baphy/util/enum_class_bitops.hpp"
#include "baphy/util/envvars.hpp"
#include "baphy/util/io.hpp"
#include "baphy/util/math.hpp"
#include "baphy/util/platform.hpp"
#include "baphy/util/time.hpp"
#include "baphy/window.hpp"
/* [[[end]]] (sum: pwWNRewtVW) */

namespace baphy {
const char *version();

template<typename T>
    requires std::derived_from<T, Application>
int run(const WindowOpts &window_opts) {
    try {
        Runner().run<T>(window_opts);
    } catch (std::runtime_error &e) {
        BAPHY_LOG_ERROR("Runtime error: {}", e.what());
        return 1;
    }

    return 0;
}
} // namespace baphy

#include <fmt/format.h>
#include "clero/clero.hpp"

#endif // BAPHY_BAPHY_HPP
