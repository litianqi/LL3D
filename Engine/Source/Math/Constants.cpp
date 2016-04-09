#include "Math/Vector.h"
#include "Math/Color.h"
#include "Math/Matrix.h"
#include "Math/Quaternion.h"

/****************************************************************************
 *
 * Constants
 *
 ****************************************************************************/

namespace LL3D
{
    namespace Math
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1800)
        const Vector2 Vector2::Zero(0.f, 0.f);
        const Vector2 Vector2::One(1.f, 1.f);
        const Vector2 Vector2::UnitX(1.f, 0.f);
        const Vector2 Vector2::UnitY(0.f, 1.f);

        const Vector3 Vector3::Zero(0.f, 0.f, 0.f);
        const Vector3 Vector3::One(1.f, 1.f, 1.f);
        const Vector3 Vector3::UnitX(1.f, 0.f, 0.f);
        const Vector3 Vector3::UnitY(0.f, 1.f, 0.f);
        const Vector3 Vector3::UnitZ(0.f, 0.f, 1.f);
        const Vector3 Vector3::Up(0.f, 1.f, 0.f);
        const Vector3 Vector3::Down(0.f, -1.f, 0.f);
        const Vector3 Vector3::Right(1.f, 0.f, 0.f);
        const Vector3 Vector3::Left(-1.f, 0.f, 0.f);
        const Vector3 Vector3::Forward(0.f, 0.f, 1.f);
        const Vector3 Vector3::Backward(0.f, 0.f, -1.f);

        const Vector4 Vector4::Zero(0.f, 0.f, 0.f, 0.f);
        const Vector4 Vector4::One(1.f, 1.f, 1.f, 1.f);
        const Vector4 Vector4::UnitX(1.f, 0.f, 0.f, 0.f);
        const Vector4 Vector4::UnitY(0.f, 1.f, 0.f, 0.f);
        const Vector4 Vector4::UnitZ(0.f, 0.f, 1.f, 0.f);
        const Vector4 Vector4::UnitW(0.f, 0.f, 0.f, 1.f);

        const Matrix Matrix::Identity(1.f, 0.f, 0.f, 0.f,
                                      0.f, 1.f, 0.f, 0.f,
                                      0.f, 0.f, 1.f, 0.f,
                                      0.f, 0.f, 0.f, 1.f);

        const Quaternion Quaternion::Identity(0.f, 0.f, 0.f, 1.f);
    #else
    const Vector2 Vector2::Zero = { 0.f, 0.f };
    const Vector2 Vector2::One = { 1.f, 1.f };
    const Vector2 Vector2::UnitX = { 1.f, 0.f };
    const Vector2 Vector2::UnitY = { 0.f, 1.f };

    const Vector3 Vector3::Zero = { 0.f, 0.f, 0.f };
    const Vector3 Vector3::One = { 1.f, 1.f, 1.f };
    const Vector3 Vector3::UnitX = { 1.f, 0.f, 0.f };
    const Vector3 Vector3::UnitY = { 0.f, 1.f, 0.f };
    const Vector3 Vector3::UnitZ = { 0.f, 0.f, 1.f };
    const Vector3 Vector3::Up = { 0.f, 1.f, 0.f };
    const Vector3 Vector3::Down = { 0.f, -1.f, 0.f };
    const Vector3 Vector3::Right = { 1.f, 0.f, 0.f };
    const Vector3 Vector3::Left = { -1.f, 0.f, 0.f };
    const Vector3 Vector3::Forward = { 0.f, 0.f, 1.f };
    const Vector3 Vector3::Backward = { 0.f, 0.f, -1.f };

    const Vector4 Vector4::Zero = { 0.f, 0.f, 0.f, 0.f };
    const Vector4 Vector4::One = { 1.f, 1.f, 1.f, 1.f };
    const Vector4 Vector4::UnitX = { 1.f, 0.f, 0.f, 0.f };
    const Vector4 Vector4::UnitY = { 0.f, 1.f, 0.f, 0.f };
    const Vector4 Vector4::UnitZ = { 0.f, 0.f, 1.f, 0.f };
    const Vector4 Vector4::UnitW = { 0.f, 0.f, 0.f, 1.f };

        const Matrix Matrix::Identity = { 1.f, 0.f, 0.f, 0.f,
                                          0.f, 1.f, 0.f, 0.f,
                                          0.f, 0.f, 1.f, 0.f,
                                          0.f, 0.f, 0.f, 1.f };

        const Quaternion Quaternion::Identity = { 0.f, 0.f, 0.f, 1.f };

        const Color Color::White = { 1.0f, 1.0f, 1.0f, 1.0f };
        const Color Color::Black = { 0.0f, 0.0f, 0.0f, 1.0f };
        const Color Color::Red = { 1.0f, 0.0f, 0.0f, 1.0f };
        const Color Color::Green = { 0.0f, 1.0f, 0.0f, 1.0f };
        const Color Color::Blue = { 0.0f, 0.0f, 1.0f, 1.0f };
        const Color Color::Yellow = { 1.0f, 1.0f, 0.0f, 1.0f };
        const Color Color::Cyan = { 0.0f, 1.0f, 1.0f, 1.0f };
        const Color Color::Magenta = { 1.0f, 0.0f, 1.0f, 1.0f };
        const Color Color::Silver = { 0.75f, 0.75f, 0.75f, 1.0f };
        const Color Color::LightSteelBlue = { 0.69f, 0.77f, 0.87f, 1.0f };
        const Color Color::Grey = { 0.294f, 0.294f, 0.294f, 1.0f };
    #endif
    }
}



