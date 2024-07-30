#include "register_types.h"

#include <gdextension_interface.h>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/godot.hpp>

#include "Controller.h"

static godot_scgms::GDGameWrapper* _gd_scgms_wrapper_singleton;

void gdextension_initialize(godot::ModuleInitializationLevel p_level)
{
	if (p_level == godot::MODULE_INITIALIZATION_LEVEL_SCENE)
	{
		godot::ClassDB::register_class<godot_scgms::GDGameWrapper>();

		_gd_scgms_wrapper_singleton = memnew(godot_scgms::GDGameWrapper);
		godot::Engine::get_singleton()->register_singleton("GDGameWrapper", _gd_scgms_wrapper_singleton);
	}
}

void gdextension_terminate(godot::ModuleInitializationLevel p_level)
{
	if (p_level == godot::MODULE_INITIALIZATION_LEVEL_SCENE)
	{
		godot::Engine::get_singleton()->unregister_singleton("GDGameWrapper");
		godot::memdelete(_gd_scgms_wrapper_singleton);
	}
}

extern "C"
{
	GDExtensionBool GDE_EXPORT gdextension_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, GDExtensionClassLibraryPtr p_library, GDExtensionInitialization* r_initialization)
	{
		godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

		init_obj.register_initializer(gdextension_initialize);
		init_obj.register_terminator(gdextension_terminate);
		init_obj.set_minimum_library_initialization_level(godot::MODULE_INITIALIZATION_LEVEL_SCENE);

		return init_obj.init();
	}
}
