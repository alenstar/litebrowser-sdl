#include "container_sdl.h"
#include "http_loader.h"
#include "logdef.h"
#define _USE_MATH_DEFINES
#include "nanovg.h"
#include <iostream>
#include <math.h>

using namespace litehtml;

container_sdl::container_sdl(litehtml::context *context
                          ,struct NVGcontext *vg  ) {
    // std::cout << "#ctor\n";
    m_html_context = context;
    m_html = NULL;
    m_vg = vg;
}

container_sdl::~container_sdl(void) {}

litehtml::uint_ptr container_sdl::create_font(const litehtml::tchar_t *faceName,
                                              int size, int weight,
                                              litehtml::font_style italic,
                                              unsigned int decoration,
                                              litehtml::font_metrics *fm) {
    // std::cout << "[create_font] faceName: " << faceName
    //     << ", size: " << size
    //     << ", weight: " << weight
    //     << ", italic: " << italic
    //     << ", decoration: " << decoration << std::endl;

    // TODO load specific fonts

    //int iWidth = 0, iHeight = 0;
    //TTF_SizeText(font, _t("x"), &iWidth, &iHeight);
    int font = nvgFindFont(m_vg, faceName);
    if (fm) {
        float ascender;
        float descender;
        float lineh;
        nvgTextMetrics(m_vg, &ascender, &descender, &lineh);
        // fixme cache this
        fm->ascent = ascender;
        fm->descent = descender;
        fm->height = lineh;
        fm->x_height = 0; // FIXME
        fm->draw_spaces = italic == fontStyleItalic || decoration;

        // std::cout << "[create_font] ascent: " << fm->ascent
        //     << ", descent: " << fm->descent
        //     << ", height: " << fm->height
        //     << ", x_height: " << fm->x_height
        //     << ", draw_spaces: " << fm->draw_spaces  << std::endl;
    }

    return (uint_ptr)font;
}

void container_sdl::delete_font(litehtml::uint_ptr hFont) {
    // todo fix segfault
    return;

    TTF_Font *font = (TTF_Font *)hFont;

    if (font) {
        TTF_CloseFont(font);
    }
}

int container_sdl::text_width(const litehtml::tchar_t *text,
                              litehtml::uint_ptr hFont) {
    TTF_Font *font = (TTF_Font *)hFont;

    if (!font) {
        // std::cout << "[text_width](" << text << ") font: null" << std::endl;
        LOGE("Bad font");
        return 0;
    }

    int iWidth = 0, iHeight = 0;
    TTF_SizeText(font, _t(text), &iWidth, &iHeight);
    return iWidth;
}

void container_sdl::draw_text(litehtml::uint_ptr hdc,
                              const litehtml::tchar_t *text,
                              litehtml::uint_ptr hFont,
                              litehtml::web_color color,
                              const litehtml::position &pos) {
    struct NVGcontext *vg = (struct NVGcontext*)hdc;
    // SDL_Color sdlcolor = {color.red, color.green, color.blue, color.alpha};
    // SDL_Surface *info;
    // TTF_Font *font = (TTF_Font *)hFont;
    // LOGD("draw_text: %s %d %d", text, pos.left(), pos.bottom());
    nvgSave(vg);
    
    // nvgBeginPath(vg);
    // nvgRoundedRect(vg, 0,0, 800, 600, 3.0f);
    // nvgFillColor(vg, nvgRGBA(255,255,255,255));
    // nvgFill(vg);

    nvgFontSize(vg, 24.0f);
    nvgFontFace(vg, "sans");
    nvgFillColor(vg, nvgRGBA(color.red, color.green, color.blue, color.alpha));
    nvgTextAlign(vg,NVG_ALIGN_LEFT|NVG_ALIGN_MIDDLE);
    nvgText(vg, pos.top(), pos.bottom(),text, NULL);

    nvgRestore(vg);
}

int container_sdl::pt_to_px(int pt) {
    return pt; // (int) round(pt * 125.0 / 72.0);
}

int container_sdl::get_default_font_size() const { return 16; }

void container_sdl::draw_list_marker(litehtml::uint_ptr hdc,
                                     const litehtml::list_marker &marker) {

     struct NVGcontext *vg = (struct NVGcontext*)hdc;
}

void container_sdl::load_image(const litehtml::tchar_t *src,
                               const litehtml::tchar_t *baseurl,
                               bool redraw_on_ready) {
    std::cout << "#loadImage " << src << "\n";
}

void container_sdl::get_image_size(const litehtml::tchar_t *src,
                                   const litehtml::tchar_t *baseurl,
                                   litehtml::size &sz) {
    std::cout << "#getImageSize " << src << "\n";

    std::string basePath = "../examples/scenes/";
    auto image = IMG_Load((basePath + src).c_str());
    sz.width = image->w;
    sz.height = image->h;

    std::cout << "width:" << image->w << " height" << image->h << "\n";

    // SDL_FreeSurface(image);
}

