/*
  ==============================================================================

    Point3D.h
    Created: 28 Jul 2016 8:17:47am
    Author:  christian.schweizer

  ==============================================================================
*/

#ifndef POINT3D_H_INCLUDED
#define POINT3D_H_INCLUDED

#include "JuceHeader.h"
#define SQRT3 sqrt(3)
#define PI 3.1415926535897932384626433832795

template <typename ValueType>
class Point3D
{
public:
	/** Creates a Point3D at the origin */
	Point3D() noexcept : x(), y(), z() { xyzChanged = true; aedChanged = false; }

	/** Creates a copy of another Point3D. */
	Point3D(const Point3D& other) noexcept : x(other.x), y(other.y), z(other.z) { xyzChanged = true; aedChanged = false; }

	/** Creates a Point3D from an (x, y, z) position. */
	Point3D(ValueType initialX, ValueType initialY, ValueType initialZ) noexcept : x(initialX), y(initialY), z(initialZ) { xyzChanged = true; aedChanged = false; }

	//==============================================================================
	/** Copies this Point3D from another one. */
	Point3D& operator= (Point3D& other) noexcept{ xyzChanged = true; x = other.getX(); y = other.getY(); z = other.getZ(); return *this; }

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
	inline void setX(ValueType newX) noexcept{ xyzChanged = true; x = newX; }

		/** Sets the Point3D's y coordinate. */
	inline void setY(ValueType newY) noexcept{ xyzChanged = true; y = newY; }

		/** Sets the Point3D's z coordinate. */
	inline void setZ(ValueType newZ) noexcept{ xyzChanged = true; z = newZ; }

		/** Sets the Point3D's azimuth. */
	inline void setAzimuth(ValueType newAzimuth) noexcept{ aedChanged = true; azimuth = newAzimuth; }

		/** Sets the Point3D's elevation. */
	inline void setElevation(ValueType newElevation) noexcept{ aedChanged = true; elevation = newElevation; }

		/** Sets the Point3D's distance. */
	inline void setDistance(ValueType newDistance) noexcept{ aedChanged = true; distance = newDistance; }

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

	//	/** Adds a pair of coordinates to this value. */
	//void addXY(ValueType xToAdd, ValueType yToAdd) noexcept{ xyzChanged = true; x += xToAdd; y += yToAdd; }

		//==============================================================================
		/** Returns a Point3D with a given offset from this one. */
	Point3D translated(ValueType deltaX, ValueType deltaY, ValueType deltaZ) const noexcept{ return Point3D(getX() + deltaX, getY() + deltaY, getZ() + deltaZ); }

		/** Adds two Point3Ds together */
	Point3D operator+ (Point3D other) const noexcept{ return Point3D(getX() + other.getX(), getY() + other.getY(), getZ() + other.getZ()); }

	//	/** Adds another Point3D's coordinates to this one */
	//Point3D& operator+= (Point3D other) noexcept{ this.xyzChanged = true; x += other.x; y += other.y; z += other.z; return *this; }

		/** Subtracts one Point3Ds from another */
	Point3D operator- (Point3D other) const noexcept{ return Point3D(getX() - other.getX(), getY() - other.getY(), getZ() -other.getZ()); }

	//	/** Subtracts another Point3D's coordinates to this one */
	//Point3D& operator-= (Point3D other) noexcept{ this.xyzChanged = true; x -= other.x; y -= other.y; z -= other.z; return *this; }

		
	//	/** Multiplies two Point3Ds together */
	//	template <typename OtherType>
	//Point3D operator* (Point3D<OtherType> other) const noexcept{ return Point3D((ValueType)(x * other.x), (ValueType)(y * other.y)); }

	//	/** Multiplies another Point3D's coordinates to this one */
	//	template <typename OtherType>
	//Point3D& operator*= (Point3D<OtherType> other) noexcept{ *this = *this * other; return *this; }

	//	/** Divides one Point3D by another */
	//	template <typename OtherType>
	//Point3D operator/ (Point3D<OtherType> other) const noexcept{ return Point3D((ValueType)(x / other.x), (ValueType)(y / other.y)); }

	//	/** Divides this Point3D's coordinates by another */
	//	template <typename OtherType>
	//Point3D& operator/= (Point3D<OtherType> other) noexcept{ *this = *this / other; return *this; }

		/** Returns a Point3D whose coordinates are multiplied by a given scalar value. */
		template <typename FloatType>
	Point3D operator* (FloatType multiplier) const noexcept{ return Point3D((ValueType)(getX() * multiplier), (ValueType)(getY() * multiplier), (ValueType)(getZ() * multiplier)); }

		/** Returns a Point3D whose coordinates are divided by a given scalar value. */
		template <typename FloatType>
	Point3D operator/ (FloatType divisor) const noexcept{ return Point3D((ValueType)(getX() / divisor), (ValueType)(getY() / divisor), (ValueType)(getZ() / divisor)); }

	//	/** Multiplies the Point3D's coordinates by a scalar value. */
	//	template <typename FloatType>
	//Point3D& operator*= (FloatType multiplier) noexcept{ this.xyzChanged = true; x = (ValueType)(x * multiplier); y = (ValueType)(y * multiplier); z = (ValueType)(z * multiplier); return *this; }

	//	/** Divides the Point3D's coordinates by a scalar value. */
	//	template <typename FloatType>
	//Point3D& operator/= (FloatType divisor) noexcept{ this.xyzChanged = true; x = (ValueType)(x / divisor); y = (ValueType)(y / divisor); z = (ValueType)(z / divisor); return *this; }

