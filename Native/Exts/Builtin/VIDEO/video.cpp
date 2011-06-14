/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/EDK/EDK.h>

using namespace Fabric::EDK;

#include <map>

extern "C" {
#include <stdint.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
}

class videoStream
{
private:
  static bool sRegistered;
  AVFormatContext * mFormatCtx;
  AVCodecContext * mCodecCtx;
  int mVideoStreamID;
  AVCodec * mCodec;
  AVFrame * mFrameYUV;
  AVFrame * mFrameRGB;
  uint8_t * mBuffer;
  KL::Scalar mTime;
  SwsContext * mConvertCtx;

public:
    
  videoStream()
  {
    if(!sRegistered)
    {
      av_register_all();
      sRegistered = true;
    }
    
    mFormatCtx = NULL;
    mCodecCtx = NULL;
    mVideoStreamID = -1;
    mCodec = NULL;
    mFrameYUV = NULL;
    mFrameRGB = NULL;
    mBuffer = NULL;
    mTime = -1000.0;
    mConvertCtx = NULL;
  }
  
  ~videoStream()
  {
    av_free(mBuffer);
    av_free(mFrameRGB);
    av_free(mFrameYUV);
    avcodec_close(mCodecCtx);
    av_close_input_file(mFormatCtx);
  }
  
  KL::Size getWidth()
  {
    return mCodecCtx ? mCodecCtx->width : 0;
  }
  
  KL::Size getHeight()
  {
    return mCodecCtx ? mCodecCtx->height : 0;
  }
  
  KL::Scalar getDuration()
  {
    if(!mFormatCtx)
      return 0;
    return KL::Scalar(mFormatCtx->duration) / KL::Scalar(AV_TIME_BASE);
  }

  KL::Scalar getFPS()
  {
    return KL::Scalar(mFormatCtx->streams[mVideoStreamID]->r_frame_rate.num) / KL::Scalar(mFormatCtx->streams[mVideoStreamID]->r_frame_rate.den);
  }
  
  KL::Scalar getTime()
  {
    return mTime;
  }
  
  bool init(KL::String & filename)
  {
    if(filename.data()== NULL){
      printf("=======> VIDEO-ERROR: No filename specified.......\n");
      return false;
    }
    
    // open video file
    if(av_open_input_file(&mFormatCtx, filename.data(), NULL, 0, NULL)!=0)
    {
      printf("=======> VIDEO-ERROR: Cannot open video file '%s'.\n",filename.data());
      return false;
    }

    // retrieve stream information
    if(av_find_stream_info(mFormatCtx)<0)
    {
      printf("=======> VIDEO-ERROR: Cannot read stream information in '%s'.\n",filename.data());
      return false;
    }

    // find the first video stream
    for(int i=0; i<mFormatCtx->nb_streams; i++)
    {
      if(mFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO)
      {
        mVideoStreamID=i;
        break;
      }
    }
    if(mVideoStreamID==-1)
    {
      printf("=======> VIDEO-ERROR: Didn't find a video stream in '%s'.\n",filename.data());
      return false;
    }
    
    // get a pointer to the codec context for the video stream
    mCodecCtx = mFormatCtx->streams[mVideoStreamID]->codec;
    
    // find the decoder for the video stream
    mCodec = avcodec_find_decoder(mCodecCtx->codec_id);
    if(mCodec==NULL)
    {
      printf("=======> VIDEO-ERROR: Unsupported codec in '%s'.\n",filename.data());
      return false;
    }
    
    // open codec
    if(avcodec_open(mCodecCtx, mCodec)<0)
    {
      printf("=======> VIDEO-ERROR: Could not open (supported) codec for '%s'.\n",filename.data());
      return false;
    }
    
    // allocate video frame (for YUV and RGB)
    mFrameYUV = avcodec_alloc_frame();
    mFrameRGB = avcodec_alloc_frame();

    // determine required buffer size and allocate buffer
    int numBytes = avpicture_get_size( PIX_FMT_RGB24, getWidth(),getHeight());
    mBuffer = (uint8_t *)av_malloc(numBytes*sizeof(uint8_t));
    
    // check if we received the memory
    if(!mFrameRGB || !mFrameYUV || !mBuffer)
    {
      printf("=======> VIDEO-ERROR: Couldn't allocate buffer in '%s'.\n",filename.data());
      return false;
    }

    // connect the picture frame with the buffer      
    avpicture_fill((AVPicture *)mFrameRGB, mBuffer, PIX_FMT_RGB24, getWidth(), getHeight());      

    return true;
  }
  
