#include "MathHelper.h"

juce::Quaternion<double> MathHelper::EulerToQuaternion(double roll, double pitch, double yaw) // roll (x), pitch (y), yaw (z), angles are in radians
{
    // Abbreviations for the various angular functions

    double cr = cos(roll * 0.5);
    double sr = sin(roll * 0.5);
    double cp = cos(pitch * 0.5);
    double sp = sin(pitch * 0.5);
    double cy = cos(yaw * 0.5);
    double sy = sin(yaw * 0.5);


    double w = cr * cp * cy + sr * sp * sy;
    double x = sr * cp * cy - cr * sp * sy;
    double y = cr * sp * cy + sr * cp * sy;
    double z = cr * cp * sy - sr * sp * cy;

    Quaternion<double> q(x, y, z, w);

    return q;
}

juce::Vector3D<double> MathHelper::QuaternionToEuler(const juce::Quaternion<double>& q)
{
    // Convert quaternion to rotation matrix
    auto rotationMatrix = q.getRotationMatrix();
    
    // Extract the 3x3 rotation matrix from the 4x4 matrix
    // mat[16] is in column-major order:
    // [0] [4] [8]  [12]
    // [1] [5] [9]  [13]
    // [2] [6] [10] [14]
    // [3] [7] [11] [15]
    
    const auto& m = rotationMatrix.mat;
    
    // Extract the 3x3 rotation part (upper left)
    double m00 = m[0], m01 = m[4], m02 = m[8];   // First column
    double m10 = m[1], m11 = m[5], m12 = m[9];   // Second column
    double /*m 20 = m[2], m21 = m[6], */ m22 = m[10];  // Third column
    
    double roll, pitch, yaw;
    
    // Extract Euler angles from rotation matrix (XYZ convention)
    if (m02 < 1.0) {
        if (m02 > -1.0) {
            pitch = std::asin(-m02);
            roll = std::atan2(m12, m22);
            yaw = std::atan2(m01, m00);
        } else { // m02 = -1
            pitch = MathConstants<double>::pi / 2.0;
            roll = std::atan2(-m10, m11);
            yaw = 0.0;
        }
    } else { // m02 = 1
        pitch = -MathConstants<double>::pi / 2.0;
        roll = std::atan2(-m10, m11);
        yaw = 0.0;
    }
    
    return juce::Vector3D<double>(roll, pitch, yaw);
}
