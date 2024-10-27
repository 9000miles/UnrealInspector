namespace DETAILS_VIEWER
{
#define IMPLEMENT_ITYPENAME(ClassName) \
public: \
	FString GetTypeName() override { return ClassName::TypeName(); } \
	static FString TypeName() { return TEXT(#ClassName); }

	class ITypeName
	{
	public:
		virtual FString GetTypeName() = 0;
		virtual void Initalized() {}

		template<typename T>
		T CreateInstance() {}
	};
}