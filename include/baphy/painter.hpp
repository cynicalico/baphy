#ifndef BAPHY_PAINTER_HPP
#define BAPHY_PAINTER_HPP

#include <glm/glm.hpp>
#include <glm/gtc/type_precision.hpp>
#include <memory>
#include <optional>
#include <span>
#include <utility>
#include <vector>
#include "baphy/color.hpp"
#include "baphy/detail/polyline.hpp"
#include "baphy/glh/glh.hpp"

namespace baphy {
struct PrimitiveVertex {
    glm::vec3 pos;
    glm::u8vec4 color;
};

struct TextureVertex {
    glm::vec3 pos;
    glm::u8vec4 color;
    glm::vec2 tex_coord;
};

class Painter {
public:
    Painter();
    ~Painter();

    Painter(const Painter &other) = delete;
    Painter(Painter &&other) noexcept = delete;

    Painter &operator=(const Painter &other) = delete;
    Painter &operator=(Painter &&other) noexcept = delete;

    void point(glm::vec2 p0, const Color &color);

    void line(glm::vec2 p0, glm::vec2 p1, float width, const Color &color);
    void polyline(std::span<const glm::vec2> points, float width, bool closed, LineJoin join, const Color &color);

    void fill_tri(glm::vec2 p0, glm::vec2 p1, glm::vec2 p2, const Color &color);

    void fill_rect(glm::vec2 p0, glm::vec2 size, const Color &color);
    void fill_square(glm::vec2 p0, float size, const Color &color);

    void fill_ellipse(glm::vec2 center, glm::vec2 size, const Color &color);
    void fill_circle(glm::vec2 center, float size, const Color &color);

    void tex(glh::Texture &t, glm::vec2 p0, glm::vec2 size, const Color &color = baphy::rgb(255, 255, 255));

    void draw(const glm::mat4 &projection);

private:
    enum class GeometryType {
        opaque,
        translucent,
    };

    float z_{1.0f};
    std::optional<GeometryType> last_geometry_type_{std::nullopt};

    float next_z_(GeometryType type);

    GLuint primitive_shader_{0};
    GLuint primitive_vao_{0};
    std::vector<std::unique_ptr<glh::VecBuffer<PrimitiveVertex>>> primitive_vbos_{};
    std::size_t curr_primitive_vbo_idx_{0};

    std::vector<std::unique_ptr<glh::VecBuffer<PrimitiveVertex>>> translucent_primitive_vbos_{};
    std::size_t curr_translucent_primitive_vbo_idx_{0};

    GLuint tex_shader_{0};
    GLuint tex_vao_{0};
    std::vector<std::unique_ptr<glh::VecBuffer<TextureVertex>>> tex_vbos_{};
    std::size_t curr_tex_vbo_idx_{0};

    struct DrawCall {
        std::optional<GLuint> tex_id;
        GLuint vbo_id;
        GLint first;
        GLsizei count;
    };

    struct PendingDrawCall {
        std::optional<GLuint> tex_id;
        std::size_t first;
    };

    std::optional<PendingDrawCall> pending_draw_call_{std::nullopt};
    std::vector<DrawCall> translucent_draw_calls_{};

    void fill_tri_opaque_(glm::vec2 p0, glm::vec2 p1, glm::vec2 p2, glm::u8vec4 color);
    void fill_tri_translucent_(glm::vec2 p0, glm::vec2 p1, glm::vec2 p2, glm::u8vec4 color);

    [[nodiscard]] std::pair<GLuint, std::size_t> curr_translucent_vbo_(bool textured) const;

    void start_draw_call_(std::optional<GLuint> tex_id);
    void save_draw_call_();
};
} // namespace baphy

#endif // BAPHY_PAINTER_HPP
