/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/EDK/EDK.h>

using namespace Fabric::EDK;
IMPLEMENT_FABRIC_EDK_ENTRIES

#include <stdint.h>

#ifdef _WIN32
  #ifndef INT64_C
    #define INT64_C(c) (c ## LL)
    #define UINT64_C(c) (c ## ULL)
  #endif
#endif

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
}

static AVFrame *alloc_picture(enum PixelFormat pix_fmt, int width, int height)
{
  AVFrame *picture;
  uint8_t *picture_buf;
  int size;
  
  picture = avcodec_alloc_frame();
  if (!picture)
    return NULL;
  size = avpicture_get_size(pix_fmt, width, height);
  picture_buf = (uint8_t*)av_malloc(size);
  if (!picture_buf) {
    av_free(picture);
    return NULL;
  }
  avpicture_fill((AVPicture *)picture, picture_buf,
  pix_fmt, width, height);
  return picture;
}

class videoStream;

const float cFrameSkipTolerance = 0.005f;

struct videoHandle
{
  videoStream * pointer;
  KL::Size width;
  KL::Size height;
  KL::Scalar duration;
  KL::Scalar fps;
  KL::Scalar time;
};

class videoStream
{
private:
  static bool sRegistered;
  bool mReadOnly;
  AVFormatContext * mFormatCtx;
  AVCodecContext * mCodecCtx;
  AVStream * mVideoStream;
  int mVideoStreamID;
  AVCodec * mCodec;
  AVFrame * mFrameYUV;
  AVFrame * mFrameRGB;
  uint8_t * mBuffer;
  SwsContext * mConvertCtx;
  videoHandle * mHandle;
  uint8_t * mVideoBuffer;
  size_t mVideoBufferSize;

public:
    
  videoStream(videoHandle * in_Handle)
  {
    mHandle = in_Handle;
    
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
    mConvertCtx = NULL;
    mVideoBuffer = NULL;
    mVideoBufferSize = 0;
  }
  
  ~videoStream()
  {
    if(mReadOnly)
    {
      av_free(mBuffer);
      av_free(mFrameRGB);
      av_free(mFrameYUV);
      avcodec_close(mCodecCtx);
      av_close_input_file(mFormatCtx);
    }
    else
    {
      av_write_trailer(mFormatCtx);

      avcodec_close(mVideoStream->codec);
      av_free(mFrameRGB->data[0]);
      av_free(mFrameRGB);
      av_free(mVideoBuffer);

      /* free the streams */
      for(size_t i = 0; i < mFormatCtx->nb_streams; i++) {
        av_freep(&mFormatCtx->streams[i]->codec);
        av_freep(&mFormatCtx->streams[i]);
      }
      
      if (!(mFormatCtx->oformat->flags & AVFMT_NOFILE)) {
        /* close the output file */
        url_fclose(mFormatCtx->pb);
      }
      
      /* free the format ctx */
      av_free(mFormatCtx);

    }
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
    if(!mReadOnly)
      return 0;
    if(!mFormatCtx)
      return 0;
    return KL::Scalar(mFormatCtx->duration) / KL::Scalar(AV_TIME_BASE);
  }

  KL::Scalar getFPS()
  {
    if(!mReadOnly)
      return 0;
    return KL::Scalar(mFormatCtx->streams[mVideoStreamID]->r_frame_rate.num) / KL::Scalar(mFormatCtx->streams[mVideoStreamID]->r_frame_rate.den);
  }
  
  KL::Scalar getTime()
  {
    if(!mReadOnly)
      return 0;
    return mHandle->time;
  }
  
