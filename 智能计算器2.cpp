#include<iostream>
#include<stdio.h>
#include<string.h>
#include<math.h>
  using namespace std;
#define MAXSIZE 1024
#define ERROR -1;
#define OK    1
#define TRUE 1
#define FALSE 0
typedef int Status;
typedef struct {
  double data[MAXSIZE];
  int top;
} Stack;

char infix[1024];

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
  } else if (sign == '*' || sign == '/') {
    return 2;
  } else if (sign == '%') {
    return 3;
  } else if (sign == '^') {
    return 4;
  }

  return 0;
}
// 判断是否为数字
int IsNum(char c) {
  if (c >= '0' && c <= '9') {
    return TRUE;
  } else if (c == '.') {
    return TRUE;
  }

  return FALSE;
}
// 判断是否为符号
int IsSign(char c) {
  if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '^') {
    return TRUE;
  }

  return FALSE;
}
// 判断是否为括号
int IsKuo(char c) {
  if (c == '(' || c == ')') {
    return TRUE;
  }

  return FALSE;
}
// 检查非法字符
Status isInvalid(char* infix){
    int n=strlen(infix);
    for (int i = 0; i < n; i++) {
        if (!IsNum(infix[i]) && !IsSign(infix[i]) && !IsKuo(infix[i])) {
            cout << "包含非法字符\n"<<endl;
            return FALSE;
        }
    }
    return TRUE;
}

// 根据符号计算
double cal(double a, double b, double sign) {
  if (sign == '+') {
    return a + b;
  } else if (sign == '-') {
    return a - b;
  } else if (sign == '*') {
    return a * b;
  } else if (sign == '/') {
    return a / b;
  } else {
    int aa = a, bb = b;
    if (sign == '%') {
      return aa % bb;
    } else if (sign == '^') {
      return pow(aa, bb);
    }
  }
  return ERROR;
}
//插入一个字符到index位置
void insertChar(char *str, char ch, int index){
    int len=strlen(str);
    if(len==0||index>len){
        return;
    }
    for(int j=len;j>=index;j--){
        str[j+1]=str[j];
    }
    str[index]=ch;
}
//处理负数
void handleMinus(char* str){
    int n=strlen(str);
    for(int i = 0;i < n;i++){
        //前面有运算符，就是负号
        if(str[i]=='-'&& (IsSign(str[i-1])||i==0)){
            insertChar(str, '0',i);
            insertChar(str,'(', i);
            n+=2;
            int j=i+2;
            while(IsNum(str[j+1]) && j+1 < n){
                j++;
            }
            insertChar(str,')',j+1);
            n++;
        }
    }
}
// 输入表达式
int GetFix(char *infix) {
  gets(infix);
  handleMinus(infix);
  //printf("%s\n",infix);
  return strlen(infix);
}

// 检查表达式格式
Status CheckFix(char *infix,int n) {
  int left = 0, right = 0;
  char a, b, c;
  //检查非法字符
  if(!isInvalid(infix)){
    return FALSE;
  }
  //检查首尾字符
  if (IsSign(infix[0]) || infix[0] == ')') {
    cout << "首字符错误\n";
    return FALSE;
  }
  if (IsSign(infix[n-1]) || infix[n-1] == '(') {
    cout << "末尾字符错误\n";
    return FALSE;
  }
  for (int i = 0; i < n; i++) {
    c = infix[i];
    if (c == '(') {
      left++;
      if (i > 0 && !IsSign(infix[i - 1])) {
        cout << "第" << left << "个左括号前" << "缺少运算符\n";
        return FALSE;
      }
    }else if (c == ')') {
      right++;
      if (i > 0 && infix[i - 1] == '(') {
        cout << "括号内不能为空"<<endl;
        return FALSE;
      }
      if (i + 1 < n && !IsSign(infix[i + 1])) {
        cout << "第" << right << "个右括号后缺少运算符\n";
        return FALSE;
      }
    } else if (IsSign(c) && i > 0) {
      a = infix[i - 1];
      b = infix[i + 1];
      if (IsSign(a) || IsSign(b) || a == '(' || b == ')') {
        cout << "第" << i + 1 << "个字符" << c << "两边缺少数字或表达式\n";
        return FALSE;
      }
    }
  }
  if (left > right){
    cout << "缺少" << left - right << "个右括号"<<endl;
    return FALSE;
  } else if(right > left){
    cout << "缺少" << right - left << "个左括号"<<endl;
    return FALSE;
  }else{
    return TRUE;
  }

}

// 将表达式转成逆波兰表达式
void ToPostfix(char *infix, int n,Stack * postfix) {
  char c, last;
  double e;                     // 存取符号栈字符
  Stack sign;                   // 符号栈
  InitStack(&sign);
  for (int i = 0; i < n; i++) {
    c = infix[i];               // 当前字符
    last = infix[i - 1];        // 上一个字符
    //数字串末尾加空格
    if (IsNum(last) && !IsNum(c)) {
      Push(postfix, ' ');
    }
    // 数字字符直接输出到postfix
    if (IsNum(c)) {
      Push(postfix, c);
    } else {
      // 符号栈为空或者字符是左括号直接压入符号栈sign
      if (IsEmpty(&sign) || c == '(') {
        Push(&sign, c);
      }
      // 字符是右括号,输出符号栈栈顶的符号到postfix,直到栈顶是左括号
      else if (c == ')') {
        Pop(&sign, &e);
        while (e != '(') {
          Push(postfix, e);
          Pop(&sign, &e);
        }
      }
      // 其他字符则输出符号栈比它等级更高，或相等的符号到postfix
      else {
        while (Rank(sign.data[sign.top]) >= Rank(c) && (!IsEmpty(&sign))) {
          Pop(&sign, &e);
          Push(postfix, e);
        }
        Push(&sign, c);
      }
    }
  }
  // 最后字符是数字，往postfix加空格
  if (IsNum(c)) {
    Push(postfix, ' ');
  }
  // 输出符号栈所有剩余符号，到postfix
  while (!IsEmpty(&sign)) {
    Pop(&sign, &e);
    Push(postfix, e);
  }
}
// 计算逆波兰表达式
void ToNum(Stack * postfix, Stack * num) {
  double a, b, temp = 0;
  char c;
  int p = 0;                    // 小数点位置
  int len = 0;                  // 数字串长度
  for (int i = 0; i <= postfix->top; i++) {
    c = postfix->data[i];
    // 空格压入存在temp中的数字
    if (c == ' ') {
      if (p != 0) {
        temp = temp / pow(10, len - p); // 转换成小数
      }
      Push(num, temp);
      temp = len = p = 0;
    } else if (c >= '0' && c <= '9') {
      len++;
      c = c - '0';
      temp = temp * 10 + c; // 将数字字符存入temp
    } else if (c == '.') {
      len++;
      p = len;  // 记录小数点位置
    } else {
      Pop(num, &b);
      Pop(num, &a);
      Push(num, cal(a, b, c));  // 弹出两个数字运算
    }
  }
  cout <<infix<<"="<<num->data[num->top]<<endl;
}

int main() {

  Stack num;                    // 计算结果栈
  Stack postfix;                // 逆波兰表达式栈
  int n;
  while (1) {

    cout << "请输入表达式:"<<endl;
    n = GetFix(infix);
    while(!CheckFix(infix, n)){
         cout << "请重新输入"<<endl;
         n = GetFix(infix);
    }
    InitStack(&num);
    InitStack(&postfix);
    ToPostfix(infix,n,&postfix);
    ToNum(&postfix, &num);
  }
}
