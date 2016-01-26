
class speedy {
  public:
    float x,y,h,w;
    VAO *rectangle,*rectangle2;
    speedy() {
      x=5;
      y=18;
      h=1;
      w=0.5;
      // GL3 accepts onlytime Triangles. Quads are not supported
      GLfloat vertex_buffer_data [] = {
        -w/2,-h/2,0, // vertex 1
        w/2,-h/2,0, // vertex 2
        w/2, h/2,0, // vertex 3


        -w/2,-h/2,0, // vertex 1
        w/2, h/2,0, // vertex 3
        -w/2, h/2,0 // vertex 4
      };

      GLfloat color_buffer_data [] = {
        1,0,0, // color 1
        1,0,0, // color 2
        1,0,0, // color 3


        1,0,0,  // color 1
        1,0,0, // color 3
        1,0,0, // color 4
      };

      GLfloat color_buffer_data2 [] = {
        0,0,0, // color 1
        0,0,0, // color 2
        0,0,0, // color 3


        0,0,0,  // color 1
        0,0,0, // color 3
        0,0,0, // color 4
      };

      rectangle = create3DObject(GL_TRIANGLES, 6, vertex_buffer_data, color_buffer_data, GL_FILL);
      rectangle2 = create3DObject(GL_TRIANGLES, 6, vertex_buffer_data, color_buffer_data2, GL_FILL);

      // GL3 accepts onlytime Triangles. Quads are not supported

    }
    void draw () {
      glUseProgram (programID);

      float tempx=w/2;
      for(int i=4;i<=20;++i){
        tempx+=h/2;
        glm::mat4 translateRectangle;
        glm::mat4 VP = Matrices.projection * Matrices.view;

        // Send our transformation to the currently bound shader, in the "MVP" uniform
        // For each model you render, since the MVP will be different (at least the M part)
        //  Don't change unless you are sure!!
        glm::mat4 MVP;  // MVP = Projection * View * Model

        // Load identity to model matrix
        Matrices.model = glm::mat4(1.0f);

        translateRectangle = glm::translate (glm::vec3(tempx, y, 0));        // glTranslatef
        //glm::mat4 rotateRectangle = glm::rotate((float)(theta*M_PI/180.0f), glm::vec3(0,0,1)); // rotate about vector (-1,1,1)
        //Matrices.model *= (translateRectangle * rotateRectangle);
        Matrices.model *= (translateRectangle );

        MVP = VP * Matrices.model;
        glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);


        // draw3DObject draws the VAO given to it using current MVP matrix
        if (i<=boost)
          draw3DObject(rectangle);
        else
          draw3DObject(rectangle2);

      }

    }
};

