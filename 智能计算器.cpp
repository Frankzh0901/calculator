#include<iostream>
#include<conio.h>
#include<math.h>
using namespace std;
#define MAXSIZE 50
#define ERROR -1;
#define OK    1
#define TRUE 1
#define FALSE 0
typedef int Status;
typedef struct{
	double data[MAXSIZE];
	int top;
} Stack;
// 初始化栈
Status InitStack(Stack * s) {
	s->top = -1;
	return OK;
}
// 入栈
Status Push(Stack * s, double e) {
	if (s->top == MAXSIZE - 1) {
		return ERROR;
	}
	s->top++;
	s->data[s->top] = e;
	return OK;
}
// 出栈
Status Pop(Stack * s, double *e) {
	if (s->top == -1) {
		return ERROR;
	}
	*e = s->data[s->top];
	s->top--;
	return OK;
}
// 判断是否为空
Status IsEmpty(Stack * s) {
	if (s->top == -1) {
		return TRUE;
	}
	return FALSE;
}
// 判断符号等级 
int Rank(char sign) {
	if (sign == '+' || sign == '-') {
		return 1;
	}
	else if (sign == '*' || sign == '/') {
		return 2;
	}
	else if(sign=='%'){
		return 3;
	}
	else if(sign=='^'){
		return 4;
	}
	
	return 0;
}
//判断是否为数字
int IsNum(char c) {
	if(c>='0'&&c<='9'){
		return TRUE;
	}
	else if(c=='.'){
		return TRUE;
	}
	
	return FALSE;
}
//判断符号
int IsSign(char c){
	if(c=='+'||c=='-'||c=='*'||c=='/'||
	c=='%'||c=='^'){
		return TRUE;
	}
	
	return FALSE;
}
//判断括号
int IsKuo(char c){
	if(c=='('||c==')'){
		return TRUE;
	}
	
	return FALSE;
}
				
// 根据符号计算
double cal(double a, double b, double sign) {
	if (sign == '+'){
		return a + b;
	}
	else if (sign == '-'){
		return a - b;	
	}
	else if (sign == '*'){
		return a * b;
	}
	else if (sign == '/'){
		return a / b;
	}
	else {
		int aa = a, bb = b;
		if (sign == '%'){
			return aa % bb;
		}
		else if(sign=='^'){
			return pow(a,b);
		}
	}
	return ERROR;
}
// 获取表达式
int GetFix(char *infix) {
	int n = -1;
	char temp=' ';
	char c = getch();
	while (c != '\n') {
		//如果用户已输入')'
		//清空temp
		if(c==')')
			temp=' ';
		//数字串结束，之前有负号
		//加入并输出')'
		if(!IsNum(c)&&temp!=' '){
			n++;
			infix[n] =temp;
			cout<<temp;
			temp=' '; //清空temp
		}
		//是负号，加入"(0"
		if(c=='-'&&(n==-1||IsSign(infix[n])||
		infix[n]=='(')){
				infix[n+1]='(';
				infix[n+2]='0';
				n=n+2;
				cout<<"(";
				temp=')';	//把右括号存入temp
		}
		//其他符号,直接存入
		n++;
		infix[n] =c;
		cout << c;
		c = getch();
	}
	//如果')'未输出
	if(temp!=' '){
		n++;
		infix[n] =temp;
		cout<<temp;
	}
		
	cout << endl;
	return n;
}
//检查表达式
Status CheckFix(char *infix,int n){
	int left=0,right=0;
	char a,b,c;
	for(int i=0;i<=n;i++){
		c=infix[i];
		//非法字符
		if(!IsNum(c)&&!IsSign(c)&&!IsKuo(c)){
			cout<<"包含非法字符\n";
			return FALSE;
		}
	}
	//首字符为运算符或右括号
	if(IsSign(infix[0])||infix[0]==')'){
		cout<<"首字符错误\n";
		return FALSE;
	}
	//尾字符为运算符或左括号
	if(IsSign(infix[n])||infix[n]=='('){
		cout<<"末尾字符错误\n";
		return FALSE;
	}
	for(int i=0;i<=n;i++){
		c = infix[i];
		//左括号
		if(c=='('){
			left++;
			//前一个字符是数字
			if(i>0&&IsNum(infix[i-1])){
				cout<<"第"<<left;
				cout<<"个左括号前";
				cout<<"缺少运算符\n";
				return FALSE;
			}
		}
		//右括号
		else if(c==')'){
			right++;
			//前一个字符是左括号
			if(i>0&&infix[i-1]=='('){
				cout<<"括号内不能为空";
				return FALSE;
			}
			//后一个字符是左括号或数字
			if(i<n&&(IsNum(infix[i+1])||
			infix[i+1]=='(')){
				cout<<"第"<<right;
				cout<<"个右括号后";
				cout<<"缺少运算符\n";
				return FALSE;
			}
		}
		//运算符
		else if(IsSign(c)&&i>0){
			a=infix[i-1];
			b=infix[i+1];
			//左右是运算符或
			//左边是左括号
			//右边是右括号
			if(IsSign(a)||IsSign(b)||
			a=='('||b==')'){
				cout<<"第"<<i+1;
				cout<<"个字符"<<c<<"两边";
				cout<<"缺少数字或括号\n";
				return FALSE;
			}
		}	
	}
	//左右括号不相等
	if(left!=right){
		if(left>right){
			cout<<"缺少"<<left-right;
			cout<< "个右括号";
		}else{
			cout<<"缺少"<<right-left;
			cout<<"个左括号";
		}
		cout<<endl;
		return FALSE;
	}
	return TRUE;
}
			
