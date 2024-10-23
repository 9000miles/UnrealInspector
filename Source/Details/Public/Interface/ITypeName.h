namespace DETAILS_VIEWER
{
	class ITypeName
	{
	public:
		virtual FString GetTypeName() = 0;
		virtual void Initalized() {}

		template<typename T>
		T CreateInstance() {}
	};
}