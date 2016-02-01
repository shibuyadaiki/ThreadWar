#include "Matrix4.h"
#include "MathUtility.h"
#include "MatrixUtility.h"

//単位行列
const Matrix4& RCMatrix4::Identity(){
	static const Matrix4 result = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
	return result;
}

//拡大縮小行列
Matrix4 RCMatrix4::scale(const Vector3& s){
	Matrix4 result = {
		s.x, 0.0f, 0.0f, 0.0f,
		0.0f, s.y, 0.0f, 0.0f,
		0.0f, 0.0f, s.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
	return result;
}

//回転行列
Matrix4 RCMatrix4::rotateX(float angle){
	float sin = Math::sin(angle);
	float cos = Math::cos(angle);
	Matrix4 result = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, cos, sin, 0.0f,
		0.0f, -sin, cos, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
	return result;
}
Matrix4 RCMatrix4::rotateY(float angle){
	float sin = Math::sin(angle);
	float cos = Math::cos(angle);
	Matrix4 result = {
		cos, 0.0f, -sin, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		sin, 0.0f, cos, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
	return result;
}

Matrix4 RCMatrix4::rotateZ(float angle){
	float sin = Math::sin(angle);
	float cos = Math::cos(angle);
	Matrix4 result = {
		cos, sin, 0.0f, 0.0f,
		-sin, cos, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
	return result;
}

//平行移動行列
Matrix4 RCMatrix4::translate(const Vector3& t){
	Matrix4 result = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		t.x, t.y, t.z, 1.0f
	};
	return result;
}

//マトリックスを求める
Matrix4 RCMatrix4::matrix(const Vector3& s, const float& rotateXangle, const float& rotateYangle, const float& rotateZangle, const Vector3& pos){
	Matrix4 result = scale(s) * rotateZ(rotateZangle) * rotateX(rotateXangle) * rotateY(rotateYangle) * translate(pos);

	return result;
}

//マトリックスを求める
Matrix4 RCMatrix4::matrix(const Matrix4& scale_, const Matrix4& rotateX_, const Matrix4& rotateY_, const Matrix4& rotateZ_, const Matrix4& translate_){
	Matrix4 result = scale_ * rotateZ_ * rotateX_ * rotateY_ * translate_;

	return result;
}

//座標変換(ベクトルと行列の積)
Vector3 RCMatrix4::transform(const Vector3& v, const Matrix4& m){
	float w = v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + m.m[3][3];
	Vector3 result = {
		(v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + m.m[3][0]) / w,
		(v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + m.m[3][1]) / w,
		(v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + m.m[3][2]) / w
	};
	return result;
}

//座標変換(回転のみ)
Vector3 RCMatrix4::transformRotateOnly(const Vector3& v, const Matrix4& m){
	Vector3 result = {
		v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0],
		v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1],
		v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2]
	};
	return result;
}

//転置行列
Matrix4 RCMatrix4::transpose(const Matrix4& m){
	Matrix4 result;
	for (int i = 0; i < 4; ++i){
		for (int j = 0; j < 4; ++j){
			result.m[i][j] = m.m[j][i];
		}
	}
	return result;
}

//転置行列
Matrix4 RCMatrix4::transposeRotateOnly(const Matrix4& m){
	Matrix4 result = m;
	for (int i = 0; i < 3; ++i){
		for (int j = 0; j < 3; ++j){
			result.m[i][j] = m.m[j][i];
		}
	}
	return result;
}

//逆行列(拡縮除く)
Matrix4 RCMatrix4::inverseFast(const Matrix4& m){
	// 回転行列の作成
	Matrix4 result = m;
	setPosition(result, vector3(0, 0, 0));
	// 回転行列の逆行列を作成
	result = transpose(result);
	// 平行移動成分の逆変換
	Vector3 invPosition = -getPosition(m) * result;
	// 逆行列を作成
	setPosition(result, invPosition);
	return result;
}

