//sceKernelDeleteThread() kxploit partial reverse by GUIDOBOT
//based on 3.36 modules

unsigned min_address; //*(*(0x00010000 + 18480) + 4)
unsigned max_len; //*(*(0x00010000 + 18480) + 8)

int SysMemForKernel_44BDF332(SceUID uid, int type, unsigned * address_store)
{
	int ret;
	int restore = sub_00012080();
	
	unsigned real_address = ((uid >> 7) << 2) | 0x88000000;
	
	if(uid & 0x80000001 != 1)
		ret = SCE_KERNEL_ERROR_UNKNOWN_UID;
	else
	{
		//real checks here are *(real_address + 12) == uid and *(real_address + 8) == type
		//sub_0000AB6C() just does some checks for heaps in invalid mem partitions in the end, dont worry
		
		if(real_address < min_address || real_address >= min_address + max_len || *(real_address + 12) != uid || !sub_0000AB6C(real_address))
			ret = SCE_KERNEL_ERROR_UNKNOWN_UID;
		else
		{
			if(*(real_address + 8) != type)
				ret = SCE_KERNEL_ERROR_UNMATCH_UID_TYPE;
			else
			{
				* address_store = real_address; //real address gets stored into third argument				
				ret = 0; //we need to get here
			};
		};
	};

	sub_000120B4(restore);
	return ret;
};