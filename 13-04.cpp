#include <Windows.h>
#include <iostream>
using namespace std;

/**Description
 * function    : 日期和时间处理
 * Author      : LIU
 * Date        : Dec 15, 2020
*/

//Part:全局变量，也可变为类中的静态变量
int time_zone[3][3] = {{0, 14, 7}, {-14, 0, -7}, {-7, 7, 0}}; //时区转换表
/**    中  美  欧
 *中   0   14  7
 *美  -14  0  -7
 *欧  -7   7   0
*/
int Monthday[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; //每个月的具体天数
string WeekName[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

//Why?  提前声明，使输入检测函数能调用Month_day()函数
class CDate;
class CDateTime;
int Month_day(CDate d);
string Get_weekname(CDateTime op);

//日期类
class CDate
{
    int Year, Month, Day;
    static int format; //显示日期的格式(1.中国，2.美国（取太平洋时间），3.欧洲（以欧洲中部时间为准）)

public:
    CDate(const CDate &d) : Year(d.Year), Month(d.Month), Day(d.Day){};
    CDate(int y = 0, int m = 0, int d = 0) : Year(y), Month(m), Day(d){};

    int Jud_Date(CDate d); //Function:判断输入的年月日是否出错

    void SetDate(CDate d);                   //Function:基于CDate对象设定时间
    int Getformat() { return format; };      //Function:  获取当前时区
    void Up_format(int op) { format = op; }; //更新时区

    inline void SetYear(int y) { Year = y; };
    inline void SetMonth(int m) { Month = m; };
    inline void SetDay(int d) { Day = d; };
    inline int GetYear() { return Year; };
    inline int GetMonth() { return Month; };
    inline int GetDay() { return Day; };
    inline void ShowDate() { cout << Year << '/' << Month << '/' << Day; };

    bool IsLeapYear(int year); //Function： 闰年判断函数

    //Part:运算符重载
    bool operator<(const CDate &d) const;
    bool operator<=(const CDate &d) const;
    bool operator>(const CDate &d) const;
    bool operator>=(const CDate &d) const;
    bool operator==(const CDate &d) const;
    bool operator!=(const CDate &d) const { return !(*this == d); };

    CDate operator+(CDate &d);
    CDate operator-(CDate &d);
    CDate operator++();
    CDate operator--();

    //Special:后置
    CDate operator++(int);
    CDate operator--(int);

    //Special:重载<<,>>
    friend ostream &operator<<(ostream &out, const CDate &d);
    friend istream &operator>>(istream &in, CDate &d);
};

class CTime
{
    int Hour, Minute, Second; //分别存放时、分、秒
public:
    CTime(const CTime &d) : Hour(d.Hour), Minute(d.Minute), Second(d.Second){};
    CTime(int h = 0, int m = 0, int s = 0) : Hour(h), Minute(m), Second(s){};

    int Jud_Time(CTime t); //Function:判断输入时间是否正确

    void SetTime(CTime t); //Function:基于CTime对象设定时间

    inline void SetHour(int h) { Hour = h; };
    inline void SetMinute(int m) { Minute = m; };
    inline void SetSecond(int s) { Second = s; };
    inline int GetHour() { return Hour; };
    inline int GetMinute() { return Minute; };
    inline int GetSecond() { return Second; };

    inline void ShowTime() { cout << Hour << ':' << Minute << ':' << Second; };

    //Part:运算符重载
    bool operator<(const CTime &t) const;
    bool operator<=(const CTime &t) const;
    bool operator>(const CTime &t) const;
    bool operator>=(const CTime &t) const;
    bool operator==(const CTime &t) const;
    bool operator!=(const CTime &t) const { return !(*this == t); };
    CTime operator+(CTime &t);
    CTime operator-(CTime &t);
    CTime operator++();
    CTime operator--();

    //Special:后置
    CTime operator++(int);
    CTime operator--(int);

    //special:重载"<<"">>"
    friend ostream &operator<<(ostream &out, const CTime &t);
    friend istream &operator>>(istream &in, CTime &t);
};

//子类
class CDateTime : public CDate, public CTime
{
public:
    CDateTime() : CDate(), CTime(){};
    CDateTime(CDate d, CTime t) : CDate(d), CTime(t){};
    CDateTime(int y, int m, int d, int h, int min, int s) : CDate(y, m, d), CTime(h, min, s){};

    //Part:重载
    bool operator<(CDateTime &op);
    bool operator<=(CDateTime &op);
    bool operator>(CDateTime &op);
    bool operator>=(CDateTime &op);
    bool operator==(CDateTime &op);
    bool operator!=(CDateTime &op);
    CDateTime operator+(CDateTime &op);
    CDateTime operator-(CDateTime &op);
    CDateTime operator++();
    CDateTime operator--();

    //Special:后置
    CDateTime operator++(int);
    CDateTime operator--(int);
    void ShowDT();       //Function:展示日期时间
    void SetDT();        //Function:设定日期时间
    void DT_trans();     //Function:时区转换
    void DateTime_adj(); //Function:调整年月日和时间，该函数是对时间加减自增溢出的处理
};

int CDate::format = 1; //静态变量初始化
//Part:<<,>>的重载
ostream &operator<<(ostream &out, const CDate &d)
{
    cout << d.Year << '/' << d.Month << '/' << d.Day;
    return out;
};

istream &operator>>(istream &in, CDate &d)
{
    cin >> d.Year >> d.Month >> d.Day;
    return in;
};

ostream &operator<<(ostream &out, const CTime &t)
{
    cout << t.Hour << ':' << t.Minute << ':' << t.Second;
    return out;
};

istream &operator>>(istream &in, CTime &t)
{
    cin >> t.Hour >> t.Minute >> t.Second;
    return in;
};

//part:类内成员函数定义
int CDate::Jud_Date(CDate d)
{
    int point = 1;
    int d_num = Month_day(d);                                        //求出该月具体天数
    if (d.Month > 0 && d.Month <= 12 && d.Day > 0 && d.Day <= d_num) //暂时用31，后面增加闰年月份判断，将31修改为一个变式
        point = 1;
    else
        point = 0;
    return point;
}

void CDate::SetDate(CDate d)
{
    if (Jud_Date(d) == 0)
    {
        cout << "输入的日期错误" << endl;
        return; //输入错误，提前结束
    }
    SetYear(d.Year);
    SetMonth(d.Month);
    SetDay(d.Day);
};

//Function： 闰年判断函数
bool CDate::IsLeapYear(int year)
{
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
        return true;
    else
        return false;
}; //闰年

//Function:判断输入时间是否正确
int CTime::Jud_Time(CTime t)
{
    int point = 1;
    if (t.Hour >= 0 && t.Hour <= 24 && t.Minute >= 0 && t.Minute <= 60 && t.Second >= 0 && t.Second <= 60)
        point = 1;
    else
        point = 0;
    return point;
};

//Function:基于CTime对象设定时间
void CTime::SetTime(CTime t)
{
    if (Jud_Time(t) == 0)
    {
        cout << "输入的日期错误" << endl;
        return; //提前跳出循环
    }
    Hour = t.Hour;
    Minute = t.Minute;
    Second = t.Second;
};

void CDateTime::ShowDT()
{
    cout << "当前日期时间: ";
    int i = CDate::Getformat();
    switch (i)
    {
    case 1:
        cout << "北京时间     ";
        break;
    case 2:
        cout << "美国时间     ";
        break;
    case 3:
        cout << "欧洲时间     ";
        break;
    }
    ShowDate();
    cout << "    ";
    ShowTime();
    cout << "    ";
    cout << Get_weekname(*this) << endl;
};

void CDateTime::SetDT()
{
    int function = 1;
    cout << "请选择设定方式（1.只设定日期 2.只设定时间 3.设定日期和时间）： ";
    cin >> function;
    CDate d;
    CTime t;
    cout << "输入格式示例:2012 12 1或18 22 35" << endl;
    switch (function)
    {
    case 1:
        cout << "请输入日期:";
        cin >> d;
        SetDate(d);
        break;
    case 2:
        cout << "请输入时间:";
        cin >> t;
        SetTime(t);
        break;
    case 3:
        cout << "请输入日期:";
        cin >> d;
        SetDate(d);
        cout << "请输入时间:";
        cin >> t;
        SetTime(t);
        break;
    default:
        cout << "设定方式输入错误" << endl;
    }
};

//Function:时区转换
void CDateTime::DT_trans()
{
    int i = CDate::Getformat();
    cout << "当前时区为：";
    switch (i)
    {
    case 1:
        cout << "中国北京";
        break;
    case 2:
        cout << "美国";
        break;
    case 3:
        cout << "欧洲";
        break;
    }
    cout << endl
         << "请选择要转换的时区：（1.中国 2.美国 3.欧洲）：";
    int j;
    cin >> j;
    if (j != 1 && j != 2 && j != 3)
    {
        cout << "输入错误,暂无该选项" << endl;
        return; //提前结束程序
    }
    if (i == j)
    {
        cout << "要转换的时区即为当前时区，无需转换！" << endl;
        return; //结束函数
    }
    Up_format(j); //更新当前时区
    //小于0，时间循环（），天数减1
    if (GetHour() - time_zone[i - 1][j - 1] < 0)
    {
        SetDay(GetDay() - 1); //天数减1
        SetHour(GetHour() + 24 - time_zone[i - 1][j - 1]);
    }
    //大于24小时
    else if (GetHour() - time_zone[i - 1][j - 1] > 24)
    {
        SetDay(GetDay() + 1); //天数+1
        SetHour(GetHour() - time_zone[i - 1][j - 1] - 24);
    }
    //不大于24，不小于0
    else
        SetHour(GetHour() - time_zone[i - 1][j - 1]);
}

//Function:调整年月日和时间，该函数是对时间加减自增溢出的处理
void CDateTime::DateTime_adj()
{
    if (GetSecond() > 60)
    {
        SetMinute(GetMinute() + GetSecond() / 60);
        SetSecond(GetSecond() % 60);
    }
    if (GetMinute() > 60)
    {
        SetHour(GetHour() + GetMinute() / 60);
        SetMinute(GetMinute() % 60);
    }
    if (GetHour() > 24)
    {
        SetDay(GetDay() + GetHour() / 24);
        SetHour(GetHour() % 24);
    }

    //此处本应该调整为根据当月天数（闰年还有天数）动态判断，但存在天数过线后会进到下一个月，然后下一个月是否满足
    //修改思路：对天数的操作按月操作，先根据当前月数减去天数，进入下一个月，再根据下一个月的天数减去下一个数，自此一个月一个月进行操作
    //存在问题：当前月份超过12如何处理？对超过十二的月份直接按30处理
    //结果分析:动态调整成功实现！！
    while (1)
    {
        int d_num = Month_day(*this);
        if (GetDay() > d_num)
        {
            SetMonth(GetMonth() + 1);
            SetDay(GetDay() - d_num);
        }
        else
            break;
    } //通过循环动态调整月份和天数
    if (GetMonth() > 12)
    {
        SetYear(GetYear() + GetMonth() / 12);
        SetMonth(GetMonth() % 12);
    }
}

//Part::函数定义

//Function: 当前各个对象的日期时间情况
void ShowCurrent(CDateTime a, CDateTime b, CDateTime c)
{
    cout << "op1:     ";
    a.ShowDT();
    cout << "op2:     ";
    b.ShowDT();
    cout << "result:  ";
    c.ShowDT();
    cout << endl;
};

//Function:求一个月的具体天数
int Month_day(CDate op)
{
    int i = op.GetMonth();
    int d_num = Monthday[i - 1];
    if (i > 12)
    {
        d_num = 30;
    }
    if (i == 2)
    {
        if (op.IsLeapYear(op.GetYear()))
            d_num = 29;
    }
    return d_num;
}

string Get_weekname(CDateTime op)
{
    int leap_num = 0; //闰年数量
    int month_day = 0;
    int day = op.GetDay();
    int sum_day = 0;
    string weekname;
    for (int i = 1; i < op.GetYear(); i++)
    {
        if (op.IsLeapYear(i))
            leap_num++;
    }
    sum_day = leap_num * 365 + (op.GetYear() - 1) * 365;

    for (int i = 1; i < op.GetMonth(); i++)
    {
        int d_num = Monthday[i - 1];
        if (i == 2)
        {
            if (op.IsLeapYear(op.GetYear()))
                d_num = 29;
        }
        month_day += d_num;
    }
    sum_day = sum_day + month_day + day;
    sum_day = sum_day % 7;
    weekname = WeekName[sum_day];
    return weekname;
}

//Part:重载函数部分
//Part:CDate运算符重载
bool CDate::operator<(const CDate &d) const
{
    if ((Year < d.Year) || (Year == d.Year && Month < d.Month) || (Year == d.Year && Month == d.Month && Day < d.Day))
        return true;
    else
        return false;
};

bool CDate::operator<=(const CDate &d) const
{
    if ((Year < d.Year) || (Year == d.Year && Month < d.Month) || (Year == d.Year && Month == d.Month && Day <= d.Day))
        return true;
    else
        return false;
};
bool CDate::operator>(const CDate &d) const
{
    if ((Year > d.Year) || (Year == d.Year && Month > d.Month) || (Year == d.Year && Month == d.Month && Day > d.Day))
        return true;
    else
        return false;
};

bool CDate::operator>=(const CDate &d) const
{
    if ((Year > d.Year) || (Year == d.Year && Month > d.Month) || (Year == d.Year && Month == d.Month && Day >= d.Day))
        return true;
    else
        return false;
};

bool CDate::operator==(const CDate &d) const
{
    if (Year == d.Year && Month == d.Month && Day == d.Day)
        return true;
    else
        return false;
};

CDate CDate::operator+(CDate &d)
{
    CDate temp;
    temp.SetYear(Year + d.GetYear());
    temp.SetMonth(Month + d.GetMonth());
    temp.SetDay(Day + d.GetDay());
    return temp;
};

CDate CDate::operator-(CDate &d)
{
    CDate temp;
    temp.SetYear(Year - d.GetYear());
    temp.SetMonth(Month - d.GetMonth());
    temp.SetDay(Day - d.GetDay());
    return temp;
};

CDate CDate::operator++()
{
    Year++;
    Month++;
    Day++;
    return *this;
};
CDate CDate::operator--()
{
    Year--;
    Month--;
    Day--;
    return *this;
};

//Part:后置重载
CDate CDate::operator++(int)
{
    CDate temp = *this;
    Year++;
    Month++;
    Day++;
    return temp;
};

CDate CDate::operator--(int)
{
    CDate temp = *this;
    Year--;
    Month--;
    Day--;
    return temp;
};

//Part:CTime运算符重载
bool CTime::operator<(const CTime &t) const
{
    if ((Hour < t.Hour) || (Hour == t.Hour && Minute < t.Minute) || (Hour == t.Hour && Minute == t.Minute && Second < t.Second))
        return true;
    else
        return false;
};

bool CTime::operator<=(const CTime &t) const
{
    if ((Hour < t.Hour) || (Hour == t.Hour && Minute < t.Minute) || (Hour == t.Hour && Minute == t.Minute && Second <= t.Second))
        return true;
    else
        return false;
};

bool CTime::operator>(const CTime &t) const
{
    if ((Hour > t.Hour) || (Hour == t.Hour && Minute > t.Minute) || (Hour == t.Hour && Minute == t.Minute && Second > t.Second))
        return true;
    else
        return false;
};

bool CTime::operator>=(const CTime &t) const
{
    if ((Hour > t.Hour) || (Hour == t.Hour && Minute > t.Minute) || (Hour == t.Hour && Minute == t.Minute && Second >= t.Second))
        return true;
    else
        return false;
};

bool CTime::operator==(const CTime &t) const
{
    if (Hour == t.Hour && Minute == t.Minute && Second == t.Second)
        return true;
    else
        return false;
};

CTime CTime::operator+(CTime &t)
{
    CTime temp;
    temp.SetHour(Hour + t.GetHour());
    temp.SetMinute(Minute + t.GetMinute());
    temp.SetSecond(Second + t.GetSecond());
    return temp;
};

CTime CTime::operator-(CTime &t)
{
    CTime temp;
    temp.SetHour(Hour - t.GetHour());
    temp.SetMinute(Minute - t.GetMinute());
    temp.SetSecond(Second - t.GetSecond());
    return temp;
};

CTime CTime::operator++()
{
    Hour++;
    Minute++;
    Second++;
    return *this;
};
CTime CTime::operator--()
{
    Hour--;
    Minute--;
    Second--;
    return *this;
};

//Part:后置
CTime CTime::operator++(int)
{
    CTime temp = *this;
    Hour++;
    Minute++;
    Second++;
    return temp;
};
CTime CTime::operator--(int)
{
    CTime temp = *this;
    Hour--;
    Minute--;
    Second--;
    return temp;
};

//Part:CDateTime运算符重载
bool CDateTime::operator<(CDateTime &op)
{
    CDate *D_temp1 = this, *D_temp2 = &op;
    CTime *T_temp1 = this, *T_temp2 = &op;
    if ((*D_temp1 < *D_temp2) || ((*D_temp1 == *D_temp2) && (*T_temp1 < *T_temp2)))
        return true;
    else
        return false;
};

bool CDateTime::operator<=(CDateTime &op)
{
    CDate *D_temp1 = this, *D_temp2 = &op;
    CTime *T_temp1 = this, *T_temp2 = &op;
    if ((*D_temp1 < *D_temp2) || ((*D_temp1 == *D_temp2) && (*T_temp1 <= *T_temp2)))
        return true;
    else
        return false;
};

bool CDateTime::operator>(CDateTime &op)
{
    CDate *D_temp1 = this, *D_temp2 = &op;
    CTime *T_temp1 = this, *T_temp2 = &op;
    if ((*D_temp1 > *D_temp2) || ((*D_temp1 >= *D_temp2) && (*T_temp1 > *T_temp2)))
        return true;
    else
        return false;
};

bool CDateTime::operator>=(CDateTime &op)
{
    CDate *D_temp1 = this, *D_temp2 = &op;
    CTime *T_temp1 = this, *T_temp2 = &op;
    if ((*D_temp1 > *D_temp2) || ((*D_temp1 >= *D_temp2) && (*T_temp1 >= *T_temp2)))
        return true;
    else
        return false;
};

bool CDateTime::operator==(CDateTime &op)
{
    CDate *D_temp1 = this, *D_temp2 = &op;
    CTime *T_temp1 = this, *T_temp2 = &op;
    if ((*D_temp1 == *D_temp2) && (*T_temp1 == *T_temp2))
        return true;
    else
        return false;
};

bool CDateTime::operator!=(CDateTime &op)
{
    CDate *D_temp1 = this, *D_temp2 = &op;
    CTime *T_temp1 = this, *T_temp2 = &op;
    if ((*D_temp1 != *D_temp2) || (*T_temp1 != *T_temp2))
        return true;
    else
        return false;
}

CDateTime CDateTime::operator+(CDateTime &op)
{
    CDateTime temp;
    CDate *D_temp1 = this, *D_temp2 = &op, *D_temp = &temp;
    CTime *T_temp1 = this, *T_temp2 = &op, *T_temp = &temp;
    *T_temp = *T_temp1 + *T_temp2;
    *D_temp = *D_temp1 + *D_temp2;
    temp.DateTime_adj();
    return temp;
};

CDateTime CDateTime::operator-(CDateTime &op)
{
    CDateTime temp;
    CDate *D_temp1 = this, *D_temp2 = &op, *D_temp = &temp;
    CTime *T_temp1 = this, *T_temp2 = &op, *T_temp = &temp;
    *T_temp = *T_temp1 - *T_temp2;
    *D_temp = *D_temp1 - *D_temp2;
    temp.DateTime_adj();
    return temp;
};

CDateTime CDateTime::operator++()
{
    CDate *D_temp = this;
    CTime *T_temp = this;
    ++(*D_temp);
    ++(*T_temp);
    this->DateTime_adj();
    return *this;
};

CDateTime CDateTime::operator--()
{
    CDate *D_temp = this;
    CTime *T_temp = this;
    --(*D_temp);
    --(*T_temp);
    this->DateTime_adj();
    return *this;
};

//Part:后置
CDateTime CDateTime::operator++(int)
{
    CDateTime temp = *this;
    CDate *D_temp = this;
    CTime *T_temp = this;
    ++(*D_temp);
    ++(*T_temp);
    this->DateTime_adj();
    temp.DateTime_adj();
    return temp;
};

CDateTime CDateTime::operator--(int)
{
    CDateTime temp = *this;
    CDate *D_temp = this;
    CTime *T_temp = this;
    --(*D_temp);
    --(*T_temp);
    this->DateTime_adj();
    temp.DateTime_adj();
    return temp;
};

int main()
{
    //设定初始时间，与系统时间统一
    SYSTEMTIME sys;
    GetLocalTime(&sys);
    CDateTime cdt(sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond);
    cdt.ShowDT();

    CDate d;       CTime t;
    //重新设定当前日期时间
    char flag = 'y';
    cout << endl<< "是否需要重新设定当前日期和时间？（Y/N）";
    cin >> flag;
    while (tolower(flag) == 'y')
    {
        cdt.SetDT();
        cdt.ShowDT(); //测试用
        cout << "是否需要继续设定当前日期和时间？（Y/N）";
        cin >> flag;
    }
    cdt.ShowDT();

    //修改时区显示
    cout << endl<< "是否需要修改时区？(Y/N)";
    cin >> flag;
    while (tolower(flag) == 'y')
    {
        cdt.DT_trans();
        cdt.ShowDT(); //Test
        cout << "是否需要继续设定修改时区？（Y/N）";
        cin >> flag;
    }
    cdt.ShowDT();

    //日期时间加减
    cout << "*******日期时间加减*******" << endl;
    CDateTime op1(2019, 1, 13, 4, 25, 59), op2(2019, 1, 13, 4, 30, 59), result;
    ShowCurrent(op1, op2, result);

    //大小判断
    cout << "************大小判断************" << endl;
    cout << "op1 < op2?  " << ((op1 < op2) ? "true" : "false") << endl;
    cout << "op1 <= op2? " << ((op1 <= op2) ? "true" : "false") << endl;
    cout << "op1 > op2?  " << ((op1 > op2) ? "true" : "false") << endl;
    cout << "op1 >= op2? " << ((op1 >= op2) ? "true" : "false") << endl;
    cout << "op1 == op2? " << ((op1 == op2) ? "true" : "false") << endl;
    cout << "op1 != op2  " << ((op1 != op2) ? "true" : "false") << endl;
    cout << endl;

    //加减自增自减运算
    cout << "************加减自增自减测试************" << endl;
    cout << "TEST '+':" << endl;
    ShowCurrent(op1, op2, result);
    result = op1 + op2; //+
    cout << "result = op1 + op2 :" << endl;
    ShowCurrent(op1, op2, result);

    cout << endl
         << "TEST '-':" << endl;
    ShowCurrent(op1, op2, result);
    result = op1 - op2; //-
    cout << "result = op1 - op2 :" << endl;
    ShowCurrent(op1, op2, result);

    cout << endl
         << "TEST '++':" << endl;
    ShowCurrent(op1, op2, result);
    result = ++op1;
    cout << "result = ++op1 :" << endl;
    ShowCurrent(op1, op2, result); //++

    cout << endl
         << "TEST '--':" << endl;
    ShowCurrent(op1, op2, result);
    result = --op2;
    cout << "result = --op2 :" << endl;
    ShowCurrent(op1, op2, result); //--

    cout << endl
         << "TEST 后置'++':" << endl;
    ShowCurrent(op1, op2, result);
    result = op1++;
    cout << "result = op1++ :" << endl;
    ShowCurrent(op1, op2, result); //++后置

    cout << endl
         << "TEST 后置'--':" << endl;
    ShowCurrent(op1, op2, result);
    result = op2--;
    cout << "result = op2-- :" << endl;
    ShowCurrent(op1, op2, result); //--  后置
    return 0;
}

/**Thinking:
 * 1.此处求星期几是通过求年月日总天数，再基于1年1月1日是星期六的事实，除以7得余数求得星期数
 * 思考：上述可以改为通过基姆拉尔森算式、蔡勒（Zeller）公式，可以尝试写写两算法的程序
 * 2.程序过于赘余
 * 思考：a.写头文件 b.有没有通用的程序算式可以写到第三个类封装
 * 3.代码是否存在优化空间？咋优化？
 * 4.shift+alt+f可以调整代码
*/