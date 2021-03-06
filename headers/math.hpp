//#include <math.h>

struct colr
{   
    float r = 1; 
    float g = 1;
    float b = 1; 
    float a = 1; 
};

//*******
//VECTORS
//*******

struct vect 
{
    float x = 0; 
    float y = 0; 
    float z = 0;
    float w = 1;
};

struct vect2
{
    float x = 0; 
    float y = 0; 
};

//Vector Multiplication with Constant
colr vector_colour_multiply(vect vector, colr colour)
{
    return {vector.x * colour.r, vector.y * colour.g, vector.z * colour.b, 1};
};

//Vector Adding 
vect vector_add(vect vector1, vect vector2)
{
    return {vector1.x + vector2.x, vector1.y + vector2.y, vector1.z + vector2.z, 1};
};

//Vector Subtracting 
vect vector_subtract(vect vector1, vect vector2)
{
    return {vector1.x - vector2.x, vector1.y - vector2.y, vector1.z - vector2.z, 1};
};

//Vector Multiplication with Constant
vect vector_multiply(vect vector, float constant)
{
    return {vector.x * constant, vector.y * constant, vector.z * constant, 1};
};

//Vector divided with constant
vect vector_divide(vect vector, float constant)
{
    return {vector.x / constant, vector.y / constant, vector.z / constant, 1};
};

//Dot Product 
float vector_dot_product(vect vector1, vect vector2)
{
    return {vector1.x * vector2.x + vector1.y * vector2.y + vector1.z * vector2.z}; 
};

//Cross Product
vect vector_cross_product(vect vector1, vect vector2)
{   
    return {vector1.y * vector2.z - vector1.z * vector2.y, vector1.z * vector2.x - vector1.x * vector2.z, vector1.x * vector2.y - vector1.y * vector2.x, 1};
};

//Magnitude of a vector
float vector_magnitude(vect vector)
{
	return pow(pow(vector.x, 2) + pow(vector.y, 2) + pow(vector.z, 2), 0.5);
};

//Normalization
vect vector_normalize(vect vector)
{  
    return vector_divide(vector, vector_magnitude(vector));
};

//Vector 1 is reflected around Vector 2
vect vector_reflect(vect vector1, vect vector2)
{
	vector_normalize(vector2); 
	return vector_subtract(vector1, vector_multiply(vector2, vector_dot_product(vector1, vector2) * 2.0f));
};

//***********
//QUATERNIONS
//***********
struct quat 
{
    float x = 0; 
    float y = 0; 
    float z = 0;
    float w = 1;
};

//Multiplying two quaternions using Hamilton Product  
quat quaternion_divide(quat quaternion, float constant)
{   
    return {quaternion.x / constant, quaternion.y / constant, quaternion.z / constant, 1};
};

//Multiplying two quaternions using Hamilton Product  
quat quaternion_multiply(quat quaternion1, quat quaternion2)
{   
    quat result;

    result.x = (quaternion1.w*quaternion2.x + quaternion1.x*quaternion2.w + quaternion1.y*quaternion2.z - quaternion1.z*quaternion2.y);
    result.y = (quaternion1.w*quaternion2.y - quaternion1.x*quaternion2.z + quaternion1.y*quaternion2.w + quaternion1.z*quaternion2.x);
    result.z = (quaternion1.w*quaternion2.z + quaternion1.x*quaternion2.y - quaternion1.y*quaternion2.x + quaternion1.z*quaternion2.w);
    result.w = (quaternion1.w*quaternion2.w - quaternion1.x*quaternion2.x - quaternion1.y*quaternion2.y - quaternion1.z*quaternion2.z);

    return result;
};

//Normalization
void quaternion_normalize(quat vector)
{   
    float length = pow(pow(vector.x, 2) + pow(vector.y, 2) + pow(vector.z, 2) + pow(vector.w, 2), 0.5);

    vector = quaternion_divide(vector, length);
};

//The basic quaternion structure 
quat quaternion_structure(vect axis, float angle)
{   
    quat quaternion;

    quaternion.x = axis.x * sinf( angle/2 );
    quaternion.y = axis.y * sinf( angle/2 );
    quaternion.z = axis.z * sinf( angle/2 );
    quaternion.w = cosf( angle/2 );

    quaternion_normalize(quaternion);

    return quaternion;
};

