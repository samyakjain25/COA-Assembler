//Two Pass Assembler to convert Assembly Code into Machine Code
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
//structure for storing Mnemonics
struct storage
{
    char text[20];
    char binary[50];
};
typedef struct storage storage;

storage hash_opcodes[100];      //Hash Table for opcodes
storage hash_register[100];     //Hash Table for registers
storage hash_label[100];        //Hash Table for labels

int count_op=0;
int count_reg=0;
int count_lab=0;
int inst_len=0;
char code[10];

//Gives the register binary code
int get_register(char*s,int l,int h,FILE*out)
{
    int i;
    char tmp[20];
    for(i=l;i<=h;i++)
    {
        tmp[i-l]=s[i];
    }
    tmp[i-l]='\0';
    for(i=0;i<count_reg;i++)
    {
        if(!strcmp(hash_register[i].text,tmp))
        {
            fprintf(out,"%s",hash_register[i].binary);
            return 1;
        }
    }
    return 0;
}

//Gives the Opcode
void get_opcode(char*s,int l,int h,FILE*out)
{
    int i;
    char tmp[20];
    for(i=l;i<=h;i++)
    {
        tmp[i-l]=s[i];
    }
    tmp[i-l]='\0';
    for(i=0;i<count_op;i++)
    {
        if(!strcmp(hash_opcodes[i].text,tmp))
        {
            fprintf(out,"%s",hash_opcodes[i].binary);
            return;
        }
    }
}

//Gives the Label
void get_label(char*s,int l,int h,FILE*out)
{
    int i;
    char tmp[20];
    for(i=l;i<=h;i++)
    {
        tmp[i-l]=s[i];
    }
    tmp[i-l]='\0';
    for(i=0;i<count_lab;i++)
    {
        if(!strcmp(hash_label[i].text,tmp))
        {
            fprintf(out,"%s",hash_label[i].binary);
            return;
        }
    }
}

//function to generate hash table for opcodes
void gen_hash_table_opcodes()
{
    FILE *op=fopen("opcodes.txt","r");
    if(op==NULL)
    {
        printf("Opcode Table not found\n");
    }
    else
    {
        while(fscanf(op,"%s %s",hash_opcodes[count_op].text,hash_opcodes[count_op].binary)!=EOF){count_op++;}
    }
    fclose(op);
}
//function to generate hash table for registers
void gen_hash_table_regsiters()
{
    FILE *op=fopen("registers.txt","r");
    if(op==NULL)
    {
        printf("Register Table not found\n");
    }
    else
    {
        while(fscanf(op,"%s %s",hash_register[count_reg].text,hash_register[count_reg].binary)!=EOF){count_reg++;}
    }
    fclose(op);
}
//function to reverse a string
void strr(char *s)
{
    int i,j,l=strlen(s);
    char c;
    for(i=0,j=l-1;i<j;i++,j--)
    {
        c=s[i];
        s[i]=s[j];
        s[j]=c;
    }
}
//convert a decimal number into a binary string
void binary(int num,int len,char *mem)
{
    int i;
    for(i=0;i<len;i++)
    {
        mem[i]=num%2+'0';
        num>>=1;
    }
    mem[i]='\0';
    strr(mem);
}
//this is pass 1, all the labels are looked for and jump statements are assigned proper Instruction Locations
void parse_labels()
{
    int i;
    FILE *as=fopen(code,"r");
    FILE *lb=fopen("labels.txt","w");
    char s[50],lab[10],mem[10];
    int line_cnt=0,lab_cnt=0;
    while(fgets(s,50,as)!=NULL)
    {
        s[strlen(s)-1]='\0';
        line_cnt++;
        if(s[0]=='L' &&(s[1]>='0'&&s[1]<='9'))
        {
            lab_cnt++;
            lab[0]=s[0];
            lab[1]=s[1];
            if(s[2]==':')
            {
                lab[2]='\0';
            }
            else
            {
                lab[2]=s[2];
                lab[2]='\0';
            }
            binary(line_cnt-(lab_cnt-1),6,mem);
            fprintf(lb,"%s %s\n",lab,mem);           //Will print the Labels and its line number
        }
    }
    fclose(as);
    fclose(lb);
}
//function to generate hash table for labels
void gen_hash_table_labels()
{
    count_lab=0;
    FILE *op=fopen("labels.txt","r");
    if(op==NULL)
    {
        printf("Label Table not found\n");
    }
    else
    {
        while(fscanf(op,"%s %s",hash_label[count_lab].text,hash_label[count_lab].binary)!=EOF){count_lab++;}
    }
    fclose(op);
}


