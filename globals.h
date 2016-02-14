#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <FTGL/ftgl.h>
#include <SOIL/SOIL.h>



using namespace std;

GLuint textureID[100];


 

struct VAO {
  GLuint VertexArrayID;
  GLuint VertexBuffer;
  GLuint ColorBuffer;
  GLuint TextureBuffer;
  GLuint TextureID;

  GLenum PrimitiveMode; // GL_POINTS, GL_LINE_STRIP, GL_LINE_LOOP, GL_LINES, GL_LINE_STRIP_ADJACENCY, GL_LINES_ADJACENCY, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_TRIANGLES, GL_TRIANGLE_STRIP_ADJACENCY and GL_TRIANGLES_ADJACENCY
  GLenum FillMode; // GL_FILL, GL_LINE
  int NumVertices;
};
typedef struct VAO VAO;

struct GLMatrices {
  glm::mat4 projection;
  glm::mat4 model;
  glm::mat4 view;
  GLuint MatrixID; // For use with normal shader
  GLuint TexMatrixID; // For use with texture shader
} Matrices;

struct FTGLFont {
  FTFont* font;
  GLuint fontMatrixID;
  GLuint fontColorID;
} GL3Font;

static const GLfloat texture_buffer_data [] = {
  0,1, // TexCoord 1 - bot left
  1,1, // TexCoord 2 - bot right
  1,0, // TexCoord 3 - top right

  1,0, // TexCoord 3 - top right
  0,0, // TexCoord 4 - top left
  0,1  // TexCoord 1 - bot left
};

static const GLfloat texture_buffer_data_triangle [] = {
  0,1, // TexCoord 1 - bot left
  1,1, // TexCoord 2 - bot right
  1,0 // TexCoord 3 - top right
};

GLuint programID, fontProgramID, textureProgramID;
float camera_rotation_angle = 90;
float rectangle_rotation = 0;
float triangle_rotation = 0;
float  gravity = 5;
float groundLevel = -18;


float ball_ix = -6.5f;
float ball_iy = -5.0f;
float ball_x = -6.5f;
float ball_y = -5.0f;
float ball_vx = 1.0f;
float ball_vy = 0.0f;
float boost = 10.0;
float coeff = 0.8;
float zoom_scale = 20;
float zoom_flag = 0;
float panx = -10;
float scale=1.0;
float canonX=-26, canonY=-17;


int points = 0, shoot = 0, magic = 0;

bool onMenu = true, hit[3], onMenu2 = false;
bool lostLife = false;
double xpos, ypos, prevXpos, prevYpos;

float triangle_rot_dir = 1;
float rectangle_rot_dir = 1;
float ball_rot_dir = 1;
bool triangle_rot_status = true;
bool rectangle_rot_status = false;
bool ball_rot_status = false;
bool ball_trans_stat = false;
int speed_inc = 0, lives = 3;

/* Function to load Shaders - Use it as it is */
GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path) {

  // Create the shaders
  GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
  GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

  // Read the Vertex Shader code from the file
  std::string VertexShaderCode;
  std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
  if(VertexShaderStream.is_open())
  {
    std::string Line = "";
    while(getline(VertexShaderStream, Line))
      VertexShaderCode += "\n" + Line;
    VertexShaderStream.close();
  }

  // Read the Fragment Shader code from the file
  std::string FragmentShaderCode;
  std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
  if(FragmentShaderStream.is_open()){
    std::string Line = "";
    while(getline(FragmentShaderStream, Line))
      FragmentShaderCode += "\n" + Line;
    FragmentShaderStream.close();
  }

  GLint Result = GL_FALSE;
  int InfoLogLength;

  // Compile Vertex Shader
  cout << "Compiling shader : " <<  vertex_file_path << endl;
  char const * VertexSourcePointer = VertexShaderCode.c_str();
  glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
  glCompileShader(VertexShaderID);

  // Check Vertex Shader
  glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
  glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
  std::vector<char> VertexShaderErrorMessage( max(InfoLogLength, int(1)) );
  glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
  cout << VertexShaderErrorMessage.data() << endl;

  // Compile Fragment Shader
  cout << "Compiling shader : " << fragment_file_path << endl;
  char const * FragmentSourcePointer = FragmentShaderCode.c_str();
  glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
  glCompileShader(FragmentShaderID);

  // Check Fragment Shader
  glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
  glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
  std::vector<char> FragmentShaderErrorMessage( max(InfoLogLength, int(1)) );
  glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
  cout << FragmentShaderErrorMessage.data() << endl;

  // Link the program
  cout << "Linking program" << endl;
  GLuint ProgramID = glCreateProgram();
  glAttachShader(ProgramID, VertexShaderID);
  glAttachShader(ProgramID, FragmentShaderID);
  glLinkProgram(ProgramID);

  // Check the program
  glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
  glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
  std::vector<char> ProgramErrorMessage( max(InfoLogLength, int(1)) );
  glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
  cout << ProgramErrorMessage.data() << endl;

  glDeleteShader(VertexShaderID);
  glDeleteShader(FragmentShaderID);

  return ProgramID;
}

