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
      Assert::AreEqual(1, 1);
		}

    TEST_METHOD(Fail)
    {
      Assert::Fail(L"Failed");
    }

  private:
    
	};
}