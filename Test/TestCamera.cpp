#include <stdafx.h>
#include <CppUnitTest.h>
#include <Graphics/Camera.h>
#include <Math/Math.h>
#include <GameObject.h>
#include <Transform.h>

using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace LL3D;

namespace Test
{		
	TEST_CLASS(Camera)
	{
	public:
		
		TEST_METHOD(Direction)
		{
      auto object = ConstructAGameObject();
      Assert::AreEqual(1, 1);
		}

    TEST_METHOD(Fail)
    {
      Assert::Fail(L"Failed");
    }

  private:
    static GameObject ConstructAGameObject() 
    {
      auto camera = make_unique<Graphics::Camera>(Graphics::Camera::Frustum(
        Math::XM_PI / 8.f,
        0.5f,
        1,
        1000),
        Math::Vector3(0.f, -100.f, 100.f)
        );

      auto object =GameObject();
      
      object.transform().setPosition(Math::Vector3(0.f, 100.f, -100.f));
      object.addComponent(std::move(camera));

      return object;
    }
	};
}