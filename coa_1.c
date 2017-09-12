
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

int reg[16]={0};
/*
R[0] to R[15] are general purpose registers
*/
struct Opcode
{
	char name[10];			/*structure for Opcode mapping*/
	char bin[35];
};

typedef struct Opcode Opcode;

Opcode opcode_table[35];
int status[16];
char ram[256];
char out_instr[1000][35];		/*An array of strings for storing binary output which came after running the first code*/

int PC;		//Program counter
char MDR[35];
bool OV,N,Z;	//Flag variables

int ALU(int a,int b,int x)
{
	if(x==1)
		return a+b;
	else if(x==2)
		return a-b;
	else if(x==3)
		return a*b;
	else if(x==4)
		return a/b;
	else if(x==5)
		return a%b;
}

int Search_Instruction()
{
	int i,c=0,l,check=1;
	for(i=0;i<31;i++)
	{
		//printf("The instruction being checked : %s and its format is %s\n",opcode_table[i].name,opcode_table[i].format);

		check=1;
		if(strlen(opcode_table[i].bin)==4)
		{

			l=4;
			for(c=0;c<l;c++)
			{
				if(MDR[c]!=opcode_table[i].bin[c])
					{
						check=0;
						break;
					}
			}
			if(check==1)
			{
				return i;
			}
		}

		else if(strlen(opcode_table[i].bin)==8)
		{
			l=8;
			for(c=0;c<l;c++)
			{
				if(MDR[c]!=opcode_table[i].bin[c])
					{
						check=0;
						break;
					}
			}
			if(check==1)
			{

				return i;
			}
		}

		else if(strlen(opcode_table[i].bin)==10)
		{
			l=10;
			//printf("%d\n",i);
			for(c=0;c<l;c++)
			{

				if(MDR[c]!=opcode_table[i].bin[c])
					{
						check=0;
						break;
					}
			}

			//printf("c=%d\n",c);
			if(check==1)
			{
				//printf("true\n");
				return i;
			}
		}

		else if(strlen(opcode_table[i].bin)==12)
		{
			l=12;
			for(c=0;c<l;c++)
			{
				if(MDR[c]!=opcode_table[i].bin[c])
					{
						check=0;
						break;
					}
			}
			if(check==1)
			{
				return i;
			}
		}

		else if(strlen(opcode_table[i].bin)==16)
		{
			l=16;
			for(c=0;c<l;c++)
			{
				if(MDR[c]!=opcode_table[i].bin[c])
					{
						check=0;
						break;
					}
			}
			if(check==1)
			{
				return i;
			}
		}

	}
	return -1;
}

int convert2decimal(int lo,int hi)
{
	int s=0,l,c=1;
	for(l=hi;l>=lo;l--)
	{
		s+=(c*(MDR[l]-48));
		c=c*2;
	}
	return s;

}

void CMPD()
{
	int x,y,diff;
	x=convert2decimal(12,15);
	y=convert2decimal(4,11);
	diff=ALU(reg[x],y,2);
	if(diff<0)
	{
		N=true;
		Z=false;
		printf("!!!!!!!!!!!!!!!!Flag(N) was raised!!!!!!!!!!!!!!\n");
	}
	else if(diff>0)
	{
		N=false;
		Z=false;
	}
	else
	{
	    N=false;
		Z=true;
		printf("!!!!!!!!!!!!!!!Flag(z) was raised!!!!!!!!!!!!!!\n");
	}
}

void CMP()
{
	int x,y,diff;
	x=convert2decimal(12,15);
	y=convert2decimal(8,11);
	diff=ALU(reg[y],reg[x],2);;
	//printf("diff = %d\n",diff);
	if(diff>0)
	{
		N=false;
		Z=false;
	}
	else if(diff<0)
	{
		N=true;
		Z=false;
		printf("!!!!!!!!!!!!!!!!Flag(N) was raised!!!!!!!!!!!!!!\n");
	}
	else
	{
	    N=false;
		Z=true;
		printf("!!!!!!!!!!!!!!!Flag(z) was raised!!!!!!!!!!!!!!\n");
	}
}

void MOV()
{
	int x,y,diff;
	x=convert2decimal(12,15);
	y=convert2decimal(8,11);
	reg[x]=reg[y];
	printf("REG%d UPDATED WITH VALUE %d\n",x,reg[y]);
}