//function to convert a number in form of string to binary string
void get_binary(char *s,int l,int h,char *mem)
{
    char tmp[20];
    memset(mem,0,sizeof(mem));
    int num=0,i;
    for(i=l;i<=h;i++)
    {
        num=num*10+(s[i]-'0');
    }
    for(i=0;i<4;i++)
    {
        tmp[i]=num%2+'0';
        num>>=1;
    }
    for(i=0;i<4;i++)
    {
        mem[i]=tmp[3-i];
    }
    mem[4]='\0';
}

//function to convert a 8 bit number in form of string to binary string
void get_binary8(char *s,int l,int h,char *mem)
{
    char tmp[20];
    memset(mem,0,sizeof(mem));
    int num=0,i;
    for(i=l;i<=h;i++)
    {
        num=num*10+(s[i]-'0');
    }
    for(i=0;i<8;i++)
    {
        tmp[i]=num%2+'0';
        num>>=1;
    }
    for(i=0;i<8;i++)
    {
        mem[i]=tmp[7-i];
    }
    mem[8]='\0';
    //printf("%s\n",mem);
}



//print functions :
//Prints the Opcode an their respective binary code
void print_opcode_hash_table()
{
    int i=0;
    for(i=0;i<count_op;i++)
    {
        printf("%s %s\n",hash_opcodes[i].text,hash_opcodes[i].binary);
    }
}

//Prints the Register an their respective binary codes
void print_reg_hash_table()
{
    int i=0;
    for(i=0;i<count_reg;i++)
    {
        printf("%s %s\n",hash_register[i].text,hash_register[i].binary);
    }
}

//Prints the Label an their respective binary codes
void print_label_hash_table()
{
    int i=0;
    for(i=0;i<count_lab;i++)
    {
        printf("%s %s\n",hash_label[i].text,hash_label[i].binary);
    }
}

//Prints the Assembly code
void print_assembly_code()
{
    FILE *as=fopen(code,"r");
    char s[50];
    while(fgets(s,50,as)!=NULL)
    {
        printf("%s",s);
    }
    fclose(as);
}

//Prints the generated Machine code
void print_machine_code()
{
    FILE *as=fopen("machine.txt","r");
    char s[50];
    while(fgets(s,50,as)!=NULL)
    {
        printf("%s",s);
    }
    fclose(as);
}

