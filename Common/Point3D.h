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

	void getAmbisonicsCoefficients(int numCoefficients, ValueType* pCoefficients, bool flipDirection, bool inverseNormalisation)
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
		ValueType snf1 = inverseNormalisation ? 1.0 : 3.0;
		pCoefficients[1] = snf1 * cosE * sinA;
		pCoefficients[2] = snf1 * sinE;
		pCoefficients[3] = snf1 * cosE * cosA;

		if (numCoefficients < 9) // 1st order
			return;

		ValueType cosE2 = pow(cos(e), 2.0);
		ValueType sin2A = sin(2.0 * a);
		ValueType sin2E = sin(2.0 * e);
		ValueType sinE2 = pow(sin(e), 2.0);
		ValueType cos2A = cos(2.0 * a);
		ValueType nf2 = inverseNormalisation ? 1.0 / SQRT5 : SQRT5;
		pCoefficients[4] = nf2 * SQRT15 / 2.0 * cosE2 * sin2A;
		pCoefficients[5] = nf2 * SQRT15 / 2.0 * sin2E * sinA;
		pCoefficients[6] = nf2 * SQRT5 / 2.0 * (3 * sinE2 - 1);
		pCoefficients[7] = nf2 * SQRT15 / 2.0 * sin2E * cosA;
		pCoefficients[8] = nf2 * SQRT15 / 2.0 * cosE2 * cos2A;

		if (numCoefficients < 16)	// 2nd order
			return;

		ValueType cosE3 = pow(cos(e), 3.0);
		ValueType sin3A = sin(3.0 * a);
		ValueType cos3A = cos(3.0 * a);
		ValueType nf3 = inverseNormalisation ? 1.0 / SQRT7 : SQRT7;
		pCoefficients[9] = nf3 * SQRT35_8 * cosE3 * sin3A;
		pCoefficients[10] = nf3 * SQRT105 / 2.0 * sinE * cosE2 * sin2A;
		pCoefficients[11] = nf3 * SQRT21_8 * cosE * (5.0 * sinE2 - 1.0) * sinA;
		pCoefficients[12] = nf3 * SQRT7 / 2.0 * sinE * (5.0 * sinE2 - 3);
		pCoefficients[13] = nf3 * SQRT21_8 * cosE * (5.0 * sinE2 - 1.0) * cosA;
		pCoefficients[14] = nf3 * SQRT105 / 2.0 * sinE * cosE2 * cos2A;
		pCoefficients[15] = nf3 * SQRT35_8 * cosE3 * cos3A;
			
		if (numCoefficients < 25)	// 3rd order
			return;

		ValueType cosE4 = pow(cos(e), 4.0);
		ValueType sin4A = sin(4.0 * a);
		ValueType sinE4 = pow(sin(e), 4.0);
		ValueType cos4A = cos(4.0 * a);
		ValueType nf4 = inverseNormalisation ? 1.0 / 3.0 : 3.0;
		pCoefficients[16] = nf4 * 3.0 / 8.0 * SQRT35 * cosE4 * sin4A;
		pCoefficients[17] = nf4 * 1.5 * SQRT35_2 * sinE * cosE3 * sin3A;
		pCoefficients[18] = nf4 * 3.0 * SQRT5 / 4.0 * (7.0 * sinE2 - 1.0) * cosE2 * sin2A;
		pCoefficients[19] = nf4 * 3.0 / 4.0 * SQRT5_2 * sin2E * (7.0 * sinE2 - 3.0) * sinA;
		pCoefficients[20] = nf4 * 3.0 / 8.0 * (35.0 * sinE4 - 30.0 * sinE2 + 3.0);
		pCoefficients[21] = nf4 * 3.0 / 4.0 * SQRT5_2 * sin2E * (7.0 * sinE2 - 3.0) * cosA;
		pCoefficients[22] = nf4 * 3.0 * SQRT5 / 4.0 * (7.0 * sinE2 - 1.0) * cosE2 * cos2A;
		pCoefficients[23] = nf4 * 1.5 * SQRT35_2 * sinE * cosE3 * cos3A;
		pCoefficients[24] = nf4 * 3.0 / 8.0 * SQRT35 * cosE4 * cos4A;

		if (numCoefficients < 36)	// 4th order
			return;

		ValueType cosE5 = pow(cos(e), 5.0);
		ValueType sin5A = sin(5.0 * a);
		ValueType sinE5 = pow(sin(e), 5.0);
		ValueType sinE3 = pow(sin(e), 3.0);
		ValueType cos5A = cos(5.0 * a);
		ValueType nf5 = inverseNormalisation ? 1.0 / SQRT11 : SQRT11;
		pCoefficients[25] = nf5 * 3.0 / 8.0 * SQRT77_2 * cosE5 * sin5A;
		pCoefficients[26] = nf5 * 3.0 / 8.0 * SQRT385 * sinE * cosE4 * sin4A;
		pCoefficients[27] = nf5 / 8.0 * SQRT385_2 * (9.0 * sinE2 - 1.0) * cosE3 * sin3A;
		pCoefficients[28] = nf5 * SQRT1155 / 4.0 * sinE * (3.0 * sinE2 - 1.0) * cosE2 * sin2A;
		pCoefficients[29] = nf5 * SQRT165 / 8.0 * (21.0 * sinE4 - 14.0 * sinE2 + 1.0) * cosE * sinA;
		pCoefficients[30] = nf5 * SQRT11 / 8.0 * (63.0 * sinE5 - 70.0 * sinE3 + 15.0 * sinA);
		pCoefficients[31] = nf5 * SQRT165 / 8.0 * (21.0 * sinE4 - 14.0 * sinE2 + 1.0) * cosE * cosA;
		pCoefficients[32] = nf5 * SQRT1155 / 4.0 * sinE * (3.0 * sinE2 - 1.0) * cosE2 * cos2A;
		pCoefficients[33] = nf5 / 8.0 * SQRT385_2 * (9.0 * sinE2 - 1.0) * cosE3 * cos3A;
		pCoefficients[34] = nf5 * 3.0 / 8.0 * SQRT385 * sinE * cosE4 * cos4A;
		pCoefficients[35] = nf5 * 3.0 / 8.0 * SQRT77_2 * cosE5 * cos5A;

		if (numCoefficients < 49)	// 5th order
			return;

		ValueType cosE6 = pow(cos(e), 6.0);
		ValueType sin6A = sin(6.0 * a);
		ValueType sinE6 = pow(sin(e), 6.0);
		ValueType cos6A = cos(6.0 * a);
		ValueType nf6 = inverseNormalisation ? 1.0 / SQRT13 : SQRT13;
		pCoefficients[36] = nf6 / 16.0 * SQRT3003_2 * cosE6 * sin6A;
		pCoefficients[37] = nf6 * 3.0 / 8.0 * SQRT1001_2 * sinE * cosE5 * sin5A;
		pCoefficients[38] = nf6 * 3.0 / 16.0 * SQRT91 * (11.0 * sinE2 - 1.0) * cosE4 * sin4A;
		pCoefficients[39] = nf6 / 8.0 * SQRT1365_2 * sinE * (11.0 * sinE2 - 3.0) * cosE3 * sin3A;
		pCoefficients[40] = nf6 / 16.0 * SQRT1365_2 * (33.0 * sinE4 - 18.0 * sinE2 + 1.0) * cosE2 * sin2A;
		pCoefficients[41] = nf6 / 16.0 * SQRT273 * sin2E * (33.0 * sinE4 - 30.0 * sinE2 + 5.0) * sinA;
		pCoefficients[42] = nf6 * SQRT13 / 16.0 * (231.0 * sinE6 - 315.0 * sinE4 + 105.0 * sinE2 - 5.0);
		pCoefficients[43] = nf6 / 16.0 * SQRT273 * sin2E * (33.0 * sinE4 - 30.0 * sinE2 + 5.0) * cosA;
		pCoefficients[44] = nf6 / 16.0 * SQRT1365_2 * (33.0 * sinE4 - 18.0 * sinE2 + 1.0) * cosE2 * cos2A;
		pCoefficients[45] = nf6 / 8.0 * SQRT1365_2 * sinE * (11.0 * sinE2 - 3.0) * cosE3 * cos3A;
		pCoefficients[46] = nf6 * 3.0 / 16.0 * SQRT91 * (11.0 * sinE2 - 1.0) * cosE4 * cos4A;
		pCoefficients[47] = nf6 * 3.0 / 8.0 * SQRT1001_2 * sinE * cosE5 * cos5A;
		pCoefficients[48] = nf6 / 16.0 * SQRT3003_2 * cosE6 * cos6A;

		if (numCoefficients < 64)	// 6th order
			return;

		ValueType cosE7 = pow(cos(e), 7.0);
		ValueType sin7A = sin(7.0 * a);
		ValueType sinE7 = pow(sin(e), 7.0);
		ValueType cos7A = cos(7.0 * a);
		ValueType nf7 = inverseNormalisation ? 1.0 / SQRT15 : SQRT15;
		pCoefficients[49] = nf7 * 3.0 / 32.0 * SQRT715 * cosE7 * sin7A;
		pCoefficients[50] = nf7 * 3.0 / 16.0 * SQRT5005_2 * sinE * cosE6 * sin6A;
		pCoefficients[51] = nf7 * 3.0 / 32.0 * SQRT385 * (13.0 * sinE2 - 1.0) * cosE5 * sin5A;
		pCoefficients[52] = nf7 * 3.0 / 16.0 * SQRT385 * (13.0 * sinE3 - 3.0 * sinE) * cosE4 * sin4A;
		pCoefficients[53] = nf7 * 3.0 / 32.0 * SQRT35 * (143.0 * sinE4 - 66.0 * sinE2 + 3.0) * cosE3 * sin3A;
		pCoefficients[54] = nf7 * 3.0 / 16.0 * SQRT35_2 * (143.0 * sinE5 - 110.0 * sinE3 + 15.0 * sinE) * cosE2 * sin2A;
		pCoefficients[55] = nf7 / 32.0 * SQRT105 * (429.0 * sinE6 - 495.0 * sinE4 + 135.0 * sinE2 - 5.0) * cosE * sinA;
		pCoefficients[56] = nf7 / 16.0 * SQRT15 * (429.0 * sinE7 - 693.0 * sinE5 + 315.0 * sinE3 - 35.0 * sinE);
		pCoefficients[57] = nf7 / 32.0 * SQRT105 * (429.0 * sinE6 - 495.0 * sinE4 + 135.0 * sinE2 - 5.0) * cosE * cosA;
		pCoefficients[58] = nf7 * 3.0 / 16.0 * SQRT35_2 * (143.0 * sinE5 - 110.0 * sinE3 + 15.0 * sinE) * cosE2 * cos2A;
		pCoefficients[59] = nf7 * 3.0 / 32.0 * SQRT35 * (143.0 * sinE4 - 66.0 * sinE2 + 3.0) * cosE3 * cos3A;
		pCoefficients[60] = nf7 * 3.0 / 16.0 * SQRT385 * (13.0 * sinE3 - 3.0 * sinE) * cosE4 * cos4A;
		pCoefficients[61] = nf7 * 3.0 / 32.0 * SQRT385 * (13.0 * sinE2 - 1.0) * cosE5 * cos5A;
		pCoefficients[62] = nf7 * 3.0 / 16.0 * SQRT5005_2 * sinE * cosE6 * cos6A;
		pCoefficients[63] = nf7 * 3.0 / 32.0 * SQRT715 * cosE7 * cos7A;
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