// 将表达式转成逆波兰表达式
void ToPostfix(char *infix, int n, Stack * postfix) {
	char c, last;
	double e;//存取符号栈字符
	Stack sign;//符号栈
	InitStack(&sign);
	for (int i = 0; i <= n; i++) {
		c = infix[i];//当前字符
		last = infix[i - 1];//上一个字符
		// 上个字符是数字
		//且当前字符不是数字
		//往postfix加空格
		if (IsNum(last) && (!IsNum(c))) {
			Push(postfix, ' ');
		}
		//数字字符直接输出到postfix
		if (IsNum(c)){
			Push(postfix, c);
		} 
		else {
			//符号栈为空或者
			//字符是左括号直接压入
			//符号栈sign
			if (IsEmpty(&sign) || c == '(') {
				Push(&sign, c);
			} 
			//字符是右括号
			// 输出符号栈栈顶的符号
			//到postfix,直到栈顶是左括号
			else if (c == ')') {
				Pop(&sign, &e);
				while (e != '(') {
					Push(postfix, e);
					Pop(&sign, &e);
				}
			} 
			//其他字符则
			// 输出符号栈比它等级更高
			// 或相等的符号到postfix
			else {
				while (Rank(sign.data[sign.top]) >= Rank(c) && (!IsEmpty(&sign))) {
					Pop(&sign, &e);
					Push(postfix, e);
				}
				Push(&sign, c);
			}
		}
	}
	// 最后字符是数字
	//往postfix加空格
	if (IsNum(c)){
		Push(postfix, ' ');
	}
	// 输出符号栈所有剩余符号
	//到postfix
	while (!IsEmpty(&sign)) {
		Pop(&sign, &e);
		Push(postfix, e);
	}
	for (int i = 0; i <= postfix->top; i++) {
		char temp = postfix->data[i];
		cout << temp;
	}
}
// 计算逆波兰表达式
void ToNum(Stack * postfix, Stack * num) {
	double a, b, temp = 0;
	char c;
	int p=0;   //小数点位置
	int len=0;  //数字串长度
	for (int i = 0; i <= postfix->top; i++) {
		c = postfix->data[i];
		//遇到空格，压入完整数字
		if (c == ' ') {
			//有小数点,转换成小数
			if(p!=0){
				temp=temp/pow(10,len-p);
			}
			Push(num, temp);
			temp = 0;
			len=0;
			p=0;
		}
		//是数字，转换成整数
		else if (c >= '0' && c <= '9') {
			len++;
			c = c - '0';
			temp = temp * 10 + c;
		
		}
		//是小数点，记录小数点位置
		else if(c=='.'){
			len++;
			p=len;
		}
		//是运算符弹出两个数字运算
		//结果进栈
		else {
			Pop(num, &b);
			Pop(num, &a);
			Push(num, cal(a, b, c));
		}
	}
	cout << endl;
	cout << num->data[num->top];
}
int main() {
	char infix[50];
	Stack num;//计算结果栈
	Stack postfix; //逆波兰表达式栈
	int n;//输入表达式末尾字符下标
	bool IsFalse;
	while (1) {
		cout << "请输入表达式\n";
		do{
			n = GetFix(infix);
			IsFalse=!CheckFix(infix,n);
			if(IsFalse){
				cout<<"请重新输入\n";
			}
		}while(IsFalse);
		InitStack(&num);
		InitStack(&postfix);
		ToPostfix(infix, n, &postfix);
		ToNum(&postfix, &num);
		cout << endl;
	}
}