		/** Returns the inverse of this Point3D. */
	Point3D operator-() const noexcept{ return Point3D(-getX(), -getY(), -getZ()); }

		//==============================================================================
		/** This type will be double if the Point3D's type is double, otherwise it will be float. */
	typedef typename TypeHelpers::SmallestFloatType<ValueType>::type FloatType;

	////==============================================================================
	///** Returns the straight-line distance between this Point3D and the origin. */
	//ValueType getDistanceFromOrigin() const noexcept{ return juce_hypot(x, y); }

	//	/** Returns the straight-line distance between this Point3D and another one. */
	//ValueType getDistanceFrom(Point3D other) const noexcept{ return juce_hypot(x - other.x, y - other.y); }

	//	/** Returns the square of the straight-line distance between this Point3D and the origin. */
	//ValueType getDistanceSquaredFromOrigin() const noexcept{ return x * x + y * y; }

	//	/** Returns the square of the straight-line distance between this Point3D and another one. */
	//ValueType getDistanceSquaredFrom(Point3D other) const noexcept{ return (*this - other).getDistanceSquaredFromOrigin(); }

	//	/** Returns the angle from this Point3D to another one.

	//	Taking this Point3D to be the centre of a circle, and the other Point3D being a position on
	//	the circumference, the return value is the number of radians clockwise from the 12 o'clock
	//	direction.
	//	So 12 o'clock = 0, 3 o'clock = Pi/2, 6 o'clock = Pi, 9 o'clock = -Pi/2
	//	*/
	//	FloatType getAngleToPoint3D(Point3D other) const noexcept
	//{
	//	return static_cast<FloatType> (std::atan2(static_cast<FloatType> (other.x - x),
	//	static_cast<FloatType> (y - other.y)));
	//}

	//	/** Returns the Point3D that would be reached by rotating this Point3D clockwise
	//	about the origin by the specified angle.
	//	*/
	//	Point3D rotatedAboutOrigin(ValueType angleRadians) const noexcept
	//{
	//	return Point3D(x * std::cos(angleRadians) - y * std::sin(angleRadians),
	//	x * std::sin(angleRadians) + y * std::cos(angleRadians));
	//}

	//	/** Taking this Point3D to be the centre of a circle, this returns a Point3D on its circumference.
	//	@param radius   the radius of the circle.
	//	@param angle    the angle of the Point3D, in radians clockwise from the 12 o'clock position.
	//	*/
	//	Point3D<FloatType> getPoint3DOnCircumference(float radius, float angle) const noexcept
	//{
	//	return Point3D<FloatType>(static_cast<FloatType> (x + radius * std::sin(angle)),
	//	static_cast<FloatType> (y - radius * std::cos(angle)));
	//}

	//	/** Taking this Point3D to be the centre of an ellipse, this returns a Point3D on its circumference.
	//	@param radiusX  the horizontal radius of the circle.
	//	@param radiusY  the vertical radius of the circle.
	//	@param angle    the angle of the Point3D, in radians clockwise from the 12 o'clock position.
	//	*/
	//	Point3D<FloatType> getPoint3DOnCircumference(float radiusX, float radiusY, float angle) const noexcept
	//{
	//	return Point3D<FloatType>(static_cast<FloatType> (x + radiusX * std::sin(angle)),
	//	static_cast<FloatType> (y - radiusY * std::cos(angle)));
	//}

	//	/** Returns the dot-product of two Point3Ds (x1 * x2 + y1 * y2). */
	//FloatType getDotProduct(Point3D other) const noexcept{ return x * other.x + y * other.y; }

	//	//==============================================================================
	//	/** Uses a transform to change the Point3D's coordinates.
	//	This will only compile if ValueType = float!

	//	@see AffineTransform::transformPoint3D
	//	*/
	//void applyTransform(const AffineTransform& transform) noexcept{ transform.transformPoint3D(x, y); }

	//	/** Returns the position of this Point3D, if it is transformed by a given AffineTransform. */
	//	Point3D transformedBy(const AffineTransform& transform) const noexcept
	//{
	//	return Point3D(static_cast<ValueType> (transform.mat00 * x + transform.mat01 * y + transform.mat02),
	//	static_cast<ValueType> (transform.mat10 * x + transform.mat11 * y + transform.mat12));
	//}

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

	ValueType getAmbisonicsCoefficient(int ambiChannel, bool applyDistance)
	{
		double value;

		switch (ambiChannel) 
		{
		case 0: // W
			value = 1.0; 
			break;
		case 1: // X
			value = SQRT3 * cos(getElevation()) * cos(getElevation());
			break;
		case 2: // Y
			value = SQRT3 * sin(getAzimuth()) * cos(getElevation());
			break;
		case 3: // Z
			value = SQRT3 * sin(getElevation());
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

	String label;

	void calculateAed()
	{
		distance = sqrt(pow(x, 2.0) + pow(y, 2.0) + pow(z, 2.0));
		azimuth = atan2(y, x);
		elevation = atan2(z, sqrt(pow(x, 2.0) + pow(y, 2.0)));
		xyzChanged = false;
	}

	void calculateXyz()
	{
		x = distance * cos(elevation) * cos(azimuth);
		y = distance * cos(elevation) * sin(azimuth);
		z = distance * sin(elevation);
		aedChanged = false;
	}
};

/** Multiplies the Point3D's coordinates by a scalar value. */
template <typename ValueType>
Point3D<ValueType> operator* (ValueType value, Point3D<ValueType> p) noexcept{ return p * value; }

#endif  // POINT3D_H_INCLUDED

