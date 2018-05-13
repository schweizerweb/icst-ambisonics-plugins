/*
  ==============================================================================

    Point3D.h
    Created: 28 Jul 2016 8:17:47am
    Author:  Christian Schweizer

  ==============================================================================
*/

#ifndef POINT3D_H_INCLUDED
#define POINT3D_H_INCLUDED

#include "JuceHeader.h"
#include "AudioParameterSet.h"

#define	SQRT105		10.246950766	/*sqrt(105.0)*/
#define SQRT15		3.87298334621	/*sqrt(15.0)*/
#define SQRT7		2.64575131106	/*sqrt(7.0)*/
#define SQRT5		2.2360679775	/*sqrt(5.0)*/
#define SQRT35_8	2.09165006634	/*sqrt(35.0/8.0)*/
#define SQRT21_8	1.6201851746	/*sqrt(21.0/8.0)*/
#define SQRT35		5.9160797831	/*sqrt(35.0)*/
#define SQRT35_2	4.18330013267	/*sqrt(35.0/2.0)*/
#define SQRT5_2		1.58113883008	/*sqrt(5.0/2.0)*/
#define SQRT77_2	6.204836823		/*sqrt(77.0/2.0)*/
#define SQRT11		3.31662479036	/*sqrt(11.0)*/
#define SQRT385		19.6214168703	/*sqrt(385.0)*/
#define SQRT1155	33.9852909359	/*sqrt(1155.0)*/
#define SQRT165		12.8452325787	/*sqrt(165.0)*/
#define SQRT385_2	13.8744369255	/*sqrt(385.0/2.0)*/
#define SQRT13		3.60555127546	/*sqrt(13.0)*/
#define SQRT3003_2	38.74919354		/*sqrt(3003.0/2.0)*/
#define SQRT1001_2	22.3718573212	/*sqrt(1001.0/2.0)*/
#define SQRT91		9.53939201417	/*sqrt(91.0)*/
#define SQRT1365_2	26.1247009552	/*sqrt(1365.0/2.0)*/
#define SQRT273		16.5227116419	/*sqrt(273)*/
#define SQRT715		26.7394839142	/*sqrt(715.0)*/
#define SQRT5005_2	50.0249937531	/*sqrt(5005.0/2.0)*/

#define PI 3.1415926535897932384626433832795
#define DISTANCE_MIN_VALUE 0.0001

template <typename ValueType>
class Point3D
{
public:
	/** Creates a Point3D at the origin */
	Point3D() noexcept : x(), y(), z() { xyzChanged = true; aedChanged = false; }

	/** Creates a copy of another Point3D. */
	Point3D(const Point3D& other) noexcept : x(other.x), y(other.y), z(other.z), audioParams(other.audioParams) { xyzChanged = true; aedChanged = false; }

	/** Creates a Point3D from an (x, y, z) position. */
	Point3D(ValueType initialX, ValueType initialY, ValueType initialZ) noexcept : x(initialX), y(initialY), z(initialZ) { xyzChanged = true; aedChanged = false; }
	
	/** Creates a Point3D from an (x, y, z) position. Keeps a reference to the corresponding audio parameter.*/
	Point3D(ValueType initialX, ValueType initialY, ValueType initialZ, AudioParameterSet paramSet) noexcept : x(initialX), y(initialY), z(initialZ), audioParams(paramSet) { xyzChanged = true; aedChanged = false; }

	//==============================================================================
	/** Copies this Point3D from another one. */
	Point3D& operator= (Point3D& other) noexcept { xyzChanged = true; x = other.getX(); y = other.getY(); z = other.getZ(); audioParams = other.audioParams; return *this; }

	inline bool operator== (Point3D other) const noexcept{ return getX() == other.getX() && getY() == other.getY() && getZ() == other.getZ(); }
	inline bool operator!= (Point3D other) const noexcept{ return getX() != other.getX() || getY() != other.getY() || getZ() != other.getZ(); }

		/** Returns true if the Point3D is (0, 0, 0). */
	bool isOrigin() const noexcept{ return getX() == ValueType() && getY() == ValueType() && getZ() == ValueType(); }

		/** Returns true if the coordinates are finite values. */
	inline bool isFinite() const noexcept{ return juce_isfinite(getX()) && juce_isfinite(getY()) && juce_isfinite(getZ()); }