static void error_callback(int error, const char* description)
{
  fprintf(stderr, "Error: %s\n", description);
}

void quit(GLFWwindow *window)
{
  glfwDestroyWindow(window);
  glfwTerminate();
  exit(EXIT_SUCCESS);
}

glm::vec3 getRGBfromHue (int hue)
{
  float intp;
  float fracp = modff(hue/60.0, &intp);
  float x = 1.0 - abs((float)((int)intp%2)+fracp-1.0);

  if (hue < 60)
    return glm::vec3(1,x,0);
  else if (hue < 120)
    return glm::vec3(x,1,0);
  else if (hue < 180)
    return glm::vec3(0,1,x);
  else if (hue < 240)
    return glm::vec3(0,x,1);
  else if (hue < 300)
    return glm::vec3(x,0,1);
  else
    return glm::vec3(1,0,x);
}

/* Generate VAO, VBOs and return VAO handle */
struct VAO* create3DObject (GLenum primitive_mode, int numVertices, const GLfloat* vertex_buffer_data, const GLfloat* color_buffer_data, GLenum fill_mode=GL_FILL)
{
  struct VAO* vao = new struct VAO;
  vao->PrimitiveMode = primitive_mode;
  vao->NumVertices = numVertices;
  vao->FillMode = fill_mode;

  // Create Vertex Array Object
  // Should be done after CreateWindow and before any other GL calls
  glGenVertexArrays(1, &(vao->VertexArrayID)); // VAO
  glGenBuffers (1, &(vao->VertexBuffer)); // VBO - vertices
  glGenBuffers (1, &(vao->ColorBuffer));  // VBO - colors

  glBindVertexArray (vao->VertexArrayID); // Bind the VAO 
  glBindBuffer (GL_ARRAY_BUFFER, vao->VertexBuffer); // Bind the VBO vertices 
  glBufferData (GL_ARRAY_BUFFER, 3*numVertices*sizeof(GLfloat), vertex_buffer_data, GL_STATIC_DRAW); // Copy the vertices into VBO
  glVertexAttribPointer(
      0,                  // attribute 0. Vertices
      3,                  // size (x,y,z)
      GL_FLOAT,           // type
      GL_FALSE,           // normalized?
      0,                  // stride
      (void*)0            // array buffer offset
      );

  glBindBuffer (GL_ARRAY_BUFFER, vao->ColorBuffer); // Bind the VBO colors 
  glBufferData (GL_ARRAY_BUFFER, 3*numVertices*sizeof(GLfloat), color_buffer_data, GL_STATIC_DRAW);  // Copy the vertex colors
  glVertexAttribPointer(
      1,                  // attribute 1. Color
      3,                  // size (r,g,b)
      GL_FLOAT,           // type
      GL_FALSE,           // normalized?
      0,                  // stride
      (void*)0            // array buffer offset
      );

  return vao;
}

/* Generate VAO, VBOs and return VAO handle - Common Color for all vertices */
struct VAO* create3DObject (GLenum primitive_mode, int numVertices, const GLfloat* vertex_buffer_data, const GLfloat red, const GLfloat green, const GLfloat blue, GLenum fill_mode=GL_FILL)
{
  GLfloat* color_buffer_data = new GLfloat [2*numVertices];
  for (int i=0; i<numVertices; i++) {
    color_buffer_data [3*i] = red;
    color_buffer_data [3*i + 1] = green;
    color_buffer_data [3*i + 2] = blue;
  }