//Seting up the new global rotation based on input axis, change in angles and total quaternion
quat quaternion_setup(quat total_quaternion, vect angle, vect x_axis, vect y_axis, vect z_axis)
{   
	//Set-up the rotation quaternions 
    quat quaternion_x = quaternion_structure(x_axis, angle.x);
    quat quaternion_y = quaternion_structure(y_axis, angle.y);
    quat quaternion_z = quaternion_structure(z_axis, angle.z);

    //Multiplying change in quaternion by universal quaternion then rotating point
    quat quaternion = quaternion_multiply(quaternion_multiply(quaternion_multiply(quaternion_x, quaternion_y), quaternion_z), total_quaternion);

    return quaternion;
};

//Provides the conjugate of a quaternion
quat quaternion_conjugatation(quat quaternion)
{
    return {-quaternion.x, -quaternion.y, -quaternion.z, quaternion.w};
};

//Converting a Vector into a quaternion to match Data Types 
quat quaternion_from_vector(vect vector)
{
    return{vector.x, vector.y, vector.z, vector.w};
};

//Converting a Vector into a quaternion to match Data Types 
vect vector_from_quaternion(quat quaternion)
{
    return{quaternion.x, quaternion.y, quaternion.z, quaternion.w};
};

//Rotates a point or vector based on: R = P*Q*P^-1
vect quaternion_rotation(quat quaternion, vect position)
{   
    // quat conjugate = quaternion_conjugatation(quaternion);
    // return(vector_from_quaternion(quaternion_multiply(quaternion_multiply(quaternion, quaternion_from_vector(position)), conjugate)));

	//Perphaps more efficient to use: return v' = v + 2.0 * cross(cross(v, q.xyz) + q.w * v, q.xyz);
	// vect quat_vect = vector_from_quaternion(quaternion); 
	// return vector_add(position, vector_multiply(vector_cross_product(vector_add(vector_cross_product(position, quat_vect), vector_multiply(position, quaternion.w)), quat_vect), 2));

	//Maybe More Efficient
	float x2 = quaternion.x + quaternion.x;
    float y2 = quaternion.y + quaternion.y;
    float z2 = quaternion.z + quaternion.z;
    float wx2 = quaternion.w * x2;
    float wy2 = quaternion.w * y2;
    float wz2 = quaternion.w * z2;
    float xx2 = quaternion.x * x2;
    float xy2 = quaternion.x * y2;
    float xz2 = quaternion.x * z2;
    float yy2 = quaternion.y * y2;
    float yz2 = quaternion.y * z2;
    float zz2 = quaternion.z * z2;
    float x = position.x * (1.0 - yy2 - zz2) + position.y * (xy2 - wz2) + position.z * (xz2 + wy2);
    float y = position.x * (xy2 + wz2) + position.y * (1.0 - xx2 - zz2) + position.z * (yz2 - wx2);
    float z = position.x * (xz2 - wy2) + position.y * (yz2 + wx2) + position.z * (1.0 - xx2 - yy2);

    return {x, y, z, 1};
}; 

// Angles between Vectors
vect quaternion_to_euler(quat quaternion)
{		
	vect euler;

	// if the input quaternion is normalized, this is exactly one. Otherwise, this acts as a correction factor for the quaternion's not-normalizedness
	float unit = (quaternion.x * quaternion.x) + (quaternion.y * quaternion.y) + (quaternion.z * quaternion.z) + (quaternion.w * quaternion.w);

	// this will have a magnitude of 0.5 or greater if and only if this is a singularity case
	float test = quaternion.x * quaternion.w - quaternion.y * quaternion.z;

	if (test > 0.4995f * unit) // singularity at north pole
	{
		euler.x = 3.141593 / 2;
		euler.y = 2.0f * atan2(quaternion.y, quaternion.x);
		euler.z = 0;
	}
	else if (test < -0.4995f * unit) // singularity at south pole
	{
		euler.x = -3.141593 / 2;
		euler.y = -2.0f * atan2(quaternion.y, quaternion.x);
		euler.z = 0;
	}
	else // no singularity - this is the majority of cases
	{
		euler.x = asin(2.0f * (quaternion.w * quaternion.x - quaternion.y * quaternion.z));
		euler.y = atan2(2.0f * quaternion.w * quaternion.y + 2.0f * quaternion.z * quaternion.x, 1 - 2.0f * (quaternion.x * quaternion.x + quaternion.y * quaternion.y));
		euler.z = atan2(2.0f * quaternion.w * quaternion.z + 2.0f * quaternion.x * quaternion.y, 1 - 2.0f * (quaternion.z * quaternion.z + quaternion.x * quaternion.x));
	}

	return euler;
};

//******** 
//MATRICES
//********

