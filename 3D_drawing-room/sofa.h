#ifndef sofa_h
#define sofa_h

#include "shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include<vector>


class Sofa {

public:
	std::vector<glm::mat4> modelMatrices;
	float tox, toy, toz;
	Sofa(float x = 0, float y = 0, float z = 0) {
		tox = x;
		toy = y;
		toz = z;
	}
	glm::mat4 transforamtion(float tx, float ty, float tz, float rx, float ry, float rz, float sx, float sy, float sz) {
		tx += tox;
		ty += toy;
		tz += toz;
		glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
		translateMatrix = glm::translate(identityMatrix, glm::vec3(tx, ty, tz));
		rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rx), glm::vec3(1.0f, 0.0f, 0.0f));
		rotateYMatrix = glm::rotate(identityMatrix, glm::radians(ry), glm::vec3(0.0f, 1.0f, 0.0f));
		rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rz), glm::vec3(0.0f, 0.0f, 1.0f));
		scaleMatrix = glm::scale(identityMatrix, glm::vec3(sx, sy, sz));
		model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
		return model;
	}

	Shader local_rotation(Shader ourShader, unsigned int  VAO, unsigned int VAO2, unsigned int VAO3, unsigned int VAO4, unsigned int VAO5, unsigned int VAO6, float angle = 0) {
		glm::mat4 model;
		float rotateAngle_X = 0;
		float rotateAngle_Y = 0;
		float rotateAngle_Z = 0;
		model = transforamtion(-1.5, -.60, -5.25, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 0.2f, 3.0f, 9.0f);
		modelMatrices.push_back(model);
		model = transforamtion(-1.4, -.50, -5.25, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 2.0f, 0.1f, 9.0f);
		modelMatrices.push_back(model);

		model = transforamtion(-1.4, -.50 + .21, -5.25, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 0.08f, 0.1f, 9.0f);
		modelMatrices.push_back(model);


		model = transforamtion(-1.4, -.75, -5.25 - .35 / 2, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 2.0f, 2.0f, 0.35f);
		modelMatrices.push_back(model);
		model = transforamtion(-1.4, -.75, -5.25 + 9.0 / 2, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 2.0f, 2.0f, 0.35f);
		modelMatrices.push_back(model);
		model = transforamtion(-1.4, -.50 + .1 / 2, -5.25, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 2.0f, 0.1f + .3f, 9.0f);
		modelMatrices.push_back(model);
		unsigned int vertex_array[] = { VAO, VAO2, VAO3, VAO4, VAO5, VAO6 };
		glm::vec3 averagePosition(0.0f);
		for (const glm::mat4& model : modelMatrices) {
			averagePosition += glm::vec3(model[3]);
		}
		averagePosition /= modelMatrices.size();

		glm::mat4 moveToOrigin = glm::translate(glm::mat4(1.0f), -averagePosition);

		glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));

		glm::mat4 moveToOriginalPosition = glm::translate(glm::mat4(1.0f), averagePosition);

		glm::mat4 groupTransform = moveToOriginalPosition * rotation * moveToOrigin;

		int i = 0;
		for (glm::mat4& model : modelMatrices) {

			model = groupTransform * model;
			ourShader.setMat4("model", model);
			glBindVertexArray(vertex_array[i]);
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
			i++;
		}
		return ourShader;
	}

	Shader ret_shader(Shader ourShader, unsigned int  VAO, unsigned int VAO2, unsigned int VAO3, unsigned int VAO4, unsigned int VAO5, unsigned int VAO6) {
		glm::mat4 model;
		float rotateAngle_X = 0;
		float rotateAngle_Y = 0;
		float rotateAngle_Z = 0;

		//sofa back
		model = transforamtion(-1.5, -.60, -5.25, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 0.2f, 3.0f, 9.0f);
		ourShader.setMat4("model", model);
		// ourShader.setVec4("color", glm::vec4(0.5f, 0.5f, 0.0f,1.0f));
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


		//sofa seat
		model = transforamtion(-1.4, -.50, -5.25, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 2.0f, 0.1f, 9.0f);
		ourShader.setMat4("model", model);
		//ourShader.setVec4("color", glm::vec4(0.5f, 0.5f, 0.0f, 1.0f));
		glBindVertexArray(VAO2);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


		//sofa splitter
		model = transforamtion(-1.4, -.50+.21, -5.25, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 0.08f, 0.1f, 9.0f);
		ourShader.setMat4("model", model);
		//ourShader.setVec4("color", glm::vec4(0.5f, 0.5f, 0.0f, 1.0f));
		glBindVertexArray(VAO3);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


		//sofa left side
		model = transforamtion(-1.4, -.75, -5.25 - .35 / 2, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 2.0f, 2.0f, 0.35f);
		ourShader.setMat4("model", model);
		//ourShader.setVec4("color", glm::vec4(0.5f, 0.5f, 0.0f, 1.0f));
		glBindVertexArray(VAO4);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


		//sofa right side
		model = transforamtion(-1.4, -.75, -5.25 + 9.0 / 2, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 2.0f, 2.0f, 0.35f);
		ourShader.setMat4("model", model);
		//ourShader.setVec4("color", glm::vec4(0.5f, 0.5f, 0.0f, 1.0f));
		glBindVertexArray(VAO5);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


		//sofa left seat
		model = transforamtion(-1.4, -.50 + .1 / 2, -5.25, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 2.0f, 0.1f + .3f, 9.0f);
		ourShader.setMat4("model", model);
		//ourShader.setVec4("color", glm::vec4(0.5f, 0.5f, 0.0f, 1.0f));
		glBindVertexArray(VAO6);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		return ourShader;
	}
};


#endif