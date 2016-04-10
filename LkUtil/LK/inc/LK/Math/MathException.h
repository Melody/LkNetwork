#pragma once
/**@authour W意波 编写*/
namespace LK
{
	namespace Math
	{
		/**
		@brief 包含数学计算错误的枚举*/
		struct MathError
		{
			/**
			@brief 加法运算过程发生指数上限溢出*/
			static const int PlusExpOverflow = -2;
			/**
			@brief 减法运算过程发生指数上限溢出*/
			static const int MinusExpOverflow = -3;
			/**
			@brief 乘法运算过程发生指数上限溢出*/
			static const int MultiplyExpOverflow = -4;
			/**
			@brief 除法运算过程发生指数上限溢出*/
			static const int DivideExpOverflow = -5;
			/**
			@brief 除以0错误*/
			static const int Divide0 = -6;
			/**
			@brief 求余运算除以0错误*/
			static const int ModDivide0 = -7;
			/**
			@brief 计算e的x次方发生上限溢出*/
			static const int ExpEOverflow = -8;
			/**
			@brief 计算2的x次方发生上限溢出*/
			static const int Exp2Overflow = -9;
			/**
			@brief 计算Arcsin时发生超出范围错误*/
			static const int Arcsin = -10;
			/**
			@brief 计算Arccos时发生超出范围错误*/
			static const int Arccos = -11;
			/**
			@brief 计算Tan时发生越限错误*/
			static const int Tan = -12;
			/**
			@brief 计算Cot时发生越限错误*/
			static const int Cot = -13;
			/**
			@brief 计算Ln时发生真数小于等于0错误*/
			static const int LnLessOrEqual0 = -14;
			/**
			@brief 计算log时发生真数或底数小于等于0错误*/
			static const int LogLessOrEqual0 = -15;
			/**
			@brief 计算log时发生底数为1错误*/
			static const int Log1x = -16;
			/**
			@brief 计算Sqrt时发生被开方数小于0错误*/
			static const int SqrtLessThan0 = -17;
			/**
			@brief 计算Pow时发生底数小于等于0且指数小于0错误*/
			static const int PowLessThan0 = -18;
			/**
			@brief 计算Pow时发生指数上限溢出错误*/
			static const int PowExpOverflow = -19;
			/**
			@brief 计算Pow时发生0^0错误*/
			static const int Pow00 = -20;
			/**
			@brief 从字符串输入时发生上限溢出错误*/
			static const int InputOverflow = -21;
			/**
			@brief 左移发生溢出*/
			static const int MoveLeftOverflow = -22;
			/**
			@brief 计算过程终止*/
			static const int CalculationStop = -23;
			/**
			@brief 对应错误描述字符串*/
			static char const *description[];
		};
		/**
		@brief 数学计算异常类*/
		struct MathException :std::logic_error {
			//使用一个错误码来生成异常，一般errorCode<-1
			MathException(int errorCode);
			//使用一个错误描述字符串来生成异常
			MathException(std::string const& str);
		};
	}
}