//Clears a 4x4 matrix and sets the content to 0
void matrix_clear(float matrix[4][4])
{
	for (int i = 0; i < 5; i ++)
	{     
		for (int j = 0; j < 5; j ++)
		{
            matrix[i][j] = 0.0;
        }
    }
};

//Matrix Vector Multiplication
vect matrix_vector_multiplication(vect vector, float matrix[4][4])
{   
    vect result;

	result.x = vector.x * matrix[0][0] + vector.y * matrix[1][0] + vector.z * matrix[2][0] + vector.w * matrix[3][0];
    result.y = vector.x * matrix[0][1] + vector.y * matrix[1][1] + vector.z * matrix[2][1] + vector.w * matrix[3][1];
    result.z = vector.x * matrix[0][2] + vector.y * matrix[1][2] + vector.z * matrix[2][2] + vector.w * matrix[3][2];
    result.w = vector.x * matrix[0][3] + vector.y * matrix[1][3] + vector.z * matrix[2][3] + vector.w * matrix[3][3];

    return result;
};

//Matrix Vector Multiplication
void matrix_matrix_multiplication(float result[4][4], float matrix1[4][4], float matrix2[4][4])
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			result[j][i] = matrix1[j][0] * matrix2[0][i] + matrix1[j][1] * matrix2[1][i] + matrix1[j][2] * matrix2[2][i] + matrix1[j][3] * matrix2[3][i];
		};
	};
};

//Model Matrix
void matrix_model(float matrix[4][4], vect position, vect foward, vect up, vect right)
{
    matrix[0][0] = right.x;    matrix[0][1] = up.x;       matrix[0][2] = foward.x;   matrix[0][3] = 0;
    matrix[1][0] = right.y;    matrix[1][1] = up.y;       matrix[1][2] = foward.y;   matrix[1][3] = 0;
    matrix[2][0] = right.z;    matrix[2][1] = up.z;       matrix[2][2] = foward.z;   matrix[2][3] = 0;
    matrix[3][0] = position.x; matrix[3][1] = position.y; matrix[3][2] = position.z; matrix[3][3] = 1;	
};

//Rotation Matrix
void matrix_rotation(float matrix[4][4], vect foward, vect up, vect right)
{
    matrix[0][0] = right.x;    matrix[0][1] = up.x;       matrix[0][2] = foward.x;   matrix[0][3] = 0;
    matrix[1][0] = right.y;    matrix[1][1] = up.y;       matrix[1][2] = foward.y;   matrix[1][3] = 0;
    matrix[2][0] = right.z;    matrix[2][1] = up.z;       matrix[2][2] = foward.z;   matrix[2][3] = 0;
    matrix[3][0] =0; 		   matrix[3][1] = 0; 		  matrix[3][2] = 0; 		 matrix[3][3] = 1;	
};

//Look at Matrix
void matrix_lookat(float matrix[4][4], vect camera_position, vect camera_foward, vect camera_up, vect camera_right)
{
	float x = -(camera_position.x * camera_right.x  + camera_position.y * camera_right.y  + camera_position.z * camera_right.z);
	float y = -(camera_position.x * camera_up.x     + camera_position.y * camera_up.y     + camera_position.z * camera_up.z);
	float z = -(camera_position.x * camera_foward.x + camera_position.y * camera_foward.y + camera_position.z * camera_foward.z);
	
    matrix[0][0] = camera_right.x;  matrix[0][1] = camera_up.x;  matrix[0][2] = camera_foward.x;  matrix[0][3] = 0;
    matrix[1][0] = camera_right.y;  matrix[1][1] = camera_up.y;  matrix[1][2] = camera_foward.y;  matrix[1][3] = 0;
    matrix[2][0] = camera_right.z;  matrix[2][1] = camera_up.z;  matrix[2][2] = camera_foward.z;  matrix[2][3] = 0;
    matrix[3][0] = x; 	   	        matrix[3][1] = y; 			 matrix[3][2] = z; 		  		  matrix[3][3] = 1;
};

//Projection Matrix
void matrix_projection(float matrix[4][4], float camera_view_angle, float screen_height, float screen_width, float z_max_distance, float z_min_distance)
{
	//Set all values to zero
	matrix_clear(matrix); 

    //Projection Matrix Set-up
    float view_angle =    (float) camera_view_angle*(3.141593 / 180);
    float aspect_ratio =  (float) screen_height/screen_width;
    float z_normalize =   (float) z_max_distance/(z_max_distance - z_min_distance);
    float z_other =       (float) (-z_max_distance * z_min_distance)/(z_max_distance - z_min_distance);
    float feild_of_view = (float) 1/tan(view_angle / 2);

    //Creating the matrix
    matrix[0][0] = aspect_ratio * feild_of_view; 
    matrix[1][1] = feild_of_view; 
    matrix[2][2] = z_normalize; 
    matrix[3][2] = z_other; 
    matrix[2][3] = 1; 
};