  return create3DObject(primitive_mode, numVertices, vertex_buffer_data, color_buffer_data, fill_mode);
}


struct VAO* create3DTexturedObject (GLenum primitive_mode, int numVertices, const GLfloat* vertex_buffer_data, const GLfloat* texture_buffer_data, GLuint textureID, GLenum fill_mode=GL_FILL)
{
  struct VAO* vao = new struct VAO;
  vao->PrimitiveMode = primitive_mode;
  vao->NumVertices = numVertices;
  vao->FillMode = fill_mode;
  vao->TextureID = textureID;

  // Create Vertex Array Object
  // Should be done after CreateWindow and before any other GL calls
  glGenVertexArrays(1, &(vao->VertexArrayID)); // VAO
  glGenBuffers (1, &(vao->VertexBuffer)); // VBO - vertices
  glGenBuffers (1, &(vao->TextureBuffer));  // VBO - textures

  glBindVertexArray (vao->VertexArrayID); // Bind the VAO
  glBindBuffer (GL_ARRAY_BUFFER, vao->VertexBuffer); // Bind the VBO vertices
  glBufferData (GL_ARRAY_BUFFER, 3*numVertices*sizeof(GLfloat), vertex_buffer_data, GL_STATIC_DRAW); // Copy the vertices into VBO
  glVertexAttribPointer(
      0,                  // attribute 0. Vertices
      3,                  // size (x,y,z)
      GL_FLOAT,           // type
      GL_FALSE,           // normalized?
      0,                  // stride
      (void*)0            // array buffer offset
      );

  glBindBuffer (GL_ARRAY_BUFFER, vao->TextureBuffer); // Bind the VBO textures
  glBufferData (GL_ARRAY_BUFFER, 2*numVertices*sizeof(GLfloat), texture_buffer_data, GL_STATIC_DRAW);  // Copy the vertex colors
  glVertexAttribPointer(
      2,                  // attribute 2. Textures
      2,                  // size (s,t)
      GL_FLOAT,           // type
      GL_FALSE,           // normalized?
      0,                  // stride
      (void*)0            // array buffer offset
      );

  return vao;
}

/* Render the VBOs handled by VAO */
void draw3DObject (struct VAO* vao)
{
  // Change the Fill Mode for this object
  glPolygonMode (GL_FRONT_AND_BACK, vao->FillMode);

  // Bind the VAO to use
  glBindVertexArray (vao->VertexArrayID);

  // Enable Vertex Attribute 0 - 3d Vertices
  glEnableVertexAttribArray(0);
  // Bind the VBO to use
  glBindBuffer(GL_ARRAY_BUFFER, vao->VertexBuffer);

  // Enable Vertex Attribute 1 - Color
  glEnableVertexAttribArray(1);
  // Bind the VBO to use
  glBindBuffer(GL_ARRAY_BUFFER, vao->ColorBuffer);

  // Draw the geometry !
  glDrawArrays(vao->PrimitiveMode, 0, vao->NumVertices); // Starting from vertex 0; 3 vertices total -> 1 triangle
}

void draw3DTexturedObject (struct VAO* vao)
{
  // Change the Fill Mode for this object
  glPolygonMode (GL_FRONT_AND_BACK, vao->FillMode);

  // Bind the VAO to use
  glBindVertexArray (vao->VertexArrayID);

  // Enable Vertex Attribute 0 - 3d Vertices
  glEnableVertexAttribArray(0);
  // Bind the VBO to use
  glBindBuffer(GL_ARRAY_BUFFER, vao->VertexBuffer);

  // Bind Textures using texture units
  glBindTexture(GL_TEXTURE_2D, vao->TextureID);

  // Enable Vertex Attribute 2 - Texture
  glEnableVertexAttribArray(2);
  // Bind the VBO to use
  glBindBuffer(GL_ARRAY_BUFFER, vao->TextureBuffer);

  // Draw the geometry !
  glDrawArrays(vao->PrimitiveMode, 0, vao->NumVertices); // Starting from vertex 0; 3 vertices total -> 1 triangle

  // Unbind Textures to be safe
  glBindTexture(GL_TEXTURE_2D, 0);
}

