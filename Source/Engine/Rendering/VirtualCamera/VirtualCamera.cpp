#include "VirtualCamera.h"

using namespace EngineAPI::Rendering;

VirtualCamera::VirtualCamera()
{
	//Init position and the right, up, look vectors. 
	position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	right = XMFLOAT3(1.0f, 0.0f, 0.0f);
	up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	look = XMFLOAT3(0.0f, 0.0f, -1.0f);

	//Init projection matrix. 
	nearZ = 1.0f;
	farZ = 1000.0f;
	aspect = 1.0f;
	fovYRads = 0.25f* DirectX::XM_PI;
	InitCameraPerspectiveProjectionProperties(fovYRads, aspect, nearZ, farZ);
}

void VirtualCamera::Shutdown()
{
	//Super
	__super::Shutdown();
}

void VirtualCamera::InitCameraViewProperties(const XMFLOAT3& cameraPosition, const XMFLOAT3& cameraTarget, const XMFLOAT3& worldUp)
{
	XMVECTOR pos = XMLoadFloat3(&cameraPosition);
	XMVECTOR target =XMLoadFloat3(&cameraTarget);
	XMVECTOR worldUpV = XMLoadFloat3(&up);

	XMVECTOR l = XMVector3Normalize(XMVectorSubtract(target, pos));
	XMVECTOR r = XMVector3Normalize(XMVector3Cross(worldUpV, l));
	XMVECTOR u = XMVector3Cross(l, r);

	XMStoreFloat3(&position, pos);
	XMStoreFloat3(&look, l);
	XMStoreFloat3(&right, r);
	XMStoreFloat3(&up, u);
}

void VirtualCamera::InitCameraPerspectiveProjectionProperties(float fovYDegrees, float aspect, float nearZ, float farZ)
{
	XMStoreFloat4x4(&proj, DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(fovYDegrees), aspect, nearZ, farZ));

	//Cache data
	this->fovYRads = DirectX::XMConvertToRadians(fovYDegrees);
	this->aspect = aspect;
	this->nearZ = nearZ;
	this->farZ = farZ;

	isPerspectiveMatrix = true;
}

void VirtualCamera::Strafe(float distance)
{
	XMVECTOR s = XMVectorReplicate(distance);
	XMVECTOR r = XMLoadFloat3(&right);
	XMVECTOR p = XMLoadFloat3(&position);

	XMStoreFloat3(&position, XMVectorMultiplyAdd(s, r, p));
}

void VirtualCamera::Walk(float distance)
{
	XMVECTOR s = XMVectorReplicate(distance);
	XMVECTOR l = XMLoadFloat3(&look);
	XMVECTOR p = XMLoadFloat3(&position);

	XMStoreFloat3(&position, XMVectorMultiplyAdd(s, l, p));
}

void VirtualCamera::AddPitch(float angleXDegrees)
{
	//Rotate up and look vector about the right vector.
	XMMATRIX r = XMMatrixRotationAxis(XMLoadFloat3(&right), XMConvertToRadians(angleXDegrees));

	XMStoreFloat3(&up, XMVector3TransformNormal(XMLoadFloat3(&up), r));
	XMStoreFloat3(&look, XMVector3TransformNormal(XMLoadFloat3(&look), r));
}

void VirtualCamera::AddYaw(float angleYDegrees)
{
	//Rotate the basis vector about the world axis (y)
	XMMATRIX r = XMMatrixRotationY(XMConvertToRadians(angleYDegrees));

	XMStoreFloat3(&right, XMVector3TransformNormal(XMLoadFloat3(&right), r));
	XMStoreFloat3(&up, XMVector3TransformNormal(XMLoadFloat3(&up), r));
	XMStoreFloat3(&look, XMVector3TransformNormal(XMLoadFloat3(&look), r));
}

void VirtualCamera::RebuildView()
{
	XMVECTOR r = XMLoadFloat3(&right);
	XMVECTOR u = XMLoadFloat3(&up);
	XMVECTOR l = XMLoadFloat3(&look);
	XMVECTOR p = XMLoadFloat3(&position);

	//Keep cameras axis orthogonal to each other and normalized
	l = XMVector3Normalize(l);
	u = XMVector3Normalize(XMVector3Cross(l, r));

	//update right vector - return will already be normalized because u and l
	//have been above.
	r = XMVector3Cross(u, l);

	//Fill in view matrix entries.
	float x = -XMVectorGetX(XMVector3Dot(p, r));
	float y = -XMVectorGetX(XMVector3Dot(p, u));
	float z = -XMVectorGetX(XMVector3Dot(p, l));

	XMStoreFloat3(&right, r);
	XMStoreFloat3(&up, u);
	XMStoreFloat3(&look, l);

	view(0, 0) = right.x;
	view(1, 0) = right.y;
	view(2, 0) = right.z;
	view(3, 0) = x;

	view(0, 1) = up.x;
	view(1, 1) = up.y;
	view(2, 1) = up.z;
	view(3, 1) = y;

	view(0, 2) = look.x;
	view(1, 2) = look.y;
	view(2, 2) = look.z;
	view(3, 2) = z;

	view(0, 3) = 0.0f;
	view(1, 3) = 0.0f;
	view(2, 3) = 0.0f;
	view(3, 3) = 1.0f;
}

XMMATRIX VirtualCamera::CalculateInverseViewMatrix()
{
	XMMATRIX viewXM = XMLoadFloat4x4(&view);

	XMVECTOR det = XMMatrixDeterminant(viewXM);
	XMMATRIX invView = XMMatrixInverse(&det, viewXM);

	return invView;
}