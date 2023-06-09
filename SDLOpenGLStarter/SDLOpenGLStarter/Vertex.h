#ifndef VERTEX_H
#define VERTEX_H

// Represents a vertex in the engine
struct Vertex {
	// Location
	float x, y, z;
	float u, v;
	float nx, ny, nz;

	// Constructors
	// Sets vertex to passed values
	Vertex(float _x, float _y, float _z, float _u, float _v, float _nx, float _ny, float _nz) {
		// Location
		x = _x;
		y = _y;
		z = _z;

		// Colour
		u = _u;
		v = _v;

		nx = _nx;
		ny = _ny;
		nz = _nz;
	}

	// Default, set to 0.0
	Vertex() {
		x = y = z = 0.0f;
		u = v = 0.0f;
		nx = ny = nz = 0.0f;
	}
};
#endif // VERTEX_H