//*********************
//Collision Detetection Cube and a Triangle
//*********************

#pragma once
#include <cmath>
//#include <glm/glm.hpp>

inline void findMinMax(float x0, float x1, float x2, float &min, float &max) {
	min = max = x0;
	if (x1 < min)
		min = x1;
	if (x1 > max)
		max = x1;
	if (x2 < min)
		min = x2;
	if (x2 > max)
		max = x2;
}

inline bool planeBoxOverlap(glm::vec3 normal, glm::vec3 vert, glm::vec3 maxbox) {
	glm::vec3 vmin, vmax;
	float v;
	for (size_t q = 0; q < 3; q++) {
		v = vert[q];
		if (normal[q] > 0.0f) {
			vmin[q] = -maxbox[q] - v;
			vmax[q] = maxbox[q] - v;
		} else {
			vmin[q] = maxbox[q] - v;
			vmax[q] = -maxbox[q] - v;
		}
	}
	if (glm::dot(normal, vmin) > 0.0f)
		return false;
	if (glm::dot(normal, vmax) >= 0.0f)
		return true;

	return false;
}

/*======================== X-tests ========================*/

inline bool axisTestX01(float a, float b, float fa, float fb, const glm::vec3 &v0,
	const glm::vec3 &v2, const glm::vec3 &boxhalfsize, float &rad, float &min,
	float &max, float &p0, float &p2) {
	p0 = a * v0.y - b * v0.z;
	p2 = a * v2.y - b * v2.z;
	if (p0 < p2) {
		min = p0;
		max = p2;
	} else {
		min = p2;
		max = p0;
	}
	rad = fa * boxhalfsize.y + fb * boxhalfsize.z;
	if (min > rad || max < -rad)
		return false;
	return true;
}
inline bool axisTestX2(float a, float b, float fa, float fb, const glm::vec3 &v0,
	const glm::vec3 &v1, const glm::vec3 &boxhalfsize, float &rad, float &min,
	float &max, float &p0, float &p1) {
	p0 = a * v0.y - b * v0.z;
	p1 = a * v1.y - b * v1.z;
	if (p0 < p1) {
		min = p0;
		max = p1;
	} else {
		min = p1;
		max = p0;
	}
	rad = fa * boxhalfsize.y + fb * boxhalfsize.z;
	if (min > rad || max < -rad)
		return false;
	return true;
}

/*======================== Y-tests ========================*/

inline bool axisTestY02(float a, float b, float fa, float fb, const glm::vec3 &v0,
	const glm::vec3 &v2, const glm::vec3 &boxhalfsize, float &rad, float &min,
	float &max, float &p0, float &p2) {
	p0 = -a * v0.x + b * v0.z;
	p2 = -a * v2.x + b * v2.z;
	if (p0 < p2) {
		min = p0;
		max = p2;
	} else {
		min = p2;
		max = p0;
	}
	rad = fa * boxhalfsize.x + fb * boxhalfsize.z;
	if (min > rad || max < -rad)
		return false;
	return true;
}

inline bool axisTestY1(float a, float b, float fa, float fb, const glm::vec3 &v0,
	const glm::vec3 &v1, const glm::vec3 &boxhalfsize, float &rad, float &min,
	float &max, float &p0, float &p1) {
	p0 = -a * v0.x + b * v0.z;
	p1 = -a * v1.x + b * v1.z;
	if (p0 < p1) {
		min = p0;
		max = p1;
	} else {
		min = p1;
		max = p0;
	}
	rad = fa * boxhalfsize.x + fb * boxhalfsize.z;
	if (min > rad || max < -rad)
		return false;
	return true;
}

/*======================== Z-tests ========================*/
inline bool axisTestZ12(float a, float b, float fa, float fb, const glm::vec3 &v1,
	const glm::vec3 &v2, const glm::vec3 &boxhalfsize, float &rad, float &min,
	float &max, float &p1, float &p2) {
	p1 = a * v1.x - b * v1.y;
	p2 = a * v2.x - b * v2.y;
	if (p1 < p2) {
		min = p1;
		max = p2;
	} else {
		min = p2;
		max = p1;
	}
	rad = fa * boxhalfsize.x + fb * boxhalfsize.y;
	if (min > rad || max < -rad)
		return false;
	return true;
}

