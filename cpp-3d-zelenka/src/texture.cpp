#include "texture.h"

#include <IL/il.h>

extern std::ofstream log_file;

GLuint get_textureID(const std::string& name) {
    GLuint textureID = 0;
    int error = IL_NO_ERROR;

	//иницилилизует только в первый вызов
    ilInit();                           

    GLuint imageID = 0;
    ilGenImages(1, &imageID); //Generation of one image name
    ilBindImage(imageID);     //Binding of image name

    ilEnable(IL_ORIGIN_SET);  //Flip image
    ilOriginFunc(IL_ORIGIN_UPPER_LEFT);

    if( ilLoadImage(name.c_str()) ) {
        /* Convert image to RGBA */
        ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        // Specify a linear filter for both the minification and
        // magnification.
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        // при фильтрации игнорируются тексели, выходящие за границу текстуры для s координаты
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        // при фильтрации игнорируются тексели, выходящие за границу текстуры для t координаты
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // Sets drawing mode to GL_MODULATE
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

        glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP),
                    ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0,
                    ilGetInteger(IL_IMAGE_FORMAT), ilGetInteger(IL_IMAGE_TYPE),
                    ilGetData());

        glBindTexture(GL_TEXTURE_2D, 0);
    }
    else  error = ilGetError();
        
    if (imageID) 
		ilDeleteImages(1, &imageID);

    if (error != IL_NO_ERROR) //throw ILexception(name.c_str(), error);
        log_file<<"error, get_textureID( "<<name.c_str()<<" ) != IL_NO_ERROR \n";        

    return textureID;
}

GLuint get_textureDepthID(GLsizei width, GLsizei height) {
	GLuint texture = 0;
	// запросим у OpenGL свободный индекс текстуры
	glGenTextures(1, &texture);

	// сделаем текстуру активной
	glBindTexture(GL_TEXTURE_2D, texture);

	// установим параметры фильтрации текстуры - линейная фильтрация
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// установим параметры "оборачиваниея" текстуры - отсутствие оборачивания
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// необходимо для использования depth-текстуры как shadow map
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);

	// создаем "пустую" текстуру под depth-данные
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
	          width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

	// проверим на наличие ошибок
	GLenum error = glGetError();
	if(error != GL_NO_ERROR)
		log_file<<"Error create texture Depth, error = "<<error<<std::endl;

	return texture;
}