//逆行列
Matrix4 RCMatrix4::inverse(const Matrix4& m){
	float a0 = m.m[0][0] * m.m[1][1] - m.m[0][1] * m.m[1][0];
	float a1 = m.m[0][0] * m.m[1][2] - m.m[0][2] * m.m[1][0];
	float a2 = m.m[0][0] * m.m[1][3] - m.m[0][3] * m.m[1][0];
	float a3 = m.m[0][1] * m.m[1][2] - m.m[0][2] * m.m[1][1];
	float a4 = m.m[0][1] * m.m[1][3] - m.m[0][3] * m.m[1][1];
	float a5 = m.m[0][2] * m.m[1][3] - m.m[0][3] * m.m[1][2];
	float b0 = m.m[2][0] * m.m[3][1] - m.m[2][1] * m.m[3][0];
	float b1 = m.m[2][0] * m.m[3][2] - m.m[2][2] * m.m[3][0];
	float b2 = m.m[2][0] * m.m[3][3] - m.m[2][3] * m.m[3][0];
	float b3 = m.m[2][1] * m.m[3][2] - m.m[2][2] * m.m[3][1];
	float b4 = m.m[2][1] * m.m[3][3] - m.m[2][3] * m.m[3][1];
	float b5 = m.m[2][2] * m.m[3][3] - m.m[2][3] * m.m[3][2];
	float det = a0*b5 - a1*b4 + a2*b3 + a3*b2 - a4*b1 + a5*b0;
	if (det == 0.0f) {
		return m; // 逆行列が存在しない！
	}
	float invDet = 1.0f / det;
	Matrix4 result = {
		(m.m[1][1] * b5 - m.m[1][2] * b4 + m.m[1][3] * b3) * invDet,
		(-m.m[0][1] * b5 + m.m[0][2] * b4 - m.m[0][3] * b3) * invDet,
		(m.m[3][1] * a5 - m.m[3][2] * a4 + m.m[3][3] * a3) * invDet,
		(-m.m[2][1] * a5 + m.m[2][2] * a4 - m.m[2][3] * a3) * invDet,
		(-m.m[1][0] * b5 + m.m[1][2] * b2 - m.m[1][3] * b1) * invDet,
		(m.m[0][0] * b5 - m.m[0][2] * b2 + m.m[0][3] * b1) * invDet,
		(-m.m[3][0] * a5 + m.m[3][2] * a2 - m.m[3][3] * a1) * invDet,
		(m.m[2][0] * a5 - m.m[2][2] * a2 + m.m[2][3] * a1) * invDet,
		(m.m[1][0] * b4 - m.m[1][1] * b2 + m.m[1][3] * b0) * invDet,
		(-m.m[0][0] * b4 + m.m[0][1] * b2 - m.m[0][3] * b0) * invDet,
		(m.m[3][0] * a4 - m.m[3][1] * a2 + m.m[3][3] * a0) * invDet,
		(-m.m[2][0] * a4 + m.m[2][1] * a2 - m.m[2][3] * a0) * invDet,
		(-m.m[1][0] * b3 + m.m[1][1] * b1 - m.m[1][2] * b0) * invDet,
		(m.m[0][0] * b3 - m.m[0][1] * b1 + m.m[0][2] * b0) * invDet,
		(-m.m[3][0] * a3 + m.m[3][1] * a1 - m.m[3][2] * a0) * invDet,
		(m.m[2][0] * a3 - m.m[2][1] * a1 + m.m[2][2] * a0) * invDet
	};
	return result;
}


//視野変換行列
Matrix4 lookAt(const Vector3& eye, const Vector3& at, const Vector3& up){
	Vector3 z = RCVector3::normalize(eye - at);
	Vector3 x = RCVector3::normalize(RCVector3::cross(up, z));
	Vector3 y = RCVector3::cross(z, x);
	Matrix4 result = {
		x.x, y.x, z.x, 0.0f,
		x.y, y.y, z.y, 0.0f,
		x.z, y.z, z.z, 0.0f,
		-RCVector3::dot(eye, x), -RCVector3::dot(eye, y), -RCVector3::dot(eye, z), 1.0f
	};
	return result;
}

//透視変換行列を作成する関数
Matrix4 perspective(float fov, float aspect, float zn, float zf){
	float ys = 1.0f / Math::tan(fov / 2.0f);
	float xs = ys / aspect;
	Matrix4 result = {
		xs, 0.0f, 0.0f, 0.0f,
		0.0f, ys, 0.0f, 0.0f,
		0.0f, 0.0f, (zf + zn) / (zn - zf), -1.0f,
		0.0f, 0.0f, (2.0f*zf*zn) / (zn - zf), 0.0f
	};
	return result;
}

//スクリーン座標変換行列を作成する関数
Matrix4 screen(float x, float y, float width, float height){
	float w = width / 2.0f;
	float h = height / 2.0f;
	Matrix4 result = {
		w, 0.0f, 0.0f, 0.0f,
		0.0f, -h, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		w + x, h + y, 0.0f, 1.0f
	};
	return result;
}

//代入演算子オーバーロード
Matrix4& operator *= (Matrix4& m1, const Matrix4& m2){
	Matrix4 result = { 0 };
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			for (int k = 0; k < 4; k++) {
				result.m[i][j] += m1.m[i][k] * m2.m[k][j];
			}
		}
	}
	m1 = result;
	return m1;
}

//2項演算子オーバーロード
Matrix4 operator * (const Matrix4& m1, const Matrix4& m2){
	Matrix4 result = m1;
	return result *= m2;
}
Vector3 operator * (const Vector3& v, const Matrix4& m){
	return RCMatrix4::transform(v, m);
}