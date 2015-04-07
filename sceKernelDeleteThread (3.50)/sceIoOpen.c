//sceKernelDeleteThread() kxploit partial reverse by GUIDOBOT
//based on 3.36 modules

SceUID sceIoOpen(const char * path, int flags, int mode)
{
	//this syscall checks the k1 register status after being shifted to setup an interrupt on fifth argument
	//and the k1 register status before the shifting is used as the sixth argument for the next call
	
	return sub_000049F4(path, flags, mode, 0, ...);
};

SceUID sub_000049F4(const char * path, int flags, int mode, int unknown, ...)
{
	//kernel address check
	if(0x80000000 & path < 0)
		return SCE_KERNEL_ERROR_ILLEGAL_ADDR; //restores k1 and returns
		
	//...
	
	if(KDebugForKernel_F339073C())
	{
		//function requests for the IO path buffer address
		char * io_path_buffer = sub_00003AC8();
		
		if(io_path_buffer)
		{
			//copies file path to buffer
			strncpy(io_path_buffer, path, 1023);
			io_path_buffer[1023] = 0x0;
		};
		
		//...
	};
		
	//...
	//the sub continues but we don't care, it will eventually return a valid UID if the file exists
	//...
};