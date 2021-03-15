#ifndef IBKMK_POLYGON_H
#define IBKMK_POLYGON_H

#include "IBKMK_Vector3D.h"

namespace IBKMK {

class Polygon {
public:

	static double calcSurfaceArea(const std::vector<Vector3D> & polyline);


	static IBKMK::Vector3D normal(const std::vector<IBKMK::Vector3D> &polyline);

	static std::vector<IBKMK::Vector3D> eraseCoolinearPoints(const std::vector<IBKMK::Vector3D> &polyline);

	std::vector<IBKMK::Vector3D>	m_polyline;

};
// ---------------------------------------------------------------------------

} // namespace IBKMK


#endif // IBKMK_POLYGON_H
