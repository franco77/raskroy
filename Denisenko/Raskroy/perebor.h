#include "common.h"

namespace raskroy {

	class perebor {
		t_other_sizes::const_iterator i;
		t_other_sizes::const_iterator end;
		t_amounts &remains;

		scalar recursive(scalar size, t_amounts &rashod);

	public:
		scalar remain;
		scalar saw_thickness;
		scalar opilki;

		perebor(t_amounts &remains, scalar saw_thickness) : remains(remains), saw_thickness(saw_thickness) {}
		bool make(const t_size &size, scalar other_size, t_raskroy::t_details&, t_amounts &rashod);
	};
}