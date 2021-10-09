#include <iostream>
#include <fstream> //file stream
#include <cstdlib>
#include <string>
#define MAX_TERMS 100
using namespace std;
enum type{Rtype,lw,sw,beq};
string filepath;
int Register[10];
int x00[5];
bool bobble=false,branch=false;
int bin_to_dec(string,int,int);
void initial();
string Instruction[MAX_TERMS];
int i=0,k=0,C=0;
int PC=0;
int ReadData=0,ReadData1=0,ReadData2=0,sign_ext=0,Rs=0,Rt=0,Rd=0,ALUout[2],WriteData=0,Rt_Rd[2];
string Control_signals[3];
string control_signal(int);
int ALUoperation(int);
void output(int);
void MEM_WB();
void pipline();
char intofile[MAX_TERMS];
ofstream test("test.txt",ios::out);
int main()
{
    if(!test)
    {
        cout <<"test could not be opened"<<endl;
        exit(1);
    }
    ifstream testin("test.txt",ios::in);
    if(!testin)
    {
        cout <<"testin could not be opened"<<endl;
        exit(1);
    }
    //Sample
    ifstream SampleInput("SampleInput.txt",ios::in);
    if(!SampleInput)
    {
        cout <<"SampleInput could not be opened"<<endl;
        exit(1);
    }
    /****************
    1.  or $3, $6, $0s
        add $1, $4, $5
        and $8, $7, $2

    ****************/

    ifstream General("General.txt",ios::in);
    if(!General)
    {
        cout <<"General could not be opened"<<endl;
        exit(1);
    }

    ofstream genResult("genResult.txt",ios::out);
    if(!test)
    {
        cout <<"genResult could not be opened"<<endl;
        exit(1);
    }

    //General >> Instruction[i];

    while(General >> Instruction[k])
        k++;
    for(int s=0;s<10;s++)
        Instruction[k+s]="00000000000000000000000000000000";
    pipline();
    test << "!!!" <<endl;


    /****************
    2.  add $5, $2, $3
        sub $4, $5, $3
        and $6, $5, $7
        or $8, $5, $9
    ****************/
    ifstream Datahazard("Datahazard.txt",ios::in);
    if(!Datahazard)
    {
        cout <<"Datahazard could not be opened"<<endl;
        exit(1);
    }
    ofstream dataResult("dataResult.txt",ios::out);
    if(!test)
    {
        cout <<"dataResult could not be opened"<<endl;
        exit(1);
    }

    k=0;
    while(Datahazard >> Instruction[k])
        k++;
    for(int s=0;s<10;s++)
        Instruction[k+s]="00000000000000000000000000000000";
    pipline();
    test << "!!!" <<endl;

    //test.close();
    /****************
    3.  lw $2, 0x04($7)
        and $4, $2, $5
        or $4, $4, $2
        add $3, $2, $4
    ****************/
    ifstream Lwhazard("Lwhazard.txt",ios::in);
    if(!Lwhazard)
    {
        cout <<"Lwhazard could not be opened"<<endl;
        exit(1);
    }
    ofstream loadResult("loadResult.txt",ios::out);
    if(!loadResult)
    {
        cout <<"loadResult could not be opened"<<endl;
        exit(1);
    }

    k=0;
    while(Lwhazard >> Instruction[k])
        k++;
    for(int s=0;s<10;s++)
        Instruction[k+s]="00000000000000000000000000000000";
    pipline();
    test << "!!!" <<endl;
    /****************
    4.  beq $8, $2, 0x03 # (branch ¦Ü and «ü¥O)
        addi $3, $6, 15
        sub $4, $5, $1
        lw $6, 0x00($0)
        and $7, $2, $7
    ****************/
    ifstream Branchhazard("Branchhazard.txt",ios::in);
    if(!Branchhazard)
    {
        cout <<"Branchhazard could not be opened"<<endl;
        exit(1);
    }
    ofstream branchResult("branchResult.txt",ios::out);
    if(!branchResult)
    {
        cout <<"branchResult could not be opened"<<endl;
        exit(1);
    }
    k=0;

    while(Branchhazard >> Instruction[k])
        k++;
    for(int s=0;s<10;s++)
        Instruction[k+s]="00000000000000000000000000000000";
    pipline();
    test << "!!!" <<endl;
    //put all of them in the correct file

    while(testin.getline(intofile,sizeof(intofile)))
    {
        if(intofile[0]=='!')
            break;
        //testin.getline(intofile,sizeof(intofile));
        genResult << intofile<<endl;
        for(int a=0;a<MAX_TERMS;a++)
            intofile[a]=' ';
    }
    while(testin.getline(intofile,sizeof(intofile)))
    {
        if(intofile[0]=='!')
            break;
        dataResult << intofile <<endl;
        for(int a=0;a<MAX_TERMS;a++)
            intofile[a]=' ';
    }
    while(testin.getline(intofile,sizeof(intofile)))
    {
        if(intofile[0]=='!')
            break;
        //testin.getline(intofile,sizeof(intofile));
        loadResult << intofile<<endl;
        for(int a=0;a<MAX_TERMS;a++)
            intofile[a]=' ';
    }
    while(testin.getline(intofile,sizeof(intofile)))
    {
        if(intofile[0]=='!')
            break;
        branchResult << intofile <<endl;
        for(int a=0;a<MAX_TERMS;a++)
            intofile[a]=' ';
    }

    /*
    for(int i=0;i<3;i++)
        SampleInput >> Instruction[i];

    for(int i=0;i<3;i++)
        cout << Instruction[i] << endl;
    */
    return 0;
}
void initial()
{
    Register[0]=0;
    Register[1]=9;
    Register[2]=5;
    Register[3]=7;
    Register[4]=1;
    Register[5]=2;
    Register[6]=3;
    Register[7]=4;
    Register[8]=5;
    Register[9]=6;
    x00[0]=5;
    x00[1]=9;
    x00[2]=4;
    x00[3]=8;
    x00[4]=7;
    Control_signals[0]="000000000";
    Control_signals[1]="00000";
    Control_signals[2]="00";
    PC=0;
    i=0;
    ReadData=0,ReadData1=0,ReadData2=0,sign_ext=0,Rs=0,Rt=0,Rd=0,WriteData=0;
    Rt_Rd[0]=0;
    Rt_Rd[1]=0;
    ALUout[0]=0;
    ALUout[1]=0;
}
int bin_to_dec(string binary,int left,int right)
{
    int decimal=0;
    int ifneg=1;
    bool Positive=true;
    //cout<<"bin_to_dec: "<<endl;
    if(binary[left]=='1')
    {
        Positive=false;
        left++;
    }
    //cout <<binary<<endl;
    while(right>=left)
    {
        //cout <<decimal<<endl;
        decimal=decimal*2;
        ifneg=ifneg*2;
        //cout <<decimal<<endl;
        //cout <<"binary[left]"<<binary[left]<<endl;
        if(binary[left]=='1')
            decimal=decimal+1;
        //cout <<decimal<<endl;
        left++;
    }
    if(!Positive)
    {
        decimal=ifneg-decimal;
        decimal=decimal*(-1);
    }
    //cout <<decimal<<endl;
    return decimal;
}
string control_signal(int index)
{
    if(!Instruction[index].compare("00000000000000000000000000000000"))
        return "000000000";
    else if(Instruction[index][0]=='0'&&Instruction[index][1]=='0'&&Instruction[index][2]=='0'&&Instruction[index][3]=='0'&&Instruction[index][4]=='0'&&Instruction[index][5]=='0')//R-type
    {
        cout <<"1.R-type"<<endl;
        return "110000010";
    }else if(Instruction[index][0]=='1'&&Instruction[index][1]=='0'&&Instruction[index][2]=='0'&&Instruction[index][3]=='0'&&Instruction[index][4]=='1'&&Instruction[index][5]=='1')//lw
    {
        cout <<"1.lw"<<endl;
        return "000101011";
    }else if(Instruction[index][0]=='1'&&Instruction[index][1]=='0'&&Instruction[index][2]=='1'&&Instruction[index][3]=='0'&&Instruction[index][4]=='1'&&Instruction[index][5]=='1')//sw
    {
        cout <<"1.sw"<<endl;
        return "000100100";//X0010010X
    }else if(Instruction[index][0]=='0'&&Instruction[index][1]=='0'&&Instruction[index][2]=='1'&&Instruction[index][3]=='0'&&Instruction[index][4]=='0'&&Instruction[index][5]=='0')//addi
    {
        cout <<"1.addi"<<endl;
        return "000100010";
    }else if(Instruction[index][0]=='0'&&Instruction[index][1]=='0'&&Instruction[index][2]=='1'&&Instruction[index][3]=='1'&&Instruction[index][4]=='0'&&Instruction[index][5]=='0')//andi
    {
        cout <<"1.andi"<<endl;
        return "011100010";
    }else if(Instruction[index][0]=='0'&&Instruction[index][1]=='0'&&Instruction[index][2]=='0'&&Instruction[index][3]=='1'&&Instruction[index][4]=='0'&&Instruction[index][5]=='0')//beq
    {
        cout <<"1.beq"<<endl;
        return "001010000";//X0101000X
    }else
        return "000000000";

}
void output(int a)
{

    test<<"CC "<<C<<":"<<endl<<endl;
    cout<<"CC "<<C<<":"<<endl<<endl;
    test<<"Registers:"<<endl;
    for(int j=0;j<10;j++)
        test << "$" << j << ": " << Register[j]<<endl;
    test <<endl;
    test <<"Data memory:"<<endl;
    test <<"0x00: "<<x00[0]<<endl;
    test <<"0x04: "<<x00[1]<<endl;
    test <<"0x08: "<<x00[2]<<endl;
    test <<"0x0C: "<<x00[3]<<endl;
    test <<"0x10: "<<x00[4]<<endl<<endl;

    test <<"IF/ID :"<<endl;
    test <<"PC\t\t"<<PC<<endl;
    if(a>=k)
        test <<"Instruction\t"<<"00000000000000000000000000000000"<<endl<<endl;
    else if(bobble)
    {
        test <<"Instruction\t"<<Instruction[a-1]<<endl<<endl;
        bobble=false;
    }
    else if(branch)
    {
        test <<"Instruction\t"<<Instruction[a+sign_ext-1]<<endl<<endl;
        cout <<"a= "<<a<<"    sign_ext= "<<sign_ext<<endl;
        cout <<"Instruction\t"<<Instruction[a+sign_ext-1]<<endl<<endl;//a+sign_ext-2+1
        branch=false;
        i=sign_ext+i;
        sign_ext=0;
    }else
        test <<"Instruction\t"<<Instruction[a]<<endl<<endl;

    test <<"ID/EX :"<<endl;
    test <<"ReadData1\t"<<ReadData1<<endl;
    test <<"ReadData2\t"<<ReadData2<<endl;
    test <<"sign_ext\t"<<sign_ext<<endl;
    test <<"Rs\t\t"<<Rs<<endl;
    test <<"Rt\t\t"<<Rt<<endl;
    test <<"Rd\t\t"<<Rd<<endl;
    test <<"Control signals\t"<<Control_signals[0]<<endl<<endl;

    test <<"EX/MEM :"<<endl;
    test <<"ALUout\t\t"<<ALUout[0]<< endl;
    test <<"WriteData\t"<<WriteData<< endl;
    test <<"Rt/Rd\t\t"<<Rt_Rd[0]<< endl;
    test <<"Control signals\t"<<Control_signals[1]<<endl<<endl;

    test <<"MEM/WB :"<<endl;
    test <<"ReadData\t"<<ReadData<< endl;
    test <<"ALUout\t\t"<<ALUout[1]<< endl;
    test <<"Rt/Rd\t\t"<<Rt_Rd[1]<< endl;
    test <<"Control signals\t"<<Control_signals[2]<<endl;
    test <<"================================================================="<<endl;
}
int ALUoperation(int a)
{
    int operand1=ReadData1;
    int operand2=ReadData2;
    char Operator='+';
    cout <<Instruction[a]<<endl;
    if(Control_signals[0][0]=='0'&&Control_signals[0][1]=='0'&&Control_signals[0][2]=='0'&&Control_signals[0][3]=='0'&&Control_signals[0][4]=='0'&&Control_signals[0][5]=='0'&&Control_signals[0][6]=='0'&&Control_signals[0][7]=='0'&&Control_signals[0][8]=='0')
        return 0;
    else{
    if(Control_signals[0][3]=='1')//it is or I-type(CPU Src=1)
    {
        operand2=sign_ext;
        if(Control_signals[2][0]=='1'&&Rt_Rd[1]==Rs)//hazard 2
        {
            cout <<"hazard 2 before Rs-1"<<endl;
            operand1=ALUout[1];
        }
        if(Control_signals[2][1]!='0'&&Rt_Rd[1]==Rs)//hazard 2 before stall
        {
            cout <<"hazard 2 before stall Rs"<<endl;
            operand1=ReadData;
        }
        if(Control_signals[1][3]!='0'&&Rt_Rd[0]==Rs)//hazard 1
        {
            cout <<"hazard 1 before Rs"<<endl;
            operand1=ALUout[0];
        }
        cout <<"operand2=sign_ext"<<endl;
    }
    if(Control_signals[0][1]=='1'&&Control_signals[0][2]=='0')//R-type
    {
        if(Control_signals[2][0]=='1')//hazard 2 before
        {
            cout <<"Rt_Rd[1] : "<<Rt_Rd[1]<<endl;
            if(Rt_Rd[1]==Rs)
            {
                cout <<"hazard 2 before Rs"<<endl;
                operand1=ALUout[1];
            }
            if(Rt_Rd[1]==Rt)
            {
                cout <<"hazard 2 before Rt"<<endl;
                operand2=ALUout[1];
                WriteData=operand2;
            }
        }
        if(Control_signals[2][1]=='1')//hazard 2 before stall
        {

            if(Rt_Rd[1]==Rs)
            {
                cout <<"hazard 2 before stall Rs"<<endl;
                operand1=ReadData;
            }
            if(Rt_Rd[1]==Rt)
            {
                cout <<"hazard 2 before stall Rt"<<endl;
                operand2=ReadData;
                WriteData=operand2;
            }
        }
        if(Control_signals[1][3]=='1')//hazard 1 before
        {
            cout <<"Rt_Rd[0] : "<<Rt_Rd[0]<<endl;
            cout <<"Rs : "<<Rs<<endl;
            if(Rt_Rd[0]==Rs)
            {
                 cout <<"hazard 1 before Rs"<<endl;
                operand1=ALUout[0];
            }
            if(Rt_Rd[0]==Rt)
            {
                 cout <<"hazard 1 before Rt"<<endl;
                operand2=ALUout[0];
                WriteData=operand2;
            }
        }
        if(Instruction[a][28]=='0'&&Instruction[a][29]=='0'&&Instruction[a][30]=='0'&&Instruction[a][31]=='0')//func:100000 add
        {
            cout << "add"<<endl;
            Operator='+';
        }else if(Instruction[a][28]=='0'&&Instruction[a][29]=='0'&&Instruction[a][30]=='1'&&Instruction[a][31]=='0')//func:100010 Subtract
        {
            cout << "sub"<<endl;
            Operator='-';
        }else if(Instruction[a][28]=='0'&&Instruction[a][29]=='1'&&Instruction[a][30]=='0'&&Instruction[a][31]=='0')//func:100100 and
        {
            cout << "and"<<endl;
            Operator='&';
        }else if(Instruction[a][28]=='0'&&Instruction[a][29]=='1'&&Instruction[a][30]=='0'&&Instruction[a][31]=='1')//func:100101 or
        {
            cout << "or"<<endl;
            Operator='|';
        }else//Set on <
        {
            cout << "slt"<<endl;
            Operator='<';
        }
    }else if(Control_signals[0][1]=='1'&&Control_signals[0][2]=='1')//I-type and
    {
        cout <<"I-type and"<<endl;
        Operator='&';
    }else if(Control_signals[0][1]=='0'&&Control_signals[0][2]=='0')//I-type add
    {
        cout <<"I-type add"<<endl;
        Operator='+';
    }else if(Control_signals[0][1]=='0'&&Control_signals[0][2]=='1')//I-type subtract
    {
        cout <<"I-type subtract"<<endl;
        Operator='-';
    }
    if(Operator=='+')
        return operand1+operand2;
    else if(Operator=='-')
        return operand1-operand2;
    else if(Operator=='&')
        return operand1&operand2;
    else if(Operator=='|')
        return operand1|operand2;
    else //Set on <
    {
        if(operand1-operand2<0)
            return 1;
        else
            return 0;
    }
    }
}
void MEM_WB()
{
    if(Control_signals[2][0]=='1')//Reg write
    {
        if(Control_signals[2][1]=='1')//mem to reg
        {
            Register[Rt_Rd[1]]=ReadData;
        }
        else
            Register[Rt_Rd[1]]=ALUout[1];
    }
}
void pipline()
{
    i=0;
    C=i+1;
    initial();
    while(i<k+4)
    {
        MEM_WB();
        //WB

        if(Control_signals[1][1]=='1')// mem read
            ReadData=x00[ALUout[0]/4];
        else
            ReadData=0;
        ALUout[1]=ALUout[0];

        Control_signals[2]=Control_signals[2].assign(Control_signals[1], 3, 2);

        //MEM
        WriteData=ReadData2;
        if(i>1)
            ALUout[0]=ALUoperation(i-2);
        Rt_Rd[1]=Rt_Rd[0];
        if(Control_signals[0][0]=='0')//
            Rt_Rd[0]=Rt;
        else
            Rt_Rd[0]=Rd;

        Control_signals[1]=Control_signals[1].assign(Control_signals[0], 4, 5);

        if(Control_signals[1][2]=='1')// mem write
            x00[ALUout[0]/4]=WriteData;
        if(ALUout[0]==0&&Control_signals[1][0]=='1')//beq
        {
            PC=PC+4*(sign_ext-1);
            cout <<"PC: "<<PC<<endl;
            branch=true;
        }
        //EX/MEM
        cout <<"i:"<<i<<endl;
        if(i>0&&!branch)
        {
            Rs=bin_to_dec(Instruction[i-1],6,10);//Rs
            Rt=bin_to_dec(Instruction[i-1],11,15);//Rt
            Rd=bin_to_dec(Instruction[i-1],16,20);//Rd
            sign_ext=bin_to_dec(Instruction[i-1],16,31);
            Control_signals[0]=control_signal(i-1);
            ReadData1=Register[Rs];
            ReadData2=Register[Rt];

        }

        //ID/EX

        //cout <<Instruction[i]<<endl;;
        PC=PC+4;
        //IF/ID

        if(!Control_signals[1].compare("01011")&&(Rt_Rd[0]==Rs||Rt_Rd[0]==Rt))//before is lw
        {
            bobble=true;
            Control_signals[0]="000000000";
            PC=PC-4;
            output(i);
            i--;

            /*
            ReadData1=0;
            ReadData2=0;
            sign_ext=0;
            Rs=0;
            Rt=0;//Rt
            Rd=0;
            */
        }else if(branch)
        {
            Control_signals[0]="000000000";
            ReadData1=0;
            ReadData2=0;
            Rs=0;
            Rt=0;//Rt
            Rd=0;
            cout <<"??i:"<<i<<endl;
            output(i);

            i--;//we will +1 after
            cout <<"??i:"<<i<<endl;
        }else
            output(i);
        cout <<"i:"<<i<<endl;
        i++;
        C++;
    }

}