/* Create an OpenGL Texture from an image */
GLuint createTexture (const char* filename)
{
  GLuint TextureID;
  // Generate Texture Buffer
  glGenTextures(1, &TextureID);
  // All upcoming GL_TEXTURE_2D operations now have effect on our texture buffer
  glBindTexture(GL_TEXTURE_2D, TextureID);
  // Set our texture parameters
  // Set texture wrapping to GL_REPEAT
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // Set texture filtering (interpolation)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  // Load image and create OpenGL texture
  int twidth, theight;
  unsigned char* image = SOIL_load_image(filename, &twidth, &theight, 0, SOIL_LOAD_RGB);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, twidth, theight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
  glGenerateMipmap(GL_TEXTURE_2D); // Generate MipMaps to use
  SOIL_free_image_data(image); // Free the data read from file after creating opengl texture
  glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess it up

  return TextureID;
}

/* Executed when a regular key is pressed/released/held-down */
/* Prefered for Keyboard events */
void keyboard (GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (action == GLFW_RELEASE) {
    switch (key) {
      case GLFW_KEY_A:

        rectangle_rot_status = false;
        ball_rot_status = false;
        break;
      case GLFW_KEY_B:
        rectangle_rot_status = false;
        ball_rot_status = false;
        break;
      case GLFW_KEY_P:
        triangle_rot_status = !triangle_rot_status;
        break;
      case GLFW_KEY_SPACE:
        if(shoot == 0) shoot = 1;
        break;
      case GLFW_KEY_F:
        speed_inc = 0;
        break;
      case GLFW_KEY_S:
        speed_inc = 0;

      case GLFW_KEY_DOWN:
        zoom_flag = 0;
        break;
      case GLFW_KEY_UP:
        zoom_flag = 0;
        break;
      default:
        break;
    }
  }
  else if (action == GLFW_PRESS) {
    switch (key) {
      case GLFW_KEY_ESCAPE:
        quit(window);
        break;
      case GLFW_KEY_A:
        ball_rot_dir = 1;
        rectangle_rot_dir = 1;
        ball_rot_status = true;
        rectangle_rot_status = true;
        break;
      case GLFW_KEY_B:
        ball_rot_dir = -1;
        rectangle_rot_dir = -1;
        ball_rot_status = true;
        rectangle_rot_status = true;
        break;
      case GLFW_KEY_F:
        speed_inc = 1;
        break;
      case GLFW_KEY_S:
        speed_inc = -1;

        break;
      case GLFW_KEY_DOWN:
        zoom_flag = 0.5;
        break;
      case GLFW_KEY_UP:
        zoom_flag = -0.5;
        break;
      case GLFW_KEY_LEFT:
        if(panx>-10) panx -= 1;
        break;
      case GLFW_KEY_RIGHT:
        if(panx<10) panx += 1;
        break;
      default:
        break;
    }
  }
}

/* Executed for character input (like in text boxes) */
void keyboardChar (GLFWwindow* window, unsigned int key)
{
  switch (key) {
    case 'Q':
    case 'q':
      quit(window);
      break;
    default:
      break;
  }
}

void scrollCallback(GLFWwindow* window, double x, double y) {
  if(y > 0) zoom_flag = -0.5;
  else if(y < 0) zoom_flag = 0.5;
  else zoom_flag = 0;


}

