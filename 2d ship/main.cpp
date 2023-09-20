#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
float rotateAngle = 0.0;
float translate_X = 0.0;
float translate_Y = 0.0;
float scale_X = 1.0;
float scale_Y = 1.0;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"uniform mat4 transform;\n"
"void main()\n"
"{\n"
"   gl_Position = transform * vec4(aPos, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

//lower body color
const char* fragmentShaderSource2 = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.9f, 0.9f, 0.9f, 1.0f);\n"
"}\n\0";

//lower body extra color
const char* fragmentShaderSource3 = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.431f, 0.8f, 0.592f, 1.0f);\n"
"}\n\0";

//upper rail color
const char* fragmentShaderSource4 = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.8f, 0.612f, 0.49f, 1.0f);\n"
"}\n\0";

//rod color
const char* fragmentShaderSource5 = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.38f, 0.333f, 0.325f, 1.0f);\n"
"}\n\0";


//rod tip color
const char* fragmentShaderSource6 = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.38f, 0.333f, 0.325f, 1.0f);\n"
"}\n\0";



//flag color
const char* fragmentShaderSource7 = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.91f, 0.192f, 0.043f, 1.0f);\n"
"}\n\0";


//right sail color
const char* fragmentShaderSource8 = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.514f, 0.69f, 0.949f, 1.0f);\n"
"}\n\0";


//left sail color
const char* fragmentShaderSource9 = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.514f, 0.69f, 0.949f, 1.0f);\n"
"}\n\0";


//sail strip color
const char* fragmentShaderSource10 = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.835f, 0.847f, 0.859f, 1.0f);\n"
"}\n\0";



//window color
const char* fragmentShaderSource11 = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.729f, 0.741f, 0.761f, 0.961f);\n"
"}\n\0";





