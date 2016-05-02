#ifndef IBK_physicsH
#define IBK_physicsH

#include <limits>

#include "IBK_math.h"

#ifdef _WIN32
#undef min
#undef max
#endif

namespace IBK {

extern const double PI;								///< The constant PI.
extern const double BOLTZMANN;						///< The BOLTZMANN constant (= 5.67e-8 W/m2K4).
extern const double FARADAY;						///< Faraday's constant (= 96485.3415 sA/mol).
extern const double R_IDEAL_GAS;					///< Universal gas constant (= 8.314472 J/molK).
extern const double DEG2RAD;						///< = 2*Pi/360
extern const double R_VAPOR;						///< Gas constant for water vapour in [J/kgK]
extern const double R_AIR;			 				///< Gas constant for dry air in [J/kgK]
extern const double GRAVITY;						///< Gravitational acceleration in [m/s2]

// The constants below are used by several physical programs: Cond, Delphin, WallModel, Therakles, Nandrad
extern const double RHO_W;							///< Density of water in [kg/m3]
extern const double RHO_AIR;						///< Density of air at 20 degC in [kg/m3] RHO_AIR = GASPRESS_REF/(R_AIR*T_REF_20)
extern const double RHO_ICE;						///< Density of water ice at 0 degC in [kg/m3]
extern const double T_DEFAULT;						///< Default temperature for isothermal calculations in [K].
extern const double T_REF_23;						///< Reference temperature for transformation of material functions
extern const double C_WATER;						///< Specific heat capacity of the liquid water in [J/kgK]
extern const double C_ICE;							///< Specific heat capacity of the ice in [J/kgK]
extern const double C_VAPOR;						///< Specific heat capacity of the water vapour in [J/kgK]
extern const double C_AIR;							///< Specific heat capacity of air in [J/kgK]
extern const double H_EVAP;							///< Specific heat of evaporation of the water vapour in [J/kg] with respect to reference temperature of 0K
extern const double H_FREEZE;						///< Specific heat of crystallization of the ice in [J/kg] with respect to reference temperature of 0K
extern const double KELVIN_FACTOR;					///< = 1.0/(1000*462*T_DEFAULT)
extern const double GASPRESS_REF;					///< Reference pressure in [Pa]
extern const double MIN_RH;							///< Minimum relative humidity [-]
extern const double MIN_PC_T;						///< = f_log(MIN_RH)*RHO_W*R_VAPOR, pc/T at MIN_RH
extern const double DV_AIR;							///< Water vapor diffusivity in air at reference temperature
extern const double SIGMA_W;						///< Surface tension of water (0 degC) in [N/m]

/*! Calculates the saturation pressure in [Pa] for a temperature T in [K]. */
inline double f_psat(double T) {
	if (T<124.6)	T = 124.6;
	if (T<273.15)   return 1.17782355e-24*IBK::f_pow(T-124.55, 12.3);
	else            return 2.632792891e-14*IBK::f_pow(T-163.35, 8.02);
}

/*! Calculates the saturation pressure in [Pa] for a temperature T in [K] using the DIN function. */
inline double f_psat_DIN1(double T) {
	if (T<273.15)   return 4.689*IBK::f_pow(1.486 + 0.01*(T-273.15), 12.3);
	else            return 288.68*IBK::f_pow(1.098 + 0.01*(T-273.15), 8.02);
}

/*! Calculates the dew point temperature in K for a temperature T in K and a relative humidity in [---] using the DIN function. */
inline double f_dew_DIN1(double T, double phi) {
	if (T<273.15)
		return IBK::f_pow(phi, 1.0/12.3) * (T - 124.55) + 124.55;

	// function is only valid result is > 273.15;
	return IBK::f_pow(phi, 1.0/8.02) * (T - 163.25) + 163.35;
}

/*! Calculates the saturation pressure in [Pa] for a temperature T in [K] using the DIN function (reformulated equation). */
inline double f_psat_DIN2(double T) {
	if (T<272.94529){ if (T<130) return(1.35000E-15); else return(  4.689 * IBK::f_pow(T/100-1.2455,12.300)); }
	else            { if (T>373) return(1.09347E+05); else return(288.680 * IBK::f_pow(T/100-1.6335, 8.020)); }
}

/*! Calculates the saturation pressure in [Pa] for a temperature T in [K] using the Clausius-Clapeyron equation. */
inline double f_psat_CLAUSIUS(double T) {
	double TC=T-273.15;
	if (T<273.15){ return(610.5 * std::exp(21.87*TC/(265.5+TC))); }
	else         { return(610.5 * std::exp(17.26*TC/(237.3+TC))); }
}

/*! Alternative formulation of the saturation pressure equation (Magnus equation). */
inline double f_psat_MAGNUS(double T) {
	double TC=T-273.15;
	return 611.213*std::exp(17.5043*TC/(241.2 + TC));
}

/*! Calculates the relative humidity (unitless) from a given capillary pressure in Pa and temperature T in [K]. */
inline double f_relhum(double T, double pc)   { return IBK::f_exp(pc/(IBK::RHO_W*IBK::R_VAPOR * T)); }

/*! Calculates the water vapour pressure at a temperature T in [K] and relative humidity (unitless). */
inline double f_pv(double T, double relhum)   { return f_psat(T)*relhum; }


/*! Calculates the capillary pressure from a given relative humidity in [---]
	and temperature in [K].
*/
inline double f_pc_rh(double rh, double T) {
	// TODO : notify user of manual correction of physical/math relation
	if (rh < IBK::MIN_RH)
		return IBK::MIN_PC_T * T;

	if (rh >= f_relhum(T, -1.0))
		return -1;

	return IBK::f_log(rh) * IBK::RHO_W*IBK::R_VAPOR * T;
}

/*! Calculates the logarithmic capillary pressure from pore radius.*/
inline double f_pC_r (double r){
	return std::log10(2*IBK::SIGMA_W/r);
}

/*! Calculates the pore radius from logarithmic capillary pressure.*/
inline double f_r_pC (double pC){
	return (2*IBK::SIGMA_W/std::pow(10,pC));
}

/*! Vapour diffusion constant in air in [m2/s]*/
inline double f_Dv_air (double T) {
	return 2.3E-05*std::pow((T/273.15), 1.81);
}

/*! Water vapour density function in [kg/m3] */
inline double f_rho_vap (double T, double phi) {
	return (phi*f_psat_DIN1(T))/(IBK::R_VAPOR*T);
}

/*! Logarithmus of capillary radius calculated from capillary pressure in [log(m)] */
inline double f_lr_pc (double pc) {
	if( pc == 0.0)
		return std::numeric_limits<double>::max();
	return std::log10(2*IBK::SIGMA_W/pc);
}

/*! Capillary radius calculated from capillary pressure in [m] */
inline double f_r_pc (double pc) {
	if( pc == 0.0)
		return std::numeric_limits<double>::max();
	return -(2*IBK::SIGMA_W/pc);
}

/*! Logarithmus of capillary radius calculated from logarithmic capillary pressure in [log(m)] */
inline double f_lr_pC (double pC){
	return std::log10(2*IBK::SIGMA_W/std::pow(10,pC));
}

/*! Capillary pressure from logarithmic capillary radius calculated in [Pa] */
inline double f_pc_lr (double lr) {
	return (2*IBK::SIGMA_W/std::pow(10, lr));
}

/*! Capillary pressure from capillary radius calculated in [Pa] */
inline double f_pc_r (double r) {
	return -(2*IBK::SIGMA_W/r);
}

/*! logarithmic capillary pressure from logarithmic capillary radius calculated in [log(Pa)] */
inline double f_pC_lr (double lr){
	return std::log10(2*IBK::SIGMA_W/std::pow(10, lr));
}

/*! Calculates the relative Moisture content from ablsolute moisture content.*/
inline double f_OlRel(double OlDry, double OlPor, double Ol) {
	if (Ol < OlDry)
		return 0;
	return ((Ol - OlDry)/(OlPor - OlDry));
}

} // namespace IBK

/*! \file IBK_physics.h
	\brief Contains various functions commonly used in building physics.
*/

#endif // IBK_physicsH