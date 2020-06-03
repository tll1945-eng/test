#include <stdio.h>
int sex = 0;
int countBMI(int sex)
{
        int sco;
        float BMI;
 float weight, height;
        fprintf(stdout,"请依次输入体重（kg)、身高（m):");
 fscanf(stdin, "%f%f", &weight, &height);
 BMI = weight / (height*height);
 switch (sex)
 {
 case 0:
  if (BMI > 17.9 &&BMI< 23.9)
   sco = 100;
  else if (BMI <= 17.8 || (BMI>24.0&&BMI < 27.9))
   sco = 80;
  else
   sco = 60;
  break;
 case 1:
  if (BMI>17.2 &&BMI< 23.9)
   sco = 100;
  else if (BMI <= 17.1 || (BMI>24.0&&BMI < 27.9))
   sco = 80;
  else
   sco = 60;
  break;
 default:
  sco = 0;
 }
 fprintf(stdout, "体重指数为：%.2f，成绩为：%d\n", BMI, sco);
 return sco;
}
int countFVC(int sex)
{
 int FVC, sco;
 fprintf(stdout, "请输入肺活量（ml)：");
 fscanf(stdin, "%d", &FVC);
 switch (sex)
 {
 case 0:
  if (FVC > 4800)
   sco = 100;
  else if (FVC > 4180 && FVC <= 4800)
   sco = 80;
  else if (FVC > 3100 && FVC <= 4180)
   sco = 60;
  else
   sco = 30;
  break;
 case 1:
  if (FVC > 3400)
   sco = 100;
  else if (FVC > 3000 && FVC <= 3400)
   sco = 80;
  else if (FVC > 2050 && FVC <= 3000)
   sco = 60;
  else
   sco = 30;
  break;
 default:
  break;
 }
 return sco;
}
int countChinups()
{
 int UPs = 0, sco = 0;
 if (sex == 1)
 {
  fprintf(stdout, "引体向上为男生特有项目！\n");
  return -10;
 }
 fprintf(stdout, "引体向上计数为：");
 fscanf(stdin, "%d", &UPs);
 if (UPs > 19)
  sco = 100;
 else if (UPs > 15 && UPs <= 19)
  sco = 80;
 else if (UPs > 10 && UPs <= 15)
  sco = 60;
 else
  sco = 30;
 return sco;
}
int countSitup()
{
 int UPs = 0, sco = 0;
 if (sex == 0)
 {
  fprintf(stdout, "仰卧起坐为女生特有项目！\n");
  return -1000000;
 }
 fprintf(stdout, "仰卧起坐计数为：");
 fscanf(stdin, "%d", &UPs);
 if (UPs > 56)
  sco = 100;
 else if (UPs > 52 && UPs <= 56)
  sco = 80;
 else if (UPs > 26 && UPs <= 52)
  sco = 60;
 else
  sco = 30;
 return sco;
}
void getNum()
{
 int BMI, FVC, Cups, Sups;
 double num;
 BMI = countBMI(sex);
 FVC = countFVC(sex);
 if (sex == 0)
 {
  Cups = countChinups();
  num = BMI*0.25 + FVC*0.35 + Cups*0.4;
 }
 else
 {
  Sups = countSitup();
  num = BMI*0.25 + FVC*0.35 + Sups*0.4;
 }
 if (num > 95)
  fprintf(stdout, "综合成绩为%.2f,优秀\n", num);
 else if (num > 80 && num <= 95)
  fprintf(stdout, "综合成绩为%.2f，良好\n", num);
 else if (num > 60 && num <= 80)
  fprintf(stdout, "综合成绩为%.2f,及格\n", num);
 else
  fprintf(stdout, "综合成绩为%.2f，不及格\n", num);
}
int menu()
{
 int sec;
 fprintf(stdout, "功能菜单\n");
 fprintf(stdout, "=============\n");
 fprintf(stdout, "1.体重指数BMI\n");
 fprintf(stdout, "2.肺活量FVC\n");
 fprintf(stdout, "3.引体向上\n");
 fprintf(stdout, "4.仰卧起坐\n");
 fprintf(stdout, "5.总成绩\n");
 fprintf(stdout, "0.退出\n");
 fprintf(stdout, "=============\n");
 fprintf(stdout, "请输入性别(男:0/女:1）：");
 fscanf(stdin, "%d", &sex);
 while (sex != 0 && sex != 1)
 {
  fprintf(stdout, "选择有误！\n请重新输入：");
  fscanf(stdin, "%d", &sec);
 }
 fprintf(stdout, "请输入要选择的功能：");
 fscanf(stdin, "%d", &sec);
 while (sec > 5 || sec < 0)
 {
  fprintf(stdout, "选择有误！\n请重新输入：");
  fscanf(stdin, "%d", &sec);
 }
 return sec;
}
int main(argc, argv)
int argc;
char *argv[];
{
 int sec = menu();
 switch (sec)
 {
 case 0:return (0); break;
 case 1:countBMI(sex); break;
 case 2:countFVC(sex); break;
 case 3:countChinups(); break;
 case 4:countSitup(); break;
 case 5:getNum(); break;
 default:break;
 }
 return 0;
}
