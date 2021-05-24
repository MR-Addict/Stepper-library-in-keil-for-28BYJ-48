uint8_t code BeatCode[] = {
	0xe,0xc,0xd,0x9,0xb,0x3,0x7,0x6
};

bit Mode;
bit Direction;
int64_t beats = 0;	//节拍所需总数

void Interrupt0_On(){
	EA = 1;	//使能总中断
	TMOD = 0x01;	//设置T0为模式1
	TH0 = 0xf8;	//为T0赋初值0xf8cd，即2ms
	TL0 = 0xcd;	
	ET0 = 1;	//使能T0
	TR0 = 1;	//启动T0
}

void Motor_Off(){
	P1 = P1 | 0x0f;
}

void Motor_On(bit direction){
	Interrupt0_On();
	Mode = 0;
	Direction = direction;
}

void Motor_Turn(int64_t angle){
	Mode = 1;
	EA = 0;
	beats = angle/360*4076;	//4076节拍为一圈
	EA = 1;
	Interrupt0_On();
}

void Move_To(int64_t angle){
	uint8_t temp;	//临时变量
	int8_t index = 0;	//定义节拍输出索引
	beats = angle/360*4076;	//4076节拍为一圈
	while(beats){
		temp = P1;	//缓存P1口的当前值
		temp = temp & 0xf0;	//&运算清零低四位
		temp = temp | BeatCode[index];	//|运算把节拍写到低四位
		P1 = temp;	//把低四位节拍和原来的高四位送回P1口
		//正转
		if(beats>0){
			index ++;	//节拍输出索引递增
			index = index & 0x07;	//&操作使得8位归零
			beats --;
		}
			
		//反转
		else{
			index --;	//节拍输出索引递增
			index = index & 0x07;	//&操作使得8位归零
			beats ++;
		}
		delay_ms(2);	//延时2ms，即2ms一个节拍
	}
	Motor_Off();	//关闭电机所有的相
}

void Interrupt0() interrupt 1{
	uint8_t temp;	//临时变量
	static int8_t index = 0;	//定义节拍输出索引
	
	TH0 = 0xf8;	//为T0赋初值0xf8cd，即2ms
	TL0 = 0xcd;	
	if(Mode){
		if(beats != 0){
			temp = P1;	//缓存P1口的当前值
			temp = temp & 0xf0;	//&运算清零低四位
			temp = temp | BeatCode[index];	//|运算把节拍写到低四位
			P1 = temp;	//把低四位节拍和原来的高四位送回P1口
			
			//正转
			if(beats>0){
				index ++;	//节拍输出索引递增
				index = index & 0x07;	//&操作使得8位归零
				beats --;
			}
			
			//反转
			else{
				index --;	//节拍输出索引递增
				index = index & 0x07;	//&操作使得8位归零
				beats ++;
			}
		}
		else{
			Motor_Off();
		}
	}
	else{
		temp = P1;	//缓存P1口的当前值
		temp = temp & 0xf0;	//&运算清零低四位
		temp = temp | BeatCode[index];	//|运算把节拍写到低四位
		P1 = temp;	//把低四位节拍和原来的高四位送回P1口
		if(Direction){
			index ++;	//节拍输出索引递增
			index = index & 0x07;	//&操作使得8位归零
		}
		else{
			index --;	//节拍输出索引递增
			index = index & 0x07;	//&操作使得8位归零
		}
	}
}