/* Executed when a mouse button is pressed/released */
void mouseButton (GLFWwindow* window, int button, int action, int mods)
{
  switch (button) {
    case GLFW_MOUSE_BUTTON_LEFT:
      if(action == GLFW_PRESS) {
        glfwGetCursorPos(window, &xpos, &ypos);
        prevXpos = xpos;
        prevYpos = ypos;

      }
      if (action == GLFW_RELEASE) {
        glfwGetCursorPos(window, &xpos, &ypos);
        if(xpos > prevXpos) {
          speed_inc = 1;

        }
        else if(xpos<prevXpos) {
          speed_inc = -1;
        }
        else speed_inc = 0;
        xpos = zoom_scale*(xpos/(1024.0/2.0) - 1)-10;
        ypos = -(zoom_scale*(ypos/(715.0/2.0) - 1));
        if(xpos>=-19&&xpos<=-7&&ypos>=-17&&ypos<=-3&&onMenu) {
          onMenu = false;
        }
      
        rectangle_rotation = atan2(((float)ypos -canonY),((float)xpos-canonX))*180.0/M_PI;
        
      }
      break;
    case GLFW_MOUSE_BUTTON_RIGHT:
      if(action == GLFW_PRESS) {
        glfwGetCursorPos(window, &xpos, &ypos);
        prevXpos = xpos;
        prevYpos = ypos;
      }
      if (action == GLFW_RELEASE) {

        glfwGetCursorPos(window, &xpos, &ypos);
        //  cout<<xpos<<" "<<prevXpos<<endl;
        if(xpos > prevXpos) {
          if(panx>-zoom_scale) panx -= 1;
        }
        else if(xpos < prevXpos) {
          if(panx < zoom_scale) panx += 1;
        }
      }
      break;
    default:
      break;
  }
}


