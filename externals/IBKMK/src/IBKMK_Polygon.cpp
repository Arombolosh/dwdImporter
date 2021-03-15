#include "IBKMK_Polygon.h"

#include "IBKMK_Vector3D.h"

namespace IBKMK{


double Polygon::calcSurfaceArea(const std::vector<Vector3D> & polyline){

	const char * const FUNC_ID = "[Polygon::calcSurfaceSize]";

	std::vector<IBKMK::Vector3D> newPolyline = eraseCoolinearPoints(polyline);


	IBKMK::Vector3D vec(0,0,0);
	for (size_t i=0; i<newPolyline.size(); ++i) {
		size_t j= i-1;
		if(i==0)
			j=newPolyline.size()-1;
		 vec += newPolyline[i].crossProduct(newPolyline[j]);
	}
	return normal(newPolyline).scalarProduct(newPolyline[0]-newPolyline[1]) * 0.5;
}

Vector3D Polygon::normal(const std::vector<Vector3D> & polyline)
{
	std::vector<IBKMK::Vector3D> newPolyline =eraseCoolinearPoints(polyline);
	IBKMK::Vector3D normalVec = (polyline[0]-polyline[1]).crossProduct(polyline[1]-polyline[2]);
	normalVec.normalize();
	return normalVec;
}

std::vector<Vector3D> Polygon::eraseCoolinearPoints(const std::vector<Vector3D> & polyline)
{
	const char * const FUNC_ID = "[Polygon::eraseCoolinearPoints]";
	std::vector<Vector3D> newPolyline;
	size_t polySize  = polyline.size();
	if(polySize<3)
		throw IBK::Exception(IBK::FormatString("Polyline has only '%1' points. Minimum is 3 points.").arg(polySize), FUNC_ID);

	size_t i=0;
	IBKMK::Vector3D vecA = polyline[1]-polyline[0];
	vecA.normalize();
	while (i<polySize) {
		size_t j, k;
		if(i==0){
			j=polySize-1;
			k=j-1;
		}
		else{
			j=i-1;
			if(j==0)
				k=polySize-1;
			else
				k=j-1;
		}
		IBKMK::Vector3D vecB = polyline[k]-polyline[j];

		vecB.normalize();

		if(vecA.scalarProduct(vecB) != 0 && vecA != vecB && vecA != -1 * vecB)
			newPolyline.push_back(polyline[j]);

		vecA = vecB;
		++i;
	}

	if(newPolyline.size()>2)
		return newPolyline;
	else {
		throw IBK::Exception(IBK::FormatString("Polygon has not enough colinear points."), FUNC_ID);
	}
}










}
