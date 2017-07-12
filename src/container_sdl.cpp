#include "container_sdl.h"
#include "http_loader.h"
#include "logdef.h"
#define _USE_MATH_DEFINES
#include "nanovg.h"
#include <iostream>
#include <math.h>

using namespace litehtml;

container_sdl::container_sdl(litehtml::context *context,
                             struct NVGcontext *vg) {
    // std::cout << "#ctor\n";
    m_html_context = context;
    m_html = NULL;
    m_vg = vg;
}

container_sdl::~container_sdl(void) {

    for (auto &it : m_fonts) {
        delete it.second;
    }
}

litehtml::uint_ptr container_sdl::create_font(const litehtml::tchar_t *faceName,
                                              int size, int weight,
                                              litehtml::font_style italic,
                                              unsigned int decoration,
                                              litehtml::font_metrics *fm) {
    FontConfig *fc = NULL;
    LOGD("create_font: %s", faceName);
    auto it = m_fonts.find(faceName);
    if (it == m_fonts.end()) {
        fc = new FontConfig;
        fc->size = size;
        fc->weight = weight;
        fc->style = italic;
        m_fonts.insert(std::make_pair(faceName, fc));
    } else {
        fc = it->second;
        fc->size = size;
        fc->weight = weight;
        fc->style = italic;
    }
    // int font = nvgFindFont(m_vg, faceName);
    if (fm) {
        float ascender;
        float descender;
        float lineh;
        nvgTextMetrics(m_vg, &ascender, &descender, &lineh);
        // fixme cache this
        fm->ascent = ascender;
        fm->descent = descender;
        fm->height = lineh;
        fm->x_height = lineh; // FIXME
        fm->draw_spaces = true;
    }

    return (litehtml::uint_ptr)fc;
}

void container_sdl::delete_font(litehtml::uint_ptr hFont) {
    // todo fix segfault
    return;
}

int container_sdl::text_width(const litehtml::tchar_t *text,
                              litehtml::uint_ptr hFont) {
    FontConfig *fc = (FontConfig *)hFont;
    float bounds[4] = {0.0f};
    int width = (int)(nvgTextBounds(m_vg, 0, 0, text, NULL, bounds) + 0.5f);
    LOGD("text_width: %d %f %f %f %f", width, bounds[0], bounds[1], bounds[2],
         bounds[3]);
    fc->width = width;
    return width;
}

void container_sdl::draw_text(litehtml::uint_ptr hdc,
                              const litehtml::tchar_t *text,
                              litehtml::uint_ptr hFont,
                              litehtml::web_color color,
                              const litehtml::position &pos) {
    struct NVGcontext *vg = (struct NVGcontext *)hdc;
    FontConfig *fc = (FontConfig *)hFont;

    nvgSave(vg);


    nvgFontSize(vg, fc->size);
    nvgFontFace(vg, "sans");
    nvgFillColor(vg, nvgRGBA(color.red, color.green, color.blue, color.alpha));
    nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
    nvgText(vg, pos.x, pos.y , text, NULL);

    nvgRestore(vg);
}

int container_sdl::pt_to_px(int pt) {
    return pt; // (int) round(pt * 125.0 / 72.0);
}

int container_sdl::get_default_font_size() const { return 16; }

void container_sdl::draw_list_marker(litehtml::uint_ptr hdc,
                                     const litehtml::list_marker &marker) {

    struct NVGcontext *vg = (struct NVGcontext *)hdc;
}

void container_sdl::load_image(const litehtml::tchar_t *src,
                               const litehtml::tchar_t *baseurl,
                               bool redraw_on_ready) {
    // std::cout << "#loadImage " << src << "\n";
    LOGD("load_image");
}

void container_sdl::get_image_size(const litehtml::tchar_t *src,
                                   const litehtml::tchar_t *baseurl,
                                   litehtml::size &sz) {
     LOGD("get_image_size");

    // std::string basePath = "../examples/scenes/";
    // auto image = IMG_Load((basePath + src).c_str());
    // sz.width = image->w;
    // sz.height = image->h;

    // std::cout << "width:" << image->w << " height" << image->h << "\n";

    // SDL_FreeSurface(image);
}

