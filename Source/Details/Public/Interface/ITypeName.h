namespace DETAILS_VIEWER
{
	class ITypeName
	{
	public:
		virtual FString GetTypeName() = 0;

		template<typename T>
		T CreateInstance() {}
	};
}