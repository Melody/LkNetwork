#ifndef LK_MATH_CLASSES_
#define LK_MATH_CLASSES_
namespace LK
{
	/**
	@brief Math命名空间包含数学计算相关内容*/
	namespace Math
	{
		/**
		@brief 数学异常*/
		struct MathException;
		/**
		@brief Float256类表示一个256位浮点数（实体）*/
		struct Float256;
		/**
		@brief Int表示一个大数整形实体*/
		struct Int;
		/**
		@brief 表示一个整形大数（引用模拟）*/
		typedef ptr<Int> intx;
		/**
		@brief 表示一个N*64位浮点数*/
		template<size_t N>struct FloatN;
		/**
		@brief Float表示一个浮点型大数*/
		struct Float;
		/**
		@brief 表示一个浮点大数（引用模拟）*/
		typedef ptr<Float> floatx;
		/**
		@brief LDouble是double类型的扩展，精度与double相同，但是表示的数字范围将得到扩展*/
		struct LDouble;
		/**
		@brief DDouble是double类型的扩展，精度与double相同，但是表示的数字范围将得到极大扩展*/
		struct DDouble;
		/**
		@brief 表示一个复数（double成员）*/
		struct DoubleComplex;
		/**
		@brief 表示一个复数（Float128成员）*/
		struct Complex128;
		/**
		@brief 表示一个运算符代理*/
		struct POperatorObjects;
		/**
		@brief 运算符对象接口*/
		template<class T>struct IOperatorObject;
		/**
		@brief 运算符对象代理*/
		template<class T>struct POperatorObject;
		/**
		@brief 算式解析节点接口*/
		struct ICalculationNodes;
		/**
		@brief 表示一个算式解析对象的代理*/
		template<class T>class PCalculation;
		/**
		@brief 表示具体数字类型的解析节点*/
		template<class T>struct PCalculationNode;
		/**
		@brief 表示一个运算符表代理*/
		struct POperatorTables;
		/**
		@brief 表示一个运算符表接口*/
		struct IOperatorTables;
		/**
		@brief 算式接口代理*/
		struct PCalculations;
		/**
		@brief 动态算式运算符接口*/
		template<class T>class IDynamicFunctionOperator;
		/**
		@brief 动态算式运算符接口代理*/
		template<class T>struct PDynamicFunctionOperator;

		template<class T>struct OperatorTable;
		template<class T>struct POperatorTable;
	}
}
#endif