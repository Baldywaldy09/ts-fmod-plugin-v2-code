#pragma once

namespace prism::ui
{
	typedef uint64_t** (*load_window_t)(
		uint64_t* load_window_onto_this,
		string* window_name,
		string* window_type,
		int unk,
		string* window_script,
		int unk_2
	);
	inline load_window_t load_window;

	typedef uint64_t** (*update_value_t)(
		text_common_u* text_common,
		int unk
	);
	inline update_value_t update_value;
};