class elements {
  public:
    VAO *ground, *sky, *welcome, *banner, *piggies[19], *bluesGif[9], *play[2], *fall[8], *life[7], *statue[3];
    char buffer [33];
    float w, h, x, y, livesX, statueY, theta;
    int frames, idx, idx1, idx2, idx3, idx4;
    bool hit[3], alive[3];
    elements() {
      for(int i = 0; i<3; i++) {
        hit[i] = false;
        alive[i] = true;
      }
      lives = 3;
      frames = 0;
      idx = 0;
      idx1 = 0;
      idx2 = 0;
      idx3 = 0;
      idx4 = 0;
      livesX = 1.5;
      statueY = 6.5;

      x = 0;
      y = -12;
      h = 2;
      w = 20;
      GLfloat vertex_buffer_data [] = {
        -30,-20,0, // vertex 1
        30,-20,0, // vertex 2
        30, 20,0, // vertex 3
        30, 20,0, // vertex 3
        -30, 20,0, // vertex 4
        -30,-20,0 // vertex 1      
      };
      GLfloat vertex_buffer_data_welcome [] = {
        -15,-15,0, // vertex 1
        15,-15,0, // vertex 2
        15, 15,0, // vertex 3      
        15, 15,0, // vertex 3
        -15, 15,0, // vertex 4
        -15,-15,0 // vertex 1
      };
      GLfloat vertex_buffer_data_banner [] = {
        -14,-7,0, // vertex 1
        14,-7,0, // vertex 2
        14, 7,0, // vertex 3      
        14, 7,0, // vertex 3
        -14, 7,0, // vertex 4
        -14,-7,0 // vertex 1
      };
      GLfloat vertex_buffer_data_blues [] = {
        -4,-7,0, // vertex 1
        4,-7,0, // vertex 2
        4, 7,0, // vertex 3      
        4, 7,0, // vertex 3
        -4, 7,0, // vertex 4
        -4,-7,0 // vertex 1
      };
        GLfloat vertex_buffer_data_statues [] = {
        -1.5,-5,0, // vertex 1
        1.5,-5,0, // vertex 2
        1.5, 5,0, // vertex 3      
        1.5, 5,0, // vertex 3
        -1.5, 5,0, // vertex 4
        -1.5,-5,0 // vertex 1
      };

      GLfloat vertex_buffer_data_lives [] = {
        -1,-1,0, // vertex 1
        1,-1,0, // vertex 2
        1, 1,0, // vertex 3      
        1, 1,0, // vertex 3
        -1, 1,0, // vertex 4
        -1,-1,0 // vertex 1
      };
      GLfloat color_buffer_data [] = {
        1,0,0, // color 1
        0,0,1, // color 2
        0,1,0, // color 3
        1,0,0,  // color 1
        0,1,0, // color 3
        0.3,0.3,0.3, // color 4
      };
      //ground = create3DTexturedObject(GL_TRIANGLES, 6, vertex_buffer_data, texture_buffer_data, textureID[2], GL_FILL);
      //sky = create3DTexturedObject(GL_TRIANGLES, 6, vertex_buffer_data, texture_buffer_data, textureID[3], GL_FILL);
      welcome = create3DTexturedObject(GL_TRIANGLES, 6, vertex_buffer_data_welcome, texture_buffer_data, textureID[8], GL_FILL);
      banner = create3DTexturedObject(GL_TRIANGLES, 6, vertex_buffer_data_banner, texture_buffer_data, textureID[9], GL_FILL);
      //piggies = create3DTexturedObject(GL_TRIANGLES, 6, vertex_buffer_data_blues, texture_buffer_data, textureID[12], GL_FILL);
      for(int i=13; i<=21; i++) {
        bluesGif[i-13] = create3DTexturedObject(GL_TRIANGLES, 6, vertex_buffer_data_blues, texture_buffer_data, textureID[i], GL_FILL);
      }
      for(int i=22; i<=40; i++) {
        piggies[i-22] = create3DTexturedObject(GL_TRIANGLES, 6, vertex_buffer_data_blues, texture_buffer_data, textureID[i], GL_FILL);
      }
      play[0] = create3DTexturedObject(GL_TRIANGLES, 6, vertex_buffer_data_blues, texture_buffer_data, textureID[41], GL_FILL);
      play[1] = create3DTexturedObject(GL_TRIANGLES, 6, vertex_buffer_data_blues, texture_buffer_data, textureID[42], GL_FILL);
      for(int i=43; i<=50; i++) {
        fall[i-43] = create3DTexturedObject(GL_TRIANGLES, 6, vertex_buffer_data, texture_buffer_data, textureID[i], GL_FILL);
      }
      for(int i=51; i<=57; i++) {
        life[i-51] = create3DTexturedObject(GL_TRIANGLES, 6, vertex_buffer_data_lives, texture_buffer_data, textureID[i], GL_FILL);
      }
      for(int i = 0; i<3; i++) {
            statue[i] = create3DTexturedObject(GL_TRIANGLES, 6, vertex_buffer_data_statues, texture_buffer_data, textureID[58+i], GL_FILL);
          }


    }
    void draw ()
    {
      // clear the color and depth in the frame buffer
      glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      // use the loaded shader program
      // Don't change unless you know what you are doing
      glUseProgram (textureProgramID);

      // Eye - Location of camera. Don't change unless you are sure!!
      glm::vec3 eye ( 5*cos(camera_rotation_angle*M_PI/180.0f), 0, 5*sin(camera_rotation_angle*M_PI/180.0f) );
      // Target - Where is the camera looking at.  Don't change unless you are sure!!
      glm::vec3 target (0, 0, 0);
      // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
      glm::vec3 up (0, 1, 0);

      // Compute Camera matrix (view)
      // Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
      //  Don't change unless you are sure!!
      Matrices.view = glm::lookAt(glm::vec3(panx,0,3), glm::vec3(panx,0,0), glm::vec3(0,1,0)); // Fixed camera for 2D (ortho) in XY plane

      glm::mat4 translateRectangle;


      glm::mat4 VP = Matrices.projection * Matrices.view;

      // Send our transformation to the currently bound shader, in the "MVP" uniform
      // For each model you render, since the MVP will be different (at least the M part)
      //  Don't change unless you are sure!!
      glm::mat4 MVP;  // MVP = Projection * View * Model

      Matrices.model = glm::mat4(1.0f);

      translateRectangle = glm::translate (glm::vec3(15, 15, 0));        // glTranslatef
      //glm::mat4 rotateRectangle = glm::rotate((float)(rectangle_rotation*M_PI/180.0f), glm::vec3(0,0,1)); // rotate about vector (-1,1,1)
      Matrices.model *= (translateRectangle);
      MVP = VP * Matrices.model;
      glUniformMatrix4fv(Matrices.TexMatrixID, 1, GL_FALSE, &MVP[0][0]);

      // Set the texture sampler to access Texture0 memory
      glUniform1i(glGetUniformLocation(textureProgramID, "texSampler"), 0);

      // draw3DObject draws the VAO given to it using current MVP matrix
      //    draw3DTexturedObject(sky);

      // Load identity to model matrix
      Matrices.model = glm::mat4(1.0f);

      translateRectangle = glm::translate (glm::vec3(x, y, 0));        // glTranslatef
      //glm::mat4 rotateRectangle = glm::rotate((float)(rectangle_rotation*M_PI/180.0f), glm::vec3(0,0,1)); // rotate about vector (-1,1,1)
      Matrices.model *= (translateRectangle);
      MVP = VP * Matrices.model;
      glUniformMatrix4fv(Matrices.TexMatrixID, 1, GL_FALSE, &MVP[0][0]);

      // Set the texture sampler to access Texture0 memory
      glUniform1i(glGetUniformLocation(textureProgramID, "texSampler"), 0);

      // draw3DObject draws the VAO given to it using current MVP matrix
      //draw3DTexturedObject(ground);
      // Render font on screen
     
      //rectangle_rotation = rectangle_rotation + 1*rectangle_rot_dir*rectangle_rot_status;
    }
    void drawWelcome() {
      glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glUseProgram (textureProgramID);
      glm::vec3 eye ( 5*cos(camera_rotation_angle*M_PI/180.0f), 0, 5*sin(camera_rotation_angle*M_PI/180.0f) );
      glm::vec3 target (0, 0, 0);
      glm::vec3 up (0, 1, 0);
      Matrices.view = glm::lookAt(glm::vec3(panx,0,3), glm::vec3(panx,0,0), glm::vec3(0,1,0)); // Fixed camera for 2D (ortho) in XY plane
      glm::mat4 translateRectangle;
      glm::mat4 VP = Matrices.projection * Matrices.view;
      glm::mat4 MVP;  // MVP = Projection * View * Model
      Matrices.model = glm::mat4(1.0f);
      translateRectangle = glm::translate (glm::vec3(0, 0, 0));        // glTranslatef
      Matrices.model *= (translateRectangle);
      MVP = VP * Matrices.model;
      glUniformMatrix4fv(Matrices.TexMatrixID, 1, GL_FALSE, &MVP[0][0]);
      glUniform1i(glGetUniformLocation(textureProgramID, "texSampler"), 0);
      draw3DTexturedObject(welcome);
    }
    void drawMenu() {
      glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glUseProgram (textureProgramID);
      glm::vec3 eye ( 5*cos(camera_rotation_angle*M_PI/180.0f), 0, 5*sin(camera_rotation_angle*M_PI/180.0f) );
      glm::vec3 target (0, 0, 0);
      glm::vec3 up (0, 1, 0);
      Matrices.view = glm::lookAt(glm::vec3(panx,0,3), glm::vec3(panx,0,0), glm::vec3(0,1,0)); // Fixed camera for 2D (ortho) in XY plane
      glm::mat4 translateRectangle;
      glm::mat4 VP = Matrices.projection * Matrices.view;
      glm::mat4 MVP;  // MVP = Projection * View * Model
      Matrices.model = glm::mat4(1.0f);
      translateRectangle = glm::translate (glm::vec3(0, 7, 0));        // glTranslatef
      Matrices.model *= (translateRectangle);
      MVP = VP * Matrices.model;
      glUniformMatrix4fv(Matrices.TexMatrixID, 1, GL_FALSE, &MVP[0][0]);
      glUniform1i(glGetUniformLocation(textureProgramID, "texSampler"), 0);
      draw3DTexturedObject(banner);
      Matrices.model = glm::mat4(1.0f);
      translateRectangle = glm::translate (glm::vec3(-10, -7, 0));        // glTranslatef
      Matrices.model *= (translateRectangle);
      MVP = VP * Matrices.model;
      glUniformMatrix4fv(Matrices.TexMatrixID, 1, GL_FALSE, &MVP[0][0]);
      glUniform1i(glGetUniformLocation(textureProgramID, "texSampler"), 0);
      // draw3DTexturedObject(bluesGif[idx]);
      Matrices.model = glm::mat4(1.0f);
      translateRectangle = glm::translate (glm::vec3(6, -7, 0));        // glTranslatef
      Matrices.model *= (translateRectangle);
      MVP = VP * Matrices.model;
      glUniformMatrix4fv(Matrices.TexMatrixID, 1, GL_FALSE, &MVP[0][0]);
      glUniform1i(glGetUniformLocation(textureProgramID, "texSampler"), 0);
      draw3DTexturedObject(piggies[idx1]);
      Matrices.model = glm::mat4(1.0f);
      translateRectangle = glm::translate (glm::vec3(-5, -7, 0));        // glTranslatef
      Matrices.model *= (translateRectangle);
      MVP = VP * Matrices.model;
      glUniformMatrix4fv(Matrices.TexMatrixID, 1, GL_FALSE, &MVP[0][0]);
      glUniform1i(glGetUniformLocation(textureProgramID, "texSampler"), 0);
      draw3DTexturedObject(play[idx2]);
      frames++;
      if(frames == 8) {
        idx++;
        idx1++;
        idx2++;
        frames = 0;
        if(idx==9) idx =0;
        if(idx1==19) idx1 = 0;
        if(idx2==2) idx2 = 0;
      }      
    }