//main function to convert assembly code to machine code
void convert_assembly_to_machine()
{
    FILE *as=fopen(code,"r");
    FILE *out=fopen("machine.txt","w");
    char s[50];
    int i,j,flag,in_c;
    if(as==NULL)
    {
        printf("Assembly Code not found\n");
        return;
    }
    while(fgets(s,50,as)!=NULL)
    {
        s[strlen(s)-1]='\0';
        printf("%s\n",s);
        j=0;
        in_c=0;
        flag=0;
        int z=0;
        int fl=0;
        char tmp[20];
        int chk,l,k;
        if(s[0]=='S'&&s[1]=='T' || s[0]=='L'&&s[1]=='D')    //For Store and Load Operation
        {
            get_opcode(s,0,1,out);
            if(s[0]=='S' && s[1]=='T')                      //For Store Operation
            {
                l=3;
                for(k=l;k<strlen(s);k++)
                {
                    if(s[k]=='\0' || s[k]==' ')
                    {
                        break;
                    }
                }
//                chk=get_register(s,l,k-1,tmp);
//                fprintf(out,"%s",tmp);
//                get_binary8(s,k+1,strlen(s)-1,tmp);
//                fprintf(out,"%s",tmp);

                get_binary8(s,l,k-1,tmp);
                fprintf(out,"%s",tmp);
                chk=get_register(s,k+1,strlen(s)-1,out);
            }
            else if(s[0]=='L' && s[1]=='D')                 //For Load Operation
            {
                l=3;
                for(k=l;k<strlen(s);k++)
                {
                    if(s[k]=='\0' || s[k]==' ')
                    {
                        break;
                    }
                }
                chk=get_register(s,l,k-1,out);
                get_binary8(s,k+1,strlen(s)-1,tmp);
                fprintf(out,"%s",tmp);
            }
            fprintf(out,"\n");
        }
        else if (s[0]=='L')
            continue;
        else if (s[0]=='J')                     //For Jump Operations
        {
            for(k=0;k<strlen(s);k++)
            {
                if(s[k]=='\0' || s[k]==' ')
                    break;
            }
            get_opcode(s,0,k-1,out);

            l=k+1;
            for(k=l;k<strlen(s);k++)
            {
                if(s[k]=='\0' || s[k]==' ')
                    break;
            }
            get_label(s,l,k-1,out);
            fprintf(out,"\n");
        }
        else if((s[0]=='M'&&s[2]=='V'&&s[3]=='D')||(s[0]=='C'&&s[2]=='P'&&s[3]=='D')) //For Moving or Comparing a 8-bit constant to a value stored in Register
        {
            for(k=0;k<strlen(s);k++)            //Getting opcode
            {
                if(s[k]=='\0' || s[k]==' ')
                    break;
            }
            get_opcode(s,0,k-1,out);

            l=k+1;
            for(k=l;k<strlen(s);k++)            //Constant 8-bit value
            {
                if(s[k]=='\0' || s[k]==' ')
                    break;
            }
            get_binary8(s,l+1,k-1,tmp);
            fprintf(out,"%s",tmp);

            l=k+1;
            for(k=l;k<strlen(s);k++)              //Register
            {
                if(s[k]=='\0' || s[k]==' ')
                    break;
            }
            chk=get_register(s,l,k-1,out);
            if(s[k]=='\0')
            {
                fprintf(out,"\n");
                continue;
            }
        }
        else                    //For other Operations
        {
            for(k=0;k<strlen(s);k++)            //Getting Opcode
            {
                if(s[k]=='\0' || s[k]==' ')
                    break;
            }
            printf("k=%d\n",k);
            get_opcode(s,0,k-1,out);
            if(s[k]=='\0')                      //Stop processing if reached end of instruction
            {
                fprintf(out,"\n");
                continue;
            }

            l=k+1;
            for(k=l;k<strlen(s);k++)            //Getting Register/Constant value
            {
                if(s[k]=='\0' || s[k]==' ')
                    break;
            }
            if(!(get_register(s,l,k-1,out)))       //if not register, then must be constant
            {
                get_binary(s,l+1,k-1,tmp);
                fprintf(out,"%s",tmp);
            }
            printf("k=%d\n",k);
            if(s[k]=='\0')                  //Stop processing if reached end of instruction
            {
                fprintf(out,"\n");
                continue;
            }

            l=k+1;
            for(k=l;k<strlen(s);k++)        //Getting Register
            {
                if(s[k]=='\0' || s[k]==' ')
                    break;
            }
            chk=get_register(s,l,k-1,out);
            printf("k=%d\n",k);
            if(s[k]=='\0')                  //Stop processing if reached end of instruction
            {
                fprintf(out,"\n");
                continue;
            }

            l=k+1;
            for(k=l;k<strlen(s);k++)        //Getting Register
            {
                if(s[k]=='\0' || s[k]==' ')
                    break;
            }
            chk=get_register(s,l,k-1,out);
            printf("k=%d\n",k);
            if(s[k]=='\0')                  //Stop processing if reached end of instruction
            {
                fprintf(out,"\n");
                continue;
            }
        }
    }
    fclose(as);
    fclose(out);
    printf("Successfully Converted to Machine instruction\n");
}

int main()
{
    int choice=0;
    //generate hash tables from "opcodes.txt" and "registers.txt"
    gen_hash_table_opcodes();
    gen_hash_table_regsiters();
    //Menu
    while(choice!=7)
    {
        printf("Please make a choice \n");
        printf("================================\n");
        printf("1.Print Opcode Table\n");
        printf("2.Print Register Table\n");
        printf("3.Convert Assembly to binary\n");
        printf("4.Print Label Table\n");
        printf("5.Print Assembly Code\n");
        printf("6.Print Machine Code\n");
        printf("7.Exit\n");

        scanf("%d",&choice);
        printf("\n");
        switch(choice)
        {
        case 1:
            {
                print_opcode_hash_table();
                break;
            }
        case 2:
            {
                print_reg_hash_table();
                break;
            }
        case 3:
            {
                printf("Enter the file name\n");
                scanf("%s",code);
                parse_labels();          //this is pass 1
                gen_hash_table_labels();
                convert_assembly_to_machine();  //pass 2
                break;
            }
        case 4:
            {
                print_label_hash_table();
                break;
            }
        case 5:
            {
                print_assembly_code();
                break;
            }
        case 6:
            {
                print_machine_code();
                break;
            }
        }
        printf("\n\n");
    }
    return 0;
}