inline bool axisTestZ0(float a, float b, float fa, float fb, const glm::vec3 &v0,
	const glm::vec3 &v1, const glm::vec3 &boxhalfsize, float &rad, float &min,
	float &max, float &p0, float &p1) {
	p0 = a * v0.x - b * v0.y;
	p1 = a * v1.x - b * v1.y;
	if (p0 < p1) {
		min = p0;
		max = p1;
	} else {
		min = p1;
		max = p0;
	}
	rad = fa * boxhalfsize.x + fb * boxhalfsize.y;
	if (min > rad || max < -rad)
		return false;
	return true;
}

bool triBoxOverlap(glm::vec3 boxcenter, glm::vec3 boxhalfsize, glm::vec3 tv0, glm::vec3 tv1,
	glm::vec3 tv2) {
	/*    use separating axis theorem to test overlap between triangle and box */
	/*    need to test for overlap in these directions: */
	/*    1) the {x,y,z}-directions (actually, since we use the AABB of the triangle */
	/*       we do not even need to test these) */
	/*    2) normal of the triangle */
	/*    3) crossproduct(edge from tri, {x,y,z}-directin) */
	/*       this gives 3x3=9 more tests */
	glm::vec3 v0, v1, v2;
	float min, max, p0, p1, p2, rad, fex, fey, fez;
	glm::vec3 normal, e0, e1, e2;

	/* This is the fastest branch on Sun */
	/* move everything so that the boxcenter is in (0,0,0) */
	v0 = tv0 - boxcenter;
	v1 = tv1 - boxcenter;
	v2 = tv2 - boxcenter;

	/* compute triangle edges */
	e0 = v1 - v0;
	e1 = v2 - v1;
	e2 = v0 - v2;

	/* Bullet 3:  */
	/*  test the 9 tests first (this was faster) */
	fex = fabsf(e0.x);
	fey = fabsf(e0.y);
	fez = fabsf(e0.z);

	if (!axisTestX01(e0.z, e0.y, fez, fey, v0, v2, boxhalfsize, rad, min, max, p0, p2))
		return false;
	if (!axisTestY02(e0.z, e0.x, fez, fex, v0, v2, boxhalfsize, rad, min, max, p0, p2))
		return false;
	if (!axisTestZ12(e0.y, e0.x, fey, fex, v1, v2, boxhalfsize, rad, min, max, p1, p2))
		return false;

	fex = fabsf(e1.x);
	fey = fabsf(e1.y);
	fez = fabsf(e1.z);

	if (!axisTestX01(e1.z, e1.y, fez, fey, v0, v2, boxhalfsize, rad, min, max, p0, p2))
		return false;
	if (!axisTestY02(e1.z, e1.x, fez, fex, v0, v2, boxhalfsize, rad, min, max, p0, p2))
		return false;
	if (!axisTestZ0(e1.y, e1.x, fey, fex, v0, v1, boxhalfsize, rad, min, max, p0, p1))
		return false;

	fex = fabsf(e2.x);
	fey = fabsf(e2.y);
	fez = fabsf(e2.z);
	if (!axisTestX2(e2.z, e2.y, fez, fey, v0, v1, boxhalfsize, rad, min, max, p0, p1))
		return false;
	if (!axisTestY1(e2.z, e2.x, fez, fex, v0, v1, boxhalfsize, rad, min, max, p0, p1))
		return false;
	if (!axisTestZ12(e2.y, e2.x, fey, fex, v1, v2, boxhalfsize, rad, min, max, p1, p2))
		return false;

	/* Bullet 1: */
	/*  first test overlap in the {x,y,z}-directions */
	/*  find min, max of the triangle each direction, and test for overlap in */
	/*  that direction -- this is equivalent to testing a minimal AABB around */
	/*  the triangle against the AABB */

	/* test in X-direction */
	findMinMax(v0.x, v1.x, v2.x, min, max);
	if (min > boxhalfsize.x || max < -boxhalfsize.x)
		return false;

	/* test in Y-direction */
	findMinMax(v0.y, v1.y, v2.y, min, max);
	if (min > boxhalfsize.y || max < -boxhalfsize.y)
		return false;

	/* test in Z-direction */
	findMinMax(v0.z, v1.z, v2.z, min, max);
	if (min > boxhalfsize.z || max < -boxhalfsize.z)
		return false;

	/* Bullet 2: */
	/*  test if the box intersects the plane of the triangle */
	/*  compute plane equation of triangle: normal*x+d=0 */
	normal = glm::cross(e0, e1);
	if (!planeBoxOverlap(normal, v0, boxhalfsize))
		return false;

	return true; /* box and triangle overlaps */
}