    void drawBackground() {
      static int frames = 0;
      glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glUseProgram (textureProgramID);
      glm::vec3 eye ( 5*cos(camera_rotation_angle*M_PI/180.0f), 0, 5*sin(camera_rotation_angle*M_PI/180.0f) );
      glm::vec3 target (0, 0, 0);
      glm::vec3 up (0, 1, 0);
      Matrices.view = glm::lookAt(glm::vec3(panx,0,3), glm::vec3(panx,0,0), glm::vec3(0,1,0)); // Fixed camera for 2D (ortho) in XY plane
      glm::mat4 translateRectangle;
      glm::mat4 VP = Matrices.projection * Matrices.view;
      glm::mat4 MVP;  // MVP = Projection * View * Model
      Matrices.model = glm::mat4(1.0f);
      translateRectangle = glm::translate (glm::vec3(0, 7, 0));        // glTranslatef
      Matrices.model *= (translateRectangle);
      MVP = VP * Matrices.model;
      glUniformMatrix4fv(Matrices.TexMatrixID, 1, GL_FALSE, &MVP[0][0]);
      glUniform1i(glGetUniformLocation(textureProgramID, "texSampler"), 0);
      draw3DTexturedObject(banner);
      Matrices.model = glm::mat4(1.0f);
      translateRectangle = glm::translate (glm::vec3(0, 0, 0));        // glTranslatef
      Matrices.model *= (translateRectangle);
      MVP = VP * Matrices.model;
      glUniformMatrix4fv(Matrices.TexMatrixID, 1, GL_FALSE, &MVP[0][0]);
      glUniform1i(glGetUniformLocation(textureProgramID, "texSampler"), 0);
      draw3DTexturedObject(fall[idx3]);

      Matrices.model = glm::mat4(1.0f);
      translateRectangle = glm::translate (glm::vec3(livesX, 15, 0));        // glTranslatef
      Matrices.model *= (translateRectangle);
      MVP = VP * Matrices.model;
      glUniformMatrix4fv(Matrices.TexMatrixID, 1, GL_FALSE, &MVP[0][0]);
      glUniform1i(glGetUniformLocation(textureProgramID, "texSampler"), 0);
      draw3DTexturedObject(life[idx4]);
if(alive[0]) {
      Matrices.model = glm::mat4(1.0f);
      translateRectangle = glm::translate (glm::vec3(-15, statueY, 0));
      glm::mat4 rotateRectangle = glm::rotate((float)(theta*M_PI/180.0f), glm::vec3(0,0,1));        // glTranslatef
      Matrices.model *= (translateRectangle * rotateRectangle );
      MVP = VP * Matrices.model;
      glUniformMatrix4fv(Matrices.TexMatrixID, 1, GL_FALSE, &MVP[0][0]);
      glUniform1i(glGetUniformLocation(textureProgramID, "texSampler"), 0);
      draw3DTexturedObject(statue[0]);
    }
    if(alive[1]) {
      Matrices.model = glm::mat4(1.0f);
      translateRectangle = glm::translate (glm::vec3(-11, statueY, 0));
      glm::mat4 rotateRectangle = glm::rotate((float)(theta*M_PI/180.0f), glm::vec3(0,0,1));        // glTranslatef
      Matrices.model *= (translateRectangle * rotateRectangle );
      MVP = VP * Matrices.model;
      glUniformMatrix4fv(Matrices.TexMatrixID, 1, GL_FALSE, &MVP[0][0]);
      glUniform1i(glGetUniformLocation(textureProgramID, "texSampler"), 0);
      draw3DTexturedObject(statue[1]);
    }
    if(alive[2]) {
      Matrices.model = glm::mat4(1.0f);
      translateRectangle = glm::translate (glm::vec3(-7, statueY, 0));
      glm::mat4 rotateRectangle = glm::rotate((float)(theta*M_PI/180.0f), glm::vec3(0,0,1));        // glTranslatef
      Matrices.model *= (translateRectangle * rotateRectangle );
      MVP = VP * Matrices.model;
      glUniformMatrix4fv(Matrices.TexMatrixID, 1, GL_FALSE, &MVP[0][0]);
      glUniform1i(glGetUniformLocation(textureProgramID, "texSampler"), 0);
      draw3DTexturedObject(statue[2]);
    }
      if(hit[0]) {
          theta-=1;
          if(theta == -100) {
            alive[0]=false;
            statueY = 6.5;
            theta = 0;
            hit[0] = false;
          }  
        statueY -= 0.09;
      }
       if(hit[1]) {
          theta-=1;
          if(theta == -100) {
            alive[1]=false;
            statueY = 6.5;
            theta = 0;
            hit[1] = false;
          }  
        statueY -= 0.09;
      }
       if(hit[2]) {
          theta-=1;
          if(theta == -100) {
            alive[2]=false;
            statueY = 6.5;
            theta = 0;
            hit[2] = false;

          }  
        statueY -= 0.09;
      }

      
      if(lives>1) {

        Matrices.model = glm::mat4(1.0f);
      translateRectangle = glm::translate (glm::vec3(livesX+3, 15, 0));        // glTranslatef
      Matrices.model *= (translateRectangle);
      MVP = VP * Matrices.model;
      glUniformMatrix4fv(Matrices.TexMatrixID, 1, GL_FALSE, &MVP[0][0]);
      glUniform1i(glGetUniformLocation(textureProgramID, "texSampler"), 0);
      draw3DTexturedObject(life[idx4]);

      }
      if(lives>2) {

        Matrices.model = glm::mat4(1.0f);
      translateRectangle = glm::translate (glm::vec3(livesX+6 , 15, 0));        // glTranslatef
      Matrices.model *= (translateRectangle);
      MVP = VP * Matrices.model;
      glUniformMatrix4fv(Matrices.TexMatrixID, 1, GL_FALSE, &MVP[0][0]);
      glUniform1i(glGetUniformLocation(textureProgramID, "texSampler"), 0);
      draw3DTexturedObject(life[idx4]);
    }

      frames++;
      if(frames == 8) {
        idx3++;
        idx4++;
        frames = 0;
        if(idx3==8) idx3 = 0;
        if(idx4==7) idx4=0;
      }   

      if(zoom_flag && zoom_scale+zoom_flag>=10 && zoom_scale+zoom_flag<=20) zoom_scale += zoom_flag;
      else if(zoom_scale+zoom_flag>20) zoom_scale = 20;
      else if(zoom_scale+zoom_flag<10) zoom_scale = 10;

      //  cout<<zoom_scale<<endl;
      Matrices.projection = glm::ortho(-zoom_scale, zoom_scale, -zoom_scale, zoom_scale, 0.1f, 500.0f);
       static int fontScale = 0;
      float fontScaleValue = 3.0 + 0.25*sinf(fontScale*M_PI/180.0f);
      glm::vec3 fontColor = getRGBfromHue (fontScale);
      glUseProgram(fontProgramID);
      // Transform the text
      Matrices.model = glm::mat4(1.0f);
      glm::mat4 translateText = glm::translate(glm::vec3(0.5,11.5,0));
      glm::mat4 scaleText = glm::scale(glm::vec3(fontScaleValue,fontScaleValue,fontScaleValue));
      Matrices.model *= (translateText * scaleText);
      MVP = Matrices.projection * Matrices.view * Matrices.model;
      // send font's MVP and font color to fond shaders
      glUniformMatrix4fv(GL3Font.fontMatrixID, 1, GL_FALSE, &MVP[0][0]);
      glUniform3fv(GL3Font.fontColorID, 1, &fontColor[0]);
      snprintf(buffer, sizeof(buffer), "%s", "SCORE:");
      GL3Font.font->Render(buffer);
      Matrices.model = glm::mat4(1.0f);
      translateText = glm::translate(glm::vec3(7.5,11.5,0));
      scaleText = glm::scale(glm::vec3(fontScaleValue,fontScaleValue,fontScaleValue));
      Matrices.model *= (translateText * scaleText);
      MVP = Matrices.projection * Matrices.view * Matrices.model;
      // send font's MVP and font color to fond shaders
      glUniformMatrix4fv(GL3Font.fontMatrixID, 1, GL_FALSE, &MVP[0][0]);
      glUniform3fv(GL3Font.fontColorID, 1, &fontColor[0]);
      snprintf(buffer, sizeof(buffer), "%d", points);
      GL3Font.font->Render(buffer);
      // Render font
      //GL3Font.font->Render(buffer);

    }
};

