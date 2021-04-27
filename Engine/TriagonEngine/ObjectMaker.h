#pragma once
#include "dependencies.h"

#include "ShaderMaker.h"

struct Object {

private:

	double* verticesArrayPointer;
	int numberOf_Vertices = 0;

	unsigned int* indicesArrayPointer;
	int numberOf_Indices = 0;

public:
	struct Transform { //TODO
		double X = 0.0;
		double Y = 0.0;
		double Z = 0.0;
	};

	bool InstanceObject = false;

	bool CleanObject = true; //Any Errors Occur During Object Creation Will Be Reflected Here, Hopefully, its not fully implemented

	unsigned int CreateObjectID; //The Main Purpose of ObjectMaker is to "assemble" this OpenGl ID

	const char* DebugObjectName = "No Name";

	Object(const char* OBJFilePath, const char* ObjectDebugName, bool InstancePrep) {

		InstanceObject = InstancePrep;

		//"Direct" Coordinates//

		double* vertexNormalsArrayPointer;
		int numberOf_VertexNormals = 0;

		double* textureCoordsArrayPointer;
		int numberOf_TextureCoords = 0;

		//^"Direct" Coordinates^//

		unsigned int* messyArrayPointer; //Temporary Array Housing Many Diffrent Technical Coordinaes From OBJ File
		int numberOf_messyArrayStuff = 0;

		DebugObjectName = ObjectDebugName;

		std::ifstream ObjFile;

		std::string FileDataRAWString;

		//ensure ifstream objects can throw exceptions:
		ObjFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try {
			//Save Vertex Shader Source //This is just sample code to read from file
			ObjFile.open(OBJFilePath);
			std::stringstream FileDataStream;
			FileDataStream << ObjFile.rdbuf();
			FileDataRAWString = FileDataStream.str();
			ObjFile.close();
		}
		catch (std::ifstream::failure e) {
			printf("Coult Not Open Obj File: %s\n", DebugObjectName);
			CleanObject = false;
		}

		std::replace(FileDataRAWString.begin(), FileDataRAWString.end(), '/', ' '); // replace all '/' to ' ' for the indecies fields

		numberOf_Vertices = ExtractCoods(FileDataRAWString, verticesArrayPointer, "v  ", 3, 3, false); //Extract all coods with the prefix "v" for verticies 
		numberOf_VertexNormals = ExtractCoods(FileDataRAWString, vertexNormalsArrayPointer, "vn ", 3, 3, false); //Extract all coods with the prefix "vn" for vertex normals
		numberOf_TextureCoords = ExtractCoods(FileDataRAWString, textureCoordsArrayPointer, "vt ", 3, 3, false); //Extract all coods with the prefix "vt" for texture coods

		numberOf_messyArrayStuff = ExtractCoods(FileDataRAWString, messyArrayPointer, "f ", 2, 9, true); //Extract all coods with the prefix "f" for face information

		MergeDataToIndiciesArray( //Combine all the data into an array opengl can understand
			&numberOf_Vertices,
			&numberOf_Indices,
			numberOf_messyArrayStuff,
			verticesArrayPointer,
			indicesArrayPointer,
			textureCoordsArrayPointer,
			vertexNormalsArrayPointer,
			messyArrayPointer
		);
		CreateOpenGlObjects(InstancePrep);

		PurityChecker();

	}
public:

	void Draw(Shader shader) {

		if (InstanceObject) {
			return;
		}

		if (CleanObject == false) {
			return;
		}
		
		shader.Use();
		glBindVertexArray(CreateObjectID); //Binds the all the related data from a VBO and an EBO to the corresponding GPU buffers
		glDrawElements(GL_TRIANGLES, numberOf_Indices, GL_UNSIGNED_INT, 0); //Renders what is currently in the GPU buffer
		glBindVertexArray(0);

		
	}
	void DrawInstanced(Shader shader, int count) {

		if (InstanceObject == false) {
			return;
		}

		if (CleanObject == false) {
			return;
		}
		shader.Use();
		glBindVertexArray(CreateObjectID); //Binds the all the related data from a VBO and an EBO to the corresponding GPU buffers
		glDrawElementsInstanced(GL_TRIANGLES, numberOf_Indices, GL_UNSIGNED_INT, 0, count); //Renders what is currently in the GPU buffer
		glBindVertexArray(0);

	}
	void PrintDebug() { //Prints info about the object such as its debug name, as well as "vertex"(9 per row) and "index"(3 per row) rows
		printf("Object Name %s", DebugObjectName);

		printf("Object Pure: %s", CleanObject);

		printf("\n_________________________\n");

		for (int i = 0; i < numberOf_Vertices; i++) {
			if (i % 9 == 0) {
				printf("\n");
			}
			printf("%f ", verticesArrayPointer[i]);
		}
		printf("\n_________________________\n");

		for (int i = 0; i < numberOf_Indices; i++) {
			if (i % 3 == 0) {
				printf("\n");
			}
			printf("%u ", indicesArrayPointer[i]);
		}
		printf("\n_________________________\n");
	}

private:

	void PurityChecker() {
		if (CreateObjectID == NULL) {
			CleanObject = false;
		}
		if (CleanObject == false) {
			printf("Object Error: %s Object Is Impure\n", DebugObjectName);
		}
	}
	void CreateOpenGlObjects(bool InstancePrep) {

		if (CleanObject == false) {
			return;
		}

		unsigned int VAO; //Vertext Array Object (VAO) & (EBO) is basically the combination of which vertext objects (VBOs) to use which shaders and what inputs to use
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		unsigned int VBO;
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO); //This line Binds VBO to the VAO
		glBufferData(GL_ARRAY_BUFFER, sizeof(double) * numberOf_Vertices, verticesArrayPointer, GL_STATIC_DRAW); //Send the Verticies array to the GPU

		glEnableVertexAttribArray(0); //Position
		glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 9 * sizeof(double), (void*)0); // The first parameter specifies which vertex attribute we want to configure. Remember that we specified the location of the position vertex attribute in the vertex shader with layout (location = 0).
		//glVertexAttribDivisor(0, InstancePrep); //If This is an instance object we need to "DIVIDE" the possitions for each instance

		//For future referance this is how to add more atributes using the "brute force" way

		glEnableVertexAttribArray(1); //TextureCoords
		glVertexAttribPointer(1, 3, GL_DOUBLE, GL_FALSE, 9 * sizeof(double), (void*)(3 * sizeof(double)));

		glEnableVertexAttribArray(2); //Normal Coods
		glVertexAttribPointer(2, 3, GL_DOUBLE, GL_FALSE, 9 * sizeof(double), (void*)(6 * sizeof(double)));

		unsigned int EBO;
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); //This line Binds EBO to the VAO
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numberOf_Indices, indicesArrayPointer, GL_STATIC_DRAW); //Copy the draw order array into the GPU

		CreateObjectID = VAO;


	}

	template <typename VariableType>
	int ExtractCoods(std::string RawData, VariableType*& ArrayPointer, const char* prefix, int prefixLength, int numberOfCoods, bool RoundToInt) {

		int numberOf_Coordinates = numberOfCoods;
		ArrayPointer = (VariableType*)malloc(numberOf_Coordinates);
		for (int nextLine = RawData.find(prefix); nextLine >= 0; nextLine = RawData.find(prefix, nextLine + 1)) { //find the next line

			std::string currentNumber = "";

			for (int i = nextLine + prefixLength; RawData.at(i) != '\n'; i++) { //find the first number
				currentNumber += RawData.at(i);
			}
			ArrayPointer = (VariableType*)realloc(ArrayPointer, sizeof(VariableType) * numberOf_Coordinates); //TODO Maybe a memory leak on bad file load

			if (ArrayPointer == NULL) {
				printf("Could Not Allocate Memory");
				CleanObject = false;
				return NULL;
			}
			else {
				//std::string::size_type InnitalOffset = 0;
				std::string::size_type OffsetToNextChar = 0;
				std::string::size_type TotalOffset = 0;

				for (int i = 0; i < numberOfCoods; i++) {
					if (RoundToInt) {
						ArrayPointer[(numberOf_Coordinates - numberOfCoods) + i] = (int)(std::stod(currentNumber.substr(TotalOffset), &OffsetToNextChar));
					}
					else {
						ArrayPointer[(numberOf_Coordinates - numberOfCoods) + i] = std::stod(currentNumber.substr(TotalOffset), &OffsetToNextChar);
					}
					TotalOffset += OffsetToNextChar;
				}
				numberOf_Coordinates += numberOfCoods;
			}
		}
		return numberOf_Coordinates -= numberOfCoods;
	}

	void MergeDataToIndiciesArray(int* numberOfVerticesPointer, int* numberOfIndiciesPointer,int numberOfMessyArrayStuff, double*& VerticiesArrayPointer, unsigned int*& IndiciesArrayPointer ,double*& TextureCoordsArrayPointer, double*& NormalCoordsArrayPointer, unsigned int*& MessyArrayPointer) {

		for (int i = 0; i < numberOfMessyArrayStuff; i++) { //Decriment all indicies, opengl starts from 0;
			MessyArrayPointer[i]--;
		}
		double* CleanVerticies = (double*)malloc(sizeof(double));//Final verticies array
		int numberOf_CleanVerticies = 0;

		unsigned int* CleanIndicies = (unsigned int*)malloc(sizeof(unsigned int)); //Final indicies array
		int numberOf_CleanIndicies = 0;
		int largestIndicie = 0;

		unsigned int* FoundIndicies = (unsigned int*)malloc(sizeof(unsigned int)); //Array used to check is the current messy index was found
		int numberOf_FoundIndicies = 0;

		for (int i = 0; i+3 <= numberOfMessyArrayStuff; i+=3) {
			
			int currentCompressedIndex = (MessyArrayPointer[i] * 100) + (MessyArrayPointer[i + 1] * 10) + MessyArrayPointer[i + 2];

			int j = 0;

			for (; j < numberOf_FoundIndicies; j++) { //Check if current vertex is unique, I.E. We already found it

				if (FoundIndicies[j] == currentCompressedIndex) { //Add it into indicies

					CleanIndicies = (unsigned int*)realloc(CleanIndicies, sizeof(unsigned int) + (sizeof(unsigned int) * numberOf_CleanIndicies));
					CleanIndicies[numberOf_CleanIndicies] = j;
					numberOf_CleanIndicies++;
					break;
				}

			}
			if (j == numberOf_FoundIndicies) { //Yes current vertex is unique

				CleanIndicies = (unsigned int*)realloc(CleanIndicies, sizeof(unsigned int) + (sizeof(unsigned int) * numberOf_CleanIndicies)); //Add a new index
				CleanIndicies[numberOf_CleanIndicies] = largestIndicie;
				numberOf_CleanIndicies++;
				largestIndicie++;

				CleanVerticies = (double*)realloc(CleanVerticies, 9 * sizeof(double) + (sizeof(double) * numberOf_CleanVerticies)); //Add new verticies 

				CleanVerticies[numberOf_CleanVerticies + 0] = VerticiesArrayPointer[3 * MessyArrayPointer[i]];
				CleanVerticies[numberOf_CleanVerticies + 1] = VerticiesArrayPointer[3 * MessyArrayPointer[i] + 1];
				CleanVerticies[numberOf_CleanVerticies + 2] = VerticiesArrayPointer[3 * MessyArrayPointer[i] + 2];

				CleanVerticies[numberOf_CleanVerticies + 3] = TextureCoordsArrayPointer[3 * MessyArrayPointer[i + 1]];
				CleanVerticies[numberOf_CleanVerticies + 4] = TextureCoordsArrayPointer[3 * MessyArrayPointer[i + 1] + 1];
				CleanVerticies[numberOf_CleanVerticies + 5] = TextureCoordsArrayPointer[3 * MessyArrayPointer[i + 1] + 2];

				CleanVerticies[numberOf_CleanVerticies + 6] = NormalCoordsArrayPointer[3 * MessyArrayPointer[i + 2]];
				CleanVerticies[numberOf_CleanVerticies + 7] = NormalCoordsArrayPointer[3 * MessyArrayPointer[i + 2] + 1];
				CleanVerticies[numberOf_CleanVerticies + 8] = NormalCoordsArrayPointer[3 * MessyArrayPointer[i + 2] + 2];

				numberOf_CleanVerticies += 9;

				FoundIndicies = (unsigned int*)realloc(FoundIndicies, sizeof(unsigned int) + (sizeof(unsigned int) * numberOf_FoundIndicies)); //Add a new found index
				FoundIndicies[numberOf_FoundIndicies] = currentCompressedIndex;
				numberOf_FoundIndicies++;
			}

		}
		free(FoundIndicies);

		free(VerticiesArrayPointer);
		free(TextureCoordsArrayPointer);
		free(NormalCoordsArrayPointer);

		free(MessyArrayPointer);

		IndiciesArrayPointer = CleanIndicies;
		VerticiesArrayPointer = CleanVerticies;

		*numberOfVerticesPointer = numberOf_CleanVerticies;
		*numberOfIndiciesPointer = numberOf_CleanIndicies;

		if (numberOf_CleanVerticies <= 0 || numberOf_CleanIndicies <= 0) {
			CleanObject = false;
		}
	}
};


glm::mat4 Translate(float X, float Y, float Z) {
	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::translate(trans,glm::vec3(X, Y, Z));

	return trans;
}

glm::mat4 SetRotation(float EulerX, float EulerY, float EulerZ) { //This Is Very Buggy Now
	glm::mat4 transX = glm::mat4(1.0f); //Create Identity Matrix(s)
	glm::mat4 transY = glm::mat4(1.0f);
	glm::mat4 transZ = glm::mat4(1.0f);

	transX = glm::rotate(transX, glm::radians(EulerX), glm::vec3(1.0, 0.0, 0.0));
	transY = glm::rotate(transY, glm::radians(EulerY), glm::vec3(0.0, 1.0, 0.0));
	transZ = glm::rotate(transZ, glm::radians(EulerZ), glm::vec3(0.0, 0.0, 1.0));

	return transX * transY * transZ;
}

