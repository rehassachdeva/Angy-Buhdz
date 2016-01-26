

class block {
  public:
    VAO *rectangle;
    float x,y,h,w, vx,vy, boost, theta,drag;
    int update_theta, fall, type;
    int score, lives;    
    bool alive;

    block(float x, float y, float h, float w, float col, int type) {
      this->x=x;
      this->y=y;
      this->h=h;
      this->w=w;
      this->type = type;
      this->drag = 5;
      this->update_theta = 0;
      //if(y-h/2<=groundLevel)
      this->fall = 0;
      this->theta = 0;
      alive = true;
      this->vx = 0;
      this->vy = 0;

      GLfloat vertex_buffer_data [] = {
        -w/2,-h/2,0, // vertex 1
        w/2,-h/2,0, // vertex 2
        w/2, h/2,0, // vertex 3
w/2, h/2,0, // vertex 3
 -w/2, h/2,0, // vertex 4
        -w/2,-h/2,0 // vertex 1
        
       
      };
      GLfloat color_buffer_data [] = {
        0,col,1, // color 1
        0,col,1, // color 2
        0,col,1, // color 3


        0,col,1,  // color 1
        0,col,1, // color 3
        0,col,1, // color 4
      };

     
      if(type==3) 
        rectangle = create3DTexturedObject(GL_TRIANGLES, 6, vertex_buffer_data, texture_buffer_data, textureID[5], GL_FILL);
      else if(type == 1)
        rectangle = create3DTexturedObject(GL_TRIANGLES, 6, vertex_buffer_data, texture_buffer_data, textureID[6], GL_FILL);
      else if(type == 2)
        rectangle = create3DTexturedObject(GL_TRIANGLES, 6, vertex_buffer_data, texture_buffer_data, textureID[7], GL_FILL);




      //rectangle 
    }
    void fire(float boost) {
      vx = boost*10;
    }
    bool checkBelow(block t2){

      float l1x=x-w/2,l1y=y+h/2,r1x=x+w/2,r1y=y-h/2;
      float l2x=t2.x-t2.w/2,l2y=t2.y+t2.h/2,r2x=t2.x+t2.w/2,r2y=t2.y-t2.h/2;

      if(l2y>=r1y and l2y<=y and l2x<=r1x and l2x>=x)
        return true;
      if(l2y>=r1y and l2y<=y and r2x>=l1x and r2x<=x) 
        return true;

      if(l2y>=r1y and l2y<=y and l2x<=x and r2x>=x) 
        return true;

      if(l2y>=r1y and l2y<=y and t2.x>=l1x and t2.x<=r1x) 
        return true;

      return false;   


      //if(t2.y+t2.h/2>=y-h/2 && (t2.x+t2.w/2>=x-w/2 || t2.x-t2.w/2<=x+w/2)) return true;
        //        else return false;
      //if((t2.x+t2.w/2<x-w/2 or t2.x-t2.w/2>x+w/2)) return false;
      //else if()  or (t2.y-t2.h/2>y+h/2 or t2.x-t2.w/2>x+w/2 or t2.y+t2.h/2<y-h/2 ) return false;
      //return true;
      //checks if t2 is below this block
      /*float l1x=x-w/2,l1y=y+h/2,r1x=x+w/2,r1y=y-h/2;
        float l2x=t3.x-w/2,l2y=t3.y+h/2,r2x=t3.x+w/2,r2y=t3.y-h/2;

        if(l2y<r1y and r2x>l1x)
        return false;
        return true;*/
    }
    bool checkCollision(block t3) {
      float l1x=x-w/2,l1y=y+h/2,r1x=x+w/2,r1y=y-h/2;
      float l2x=t3.x-t3.w/2,l2y=t3.y+t3.h/2,r2x=t3.x+t3.w/2,r2y=t3.y-t3.h/2;

      if(t3.type == 3 && l1y >= l2y) return false;
     
      if (l1x > r2x || l2x > r1x)
        return false;
      if (l1y < r2y || l2y < r1y)
        return false;
      return true;

    }

    void update() {
      if(type==3) return;
      //cout<<vx<<vy<<fall<<endl;
      

      if(vx>0) {
        this->x += this->vx*0.01 - 0.5*drag*0.0001;
        this->vx -= drag*0.01;
      }
      //cout<<vx<< " "<<x<<endl;
      if(fall) {
        this->y -= abs(this->vy)*0.01 + 0.5*gravity*0.0001;
        this->vy -= gravity*0.01;
      }
      if(y-h/2<=groundLevel+1) {
        fall = 0;
        vy = 0;
        y = groundLevel+1 +h/2;
      }
    }
    int getLayer(){
      if(y+h/2<=-7) 
        return 1;
      if(y+h/2<=-6) 
        return 2;
      if(y+h/2<=-5) 
        return 3;
    }

    void draw () {
      if(alive) {
         glUseProgram (textureProgramID);
        glm::mat4 translateRectangle;


        glm::mat4 VP = Matrices.projection * Matrices.view;

        // Send our transformation to the currently bound shader, in the "MVP" uniform
        // For each model you render, since the MVP will be different (at least the M part)
        //  Don't change unless you are sure!!
        glm::mat4 MVP;  // MVP = Projection * View * Model

        // Load identity to model matrix
        Matrices.model = glm::mat4(1.0f);

        translateRectangle = glm::translate (glm::vec3(x, y, 0));        // glTranslatef
        glm::mat4 rotateRectangle = glm::rotate((float)(theta*M_PI/180.0f), glm::vec3(0,0,1)); // rotate about vector (-1,1,1)
        Matrices.model *= (translateRectangle * rotateRectangle);
        //Matrices.model *= (translateRectangle );

        MVP = VP * Matrices.model;
            glUniformMatrix4fv(Matrices.TexMatrixID, 1, GL_FALSE, &MVP[0][0]);
            glUniform1i(glGetUniformLocation(textureProgramID, "texSampler"), 0);
            draw3DTexturedObject(rectangle);
       
        // draw3DObject draws the VAO given to it using current MVP matrix
      
     //   theta -= boost*update_theta;
     //   if(theta <= -90) update_theta = 0;

        update();
      }
    }
};