		/** Returns the Point3D's x coordinate. */
	inline ValueType getX() noexcept{ if (aedChanged) calculateXyz(); return x; }

		/** Returns the Point3D's y coordinate. */
	inline ValueType getY() noexcept{ if (aedChanged) calculateXyz(); return y; }

		/** Returns the Point3D's z coordinate. */
	inline ValueType getZ() noexcept{ if (aedChanged) calculateXyz(); return z; }

		/** Returns the Point3D's azimuth. */
	inline ValueType getAzimuth() noexcept{ if (xyzChanged) calculateAed(); return azimuth; }

		/** Returns the Point3D's elevation. */
	inline ValueType getElevation() noexcept{ if (xyzChanged) calculateAed(); return elevation; }
		
		/** Returns the Point3D's distance. */
	inline ValueType getDistance() noexcept{ if (xyzChanged) calculateAed(); return distance; }
		
		/** Sets the Point3D's x coordinate. */
	inline void setX(ValueType newX, bool notify = true) noexcept { xyzChanged = true; x = newX; if(notify) audioParams.notifyX(x); }

		/** Sets the Point3D's y coordinate. */
	inline void setY(ValueType newY, bool notify = true) noexcept { xyzChanged = true; y = newY; if (notify) audioParams.notifyY(y); }

		/** Sets the Point3D's z coordinate. */
	inline void setZ(ValueType newZ, bool notify = true) noexcept { xyzChanged = true; z = newZ; if (notify) audioParams.notifyZ(z); }

		/** Sets the Point3D's azimuth. */
	inline void setAzimuth(ValueType newAzimuth, bool notify = true) noexcept { aedChanged = true; azimuth = newAzimuth; if (notify) audioParams.notifyA(azimuth); }

		/** Sets the Point3D's elevation. */
	inline void setElevation(ValueType newElevation, bool notify = true) noexcept { aedChanged = true; elevation = newElevation; if (notify) audioParams.notifyE(elevation); }

		/** Sets the Point3D's distance. */
	inline void setDistance(ValueType newDistance, bool notify = true) noexcept { aedChanged = true; distance = (newDistance < DISTANCE_MIN_VALUE ? DISTANCE_MIN_VALUE : newDistance); if (notify) audioParams.notifyD(distance); }

		/** Returns a Point3D which has the same Y, Z position as this one, but a new X. */
	Point3D withX(ValueType newX) const noexcept{ return Point3D(newX, y, z); }

		/** Returns a Point3D which has the same X, Z position as this one, but a new Y. */
	Point3D withY(ValueType newY) const noexcept{ return Point3D(x, newY, z); }

		/** Changes the Point3D's x and y coordinates. */
	void setXY(ValueType newX, ValueType newY) noexcept{ xyzChanged = true; x = newX; y = newY; }

		/** Changes the Point3D's x and z coordinates. */
	void setXZ(ValueType newX, ValueType newZ) noexcept{ xyzChanged = true; x = newX; z = newZ; }

		/** Changes the Point3D's y and z coordinates. */
	void setYZ(ValueType newY, ValueType newZ) noexcept{ xyzChanged = true; y = newY; z = newZ; }

		//==============================================================================
		/** Returns a Point3D with a given offset from this one. */
	Point3D translated(ValueType deltaX, ValueType deltaY, ValueType deltaZ) const noexcept{ return Point3D(getX() + deltaX, getY() + deltaY, getZ() + deltaZ); }

		/** Adds two Point3Ds together */
	Point3D operator+ (Point3D other) const noexcept{ return Point3D(getX() + other.getX(), getY() + other.getY(), getZ() + other.getZ()); }

		/** Subtracts one Point3Ds from another */
	Point3D operator- (Point3D other) const noexcept{ return Point3D(getX() - other.getX(), getY() - other.getY(), getZ() -other.getZ()); }

		/** Returns a Point3D whose coordinates are multiplied by a given scalar value. */
		template <typename FloatType>
	Point3D operator* (FloatType multiplier) const noexcept{ return Point3D((ValueType)(getX() * multiplier), (ValueType)(getY() * multiplier), (ValueType)(getZ() * multiplier)); }

		/** Returns a Point3D whose coordinates are divided by a given scalar value. */
		template <typename FloatType>
	Point3D operator/ (FloatType divisor) const noexcept{ return Point3D((ValueType)(getX() / divisor), (ValueType)(getY() / divisor), (ValueType)(getZ() / divisor)); }

