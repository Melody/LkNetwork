#include "stdafx.h"
#ifdef LK_MATH_CLASSES_
namespace LK
{
	namespace Math
	{
		const char* MathError::description[] =
		{
			"未知",
			"未知异常",
			"运行加法时发生指数溢出",
			"运行减法时发生指数溢出",
			"运行乘法时发生指数溢出",
			"运行除法时发生指数溢出",
			"运行除法时发生除以0错误",
			"运行求余时发生除以0错误",
			"求e的x次幂时发生指数溢出",
			"求2的x次幂时发生指数溢出",
			"求arcsin时发生输入数据错误",
			"求arccos时发生输入数据错误",
			"求Tan时发生被0除错误",
			"求Cot时发生被0除错误",
			"求对数时发生真数<=0错误",
			"求对数时发生真数<=0或底数<=0错误",
			"求对数时发生底数=1错误",
			"求开平方时发生被开方数小于0错误",
			"求x次幂时发生底数小于等于0且指数小于0错误",
			"求x次幂时发生指数溢出",
			"求x次幂时发生0的0次方错误",
			"从字符串初始化时发生指数溢出错误",
			"左移操作发生溢出",
			"计算过程被终止"
		};
		//使用一个错误码来生成异常，一般errorCode<-1
		MathException::MathException(int errorCode) 
			:std::logic_error(errorCode>0?"错误码大于0":
			-errorCode>=(24)?"未知的异常"
				:MathError::description[-errorCode]){
		}
		//使用一个错误描述字符串来生成异常
		MathException::MathException(std::string const& str)
			:std:: logic_error(str) {
		}

	}
}
#endif