void MOVD()
{
	int x,y,diff;
	x=convert2decimal(12,15);
	y=convert2decimal(4,11);
	reg[x]=y;
	printf("REG%d UPDATED WITH VALUE %d\n",x,y);
}

void ADDD()
{
	int x,y;
	x=convert2decimal(12,15);
	y=convert2decimal(8,11);
	reg[x]=ALU(reg[x],y,1);

	printf("REG%d=%d\n",x,reg[x]);
}

void SUBD()
{
	int x,y;
	x=convert2decimal(12,15);
	y=convert2decimal(8,11);
	reg[x]=ALU(reg[x],y,2);

	printf("REG%d=%d\n",x,reg[x]);
}

void MULD()
{
	int x,y;
	x=convert2decimal(12,15);
	y=convert2decimal(8,11);
	reg[x]=ALU(reg[x],y,3);

	printf("REG%d=%d\n",x,reg[x]);
}

void DIVD()
{
	int x,y;
	x=convert2decimal(12,15);
	y=convert2decimal(8,11);
	reg[x]=ALU(reg[x],y,4);

	printf("REG%d=%d\n",x,reg[x]);
}


void ADD()
{
	int x,y;
	x=convert2decimal(12,15);
	y=convert2decimal(8,11);
	reg[x]=ALU(reg[x],reg[y],1);

	printf("REG%d=%d\n",x,reg[x]);
}

void SUB()
{
	int x,y;
	x=convert2decimal(12,15);
	y=convert2decimal(8,11);
	reg[x]=ALU(reg[x],reg[y],2);

	printf("REG%d=%d\n",x,reg[x]);
}

void MUL()
{
	int x,y;
	x=convert2decimal(12,15);
	y=convert2decimal(8,11);
	reg[x]=ALU(reg[x],reg[y],3);

	printf("REG%d=%d\n",x,reg[x]);
}

void DIV()
{
	int x,y;
	x=convert2decimal(12,15);
	y=convert2decimal(8,11);
	reg[x]=ALU(reg[x],reg[y],4);

	printf("REG%d=%d\n",x,reg[x]);
}

void RCAL()
{
    int range;
    int mileage,fuelLeft;
    mileage=reg[convert2decimal(8,11)];    //Mileage of Car
	fuelLeft=reg[convert2decimal(12,15)];  //Fuel Left
	range=mileage*fuelLeft;
    printf("RANGE OF THE CAR = %d\n",range);
}

void OCHK()
{
    int frCons,obsDist,speed;
    frCons=reg[convert2decimal(4,7)];
    obsDist=reg[convert2decimal(8,11)];
    speed=reg[convert2decimal(12,15)];
    if(((speed*speed)/(2*frCons))+5>obsDist)
        printf("PLEASE STOP THE CAR \n");
    else
        printf("SAFE DISTANCE \n");
}

void CRT()
{
    int toggle,speed,toggleReg,speedReg;
    speedReg=convert2decimal(12,15);
    toggle=status[1];
    speed=reg[speedReg];
    if(speed>60 && toggle==0)
    {
        printf("CANNOT OPEN THE ROOF\n!!!");
    }
    else if(speed<=60 && toggle==0)
    {
        printf("ROOF IS OPENING\n");
        status[1]=1;
    }
    else if(toggle==1)
    {
        printf("ROOF IS CLOSING\n");
        status[1]=0;
    }
}

void GRC()
{
    int currGear,speed,gearReg,speedReg;
    gearReg=convert2decimal(8,11);
    speedReg=convert2decimal(12,15);
    currGear=reg[gearReg];
    speed=reg[speedReg];
    if(speed<10 && currGear!=1)
    {
        printf("PLEASE CHANGE THE GEAR TO 1\n");
    }
    else if((speed>=10 && speed<30) && currGear!=2)
    {
        printf("PLEASE CHANGE THE GEAR TO 2\n");
    }
    else if((speed>=30 && speed<50) && currGear!=3)
    {
        printf("PLEASE CHANGE THE GEAR TO 3\n");
    }
    else if((speed>=50 && speed<80) && currGear!=4)
    {
        printf("PLEASE CHANGE THE GEAR TO 4\n");
    }
    else if((speed>=80) && currGear!=5)
    {
        printf("PLEASE CHANGE THE GEAR TO 5\n");
    }
}