  bool init(KL::String & filename, bool in_readOnly, KL::Size width = 0, KL::Size height = 0)
  {
    mReadOnly = in_readOnly;
    if(mReadOnly)
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
      for(int i=0; i<int(mFormatCtx->nb_streams); i++)
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
      
      // init the handle's values
      mHandle->time = -10000.0;
      mHandle->width = getWidth();
      mHandle->height = getHeight();
      mHandle->duration = getDuration();
      mHandle->fps = getFPS();
    }
    else
    {
      // init the video for writing
      AVOutputFormat * fmt = av_guess_format("h264",NULL,NULL);
      if(!fmt)
      {
        Fabric::EDK::throwException("Video Extension: h264 format not found!");
        return false;
      }
      mFormatCtx = avformat_alloc_context();
      mFormatCtx->oformat = fmt;
      
      // store the filename
      snprintf(mFormatCtx->filename, sizeof(mFormatCtx->filename), "%s", filename.data());
      
      // create a video stream
      mVideoStream = av_new_stream(mFormatCtx, 0);

      mCodecCtx = mVideoStream->codec;
      mCodecCtx->codec_id = fmt->video_codec;
      mCodecCtx->codec_type = AVMEDIA_TYPE_VIDEO;

      mCodecCtx->bit_rate = 400000;
      mCodecCtx->width = width;
      mCodecCtx->height = height;
      /* time base: this is the fundamental unit of time (in seconds) in terms
         of which frame timestamps are represented. for fixed-fps content,
         timebase should be 1/framerate and timestamp increments should be
         identically 1. */
      mCodecCtx->time_base.den = 25;
      mCodecCtx->time_base.num = 1;
      mCodecCtx->gop_size = 12; /* emit one intra frame every twelve frames at most */
      mCodecCtx->pix_fmt = PIX_FMT_RGB24;

      // some formats want stream headers to be separate
      if(fmt->flags & AVFMT_GLOBALHEADER)
        mCodecCtx->flags |= CODEC_FLAG_GLOBAL_HEADER;      

      /* set the output parameters (must be done even if no
         parameters). */
      if (av_set_parameters(mFormatCtx, NULL) < 0) {
        Fabric::EDK::throwException("Video Extension: Invalid output format parameters!");
        return false;
      }
      //av_dump_format(mCodecCtx, 0, filename.data(), 1);
      
      /* find the video encoder */
      AVCodec * codec = avcodec_find_encoder(mCodecCtx->codec_id);
      if (!codec) {
        Fabric::EDK::throwException("Video Extension: Codec not found!");
        return false;
      }

      /* open the codec */
      if (avcodec_open(mCodecCtx, codec) < 0) {
        Fabric::EDK::throwException("Video Extension: Could not open codec!");
        return false;
      }

      mVideoBuffer = NULL;
      if (!(mFormatCtx->oformat->flags & AVFMT_RAWPICTURE)) {
        /* allocate output buffer */
        /* XXX: API change will be done */
        /* buffers passed into lav* can be allocated any way you prefer,
        as long as they're aligned enough for the architecture, and
        they're freed appropriately (such as using av_free for buffers
        allocated with av_malloc) */
        mVideoBufferSize = 200000;
        mVideoBuffer = (uint8_t*)av_malloc(mVideoBufferSize);
      }

      /* allocate the encoded raw picture */
      mFrameRGB = alloc_picture(mCodecCtx->pix_fmt, mCodecCtx->width, mCodecCtx->height);
      if (!mFrameRGB) {
        Fabric::EDK::throwException("Video Extension: Could not allocate RGB picture!");
        return false;
      }

      /* open the output file, if needed */
      if (!(fmt->flags & AVFMT_NOFILE)) {
        if (url_fopen(&mFormatCtx->pb, filename.data(), URL_WRONLY) < 0) {
          Fabric::EDK::throwException("Video Extension: Could not open output file!");
          return false;
        }
      }
      
      /* write the stream header, if any */
      av_write_header(mFormatCtx);
    }

    return true;
  }
  
  bool readNextFrame(KL::Boolean &loop)
  {
    if(!mReadOnly)
      return false;
    AVPacket packet;
    av_init_packet(&packet);
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
      mHandle->time += 1.0 / getFPS();

    return result;
  }

  bool seekTime(KL::Scalar &time)
  {
    if(!mReadOnly)
      return false;
    KL::Boolean loop = false;

    if(fabs(time - mHandle->time) < cFrameSkipTolerance)
      return true;

    if(fabs(time - mHandle->time) > 1.0 / getFPS() + cFrameSkipTolerance)
    {
      if(fabs(time - mHandle->time) < 2.0 / getFPS() + cFrameSkipTolerance)
      {
        readNextFrame(loop);
      }
      else
      {
        int64_t frame = (int64_t)(0.5 * (time - 1.0 / getFPS()) * KL::Scalar(mCodecCtx->time_base.den) / KL::Scalar(mCodecCtx->time_base.num));
        av_seek_frame(mFormatCtx,mVideoStreamID,frame,0);
        mHandle->time = time - 1.0 / getFPS();
      }
    }
    
    return readNextFrame(loop);
  }
  
  bool getAllPixels(KL::VariableArray<KL::RGB> &pixels)
  {
    if(!mReadOnly)
      return false;
    mHandle->width = getWidth();
    mHandle->height = getHeight();
    pixels.resize(getHeight() * getWidth());
    memcpy(&pixels[0],mFrameRGB->data[0],sizeof(KL::RGB) * getHeight() * getWidth());
    return true;
  }

  bool writeFrame(KL::VariableArray<KL::RGB> &pixels)
  {
    if(mReadOnly)
      return false;
    
    if(pixels.size() != getWidth() * getHeight())
      return false;
    memcpy(mFrameRGB->data[0],&pixels[0],sizeof(KL::RGB) * getHeight() * getWidth());

    /* encode the image */
    size_t out_size = avcodec_encode_video(mCodecCtx, mVideoBuffer, mVideoBufferSize, mFrameRGB);
    /* if zero size, it means the image was buffered */
    if (out_size > 0) {
      AVPacket pkt;
      av_init_packet(&pkt);

      if (mCodecCtx->coded_frame->pts != AV_NOPTS_VALUE)
        pkt.pts= av_rescale_q(mCodecCtx->coded_frame->pts, mCodecCtx->time_base, mVideoStream->time_base);
      if(mCodecCtx->coded_frame->key_frame)
        pkt.flags |= AV_PKT_FLAG_KEY;
      pkt.stream_index= mVideoStream->index;
      pkt.data = mVideoBuffer;
      pkt.size = out_size;

      /* write the compressed frame in the media file */
      av_interleaved_write_frame(mFormatCtx, &pkt);
    }
  }
  
};
bool videoStream::sRegistered = false;

