#include "c32string.h"

#define str(raw_str) c32string raw_str (#raw_str)

namespace xkball {
	namespace strings {
		str(stringProperties);
		str(intProperties);
		str(doubleProperties);
		str(boolProperties);
		str(enumProperties);
		str(name);
		str(count);
		str(rules);
		str(available_values);
		str(include);
		str(seed);
		str(results);
		c32string true_("true");
		c32string false_("false");
	}
}