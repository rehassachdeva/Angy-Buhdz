#include "globals.h"
#include "block.h"
#include "bird.h"
#include "canon.h"
#include "speedy.h"

#include <AL/al.h>
#include <AL/alc.h>

#ifdef LIBAUDIO
#include <audio/wave.h>
#define BACKEND "libaudio"
#else
#include <AL/alut.h>
#define BACKEND "alut"
#endif

bool hitFlag = false;

static void list_audio_devices(const ALCchar *devices)
{
  const ALCchar *device = devices, *next = devices + 1;
  size_t len = 0;

  fprintf(stdout, "Devices list:\n");
  fprintf(stdout, "----------\n");
  while (device && *device != '\0' && next && *next != '\0') {
    fprintf(stdout, "%s\n", device);
    len = strlen(device);
    device += (len + 1);
    next += (len + 2);
  }
  fprintf(stdout, "----------\n");
}

#define TEST_ERROR(_msg)    \
  error = alGetError();   \
  if (error != AL_NO_ERROR) { \
    fprintf(stderr, _msg "\n"); \
    return -1;    \
  }



static inline ALenum to_al_format(short channels, short samples)
{
  bool stereo = (channels > 1);

  switch (samples) {
  case 16:
    if (stereo)
      return AL_FORMAT_STEREO16;
    else
      return AL_FORMAT_MONO16;
  case 8:
    if (stereo)
      return AL_FORMAT_STEREO8;
    else
      return AL_FORMAT_MONO8;
  default:
    return -1;
  }
}




int main (int argc, char** argv)
{
 ALboolean enumeration;
  const ALCchar *devices;
  const ALCchar *defaultDeviceName = argv[1];
  int ret;
#ifdef LIBAUDIO
  WaveInfo *wave;
#endif
  char *bufferData;
  ALCdevice *device;
  ALvoid *data;
  ALCcontext *context;
  ALsizei size, freq;
  ALenum format;
  ALuint buffer, source;
  ALfloat listenerOri[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };
  ALboolean loop = AL_FALSE;
  ALCenum error;
  ALint source_state;

  fprintf(stdout, "Using " BACKEND " as audio backend\n");

  enumeration = alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT");
  if (enumeration == AL_FALSE)
    fprintf(stderr, "enumeration extension not available\n");

  list_audio_devices(alcGetString(NULL, ALC_DEVICE_SPECIFIER));

  if (!defaultDeviceName)
    defaultDeviceName = alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);

  device = alcOpenDevice(defaultDeviceName);
  if (!device) {
    fprintf(stderr, "unable to open default device\n");
    return -1;
  }

  fprintf(stdout, "Device: %s\n", alcGetString(device, ALC_DEVICE_SPECIFIER));

  alGetError();

  context = alcCreateContext(device, NULL);
  if (!alcMakeContextCurrent(context)) {
    fprintf(stderr, "failed to make default context\n");
    return -1;
  }
  TEST_ERROR("make default context");

  /* set orientation */
  alListener3f(AL_POSITION, 0, 0, 1.0f);
  TEST_ERROR("listener position");
      alListener3f(AL_VELOCITY, 0, 0, 0);
  TEST_ERROR("listener velocity");
  alListenerfv(AL_ORIENTATION, listenerOri);
  TEST_ERROR("listener orientation");

  alGenSources((ALuint)1, &source);
  TEST_ERROR("source generation");

  alSourcef(source, AL_PITCH, 1);
  TEST_ERROR("source pitch");
  alSourcef(source, AL_GAIN, 1);
  TEST_ERROR("source gain");
  alSource3f(source, AL_POSITION, 0, 0, 0);
  TEST_ERROR("source position");
  alSource3f(source, AL_VELOCITY, 0, 0, 0);
  TEST_ERROR("source velocity");
  alSourcei(source, AL_LOOPING, AL_TRUE);
  TEST_ERROR("source looping");

  alGenBuffers(1, &buffer);
  TEST_ERROR("buffer generation");

#ifdef LIBAUDIO
  /* load data */
  wave = WaveOpenFileForReading("test.wav");
  if (!wave) {
    fprintf(stderr, "failed to read wave file\n");
    return -1;
  }

  ret = WaveSeekFile(0, wave);
  if (ret) {
    fprintf(stderr, "failed to seek wave file\n");
    return -1;
  }

  bufferData = malloc(wave->dataSize);
  if (!bufferData) {
    perror("malloc");
    return -1;
  }

  ret = WaveReadFile(bufferData, wave->dataSize, wave);
  if (ret != wave->dataSize) {
    fprintf(stderr, "short read: %d, want: %d\n", ret, wave->dataSize);
    return -1;
  }

  alBufferData(buffer, to_al_format(wave->channels, wave->bitsPerSample),
      bufferData, wave->dataSize, wave->sampleRate);
  TEST_ERROR("failed to load buffer data");
#else
  alutLoadWAVFile((ALbyte* )"test.wav", &format, &data, &size, &freq, &loop);
  TEST_ERROR("loading wav file");

  alBufferData(buffer, format, data, size, freq);
  TEST_ERROR("buffer copy");
