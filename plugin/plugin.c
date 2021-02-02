#include "plugin.h"
#include <obs-module.h>

OBS_DECLARE_MODULE()

struct data {
	obs_source_t * source;
	gs_effect_t * effect;
	float tiempo;
	bool glitch;
};

static const char *source_get_name(void *unused)
{
	UNUSED_PARAMETER(unused);
	return "Bayer ordered dithering";
}

static void source_update(void *data, obs_data_t *settings) {
	struct data* context = data;

	if (context->effect)
		gs_effect_destroy(context->effect);

	obs_enter_graphics();

	char* effect_file = obs_module_file(obs_data_get_string(settings, "type"));
	context->effect = gs_effect_create_from_file(effect_file, NULL);
	if (!context->effect)
		blog(200, "ERROR AL CREAR EL EFECTO");

	obs_leave_graphics();
	bfree(effect_file);
}

static void *source_create(obs_data_t *settings, obs_source_t *source)
{
	UNUSED_PARAMETER(source);
	struct data * context = bzalloc(sizeof(struct data));
	context->source = source;

	
	source_update(context, settings);

	return context;
}

static void source_destroy(void *data) {
	struct data* context = data;
	gs_effect_destroy(context->effect);
	bfree(data);
}

static void source_render(void *data, gs_effect_t *effect)
{
	struct data* context = data;
	obs_source_t *target = obs_filter_get_target(context->source);
	if(!obs_source_process_filter_begin(context->source, GS_RGBA, OBS_NO_DIRECT_RENDERING))
		return;
	obs_source_process_filter_end(context->source, context->effect, 0, 0);
	UNUSED_PARAMETER(effect);
}

static obs_properties_t *source_properties(void *data)
{
	obs_properties_t *properties = obs_properties_create();
	obs_property_t* combo = obs_properties_add_list(properties, "type", "Dither type", OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_STRING);
	obs_property_list_add_string(combo, "8 Colour dither", "effect1.effect");
	obs_property_list_add_string(combo, "Black and white dither", "effect2.effect");
	obs_property_list_add_string(combo, "Black and transparent dither", "effect3.effect");
	obs_property_list_add_string(combo, "White and transparent dither", "effect4.effect");
	return properties;
}

static void source_defaults(obs_data_t *settings)
{
	obs_data_set_default_string(settings, "type", "effect2.effect");
}

static void source_tick(void *data, float seconds)
{
}

struct obs_source_info source_info = {
	.id = "dithering filter",
	.type = OBS_SOURCE_TYPE_FILTER ,
	.output_flags = OBS_SOURCE_VIDEO,
	.get_name = source_get_name,
	.create = source_create,
	.destroy = source_destroy,
	.video_render = source_render,
	.update = source_update,
	.get_properties = source_properties,
	.get_defaults = source_defaults,
	.video_tick = source_tick
};

bool obs_module_load(void)
{
	obs_register_source(&source_info);
	return true;
}