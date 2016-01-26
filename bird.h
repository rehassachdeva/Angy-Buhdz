
class bird {
  public:
    float x,y,vx,vy,boost,theta,radius, prevVx, prevVy, ix, iy;
    float ball_rotation;
    bool shot;
    bool ball_rot_stat;

    int flag;
    float drag, fireTime;
    VAO* triangle;
    bird(float x, float y) {
  
      ix = -26;
      iy = -17;
      this->drag=0.001;
      ball_rotation = 0;
      //ball_trans_stat = 0;
      ball_rot_stat = 1;
      shot=false;
      this->x = -26;
      this->y = -17;
      radius = 0.3;
      boost=10.0;

       static const GLfloat vertex_buffer_data [] = {
        0, 0.3,0, // vertex 0
        -0.3,-0.3,0, // vertex 1
        0.3,-0.3,0, // vertex 2
      };


      // create3DObject creates and returns a handle to a VAO that can be used later
     triangle = create3DTexturedObject(GL_TRIANGLES, 6, vertex_buffer_data, texture_buffer_data, textureID[4], GL_FILL);
      flag=0;
      ball_trans_stat=0;
    }


    bool checkCollision(float xx, float yy, float hh, float ww, bool alive) {
      if(alive==false) return false;
      float tmpx, tmpy;
      for(int i=0; i<=360; i++) {
        tmpx = x+radius*cos(i*M_PI/180.0);
        tmpy = y+radius*sin(i*M_PI/180.0);

        if(tmpx>= xx-ww/2 && tmpx<=xx+ww/2 && tmpy>= yy-hh/2 && tmpy <=yy+hh/2) {
          if(tmpx>xx-ww/2 && tmpx<xx+ww/2) vy=-coeff*vy;
          //cout<<vx<<" "<<vy<<endl;
          if(tmpy>= yy-hh/2 && tmpy <=yy+hh/2) vx =-coeff*vx;
          return true;
        }
        return false;
      }
    }

    void update() {
      /* if(this->vx<9.8*0.01 || this->vy < 3*0.01) {
         this->ball_trans_stat = 0;
         }*/
      prevVx = vx;
      prevVy = vy;

      if(abs(vy)<0.01)vy=0;
      if(abs(vx)<0.01)vx=0;

      if(shot){
        if(y <= groundLevel+1.4) {
          if(vx<-0.01) drag = -5;
          else if(vx>0.01) drag = 2;
          else drag = 0;
          vy=-vy*coeff;
        }   
        else {
          if(vx<-0.01) drag = -0.01;
          else if(vx>0.01) drag = 0.01;
          else drag = 0;
        } 



        if(vx||vy) {
          this->x += this->vx*0.01 - 0.5*drag*0.0001;

          this->y += this->vy*0.01 - 0.5*gravity*0.0001;
          this->vy -= gravity*0.01;
          this->vx -= drag*0.01;
          if(ball_rot_stat) this->ball_rotation = this->ball_rotation+1;
          // if(vy<=0) vy = 0; 
        }
        if(vx == 0) drag = 0;
        else if(vx==0 and vy==0)
        {


          /*if(flag<=5)
            flag++,this->ball_rotation = this->ball_rotation+1;
            else*/
          ball_rotation=0;
        }
        if(prevVx==vx&&prevVy==vy || glfwGetTime()-fireTime>15 || abs(x)>24) {
          ball_rot_stat = 0;
          flag++;
          if(flag==20) {
            this->drag=0.01;
            ball_rot_stat = 1;
          
      ball_rotation = 0;
      //ball_trans_stat = 0;
      
      shot=false;
      this->x = -12;
      this->y = groundLevel+2;
      radius = 0.3;
      boost=10.0;
      flag = 0;
      lives--;
      if(lives == 0) {
        points = 0;
        lives = 3;
      }
      

     
    }

        }
        else ball_rot_stat = 1;
      }

    }

    void fire(float boost, float theta) {
      this->boost = boost;
      if(boost<4)
      boost=4;
      else if(boost>20)
      boost=20;
      this->theta = theta;
      this->vx = boost*cos(theta*M_PI/180.0);
      this->vy = boost*sin(theta*M_PI/180.0);
      shot=true;
      flag=0;
      fireTime = glfwGetTime();


    }

    void draw ()
    {

      float increments;
      glm::mat4 translateRectangle;

     

      // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
      //  Don't change unless you are sure!!
      glm::mat4 VP = Matrices.projection * Matrices.view;

      // Send our transformation to the currently bound shader, in the "MVP" uniform
      // For each model you render, since the MVP will be different (at least the M part)
      //  Don't change unless you are sure!!
      glm::mat4 MVP;  // MVP = Projection * View * Model

      // Load identity to model matrix
      Matrices.model = glm::mat4(1.0f);

      glm::mat4 translateBall = glm::translate (glm::vec3(this->x, this->y, 0.0f)); // glTranslatef
      glm::mat4 rotateBall = glm::rotate((float)(this->ball_rotation*M_PI/180.0f), glm::vec3(0,0,1));  // rotate about vector (1,0,0)
      glm::mat4 ballTransform = translateBall * rotateBall;
      //glm::mat4 ballTransform = translateBall;

      Matrices.model *= ballTransform; 
      MVP = VP * Matrices.model; // MVP = p * V * M
      // draw3DObject draws the VAO given to it using current MVP matrix
        glUniformMatrix4fv(Matrices.TexMatrixID, 1, GL_FALSE, &MVP[0][0]);

      // Set the texture sampler to access Texture0 memory
      glUniform1i(glGetUniformLocation(textureProgramID, "texSampler"), 0);
draw3DTexturedObject(triangle);
      update();
            //camera_rotation_angle++; // Simulating camera 

    }
};
