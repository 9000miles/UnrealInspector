namespace DETAILS_VIEWER
{
	class IJsonable
	{
	public:
		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject)
		{
		}

		void FromJsonString(const FString& JsonString)
		{
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
			TSharedPtr<FJsonObject> JsonObject;
			if (FJsonSerializer::Deserialize(Reader, JsonObject))
				FromJson(JsonObject);
		}
		virtual TSharedPtr<FJsonObject> ToJson()
		{
			return MakeShared<FJsonObject>();
		}

		FString ToJsonString()
		{
			TSharedPtr<FJsonObject> JsonObject = ToJson();
			FString JsonString;
			TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
			FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
			return JsonString;
		}
	};
}