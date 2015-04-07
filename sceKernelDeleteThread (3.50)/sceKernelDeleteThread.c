//sceKernelDeleteThread() kxploit partial reverse by GUIDOBOT
//based on 3.36 modules

int t_type = 0x88015630; //*(0x00020000 - 24832 + 1228)
int some_address1; //*(0x00020000 - 24832 + 1292)
int some_address2; //*(0x00020000 - 24832 + 1296)
int context_value; //*(*(0x00020000 - 24832) + 136)
int (* unknown_function)(int, int, int, SceUID); //*(0x00020000 - 24832 + 8);
						
int sceKernelDeleteThread(SceUID thid)
{
	int ret;
	int k1 = pspSdkSetK1(0);
	
	if(context_value) //pretty much out of our control, should not enter here anyway
		ret = SCE_KERNEL_ERROR_ILLEGAL_CONTEXT;
	else
	{
		if(thid == 0) //the argument will not be zero
			ret = SCE_KERNEL_ERROR_NOT_DORMANT;
		else
		{
			int interrupts = sceKernelCpuSuspendIntr();
			unsigned real_address;
			
			if(SysMemForKernel_44BDF332(thid, t_type, &real_address)) //check SysMemForKernel_44BDF332.c to see how to bypass this
				ret = SCE_KERNEL_ERROR_UNKNOWN_THID;
			else
			{
				//any u16 value at *(real_address + 22) will do as long as it's ored with 0x10
				int permission = (pspSdkGetK1() != 0x80000000)? 16: *(real_address + 22) & 0x10; //short
				
				if(permission == 0)
					ret = SCE_KERNEL_ERROR_ILLEGAL_PERM;
				else
				{
					unsigned base_address = real_address + 0x30;
					
					//this checks for two particular addresses, would be very unlucky if we landed in any of them
					if(base_address == some_address1 || base_address == some_address2)
						ret = SCE_KERNEL_ERROR_UNKNOWN_THID;	
					else
					{
						if((((*(base_address + 12)) | 0x10) > 0) && (((*(base_address + 12)) | 0x20) > 0))
							ret = SCE_KERNEL_ERROR_NOT_DORMANT;
						else
						{
							//setting *(base_address + 136) to 0 will do
							if(*(base_address + 136))
								ret = SCE_KERNEL_ERROR_ILLEGAL_CONTEXT;
							else
							{
								if(unknown_function)
									unknown_function(17, base_address, 1, thid);
								
								//no checks of these calls returned values
								sub_00001580(8, base_address, interrupts);
								sub_000060D4(base_address);
								sub_000066F0(base_address);
								
								//these two lines might break memory
								*(base_address + 12) = 0;
								*(* base_address + 4) = *(base_address + 4);
								
								//we kxploit here
								*(*(base_address + 4)) = * base_address;
								
								sub_000155B4(base_address, interrupts);
								SysMemForKernel_361F0F88(*(base_address + 8));
								ret = 0;
							};
						};
					};
				};
			};
			
			sceKernelCpuResumeIntr(interrupts);
		};
	};
	
	pspSdkSetK1(k1);
	return ret;
};