		/** Returns the inverse of this Point3D. */
	Point3D operator-() const noexcept{ return Point3D(-getX(), -getY(), -getZ()); }

		//==============================================================================
		/** This type will be double if the Point3D's type is double, otherwise it will be float. */
	typedef typename TypeHelpers::SmallestFloatType<ValueType>::type FloatType;

	//	//==============================================================================
		/** Casts this Point3D to a Point3D<int> object. */
	Point3D<int> toInt() const noexcept{ return Point3D<int>(static_cast<int> (getX()), static_cast<int> (getY()), static_cast<int> (getZ())); }

		/** Casts this Point3D to a Point3D<float> object. */
	Point3D<float> toFloat() const noexcept{ return Point3D<float>(static_cast<float> (getX()), static_cast<float> (getY()), static_cast<float> (getZ())); }

		/** Casts this Point3D to a Point3D<double> object. */
	Point3D<double> toDouble() const noexcept{ return Point3D<double>(static_cast<double> (getX()), static_cast<double> (getY()), static_cast<double> (getZ())); }

		/** Casts this Point3D to a Point3D<int> object using roundToInt() to convert the values. */
	Point3D<int> roundToInt() const noexcept{ return Point3D<int>(juce::roundToInt(getX()), juce::roundToInt(getY()), juce::roundToInt(getZ())); }

		/** Returns the Point3D as a string in the form "x, y". */
	String toString()                                       { return "X:" + String(getX(), 2) + "; Y:" + String(getY(), 2) + "; Z:" + String(getZ(), 2); }
	String toStringAed()                                       { return "A:" + String(getAzimuth()*180/PI, 2) + "; E:" + String(getElevation()*180/PI, 2) + "; D:" + String(getDistance(), 2); }

	String toShortStringXyz()	{ return String(getX(), 2) + ", " + String(getY(), 2) + ", " + String(getZ(), 2); }
	String toShortStringAed()	{ return String(getAzimuth()*180/PI, 2) + ", " + String(getElevation()*180/PI, 2) + ", " + String(getDistance(), 2); }

