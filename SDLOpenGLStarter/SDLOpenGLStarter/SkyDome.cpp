#include "SkyDome.h"

namespace GE
{
	SkyDome::SkyDome(float res, float rad)
	{
		float pi = 3.14159;
		float resolution = res;
		float azimuthStep = (2.0f * pi) / (float)(resolution - 1);
		float elevationStep = pi / (float)(resolution - 1);

		float radius = rad;
		float x, y, z, u, v, nx, ny, nz;
		float theta, phi;

		for (int latitude = 0; latitude < resolution + 1; latitude++)
		{
			for (int longitude = 0; longitude < resolution + 1; longitude++)
			{
				theta = pi / 2.0f - longitude * elevationStep;
				phi = latitude * azimuthStep;

				x = (radius * cosf(theta)) * cosf(phi);
				z = (radius * cosf(theta)) * sinf(phi);
				y = radius * sinf(theta);

				u = (float)latitude / (float)resolution;
				v = (float)longitude / (float)resolution;

				nx = 0.0f;
				ny = 0.0f;
				nz = 0.0f;

				vertices.emplace_back(x, y, z, u, v, nx, ny, nz);
			}
		}

		unsigned int c, n;

		for (int latitude = 0; latitude < resolution - 1; latitude++)
		{
			c = latitude * (resolution + 1);
			n = c + resolution + 1;

			for (int longitude = 0; longitude < resolution - 1; longitude++)
			{
				if (longitude != 0)
				{
					indices.push_back(c);
					indices.push_back(n);
					indices.push_back(c + 1);
				}

				if (longitude != resolution - 1)
				{
					indices.push_back(c + 1);
					indices.push_back(n);
					indices.push_back(n + 1);
				}
				c++;
				n++;
			}
		}

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		indexCount = indices.size();
	}

	SkyDome::~SkyDome() {} // Smart pointer will handle memory release

	GLuint SkyDome::getVbo() const { return vbo; }

	GLuint SkyDome::getIbo() const { return ibo; }

	GLuint SkyDome::getIndexCount() const { return indexCount; }
}