//sceVideocodecStop kxploit reverse by GUIDOBOT

//u are unknown, p are pointers and v are values
typedef struct s_data
{
	unsigned u0;	//0
	unsigned u4;	//1
	unsigned u8;	//2
	unsigned p12; 	//3
	unsigned p16; 	//4
	unsigned u20; 	//5
	unsigned u24; 	//6
	unsigned u28; 	//7
	unsigned u32;	//8
	unsigned u36;	//9
	unsigned u40;	//10
	unsigned p44;	//11, *
	unsigned p48;	//12
	unsigned u52;	//13
	unsigned p56;	//14
	unsigned v60;	//15
} t_data;

int sceVideocodecStop(t_data * data, int flag)
{
	//we want this to return 0, so we feed it with valid values
	int check = sub_00000C38(data); 
	
	if(!check)
	{
		//both subs here write back some data from the structure to mem, who cares?
		sub_00000D2C(data, flag);
		sub_00000B8C(data, flag);
		
		//we want to get here, the values of the structure should be already swapped at this point for this to work
		int val = sceMeVideo_driver_4D78330C(flag, data);
		
		//etc, not needed
		//..
	};

	//etc, not needed..
	//..
};

//filters kernel addresses
int sub_00000C38(const t_data * data, int flag)
{
	//this chunk of code basically checks data, data->p12, data->p16, data->p44 and data->p56 values

	if((0x80000000 & ((data + 96) | data) >= 0) && (0x80000000 & data->p12 >= 0))
	{
		int size = 256;
		
		if(flag == 0)
			size = 40;
		else if(flag >= 3)
			return 0x806201FE;

		if(0x80000000 & ((data->p16 + size) | data->p16) >= 0)
		{		
			if(flag)
				return 0; //sadly we wont get here, keep reading

			if(0x80000000 & ((((data->p44 + (data->v60 * 44))) | data->p44) | (data->v60 * 44)) >= 0)
			{		
				if(0x80000000 & ((data->p48 + 100) | data->p48) >= 0)
				{
					if(0x80000000 & (((data->p56 + data->v60 * 328) | data->p56) | (data->v60 * 328)) >= 0)
						return 0; //yay
				};
			};
		};
	};

	//we dont want to get here
	return 0x80000023;
};

int sceMeWrapper_driver_4D78330C(int flag, t_data * data)
{
	if(data->v0 > 0x05100601) //its necessary to do data->v0 = 0x05100601 to get funny
		return -2;
		
	data->v8 = 0;
	if(flag == 0)
	{
		if(data->v60 - 1 >= 4)
			return -1;
		
		//if data->v60 is 1 then count will be 0
		int count = data->v60 - 1;

		if(data->p56)
		{
			//this is awful, better go for the else (we can kxploit here too btw)
			void * address = count * 44 + data->p44;
			unsigned value = count * 328 + data->p56;
			
			for(int i = count; i > 0; i--)
			{
				*(address + 36) = value;
				*(address + 40) = value + 164;
				
				value -= 328;
				address -= 44;
			};
		}
		else //this means data->p56 is NULL
		{
			//if count is 0 then address will be data->p44
			void * address = count * 44 + data->p44;
			
			for(int i = count; i > 0; i--)
			{
				*(address + 36) = 0; //do me
				*(address + 40) = 0; //and me
				address -= 44;
			};
			
			//^ we nopped 2 instructions there, we dont want to break more code thus we want count to be 0
			//pointing data->p44 to (ADDRESS - 36) will nop ADDRESS and (ADDRESS + 4)
		};
		
		int result2 = sceMeWrapper_driver_635397BB(3, data->p12, data->p16, data->p44);
		
		if(result2 >= 0)
			return 0;
			
		return sub_00001194(data, result2);
	}
	else if(flag != 1)
		return -1;

	int result = sceMeWrapper_driver_635397BB(33, data->v12);
	
	if(result)
		return sub_000011C4(data, result);
	
	return 0;
};