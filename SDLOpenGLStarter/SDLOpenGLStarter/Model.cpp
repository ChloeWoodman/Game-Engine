#include "Model.h"

namespace GE {
	bool Model::loadFromFile(const char* filename)
	{
		// temporary vector for storing model vertices loaded from file
		std::vector<Vertex> loadedVertices;

		// create an importer object
		Assimp::Importer imp;

		// load the model into a scene object
		const aiScene* pScene = imp.ReadFile(filename, aiProcessPreset_TargetRealtime_Quality | aiProcess_FlipUVs);

		// check if the file was opened successfully
		if (!pScene) {
			return false;
		}

		// loop through all the meshes in the scene and get the vertices
		for (unsigned int MeshIdx = 0; MeshIdx < pScene->mNumMeshes; MeshIdx++) {
			const aiMesh* mesh = pScene->mMeshes[MeshIdx];

			// loop through all the faces in the mesh to extract vertices
			for (unsigned int faceIdx = 0; faceIdx < mesh->mNumFaces; faceIdx++) {
				const aiFace& face = mesh->mFaces[faceIdx];

				// extract a vertex from the mesh's main vertex array for each point in the face
				for (unsigned int vertIdx = 0; vertIdx < 3; vertIdx++) {
					// extract the position and texture coordinates based on the index number
					const aiVector3D* pos = &mesh->mVertices[face.mIndices[vertIdx]];
					const aiVector3D uv = mesh->mTextureCoords[0][face.mIndices[vertIdx]];
					const aiVector3D* norm = &mesh->mNormals[face.mIndices[vertIdx]];

					// create a new object in the shape array based on extracted vertex
					// this shape array will be used to create the vertex buffer
					loadedVertices.push_back(Vertex(pos->x, pos->y, pos->z, uv.x, uv.y, norm->x, norm->y, norm->z));
				}
			}
		}

		// number of vertices is derived from number of items in temp vector
		numVertices = loadedVertices.size();

		// copy vertices into memory buffer to transfer to a VBO later
		glGenBuffers(1, &vbo);

		// create the buffer
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		// copy vertex data from vector to buffer
		glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vertex), loadedVertices.data(), GL_STATIC_DRAW);

		//Remove the buffer from the pipeline
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		return true;
	}
}