void ABG()
{
    int prevSpeed,currSpeed;
    prevSpeed=reg[convert2decimal(8,11)];
    currSpeed=reg[convert2decimal(12,15)];
    if (currSpeed-prevSpeed>10)
    {
        printf("INFLATE AIRBAGS\n");
    }
    else
    {
        printf("DON'T INFLATE AIRBAGS\n");
    }
}

void MLGC()
{
    int distance,fuelconsumed,dest;
    dest=convert2decimal(12,15);
    distance=reg[convert2decimal(8,11)];    //Total distance travelled
	fuelconsumed=reg[convert2decimal(4,7)]; //Fuel consumed
	int mileage;
	mileage=distance/fuelconsumed;
    reg[dest]=mileage;
	printf("MILEAGE OF THE CAR = %d\n",mileage);
}

void ACC()
{
    int temperature,temp;
    temp=convert2decimal(12,15);
    temperature=reg[temp];
    if(temperature>=25 && status[2]==0)
    {
        status[2]=1;
        printf("AC SWITCHED ON\n");
    }
    else if(temperature<=16 && status[2]==1)
    {
        status[2]=0;
        printf("AC SWITCHED OFF\n");
    }

}

void FCHK()
{
    printf("FUEL REMAINING = %d\n",status[3]);
}

void TPRS()
{
     printf("TYRE PRESSURE = %d\n",status[4]);
}

void RPM()
{

    printf("RPM = %d\n",status[5]);
}

void ST()
{
    int x,y;
	x=convert2decimal(12,15);
	y=convert2decimal(4,11);
    ram[y]=x;
    printf("VALUE %d STORED AT MEMORY LOCATION %d\n",x,y);
}

void LD()
{
    int x,y;
    x=convert2decimal(8,15);
	y=convert2decimal(4,7);
    reg[y]=ram[x];
    printf("VALUE %d LOADED FROM MEMORY LOCATION %d TO REG%d New value of REG%d is %d\n",ram[x],x,y,y,reg[y]);
}


void Display_Reg()
{
	int i;
	for(i=0;i<16;i++)
	{
		printf("REG[%d] = %d\n",i,reg[i]);
	}
}