#endif

  alSourcei(source, AL_BUFFER, buffer);
  TEST_ERROR("buffer binding");

  alSourcePlay(source);
  TEST_ERROR("source playing");




  int width = 1400;
  int height = 715;
  int numBlocks = 29;

  float v[][6] = {
    -8, -17, 3, 0.5,0,3,
    -3, -15, 4, 0.5,0,3,
    2, -13, 5, 0.5,0,3,
    7, -11, 6, 0.5,0,3,
    12, -9, 6, 0.5,0,3,
    17, -7, 6, 0.5,0,3,

    -8, -15, 1, 3,0,1,
    -3, -13, 1, 3,0,1,
    2, -10, 1, 3,0,1,
    7, -8, 1, 3,0,1,
    12, -6, 1, 3,0,1,
    17, -4, 1, 3,0,1,

    -8, -13, 2, 2,0,2,
    -3, -11, 2, 2,0,2,
    2, -8, 2, 2,0,2,
    7, -6, 2, 2,0,2,
    12, -4, 2, 2,0,2,
    17, -2, 2, 2,0,2,

    -6, -11, 1.5, 1.5, 0,4,
    -13, -10, 1.5, 1.5, 0,4,
    5, -13, 1.5, 1.5, 0,4,
    -16, -16, 1.5, 1.5, 0,4,
    -22, -10, 1.5, 1.5, 0,4,
    0  , -16, 1.5, 1.5, 0,4,
    8  , -18, 1.5, 1.5, 0,4,
    14  , -15, 1.5, 1.5, 0,4,
    20  , -18, 1.5, 1.5, 0,4,
    26  , -12, 1.5, 1.5, 0,4,
    15, -9, 1.5, 1.5, 0, 4

  };

  GLFWwindow* window = initGLFW(width, height);

  initGL (window, width, height);

  double last_update_time = glfwGetTime(), current_time, init_time = 0;

  canon c;
  bird b(canonX, canonY);
  speedy s;
  vector<block> arr;
  elements e;

  for(int i=0; i<numBlocks;++i){
    block a(v[i][0], v[i][1], v[i][2], v[i][3], v[i][4], v[i][5]);
    arr.push_back(a);
  }
  int cnt = 0;
  while (!glfwWindowShouldClose(window)) {
    if(init_time == 0) init_time =  glfwGetTime();
    if( glfwGetTime() - init_time <1.75) {
      e.drawWelcome();
    }
    else if(onMenu) {
    e.drawMenu();
    }
   

    if(onMenu == false) {
      e.drawBackground();
    c.draw();
    b.draw();
    s.draw();

    if(shoot == 1) {
      hitFlag = false;
      b.fire(boost, rectangle_rotation);

    }

    for(int i=0; i<numBlocks;++i){
      bool flag=false;
      for(int j=0; j<numBlocks;++j) {
        if(arr[i].checkBelow(arr[j])==true and i!=j){
          flag=true;
        }
      }
      if(flag==false && i>3) {
        if(arr[i].fall!=1 && arr[i].type==2) arr[i].score-=50;
        arr[i].fall=1;

      }
      else arr[i].fall = 0, arr[i].vy=0;
      arr[i].draw();
    }

    for(int i=4; i<numBlocks;++i) {
      for(int j=0; j<numBlocks;++j) {
        if(arr[i].checkCollision(arr[j]) && i!=j) {
          if(arr[j].type==1 && arr[i].type==1) {
            arr[i].vx = arr[j].vx = (arr[i].vx+arr[j].vx)/2;
          }
        }
      }
    }

    if(b.x>=-16.4&&b.x<=-13.6&&b.y>=1.5&&b.y<=5.5) {
      hit[0] = true;
      magic += rand()%100;
    }
    if(b.x>=-12.4&&b.x<=-9.6&&b.y>=1.5&&b.y<=5.5) {
      hit[1] = true;
      magic += rand()%100;
    }
    if(b.x>=-8.4&&b.x<=-5.6&&b.y>=1.5&&b.y<=5.5) {
      hit[2] = true;
      magic += rand()%100;

    }


    int hit,diff;

    for(int i=0; i<numBlocks;++i) {
      if(b.checkCollision(arr[i].x, arr[i].y, arr[i].h, arr[i].w, arr[i].alive))
      {
        if(arr[i].type == 2) {
          points+=arr[i].score;
          arr[i].alive = false;
        }
        if(arr[i].type == 1) {
          arr[i].fire(scale*2);
        }
        if(arr[i].type==4) {
          if(!hitFlag) {
            lives--;
            hitFlag = true;
          }

          if(lives == 0) {
            lives = 3;
            points = 0;magic = 0;
            arr.clear();
             for(int i=0; i<numBlocks;++i){
    block a(v[i][0], v[i][1], v[i][2], v[i][3], v[i][4], v[i][5]);
    arr.push_back(a);
  }
          }
        }
        break;
      }
    }
  }
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
  }
  
  alGetSourcei(source, AL_SOURCE_STATE, &source_state);
  TEST_ERROR("source state get");
  while (source_state == AL_PLAYING) {
    alGetSourcei(source, AL_SOURCE_STATE, &source_state);
    TEST_ERROR("source state get");
  }

  


  /* exit context */
  alDeleteSources(1, &source);
  alDeleteBuffers(1, &buffer);
  device = alcGetContextsDevice(context);
  alcMakeContextCurrent(NULL);
  alcDestroyContext(context);
  alcCloseDevice(device);
  glfwTerminate();
  exit(EXIT_SUCCESS);
}
