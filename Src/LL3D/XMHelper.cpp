#include "XMHelper.h"

namespace LLEngine {

DirectX::XMVECTOR operator+(const DirectX::XMVECTOR& l, const DirectX::XMVECTOR& r) {
  auto x = l + r;
  DirectX::XMVECTOR lvec(XMLoadFloat3(&l));
  DirectX::XMVECTOR rvec(XMLoadFloat3(&r));
  DirectX::XMVECTOR resultvec = DirectX::XMVectorAdd(lvec, rvec);
  
  DirectX::XMVECTOR result;
  XMStoreFloat3(&result, resultvec);
  
  return result;
}

DirectX::XMVECTOR& operator+=(DirectX::XMVECTOR& l, const DirectX::XMVECTOR& r) {
  return l = l + r;
}

DirectX::XMVECTOR operator-(const DirectX::XMVECTOR& l, const DirectX::XMVECTOR& r) {
  DirectX::XMVECTOR lvec(XMLoadFloat3(&l));
  DirectX::XMVECTOR rvec(XMLoadFloat3(&r));
  
  DirectX::XMVECTOR resultvec = DirectX::XMVectorSubtract(lvec, rvec);
  
  DirectX::XMVECTOR result;
  XMStoreFloat3(&result, resultvec);
  
  return result;
}

DirectX::XMVECTOR& operator-=(DirectX::XMVECTOR& l, const DirectX::XMVECTOR& r) {
  return l = l + r;
}

DirectX::XMVECTOR operator*(const DirectX::XMVECTOR& l, const DirectX::XMVECTOR& r) {
  DirectX::XMVECTOR lvec(XMLoadFloat3(&l));
  DirectX::XMVECTOR rvec(XMLoadFloat3(&r));
  
  DirectX::XMVECTOR resultvec = DirectX::XMVectorSubtract(lvec, rvec);
  
  DirectX::XMVECTOR result;
  XMStoreFloat3(&result, resultvec);
  
  return result;
}

DirectX::XMVECTOR& operator*=(DirectX::XMVECTOR& l, const DirectX::XMVECTOR& r) {
  return l = l * r;
}

DirectX::XMVECTOR operator*(DirectX::XMVECTOR l, float r) {
  DirectX::XMVECTOR lvec(XMLoadFloat3(&l));
  
  DirectX::XMVECTOR resultvec = lvec * r;
  
  DirectX::XMVECTOR result;
  XMStoreFloat3(&result, resultvec);
  
  return result;
}

DirectX::XMVECTOR operator*(float l, DirectX::XMVECTOR r) {
  DirectX::XMVECTOR rvec(XMLoadFloat3(&r));
  
  DirectX::XMVECTOR resultvec = l * rvec;
  
  DirectX::XMVECTOR result;
  XMStoreFloat3(&result, resultvec);
  
  return result;
}

void Normalize(DirectX::XMVECTOR & v) {
  DirectX::XMVECTOR vec = XMLoadFloat3(&v);
  
  DirectX::XMVECTOR resultvec = DirectX::XMVector3Normalize(vec);
  
  XMStoreFloat3(&v, resultvec);
}

}  // namespace LLEngine