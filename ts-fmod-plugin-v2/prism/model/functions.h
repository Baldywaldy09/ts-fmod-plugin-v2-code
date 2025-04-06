#pragma once

#include "../common/strings.h"
#include "../token/token.h"

namespace prism
{
	typedef __int64 (*spawn_model_t)(
		__int64 destination,
		prism::string* model_path,
		token_t* look,
		token_t* variant,
		char unk,
		__int64 destination2
	);
	inline spawn_model_t spawn_model;
};