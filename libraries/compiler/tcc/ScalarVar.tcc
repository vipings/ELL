#include <string>

namespace emll
{
	namespace compiler
	{
		template<typename T>
		ScalarVar<T>::ScalarVar(const VariableScope scope, int flags)
			: Variable(GetValueType<T>(), scope, flags)
		{
		}

		template<typename T>
		LiteralVar<T>::LiteralVar(T data)
			: ScalarVar(VariableScope::Literal, VariableFlags::none), 
			 _data(data)
		{
		}

		template<typename T>
		InitializedScalarVar<T>::InitializedScalarVar(const VariableScope scope, T data, bool isMutable)
			: ScalarVar(scope, isMutable ? (VariableFlags::isMutable | VariableFlags::hasInitValue) : VariableFlags::hasInitValue),
			 _data(data)
		{
		}

		template<typename T>
		ComputedVar<T>::ComputedVar()
			: Variable(GetValueType<T>(), VariableScope::Local)
		{
		}

		template<typename T>
		VectorElementVar<T>::VectorElementVar(Variable& src, int offset)
			: ScalarVar(VariableScope::Local, VariableFlags::isVectorRef),
			_src(src),
			_offset(offset)	
		{
		}

		template<typename T>
		bool ComputedVar<T>::Combine(ComputedVar& other)
		{
			if (other.Type() != Type() ||
				other._op != _op)
			{
				return false;
			}

			switch (_op)
			{
				case OperatorType::Add:
				case OperatorType::AddF:
				case OperatorType::Subtract:
				case OperatorType::SubtractF:
					_value += other._value;
					break;
				case OperatorType::Multiply:
				case OperatorType::MultiplyF:
				case OperatorType::DivideF:
					_value *= other._value;
					break;
				default:
					throw new CompilerException(CompilerError::notSupported);
			}

			return true;
		}
	}
}