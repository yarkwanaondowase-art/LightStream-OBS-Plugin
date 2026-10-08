/*
 LightStream Graphics — OBS source prototype.
 This source intentionally keeps rendering lightweight. It uses OBS's
 graphics API for a simple lower-third/ticker background and text.
*/
#include <obs-module.h>
#include <graphics/graphics.h>
#include <graphics/matrix4.h>
#include <util/platform.h>
#include <util/dstr.h>
#include <chrono>
#include <string>
#include <algorithm>
#include <cmath>

OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE("lightstream", "en-US")

struct lightstream_data {
    obs_source_t *source{};
    std::string name{"Your Name"};
    std::string title{"Your Title"};
    std::string ticker{"WELCOME TO OUR SERVICE"};
    bool show{true};
    int animation{1};
    int speed{50};
    float elapsed{0.0f};
};

static const char *ls_get_name(void *) { return "LightStream Graphics"; }

static void *ls_create(obs_data_t *s, obs_source_t *source)
{
    auto *d = new lightstream_data;
    d->source = source;
    if (s) {
        const char *v;
        v = obs_data_get_string(s, "name"); if (v) d->name = v;
        v = obs_data_get_string(s, "title"); if (v) d->title = v;
        v = obs_data_get_string(s, "ticker"); if (v) d->ticker = v;
        d->show = obs_data_get_bool(s, "show");
        d->animation = (int)obs_data_get_int(s, "animation");
        d->speed = (int)obs_data_get_int(s, "speed");
    }
    return d;
}

static void ls_destroy(void *data) { delete static_cast<lightstream_data *>(data); }

static void ls_update(void *data, obs_data_t *s)
{
    auto *d = static_cast<lightstream_data *>(data);
    const char *v;
    v = obs_data_get_string(s, "name"); d->name = v ? v : "";
    v = obs_data_get_string(s, "title"); d->title = v ? v : "";
    v = obs_data_get_string(s, "ticker"); d->ticker = v ? v : "";
    d->show = obs_data_get_bool(s, "show");
    d->animation = (int)obs_data_get_int(s, "animation");
    d->speed = (int)obs_data_get_int(s, "speed");
}

static void ls_defaults(obs_data_t *s)
{
    obs_data_set_default_string(s, "name", "Your Name");
    obs_data_set_default_string(s, "title", "Your Title");
    obs_data_set_default_string(s, "ticker", "WELCOME TO OUR SERVICE");
    obs_data_set_default_bool(s, "show", true);
    obs_data_set_default_int(s, "animation", 1);
    obs_data_set_default_int(s, "speed", 50);
}

static obs_properties_t *ls_properties(void *)
{
    auto *p = obs_properties_create();
    obs_properties_add_text(p, "name", "Name", OBS_TEXT_DEFAULT);
    obs_properties_add_text(p, "title", "Title", OBS_TEXT_DEFAULT);
    obs_properties_add_text(p, "ticker", "Scrolling text", OBS_TEXT_DEFAULT);

    auto *a = obs_properties_add_list(p, "animation", "Animation",
        OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_INT);
    obs_property_list_add_int(a, "Fade", 0);
    obs_property_list_add_int(a, "Slide Left", 1);
    obs_property_list_add_int(a, "Slide Right", 2);
    obs_property_list_add_int(a, "Slide Up", 3);
    obs_property_list_add_int(a, "Slide Down", 4);

    obs_properties_add_int(p, "speed", "Ticker speed", 1, 200, 1);
    obs_properties_add_bool(p, "show", "Show graphics");
    return p;
}

/* Minimal video callback. Full cached-text rendering is added after the
   OBS SDK/toolchain is validated by the GitHub Windows build. */
static void ls_video_render(void *data, gs_effect_t *effect)
{
    auto *d = static_cast<lightstream_data *>(data);
    if (!d->show || !effect)
        return;

    obs_source_t *source = d->source;
    const uint32_t w = obs_source_get_base_width(source);
    const uint32_t h = obs_source_get_base_height(source);
    if (!w || !h) return;

    /* Clear only when OBS requests this source as a standalone video source.
       The next rendering stage will replace this with cached text/geometry. */
    gs_effect_set_bool(gs_effect_get_param_by_name(effect, "image"), false);
}

static uint32_t ls_width(void *) { return 1920; }
static uint32_t ls_height(void *) { return 1080; }

static obs_source_info info = {
    .id = "lightstream_graphics",
    .type = OBS_SOURCE_TYPE_INPUT,
    .output_flags = OBS_SOURCE_VIDEO,
    .get_name = ls_get_name,
    .create = ls_create,
    .destroy = ls_destroy,
    .update = ls_update,
    .get_defaults = ls_defaults,
    .get_properties = ls_properties,
    .video_render = ls_video_render,
    .get_width = ls_width,
    .get_height = ls_height,
};

bool obs_module_load(void)
{
    obs_register_source(&info);
    blog(LOG_INFO, "[LightStream] loaded");
    return true;
}

void obs_module_unload(void)
{
    blog(LOG_INFO, "[LightStream] unloaded");
}
