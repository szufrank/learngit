#include "Camera.h"

Camera::Camera() { updateCamera(); };
Camera::~Camera() {};

glm::mat4 Camera::lookAt(const glm::vec4 &eye, const glm::vec4 &at, const glm::vec4 &up)
{
	// 定义三个正交基向量
	glm::vec3 n = glm::normalize(glm::vec3(eye - at));			// 前向量
	glm::vec3 u = glm::normalize(glm::cross(glm::vec3(up), n)); // 右向量
	glm::vec3 v = glm::cross(n, u);								// 上向量

	// 构造旋转矩阵
	glm::mat4 R(1.0f);
	R[0][0] = u.x;
	R[1][0] = u.y;
	R[2][0] = u.z;
	R[0][1] = v.x;
	R[1][1] = v.y;
	R[2][1] = v.z;
	R[0][2] = n.x;
	R[1][2] = n.y;
	R[2][2] = n.z;

	// 构造平移矩阵
	glm::mat4 T = glm::translate(glm::mat4(1.0f), -glm::vec3(eye));

	// 相机观察矩阵 = R * T
	return R * T;
}

glm::mat4 Camera::ortho(const GLfloat left, const GLfloat right,
						const GLfloat bottom, const GLfloat top,
						const GLfloat zNear, const GLfloat zFar)
{
	// @TODO: Task2:请按照实验课内容补全正交投影矩阵的计算
	glm::mat4 c = glm::mat4(1.0f);

	c[0][0] = 2.0f / (right - left);
	c[1][1] = 2.0f / (top - bottom);
	c[2][2] = -2.0f / (zFar - zNear);

	c[3][0] = -(right + left) / (right - left);
	c[3][1] = -(top + bottom) / (top - bottom);
	c[3][2] = -(zFar + zNear) / (zFar - zNear);

	return c;
}

glm::mat4 Camera::perspective(const GLfloat fov, const GLfloat aspect,
							  const GLfloat zNear, const GLfloat zFar)
{
	// @TODO: Task3:请按照实验课内容补全透视投影矩阵的计算
	glm::mat4 c = glm::mat4(1.0f);
	float f = 1.0f / tan(glm::radians(fov / 2.0f));

	c[0][0] = f / aspect;
	c[1][1] = f;
	c[2][2] = (zFar + zNear) / (zNear - zFar);
	c[2][3] = -1.0f;
	c[3][2] = (2.0f * zFar * zNear) / (zNear - zFar);
	return c;
}

glm::mat4 Camera::frustum(const GLfloat left, const GLfloat right,
						  const GLfloat bottom, const GLfloat top,
						  const GLfloat zNear, const GLfloat zFar)
{
	// 任意视锥体矩阵
	glm::mat4 c = glm::mat4(1.0f);
	c[0][0] = 2.0 * zNear / (right - left);
	c[0][2] = (right + left) / (right - left);
	c[1][1] = 2.0 * zNear / (top - bottom);
	c[1][2] = (top + bottom) / (top - bottom);
	c[2][2] = -(zFar + zNear) / (zFar - zNear);
	c[2][3] = -2.0 * zFar * zNear / (zFar - zNear);
	c[3][2] = -1.0;
	c[3][3] = 0.0;
	c = glm::transpose(c);
	return c;
}

void Camera::updateCamera()

{
	// @TODO: Task1 计算相机位置(eyex, eyey, eyez)
	float radRotate = glm::radians(rotateAngle);
	float radUp = glm::radians(upAngle);

	float eyex = radius * sin(radRotate) * cos(radUp);
	float eyey = radius * sin(radUp);
	float eyez = radius * cos(radRotate) * cos(radUp);

	eye = glm::vec4(eyex, eyey, eyez, 1.0f);

	at = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	up = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
	if (upAngle > 90)
	{
		up.y = -1;
	}
	else if (upAngle < -90)
	{
		up.y = -1;
	}
}

void Camera::keyboard(int key, int action, int mode)
{

	// 键盘事件处理
	if (key == GLFW_KEY_X && action == GLFW_PRESS && mode == 0x0000)
	{
		rotateAngle += 5.0;
	}
	else if (key == GLFW_KEY_X && action == GLFW_PRESS && mode == GLFW_MOD_SHIFT)
	{
		rotateAngle -= 5.0;
	}
	else if (key == GLFW_KEY_Y && action == GLFW_PRESS && mode == 0x0000)
	{
		upAngle += 5.0;
		if (upAngle > 180)
			upAngle = 180;
	}
	else if (key == GLFW_KEY_Y && action == GLFW_PRESS && mode == GLFW_MOD_SHIFT)
	{
		upAngle -= 5.0;
		if (upAngle < -180)
			upAngle = -180;
	}
	else if (key == GLFW_KEY_R && action == GLFW_PRESS && mode == 0x0000)
	{
		radius += 0.1;
	}
	else if (key == GLFW_KEY_R && action == GLFW_PRESS && mode == GLFW_MOD_SHIFT)
	{
		radius -= 0.1;
	}
	else if (key == GLFW_KEY_F && action == GLFW_PRESS && mode == 0x0000)
	{
		fov += 5.0;
	}
	else if (key == GLFW_KEY_F && action == GLFW_PRESS && mode == GLFW_MOD_SHIFT)
	{
		fov -= 5.0;
	}
	else if (key == GLFW_KEY_A && action == GLFW_PRESS && mode == 0x0000)
	{
		aspect += 0.1;
	}
	else if (key == GLFW_KEY_A && action == GLFW_PRESS && mode == GLFW_MOD_SHIFT)
	{
		aspect -= 0.1;
	}
	else if (key == GLFW_KEY_S && action == GLFW_PRESS && mode == 0x0000)
	{
		scale += 0.1;
	}
	else if (key == GLFW_KEY_S && action == GLFW_PRESS && mode == GLFW_MOD_SHIFT)
	{
		scale -= 0.1;
	}
	else if (key == GLFW_KEY_SPACE && action == GLFW_PRESS && mode == 0x0000)
	{
		radius = 4.0;
		rotateAngle = 0.0;
		upAngle = 0.0;
		fov = 45.0;
		aspect = 1.0;
		scale = 1.5;
	}
}