void container_sdl::draw_background(litehtml::uint_ptr hdc,
                                    const litehtml::background_paint &bg) {
     struct NVGcontext *vg = (struct NVGcontext*)hdc;
     LOGD("draw_background");
     if (bg.image.empty()) {
        // SDL_Rect fillRect = {bg.clip_box.x, bg.clip_box.y, bg.clip_box.width,
        //                      bg.clip_box.height};
        // SDL_SetRenderDrawColor(m_renderer, bg.color.red, bg.color.green,
        //                        bg.color.blue, bg.color.alpha);
        // SDL_RenderFillRect(m_renderer, &fillRect);
        LOGD("draw_background");
    nvgSave(vg);
    nvgBeginPath(vg);
    nvgRoundedRect(vg, bg.clip_box.x, bg.clip_box.y, bg.clip_box.width,bg.clip_box.height, 3.0f);
    nvgFillColor(vg, nvgRGBA(bg.color.red, bg.color.green, bg.color.blue, bg.color.alpha));
    //  nvgFillColor(vg, nvgRGBA(0,0,0,128));
    nvgFill(vg);
    nvgRestore(vg);
    } else {
        // Drawing image
        // SDL_Rect fillRect = {bg.clip_box.x, bg.clip_box.y, bg.image_size.width,
        //                      bg.image_size.height};
        LOGD("draw_background image %s", bg.image.c_str());
    }
}

// void container_sdl::make_url(const litehtml::tchar_t *url,
//                              const litehtml::tchar_t *basepath,
//                              litehtml::tstring &out) {}

void drawWindow(NVGcontext* vg, const char* title, float x, float y, float w, float h)
{
    float cornerRadius = 3.0f;
    NVGpaint shadowPaint;
    NVGpaint headerPaint;

    nvgSave(vg);
//  nvgClearState(vg);

    // Window
    nvgBeginPath(vg);
    nvgRoundedRect(vg, x,y, w,h, cornerRadius);
    // nvgFillColor(vg, nvgRGBA(28,30,34,192));
    nvgFillColor(vg, nvgRGBA(255,255,255,255));
    nvgFill(vg);

    // Drop shadow
    // shadowPaint = nvgBoxGradient(vg, x,y+2, w,h, cornerRadius*2, 10, nvgRGBA(0,0,0,128), nvgRGBA(0,0,0,0));
    // nvgBeginPath(vg);
    // nvgRect(vg, x-10,y-10, w+20,h+30);
    // nvgRoundedRect(vg, x,y, w,h, cornerRadius);
    // nvgPathWinding(vg, NVG_HOLE);
    // nvgFillPaint(vg, shadowPaint);
    // nvgFill(vg);

    // Header
    // headerPaint = nvgLinearGradient(vg, x,y,x,y+15, nvgRGBA(255,255,255,8), nvgRGBA(0,0,0,16));
    // nvgBeginPath(vg);
    // nvgRoundedRect(vg, x+1,y+1, w-2,30, cornerRadius-1);
    // nvgFillPaint(vg, headerPaint);
    // nvgFill(vg);
    // nvgBeginPath(vg);
    // nvgMoveTo(vg, x+0.5f, y+0.5f+30);
    // nvgLineTo(vg, x+0.5f+w-1, y+0.5f+30);
    // nvgStrokeColor(vg, nvgRGBA(0,0,0,32));
    // nvgStroke(vg);

    nvgFontSize(vg, 12.0f);
    nvgFontFace(vg, "sans");
    nvgTextAlign(vg,NVG_ALIGN_CENTER|NVG_ALIGN_MIDDLE);

    nvgFontBlur(vg,2);
    nvgFillColor(vg, nvgRGBA(0,0,0,255));
    nvgText(vg, x+w/2,y+16+1, title, NULL);

    // nvgFontBlur(vg,0);
    // nvgFillColor(vg, nvgRGBA(255,0,0,160));
    // nvgText(vg, x+w/2,y+16, title, NULL);

    nvgRestore(vg);
}


void container_sdl::draw_borders(litehtml::uint_ptr hdc,
                                 const litehtml::borders &borders,
                                 const litehtml::position &draw_pos,
                                 bool root) {
     struct NVGcontext *vg = (struct NVGcontext*)hdc;

    int bdr_top     = 0;
    int bdr_bottom  = 0;
    int bdr_left    = 0;
    int bdr_right   = 0;

    if(borders.top.width != 0 && borders.top.style > litehtml::border_style_hidden)
    {
        bdr_top = (int) borders.top.width;
    }
    if(borders.bottom.width != 0 && borders.bottom.style > litehtml::border_style_hidden)
    {
        bdr_bottom = (int) borders.bottom.width;
    }
    if(borders.left.width != 0 && borders.left.style > litehtml::border_style_hidden)
    {
        bdr_left = (int) borders.left.width;
    }
    if(borders.right.width != 0 && borders.right.style > litehtml::border_style_hidden)
    {
        bdr_right = (int) borders.right.width;
    }

    // TODO
    // draw right border
    // draw left border
    // draw top border
    // draw_bottom

    // nvgSave(vg);
    // nvgBeginPath(vg);
    // nvgRoundedRect(vg, 0,0, 800, 600, 3.0f);
    // nvgFillColor(vg, nvgRGBA(255,255,255,255));
    // nvgFill(vg);
    // nvgRestore(vg);
    // drawWindow(vg, "window", 0.0f, 0.0f, 800.0f, 600.0f);
    // LOGD("draw_borders: top %d bottom %d left %d right %d", bdr_top, bdr_bottom, bdr_left, bdr_right);
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
    LOGD("open_page");
    litehtml::tstring html = std::string(m_loader.load_file(url));
    LOGD("html: %s", html.c_str());
    m_html = litehtml::document::createFromString(html.c_str(), this,
                                                  m_html_context);
    if (m_html) {
        LOGD("render document");
        m_html->render(800);
        // set_size_request(800, 600);
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
