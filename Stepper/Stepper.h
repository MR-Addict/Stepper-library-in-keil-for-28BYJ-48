uint8_t code BeatCode[] = {
	0xe,0xc,0xd,0x9,0xb,0x3,0x7,0x6
};

bit Mode;
bit Direction;
int64_t beats = 0;	//������������

void Interrupt0_On(){
	EA = 1;	//ʹ�����ж�
	TMOD = 0x01;	//����T0Ϊģʽ1
	TH0 = 0xf8;	//ΪT0����ֵ0xf8cd����2ms
	TL0 = 0xcd;	
	ET0 = 1;	//ʹ��T0
	TR0 = 1;	//����T0
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
	beats = angle/360*4076;	//4076����ΪһȦ
	EA = 1;
	Interrupt0_On();
}

void Move_To(int64_t angle){
	uint8_t temp;	//��ʱ����
	int8_t index = 0;	//��������������
	beats = angle/360*4076;	//4076����ΪһȦ
	while(beats){
		temp = P1;	//����P1�ڵĵ�ǰֵ
		temp = temp & 0xf0;	//&�����������λ
		temp = temp | BeatCode[index];	//|����ѽ���д������λ
		P1 = temp;	//�ѵ���λ���ĺ�ԭ���ĸ���λ�ͻ�P1��
		//��ת
		if(beats>0){
			index ++;	//���������������
			index = index & 0x07;	//&����ʹ��8λ����
			beats --;
		}
			
		//��ת
		else{
			index --;	//���������������
			index = index & 0x07;	//&����ʹ��8λ����
			beats ++;
		}
		delay_ms(2);	//��ʱ2ms����2msһ������
	}
	Motor_Off();	//�رյ�����е���
}

void Interrupt0() interrupt 1{
	uint8_t temp;	//��ʱ����
	static int8_t index = 0;	//��������������
	
	TH0 = 0xf8;	//ΪT0����ֵ0xf8cd����2ms
	TL0 = 0xcd;	
	if(Mode){
		if(beats != 0){
			temp = P1;	//����P1�ڵĵ�ǰֵ
			temp = temp & 0xf0;	//&�����������λ
			temp = temp | BeatCode[index];	//|����ѽ���д������λ
			P1 = temp;	//�ѵ���λ���ĺ�ԭ���ĸ���λ�ͻ�P1��
			
			//��ת
			if(beats>0){
				index ++;	//���������������
				index = index & 0x07;	//&����ʹ��8λ����
				beats --;
			}
			
			//��ת
			else{
				index --;	//���������������
				index = index & 0x07;	//&����ʹ��8λ����
				beats ++;
			}
		}
		else{
			Motor_Off();
		}
	}
	else{
		temp = P1;	//����P1�ڵĵ�ǰֵ
		temp = temp & 0xf0;	//&�����������λ
		temp = temp | BeatCode[index];	//|����ѽ���д������λ
		P1 = temp;	//�ѵ���λ���ĺ�ԭ���ĸ���λ�ͻ�P1��
		if(Direction){
			index ++;	//���������������
			index = index & 0x07;	//&����ʹ��8λ����
		}
		else{
			index --;	//���������������
			index = index & 0x07;	//&����ʹ��8λ����
		}
	}
}