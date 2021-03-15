#ifndef QtExt_MaterialBaseH
#define QtExt_MaterialBaseH

#include <set>
#include <functional>

#include <QCoreApplication>
#include <QDate>
#include <QVariant>

#include "QtExt_MaterialCategory.h"
#include "QtExt_Constants.h"

namespace QtExt{


/*! Contains a material definition.*/
class MaterialBase {
	Q_DECLARE_TR_FUNCTIONS(MaterialBase)
public:

	enum parameter_t {
		MC_ID,
		MC_PARAMETRIZATION,
		MC_NAME,
		MC_CATEGORY,
		MC_PRODUCTID,
		MC_PRODUCER,
		MC_SOURCE,
		MC_RHO,
		MC_CET,
		MC_LAMBDA,
		MC_MEW,
		MC_W80,
		MC_WSAT,
		MC_AW,
		MC_SD,
		MC_KAir,
		NUM_MC
	};

	enum capabilities_t {
		TT_None			= 0x0000,
		TT_Thermal		= 0x0001,
		TT_Vapour		= 0x0002,
		TT_Liquid		= 0x0004,
		TT_Airflow		= 0x0008,
		TT_Salt			= 0x0010,
		TT_VOC			= 0x0020
	};

	/*! Standard constructor.
		Initialises the material object with non valid parameter (0).
	*/
	MaterialBase(int index) :
		m_index(index)
	{}

	/*! Set default destructor to virtual because this is a base class.*/
	virtual ~MaterialBase() = default;

	/*! The name of the material (already internationalized).*/
	virtual QString	name() const = 0;

	/*! The material category (used for grouping the material).*/
	virtual MaterialCategory			category() const = 0;

	/*! Default color used for showing the material.*/
	virtual QColor						color() const { return QColor(Qt::green); }

	/*! Information and notes for the product.*/
	virtual QString	notes() const { return QString(); }

	/*! Product-ID or brand name.*/
	virtual QString	productName() const { return QString(); }

	/*! Name of producer or provider.*/
	virtual QString	producer() const { return QString(); }

	/*! Name of data source.*/
	virtual QString	dataSource() const { return QString(); }

	/*! Bulk density in [kg/m3].*/
	virtual double						rho() const = 0;

	/*! Specific heat capacity in [Ws/kgK].*/
	virtual double						cp() const = 0;

	/*! Vapor diffusion resistance factor (optional, minimum value) [-].*/
	virtual double						mu() const = 0;

	/*! Thermal conductivity in [W/mK].*/
	virtual double						lambda() const = 0;

	/*! Hygroscopic moisture content at 80% RH in [kg/m3].*/
	virtual double						w_80() const = 0;

	/*! Saturation moisture content in [kg/m3].*/
	virtual double						w_sat() const = 0;

	/*! sd-Value in m in case of vapour retarders.
		Value is 0 for all other materials.
	*/
	virtual double						sdValue() const { return 0; }

	/*! air permeability in s (kg/msPa) in case of air permeable materials.
		Value is 0 for all other materials.
	*/
	virtual double						Kair() const { return 0; }

	/*! Hatching type of the material according DIN ISO 128-50 or DIN 1356-1.*/
	virtual HatchingType				hatchingType() const = 0;

	/*! Date for end of measuring material parameters.*/
	virtual QDate						measuringDate() const { return QDate(); }

	/*! Date of material sampling.*/
	virtual QDate						samplingDate() const { return QDate(); }

	/*! Production date of measured material.*/
	virtual QDate						productionDate() const { return QDate(); }

	/*! Returns the water absorption coefficient in [kg/m2s^0.5].*/
	virtual double Aw() const = 0;

	/*! Returns wether this material is wood.*/
	virtual bool isWood() const { return false; }

	/*! Returns wether the material is made of wood.*/
	virtual bool isWoodBased() const { return false; }

	/*! Returns wether this material is air.*/
	virtual bool isAir() const { return false; }

	/*! Returns wether this material is a foil.*/
	virtual bool isFoil() const { return false; }

	/*! Returns the material id.*/
	virtual int materialId() const = 0;

	/*! Return if a material is a user material able to edit.*/
	virtual bool isUserMaterial() const = 0;

	/*! Return a index of the current material in the original database.*/
	int materialIndex() const { return m_index; }

	/*! Return combination of transport capabilities.*/
	virtual int capabilities() const = 0;

	/*! Return if a material is marked as deprecated (value > 0) or deleted (value = -1).*/
	int deprecatedState() const { return 0; }

	/*! Returns a string of the parameter value. Formating depends on value type.*/
	QString materialValueFormatter(parameter_t type, double value) {
		if(m_valueFormatter)
			return m_valueFormatter(int(type),value);
		return defaultMaterialValueFormatter(type,value);
	}

	/*! Returns a string of the parameter value. Formating depends on value type.*/
	QString defaultMaterialValueFormatter(parameter_t type, double value) {
		switch (type) {
			case MaterialBase::MC_ID:
			case MaterialBase::MC_NAME:
			case MaterialBase::MC_CATEGORY:
			case MaterialBase::MC_PRODUCTID:
			case MaterialBase::MC_PRODUCER:
			case MaterialBase::MC_SOURCE: return QString();
			case MaterialBase::MC_RHO: return QString("%L1").arg( value, 0, 'f', 1);
			case MaterialBase::MC_CET: return QString("%L1").arg( value, 0, 'f', 1);
			case MaterialBase::MC_LAMBDA: return QString("%L1").arg( value, 0, 'f', 3);
			case MaterialBase::MC_MEW: return QString("%L1").arg( value, 0, 'f', 1);
			case MaterialBase::MC_W80: return QString("%L1").arg( value, 0, 'f', 1);
			case MaterialBase::MC_WSAT: return QString("%L1").arg( value, 0, 'f', 1);
			case MaterialBase::MC_AW: return QString("%L1").arg( value, 0, 'f', 3);
			case MaterialBase::MC_SD: return QString("%L1").arg( value, 0, 'f', 3);
			case MaterialBase::MC_KAir: return QString("%L1").arg( value, 0, 'f', 3);
			case MaterialBase::MC_PARAMETRIZATION :return QString();
			default: return QString();
		}
	}

	/*! Set a user defined value formatter function.
		This function must provid value to QString transformation for all numeric types.
		\see defaultMaterialValueFormatter for example.
	*/
	void setValueFormatterFunction(const std::function<QString(int, double)>& f) {
		m_valueFormatter = f;
	}

protected:
	/*! Index of the material in the database.*/
	int					m_index;

	std::function<QString(int, double)>	m_valueFormatter;
};


}  // namespace QtExt

/*! @file QtExt_MaterialBase.h
	@brief Contains the class MaterialBase.
*/

#endif // QtExt_MaterialBaseH