FABRIC_EXT_EXPORT void FabricVIDEOOpenFileName(
  KL::String filename,
  videoHandle &handle
)
{
  if(handle.pointer == NULL)
  {
    // init the stream
    handle.pointer = new videoStream(&handle);
    if(!handle.pointer){
      printf("=======> VIDEO-ERROR: Could not create videoStream object.\n");
      handle.pointer = NULL;
      return;
    }
    if(!handle.pointer->init(filename,true))
    {
      delete(handle.pointer);
      handle.pointer = NULL;
      return;
    }
  }
}

FABRIC_EXT_EXPORT void FabricVIDEOOpenResource(
  KL::Data resourceData,
  KL::Size resourceDataSize,
  videoHandle &handle
)
{
  if(handle.pointer == NULL && resourceData != NULL && resourceDataSize > 0)
  {
#if defined(FABRIC_OS_WINDOWS)
    char const *dir = getenv("APPDATA");
    if(dir == NULL)
      dir = getenv("TEMP");
    if(dir == NULL)
      dir = getenv("TMP");
    if(dir == NULL)
      Fabric::EDK::throwException("Alembic extension: environment variable APP_DATA or TMP or TEMP is undefined");
    KL::String fileName( _tempnam( dir, "tmpfab_" ) );
#else
    KL::String fileName(tmpnam(NULL));
#endif

    // save the file to disk
    FILE * file = fopen(fileName.data(),"wb");
    fwrite(resourceData,resourceDataSize,1,file);
    fclose(file);
    file = NULL;
    
    return FabricVIDEOOpenFileName(fileName,handle);
  }
}

FABRIC_EXT_EXPORT void FabricVIDEOOpenFileHandle(
  KL::String file,
  videoHandle &handle
)
{
  if(handle.pointer == NULL)
  {
    KL::FileHandleWrapper wrapper(file);
    return FabricVIDEOOpenFileName(wrapper.getPath(),handle);
  }
}

FABRIC_EXT_EXPORT void FabricVIDEOSeekTime(
  videoHandle & handle,
  KL::Scalar &time
)
{
  if(handle.pointer == NULL)
    return;
  handle.pointer->seekTime(time);
}

FABRIC_EXT_EXPORT void FabricVIDEOCreateFromFileHandle(
  KL::String file,
  KL::Size width,
  KL::Size height,
  videoHandle &handle
)
{
  if(handle.pointer == NULL)
  {
    KL::FileHandleWrapper wrapper(file);

    // init the stream
    handle.pointer = new videoStream(&handle);
    if(!handle.pointer){
      printf("=======> VIDEO-ERROR: Could not create videoStream object.\n");
      handle.pointer = NULL;
      return;
    }
    KL::String filePath = wrapper.getPath();
    if(!handle.pointer->init(filePath,false,width,height))
    {
      delete(handle.pointer);
      handle.pointer = NULL;
      return;
    }
  }
}

FABRIC_EXT_EXPORT void FabricVIDEOGetAllPixels(
  videoHandle & handle,
  KL::VariableArray<KL::RGB> &pixels
)
{
  if(handle.pointer == NULL)
    return;
  handle.pointer->getAllPixels(pixels);
}

FABRIC_EXT_EXPORT void FabricVIDEOWriteAllPixels(
  videoHandle & handle,
  KL::VariableArray<KL::RGB> &pixels
)
{
  if(handle.pointer == NULL)
    return;
  handle.pointer->writeFrame(pixels);
}

FABRIC_EXT_EXPORT void FabricVIDEOFreeHandle(
  videoHandle &handle
)
{
  if(handle.pointer != NULL) {
    delete(handle.pointer);
    handle.pointer = NULL;
  }
}