  bool readNextFrame(KL::Boolean &loop)
  {
    AVPacket packet;
    unsigned int i=0;
    int frameFinished = 0;
    bool result = false;
    while(av_read_frame(mFormatCtx, &packet)>=0)
    {
      // is this a packet from the video stream?
      if(packet.stream_index == mVideoStreamID)
      {
        // decode video frame
        avcodec_decode_video2(mCodecCtx, mFrameYUV, &frameFinished, &packet);
        
        // did we get a complete video frame?
        if(frameFinished != 0)
        {
          // let's see if we need to create a conversion context
          if(!mConvertCtx)
          {
            mConvertCtx = sws_getContext(getWidth(), getHeight(), mCodecCtx->pix_fmt, getWidth(), getHeight(), PIX_FMT_RGB24, SWS_BICUBIC, NULL, NULL, NULL);
            if(!mConvertCtx)
            {
              printf("=======> VIDEO-ERROR: Could not create sws_conversion object.\n");
              return false;
            }
          }

          // convert the image from its native format to RGB
          sws_scale(mConvertCtx, mFrameYUV->data, mFrameYUV->linesize, 0, getHeight(), mFrameRGB->data, mFrameRGB->linesize);
          result = true;
          break;
        }
      }
      
      // Free the packet that was allocated by av_read_frame
      av_free_packet(&packet);
    }
    
    // if we didn't find a frame (we are at the end)
    if(!result && loop)
      av_seek_frame(mFormatCtx,mVideoStreamID,0,0);
    else
      mTime += 1.0 / getFPS();

    return result;
  }

  bool seekTime(KL::Scalar &time)
  {
    if(fabs(time - mTime) < 0.0000001)
      return true;

    if(fabs(time - mTime) > 1.0 / getFPS() + 0.0001)
    {
      int64_t frame = (int64_t)(0.5 * time * KL::Scalar(mCodecCtx->time_base.den) / KL::Scalar(mCodecCtx->time_base.num));
      av_seek_frame(mFormatCtx,mVideoStreamID,frame,0);
      mTime = time;
    }
    
    KL::Boolean loop = false;
    return readNextFrame(loop);
  }
  
  bool getPixel(KL::Size &x,KL::Size &y,KL::RGB &pixel)
  {
    if(x >= getWidth() || y >= getHeight())
      return false;
    
    KL::Size offset = x*3 + y*mFrameRGB->linesize[0];
    pixel.r = (int)mFrameRGB->data[0][offset++];
    pixel.g = (int)mFrameRGB->data[0][offset++];
    pixel.b = (int)mFrameRGB->data[0][offset++];

    return true;
  }
  
  bool getAllPixels(KL::Size &width, KL::Size &height, KL::VariableArray<KL::RGB> &pixels)
  {
    width = getWidth();
    height = getHeight();
    pixels.resize(getHeight() * getWidth());
    memcpy(&pixels[0],mFrameRGB->data[0],sizeof(pixels[0]) * getHeight() * getWidth());
    return true;
  }
  
};
bool videoStream::sRegistered = false;

std::map<KL::Size,videoStream*> gvideoStreams;
KL::Size gLastStream = 0;
videoStream * gLastVideo = NULL;

videoStream * getStream(KL::Size &stream)
{
  if(gLastStream == stream)
    return gLastVideo;
  
  std::map<KL::Size,videoStream*>::iterator it = gvideoStreams.find(stream);
  if(it == gvideoStreams.end())
  {
    printf("=======> VIDEO-ERROR: Unable to find stream %d.\n",(int)stream);
    return NULL;
  }
  return it->second;
}

FABRIC_EXT_EXPORT void FabricVIDEOOpenStream(
  KL::String &filename,
  KL::Size &stream,
  KL::Size &width,
  KL::Size &height,
  KL::Scalar &duration,
  KL::Scalar &fps
  )
{
  if(stream == 0)
  {
    // let's find out the highest stream key
    for(std::map<KL::Size,videoStream*>::iterator it = gvideoStreams.begin(); it != gvideoStreams.end(); it++)
    {
      if(it->first > stream)
        stream = it->first;
    }
    stream++;

    // init the stream
    videoStream * video = new videoStream();
    if(!video){
      printf("=======> VIDEO-ERROR: Could not create videoStream object.\n");
      stream = 0;
      return;
    }
    if(!video->init(filename))
    {
      delete(video);
      stream = 0;
      
      return;
    }
    
    // extract the frame information
    width = video->getWidth();
    height = video->getHeight();
    duration = video->getDuration();
    fps = video->getFPS();
    
    // out of the sequence to later then access single frames....! :)
    gvideoStreams.insert(std::pair<KL::Size,videoStream*>(stream,video));
  }
}

FABRIC_EXT_EXPORT void FabricVIDEOLoadNextFrame(
  KL::Size &stream,
  KL::Boolean &loop
  )
{
  videoStream * video = getStream(stream);
  if(!video)
    return;
  video->readNextFrame(loop);
}

FABRIC_EXT_EXPORT void FabricVIDEOSeekTime(
  KL::Size &stream,
  KL::Scalar &time
  )
{
  videoStream * video = getStream(stream);
  if(!video)
    return;
  video->seekTime(time);
}

FABRIC_EXT_EXPORT void FabricVIDEOGetPixel(
  KL::Size &stream,
  KL::Size &x,
  KL::Size &y,
  KL::RGB &pixel
  )
{
  videoStream * video = getStream(stream);
  if(!video)
    return;
  video->getPixel(x,y,pixel);
}

FABRIC_EXT_EXPORT void FabricVIDEOGetAllPixels(
  KL::Size &stream,
  KL::Size &width,
  KL::Size &height,
  KL::VariableArray<KL::RGB> &pixels
  )
{
  videoStream * video = getStream(stream);
  if(!video)
    return;
  video->getAllPixels(width,height,pixels);
}