	void getAmbisonicsCoefficients(int numCoefficients, ValueType* pCoefficients, bool flipDirection)
	{
		// speed optimized version, calculates all coefficients in one sequence

		ValueType a = (flipDirection ? -1.0 : 1.0) * getAzimuth();
		ValueType e = getElevation();

		pCoefficients[0] = 1.0;

		if (numCoefficients < 4)	// 0th order
			return;

		ValueType sinE = sin(e);
		ValueType cosE = cos(e);
		ValueType sinA = sin(a);
		ValueType cosA = cos(a);

		pCoefficients[1] = 3.0 * cosE * sinA;
		pCoefficients[2] = 3.0 * sinE;
		pCoefficients[3] = 3.0 * cosE * cosA;

		if (numCoefficients < 9) // 1st order
			return;

		ValueType cosE2 = pow(cos(e), 2.0);
		ValueType sin2A = sin(2.0 * a);
		ValueType sin2E = sin(2.0 * e);
		ValueType sinE2 = pow(sin(e), 2.0);
		ValueType cos2A = cos(2.0 * a);
		pCoefficients[4] = SQRT15 / 2.0 * cosE2 * sin2A * SQRT5;
		pCoefficients[5] = SQRT15 / 2.0 * sin2E * sinA * SQRT5;
		pCoefficients[6] = 2.5 * (3 * sinE2 - 1);
		pCoefficients[7] = SQRT15 / 2.0 * sin2E * cosA * SQRT5;
		pCoefficients[8] = SQRT15 / 2.0 * cosE2 * cos2A * SQRT5;

		if (numCoefficients < 16)	// 2nd order
			return;

		ValueType cosE3 = pow(cos(e), 3.0);
		ValueType sin3A = sin(3.0 * a);
		ValueType cos3A = cos(3.0 * a);
		pCoefficients[9] = SQRT35_8 * cosE3 * sin3A * SQRT7;
		pCoefficients[10] = SQRT105 / 2.0 * sinE * cosE2 * sin2A * SQRT7;
		pCoefficients[11] = SQRT21_8 * cosE * (5.0 * sinE2 - 1.0) * sinA * SQRT7;
		pCoefficients[12] = 3.5 * sinE * (5.0 * sinE2 - 3);
		pCoefficients[13] = SQRT21_8 * cosE * (5.0 * sinE2 - 1.0) * cosA * SQRT7;
		pCoefficients[14] = SQRT105 / 2.0 * sinE * cosE2 * cos2A * SQRT7;
		pCoefficients[15] = SQRT35_8 * cosE3 * cos3A * SQRT7;
			
		if (numCoefficients < 25)	// 3rd order
			return;

		ValueType cosE4 = pow(cos(e), 4.0);
		ValueType sin4A = sin(4.0 * a);
		ValueType sinE4 = pow(sin(e), 4.0);
		ValueType cos4A = cos(4.0 * a);
		pCoefficients[16] = 9.0 / 8.0 * SQRT35 * cosE4 * sin4A;
		pCoefficients[17] = 4.5 * SQRT35_2 * sinE * cosE3 * sin3A;
		pCoefficients[18] = 9.0 * SQRT5 / 4.0 * (7.0 * sinE2 - 1.0) * cosE2 * sin2A;
		pCoefficients[19] = 9.0 / 4.0 * SQRT5_2 * sin2E * (7.0 * sinE2 - 3.0) * sinA;
		pCoefficients[20] = 9.0 / 8.0 * (35.0 * sinE4 - 30.0 * sinE2 + 3.0);
		pCoefficients[21] = 9.0 / 4.0 * SQRT5_2 * sin2E * (7.0 * sinE2 - 3.0) * cosA;
		pCoefficients[22] = 9.0 * SQRT5 / 4.0 * (7.0 * sinE2 - 1.0) * cosE2 * cos2A;
		pCoefficients[23] = 4.5 * SQRT35_2 * sinE * cosE3 * cos3A;
		pCoefficients[24] = 9.0 / 8.0 * SQRT35 * cosE4 * cos4A;

		if (numCoefficients < 36)	// 4th order
			return;

		ValueType cosE5 = pow(cos(e), 5.0);
		ValueType sin5A = sin(5.0 * a);
		ValueType sinE5 = pow(sin(e), 5.0);
		ValueType sinE3 = pow(sin(e), 3.0);
		ValueType cos5A = cos(5.0 * a);
		pCoefficients[25] = 3.0 / 8.0 * SQRT77_2 * cosE5 * sin5A * SQRT11;
		pCoefficients[26] = 3.0 / 8.0 * SQRT385 * sinE * cosE4 * sin4A * SQRT11;
		pCoefficients[27] = 1.0 / 8.0 * SQRT385_2 * (9.0 * sinE2 - 1.0) * cosE3 * sin3A * SQRT11;
		pCoefficients[28] = SQRT1155 / 4.0 * sinE * (3.0 * sinE2 - 1.0) * cosE2 * sin2A * SQRT11;
		pCoefficients[29] = SQRT165 / 8.0 * (21.0 * sinE4 - 14.0 * sinE2 + 1.0) * cosE * sinA * SQRT11;
		pCoefficients[30] = 11.0 / 8.0 * (63.0 * sinE5 - 70.0 * sinE3 + 15.0 * sinA);
		pCoefficients[31] = SQRT165 / 8.0 * (21.0 * sinE4 - 14.0 * sinE2 + 1.0) * cosE * cosA * SQRT11;
		pCoefficients[32] = SQRT1155 / 4.0 * sinE * (3.0 * sinE2 - 1.0) * cosE2 * cos2A * SQRT11;
		pCoefficients[33] = 1.0 / 8.0 * SQRT385_2 * (9.0 * sinE2 - 1.0) * cosE3 * cos3A * SQRT11;
		pCoefficients[34] = 3.0 / 8.0 * SQRT385 * sinE * cosE4 * cos4A * SQRT11;
		pCoefficients[35] = 3.0 / 8.0 * SQRT77_2 * cosE5 * cos5A * SQRT11;

		if (numCoefficients < 49)	// 5th order
			return;

		ValueType cosE6 = pow(cos(e), 6.0);
		ValueType sin6A = sin(6.0 * a);
		ValueType sinE6 = pow(sin(e), 6.0);
		ValueType cos6A = cos(6.0 * a);
		pCoefficients[36] = 1.0 / 16.0 * SQRT3003_2 * cosE6 * sin6A * SQRT13;
		pCoefficients[37] = 3.0 / 8.0 * SQRT1001_2 * sinE * cosE5 * sin5A * SQRT13;
		pCoefficients[38] = 3.0 / 16.0 * SQRT91 * (11.0 * sinE2 - 1.0) * cosE4 * sin4A * SQRT13;
		pCoefficients[39] = 1.0 / 8.0 * SQRT1365_2 * sinE * (11.0 * sinE2 - 3.0) * cosE3 * sin3A * SQRT13;
		pCoefficients[40] = 1.0 / 16.0 * SQRT1365_2 * (33.0 * sinE4 - 18.0 * sinE2 + 1.0) * cosE2 * sin2A * SQRT13;
		pCoefficients[41] = 1.0 / 16.0 * SQRT273 * sin2E * (33.0 * sinE4 - 30.0 * sinE2 + 5.0) * sinA * SQRT13;
		pCoefficients[42] = 13.0 / 16.0 * (231.0 * sinE6 - 315.0 * sinE4 + 105.0 * sinE2 - 5.0);
		pCoefficients[43] = 1.0 / 16.0 * SQRT273 * sin2E * (33.0 * sinE4 - 30.0 * sinE2 + 5.0) * cosA * SQRT13;
		pCoefficients[44] = 1.0 / 16.0 * SQRT1365_2 * (33.0 * sinE4 - 18.0 * sinE2 + 1.0) * cosE2 * cos2A * SQRT13;
		pCoefficients[45] = 1.0 / 8.0 * SQRT1365_2 * sinE * (11.0 * sinE2 - 3.0) * cosE3 * cos3A * SQRT13;
		pCoefficients[46] = 3.0 / 16.0 * SQRT91 * (11.0 * sinE2 - 1.0) * cosE4 * cos4A * SQRT13;
		pCoefficients[47] = 3.0 / 8.0 * SQRT1001_2 * sinE * cosE5 * cos5A * SQRT13;
		pCoefficients[48] = 1.0 / 16.0 * SQRT3003_2 * cosE6 * cos6A * SQRT13;

		if (numCoefficients < 64)	// 6th order
			return;

		ValueType cosE7 = pow(cos(e), 7.0);
		ValueType sin7A = sin(7.0 * a);
		ValueType sinE7 = pow(sin(e), 7.0);
		ValueType cos7A = cos(7.0 * a);
		pCoefficients[49] = 3.0 / 32.0 * SQRT715 * cosE7 * sin7A * SQRT15;
		pCoefficients[50] = 3.0 / 16.0 * SQRT5005_2 * sinE * cosE6 * sin6A * SQRT15;
		pCoefficients[51] = 3.0 / 32.0 * SQRT385 * (13.0 * sinE2 - 1.0) * cosE5 * sin5A * SQRT15;
		pCoefficients[52] = 3.0 / 16.0 * SQRT385 * (13.0 * sinE3 - 3.0 * sinE) * cosE4 * sin4A * SQRT15;
		pCoefficients[53] = 3.0 / 32.0 * SQRT35 * (143.0 * sinE4 - 66.0 * sinE2 + 3.0) * cosE3 * sin3A * SQRT15;
		pCoefficients[54] = 3.0 / 16.0 * SQRT35_2 * (143.0 * sinE5 - 110.0 * sinE3 + 15.0 * sinE) * cosE2 * sin2A * SQRT15;
		pCoefficients[55] = 1.0 / 32.0 * SQRT105 * (429.0 * sinE6 - 495.0 * sinE4 + 135.0 * sinE2 - 5.0) * cosE * sinA * SQRT15;
		pCoefficients[56] = 1.0 / 16.0 * 15.0 * (429.0 * sinE7 - 693.0 * sinE5 + 315.0 * sinE3 - 35.0 * sinE);
		pCoefficients[57] = 1.0 / 32.0 * SQRT105 * (429.0 * sinE6 - 495.0 * sinE4 + 135.0 * sinE2 - 5.0) * cosE * cosA * SQRT15;
		pCoefficients[58] = 3.0 / 16.0 * SQRT35_2 * (143.0 * sinE5 - 110.0 * sinE3 + 15.0 * sinE) * cosE2 * cos2A * SQRT15;
		pCoefficients[59] = 3.0 / 32.0 * SQRT35 * (143.0 * sinE4 - 66.0 * sinE2 + 3.0) * cosE3 * cos3A * SQRT15;
		pCoefficients[60] = 3.0 / 16.0 * SQRT385 * (13.0 * sinE3 - 3.0 * sinE) * cosE4 * cos4A * SQRT15;
		pCoefficients[61] = 3.0 / 32.0 * SQRT385 * (13.0 * sinE2 - 1.0) * cosE5 * cos5A * SQRT15;
		pCoefficients[62] = 3.0 / 16.0 * SQRT5005_2 * sinE * cosE6 * cos6A * SQRT15;
		pCoefficients[63] = 3.0 / 32.0 * SQRT715 * cosE7 * cos7A * SQRT15;
	}

