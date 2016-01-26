#include "globals.h"
#include "block.h"
#include "bird.h"
#include "canon.h"
#include "speedy.h"

int main (int argc, char** argv)
{


  int width = 1400;
  int height = 715;
  int numBlocks = 12;

  /*float v[][6] = {
    2.5, -8.6, 3, 0.5,0,1,
    4, -7, 1, 4,0,1,
    5.5, -8.6, 3, 0.5,0,1,
    2.5, -5.5, 2, 1,1,1,
    4, -5.5, 2, 1,1,1,

    5.5, -5.5, 2, 1,1,1,
    4, -9.5, 2.5, 1.5, 2,2,
    4, -10.5, 1, 4, 0, 3,
    9, -9, 4, 4, 0, 3
  };*/

    float v[][6] = {
    -8, -17, 3, 0.5,0,3,
    -3, -15, 4, 0.5,0,3,
    2, -13, 5, 0.5,0,3,
    7, -11, 6, 0.5,0,3,
    -8, -15, 1, 2,0,1,
    -3, -13, 1, 2,0,1,
    2, -10, 1, 2,0,1,
    7, -8, 1, 2,0,1,
    -8, -14, 1, 1,0,2,
    -3, -12, 1, 1,0,2,
    2, -9, 1, 1,0,2,
    7, -7, 1, 1,0,2
};

  /*  float v[][6] = {
    2, -10, 2, 2,0,3,
    6, -10, 2, 2,0,3,
    10, -10, 2, 2,0,3,
    3.2, -8.5, 1, 4,0,1,
    8.8, -8.5, 1, 4,0,1,
    6, -8.5, 1, 1,0,2,
    5.9, -7.5, 1, 6,0,1,
    6, -6.5, 1, 1,0,2

};*/

  GLFWwindow* window = initGLFW(width, height);

  initGL (window, width, height);

  double last_update_time = glfwGetTime(), current_time, init_time = 0;
  
  canon c;
  bird b(-12, groundLevel+2);
  speedy speedo;
  vector<block> arr;
  elements e;

  for(int i=0; i<numBlocks;++i){
    block a(v[i][0], v[i][1], v[i][2], v[i][3], v[i][4], v[i][5]);
    arr.push_back(a);
  }

  while (!glfwWindowShouldClose(window)) {
   /* if(init_time == 0) init_time = glfwGetTime();
    if(glfwGetTime() - init_time < 3) e.drawWelcome();
    if(glfwGetTime() - init_time >= 3) {
      if(onMenu) e.drawMenu();
    }*/
    //if(onMenu == false) 
      e.drawBackground();
   
    //e.draw();

    if(shoot == 1) {
      b.fire(boost, rectangle_rotation);
      shoot = 0;
    }

    c.draw();
    b.draw();

   

    for(int i=0; i<numBlocks;++i){
      bool flag=false;
     // if(i==1)
    //  cout<<i<<" "<<arr[i].vy<<" "<<arr[i].fall<<endl;
      for(int j=0; j<numBlocks;++j) {
        //if(i==1 and j==4)
        //cout<<arr[i].checkBelow(arr[j])<<" "<<;
        if(arr[i].checkBelow(arr[j])==true and i!=j){
          flag=true;
          
           //cout<<i<<" "<<j<<endl;

          //  if(arr[j].t==2 && arr[i].t==1) arr[j].lives--;
        }
        else if(arr[i].checkBelow(arr[j])==false and i!=j)
            {
                         }

      }
      if(flag==false)
        arr[i].fall=1;
      else arr[i].fall = 0,arr[i].vy=0;
      if(i<=3)
        arr[i].fall = 0,arr[i].vy=0;
      arr[i].draw();
       // if(i==4) cout<<arr[i].vy<<" "<<arr[i].fall<<" "<<arr[i].y<<endl;

    }

    for(int i=4; i<numBlocks;++i) {
      for(int j=0; j<numBlocks;++j) {
        if(arr[i].checkCollision(arr[j]) && i!=j) {
          if(arr[j].type==1 && arr[i].type==1) {
          arr[i].vx = arr[j].vx = (arr[i].vx+arr[j].vx)/2;
         // arr[i].vy = arr[j].vy = (arr[i].vy+arr[j].vy)/2;
        }
   //     else if(arr[j].type == 3) {
          //cout<<"hi"<<endl;
    //      arr[i].vx = -coeff*arr[i].vx;
     // }
    }
    }
  }


    int hit,diff;

    for(int i=0; i<numBlocks;++i) {
      if(b.checkCollision(arr[i].x, arr[i].y, arr[i].h, arr[i].w, arr[i].alive))
      {
        if(arr[i].type == 2) {
          points+=10;
          arr[i].alive = false;
        }
        if(arr[i].type == 1) {
          arr[i].fire(scale*2);
        }
        break;
      }
    }

    /*if(arr[i].t==2) {

      arr[i].lives=0;
      points+=arr[i].score;

      }*/
    /*  hit=arr[i].getLayer();
    //arr[i].fire(1);
    for(int j=0; j<numBlocks;++j){
    diff=(arr[j].getLayer()-hit);
    if(diff==0)
    arr[j].fire(scale*1.0);
    else if (diff==1) 
    arr[j].fire(scale*0.5);
    else if (diff==2) 
    arr[j].fire(scale*0.2);
    }
    break;  */


    speedo.draw();



    // Swap Frame Buffer in double buffering
    glfwSwapBuffers(window);

    // Poll for Keyboard and mouse events
    glfwPollEvents();

    // Control based on time (Time based transformation like 5 degrees rotation every 0.5s)
    current_time = glfwGetTime(); // Time in seconds
    if ((current_time - last_update_time) >= 0.5) { // atleast 0.5s elapsed since last frame
      // do something every 0.5 seconds ..
      last_update_time = current_time;
    }

 // }
}
  glfwTerminate();
  exit(EXIT_SUCCESS);
}