/* Executed when window is resized to 'width' and 'height' */
/* Modify the bounds of the screen here in glm::ortho or Field of View in glm::Perspective */
void reshapeWindow (GLFWwindow* window, int width, int height)
{
  int fbwidth=width, fbheight=height;
  /* With Retina display on Mac OS X, GLFW's FramebufferSize
     is different from WindowSize */
  glfwGetFramebufferSize(window, &fbwidth, &fbheight);

  GLfloat fov = 90.0f;

  // sets the viewport of openGL renderer
  glViewport (0, 0, (GLsizei) fbwidth, (GLsizei) fbheight);

  // set the projection matrix as perspective
  /* glMatrixMode (GL_PROJECTION);
     glLoadIdentity ();
     gluPerspective (fov, (GLfloat) fbwidth / (GLfloat) fbheight, 0.1, 500.0); */
  // Store the projection matrix in a variable for future use
  // Perspective projection for 3D views
  // Matrices.projection = glm::perspective (fov, (GLfloat) fbwidth / (GLfloat) fbheight, 0.1f, 500.0f);

  // Ortho projection for 2D views
  Matrices.projection = glm::ortho(-zoom_scale, zoom_scale, -zoom_scale, zoom_scale, 0.1f, 500.0f);
}
/* Initialise glfw window, I/O callbacks and the renderer to use */
/* Nothing to Edit here */
GLFWwindow* initGLFW (int width, int height)
{
  GLFWwindow* window; // window desciptor/handle

  glfwSetErrorCallback(error_callback);
  if (!glfwInit()) {
    exit(EXIT_FAILURE);
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(width, height, "Shoot The Target!", NULL, NULL);

  if (!window) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(window);
  gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
  glfwSwapInterval( 1 );

  /* --- register callbacks with GLFW --- */

  /* Register function to handle window resizes */
  /* With Retina display on Mac OS X GLFW's FramebufferSize
     is different from WindowSize */
  glfwSetFramebufferSizeCallback(window, reshapeWindow);
  glfwSetWindowSizeCallback(window, reshapeWindow);

  /* Register function to handle window close */
  glfwSetWindowCloseCallback(window, quit);

  /* Register function to handle keyboard input */
  glfwSetKeyCallback(window, keyboard);      // general keyboard input
  glfwSetCharCallback(window, keyboardChar);  // simpler specific character handling

  /* Register function to handle mouse click */
  glfwSetMouseButtonCallback(window, mouseButton);  // mouse button clicks
  glfwSetScrollCallback(window, scrollCallback);

  return window;
}


void initGL (GLFWwindow* window, int width, int height)
{
  /* Objects should be created before any other gl function and shaders */
  // Create the models
  // Load Textures
  // Enable Texture0 as current texture memory
  glActiveTexture(GL_TEXTURE0);
  // load an image file directly as a new OpenGL texture
  // GLuint texID = SOIL_load_OGL_texture ("beach.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_TEXTURE_REPEATS); // Buggy for OpenGL3
  textureID[0] = createTexture("images/canon.jpg");
  textureID[2] = createTexture("images/ground.jpeg");
  textureID[3] = createTexture("images/sky.png");
  textureID[4] = createTexture("images/bird.png");
  textureID[5] = createTexture("images/block.jpeg");
  textureID[6] = createTexture("images/block2.jpg");
  textureID[7] = createTexture("images/piggy.jpeg");
  textureID[8] = createTexture("images/welcome.jpg");
  textureID[9] = createTexture("images/banner.jpg");



  textureID[13] = createTexture("images/frame_0_delay-0.jpg");
  textureID[14] = createTexture("images/frame_1_delay-0.jpg");
  textureID[15] = createTexture("images/frame_2_delay-0.jpg");
  textureID[16] = createTexture("images/frame_3_delay-0.jpg");
  textureID[17] = createTexture("images/frame_4_delay-0.jpg");
  textureID[18] = createTexture("images/frame_5_delay-0.jpg");
  textureID[19] = createTexture("images/frame_6_delay-0.jpg");
  textureID[20] = createTexture("images/frame_7_delay-0.jpg");
  textureID[21] = createTexture("images/frame_8_delay-0.jpg");
  textureID[22] = createTexture("images/o_9aee85575002f815-0.jpg");
  textureID[23] = createTexture("images/o_9aee85575002f815-1.jpg");
  textureID[24] = createTexture("images/o_9aee85575002f815-2.jpg");
  textureID[25] = createTexture("images/o_9aee85575002f815-3.jpg");
  textureID[26] = createTexture("images/o_9aee85575002f815-4.jpg");
  textureID[27] = createTexture("images/o_9aee85575002f815-5.jpg");
  textureID[28] = createTexture("images/o_9aee85575002f815-6.jpg");
  textureID[29] = createTexture("images/o_9aee85575002f815-7.jpg");
  textureID[30] = createTexture("images/o_9aee85575002f815-8.jpg");
  textureID[31] = createTexture("images/o_9aee85575002f815-9.jpg");
  textureID[32] = createTexture("images/o_9aee85575002f815-10.jpg");
  textureID[33] = createTexture("images/o_9aee85575002f815-11.jpg");
  textureID[34] = createTexture("images/o_9aee85575002f815-12.jpg");
  textureID[35] = createTexture("images/o_9aee85575002f815-13.jpg");
  textureID[36] = createTexture("images/o_9aee85575002f815-14.jpg");
  textureID[37] = createTexture("images/o_9aee85575002f815-15.jpg");
  textureID[38] = createTexture("images/o_9aee85575002f815-16.jpg");
  textureID[39] = createTexture("images/o_9aee85575002f815-17.jpg");
  textureID[40] = createTexture("images/o_9aee85575002f815-18.jpg");
  textureID[41] = createTexture("images/o_8c353d210203cf60-0.jpg");
  textureID[42] = createTexture("images/o_8c353d210203cf60-1.jpg");

  textureID[43] = createTexture("images/background/frame_0_delay-0.jpg");
  textureID[44] = createTexture("images/background/frame_1_delay-0.jpg");
  textureID[45] = createTexture("images/background/frame_2_delay-0.jpg");
  textureID[46] = createTexture("images/background/frame_3_delay-0.jpg");
  textureID[47] = createTexture("images/background/frame_4_delay-0.jpg");
  textureID[48] = createTexture("images/background/frame_5_delay-0.jpg");
  textureID[49] = createTexture("images/background/frame_6_delay-0.jpg");
  textureID[50] = createTexture("images/background/frame_7_delay-0.jpg");

  textureID[51] = createTexture("images/lives/frame_0_delay-0.jpg");
  textureID[52] = createTexture("images/lives/frame_1_delay-0.jpg");
  textureID[53] = createTexture("images/lives/frame_2_delay-0.jpg");
  textureID[54] = createTexture("images/lives/frame_3_delay-0.jpg");
  textureID[55] = createTexture("images/lives/frame_4_delay-0.jpg");
  textureID[56] = createTexture("images/lives/frame_5_delay-0.jpg");
  textureID[57] = createTexture("images/lives/frame_6_delay-0.jpg");

  textureID[58] = createTexture("images/statues/statue1.png");
  textureID[59] = createTexture("images/statues/statue2.png");
  textureID[60] = createTexture("images/statues/statue3.png");

  textureID[61] = createTexture("images/wheel/frame_0_delay-0.jpg");
  textureID[62] = createTexture("images/wheel/frame_1_delay-0.jpg");
  textureID[63] = createTexture("images/wheel/frame_2_delay-0.jpg");
  textureID[64] = createTexture("images/wheel/frame_3_delay-0.jpg");
  textureID[65] = createTexture("images/point.jpeg");

  textureID[66] = createTexture("images/obstacles/frame_0_delay-0.jpg");
  textureID[67] = createTexture("images/obstacles/frame_1_delay-0.jpg");
  textureID[68] = createTexture("images/obstacles/frame_2_delay-0.jpg");

  textureID[69] = createTexture("images/images/ball/frame_0_delay-0.jpg");
  textureID[70] = createTexture("images/images/ball/frame_1_delay-0.jpg");
  textureID[71] = createTexture("images/images/ball/frame_2_delay-0.jpg");
  textureID[72] = createTexture("images/images/ball/frame_3_delay-0.jpg");
  textureID[73] = createTexture("images/images/ball/frame_4_delay-0.jpg");
  textureID[74] = createTexture("images/images/ball/frame_5_delay-0.jpg");
  textureID[75] = createTexture("images/images/ball/frame_6_delay-0.jpg");

  textureID[76] = createTexture("images/images/star.png");
  textureID[77] = createTexture("images/images/level.jpg");
  textureID[78] = createTexture("images/images/obstacles/frame_3_delay-0.jpg");



  // check for an error during the load process
  if(textureID[0] == 0 )
    cout << "SOIL loading error: '" << SOIL_last_result() << "'" << endl;

  // Create and compile our GLSL program from the texture shaders
  textureProgramID = LoadShaders( "TextureRender.vert", "TextureRender.frag" );

  programID = LoadShaders( "Sample_GL3.vert", "Sample_GL3.frag" );
  // Get a handle for our "MVP" uniform
  Matrices.MatrixID = glGetUniformLocation(programID, "MVP");
  reshapeWindow (window, width, height);

  // Background color of the scene
  glClearColor (0.0f, 0.0f, 0.0f, 0.0f); // R, G, B, A
  glClearDepth (1.0f);

  glEnable (GL_DEPTH_TEST);
  glDepthFunc (GL_LEQUAL);
  //glEnable (GL_BLEND);
  //glBlendFunc (GL_SRC_ALPHA, GL_ONE);

  const char* fontfile = "arial.ttf";
  GL3Font.font = new FTExtrudeFont(fontfile); // 3D extrude style rendering

  if(GL3Font.font->Error())
  {
    cout << "Error: Could not load font `" << fontfile << "'" << endl;
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  // Create and compile our GLSL program from the font shaders
  fontProgramID = LoadShaders( "fontrender.vert", "fontrender.frag" );
  GLint fontVertexCoordAttrib, fontVertexNormalAttrib, fontVertexOffsetUniform;
  fontVertexCoordAttrib = glGetAttribLocation(fontProgramID, "vertexPosition");
  fontVertexNormalAttrib = glGetAttribLocation(fontProgramID, "vertexNormal");
  fontVertexOffsetUniform = glGetUniformLocation(fontProgramID, "pen");
  GL3Font.fontMatrixID = glGetUniformLocation(fontProgramID, "MVP");
  GL3Font.fontColorID = glGetUniformLocation(fontProgramID, "fontColor");

  GL3Font.font->ShaderLocations(fontVertexCoordAttrib, fontVertexNormalAttrib, fontVertexOffsetUniform);
  GL3Font.font->FaceSize(1);
  GL3Font.font->Depth(0);
  GL3Font.font->Outset(0, 0);
  GL3Font.font->CharMap(ft_encoding_unicode);

  cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
  cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
  cout << "VERSION: " << glGetString(GL_VERSION) << endl;
  cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}

