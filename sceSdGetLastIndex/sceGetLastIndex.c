//sceSdGetLastIndex kxploit partial reverse by GBOT

#define ERROR_KERNEL_ADDRESS -259
#define ERROR_SEMA -260
#define ERROR -1026

typedef struct _pspChnnlsvContext1 //taken from pspsdk
{
   int   mode;
   char buffer1[0x10];
   char buffer2[0x10];
   int   count;
} pspChnnlsvContext1;

static SceUID global_semaphore; // *(10964);
static char * global_data; // *(6724);
static char * global_data2; //6784

int sceSdGetLastIndex(pspChnnlsvContext1 * ctx, unsigned char * hash, unsigned char * cryptkey)
{
   char data[16];
   char t0, v0;
   int fp;
   
   //filters kernel addresses
   if(((hash + 16) | hash) & 0x80000000 < 0)
      return ERROR_KERNEL_ADDRESS;

   if(((cryptkey + 16) | cryptkey) & 0x80000000 < 0)
      return ERROR_KERNEL_ADDRESS;
   
   if(((ctx + 40) | ctx) & 0x80000000 < 0)
      return ERROR_KERNEL_ADDRESS;
   
   //loc_000008DC
   if(sceKernelWaitSema(global_semaphore, 1, 0)) //locks semaphore
      return ERROR_SEMA;

   if(ctx->count >= 17) //invalid value
   {
      //loc_00000C80
      if(sceKernelSignalSema(global_semaphore, 1))
         return ERROR_SEMA;
         
      return ERROR;            
   };
   
   //defines fp based on context mode
   if(ctx->mode == 6)
      fp = 17;
   else if(ctx->mode == 4)
      fp = 13;
   else if(ctx->mode == 2)
      fp = 5;
   else if(ctx->mode == 1)
      fp = 3;
   else if(ctx->mode ^ 3)
      fp = 16;
   else
      fp = 12;
   
   //loc_00000950:

   //fills area with 0's
   for(i = 0; i < 16; i++)
      global_data[i] = 0x0;
   
   if(sub_000013C8(global_data2, 16, fp))
   {
      //loc_00000C80
      if(sceKernelSignalSema(global_semaphore, 1))
         return ERROR_SEMA;
         
      return ERROR;   
   };

   //loc_0000098C:
   for(int i = 0; i < 16; i++)
      data[i] = global_data[i];

   t0 = (data[0] < 0)? 135: 0;
      
   //loc_000009C0:
   for(int i = 0; i < 15; i++)
      data[i] = (data[i] << 1) | (data[i + 1] >> 7);
      
   v0 = t0 ^ (data[15] << 1);
   data[15] = v0;
   
   if(ctx->count != 16)
   {
      t0 = (data[0] < 0)? 16: 0;
         
      //loc_00000A1C:
      
      for(int i = 0; i < 15; i++)
         data[i] = (data[i + 1] >> 7) | (data[i] << 1);
      
      data[15] = t0 | (data[15] << 1);
      
      //alter ctx->count via multiple thread
      //mem. access to manipulate the following write line
      unsigned char * pointer = ctx + ctx->count;
      
      pointer[20] = 0x80; //exploit me
      
      //...more, not important
      
   };
   
   //...more, not important
   
   return 0;
};