	ValueType getAmbisonicsCoefficient(int ambiChannel, bool applyDistance, bool flipDirection)
	{
		// first implementation of coefficient calculation, allows to apply distance gain and is easier to use for debugging

		double value;
		double a = (flipDirection ? -1.0 : 1.0) * getAzimuth();
		double e = getElevation();
		switch (ambiChannel) 
		{
		case 0: // W
			value = 1.0; 
			break;

			////////////////////////////////////////////////
			// Order 1
			////////////////////////////////////////////////
		case 1: // Y
			value = 3 * sin(a) * cos(e);
			break;
		case 2: // Z
			value = 3 * sin(e);
			break;
		case 3: // X
			value = 3 * cos(a) * cos(e);
			break;

			////////////////////////////////////////////////
			// Order 2
			////////////////////////////////////////////////
		case 4: // V
			value = SQRT15 / 2.0 * pow(cos(e), 2.0) * sin(2.0 * a) * SQRT5;
			break;
		case 5: // T
			value = SQRT15 / 2.0 * sin(2.0 * e) * sin(a) * SQRT5;
			break;
		case 6:	// R
			value = 2.5 * (3 * pow(sin(e), 2.0) - 1);
			break;
		case 7: // S
			value = SQRT15 / 2.0 * sin(2 * e) * cos(a) * SQRT5;
			break;
		case 8: // U
			value = SQRT15 / 2.0 * pow(cos(e), 2.0) * cos(2.0 * a) * SQRT5;
			break;

			////////////////////////////////////////////////
			// Order 3
			////////////////////////////////////////////////
		case 9: // Q
			value = SQRT35_8 * pow(cos(e), 3.0) * sin(3.0 * a) * SQRT7;
			break;
		case 10:	// O
			value = SQRT105 / 2.0 * sin(e) * pow(cos(e), 2.0) * sin(2.0 * a) * SQRT7;
			break;
		case 11:	// M
			value = SQRT21_8 * cos(e) * (5.0 * pow(sin(e), 2.0) - 1.0) * sin(a) * SQRT7;
			break;
		case 12:	// K
			value = 3.5 * sin(e) * (5.0 * pow(sin(e), 2.0) - 3);
			break;
		case 13:	// L
			value = SQRT21_8 * cos(e) * (5.0 * pow(sin(e), 2.0) - 1.0) * cos(a) * SQRT7;
			break;
		case 14:	// N
			value = SQRT105 / 2.0 * sin(e) * pow(cos(e), 2.0) * cos(2.0 * a) * SQRT7;
			break;
		case 15:	// P
			value = SQRT35_8 * pow(cos(e), 3.0) * cos(3.0 * a) * SQRT7;
			break;

			////////////////////////////////////////////////
			// Order 4
			////////////////////////////////////////////////
		case 16:
			value = 9.0 / 8.0 * SQRT35 * pow(cos(e), 4.0) * sin(4.0 * a);
			break;
		case 17:
			value = 4.5 * SQRT35_2 * sin(e) * pow(cos(e), 3.0) * sin(3.0 * a);
			break;
		case 18:
			value = 9.0 * SQRT5 / 4.0 * (7.0 * pow(sin(e), 2.0) - 1.0) * pow(cos(e), 2.0) * sin(2.0 * a);
			break;
		case 19:
			value = 9.0 / 4.0 * SQRT5_2 * sin(2.0 * e) * (7.0 * pow(sin(e), 2.0) - 3.0) * sin(a);
			break;
		case 20:
			value = 9.0 / 8.0 * (35.0 * pow(sin(e), 4.0) - 30.0 * pow(sin(e), 2.0) + 3.0);
			break;
		case 21:
			value = 9.0 / 4.0 * SQRT5_2 * sin(2.0 * e) * (7.0 * pow(sin(e), 2.0) - 3.0) * cos(a);
			break;
		case 22:
			value = 9.0 * SQRT5 / 4.0 * (7.0 * pow(sin(e), 2.0) - 1.0) * pow(cos(e), 2.0) * cos(2.0 * a);
			break;
		case 23:
			value = 4.5 * SQRT35_2 * sin(e) * pow(cos(e), 3.0) * cos(3.0 * a);
			break;
		case 24:
			value = 9.0 / 8.0 * SQRT35 * pow(cos(e), 4.0) * cos(4.0 * a);
			break;

			////////////////////////////////////////////////
			// Order 5
			////////////////////////////////////////////////
		case 25:
			value = 3.0 / 8.0 * SQRT77_2 * pow(cos(e), 5.0) * sin(5.0 * a) * SQRT11;
			break;
		case 26:
			value = 3.0 / 8.0 * SQRT385 * sin(e) * pow(cos(e), 4.0) * sin(4.0 * a) * SQRT11;
			break;
		case 27:
			value = 1.0 / 8.0 * SQRT385_2 * (9.0 * pow(sin(e), 2.0) - 1.0) * pow(cos(e), 3.0) * sin(3.0 * a) * SQRT11;
			break;
		case 28:
			value = SQRT1155 / 4.0 * sin(e) * (3.0 * pow(sin(e), 2.0) - 1.0) * pow(cos(e), 2.0) * sin(2.0 * a) * SQRT11;
			break;
		case 29:
			value = SQRT165 / 8.0 * (21.0 * pow(sin(e), 4.0) - 14.0 * pow(sin(e), 2.0) + 1.0) * cos(e) * sin(a) * SQRT11;
			break;
		case 30:
			value = 11.0 / 8.0 * (63.0 * pow(sin(e), 5.0) - 70.0 * pow(sin(e), 3.0) + 15.0 * sin(a));
			break;
		case 31:
			value = SQRT165 / 8.0 * (21.0 * pow(sin(e), 4.0) - 14.0 * pow(sin(e), 2.0) + 1.0) * cos(e) * cos(a) * SQRT11;
			break;
		case 32:
			value = SQRT1155 / 4.0 * sin(e) * (3.0 * pow(sin(e), 2.0) - 1.0) * pow(cos(e), 2.0) * cos(2.0 * a) * SQRT11;
			break;
		case 33:
			value = 1.0 / 8.0 * SQRT385_2 * (9.0 * pow(sin(e), 2.0) - 1.0) * pow(cos(e), 3.0) * cos(3.0 * a) * SQRT11;
			break;
		case 34:
			value = 3.0 / 8.0 * SQRT385 * sin(e) * pow(cos(e), 4.0) * cos(4.0 * a) * SQRT11;
			break;
		case 35:
			value = 3.0 / 8.0 * SQRT77_2 * pow(cos(e), 5.0) * cos(5.0 * a) * SQRT11;
			break;
		default:
			return 0;
		}
		
		if (applyDistance)
			return (1.0-getDistance()) * value;
		else
			return value;
	}

private:
	//==============================================================================
	ValueType x; /**< The Point3D's X coordinate. */
	ValueType y; /**< The Point3D's Y coordinate. */
	ValueType z; /**< The Point3D's Z coordinate. */
	bool xyzChanged;