int main()
{
	FILE* input_opcode;			// file pointer for opcode file
	FILE* output_machine_code;              // file pointer for output file
	FILE* input_instruction;

    status[1]=0;    //car roof status
    status[2]=0;    //ac control
    status[3]=30;   //fuel left
    status[4]=31;   //tyre pressure
    status[5]=3000;  //rpm of the car

	int index,i=0;
	input_opcode=fopen("opcodes.txt","r+");

	while(!feof(input_opcode))
	{
		fscanf(input_opcode,"%s %s",opcode_table[i].name,opcode_table[i].bin);
		i++;
	}

	output_machine_code=fopen("machine.txt","r+");

	i=1;

	while(!feof(output_machine_code))
	{
		fscanf(output_machine_code,"%s",out_instr[i]);
		i++;
	}

	PC=1;
    int ch;
   while(1)
    {
//        printf("1. Display Registers\n2. Move to Next Instruction\n3. Print value of PC\n4. Exit\n");
//        scanf("%d",&ch);
//        if(ch==1)
//        {
//            Display_Reg();
//        }
//        else if (ch==2)
//        {
            printf("PC=%d\n",PC);
            strcpy(MDR,out_instr[PC]);
            index = Search_Instruction();
            if(index!=-1)
                printf("INSTRUCTION DETECTED : %s\n",opcode_table[index].name);
            else
            {
                printf("INSTRUCTION %s NOT DETECTED\n",MDR);
            }
            if(strcmp(opcode_table[index].name,"ADD")==0)
            {
                ADD();
                PC=ALU(PC,1,1);
            }
            else if(strcmp(opcode_table[index].name,"SUB")==0)
            {
                SUB();
                PC=ALU(PC,1,1);
            }
            else if(strcmp(opcode_table[index].name,"MUL")==0)
            {
                MUL();
                PC=ALU(PC,1,1);
            }
            else if(strcmp(opcode_table[index].name,"DIV")==0)
            {
                DIV();
                PC=ALU(PC,1,1);
            }
            else if(strcmp(opcode_table[index].name,"CMP")==0)
            {
                CMP();
                PC=ALU(PC,1,1);
            }
            else if(strcmp(opcode_table[index].name,"MOV")==0)
            {
                MOV();
                PC=ALU(PC,1,1);
            }
            else if(strcmp(opcode_table[index].name,"MOVD")==0)
            {
                MOVD();
                PC=ALU(PC,1,1);
            }
            else if(strcmp(opcode_table[index].name,"RCAL")==0)
            {
                RCAL();
                PC=ALU(PC,1,1);
            }
            else if(strcmp(opcode_table[index].name,"OCHK")==0)
            {
                OCHK();
                PC=ALU(PC,1,1);
            }
            else if(strcmp(opcode_table[index].name,"CRT")==0)
            {
                CRT();
                PC=ALU(PC,1,1);
            }
            else if(strcmp(opcode_table[index].name,"GRC")==0)
            {
                GRC();
                PC=ALU(PC,1,1);
            }
            else if(strcmp(opcode_table[index].name,"ABG")==0)
            {
                ABG();
                PC=ALU(PC,1,1);
            }
            else if(strcmp(opcode_table[index].name,"MLGC")==0)
            {
                MLGC();
                PC=ALU(PC,1,1);
            }
            else if(strcmp(opcode_table[index].name,"ACC")==0)
            {
                ACC();
                PC=ALU(PC,1,1);
            }
            else if(strcmp(opcode_table[index].name,"JEQ")==0)
            {
                //call2JEQ();
                printf("addr = %d\n",convert2decimal(10,15));
                if(Z==true){
                //PC=ALU(convert2decimal(10,15),1,1);
                PC=convert2decimal(10,15);
                Z=false;
                }
                else PC=ALU(PC,1,1);
            }
            else if(strcmp(opcode_table[index].name,"JLT")==0)
            {
                //call2JLT()
                if(N==true){
    //			PC=ALU(convert2decimal(10,15),1,1);
                PC=convert2decimal(10,15);
                N=false;
                }
                else PC=ALU(PC,1,1);
            }
            else if(strcmp(opcode_table[index].name,"JGT")==0)
            {
                //call2JGT();
                if(N==false && Z==false)
                //PC=ALU(convert2decimal(10,15),1,1);
                PC=convert2decimal(10,15);
                else PC=ALU(PC,1,1);
            }
            else if(strcmp(opcode_table[index].name,"JMP")==0)
            {
                //call2JMP();
                //PC=ALU(convert2decimal(10,15),1,1);
                PC=convert2decimal(10,15);
            }
            else if(strcmp(opcode_table[index].name,"FCHK")==0)
            {
                FCHK();
                PC=ALU(PC,1,1);
            }
            else if(strcmp(opcode_table[index].name,"TPRS")==0)
            {
                TPRS();
                PC=ALU(PC,1,1);
            }
            else if(strcmp(opcode_table[index].name,"RPM")==0)
            {
                RPM();
                PC=ALU(PC,1,1);
            }
            else if(strcmp(opcode_table[index].name,"CMPD")==0)
            {
                CMPD();
                PC=ALU(PC,1,1);
            }
            else if(strcmp(opcode_table[index].name,"ADDD")==0)
            {
                ADDD();
                PC=ALU(PC,1,1);
            }
            else if(strcmp(opcode_table[index].name,"SUBD")==0)
            {
                SUBD();
                PC=ALU(PC,1,1);
            }
            else if(strcmp(opcode_table[index].name,"MULD")==0)
            {
                MULD();
                PC=ALU(PC,1,1);
            }
            else if(strcmp(opcode_table[index].name,"DIVD")==0)
            {
                DIVD();
                PC=ALU(PC,1,1);
            }
            else if(strcmp(opcode_table[index].name,"ST")==0)
            {
                ST();
                PC=ALU(PC,1,1);
            }
            else if(strcmp(opcode_table[index].name,"LD")==0)
            {
                LD();
                PC=ALU(PC,1,1);
            }
            else if(strcmp(opcode_table[index].name,"HLT")==0)
            {
                return 0;
            }
            else
            {
                Display_Reg();
                return 0;
            }
            printf("\n");
        //}
        //else if (ch==3)
        //{
         //   printf("PC = %d\n",PC);
       // }
        //else if (ch==4)
         //   return 0;
    }

}

