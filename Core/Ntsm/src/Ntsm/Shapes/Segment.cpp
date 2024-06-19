//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/12/2024 at 6:22:06 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "NtsmPch/ntspch.h"
#include "Segment.h"

namespace nkentseu {

	namespace maths
	{
		RangeFloat Segment::Project(const Vector2& onto) {
			Vector2 ontoUnit = Vector2(onto).Normalized();
			return RangeFloat(ontoUnit.Dot(points[0]), ontoUnit.Dot(points[1]));
		}
		float32 Segment::Len() {
			return (points[1] - points[0]).Len();
		}
		bool Segment::Equivalent(const Segment& b) {
			if (*this == b) return true;
			return Len() == Segment(b).Len();
		}
	}
}    // namespace nkentseu