int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CSE 4208: Computer Graphics Laboratory", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // fragment shader1
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }



    //fragment shader 2
    unsigned int fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader2, 1, &fragmentShaderSource2, NULL);
    glCompileShader(fragmentShader2);
    // check for shader compile errors
    glGetShaderiv(fragmentShader2, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader2, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    unsigned int shaderProgram2 = glCreateProgram();
    glAttachShader(shaderProgram2, vertexShader);
    glAttachShader(shaderProgram2, fragmentShader2);
    glLinkProgram(shaderProgram2);
    // check for linking errors
    glGetProgramiv(shaderProgram2, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram2, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }



    //fragment shader 3
    unsigned int fragmentShader3 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader3, 1, &fragmentShaderSource3, NULL);
    glCompileShader(fragmentShader3);
    // check for shader compile errors
    glGetShaderiv(fragmentShader3, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader3, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    unsigned int shaderProgram3 = glCreateProgram();
    glAttachShader(shaderProgram3, vertexShader);
    glAttachShader(shaderProgram3, fragmentShader3);
    glLinkProgram(shaderProgram3);
    // check for linking errors
    glGetProgramiv(shaderProgram3, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram3, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }



    //fragment shader 4
    unsigned int fragmentShader4 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader4, 1, &fragmentShaderSource4, NULL);
    glCompileShader(fragmentShader4);
    // check for shader compile errors
    glGetShaderiv(fragmentShader4, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader4, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    unsigned int shaderProgram4 = glCreateProgram();
    glAttachShader(shaderProgram4, vertexShader);
    glAttachShader(shaderProgram4, fragmentShader4);
    glLinkProgram(shaderProgram4);
    // check for linking errors
    glGetProgramiv(shaderProgram4, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram4, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }




    //fragment shader 5
    unsigned int fragmentShader5 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader5, 1, &fragmentShaderSource5, NULL);
    glCompileShader(fragmentShader5);
    // check for shader compile errors
    glGetShaderiv(fragmentShader5, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader5, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    unsigned int shaderProgram5 = glCreateProgram();
    glAttachShader(shaderProgram5, vertexShader);
    glAttachShader(shaderProgram5, fragmentShader5);
    glLinkProgram(shaderProgram5);
    // check for linking errors
    glGetProgramiv(shaderProgram5, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram5, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }




    //fragment shader 6
    unsigned int fragmentShader6 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader6, 1, &fragmentShaderSource6, NULL);
    glCompileShader(fragmentShader6);
    // check for shader compile errors
    glGetShaderiv(fragmentShader6, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader6, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    unsigned int shaderProgram6 = glCreateProgram();
    glAttachShader(shaderProgram6, vertexShader);
    glAttachShader(shaderProgram6, fragmentShader6);
    glLinkProgram(shaderProgram6);
    // check for linking errors
    glGetProgramiv(shaderProgram6, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram6, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }




    //fragment shader 7
    unsigned int fragmentShader7 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader7, 1, &fragmentShaderSource7, NULL);
    glCompileShader(fragmentShader7);
    // check for shader compile errors
    glGetShaderiv(fragmentShader7, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader7, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    unsigned int shaderProgram7 = glCreateProgram();
    glAttachShader(shaderProgram7, vertexShader);
    glAttachShader(shaderProgram7, fragmentShader7);
    glLinkProgram(shaderProgram7);
    // check for linking errors
    glGetProgramiv(shaderProgram7, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram7, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }




    //fragment shader 8
    unsigned int fragmentShader8 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader8, 1, &fragmentShaderSource8, NULL);
    glCompileShader(fragmentShader8);
    // check for shader compile errors
    glGetShaderiv(fragmentShader8, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader8, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    unsigned int shaderProgram8 = glCreateProgram();
    glAttachShader(shaderProgram8, vertexShader);
    glAttachShader(shaderProgram8, fragmentShader8);
    glLinkProgram(shaderProgram8);
    // check for linking errors
    glGetProgramiv(shaderProgram8, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram8, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }




    //fragment shader 9
    unsigned int fragmentShader9 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader9, 1, &fragmentShaderSource9, NULL);
    glCompileShader(fragmentShader9);
    // check for shader compile errors
    glGetShaderiv(fragmentShader9, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader9, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    unsigned int shaderProgram9 = glCreateProgram();
    glAttachShader(shaderProgram9, vertexShader);
    glAttachShader(shaderProgram9, fragmentShader9);
    glLinkProgram(shaderProgram9);
    // check for linking errors
    glGetProgramiv(shaderProgram9, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram9, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }



    //fragment shader 10
    unsigned int fragmentShader10 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader10, 1, &fragmentShaderSource10, NULL);
    glCompileShader(fragmentShader10);
    // check for shader compile errors
    glGetShaderiv(fragmentShader10, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader10, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    unsigned int shaderProgram10 = glCreateProgram();
    glAttachShader(shaderProgram10, vertexShader);
    glAttachShader(shaderProgram10, fragmentShader10);
    glLinkProgram(shaderProgram10);
    // check for linking errors
    glGetProgramiv(shaderProgram10, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram10, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }









    //fragment shader 11
    unsigned int fragmentShader11 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader11, 1, &fragmentShaderSource11, NULL);
    glCompileShader(fragmentShader11);
    // check for shader compile errors
    glGetShaderiv(fragmentShader11, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader11, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    unsigned int shaderProgram11 = glCreateProgram();
    glAttachShader(shaderProgram11, vertexShader);
    glAttachShader(shaderProgram11, fragmentShader11);
    glLinkProgram(shaderProgram11);
    // check for linking errors
    glGetProgramiv(shaderProgram11, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram11, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }












    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(fragmentShader2);
    glDeleteShader(fragmentShader3);
    glDeleteShader(fragmentShader4);
    glDeleteShader(fragmentShader5);
    glDeleteShader(fragmentShader6);
    glDeleteShader(fragmentShader7);
    glDeleteShader(fragmentShader8);
    glDeleteShader(fragmentShader9);
    glDeleteShader(fragmentShader10);
    glDeleteShader(fragmentShader11);





    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    //lines, line_strip, line_loop, triangles, triangle_strip
    /*float vertices[] = {
        -0.75f, 0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        -0.25f, 0.5f, 0.0f,
         0.0f, -0.5f, 0.0f,
         0.25f, 0.5f, 0.0f,
         0.5f, -0.5f, 0.0f
    };*/

    //triangle_fan
    /*float vertices[] = {
        0.0f, 0.0f, 0.0f,
        0.5f, 0.0f, 0.0f,
        0.33f, 0.33f, 0.0f,
         0.0f, 0.5f, 0.0f,
        -0.33f, 0.33f, 0.0f,
        -0.5f, 0.0f, 0.0f
        //-0.33f, -0.33f, 0.0f,
        //0.0f, -0.5f, 0.0f,
       // 0.33f, -0.33f, 0.0f
    };*/

    //triangle
    float vertices[] = {
        //-0.5f, -0.5f, 0.0f, // left
        // 0.5f, -0.5f, 0.0f, // right
        // 0.0f,  0.5f, 0.0f  // top
        -0.746606711149792f,-0.428561584840655f,0.0f,
        -0.638153792872355f,-0.595443583118002f,0.0f,
        -0.481377762485297f,-0.756942291128338f,0.0f,
        -0.129611368609486f,-0.759095607235142f,0.0f,
        0.318644763937334f,-0.759095607235142f,0.0f,
        0.742711229253015f,-0.756403962101637f,0.0f,
        0.682581423085198f,-0.622718920470859f,0.0f,
        0.500022758808356f,-0.577858168245765f,0.0f,
        0.18944111233301f,-0.515053115130634f,0.0f,
        -0.307517633203871f,-0.447761986792994f,0.0f,
        -0.471780606886563f,-0.434303761125467f,0.0f,


        //lower extra portion
        0.74347803967399f,-0.758572231792516f,0.0f,
        -0.481669494026312f,-0.758946071394392f,0.0f,
        -0.570705788553478f,-0.667729208536702f,0.0f,







        /*0.741369701449312f,-0.756703033783137f,0.0f,
        -0.481669494026312f,-0.758946071394392f,0.0f,
        -0.572804365953042f,-0.667729208536702f,0.0f,*/




        /*- 0.5318073387411739f, -0.7061599196095323f,0.0f,
        0.49289657560894484f, -0.756403962101637f,0.0f,
        -0.47915510547688167f, -0.7581983921906408f,0.0f,*/




        //upper rails

        -0.453148363042473f,-0.433705617762465f,0.0f,
        -0.410302245120961f,-0.440434730596229f,0.0f,
        -0.366640122219898f,-0.442677768207484f,0.0f,
        -0.325480674878128f,-0.447911522633745f,0.0f,
        -0.282650174276799f,-0.451649918652502f,0.0f,
        -0.222176006198951f,-0.458379031486267f,0.0f,
        -0.135671669706421f,-0.466603502727534f,0.0f,
        -0.0373996824561518f,-0.477818690783807f,0.0f,
        0.0306566945704452f,-0.489781558043832f,0.0f,
        0.105428519275646f,-0.501744425303857f,0.0f,
        0.19365466531833f,-0.518193367786391f,0.0f,
        0.239862411409189f,-0.525670159823906f,0.0f,
        0.309605459015529f,-0.539128385491435f,0.0f,
        0.373460776912953f,-0.550343573547708f,0.0f,
        0.436492281170734f,-0.564549478418987f,0.0f,
        0.490278331880298f,-0.576512345679012f,0.0f,
        0.561715858726471f,-0.592961288161546f,0.0f,
        0.620565825505308f,-0.610157909847832f,0.0f,
        0.681906754853301f,-0.622120777107857f,0.0f,
        0.65479508701596f,-0.574269308067758f,0.0f,
        0.550549474795769f,-0.544362139917695f,0.0f,
        0.425294662599666f,-0.509968896545124f,0.0f,
        0.3143531243511f,-0.483800124413819f,0.0f,
        0.217717051389979f,-0.464360465116279f,0.0f,
        0.116040488339859f,-0.444173126614987f,0.0f,
        0.0605911930308274f,-0.435200976169968f,0.0f,
        0.0302896875461494f,-0.419499712891186f,0.0f,
        -4.30525788941427E-05f,-0.397817015982391f,0.0f,
        -0.0236447277051506f,-0.379125035888602f,0.0f,
        -0.0505611790402067f,-0.36865752703608f,0.0f,
        -0.0774424914048513f,-0.364919131017322f,0.0f,
        -0.111019729797871f,-0.364919131017322f,0.0f,
        -0.14963745827989f,-0.364171451813571f,0.0f,
        -0.189937953011606f,-0.362676093406068f,0.0f,
        -0.23106616571301f,-0.363423772609819f,0.0f,
        -0.273877144664111f,-0.363423772609819f,0.0f,
        -0.313326495445864f,-0.364171451813571f,0.0f,
        -0.330111210312328f,-0.364919131017322f,0.0f,
        -0.341008195469877f,-0.367909847832328f,0.0f,
        -0.351057941007509f,-0.372395923054838f,0.0f,
        -0.361943213174921f,-0.377629677481099f,0.0f,
        -0.371145719092636f,-0.383611111111111f,0.0f,
        -0.379512698380571f,-0.388844865537372f,0.0f,
        -0.383686427199424f,-0.393330940759881f,0.0f,
        -0.389542922267974f,-0.397069336778639f,0.0f,
        -0.39370493809669f,-0.403798449612403f,0.0f,
        -0.399569241825332f,-0.406041487223658f,0.0f,
        -0.405410119573699f,-0.412770600057422f,0.0f,
        -0.4129454765619f,-0.416508996076179f,0.0f,
        -0.41880197163045f,-0.420247392094937f,0.0f,
        -0.426341232948697f,-0.423238108909943f,0.0f,
        -0.436387074156283f,-0.428471863336204f,0.0f,
        -0.44476186210431f,-0.432210259354962f,0.0f,




        //rod 
        -0.00510696864816695f,-0.39258326155613f,0.0f,
        0.0252374844670135f,-0.416508996076179f,0.0f,
        0.0208958694561952f,0.575661307302134f,0.0f,
        0.0175069109765275f,0.581642740932146f,0.0f,
        0.0149573834566854f,0.587624174562159f,0.0f,
        0.0107446113373751f,0.590614891377165f,0.0f,
        0.00401745066863401f,0.592857928988419f,0.0f,
        -0.00102694375041056f,0.594353287395923f,0.0f,
        -0.00522409854953809f,0.594353287395923f,0.0f,
        -0.00858182238884008f,0.594353287395923f,0.0f,
        -0.0127789771879676f,0.594353287395923f,0.0f,
        -0.0152933657373983f,0.593605608192171f,0.0f,
        -0.0186432809166089f,0.592110249784668f,0.0f,
        -0.0261903508949471f,0.590614891377165f,0.0f,
        -0.0320390373034058f,0.585381136950904f,0.0f,
        -0.0378838193818187f,0.579399703320892f,0.0f,
        -0.0378564890714988f,0.574165948894631f,0.0f,
        -0.0378330630912245f,0.569679873672121f,0.0f,
        -0.0329057985735511f,-0.373891281462341f,0.0f,





        //rod-tip
        -0.00998738120529196f, 0.75635044820876f, 0.0f,
        0.00609520497462068f, 0.757596580215012f, 0.0f,
        0.00626764621830588f, 0.590614891377165f, 0.0f,
        0.000661678994354875f, 0.592484089386544f, 0.0f,
        -0.00284245722166087f, 0.593730221392796f, 0.0f,
        -0.00634008622093384f, 0.593730221392796f, 0.0f,
        -0.0112367668199159f, 0.593730221392796f, 0.0f,
        -0.017522738193493f, 0.591861023383417f, 0.0f,
        -0.0189120289680878f, 0.589991825374038f, 0.0f,
        -0.0237989487419557f, 0.58812262736466f, 0.0f,
        -0.0237989487419557f, 0.58812262736466f, 0.0f,
        -0.0246774230022382f, 0.75635044820876f, 0.0f,



        //flag
        0.00647913076244788f, 0.684074791846109f, 0.0f,
        0.00647913076244788f, 0.684074791846109f, 0.0f,
        0.00679147716610395f, 0.624260455545985f, 0.0f,
        0.0141026966887186f, 0.623911423201641f, 0.0f,
        0.0181489955256251f, 0.624271993805302f, 0.0f,
        0.0230068135801708f, 0.624271993805302f, 0.0f,
        0.0258405407786558f, 0.624271993805302f, 0.0f,
        0.0294839043195651f, 0.624271993805302f, 0.0f,
        0.0315079951756258f, 0.624271993805302f, 0.0f,
        0.0323138657676204f, 0.624993135012624f, 0.0f,
        0.0343379566236811f, 0.624993135012624f, 0.0f,
        0.0363620474797418f, 0.624993135012624f, 0.0f,
        0.0383861383358025f, 0.624993135012624f, 0.0f,
        0.0420295018767118f, 0.624993135012624f, 0.0f,
        0.0468797884303977f, 0.626435417427268f, 0.0f,
        0.0529464123729351f, 0.627517129238251f, 0.0f,
        0.0578004646770511f, 0.628238270445573f, 0.0f,
        0.0578004646770511f, 0.628238270445573f, 0.0f,
        0.0618429977635277f, 0.629319982256557f, 0.0f,
        0.062652634105952f, 0.629319982256557f, 0.0f,
        0.0674991549092081f, 0.631483405878523f, 0.0f,
        0.0683069083764175f, 0.631843976482184f, 0.0f,
        0.0731609606805333f, 0.632565117689506f, 0.0f,
        0.079630519919068f, 0.63400740010415f, 0.0f,
        0.0816489621494839f, 0.635089111915134f, 0.0f,
        0.0864973658279548f, 0.636891964933439f, 0.0f,
        0.0909390684599987f, 0.639055388555405f, 0.0f,
        0.0957837063880401f, 0.641579382781032f, 0.0f,
        0.100227291895299f, 0.643382235799338f, 0.0f,
        0.104269824981775f, 0.644463947610321f, 0.0f,
        0.104269824981775f, 0.644463947610321f, 0.0f,
        0.108715293364249f, 0.645906230024965f, 0.0f,
        0.111948190108301f, 0.646987941835948f, 0.0f,
        0.114774385805927f, 0.648430224250592f, 0.0f,
        0.11719952908277f, 0.649151365457915f, 0.0f,
        0.12003137340604f, 0.649511936061576f, 0.0f,
        0.122857569103665f, 0.65095421847622f, 0.0f,
        0.125681881926075f, 0.652757071494525f, 0.0f,
        0.125681881926075f, 0.652757071494525f, 0.0f,
        0.129722532137337f, 0.654199353909169f, 0.0f,
        0.133356481302172f, 0.656002206927474f, 0.0f,
        0.136184559875012f, 0.657083918738458f, 0.0f,
        0.141838834145477f, 0.659607912964085f, 0.0f,
        0.14708829024473f, 0.662131907189712f, 0.0f,
        0.151533758627204f, 0.663574189604357f, 0.0f,
        0.158811071332948f, 0.665377042622662f, 0.0f,
        0.16567603436662f, 0.668622178055611f, 0.0f,
        0.170933021966733f, 0.669703889866594f, 0.0f,
        0.178611387093259f, 0.672227884092222f, 0.0f,
        0.18427130998937f, 0.673670166506866f, 0.0f,
        0.191143804523902f, 0.675473019525171f, 0.0f,
        0.191143804523902f, 0.675473019525171f, 0.0f,
        0.197613363762436f, 0.676915301939815f, 0.0f,
        0.206108896732246f, 0.677997013750799f, 0.0f,
        0.215415948919696f, 0.678718154958121f, 0.0f,
        0.223107494172727f, 0.678718154958121f, 0.0f,
        0.231604910017752f, 0.679439296165443f, 0.0f,
        0.239294572395568f, 0.679799866769104f, 0.0f,
        0.244958261042108f, 0.680521007976426f, 0.0f,
        0.225493097070056f, 0.687011278842325f, 0.0f,
        0.22143550098186f, 0.68881413186063f, 0.0f,
        0.216169099005672f, 0.689535273067952f, 0.0f,
        0.213333488931972f, 0.689895843671613f, 0.0f,
        0.209272127093346f, 0.692419837897241f, 0.0f,
        0.204807829958723f, 0.694583261519207f, 0.0f,
        0.201974102760238f, 0.694583261519207f, 0.0f,
        0.197114401830477f, 0.694943832122868f, 0.0f,
        0.194273143131132f, 0.696386114537512f, 0.0f,
        0.186577832127671f, 0.697107255744834f, 0.0f,
        0.180098858513062f, 0.697467826348495f, 0.0f,
        0.173614236272808f, 0.69891010876314f, 0.0f,
        0.168752652467833f, 0.699631249970462f, 0.0f,
        0.159439951654738f, 0.699991820574123f, 0.0f,
        0.150533951888071f, 0.699991820574123f, 0.0f,
        0.147696458939156f, 0.700712961781445f, 0.0f,
        0.143243459055823f, 0.700712961781445f, 0.0f,
        0.135551913802792f, 0.700712961781445f, 0.0f,
        0.133527822946731f, 0.700712961781445f, 0.0f,
        0.132718186604307f, 0.700712961781445f, 0.0f,
        0.128265186720973f, 0.700712961781445f, 0.0f,
        0.125431459522488f, 0.700712961781445f, 0.0f,
        0.116930277927033f, 0.700712961781445f, 0.0f,
        0.111262823530063f, 0.700712961781445f, 0.0f,
        0.105595369133093f, 0.700712961781445f, 0.0f,
        0.102356823763396f, 0.700712961781445f, 0.0f,
        0.0966893693664262f, 0.700712961781445f, 0.0f,
        0.0893988765341777f, 0.701434102988767f, 0.0f,
        0.0825169676235713f, 0.701434102988767f, 0.0f,
        0.0752264747913227f, 0.702155244196089f, 0.0f,
        0.0711764102039863f, 0.70251581479975f, 0.0f,
        0.0638859173717378f, 0.703236956007072f, 0.0f,
        0.0598283212835416f, 0.705039809025377f, 0.0f,
        0.0513158424367969f, 0.707203232647344f, 0.0f,
        0.0448293373213279f, 0.709006085665649f, 0.0f,
        0.0448293373213279f, 0.709006085665649f, 0.0f,
        0.0403669230619195f, 0.710808938683954f, 0.0f,
        0.0326621976823843f, 0.713332932909582f, 0.0f,
        0.0269815631589096f, 0.715856927135209f, 0.0f,
        0.01846343568652f, 0.719102062568158f, 0.0f,
        0.014810657769536f, 0.720904915586464f, 0.0f,
        0.0119675161949763f, 0.722707768604769f, 0.0f,
        0.00587641487464463f, 0.726313474641379f, 0.0f,





        //right-sail
        0.0241006737020407f, -0.279185248987144f, 0.0f,
        0.0241006737020407f, -0.279185248987144f, 0.0f,
        0.442830310278254f, -0.358314631384184f, 0.0f,
        0.0210552962663946f, 0.571922911283376f, 0.0f,







        //left sail
        -0.0356685025138885f, 0.540968992248062f, 0.0f,
        -0.0368163755473243f, 0.567885443583118f, 0.0f,
        -0.658598114377254f, -0.345479471719782f, 0.0f,
        -0.598224678014586f, -0.332918461096755f, 0.0f,
        -0.546903040429868f, -0.323049095607235f, 0.0f,
        -0.489532814738352f, -0.314076945162216f, 0.0f,
        -0.444236339280158f, -0.307796439850703f, 0.0f,
        -0.407996347795971f, -0.303310364628194f, 0.0f,
        -0.369741722008202f, -0.298824289405685f, 0.0f,
        -0.333497045327959f, -0.295235429227677f, 0.0f,
        -0.296240366299871f, -0.292543784094172f, 0.0f,
        -0.256978423360312f, -0.288057708871662f, 0.0f,
        -0.218709741984378f, -0.286263278782658f, 0.0f,
        -0.176416477197338f, -0.283571633649153f, 0.0f,
        -0.137140478669613f, -0.281777203560149f, 0.0f,
        -0.105913646964105f, -0.281777203560149f, 0.0f,
        -0.0706716022395984f, -0.279085558426643f, 0.0f,
        -0.0333961824272911f, -0.279982773471145f, 0.0f,



        //sail strip
        -0.38123709826492f, 0.0703547787667078f, 0.0f,
        -0.384757290385519f, 0.0661481217239942f, 0.0f,
        -0.456475851671574f, -0.0420937312126372f, 0.0f,
        -0.452259133798963f, -0.0406201993456758f, 0.0f,
        -0.443515061300781f, -0.0406201993456758f, 0.0f,
        -0.438657243246235f, -0.0406201993456758f, 0.0f,
        -0.433313643386235f, -0.0406201993456758f, 0.0f,
        -0.430884734358962f, -0.0406201993456758f, 0.0f,
        -0.428941607137143f, -0.0406201993456758f, 0.0f,
        -0.42748426172078f, -0.0406201993456758f, 0.0f,
        -0.425053093243249f, -0.041052884070069f, 0.0f,
        -0.418252147966885f, -0.041052884070069f, 0.0f,
        -0.409508075468702f, -0.041052884070069f, 0.0f,
        -0.402707130192338f, -0.041052884070069f, 0.0f,
        -0.396873229626368f, -0.0419182535188555f, 0.0f,
        -0.389586502544549f, -0.0419182535188555f, 0.0f,
        -0.381328211851821f, -0.0419182535188555f, 0.0f,
        -0.374043744220261f, -0.0414855687944623f, 0.0f,
        -0.365783194077275f, -0.0419182535188555f, 0.0f,
        -0.357520384484031f, -0.0427836229676421f, 0.0f,
        -0.350226879051439f, -0.0440816771408217f, 0.0f,
        -0.340511242942347f, -0.0440816771408217f, 0.0f,
        -0.332248433349104f, -0.0449470465896083f, 0.0f,
        -0.323499841950405f, -0.0458124160383949f, 0.0f,
        -0.314263209295995f, -0.0471104702115746f, 0.0f,
        -0.309403131791191f, -0.0475431549359679f, 0.0f,
        -0.309403131791191f, -0.0475431549359679f, 0.0f,
        -0.300659059293009f, -0.0475431549359679f, 0.0f,
        -0.293851335665871f, -0.0488412091091476f, 0.0f,
        -0.283645398850809f, -0.0497065785579342f, 0.0f,
        -0.273443980936263f, -0.0497065785579342f, 0.0f,
        -0.261773920354063f, -0.0518700021799005f, 0.0f,
        -0.249618077966409f, -0.0540334258018668f, 0.0f,
        -0.23843831809018f, -0.0553314799750467f, 0.0f,
        -0.23843831809018f, -0.0553314799750467f, 0.0f,
        -0.224831908636936f, -0.0561968494238331f, 0.0f,
        -0.210240378871236f, -0.0596583272189792f, 0.0f,
        -0.199539622449687f, -0.0622544355653388f, 0.0f,
        -0.18592643464567f, -0.0644178591873051f, 0.0f,
        -0.173770592258016f, -0.0665812828092714f, 0.0f,
        -0.156745634564526f, -0.0709081300532039f, 0.0f,
        -0.146040359242462f, -0.0743696078483501f, 0.0f,
        -0.135825384626368f, -0.0769657161947097f, 0.0f,
        -0.122200899571061f, -0.0812925634386423f, 0.0f,
        -0.111976887153936f, -0.0856194106825749f, 0.0f,
        -0.0983569209991446f, -0.089080888477721f, 0.0f,
        -0.0852091799482601f, -0.0951384746192267f, 0.0f,
        -0.0735187843137392f, -0.101196060760732f, 0.0f,
        -0.0589182167470069f, -0.106388277453451f, 0.0f,
        -0.0442927952274372f, -0.116340026114496f, 0.0f,
        -0.0345613429665403f, -0.119368819185249f, 0.0f,
        -0.0350674598243162f, -0.0224474409211588f, 0.0f,
        -0.0443357247823379f, -0.0150918006064734f, 0.0f,
        -0.0506712233055688f, -0.0111976380869339f, 0.0f,
        -0.056520940023345f, -0.00730347556739463f, 0.0f,
        -0.0686971174633205f, -0.00124588942588899f, 0.0f,
        -0.0837992829873129f, 0.00697512033758296f, 0.0f,
        -0.0993759330654704f, 0.0130327064790886f, 0.0f,
        -0.116418966361023f, 0.0208210315181673f, 0.0f,
        -0.133464259106833f, 0.0290420412816392f, 0.0f,
        -0.150498254601355f, 0.0350996274231449f, 0.0f,
        -0.160713229217448f, 0.0376957357695045f, 0.0f,
        -0.173370669562362f, 0.0428879524622236f, 0.0f,
        -0.186999673518186f, 0.0480801691549428f, 0.0f,
        -0.203050808150508f, 0.0519743316744821f, 0.0f,
        -0.21765137571724f, 0.0571665483672012f, 0.0f,
        -0.235645637571381f, 0.0610607108867407f, 0.0f,
        -0.250720689692278f, 0.0640895039574934f, 0.0f,
        -0.266769564874343f, 0.0675509817526395f, 0.0f,
        -0.279415707967967f, 0.0705797748233924f, 0.0f,
        -0.293516937027697f, 0.0731758831697519f, 0.0f,
        -0.308101688442624f, 0.0753393067917183f, 0.0f,
        -0.323176740563522f, 0.0783680998624711f, 0.0f,
        -0.334842282245205f, 0.0796661540356508f, 0.0f,
        -0.346995865182601f, 0.0813968929332238f, 0.0f,
        -0.362059620052209f, 0.0822622623820104f, 0.0f,
        -0.37234947033935f, 0.0836925257765325f, 0.0f,





        //window-reqt
        -0.316233449920908f, -0.41402711649706f, 0.0f,
        -0.315779300419065f, -0.407969530355554f, 0.0f,
        -0.31484614746254f, -0.400613890040869f, 0.0f,
        -0.311475047677711f, -0.394988988623756f, 0.0f,
        -0.307609128286396f, -0.391094826104217f, 0.0f,
        -0.300813831635677f, -0.390013114293234f, 0.0f,
        -0.28526881386113f, -0.390013114293234f, 0.0f,
        -0.213860018184437f, -0.389796771931037f, 0.0f,
        -0.209002200129891f, -0.389796771931037f, 0.0f,
        -0.2046301638808f, -0.389796771931037f, 0.0f,
        -0.202673479957434f, -0.392392880277397f, 0.0f,
        -0.201195799488749f, -0.396287042796936f, 0.0f,
        -0.201179983336943f, -0.399315835867689f, 0.0f,
        -0.20052926166266f, -0.430901820748397f, 0.0f,
        -0.201485009121764f, -0.43393061381915f, 0.0f,
        -0.205852526470339f, -0.434795983267936f, 0.0f,
        -0.209738780913976f, -0.434795983267936f, 0.0f,
        -0.21507786187346f, -0.435661352716723f, 0.0f,
        -0.257340878948008f, -0.435661352716723f, 0.0f,
        -0.301547023244374f, -0.435661352716723f, 0.0f,
        -0.304947495882556f, -0.435661352716723f, 0.0f,
        -0.307862186715284f, -0.435661352716723f, 0.0f,
        -0.31126943770424f, -0.434363298543543f, 0.0f,
        -0.311755219509694f, -0.434363298543543f, 0.0f,
        -0.314681207593711f, -0.432199874921577f, 0.0f,
        -0.314701542646033f, -0.428305712402037f, 0.0f,
        -0.315700219660037f, -0.423113495709318f, 0.0f








    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);


    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // create transformations
        /*glm::mat4 trans = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        trans = glm::translate(trans, glm::vec3(translate_X, translate_Y, 0.0f));
        trans = glm::rotate(trans, glm:: radians(rotateAngle), glm::vec3(0.0f, 0.0f, 1.0f));
        trans = glm::scale(trans,glm::vec3(scale_X, scale_Y, 1.0));*/
        glm::mat4 translationMatrix;
        glm::mat4 rotationMatrix;
        glm::mat4 scaleMatrix;
        glm::mat4 modelMatrix;
        glm::mat4 identityMatrix = glm::mat4(1.0f);
        translationMatrix = glm::translate(identityMatrix, glm::vec3(translate_X, translate_Y, 0.0f));
        rotationMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X, scale_Y, 1.0f));
        modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;
        //modelMatrix = rotationMatrix * scaleMatrix;

        // get matrix's uniform location and set matrix
        glUseProgram(shaderProgram);
        unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

        // draw our first triangle
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        //glDrawArrays(GL_LINES, 0, 6);
        //glDrawArrays(GL_LINE_STRIP, 0, 6);
        //glDrawArrays(GL_LINE_LOOP, 0, 6);
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        //glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);
        //glDrawArrays(GL_TRIANGLE_FAN, 0, 11);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        // glBindVertexArray(0); // no need to unbind it every time



        //lower body
        glUseProgram(shaderProgram2);
        transformLoc = glGetUniformLocation(shaderProgram2, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

        glDrawArrays(GL_TRIANGLE_FAN, 0, 11);


        //lower body extra
        glUseProgram(shaderProgram3);
        transformLoc = glGetUniformLocation(shaderProgram3, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

        glDrawArrays(GL_TRIANGLE_FAN, 11, 3);


        //upper rail
        glUseProgram(shaderProgram4);
        transformLoc = glGetUniformLocation(shaderProgram4, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

        glDrawArrays(GL_TRIANGLE_FAN, 14, 53);




        //rod
        glUseProgram(shaderProgram5);
        transformLoc = glGetUniformLocation(shaderProgram5, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

        glDrawArrays(GL_TRIANGLE_FAN, 67, 19);





        //rod-tip
        glUseProgram(shaderProgram6);
        transformLoc = glGetUniformLocation(shaderProgram6, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

        glDrawArrays(GL_TRIANGLE_FAN, 86, 12);



        //flag
        glUseProgram(shaderProgram7);
        transformLoc = glGetUniformLocation(shaderProgram7, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

        glDrawArrays(GL_TRIANGLE_FAN, 98, 102);





        //right-sail
        glUseProgram(shaderProgram8);
        transformLoc = glGetUniformLocation(shaderProgram8, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

        glDrawArrays(GL_TRIANGLE_FAN, 200, 4);





        //left-sail
        glUseProgram(shaderProgram9);
        transformLoc = glGetUniformLocation(shaderProgram9, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

        glDrawArrays(GL_TRIANGLE_FAN, 204, 18);



        //sail-strip
        glUseProgram(shaderProgram10);
        transformLoc = glGetUniformLocation(shaderProgram10, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

        glDrawArrays(GL_TRIANGLE_FAN, 222, 77);


        //window-reqt
        glUseProgram(shaderProgram11);
        transformLoc = glGetUniformLocation(shaderProgram11, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

        glDrawArrays(GL_TRIANGLE_FAN, 299, 27);




        //window-square
        glUseProgram(shaderProgram11);
        transformLoc = glGetUniformLocation(shaderProgram11, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

        glDrawArrays(GL_TRIANGLE_FAN, 299, 22);










        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        rotateAngle += 1;
    }
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
    {
        rotateAngle -= 1;
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        translate_Y += 0.01;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        translate_Y -= 0.01;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        translate_X += 0.01;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        translate_X -= 0.01;
    }
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
    {
        scale_X += 0.01;
    }
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
    {
        scale_X -= 0.01;
    }
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
    {
        scale_Y += 0.01;
    }
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
    {
        scale_Y -= 0.01;
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