void container_sdl::draw_background(litehtml::uint_ptr hdc,
                                    const litehtml::background_paint &bg) {
    struct NVGcontext *vg = (struct NVGcontext *)hdc;
    if (bg.image.empty()) {
        nvgSave(vg);
        nvgBeginPath(vg);
        nvgRoundedRect(vg, bg.clip_box.x, bg.clip_box.y, bg.clip_box.width,
                       bg.clip_box.height, 3.0f);
        nvgFillColor(vg, nvgRGBA(bg.color.red, bg.color.green, bg.color.blue,
                                 bg.color.alpha));
        //  nvgFillColor(vg, nvgRGBA(0,0,0,128));
        nvgFill(vg);
        nvgRestore(vg);
    } else {
        // Drawing image
        // SDL_Rect fillRect = {bg.clip_box.x, bg.clip_box.y,
        // bg.image_size.width,
        //                      bg.image_size.height};
        LOGD("draw_background image %s", bg.image.c_str());
    }
}

// void container_sdl::make_url(const litehtml::tchar_t *url,
//                              const litehtml::tchar_t *basepath,
//                              litehtml::tstring &out) {}

void container_sdl::draw_borders(litehtml::uint_ptr hdc,
                                 const litehtml::borders &borders,
                                 const litehtml::position &draw_pos,
                                 bool root) {
    struct NVGcontext *vg = (struct NVGcontext *)hdc;

    int bdr_top = 0;
    int bdr_bottom = 0;
    int bdr_left = 0;
    int bdr_right = 0;

    if (borders.top.width != 0 &&
        borders.top.style > litehtml::border_style_hidden) {
        bdr_top = (int)borders.top.width;
    }
    if (borders.bottom.width != 0 &&
        borders.bottom.style > litehtml::border_style_hidden) {
        bdr_bottom = (int)borders.bottom.width;
    }
    if (borders.left.width != 0 &&
        borders.left.style > litehtml::border_style_hidden) {
        bdr_left = (int)borders.left.width;
    }
    if (borders.right.width != 0 &&
        borders.right.style > litehtml::border_style_hidden) {
        bdr_right = (int)borders.right.width;
    }

    // TODO
    // draw right border
    // draw left border
    // draw top border
    // draw_bottom

    // LOGD("draw_borders: top %d bottom %d left %d right %d", bdr_top,
    // bdr_bottom, bdr_left, bdr_right);
}

void container_sdl::transform_text(litehtml::tstring &text,
                                   litehtml::text_transform tt) {}

void container_sdl::set_clip(const litehtml::position &pos,
                             const litehtml::border_radiuses &bdr_radius,
                             bool valid_x, bool valid_y) {}

void container_sdl::del_clip() {}

void container_sdl::clear_images() {}

void container_sdl::get_client_rect(litehtml::position &client) const {}

void container_sdl::on_anchor_click(const litehtml::tchar_t *url,
                                    const litehtml::element::ptr &el) {}

void container_sdl::set_cursor(const litehtml::tchar_t *cursor) {}

void container_sdl::import_css(litehtml::tstring &text,
                               const litehtml::tstring &url,
                               litehtml::tstring &baseurl) {
    load_text_file(url, text);
}

void container_sdl::set_caption(const litehtml::tchar_t *caption) {}

void container_sdl::set_base_url(const litehtml::tchar_t *base_url) {}

const litehtml::tchar_t *container_sdl::get_default_font_name() const {
    return _t("sans-serif");
}

std::shared_ptr<litehtml::element>
container_sdl::create_element(const litehtml::tchar_t *tag_name,
                              const litehtml::string_map &attributes,
                              const std::shared_ptr<litehtml::document> &doc) {
    return 0;
}

void container_sdl::get_media_features(litehtml::media_features &media) const {
    litehtml::position client;
    get_client_rect(client);
    media.type = litehtml::media_type_screen;
    media.width = client.width;
    media.height = client.height;
    media.device_width = 512;
    media.device_height = 512;
    media.color = 8;
    media.monochrome = 0;
    media.color_index = 256;
    media.resolution = 96;
}

void container_sdl::get_language(litehtml::tstring &language,
                                 litehtml::tstring &culture) const {
    language = _t("en");
    culture = _t("");
}

void container_sdl::link(const std::shared_ptr<litehtml::document> &ptr,
                         const litehtml::element::ptr &el) {}

void container_sdl::load_text_file(const litehtml::tstring &url,
                                   litehtml::tstring &out) {
    out = m_loader.load_file(url);
}

void container_sdl::open_page(const litehtml::tstring &url) {
    litehtml::tstring html = std::string(m_loader.load_file(url));
    m_html = litehtml::document::createFromString(html.c_str(), this,
                                                  m_html_context);
    if (m_html) {
        m_html->render(800);
    }
}

void container_sdl::on_draw(struct NVGcontext *vg) {
    if (m_html) {
        litehtml::position pos;
        pos.width = 800;
        pos.height = 600;
        pos.x = 0;
        pos.y = 0;
        m_html->draw((litehtml::uint_ptr)vg, 0, 0, &pos);
    }
}