	ValueType azimuth;		// phi
	ValueType elevation;	// theta
	ValueType distance;		// d
	bool aedChanged;

	AudioParameterSet audioParams;

	void calculateAed()
	{
		distance = sqrt(pow(x, 2.0) + pow(y, 2.0) + pow(z, 2.0));
		distance = distance < DISTANCE_MIN_VALUE ? DISTANCE_MIN_VALUE : distance;
		
		azimuth = atan2(y, x);
		elevation = atan2(z, sqrt(pow(x, 2.0) + pow(y, 2.0)));
		xyzChanged = false;

		audioParams.notifyA(azimuth);
		audioParams.notifyE(elevation);
		audioParams.notifyD(distance);
	}

	void calculateXyz()
	{
		x = distance * cos(elevation) * cos(azimuth);
		y = distance * cos(elevation) * sin(azimuth);
		z = distance * sin(elevation);
		aedChanged = false;

		audioParams.notifyX(x);
		audioParams.notifyY(y);
		audioParams.notifyZ(z);
	}
};

/** Multiplies the Point3D's coordinates by a scalar value. */
template <typename ValueType>
Point3D<ValueType> operator* (ValueType value, Point3D<ValueType> p) noexcept{ return p * value; }

#endif  // POINT3D_H_INCLUDED

